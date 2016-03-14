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
#include "audio/audio.h"
#include "udp/audio.h"
#include "codec/pcm3793.h"
#include <string.h>
/* Descriptor processing */

void udp_enumerate(udp_setup_activity_t *udp_setup_pkg) {
	/* bmRequestType: type */
	switch((udp_setup_pkg->pkg.bmRequestType & 0x60) >> 5) {
		case UDP_bmRequestType_Type_STANDART :
				__udp_request_standart(udp_setup_pkg);
				break;
		case UDP_bmRequestType_Type_CLASS :
				__udp_request_class(udp_setup_pkg);
				break;
		case UDP_bmRequestType_Type_VENDOR :
				__udp_request_vendor(udp_setup_pkg);
				break;	
		default:
				udp_send_stall(&ep_control);
				__UDP_DEBUG(LOG_LVL_HIGH, "Error!! Request's field bmRequestType has uknown value.");
				break;
	}
}

void __udp_request_standart(udp_setup_activity_t *udp_setup_pkg) {
	switch(udp_setup_pkg->pkg.bmRequestType & 0x1f) {
		case UDP_bmRequestType_Recipient_DEV : 
			__udp_request_standart_device(udp_setup_pkg);
			break;
		case UDP_bmRequestType_Recipient_INT :
			__udp_request_standart_interface(udp_setup_pkg);
			break;
		case UDP_bmRequestType_Recipient_EP :
			__udp_request_standart_endpoint(udp_setup_pkg);
			break;
		default:
			__udp_request_standart_other(udp_setup_pkg);
			break;
	}
	
}

void __udp_request_class(udp_setup_activity_t *udp_setup_pkg) {
	switch(udp_setup_pkg->pkg.bmRequestType & 0x1F) {
		case UDP_bmRequestType_Recipient_DEV :
			__udp_request_class_device(udp_setup_pkg);
			break;
	
		case UDP_bmRequestType_Recipient_INT :
			__udp_request_class_interface(udp_setup_pkg);
			break;
	
		case UDP_bmRequestType_Recipient_EP :
			__UDP_DEBUG(LOG_LVL_HIGH, "Request: (class -> endpoint) : unsupported.\r\n\tAnswer: STALL.");
			udp_send_stall(&ep_control);
		break;
	
		default:
			__UDP_DEBUG(LOG_LVL_HIGH, "Request: (class -> unknown) : unsupported.\r\n\tAnswer: STALL.");
			udp_send_stall(&ep_control);
			break;
	}	
}

void __udp_request_vendor(udp_setup_activity_t *udp_setup_pkg) {
	__UDP_DEBUG(LOG_LVL_HIGH, "Request: (vendor -> ...) : unsupported.\r\n\tAnswer: STALL.");
	udp_send_stall(&ep_control);
}

void __udp_request_standart_device(udp_setup_activity_t *udp_setup_pkg) {
	switch(udp_setup_pkg->pkg.bRequest) {
		case UDP_bRequest_GET_STATUS :

		case UDP_bRequest_CLEAR_FEATURE :

		case UDP_bRequest_SET_FEATURE :

		case UDP_bRequest_SET_DESCRIPTOR :

		case UDP_bRequest_GET_CONFIGURATION :
			udp_send_stall(&ep_control);
			__UDP_DEBUG(LOG_LVL_HIGH, "Request: (standart -> device) : unsupported.\r\n\tAnswer: STALL.");
			break;

		case UDP_bRequest_SET_ADDRESS :
			udp_set_address(udp_setup_pkg->pkg.wValue); 
			break;

		case UDP_bRequest_GET_DESCRIPTOR :
			//__UDP_DEBUG(LOG_LVL_HIGH, "Request standart -> device : GET_DESCRIPTOR.");
			udp_get_descriptor(udp_setup_pkg->pkg.wValue, udp_setup_pkg->pkg.wIndex, udp_setup_pkg->pkg.wLength);
			break;

		case UDP_bRequest_SET_CONFIGURATION :
			//TODO: wtf
			/* If wIndex, wLength, or the upper byte of wValue is non-zero, 
			then the behavior of this request is not specified. */
			if((udp_setup_pkg->pkg.wLength != 0) || (udp_setup_pkg->pkg.wIndex != 0) || ((udp_setup_pkg->pkg.wValue & 0xff00) != 0x00)) break;
			udp_set_configuration(udp_setup_pkg->pkg.wValue);
			break;

		default :
			//Error
			udp_send_stall(&ep_control);
			__UDP_DEBUG(LOG_LVL_HIGH, "Request standart -> device : uknown.");
			break;
	}
}

void __udp_request_standart_interface(udp_setup_activity_t *udp_setup_pkg) {
	switch(udp_setup_pkg->pkg.bRequest) {
		case UDP_bRequest_GET_STATUS :

		case UDP_bRequest_CLEAR_FEATURE :

		case UDP_bRequest_SET_FEATURE :

		case UDP_bRequest_GET_INTERFACE :
			udp_send_stall(&ep_control);
			__UDP_DEBUG(LOG_LVL_HIGH, "Request: (standart -> interface) : unsupported.\r\n\tAnswer: STALL.");
			break;

		case UDP_bRequest_SET_INTERFACE :
			udp_set_interface(udp_setup_pkg->pkg.wIndex);
			break;

		case UDP_bRequest_GET_DESCRIPTOR :
			//__UDP_DEBUG(LOG_LVL_HIGH, "Request standart -> interface : GET_DESCRIPTOR.");
			udp_get_descriptor(udp_setup_pkg->pkg.wValue, udp_setup_pkg->pkg.wIndex,udp_setup_pkg->pkg.wLength);
			break;

		default:
			//Error
			udp_send_stall(&ep_control);
			__UDP_DEBUG(LOG_LVL_HIGH, "Request (standart -> interface) : uknown.\r\n\tAnswer: STALL.");
			break;
	}
}

void __udp_request_standart_endpoint(udp_setup_activity_t *udp_setup_pkg) {
	switch(udp_setup_pkg->pkg.bRequest) {
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
			udp_send_stall(&ep_control);
			__UDP_DEBUG(LOG_LVL_HIGH, "Request: (standart -> endpoint) : unsupported.\r\n\tAnswer: STALL.");
			break;
	}
}

void __udp_request_standart_other(udp_setup_activity_t *udp_setup_pkg) {
	udp_send_stall(&ep_control);
	__UDP_DEBUG(LOG_LVL_HIGH, "Request: (standart -> other) : unsupported.\r\n\tAnswer: STALL.");
}

void __udp_request_class_device(udp_setup_activity_t *udp_setup_pkg) {
	udp_send_stall(&ep_control);
	__UDP_DEBUG(LOG_LVL_HIGH, "Request: (class -> device) : unsupported.\r\n\tAnswer: STALL.");
}

void __udp_request_class_interface(udp_setup_activity_t *udp_setup_pkg) {
	/* What the kind of interface */
	extern const uint8_t udp_hid_interface_descriptor[];

	switch(UDP_REQUEST_INTERFACE(udp_setup_pkg->pkg.wIndex)) {
		case UDP_AC_INTERFACE : 
		case UDP_AS_IN_INTERFACE : 
		case UDP_AS_OUT_INTERFACE : 
			udp_audio_controllers_proc(udp_setup_pkg);
			break;

		case UDP_HID_INTERFACE : 
			switch(udp_setup_pkg->pkg.bRequest) {
				case UDP_bRequest_GET_REPORT :
					__UDP_DEBUG(LOG_LVL_HIGH, "Request class -> interface : GET_REPORT.");
					//udp_get_descriptor(request->wValue, request->wIndex, request->wLength);
					udp_send_setup(udp_hid_interface_descriptor, 0x09);
					//__UDP_DEBUG(LOG_LVL_HIGH, "Reply: HID_INTERFACE_DESCRIPTOR.");
					break;

				case UDP_bRequest_SET_IDLE :
					//__UDP_DEBUG(LOG_LVL_HIGH, "Request class -> interface : SET_IDLE.");
					udp_send_zlp(&ep_control);
					break;

				default:
					udp_send_stall(&ep_control);
					__UDP_DEBUG(LOG_LVL_HIGH, "Error! __udp_request_class_interface(): uknown request.");
					break;
			}
			break;
	}

}

void udp_get_descriptor(uint16_t wValue, uint16_t wIndex, uint16_t wLength) {
	const uint8_t * _p_desc = 0;
	uint32_t _s_desc = 0;

	extern const uint8_t udp_dev_descriptor[];
	extern const uint8_t udp_conf_descriptor[];
	extern const uint8_t udp_hid_interface_descriptor[];
	extern const uint8_t udp_kbd_report_descriptor[];

	// Type of descriptor is contains in high byte. Low byte contains index of descriptor.
	switch(wValue >> 8) {
		case UDP_wValue_DESCRIPTORT_DEV	:		
				_p_desc = udp_dev_descriptor; _s_desc = UDP_DESCRIPTOR_DEVICE_SIZE; 
				//__UDP_DEBUG(LOG_LVL_HIGH, "Reply: DEVICE_DESCRIPTOR.");
				break;
		case UDP_wValue_DESCRIPTORT_CONF :
				//udp_set_wTotalLength();
				_p_desc = udp_conf_descriptor; _s_desc = UDP_DESCRIPTOR_CONF_SIZE;// sizeof(udp_conf_descriptor); 
				//__UDP_DEBUG(LOG_LVL_HIGH, "Reply: CONFIGURATION_DESCRIPTOR.");
				break; 
		case UDP_wValue_DESCRIPTORT_STR	:		
				//__UDP_DEBUG(LOG_LVL_HIGH, "Reply: STRING_DESCRIPTOR.");
			switch(wValue & 0x00ff) {
				case 0x00: _p_desc = udp_str_zero_descriptor; _s_desc = *(_p_desc); break;
				case 0x01: _p_desc = udp_str_manufactur_descriptor; _s_desc = *(_p_desc); break;
				case 0x02: _p_desc = udp_str_product_descriptor; _s_desc = *(_p_desc); break;
				case 0x03: _p_desc = udp_str_serial_descriptor; _s_desc = *(_p_desc); break;
				case 0x04: _p_desc = udp_str_mixer_name; _s_desc = *(_p_desc); break;
				case 0x05: _p_desc = udp_str_phone_FU_name; _s_desc = *(_p_desc); break;
				case 0x06: _p_desc = udp_str_mic_pre_name; _s_desc = *(_p_desc); break;
				case 0x07: _p_desc = udp_str_mic_amp_name; _s_desc = *(_p_desc); break;
				default: udp_send_zlp(&ep_control);
			}
			break;

		case UDP_wValue_DESCRIPTORT_HID:
			//__UDP_DEBUG(LOG_LVL_HIGH, "Reply: HID_INTERFACE_DESCRIPTOR.");
			_p_desc = udp_hid_interface_descriptor; _s_desc = 0x09;
			break;
				
		case UDP_wValue_DESCRIPTORT_HID_REPORT :
			//__UDP_DEBUG(LOG_LVL_HIGH, "Reply: HID_REPORT_DESCRIPTOR.");
			_p_desc = udp_kbd_report_descriptor; _s_desc = UDP_DESCRIPTOR_HID_REPORT_SIZE;
			break;

		default: 
			udp_send_stall(&ep_control);
			__UDP_DEBUG(LOG_LVL_HIGH, "Request: GET_DESCRIPTOR : uknown descriptor.\r\n\tAnswer: STALL.");
			return;
			break;
	}

	if(_s_desc > wLength) _s_desc = wLength;
	udp_send_setup(_p_desc, _s_desc);
}

void udp_set_address(uint16_t wValue) {
	udp_send_zlp(&ep_control);
	
//	__UDP_DEBUG(LOG_LVL_LOW, "Receive: SET address.");
	udp_setup_pkg.callback = &_udp_set_address_callback;
}

void udp_set_configuration(uint16_t wValue) {
//	__UDP_DEBUG(LOG_LVL_HIGH, "Receive: SET configuratuion.");
	udp_send_zlp(&ep_control);
	udp_setup_pkg.callback = &_udp_set_configuration_callback;
}

void udp_set_interface(uint16_t wIndex) {
	_udp.udp_interface = wIndex;
	switch(wIndex) {
		case UDP_AC_INTERFACE :		__UDP_DEBUG(LOG_LVL_LOW, "Set interface : Audio Control."); break;
		case UDP_AS_IN_INTERFACE :	__UDP_DEBUG(LOG_LVL_LOW, "Set interface : Audio Stream IN."); break;
		case UDP_AS_OUT_INTERFACE : __UDP_DEBUG(LOG_LVL_LOW, "Set interface : Audio Stream OUT."); break;
		case UDP_HID_INTERFACE :	__UDP_DEBUG(LOG_LVL_LOW, "Set interface : HID."); break;

		default: __UDP_DEBUG(LOG_LVL_LOW, "Set interface : uknown."); break;
	}
	udp_send_zlp(&ep_control);
}

/* Callbacks */

void _udp_set_address_callback(udp_request_callback_t *p) {
	UDP->UDP_FADDR |= (udp_setup_pkg.pkg.wValue & 0x7f) | UDP_FADDR_FEN;
	udp_set_state(UDP_STATE_ADDRESS);
}

void _udp_set_configuration_callback(udp_request_callback_t *p) {
	if(udp_get_state() == UDP_STATE_ADDRESS) udp_set_state(UDP_STATE_CONFIGURED);
	else if(udp_get_state() == UDP_STATE_CONFIGURED) udp_set_state(UDP_STATE_ADDRESS);
	
	// turn on endpoints
//	__UDP_DEBUG(LOG_LVL_HIGH, "Configuration setted. Enable audio in ep.");
	ep_enable(&ep_in.ep);
	
	__ep_ctrl_set(&ep_in.ep, UDP_CSR_TXPKTRDY);
}

void udp_request_unsupport(enum udp_error_t error) {
	switch(error) {
		case UDP_ERROR_REQUEST_UNSUPPORT :
			__UDP_DEBUG(LOG_LVL_HIGH, "Error");
			udp_send_stall(&ep_control);
			break;

		// This situation id called : what the fuck?!
		case UDP_ERROR_UKNOWN : 
			// Watchdog, i am here!
			for(volatile int i;; i++);
			break;
	};
	udp_send_stall(&ep_control);
}