/*
 * endpoint.c
 *
 * Created: 23.03.2015 15:10:29
 *  Author: ale
 */ 

#include "sam.h"
#include "include/system.h"
#include "include/error_code.h"
#include "core/syslog.h"
#include "include/udp.h"
#include "udp/udp_request.h"
#include "udp/usb.h"
#include <string.h>

void ep_init(void *ep, uint8_t ep_type, uint16_t ep_size, uint8_t ep_number) {
	//TODO: don't forgiven about the situation with UDP_CSR_TXPKTRDY flag for bulk ep
	udp_ep_core_t *__core; 
	
	switch(ep_type) {
		case UDP_EP_TYPE_CONTROL : ;
			udp_ep_setup_t *__ep_control = (udp_ep_setup_t *) ep;
			memset(ep, 0x00, sizeof(&__ep_control));
			__core = &(__ep_control->ep);
			
			__ep_control->callback = NULL;
			
			__ep_ctrl_set(&(__ep_control->ep), UDP_CSR_EPTYPE_CTRL);
			__UDP_DEBUG(LOG_LVL_HIGH, "Endpoint: init control");
			
//			udp_stream_init(&(__ep_control->stream), __ep_ctrl_buffer, EP_CTRL_BUFFER_SIZE);
			break;
		
		case UDP_EP_TYPE_INT: ;
			udp_ep_hid_report_t *__ep_hid = (udp_ep_hid_report_t *) ep;
			memset(ep, 0x00, sizeof(&__ep_hid));
			__core = &(__ep_hid->ep);
		
			__ep_ctrl_set(&(__ep_hid->ep), UDP_CSR_EPTYPE_INT_IN);
			__UDP_DEBUG(LOG_LVL_HIGH, "Endpoint: init hid interrupt");
			__ep_ctrl_set(&__ep_hid->ep, UDP_CSR_TXPKTRDY);
			break;
					
		case UDP_EP_TYPE_ISO_IN: 
		case UDP_EP_TYPE_ISO_OUT: ;
			udp_ep_audio_t *__ep_audio = (udp_ep_audio_t *) ep;
			memset(ep, 0x00, sizeof(&__ep_audio));
			__core = &(__ep_audio->ep);
			
			//TODO: initializing stream
			//__ep->stream
			
			if(ep_type == UDP_EP_TYPE_ISO_IN) {
				__ep_ctrl_set(&(__ep_audio->ep), UDP_CSR_EPTYPE_ISO_IN);
				stream_init(&(__ep_audio->stream), __ep_audio_in, EP_AUDIO_BUFFER_SIZE);
				__UDP_DEBUG(LOG_LVL_HIGH, "Endpoint: init isochronous in");
			}
			else {
				 __ep_ctrl_set(&(__ep_audio->ep), UDP_CSR_EPTYPE_ISO_OUT);
				 stream_init(&(__ep_audio->stream), __ep_audio_out, EP_AUDIO_BUFFER_SIZE);
				 __UDP_DEBUG(LOG_LVL_HIGH, "Endpoint: init isochronous out");
			}
			
			break;
			
		default: return;
	}
	
	__core->type = ep_type;
	__core->number = ep_number;
	__core->size = ep_size;

	__core->state = EP_STATE_IDLE;
		
	__core->CSR = &(UDP->UDP_CSR[ep_number]);
	__core->FDR = &(UDP->UDP_FDR[ep_number]);
	
}

void ep_reset(void *ep, uint8_t ep_number, uint8_t ep_type, uint16_t ep_size) {
	ep_init(ep, ep_type, ep_size, ep_number);
	
	// hardware reset 
	UDP->UDP_RST_EP |= (1 << ep_number);
	UDP->UDP_RST_EP &= ~(1 << ep_number);
	
	ep_set_interrupt(ep);
	ep_enable(ep);
}

void ep_set_interrupt(udp_ep_core_t *ep) {
	UDP->UDP_IER |= (1 << ep->number);
}

void __ep_ctrl_set(udp_ep_core_t *ep, uint32_t mask) {
	*ep->CSR |= mask;
	for(int i = 0; i < 20; i++) nop();
}

void __ep_ctrl_clr(udp_ep_core_t *ep, uint32_t mask) {
	UDP->UDP_CSR[ep->number] &= ~mask;
	//	*ep->CSR = *ep->CSR & (~mask);
	//	while((*ep->CSR & mask) != 0);
}

void ep_enable(udp_ep_core_t *ep) {
	__ep_ctrl_set(ep, UDP_CSR_EPEDS);
}

void ep_disable(udp_ep_core_t *ep) {
	__ep_ctrl_clr(ep, UDP_CSR_EPEDS);
}

udp_setup_data_t ep_get_setup_pkg(udp_ep_setup_t *ep) {
	udp_setup_data_t request;
	for(uint8_t i = 0; i < 8; i++) *((uint8_t *) &request + i) = (uint8_t) *ep->ep.FDR;
	
	return request;
}


void ep_callback_setup(udp_ep_setup_t *ep) {
	/* Our data delivered */
	if(*ep->ep.CSR & UDP_CSR_TXCOMP) {
		switch(ep->ep.state) {
			case EP_STATE_TRANS :
				// Send data or complete transaction by calling callback() function.
				if(udp_push(ep) == EP_STATE_IDLE) {
					if(ep->callback != NULL) ep->callback();
					ep->callback = NULL;
					ep->ep.state = EP_STATE_IDLE;
				}
				break;
			
			case EP_STATE_ZLP :
				if(ep->callback != NULL) ep->callback();
				ep->callback = NULL;
				ep->ep.state = EP_STATE_IDLE;
				break;
			
			default:
				__UDP_DEBUG(LOG_LVL_LOW, "Error! EP receive TXCOMP ir but has not trans or zlp state");
				__UDP_ERROR;
				break;
		}
		
		__ep_ctrl_clr(&ep->ep, UDP_CSR_TXCOMP);
	}

/* Setup package comes to us. */
	if(*ep->ep.CSR & UDP_CSR_RXSETUP) {
		// Get setup package.
		udp_setup_data_t request = ep_get_setup_pkg(ep);
		if(request.bmRequestType & 0x80) *ep->ep.CSR |= UDP_CSR_DIR;
		*ep->ep.CSR &= ~UDP_CSR_RXSETUP;
		
		// Processing setup package.
		udp_enumerate(&request);
		
		//#ifdef UART_DEBUG
			//// At this moment ep can has just 
			//switch(ep->ep.state) {
				//case EP_STATE_IDLE	: __UDP_DEBUG(LOG_LVL_MED, "URB : Control EP state = IDLE"); break;
				//case EP_STATE_NONE	: __UDP_DEBUG(LOG_LVL_MED, "URB : Control EP state = NONE"); break;
				//case EP_STATE_TRANS : __UDP_DEBUG(LOG_LVL_MED, "URB : Control EP state = TRANS"); break;
				//case EP_STATE_SETUP : __UDP_DEBUG(LOG_LVL_MED, "URB : Control EP state = SETUP"); break;
				//case EP_STATE_ZLP	: __UDP_DEBUG(LOG_LVL_MED, "URB : Control EP state = ZLP"); break;
				//case EP_STATE_STALL : __UDP_DEBUG(LOG_LVL_MED, "URB : Control EP state = STALL"); break;
				//default : __UDP_DEBUG(LOG_LVL_MED, "URB : Control EP state = uknown."); break;
			//}
		//#endif
	
	}
	
/* Our mistake delivered */
	if(*ep->ep.CSR & UDP_CSR_STALLSENT) {
		__ep_ctrl_clr(&ep->ep, UDP_CSR_FORCESTALL);
		__ep_ctrl_clr(&ep->ep, UDP_CSR_STALLSENT);
		
		ep->ep.state = EP_STATE_IDLE;
	}
	
/* We get data or just delivery report. */
	if(*ep->ep.CSR & (UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1)) {
		*ep->ep.CSR &= ~(UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1);
	}
}

void ep_callback_hid(udp_ep_hid_report_t *ep) {
	if(*ep->ep.CSR & UDP_CSR_TXCOMP) {
		__ep_ctrl_clr(&(ep->ep), UDP_CSR_TXCOMP);
		__ep_ctrl_clr(&(ep->ep), UDP_CSR_TXPKTRDY);
	}
	
}

void ep_callback(udp_ep_audio_t *ep) {	
	if(*ep->ep.CSR & UDP_CSR_RXSETUP) {
		__UDP_DEBUG(LOG_LVL_HIGH, "Error! ep_callback() : isr rxsetup. this time which cant happen.");
		// this case should never happen
		*ep->ep.CSR &= ~UDP_CSR_RXSETUP;		
		return;
	}
	
	if(*ep->ep.CSR & UDP_CSR_TXCOMP) {		
		if(ep->ep.number == UDP_EP_IN) {
			uint32_t i = 0;
			uint16_t value;
			if(udp_stream_get_avalable_data_size(&ep->stream) == 0) {
				__UDP_DEBUG(LOG_LVL_HIGH, "no available data in stream");
			}
			for(i = 0; i < udp_stream_get_avalable_data_size(&ep->stream); i++) {
				value = stream_get(&ep->stream);
				*ep_in.ep.FDR = value & 0x00ff;
				*ep_in.ep.FDR = (value & 0xff00) >> 8;
			}
			
			__ep_ctrl_set(&(ep->ep), UDP_CSR_TXPKTRDY);
			__ep_ctrl_clr(&(ep->ep), UDP_CSR_TXCOMP);
			return;
		}

//		*ep->CSR &= ~UDP_CSR_TXCOMP;
		
	}
	
	if(*ep->ep.CSR & UDP_CSR_STALLSENT) {
		*ep->ep.CSR &= ~UDP_CSR_STALLSENT;
	}
	
	if(*ep->ep.CSR & (UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1)) {
		// read and clear udp_csr_rx_data_bkx
		uint16_t u16 = 0;
		uint32_t i;
		if(ep->ep.number == UDP_EP_OUT) {
			for(i = 0; i < (*(ep->ep.CSR) & 0x3ff0000) >> 16 ; i += 2) {
				u16 = *ep->ep.FDR;
				u16 |= (*ep->ep.FDR << 8);
				
				stream_put(&ep->stream, u16);
			}
		}
		*ep->ep.CSR &= ~(UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1);
	}
}
