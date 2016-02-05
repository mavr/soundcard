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

/* USB string descriptors. There are describes in udp_product_descriptors.c */
extern const uint8_t udp_str_zero_descriptor[];
extern const uint8_t udp_str_manufactur_descriptor[];
extern const uint8_t udp_str_product_descriptor[];
extern const uint8_t udp_str_serial_descriptor[];

//typedef struct {
//	enum udp_state state;
//} udp_t;

//udp_t _udp;

#include "udp/endpoint.h"
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
void _udp_set_address_callback(void);
void _udp_set_configuration_callback(void);

/* Class */
void _udp_ac_set_res(void);
void _udp_ac_set_cur(void);

#endif /* UDP_USB_H_ */