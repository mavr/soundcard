/*
 * usb.c
 *
 * Created: 11.03.2015 14:28:35
 *  Author: ale
 */ 

#include "usb.h"
#include "sam.h"
#include "include/udp.h"
#include "udp/udp_request.h"
#include "include/error_code.h"
#include "include/system.h"
#include "udp/udp-audio.h"
#include <string.h>


/* Descriptor processing */

void udp_get_descriptor(uint16_t wValue, uint16_t wIndex, uint16_t wLength) {
	const uint8_t * _p_desc = 0;
	uint32_t _s_desc = 0;
		
	// Type of descriptor is contains in high byte. Low byte contains index of descriptor.
	switch(wValue >> 8) {
		case UDP_wValue_DT_DEV	:		_p_desc = udp_dev_descriptor; _s_desc = *(_p_desc); break;
		case UDP_wValue_DT_CONF :		_p_desc = udp_conf_descriptor; _s_desc = *(_p_desc + 2); break; 
		case UDP_wValue_DT_STR	:		
			switch(wValue & 0x00ff) {
				case 0x00: _p_desc = udp_str_zero_descriptor; _s_desc = *(_p_desc); break;
				case 0x01: _p_desc = udp_str_manufactur_descriptor; _s_desc = *(_p_desc); break;
				case 0x02: _p_desc = udp_str_product_descriptor; _s_desc = *(_p_desc); break;
				case 0x03: _p_desc = udp_str_serial_descriptor; _s_desc = *(_p_desc); break;
//				default: udp_send_zlp(&ep_control);
			}
			break;
			
//		case UDP_wValue_DT_INT	:		_p_desc = udp_int_descriptor; break;
//		case UDP_wValue_DT_EP	:		_p_desc = udp_ep0_descriptor; break;
		
		/* UDP_wValue_DT_DEV_QUAL , UDP_wValue_DT_INT_POWER */
		default: 
			udp_send_stall(&ep_control);
		break;
	}

	if(_s_desc > wLength) _s_desc = wLength;
	udp_send_setup(&ep_control, _p_desc, _s_desc);
}

void udp_set_address(uint16_t wValue) {
	udp_send_zlp(&ep_control);
	
	ep_control.wValue = wValue;
	ep_control.callback = &_udp_set_address_callback;
}

void udp_set_configuration(uint16_t wValue) {
	udp_send_zlp(&ep_control);
	ep_control.callback = &_udp_set_configuration_callback;
}

void udp_set_interface(uint16_t wValue) {
	udp_send_zlp(&ep_control);
}

/* Callbacks */

void _udp_set_address_callback() {
	UDP->UDP_GLB_STAT = UDP_GLB_STAT_FADDEN;
	UDP->UDP_FADDR |= (ep_control.wValue & 0x7f) | UDP_FADDR_FEN;
}

void _udp_set_configuration_callback() {
//TODO: udp_get_state()
//	if(udp_get_state() == UDP_STATE_ADDRESS) udp_set_state(UDP_STATE_CONFIGURED);
//	else if(udp_get_state() == UDP_STATE_CONFIGURED) udp_set_state(UDP_STATE_ADDRESS);
	
	// turn on endpoints
	ep_enable(&ep_in.ep);
	__ep_ctrl_set(&ep_in.ep, UDP_CSR_TXPKTRDY);
}

void udp_enumerate(const udp_setup_data_t *request) {
	
	//TODO: state
	//ep_control.state = EP_STATE_SETUP;
				
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
					if((request->wLength != 0) || (request->wIndex != 0) || ((request->wValue & 0xff00) != 0x00)) break;
					udp_set_configuration(request->wValue);
					break;

				case UDP_bRequest_GET_INTERFACE :
					break;
				
				case UDP_bRequest_SET_INTERFACE :
					udp_set_interface(request->wValue);
					break;

				case UDP_bRequest_GET_STATUS :
					break;
				
				case UDP_bRequest_SET_ADDRESS :	
					udp_set_address(request->wValue); 
					break;
				
				default: 
					//ep_control.state = EP_STATE_IDLE; 
					break;
			}
			
	}
}