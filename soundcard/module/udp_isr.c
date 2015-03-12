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
		/* no understand action with ep */
		UDP->UDP_CSR[0] |= UDP_CSR_EPEDS;
		UDP->UDP_IER |= UDP_IER_EP0INT;
		
		udp_set_interrupt();
		udp_ddp_pull_up();
		
		UDP->UDP_ICR |= UDP_ICR_ENDBUSRES;
		
		return;
	}
	volatile uint8_t i;
	if((UDP->UDP_ISR & UDP_IMR_SOFINT) == 0) {
		i = 7;
	}
}