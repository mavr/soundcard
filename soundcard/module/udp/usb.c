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

void udp_enumerate(const udp_setup_data_t *request) {
	//char ddebug[80];
	//char tmp[8];
	//strcat(ddebug, "udp_enumeration: index=");
	//strcat(ddebug, itoa(request->wIndex, tmp, 10));
	//strcat(ddebug, "; length=");
	//strcat(ddebug, itoa(request->wLength, tmp, 10));
	//__UDP_DEBUG(LOG_LVL_HIGH, ddebug);
	
	/* bmRequestType: type */
	switch((request->bmRequestType & 0x60) >> 5) {
		case UDP_bmRequestType_Type_STANDART :
				__udp_request_standart(request);
				break;
		case UDP_bmRequestType_Type_CLASS :
				__udp_request_class(request);
				break;
		case UDP_bmRequestType_Type_VENDOR :
				__udp_request_vendor(request);
				break;	
		default:
				__UDP_DEBUG(LOG_LVL_HIGH, "Error!! Request's field bmRequestType has uknown value.");
				break;
	}
}

void __udp_request_standart(const udp_setup_data_t *request) {
	switch(request->bmRequestType & 0x1f) {
		case UDP_bmRequestType_Recipient_DEV : 
			__udp_request_standart_device(request);
			break;
		case UDP_bmRequestType_Recipient_INT :
			__udp_request_standart_interface(request);
			break;
		case UDP_bmRequestType_Recipient_EP :
			__udp_request_standart_endpoint(request);
			break;
		default:
			__udp_request_standart_other(request);
			break;
	}
	
}

void __udp_request_class(const udp_setup_data_t *request) {
	switch(request->bmRequestType & 0x1F) {
		case UDP_bmRequestType_Recipient_DEV :
			__udp_request_class_device(request);
			break;
	
		case UDP_bmRequestType_Recipient_INT :
			__udp_request_class_interface(request);
			break;
	
		case UDP_bmRequestType_Recipient_EP :
			__UDP_DEBUG(LOG_LVL_HIGH, "Control EP: Class request to endpoint.");
		break;
	
		default:
			__UDP_DEBUG(LOG_LVL_HIGH, "Error! Control EP: Class request to reserved values.");
	}	
}

void __udp_request_vendor(const udp_setup_data_t *request) {
	
}

void __udp_request_standart_device(const udp_setup_data_t *request) {
	switch(request->bRequest) {
		case UDP_bRequest_GET_STATUS :
			//TODO: get_status()
			break;

		case UDP_bRequest_CLEAR_FEATURE :
			//TODO: clear_feature()
			break;

		case UDP_bRequest_SET_FEATURE :
			//TODO: set_feature()
			break;

		case UDP_bRequest_SET_ADDRESS :
			udp_set_address(request->wValue); 
			break;

		case UDP_bRequest_GET_DESCRIPTOR :
			__UDP_DEBUG(LOG_LVL_HIGH, "Request standart -> device : GET_DESCRIPTOR.");
			udp_get_descriptor(request->wValue, request->wIndex, request->wLength);
			break;

		case UDP_bRequest_SET_DESCRIPTOR :
			//TODO: set_descriptor()
			break;

		case UDP_bRequest_GET_CONFIGURATION :
			//TODO: get_configuration()
			break;

		case UDP_bRequest_SET_CONFIGURATION :
			//TODO: wtf
			/* If wIndex, wLength, or the upper byte of wValue is non-zero, 
			then the behavior of this request is not specified. */
			if((request->wLength != 0) || (request->wIndex != 0) || ((request->wValue & 0xff00) != 0x00)) break;
			udp_set_configuration(request->wValue);
			break;

		default :
			//Error
			__UDP_DEBUG(LOG_LVL_HIGH, "Request standart -> device : uknown.");
			break;
	}
}

void __udp_request_standart_interface(const udp_setup_data_t *request) {
	switch(request->bRequest) {
		case UDP_bRequest_GET_STATUS :
			//TODO: get_status()
			break;

		case UDP_bRequest_CLEAR_FEATURE :
			//TODO: clear_feature()
			break;

		case UDP_bRequest_SET_FEATURE :
			//TODO: set_feature()
			break;
				
		case UDP_bRequest_GET_INTERFACE :
			//TODO: get_interface()
			__UDP_DEBUG(LOG_LVL_HIGH, "Request standart -> interface : GET_INTERFACE.");
			break;
				
		case UDP_bRequest_SET_INTERFACE :
			//TODO: set_interface
			udp_set_interface(request->wValue);
			break;
			
		case UDP_bRequest_GET_DESCRIPTOR :
			__UDP_DEBUG(LOG_LVL_HIGH, "Request standart -> interface : GET_DESCRIPTOR.");
			udp_get_descriptor(request->wValue, request->wIndex, request->wLength);
			break;
				
		default:
			//Error
			__UDP_DEBUG(LOG_LVL_HIGH, "Request standart -> interface : uknown.");
			break;
	}
}

void __udp_request_standart_endpoint(const udp_setup_data_t *request) {
	switch(request->bRequest) {
		//case UDP_bRequest_GET_STATUS :
			////TODO: get_status()
			//break;
//
		//case UDP_bRequest_CLEAR_FEATURE :
			////TODO: clear_feature()
			//break;
//
		//case UDP_bRequest_SET_FEATURE :
			////TODO: set_feature()
			//break;
			//
		//case UDP_bRequest_SYNCH_FRAME :
			////TODO: sync_frame()
			//break;
			
		default:
			//Error
			__UDP_DEBUG(LOG_LVL_HIGH, "Request standart -> endpoint : uknown.");
			break;
	}
}

void __udp_request_standart_other(const udp_setup_data_t *request) {
	
}

void __udp_request_class_device(const udp_setup_data_t *request) {
	__UDP_DEBUG(LOG_LVL_HIGH, "Control EP: Class request to device.");
}

void __udp_request_class_interface(const udp_setup_data_t *request) {
	switch(request->bRequest) {
		case UDP_bRequest_GET_REPORT :
			__UDP_DEBUG(LOG_LVL_HIGH, "Request class -> interface : GET_REPORT.");
			//udp_get_descriptor(request->wValue, request->wIndex, request->wLength);
			udp_send_setup(&ep_control, udp_hid_interface_descriptor, 0x09);
			__UDP_DEBUG(LOG_LVL_HIGH, "Reply: HID_INTERFACE_DESCRIPTOR.");
			break;
	}
}

void udp_get_descriptor(uint16_t wValue, uint16_t wIndex, uint16_t wLength) {
	const uint8_t * _p_desc = 0;
	uint32_t _s_desc = 0;
		
	// Type of descriptor is contains in high byte. Low byte contains index of descriptor.
	switch(wValue >> 8) {
		case UDP_wValue_DESCRIPTORT_DEV	:		
				_p_desc = udp_dev_descriptor; _s_desc = UDP_DESCRIPTOR_DEVICE_SIZE; 
				__UDP_DEBUG(LOG_LVL_HIGH, "Reply: DEVICE_DESCRIPTOR.");
				break;
		case UDP_wValue_DESCRIPTORT_CONF :
				_p_desc = udp_conf_descriptor; _s_desc = UDP_DESCRIPTOR_CONF_SIZE; 
				__UDP_DEBUG(LOG_LVL_HIGH, "Reply: CONFIGURATION_DESCRIPTOR.");
				break; 
		case UDP_wValue_DESCRIPTORT_STR	:		
				__UDP_DEBUG(LOG_LVL_HIGH, "Reply: STRING_DESCRIPTOR.");
			switch(wValue & 0x00ff) {
				case 0x00: _p_desc = udp_str_zero_descriptor; _s_desc = *(_p_desc); break;
				case 0x01: _p_desc = udp_str_manufactur_descriptor; _s_desc = *(_p_desc); break;
				case 0x02: _p_desc = udp_str_product_descriptor; _s_desc = *(_p_desc); break;
				case 0x03: _p_desc = udp_str_serial_descriptor; _s_desc = *(_p_desc); break;
//				default: udp_send_zlp(&ep_control);
			}
			break;
			
		case UDP_wValue_DESCRIPTORT_HID:
			__UDP_DEBUG(LOG_LVL_HIGH, "Reply: HID_INTERFACE_DESCRIPTOR.");
			_p_desc = udp_hid_interface_descriptor; _s_desc = 0x09;
			break;
				
		case UDP_wValue_DESCRIPTORT_HID_REPORT :
			__UDP_DEBUG(LOG_LVL_HIGH, "Reply: HID_REPORT_DESCRIPTOR.");
			_p_desc = udp_kbd_report_descriptor; _s_desc = UDP_DESCRIPTOR_HID_REPORT_SIZE;
			break;
			
		default: 
			udp_send_stall(&ep_control);
			__UDP_DEBUG(LOG_LVL_HIGH, "Error! udp_get_descriptor() : uknown descriptor.");
			break;
	}

	if(_s_desc > wLength) _s_desc = wLength;
	udp_send_setup(&ep_control, _p_desc, _s_desc);
}

void udp_set_address(uint16_t wValue) {
	udp_send_zlp(&ep_control);
	
	__UDP_DEBUG(LOG_LVL_HIGH, "Receive: SET address.");
	ep_control.wValue = wValue;
	ep_control.callback = &_udp_set_address_callback;
}

void udp_set_configuration(uint16_t wValue) {
	__UDP_DEBUG(LOG_LVL_HIGH, "Receive: SET configuratuion.");
	udp_send_zlp(&ep_control);
	ep_control.callback = &_udp_set_configuration_callback;
}

void udp_set_interface(uint16_t wValue) {
	udp_send_zlp(&ep_control);
}

/* Callbacks */

void _udp_set_address_callback() {
	udp_set_state(UDP_STATE_ADDRESS);
//	UDP->UDP_GLB_STAT = UDP_GLB_STAT_FADDEN;
	UDP->UDP_FADDR |= (ep_control.wValue & 0x7f) | UDP_FADDR_FEN;
}

void _udp_set_configuration_callback() {
	if(udp_get_state() == UDP_STATE_ADDRESS) udp_set_state(UDP_STATE_CONFIGURED);
	else if(udp_get_state() == UDP_STATE_CONFIGURED) udp_set_state(UDP_STATE_ADDRESS);
	
	// turn on endpoints
	__UDP_DEBUG(LOG_LVL_HIGH, "Configuration setted. Enable audio in ep.");
	ep_enable(&ep_in.ep);
	//*ep_in.ep.FDR = 0x88;
//	UDP->UDP_FDR[4] = 0x88;
	__ep_ctrl_set(&ep_in.ep, UDP_CSR_TXPKTRDY);
}

