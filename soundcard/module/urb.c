/*
 * urb.c
 *
 * Created: 23.03.2015 15:09:40
 *  Author: ale
 */ 

#include "sam.h"
#include "udp.h"
#include "error_code.h"

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
	
	return SUCCESS;
}

int udp_send_stall(udp_ep_t *ep) {
	*ep->CSR |= UDP_CSR_FORCESTALL;
	return SUCCESS;
}