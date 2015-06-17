/*
 * endpoint.c
 *
 * Created: 23.03.2015 15:10:29
 *  Author: ale
 */ 

#include "sam.h"
#include "include/system.h"
#include "udp.h"
#include "udp/udp_request.h"
#include "include/error_code.h"
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
			
//			udp_stream_init(&(__ep_control->stream), __ep_ctrl_buffer, EP_CTRL_BUFFER_SIZE);
			break;
			
		case UDP_EP_TYPE_ISO_IN: 
		case UDP_EP_TYPE_ISO_OUT: ;
			udp_ep_audio_t *__ep_audio = (udp_ep_audio_t *) ep;
			memset(ep, 0x00, sizeof(&__ep_audio));
			__core = &(__ep_audio->ep);
			
			//TODO: initializing stream
			//__ep->stream
			
			if(__core->type == UDP_EP_TYPE_ISO_IN) {
				__ep_ctrl_set(&(__ep_audio->ep), UDP_CSR_EPTYPE_ISO_IN);
				udp_stream_init(&(__ep_audio->stream), __ep_audio_in, EP_AUDIO_BUFFER_SIZE);
			}
			else {
				 __ep_ctrl_set(&(__ep_audio->ep), UDP_CSR_EPTYPE_ISO_OUT);
				 udp_stream_init(&(__ep_audio->stream), __ep_audio_out, EP_AUDIO_BUFFER_SIZE);
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
	UDP->UDP_CSR[ep->number] |= mask;
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
	if(*ep->ep.CSR & UDP_CSR_RXSETUP) {
		udp_setup_data_t request = ep_get_setup_pkg(ep);
		if(request.bmRequestType & 0x80) *ep->ep.CSR |= UDP_CSR_DIR;
		*ep->ep.CSR &= ~UDP_CSR_RXSETUP;
		
		udp_enumerate(&request);
	}
	
	if(*ep->ep.CSR & UDP_CSR_TXCOMP) {
		if(udp_push(ep) == EP_STATE_IDLE) {
			// in case of endpoint requires additional actions run callback() for this ep.
			if(ep->callback != NULL) ep->callback();
		
			ep->callback = NULL;
			ep->ep.state = EP_STATE_IDLE;
		}
	}
	
	if(*ep->ep.CSR & UDP_CSR_STALLSENT) {
		*ep->ep.CSR &= ~UDP_CSR_STALLSENT;
	}
	
	if(*ep->ep.CSR & (UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1)) {
		*ep->ep.CSR &= ~(UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1);
	}
	
	
}

void ep_callback(udp_ep_audio_t *ep) {	
	if(*ep->ep.CSR & UDP_CSR_RXSETUP) {
		// this case should never happen
		*ep->ep.CSR &= ~UDP_CSR_RXSETUP;		
		return;
	}
	
	if(*ep->ep.CSR & UDP_CSR_TXCOMP) {		
		if(ep->ep.number == UDP_EP_IN) {
			//TODO: need to read from streaming and send to host
			//for(int i = 0; i < ep->tx_count; i++)
				//UDP->UDP_FDR[4] = *(ep->tx_buffer + i);
			//ep->tx_count = 0;
			
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
		if(ep->ep.number == UDP_EP_OUT) {
			
		}
		*ep->ep.CSR &= ~(UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1);
	}
}