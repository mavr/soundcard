/*
 * udp.c
 *
 * Created: 11.03.2015 14:29:24
 *  Author: ale
 */ 

#include "sam.h"
#include "udp.h"
#include "udp_request.h"
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
	
/* Buffer ? */	

	switch(type) {
		case UDP_EP_TYPE_CONTROL:
			ep->CSR = &(UDP->UDP_CSR[0]);
			ep->FDR = &(UDP->UDP_FDR[0]);
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

void ep_enable(udp_ep_t *ep) {
	UDP->UDP_CSR[ep->number] |= UDP_CSR_EPEDS | UDP_CSR_DIR;
}

void ep_disable(udp_ep_t *ep) {
	UDP->UDP_CSR[ep->number] &= ~UDP_CSR_EPEDS;
}

void ep_set_interrupt(udp_ep_t *ep) {
	UDP->UDP_IER |= (1 << ep->number);
}

void ep_callback(udp_ep_t *ep) {
	if(*ep->CSR & UDP_CSR_RXSETUP) {
		volatile udp_setup_data_t request;
		for(uint8_t i = 0; i < 8; i++) 
//			*((uint8_t *) &request + i) = (uint8_t) *ep->FDR & 0xffffff00;
			*((uint8_t *) &request + i) = (uint8_t) UDP->UDP_FDR[0];
		
// TODO: wtf?
		if(request.bmRequestType & 0x80) *ep->CSR |= UDP_CSR_DIR;
		*ep->CSR &= ~UDP_CSR_RXSETUP;
		udp_enumerate(&request);
		
		return;
	}
	
	if(*ep->CSR & UDP_CSR_TXCOMP) {
		// write data to fifo
		*ep->CSR &= ~UDP_CSR_TXCOMP;
	}
	
	if(*ep->CSR & (UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1)) {
		// read and clear udp_csr_rx_data_bkx
	}
}

void udp_push(udp_ep_t *ep, uint8_t *data, uint32_t size) {
//TODO : need fix size condition	
	while(*ep->CSR & UDP_CSR_TXPKTRDY);
	
	if(size < UDP_EP0_TX_BUFFER_SIZE) {
		for(uint32_t i = 0; i < size; i++) UDP->UDP_FDR[0] = *(data + i);
	}
	
}

void udp_write(udp_ep_t *ep, const uint8_t *data, uint32_t size) {
	udp_push(ep, data, size);
	*ep->CSR |= UDP_CSR_TXPKTRDY;
}

void udp_setaddress_set(uint8_t type) {
	uint8_t header[8] = { 0x00, UDP_bRequest_GET_STATUS, type, index, 0x00, 0x00, 0x00, 0x00 };
//	for(uint8_t i = 0; i < 8; i++) udp_fifo_push(&ep_control, *(header + i) );

//	UDP->UDP_CSR[0] |= UDP_CSR_TXPKTRDY;
	udp_write(&ep_control, 0, 0);
	
	while(!(UDP->UDP_CSR[0] & UDP_CSR_TXCOMP) );
	
	UDP->UDP_FADDR |= temp_addr | UDP_FADDR_FEN;
	UDP->UDP_GLB_STAT |= UDP_GLB_STAT_FADDEN;
}

void udp_set_descriptor(uint8_t type, uint8_t index) {
	uint8_t header[8] = { 0x00, UDP_bRequest_SET_DESCRIPTOR, type, index, 0x00, 0x00, 0x00, 0x00 };
	uint16_t d_size;
	uint8_t *d_p;
		
	switch(type) {
		case UDP_wValue_DT_DEV :
				d_size = sizeof(udp_dev_descriptor);
				d_p = &udp_dev_descriptor;
			break;
		case UDP_wValue_DT_CONF :
			break;
		case UDP_wValue_DT_EP :
			d_size = sizeof(udp_ep0_descriptor);
			d_p = &udp_ep0_descriptor;
			break;
	}
	
	header[6] = d_size;
	
//	udp_push(&ep_control, header, 8);
//	udp_write(&ep_control, header, 8);
	udp_write(&ep_control, d_p, d_size);
}

void udp_fifo_push(udp_ep_t *ep, uint8_t value) {
	UDP->UDP_FDR[0] = value;
}

void udp_enumerate(const udp_setup_data_t *request) {
				
	/* bmRequestType: type */
	switch((request->bmRequestType & 0x60) >> 5) {
		case UDP_bmRequestType_Type_STANDART :
			
			switch(request->bRequest) {
				case UDP_bRequest_GET_DESCRIPTOR :
					/* Descriptor type */
					udp_set_descriptor(request->wValue >> 8, 0);
					break;

				case UDP_bRequest_GET_CONFIGURATION :
					break;

				case UDP_bRequest_GET_INTERFACE :
					break;

				case UDP_bRequest_GET_STATUS :
					break;
				
				case UDP_bRequest_SET_ADDRESS :
					temp_addr = (request->wValue >> 8 ) & 0x7F;
					udp_setaddress_set(0);
					break;
			}
			
	}
}