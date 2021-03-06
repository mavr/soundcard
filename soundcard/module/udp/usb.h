/*
 * IncFile1.h
 *
 * Created: 11.03.2015 14:28:06
 *  Author: ale
 */ 


#ifndef UDP_USB_H_
#define UDP_USB_H_

#include "sam.h"
#include "udp.h"
#include "urb.h"

/* USB string descriptors. There are describes in udp_product_descriptors.c */
extern const uint8_t udp_str_zero_descriptor[];
extern const uint8_t udp_str_manufactur_descriptor[];
extern const uint8_t udp_str_product_descriptor[];
extern const uint8_t udp_str_serial_descriptor[];
extern const uint8_t udp_str_mixer_name[];
extern const uint8_t udp_str_phone_FU_name[];
extern const uint8_t udp_str_mic_pre_name[];
extern const uint8_t udp_str_mic_amp_name[];
//typedef struct {
//	enum udp_state state;
//} udp_t;

//udp_t _udp;

#include "udp/endpoint.h"

#define UDP_REQUEST_INTERFACE(value) ( (uint8_t) (value & 0x00ff))

enum udp_error_t {
	UDP_ERROR_REQUEST_UNSUPPORT,
	UDP_ERROR_UKNOWN,
};

/* Configuring process of the device. */
void udp_enumerate(udp_setup_activity_t *udp_setup_pkg);
void __udp_request_standart(udp_setup_activity_t *udp_setup_pkg);
void __udp_request_class(udp_setup_activity_t *udp_setup_pkg);
void __udp_request_vendor(udp_setup_activity_t *udp_setup_pkg);

void __udp_request_standart_device(udp_setup_activity_t *udp_setup_pkg);
void __udp_request_standart_interface(udp_setup_activity_t *udp_setup_pkg);
void __udp_request_standart_endpoint(udp_setup_activity_t *udp_setup_pkg);
void __udp_request_standart_other(udp_setup_activity_t *udp_setup_pkg);

void __udp_request_class_device(udp_setup_activity_t *udp_setup_pkg);
void __udp_request_class_interface(udp_setup_activity_t *udp_setup_pkg);
void __udp_request_class_endpoint(udp_setup_activity_t *udp_setup_pkg);
void __udp_request_class_other(udp_setup_activity_t *udp_setup_pkg);

void __udp_request_class_hid(udp_setup_activity_t *udp_setup_pkg);
void __udp_request_class_report(udp_setup_activity_t *udp_setup_pkg);
void __udp_request_class_physical(udp_setup_activity_t *udp_setup_pkg);

/* Processing requests */
void udp_get_descriptor(uint16_t wValue, uint16_t wIndex, uint16_t wLength);
void udp_set_address(uint16_t wValue);
void udp_set_configuration(uint16_t wValue);
void udp_set_interface(uint16_t wIndex);

/** Callbacks for request **/
/* Standart */
void _udp_set_address_callback(udp_request_callback_t *);
void _udp_set_configuration_callback(udp_request_callback_t *);

/* UDP error callback. */
void udp_request_unsupport(enum udp_error_t error);

#endif /* UDP_USB_H_ */