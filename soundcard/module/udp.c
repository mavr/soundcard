/*
 * udp.c
 *
 * Created: 11.03.2015 14:29:24
 *  Author: ale
 */ 

#include "sam.h"
#include "udp.h"
#include "udp_request.h"
#include "error_code.h"
#include "system.h"
#include <string.h>

void udp_system() {
// make power for udp possible.
	PMC->PMC_PCER1 |= 1 << (ID_UDP - 32); // UDP id = 34
	PMC->PMC_SCER |= PMC_SCER_UDP;
	
// interrupt enable.
	NVIC_EnableIRQ(UDP_IRQn);
}

void udp_set_interrupt() {
	UDP->UDP_IER = UDP_IER_SOFINT | UDP_ISR_RXSUSP;
}

void udp_ddp_pull_up() {
	UDP->UDP_TXVC =	UDP_TXVC_PUON;
}

/* Endpoint */
void ep_init(udp_ep_t *ep, uint8_t type, uint8_t size, uint8_t number) {
	memset(ep, 0x00, sizeof(&ep));

	ep->type = type;
	ep->size = size;
	ep->number = number;
	
//	ep->state = EP_STATE_IDLE;
//	ep->callback = NULL;
	
/* Buffer ? */	

	switch(type) {
		case UDP_EP_TYPE_CONTROL:
			ep->CSR = &(UDP->UDP_CSR[0]);
			ep->FDR = &(UDP->UDP_FDR[0]);
//			ep->rx_buffer = ep0_rx_buffer;
//			ep->tx_buffer = ep0_tx_buffer;
			//ep->rx_buffer_size = 0;
			//ep->tx_size = 0;
			//ep->rx_bytes_ready = 0;
			//ep->tx_count = 0;
			break;
			
		case UDP_EP_TYPE_BULK:
			break;
			
		case UDP_EP_TYPE_INT:
			break;
	}
}

void ep_reset(udp_ep_t *ep, uint8_t ep_number, uint8_t ep_type, uint8_t ep_size) {
	ep_init(ep, ep_type, ep_size, ep_number);
	
	UDP->UDP_RST_EP |= (1 << ep_number);
	UDP->UDP_RST_EP &= ~(1 << ep_number);
	
	ep_set_interrupt(ep);
	ep_enable(ep);
}

void ep_control_set(udp_ep_t *ep, uint32_t mask) {
	UDP->UDP_CSR[ep->number] |= mask;
//	for(int i = 0; i < 1040; i++) nop();
//	*ep->CSR = *ep->CSR | mask;
//	uint32_t tmp = UDP->UDP_CSR[ep->number];
	nop();
//	while((*ep->CSR & mask) == 0);
}

void ep_control_clr(udp_ep_t *ep, uint32_t mask) {
	UDP->UDP_CSR[ep->number] &= ~mask;
//	*ep->CSR = *ep->CSR & (~mask);
//	while((*ep->CSR & mask) != 0);
}

void ep_enable(udp_ep_t *ep) {
	ep_control_set(ep, UDP_CSR_EPEDS);
}

void ep_disable(udp_ep_t *ep) {
	ep_control_clr(ep, UDP_CSR_EPEDS);
}

void ep_set_interrupt(udp_ep_t *ep) {
	UDP->UDP_IER |= (1 << ep->number);
}

inline void udp_fifo_push(udp_ep_t *ep, uint8_t value) {
	UDP->UDP_FDR[0] = value;
}

int udp_push(udp_ep_t *ep) {
	if(ep->tx_size == ep->tx_count) return LAST_TRUNK;

	while(*ep->CSR & UDP_CSR_TXPKTRDY);
	
	uint32_t size = (ep->size < (ep->tx_size - ep->tx_count)) ? ep->size : (ep->tx_size - ep->tx_count);

// TODO: fix me
//	for(uint32_t i = 0; i < size; i++) ep->FDR = *(ep->tx_buffer + ep->tx_count + i);
//	for(uint32_t i = 0; i < size; i++) UDP->UDP_FDR[0] = *(ep->tx_buffer + ep->tx_count + i);
	uint32_t i;
	uint8_t tmp;
	for(i = 0; i < size; i++) {
		tmp =  (uint8_t) *(ep->tx_buffer + ep->tx_count + i);
		debug_arr[i] = tmp;
		UDP->UDP_FDR[0] = tmp; //(uint8_t) *(udp_dev_descriptor + ep->tx_count + i);
	}
	ep->tx_count += size;
	
	ep_control_set(ep, UDP_CSR_TXPKTRDY);
	
	return SUCCESS;	
}

int udp_send(udp_ep_t *ep, uint8_t *data, uint32_t size) {
//	if(ep->state != EP_STATE_IDLE) return ERRBUSY;
//	ep->state = EP_STATE_TRANS;
	
//	if(size > UDP_EP0_TX_BUFFER_SIZE) return ERRMEMOVER;

	ep->tx_buffer = data;
	ep->tx_size = size;
	ep->tx_count = 0;
	
	udp_push(ep);
	return SUCCESS;
}

int udp_send_zlp(udp_ep_t *ep) {
//	if(ep->state != EP_STATE_IDLE) return ERRBUSY;
	while(*ep->CSR & UDP_CSR_TXPKTRDY);
	ep_control_set(ep, UDP_CSR_TXPKTRDY);		
}

int udp_send_stall(udp_ep_t *ep) {
	*ep->CSR |= UDP_CSR_FORCESTALL;
}

void udp_get_descriptor(uint16_t wValue, uint16_t wIndex, uint16_t wLength) {
	static count = 0;
	count++;
	
	uint8_t *_p_desc = 0;
	uint32_t _s_desc = 0;
		
	// Type of descriptor is contains in high byte. Low byte contains index of descriptor.
	// In current version of software index is not use.
	switch(wValue >> 8) {
		case UDP_wValue_DT_DEV	:		_p_desc = udp_dev_descriptor; _s_desc = *(_p_desc); break;
		case UDP_wValue_DT_CONF :		_p_desc = udp_conf_descriptor; _s_desc = *(_p_desc + 2); break; 
//		case UDP_wValue_DT_STR	:		break;
//		case UDP_wValue_DT_INT	:		_p_desc = udp_int_descriptor; break;
//		case UDP_wValue_DT_EP	:		_p_desc = udp_ep0_descriptor; break;
		
		/* UDP_wValue_DT_DEV_QUAL , UDP_wValue_DT_INT_POWER */
		default: 
			udp_send_stall(&ep_control);
		break;
	}

	if(_s_desc > wLength) _s_desc = wLength;
	udp_send(&ep_control, _p_desc, _s_desc);
}

void udp_set_address(uint16_t wValue) {
	udp_send_zlp(&ep_control);
	
	ep_control.wValue = wValue;
	ep_control.callback = &_udp_set_address_callback;
}

void udp_set_configuration(uint16_t wValue) {
//	ep_control.callback = &_udp_set_configuration_callback;
}

void _udp_set_address_callback() {
	UDP->UDP_GLB_STAT = UDP_GLB_STAT_FADDEN;
	UDP->UDP_FADDR |= (ep_control.wValue & 0x7f) | UDP_FADDR_FEN;
}

void udp_enumerate(const udp_setup_data_t *request) {
	
	ep_control.state = EP_STATE_SETUP;
				
	/* bmRequestType: type */
	switch((request->bmRequestType & 0x60) >> 5) {
		case UDP_bmRequestType_Type_STANDART :
			
			switch(request->bRequest) {
				case UDP_bRequest_GET_DESCRIPTOR :
					udp_get_descriptor(request->wValue, request->wIndex, request->wLength);
					break;

				case UDP_bRequest_SET_CONFIGURATION :
					/* If wIndex, wLength, or the upper byte of wValue is non-zero, 
					then the behavior of this request is not specified. */
					if((request->wLength == 0) || (request->wIndex == 0) || ((request->wValue & 0xff00) == 0x00)) break;
					udp_set_configuration(request->wValue);
					break;

				case UDP_bRequest_GET_INTERFACE :
					break;

				case UDP_bRequest_GET_STATUS :
					break;
				
				case UDP_bRequest_SET_ADDRESS :	udp_set_address(request->wValue); break;
				
				default: ep_control.state = EP_STATE_IDLE; break;
			}
			
	}
}

void ep_callback(udp_ep_t *ep) {
	if(*ep->CSR & UDP_CSR_RXSETUP) {
		udp_setup_data_t request;
		for(uint8_t i = 0; i < 8; i++)
		//			*((uint8_t *) &request + i) = (uint8_t) *ep->FDR & 0xffffff00;
		*((uint8_t *) &request + i) = (uint8_t) UDP->UDP_FDR[0];
		
		if(request.bmRequestType & 0x80) *ep->CSR |= UDP_CSR_DIR;
		*ep->CSR &= ~UDP_CSR_RXSETUP;
		udp_enumerate(&request);
		
		return;
	}
	
	if(*ep->CSR & UDP_CSR_TXCOMP) {
		
		if(ep->state == EP_STATE_SETUP) {
			if(ep->callback != NULL) ep->callback();
			ep->callback = NULL;
			ep->state = EP_STATE_IDLE;
		}
		else if(ep->state == EP_STATE_TRANS) {
			if(udp_push(ep) == LAST_TRUNK) ep->state = EP_STATE_IDLE;			
		}

		*ep->CSR &= ~UDP_CSR_TXCOMP;
	}
	
	if(*ep->CSR & UDP_CSR_STALLSENT) {
		*ep->CSR &= ~UDP_CSR_STALLSENT;
	}
	
	if(*ep->CSR & (UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1)) {
		// read and clear udp_csr_rx_data_bkx
	}
}