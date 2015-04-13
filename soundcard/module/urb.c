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
	if(ep->tx_size == ep->tx_count) return EP_STATE_IDLE;

	while(*ep->CSR & UDP_CSR_TXPKTRDY);
	
	uint32_t size = (ep->size < (ep->tx_size - ep->tx_count)) ? ep->size : (ep->tx_size - ep->tx_count);

	for(uint32_t i = 0; i < size; i++) *ep->FDR = (uint8_t) *(ep->tx_buffer + ep->tx_count + i);
	ep->tx_count += size;
	
	ep_control_set(ep, UDP_CSR_TXPKTRDY);
	
	return EP_STATE_TRANS;
}

int udp_audio_stream_in(uint16_t value) {
	static int c = 0;
	static int v = 0;
//	if(*ep_in.CSR & UDP_CSR_TXPKTRDY) return 0;

	*ep_in.FDR = (uint8_t) (value >> 8);
	*ep_in.FDR = (uint8_t) v++;
	c += 2;
	if(c == 512) {
//	if((*ep_in.CSR >> 16) == 0) {
//		*ep->CSR = 
		ep_control_set(&ep_in, UDP_CSR_TXPKTRDY);
		c = 0;
	}
}

uint16_t udp_audio_stream_out() {
	static uint16_t sound_tmp = 0;
	return (sound_tmp++ & 0x0fff) | 0x00ff ;
}

int udp_send_data(udp_ep_t *ep, uint8_t *data, uint32_t size) {
	ep->tx_buffer = data;
	ep->tx_size = size;
	ep->tx_count = 0;
	
	ep->state = udp_push(ep);
	return SUCCESS;
}

int udp_send_setup(udp_ep_t *ep, uint8_t *data, uint32_t size) {
	//	if(ep->state != EP_STATE_IDLE) return ERRBUSY;
	//	ep->state = EP_STATE_TRANS;
	
	// anti collision condition
	if(_udp.state == UDP_STATE_POWER) return ERRPOSIBLE;

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