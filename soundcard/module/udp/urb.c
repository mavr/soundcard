/*
 * urb.c
 *
 * Created: 23.03.2015 15:09:40
 *  Author: ale
 */ 

#include "sam.h"
#include "udp.h"
#include "include/udp.h"
//#include "core/syslog.h"
#include "udp/endpoint.h"
#include "include/error_code.h"

inline void udp_fifo_push(udp_ep_audio_t *ep, uint8_t value) {
	UDP->UDP_FDR[0] = value;
}

int udp_push(udp_ep_setup_t *ep) {
	if(ep->tx_size == ep->tx_count) return EP_STATE_IDLE;

	while(*ep->ep.CSR & UDP_CSR_TXPKTRDY);
	
	//TODO:maybe ep->size ?
	uint32_t size = (UDP_EP_CTRL0_SIZE < (ep->tx_size - ep->tx_count)) ? UDP_EP_CTRL0_SIZE : (ep->tx_size - ep->tx_count);
	
	for(uint32_t i = 0; i < size; i++) *ep->ep.FDR = (uint8_t) *(ep->tx_buffer + ep->tx_count + i);
	ep->tx_count += size;
	
	__ep_ctrl_set(&ep->ep, UDP_CSR_TXPKTRDY);
	
	if(ep->ep.state == EP_STATE_SETUP) return EP_STATE_SETUP;
	
	return EP_STATE_TRANS;
}

int udp_send_setup(udp_ep_setup_t *ep, const uint8_t *data, uint32_t size) {
	if(ep->ep.state != EP_STATE_IDLE) {
		__UDP_DEBUG(LOG_LVL_MED, "Trying send setup but ep is busy.");
		return ERRBUSY;
	}
	
	ep->ep.state = EP_STATE_TRANS;
	
//TODO: anti collision condition
//	if(_udp.state == UDP_STATE_POWERED) return ERRPOSIBLE;

	ep->tx_buffer = data;
	ep->tx_size = size;
	ep->tx_count = 0;
	
	udp_push(ep);
	return SUCCESS;
}

int udp_send_data(udp_ep_setup_t *ep, uint8_t *data, uint32_t size) {
	ep->tx_buffer = data;
	ep->tx_size = size;
	ep->tx_count = 0;
	
	ep->ep.state = udp_push(ep);
	return SUCCESS;
}

int udp_send_zlp(udp_ep_setup_t *ep) {
	if(ep->ep.state != EP_STATE_IDLE) return ERRBUSY;
	while(*ep->ep.CSR & UDP_CSR_TXPKTRDY);
	__ep_ctrl_set(&ep->ep, UDP_CSR_TXPKTRDY);
	
	return SUCCESS;
}

int udp_send_stall(udp_ep_setup_t *ep) {
	*ep->ep.CSR |= UDP_CSR_FORCESTALL;
	return SUCCESS;
}