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
void udp_enumerate(const udp_setup_data_t *request);

/* Processing requests */
void udp_get_descriptor(uint16_t wValue, uint16_t wIndex, uint16_t wLength);
void udp_set_address(uint16_t wValue);
void udp_set_configuration(uint16_t wValue);
void udp_set_interface(uint16_t wValue);

/* Callbacks for request */
void _udp_set_address_callback(void);
void _udp_set_configuration_callback(void);



#endif /* INCFILE1_H_ */