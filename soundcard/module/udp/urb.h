/*
 * urb.h
 *
 * Created: 22.05.2015 18:51:38
 *  Author: ale
 */ 


#ifndef URB_H_
#define URB_H_

#include "sam.h"
#include "udp/endpoint.h"

/* Structure of setup data package */
typedef struct { // __attribute__((__packed))
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
} udp_setup_data_t;

udp_ep_audio_t *test;


/** Transfer **/
/* Read received data into ep data stream. */
void udp_read(udp_ep_audio_t *ep, uint8_t *data);

/* Put value into internal FIFO register */
void udp_fifo_push(udp_ep_audio_t *ep, uint8_t value);
int udp_push(udp_ep_setup_t *ep);

/* Using for control endpoint for answer on setup package. */
int udp_send_setup(udp_ep_setup_t *ep, const uint8_t *data, uint32_t size);

/* This function set pointer to the buffer and call udp_push() function. */
int udp_send_data(udp_ep_setup_t *ep, uint8_t *data, uint32_t size);

/* Send zero length package. */
int udp_send_zlp(udp_ep_setup_t *ep);

/* Send error. */
int udp_send_stall(udp_ep_setup_t *ep);



#endif /* URB_H_ */