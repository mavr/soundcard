/*
 * udp.h
 *
 * Created: 22.05.2015 19:02:45
 *  Author: ale
 */ 


#ifndef UDP_H_
#define UDP_H_

#include "sam.h"
//#include "udp/usb.h"

/* Possible states of usb device */
typedef uint8_t	udp_state;

#define UDP_STATE_ATTACHED		0x00
#define	UDP_STATE_POWERED		0x01
#define	UDP_STATE_DEFAULT		0x02
#define	UDP_STATE_ADDRESS		0x03
#define	UDP_STATE_CONFIGURED	0x04
#define	UDP_STATE_SUSPENDED		0x05

/** 
	This function there is in udp api block of the usb stack.
	You can find description of this in include/udp.h header file.

	Initializing and turn on UDP system hardware.
	
	void udp_system(void);
**/

/* Configuration usb device */
void udp_set_interrupt(void);
void udp_set_nointrrupt(void);

/* Pull up integrated resistor */
void udp_ddp_pull_up(void);

/* Set device address in udp register */
void udp_set_dev_addr(uint8_t address);

/* Set and take device state in udp register */
void udp_set_state(udp_state state);
udp_state udp_get_state(void);


#endif /* UDP_H_ */