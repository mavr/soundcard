/*
 * IncFile1.h
 *
 * Created: 11.03.2015 14:28:06
 *  Author: ale
 */ 


#ifndef USB_H_
#define USB_H_

#include "sam.h"
#include "usb.h"
#include "udp.h"

/* Endpoint addresses */
#define UDP_EP_CONTROL		0
#define UDP_EP_IN			4
#define UDP_EP_OUT			2

/* Endpoint types */
#define UDP_EP_TYPE_CONTROL		0
#define UDP_EP_TYPE_BULK_IN		1
#define UDP_EP_TYPE_BULK_OUT	2
#define UDP_EP_TYPE_INT			3
#define UDP_EP_TYPE_ISO_IN		4
#define UDP_EP_TYPE_ISO_OUT		5

/* Endpoint size */
#define UDP_EP0_SIZE			64
#define UDP_EP4_SIZE			512
#define UDP_EP5_SIZE			64

/* Endpoint buffers */
#define UDP_EP0_RX_BUFFER_SIZE	64
#define UDP_EP0_TX_BUFFER_SIZE	64

/* Possible states of endpoint */
enum ep_state { EP_STATE_NONE, EP_STATE_IDLE, EP_STATE_TRANS, EP_STATE_SETUP };

typedef struct { // __attribute__((__packed))
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
} udp_setup_data_t;

static uint8_t udp_dev_descriptor[] = {
	0x12, // bLength
	0x01, // bDescriptorType
	0x00, // bcdUSB
	0x02,
	0xff, // bDeviceClass - vendor specific
	0x00, // bDeviceSubClass - vendor specific
	0x00, // bDeviceProtocol - vendor specific
	0x40, // bMaxPacketSize0 - 64
	0xCA, // idVendor
	0xAD,
	0x08, // idProduct
	0x10,
	0x10, // bcdDevice
	0x01,
	0x01, // iManufacturer
	0x02, // iProduct
	0x03, // iSerialNumber
	0x01  // bNumConfigurations
};

static uint8_t udp_conf_descriptor[] = {
	0x09, // bLength
	0x02, // bDescriptorType
	0x20, // wTotalLength
	0x00,
	0x01, // bNumInterface
	0x01, // bConfigurationValue
	0x00, // iConfiguration
	0x80, // bmAttributes
	0xFA,  // bMaxPower
	
	/* interface */
	0x09, // bLength
	0x04, // bDescriptorType
	0x00, // bInterfaceNumber  // ??
	0x00, // bAlternateSetting
	0x02, // bNumEndpoints
	0xFF, // bInterfaceClass - vendor specific
	0x01, // bInterfaceSubClass
	0x01, // bInterfaceProtocol
	0x00,  // iInterface
	
	/* endpoint 1 - iso, in */
	0x07, // bLength
	0x05, // bDescriptorType - ep
	0x84, // bEndpointAddress - IN direction, 1st address
	0x02, // bmAttributes - 0x01 for isochronous ep, no synchronization, data
	0x00, // wMaxPacketSize - 512 bytes
	0x02,
	0x0A,  // bInterval

	/* endpoint 2 - iso, out */
	0x07, // bLength
	0x05, // bDescriptorType - ep
	0x02, // bEndpointAddress - IN direction, 1st address
	0x02, // bmAttributes - 0x01 for isochronous ep, no synchronization, data
	//			0x00, // wMaxPacketSize - 512 bytes
	//			0x02,
	0x10,
	0x00,
	0x0A,  // bInterval
};


//static uint8_t udp_int_descriptor[] = {
//0x09, // bLength
//0x04, // bDescriptorType
//0x00, // bInterfaceNumber
//0x00, // bAlternateSetting
//0x00, // bNumEndpoints
//0x02, // bInterfaceClass
//0x02, // bInterfaceSubClass
//0x01, // bInterfaceProtocol
//0x00  // iInterface
//};
//
//static uint8_t udp_ep0_descriptor[] = {
//0x07, // bLength
//0x05, // bDescriptorType - ep
//0x00, // bEndpointAddress - 0x00 for control
//0x00, // bmAttributes - 0x00 for control and not iso
//0x00, // wMaxPacketSize
//0x40,
//0x0A  // bInterval
//};

static uint8_t udp_str_zero_descriptor[] = {
	0x04,
	0x03,
	0x00,
	0x01
	//	0x04,
	//	0x09
};

static uint8_t udp_str_empty_descriptor[] = {
	0x02,
	0x03
};

static uint8_t udp_str_manufactur_descriptor[] = {
	0x1c, // bLenght
	0x03, // bDescriptorType
	'R', 0x00, 'a', 0x00, 'd', 0x00, 'i', 0x00, 'o', 0x00,
	'A', 0x00, 'v', 0x00, 'i', 0x00, 'o', 0x00, 'n', 0x00, 'i', 0x00, 'c', 0x00, 'a', 0x00
};

static uint8_t udp_str_product_descriptor[] = {
	0x1a,
	0x03,
	'A', 0x00, 'u', 0x00, 'd', 0x00, 'i', 0x00, 'o', 0x00, ' ', 0x00,
	'd', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00
};

static uint8_t udp_str_serial_descriptor[] = {
	0x0a,
	0x03,
	'0', 0x00, '.', 0x00, '0', 0x00, '1', 0x00
};

/* structure of endpoint */
typedef struct {
	/* main block */
	uint8_t type;
	uint8_t size;
	uint8_t number;
	
	enum ep_state state;

	/* buffer block */
	uint8_t *rx_buffer;
	uint32_t rx_buffer_size;
	uint32_t rx_bytes_ready;
	uint32_t rx_bank;
	
	uint8_t *tx_buffer;
	uint32_t tx_size;
	uint32_t tx_count;
	
	/* callback block */
	void (*callback)(void);
	
	uint16_t wValue;

	/* register control block */
	uint32_t interrupt_mask;
	__IO uint32_t *CSR;
	__IO uint32_t *FDR;
} udp_ep_t;

udp_ep_t ep_control;
udp_ep_t ep_in;
udp_ep_t ep_out;


typedef struct {
	enum udp_state state;
} udp_t;

udp_t _udp;


/* Main usb device port configuration */
void udp_set_interrupt(void);
void udp_ddp_pull_up(void);
void udp_set_dev_addr(uint8_t address);

/* UDP control block */
enum udp_state udp_get_state(void);
void udp_set_state(enum udp_state state);

/* Configuring process of the device. */
void udp_enumerate(const udp_setup_data_t *request);

/* Processing urb */
void udp_read(uint8_t *data);

void udp_fifo_push(udp_ep_t *ep, uint8_t value);
int udp_push(udp_ep_t *ep);

int udp_send_data(udp_ep_t *ep, uint8_t *data, uint32_t size);
int udp_send_setup(udp_ep_t *ep, uint8_t *data, uint32_t size);
int udp_send_zlp(udp_ep_t *ep);
int udp_send_stall(udp_ep_t *ep);

/* Endpoint processing functions */
void ep_init(udp_ep_t *ep, uint8_t type, uint8_t size, uint8_t number);
void ep_reset(udp_ep_t *ep, uint8_t ep_number, uint8_t ep_type, uint8_t ep_size);
void ep_control_set(udp_ep_t *ep, uint32_t mask);
void ep_control_clr(udp_ep_t *ep, uint32_t mask);
void ep_set_buffer(udp_ep_t *ep, uint8_t *buffer, uint32_t size);
void ep_set_interrupt(udp_ep_t *ep);
//void ep_set_callback(udp_ep_t *ep);
void ep_enable(udp_ep_t *ep);
void ep_disable(udp_ep_t *ep);

void ep_callback(udp_ep_t *ep);

/* Processing requests */
void udp_get_descriptor(uint16_t wValue, uint16_t wIndex, uint16_t wLength);
void udp_set_address(uint16_t wValue);
void udp_set_configuration(uint16_t wValue);

/* Callbacks for request */
void _udp_set_address_callback(void);
void _udp_set_configuration_callback(void);



#endif /* INCFILE1_H_ */