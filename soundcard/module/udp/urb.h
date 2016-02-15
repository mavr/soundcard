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

enum udp_setup_activity_state {
	UDP_SETUP_ACT_IDLE,
	UDP_SETUP_ACT_PROC,
	UDP_SETUP_ACT_TRANS,
	UDP_SETUP_ACT_READY,
};

/* Structure of setup package */
typedef struct { // __attribute__((__packed))
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
} udp_setup_pkg_t;

/* Structure of callback. */
typedef struct {
	/* Point to incoming parameters. */
	void *uint8_t;
	/* Point to object struct. */
	void *object;
	/* Point to callback funcion. */
	void (*callback)(void *);
} udp_request_callback_t;

typedef struct {
	enum udp_setup_activity_state state;
	udp_setup_pkg_t pkg;
	
	/* Receive data. */
	uint8_t data[EP_CTRL_BUFFER_SIZE];
	
	/* Point to transmit data and size. */
	uint8_t *tx;
	uint16_t tx_size;
	uint16_t tx_count;
	
	/* Point to callback function. */
	void (*callback)(void *);
	/* Point to callback function argument. */
	void *__callback_arg;
	
} udp_setup_activity_t;

udp_setup_activity_t udp_setup_pkg;


/** Transfer **/
/* Read received data into ep data stream. */
void udp_read(udp_ep_audio_t *ep, uint8_t *data);

/* Put value into internal FIFO register */
void udp_fifo_push(udp_ep_audio_t *ep, uint8_t value);
int udp_push(udp_ep_setup_t *ep);

/* Function return received setup package */
udp_setup_pkg_t urb_get_setup_pkg(udp_ep_setup_t *ep);

/* Using for control endpoint for answer on setup package. */
int udp_send_setup(const uint8_t *data, uint32_t size);

/* This function set pointer to the buffer and call udp_push() function. */
int udp_send_data(udp_ep_setup_t *ep, uint8_t *data, uint32_t size);

/* Send zero length package. */
int udp_send_zlp(udp_ep_setup_t *ep);
int udp_send_setup_zlp(void);

/* Send error. */
int udp_send_stall(udp_ep_setup_t *ep);
int udp_send_setup_stall(void);



#endif /* URB_H_ */