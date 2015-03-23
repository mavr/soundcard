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
