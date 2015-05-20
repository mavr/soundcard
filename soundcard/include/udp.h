/*
 * udp.h
 *
 * Created: 11.03.2015 14:29:42
 *  Author: ale
 */ 


#ifndef UDP_H_
#define UDP_H_

#include "sam.h"

/* User API for usb device */

/* Possible states of usb device */
enum udp_state { 
	UDP_STATE_ATTACHED, 
	UDP_STATE_POWERED, 
	UDP_STATE_DEFAULT, 
	UDP_STATE_ADDRESS, 
	UDP_STATE_CONFIGURED, 
	UDP_STATE_SUSPENDED 
};

enum udp_state udp_get_state(void);

/* Init UDP system hardware. And turn on it. */
void udp_system(void);

/**		User API audio block 
*	In both functions return and incoming values is 16bit data
*	from/to ADC/DAC in codec's format. Nothing else.
**/
int udp_audio_stream_in(uint16_t value);
uint16_t udp_audio_stream_out(void);



#endif /* UDP_H_ */