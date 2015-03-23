/*
 * udp.h
 *
 * Created: 11.03.2015 14:29:42
 *  Author: ale
 */ 


#ifndef UDP_H_
#define UDP_H_


/* Endpoint addresses */
#define UDP_EP_CONTROL		0
#define UDP_EP_IN			1
#define UDP_EP_OUT			2

/* Endpoint types */
#define UDP_EP_TYPE_CONTROL		0
#define UDP_EP_TYPE_BULK		1
#define UDP_EP_TYPE_INT			2

/* Endpoint size */
#define UDP_EP0_SIZE			64

/* Endpoit buffers */
#define UDP_EP0_RX_BUFFER_SIZE	64
#define UDP_EP0_TX_BUFFER_SIZE	64

uint8_t temp_addr;

typedef struct __attribute__((__packed)){
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;	
} udp_setup_data_t;

typedef struct {
	
} udp_t;

static uint8_t udp_dev_descriptor[] = {
	0x12, // bLength
	0x01, // bDescriptorType
	0x00, // bcdUSB
	0x02,
	0xff, // bDeviceClass - cdc
	0x00, // bDeviceSubClass - cdc
	0x00, // bDeviceProtocol - cdc
	0x40, // bMaxPacketSize0 - 64
	0xAD, // idVendor
	0xCD, 
	0x00, // idProduct
	0x08, 
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
	0x19, // wTotalLength
	0x00,
	0x01, // bNumInterface
	0x01, // bConfigurationValue
	0x00, // iConfiguration
	0x80, // bmAttributes    
	0xFA,  // bMaxPower
	
/* interface */
		0x09, // bLength
		0x04, // bDescriptorType
		0x00, // bInterfaceNumber
		0x00, // bAlternateSetting
		0x00, // bNumEndpoints
		0x02, // bInterfaceClass
		0x02, // bInterfaceSubClass
		0x01, // bInterfaceProtocol
		0x00,  // iInterface
		
/* endpoint 0 */
			0x07, // bLength
			0x05, // bDescriptorType - ep
			0x00, // bEndpointAddress - 0x00 for control
			0x00, // bmAttributes - 0x00 for control and not iso
			0x00, // wMaxPacketSize
			0x40,
			0x0A  // bInterval

};


static uint8_t udp_int_descriptor[] = {
	0x09, // bLength
	0x04, // bDescriptorType
	0x00, // bInterfaceNumber
	0x00, // bAlternateSetting
	0x00, // bNumEndpoints
	0x02, // bInterfaceClass
	0x02, // bInterfaceSubClass
	0x01, // bInterfaceProtocol
	0x00  // iInterface
};

static uint8_t udp_ep0_descriptor[] = {
	0x07, // bLength
	0x05, // bDescriptorType - ep
	0x00, // bEndpointAddress - 0x00 for control
	0x00, // bmAttributes - 0x00 for control and not iso
	0x00, // wMaxPacketSize
	0x40, 
	0x0A  // bInterval
};


enum ep_state { EP_STATE_NONE, EP_STATE_IDLE, EP_STATE_TRANS, EP_STATE_SETUP };

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
	
	uint16_t value;

/* register control block */
	uint32_t interrupt_mask;
	uint32_t *CSR;
	uint32_t *FDR;
} udp_ep_t;

udp_ep_t ep_control;
uint8_t ep0_rx_buffer[UDP_EP0_RX_BUFFER_SIZE];
uint8_t ep0_tx_buffer[UDP_EP0_TX_BUFFER_SIZE];
uint8_t debug_arr[12];

udp_ep_t ep_in;
udp_ep_t ep_out;

void udp_system(void);

/* Main usb device port configuration */
void udp_set_interrupt(void);
void udp_ddp_pull_up(void);
void udp_set_dev_addr(uint8_t address);

void udp_enumerate(const udp_setup_data_t *request);

void udp_read(uint8_t *data);
void udp_setup(udp_ep_t *ep);

int udp_push(udp_ep_t *ep);
int udp_send(udp_ep_t *ep, uint8_t *data, uint32_t size);
int udp_send_zlp(udp_ep_t *ep);
int udp_send_stall(udp_ep_t *ep);


void udp_fifo_push(udp_ep_t *ep, uint8_t value);

void udp_get_descriptor(uint16_t wValue, uint16_t wIndex, uint16_t wLength);

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



#endif /* UDP_H_ */