/*
 * udp.h
 *
 * Created: 11.03.2015 14:29:42
 *  Author: ale
 */ 


#ifndef UDP_H_
#define UDP_H_

/* Endpoint addresses */
#define UDP_EP_CONTROLL		0
#define UDP_EP_IN			1
#define UDP_EP_OUT			2

/* Endpoint types */
#define UDP_EP_TYPE_CONTROLL	0
#define UDP_EP_TYPE_BULK		1
#define UDP_EP_TYPE_INT			2

typedef struct {
	
} udp_t;

static const uint8_t udp_dev_descriptor[] = {
	0x12, // dev descriptor size in bytes
	0x01, // descriptor type
	0x00, // usb version - 2.0
	0x02,
	0x02, // class code (cdc)
	0x00, // subclass (cdc)
	0x00, // proto (cdc)
	0x08, // max size for zero endpoint
	0xAD, // vid
	0xCD, 
	0x00, // pid
	0x08, 
	0x10, // dev version
	0x01, 
	0x01, // id vendor string
	0x02, // id dev string
	0x03, // id serial number sting
	0x01  // number of posible configuration
};

/* structure of endpoint */
typedef struct {
/* main block */
	uint8_t type;
	uint8_t size;

/* buffer block */
	uint8_t *buffer;
	uint32_t buffer_size;
	uint32_t bytes_ready;
	uint32_t bank;
	
/* callback block */

/* register controll block */
	uint32_t interrupt_mask;
} udp_ep_type;

void udp_system(void);

/* Main usb device port configuration */
void udp_set_interrupt(void);
void udp_ddp_pull_up(void);

void udp_read(uint8_t *data);
void udp_write(uint8_t *data);

/* Endpoint processing functions */
void ep_reset(udp_ep_type *ep, uint8_t ep_number, uint8_t ep_type, uint8_t ep_size);
void ep_set_interrupt(udp_ep_type *ep, uint32_t interrupt_mask);


#endif /* UDP_H_ */