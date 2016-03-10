/*
 * urb.c
 *
 * Created: 23.03.2015 15:09:40
 *  Author: ale
 */ 

#include <sam.h>
#include <stddef.h>
#include "urb.h"
#include "udp.h"
#include "include/udp.h"
//#include "core/syslog.h"
#include "udp/endpoint.h"
#include "include/error_code.h"

inline void udp_fifo_push(udp_ep_audio_t *ep, uint8_t value) {
	*ep->ep.FDR = value;
}

int udp_push(udp_ep_setup_t *ep) {
	if(udp_setup_pkg.tx == NULL) return SUCCESS;
	while(*ep->ep.CSR & UDP_CSR_TXPKTRDY);
	
	// Copy data from software buffer to hardware
	uint32_t size = (UDP_EP_CTRL0_SIZE < (udp_setup_pkg.tx_size - udp_setup_pkg.tx_count)) ? UDP_EP_CTRL0_SIZE : (udp_setup_pkg.tx_size - udp_setup_pkg.tx_count);
	for(uint32_t i = 0; i < size; i++) *ep->ep.FDR = (uint8_t) *(udp_setup_pkg.tx + udp_setup_pkg.tx_count + i);
	udp_setup_pkg.tx_count += size;

	if(udp_setup_pkg.tx_size == udp_setup_pkg.tx_count) udp_setup_pkg.tx = NULL;
	
	// Tell to USB device that data are ready to sent.
	__ep_ctrl_set(&ep->ep, UDP_CSR_TXPKTRDY);
	
	return SUCCESS;
}

int udp_send_setup(const uint8_t *data, uint32_t size) {
	// anti collision condition
	if(udp_get_state() == UDP_STATE_POWERED) return ERRPOSIBLE;

	udp_setup_pkg.state = UDP_SETUP_ACT_TRANS;
	
	udp_setup_pkg.tx = (uint8_t *) data;
	udp_setup_pkg.tx_size = size;
	udp_setup_pkg.tx_count = 0;
	
	udp_push(&ep_control);
	
	return SUCCESS;
}

/**
* Function prepare endpoint struct for transaction and call udp_push().
* After that portion(if they have) are transmit by udp_push() called by TXCOMP interrupts.
**/
int udp_send_data(udp_ep_setup_t *ep, uint8_t *data, uint32_t size) {

	udp_setup_pkg.tx = data;
	udp_setup_pkg.tx_size = size;
	udp_setup_pkg.tx_count = 0;
	
	udp_setup_pkg.state = udp_push(ep);
	return SUCCESS;
}

udp_setup_pkg_t urb_get_setup_pkg(udp_ep_setup_t *ep) {
	udp_setup_pkg_t request;
	uint8_t pkg_size = (*(ep->ep.CSR) & 0x3ff0000) >> 16 ;
	for(uint8_t i = 0; i < pkg_size; i++)
	*((uint8_t *) &request + i) = (uint8_t) *ep->ep.FDR;

	return request;
}

/**
* Function send zero length package.
* Endpoint switch in ZLP state and wait TXCOMP interrupt.
**/
int udp_send_zlp(udp_ep_setup_t *ep) {
	while(*ep->ep.CSR & UDP_CSR_TXPKTRDY);
	__ep_ctrl_set(&ep->ep, UDP_CSR_TXPKTRDY);

	return SUCCESS;
}

int udp_send_setup_zlp() {
	return udp_send_zlp(&ep_control);
}

/**
* Function switches endpoint in stall state. 
* On the bus set stall signal. We need wait for STALLSENT interrupt and remove FORCESTALL.
**/
int udp_send_stall(udp_ep_setup_t *ep) {
	__ep_ctrl_set(&ep->ep, UDP_CSR_FORCESTALL);
	return SUCCESS;
}

int udp_send_setup_stall() {
	return udp_send_stall(&ep_control);
}