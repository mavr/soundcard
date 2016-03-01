/*
 * udp_product_descriptors.c
 *
 * Created: 16.06.2015 14:51:20
 *  Author: ale
 */ 

#include "sam.h"

const uint8_t udp_str_zero_descriptor[] = { 0x04, 0x03,	0x00, 0x01 };

/* Manufacturer : RadioAvionica */
const uint8_t udp_str_manufactur_descriptor[] = {
	0x1c, // bLenght
	0x03, // bDescriptorType
	'R', 0x00, 'a', 0x00, 'd', 0x00, 'i', 0x00, 'o', 0x00,
	'A', 0x00, 'v', 0x00, 'i', 0x00, 'o', 0x00, 'n', 0x00, 'i', 0x00, 'c', 0x00, 'a', 0x00
};

/* Product : Audio device */
const uint8_t udp_str_product_descriptor[] = {
	0x1a, // bLenght
	0x03, // bDescriptorType
	'A', 0x00, 'u', 0x00, 'd', 0x00, 'i', 0x00, 'o', 0x00, ' ', 0x00,
	'd', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00
};

/* Serial Number : 0.01 */
const uint8_t udp_str_serial_descriptor[] = {
	0x0a, // bLenght
	0x03, // bDescriptorType
	'0', 0x00, '.', 0x00, '0', 0x00, '1', 0x00
};

/* Mixer unit name */
const uint8_t udp_str_mixer_name[] = {
	0x0e, // bLenght
	0x03, // bDescriptorType
	'S', 0x00, 'M', 0x00, 'i', 0x00, 'x', 0x00, 'e', 0x00, 'r', 0x00
};

/* Mixer unit name */
const uint8_t udp_str_phone_FU_name[] = {
	0x0d, // bLenght
	0x03, // bDescriptorType
	'P', 0x00, 'h', 0x00, 'o', 0x00, 'n', 0x00, 'e', 0x00
};