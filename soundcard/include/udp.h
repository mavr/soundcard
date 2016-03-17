/*
 * udp.h
 *
 * Created: 11.03.2015 14:29:42
 *  Author: ale
 */ 


#ifndef INC_UDP_H_
#define INC_UDP_H_

#include "sam.h"
#include "udp/endpoint.h"
#include "core/syslog.h"
#include "uart/uart.h"

/* User API for usb device */

#define __DEBUG_UDP_PREFIX "[udp]\t"

#ifdef UART_DEBUG
	#define __UDP_DEBUG(lvl,msg) {\
				__DEBUG(lvl, __DEBUG_UDP_PREFIX, msg);\
			}
#else
	#define __UDP_DEBUG(lvl, msg)
#endif

//#define  __UDP_DEBUG(lvl,msg)

/* For future */
#define __UDP_ERROR

/* Endpoints */
udp_ep_setup_t ep_control;
/* Isochronous audio data streams */
udp_ep_audio_t ep_in;
udp_ep_audio_t ep_out;
/* Interrupt hid ep */
udp_ep_hid_report_t ep_int;

/* Init UDP system hardware. And turn on it. */
void udp_system(void);

/* Initializing fifo buffers for endpoints.	 */
// ?? is this needful
//void codec_stream_init(udp_ep_audio_t *ep);

/**			User API audio block			**/

/*  Return true if device connected and configured. Else - false. */
uint8_t udp_ready(void);

/**
*	In both functions return and incoming values is 16bit data
*	from/to ADC/DAC in codec's format. Nothing else.
**/
void udp_audio_stream_in(uint16_t value);
uint16_t udp_audio_stream_out(void);

#endif /* UDP_H_ */