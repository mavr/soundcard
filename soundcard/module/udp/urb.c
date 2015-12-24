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
	*ep->ep.FDR = value;
}

int udp_push(udp_ep_setup_t *ep) {
	// Return if no data to sent.
	if(ep->tx_size == ep->tx_count) return EP_STATE_IDLE;

	while(*ep->ep.CSR & UDP_CSR_TXPKTRDY);
	
	// Copy data from software buffer to hardware
	//TODO:maybe ep->size ?
	uint32_t size = (UDP_EP_CTRL0_SIZE < (ep->tx_size - ep->tx_count)) ? UDP_EP_CTRL0_SIZE : (ep->tx_size - ep->tx_count);
	for(uint32_t i = 0; i < size; i++) *ep->ep.FDR = (uint8_t) *(ep->tx_buffer + ep->tx_count + i);
	ep->tx_count += size;
	
	// Tell to USB device that data are ready to sent.
	__ep_ctrl_set(&ep->ep, UDP_CSR_TXPKTRDY);
	
	// Return TRANS state and wait for the EP TXCOMP interrupt.
	return EP_STATE_TRANS;
}

int udp_send_setup(udp_ep_setup_t *ep, const uint8_t *data, uint32_t size) {
	if(ep->ep.state != EP_STATE_IDLE) {
		__UDP_DEBUG(LOG_LVL_MED, "Error! Trying send setup but ep is busy.");
		// Time for ugly code
		switch(ep->ep.state) {
			case EP_STATE_NONE	: __UDP_DEBUG(LOG_LVL_MED, "URB(send_setup) : Control EP state = NONE"); break;
			case EP_STATE_TRANS : __UDP_DEBUG(LOG_LVL_MED, "URB(send_setup) : Control EP state = TRANS"); break;
			case EP_STATE_SETUP : __UDP_DEBUG(LOG_LVL_MED, "URB(send_setup) : Control EP state = SETUP"); break;
			case EP_STATE_ZLP : __UDP_DEBUG(LOG_LVL_MED, "URB(send_setup) : Control EP state = ZLP"); break;
			case EP_STATE_STALL : __UDP_DEBUG(LOG_LVL_MED, "URB(send_setup) : Control EP state = STALL"); break;
			default : __UDP_DEBUG(LOG_LVL_MED, "URB(send_setup) : Control EP state = uknown."); break;
		}
		return ERRBUSY;
	}
	
	ep->ep.state = EP_STATE_TRANS;
	
// anti collision condition
	if(udp_get_state() == UDP_STATE_POWERED) return ERRPOSIBLE;

	ep->tx_buffer = (uint8_t *) data;
	ep->tx_size = size;
	ep->tx_count = 0;
	
	udp_push(ep);
	return SUCCESS;
}

/**
* Function prepare endpoint struct for transaction and call udp_push().
* After that portion(if they have) are transmit by udp_push() called by TXCOMP interrupts.
**/
int udp_send_data(udp_ep_setup_t *ep, uint8_t *data, uint32_t size) {
	if(ep->ep.state != EP_STATE_IDLE) {
		__UDP_DEBUG(LOG_LVL_LOW, "Error: Cant send data. Endpoint busy.");
		return ERRBUSY;
	}
	
	ep->tx_buffer = data;
	ep->tx_size = size;
	ep->tx_count = 0;
	
	ep->ep.state = udp_push(ep);
	return SUCCESS;
}

/**
* Function send zero length package.
* Endpoint switch in ZLP state and wait TXCOMP interrupt.
**/
int udp_send_zlp(udp_ep_setup_t *ep) {
	if(ep->ep.state != EP_STATE_IDLE) {
		__UDP_DEBUG(LOG_LVL_LOW, "Error: Cant send zero length pkg. Endpoint busy.");
		return ERRBUSY;
	}
	while(*ep->ep.CSR & UDP_CSR_TXPKTRDY);
	
	ep->ep.state = EP_STATE_ZLP;
	// Nothing to sent. Just mark control register as ready to sent.
	__ep_ctrl_set(&ep->ep, UDP_CSR_TXPKTRDY);
	
	return SUCCESS;
}

/**
* Function switches endpoint in stall state. 
* On the bus set stall signal. We need wait for STALLSENT interrupt and remove FORCESTALL.
**/
int udp_send_stall(udp_ep_setup_t *ep) {
	if(ep->ep.state != EP_STATE_IDLE) {
		__UDP_DEBUG(LOG_LVL_LOW, "Error: Cant send stall signal. Endpoint busy.");
		return ERRBUSY;
	}
	
	ep->ep.state = EP_STATE_STALL;
	__ep_ctrl_set(&ep->ep, UDP_CSR_FORCESTALL);
	
	return SUCCESS;
}