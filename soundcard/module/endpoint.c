/*
 * endpoint.c
 *
 * Created: 23.03.2015 15:10:29
 *  Author: ale
 */ 

#include "sam.h"
#include "system.h"
#include "udp.h"
#include "udp_request.h"
#include "error_code.h"
#include <string.h>

void ep_init(udp_ep_t *ep, uint8_t type, uint8_t size, uint8_t number) {
	memset(ep, 0x00, sizeof(&ep));

	ep->type = type;
	ep->size = size;
	ep->number = number;
	
	ep->state = EP_STATE_IDLE;
	ep->callback = NULL;
	
	/* Buffer ? */

	ep->CSR = &(UDP->UDP_CSR[ep->number]);
	ep->FDR = &(UDP->UDP_FDR[ep->number]);
		
	switch(type) {
		case UDP_EP_TYPE_CONTROL:
			ep_control_set(ep, UDP_CSR_EPTYPE_CTRL);
			break;
		
		case UDP_EP_TYPE_ISO_IN:
			ep_control_set(ep, UDP_CSR_EPTYPE_ISO_IN);
			ep_control_set(ep, UDP_CSR_DIR);
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
	for(int i = 0; i < 20; i++) nop();
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

void ep_callback(udp_ep_t *ep) {
	if(*ep->CSR & UDP_CSR_RXSETUP) {
		udp_setup_data_t request;
		for(uint8_t i = 0; i < 8; i++) *((uint8_t *) &request + i) = (uint8_t) UDP->UDP_FDR[0];
		
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
//		else if(ep->state == EP_STATE_TRANS) {
//			if(udp_push(ep) == LAST_TRUNK) ep->state = EP_STATE_IDLE;
//		}

		if(ep->number == 4) {
			*ep->CSR &= ~UDP_CSR_TXCOMP;
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
