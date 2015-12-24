/*
 * endpoint.h
 *
 * Created: 22.05.2015 18:48:42
 *  Author: ale
 */ 


#ifndef ENDPOINT_H_
#define ENDPOINT_H_

#include "sam.h"

#include "udp/udp_stream.h"

/* Endpoint addresses */
#define UDP_EP_CONTROL		0
#define UDP_EP_HID			2
#define UDP_EP_IN			4
#define UDP_EP_OUT			5

/* Endpoint types */
#define UDP_EP_TYPE_CONTROL		0
#define UDP_EP_TYPE_BULK_IN		1
#define UDP_EP_TYPE_BULK_OUT	2
#define UDP_EP_TYPE_INT			3
#define UDP_EP_TYPE_ISO_IN		4
#define UDP_EP_TYPE_ISO_OUT		5

/* Endpoint size */
#define UDP_EP0_SIZE			64
#define UDP_EP1_SIZE			64
#define UDP_EP4_SIZE			1024
#define UDP_EP5_SIZE			1024

#define UDP_EP_CTRL0_SIZE		UDP_EP0_SIZE

/* Endpoint buffers */
#define EP_CTRL_BUFFER_SIZE		4
#define EP_AUDIO_BUFFER_SIZE	1024

/* Possible states of endpoint */
enum ep_state { EP_STATE_NONE, 
				EP_STATE_IDLE, 
				EP_STATE_TRANS, 
				EP_STATE_SETUP, // ?
				EP_STATE_ZLP,
				EP_STATE_STALL
		};
	
/* core endpoint structure  */
typedef struct {
	// TODO: is this param must be remove?
	/* There is state of endpoint.
		Possible:
			EP_STATE_NONE	- ep disable;
			EP_STATE_IDLE	- ep enable;
			EP_STATE_TRANS	- ep transmit data to the host
			EP_STATE_SETUP	- ep receive and processing data from the host
	*/
	enum ep_state state;
	
	/* type of the endpoint ( CONTROL, ISO_IN, ISO_out ) */
	uint8_t type;
	/* size of ep buffer */
	uint16_t size;
	/* number in UDP hardware interface */
	uint8_t number;
		
	/* Registers */
	__IO uint32_t *CSR;
	__IO uint32_t *FDR;
} udp_ep_core_t;

/* structure of control endpoint */
typedef struct {
	// endpoint core
	udp_ep_core_t ep;
	
	// wValue =)
	uint16_t wValue;
	
	// Inside variables for transferring setup packages.
	uint32_t tx_count;
	uint32_t tx_size;
	uint8_t *tx_buffer;
	
	/* callback for setup pkg processing */
	void (*callback)(void);
	
} udp_ep_setup_t;

typedef struct {
	// endpoint core
	udp_ep_core_t ep;
	
	// microphone / headphone stream 
	_codec_stream_t stream;
	
	//TODO: callback ?
		
} udp_ep_audio_t;

typedef struct {
	// endpoint core
	udp_ep_core_t ep;
	
	uint8_t tag;
} udp_ep_hid_report_t;

/* Buffers for endpoint stream */
uint16_t __ep_ctrl_buffer[EP_CTRL_BUFFER_SIZE];
uint16_t __ep_audio_in[EP_AUDIO_BUFFER_SIZE];
uint16_t __ep_audio_out[EP_AUDIO_BUFFER_SIZE];

/* Initializing endpoint, set mode and configurations */
void ep_init(void *ep, uint8_t type, uint16_t size, uint8_t number);

/* Hardware reset endpoint and call ep_init() function */
void ep_reset(void *ep, uint8_t ep_number, uint8_t ep_type, uint16_t ep_size);

//void ep_set_buffer(udp_ep_t *ep, uint8_t *buffer, uint32_t size);

/* Function determines type of endpoint and cast __ep pointer to current type */
//inline void __udp_translate_ep_type(void *__ep, uint8_t ep_type);

/* Turn on interrupt hardware. */
void ep_set_interrupt(udp_ep_core_t *ep);

//void ep_set_callback(udp_ep_t *ep);

/* This is manipulation of CSR register */
void __ep_ctrl_set(udp_ep_core_t *ep, uint32_t mask);
void __ep_ctrl_clr(udp_ep_core_t *ep, uint32_t mask);

/* Endpoint control */
void ep_enable(udp_ep_core_t *ep);
void ep_disable(udp_ep_core_t *ep);

#include "udp/urb.h"
/* Function return received setup package */
udp_setup_data_t ep_get_setup_pkg(udp_ep_setup_t *ep);

/* This functions processing interrupt for endpoint.
   Call this after UDP_IMR_EPXINT exist. */
void ep_callback_setup(udp_ep_setup_t *ep);
void ep_callback_hid(udp_ep_hid_report_t *ep);
void ep_callback(udp_ep_audio_t *ep);



#endif /* ENDPOINT_H_ */