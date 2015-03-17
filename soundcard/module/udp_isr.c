/*
 * udp_isr.c
 *
 * Created: 12.03.2015 15:25:06
 *  Author: ale
 */ 

#include "sam.h"
#include "udp.h"

void UDP_Handler() {

/* Go go go from powered to default state */ 
	if(UDP->UDP_ISR & UDP_ISR_ENDBUSRES) {
		ep_reset(&ep_control, 0, UDP_EP_TYPE_CONTROL, 8);
		
		udp_set_interrupt();
		udp_ddp_pull_up();
		
		UDP->UDP_ICR |= UDP_ICR_ENDBUSRES;
//		UDP->UDP_FADDR = 0x00;
		
		return;
	}
	
	if(UDP->UDP_ISR & UDP_IMR_EP0INT) {
		ep_callback(&ep_control);
		UDP->UDP_ICR |= UDP_IMR_EP0INT;
	}
}