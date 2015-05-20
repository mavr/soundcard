/*
 * udp_isr.c
 *
 * Created: 12.03.2015 15:25:06
 *  Author: ale
 */ 

#include "sam.h"
#include "udp.h"
#include "usb.h"

void UDP_Handler() {

/* Go go go from powered to default state */ 
	if(UDP->UDP_ISR & UDP_ISR_ENDBUSRES) {
		ep_reset(&ep_control, UDP_EP_CONTROL, UDP_EP_TYPE_CONTROL, UDP_EP0_SIZE);
		ep_reset(&ep_in, UDP_EP_IN, UDP_EP_TYPE_ISO_IN, UDP_EP4_SIZE);
		ep_reset(&ep_out, UDP_EP_OUT, UDP_EP_TYPE_ISO_OUT, UDP_EP4_SIZE);
		
		udp_set_interrupt();
		udp_ddp_pull_up();
		
		UDP->UDP_ICR |= UDP_ICR_ENDBUSRES | UDP_ICR_SOFINT;
		udp_set_state(UDP_STATE_DEFAULT);
		
		return;
	}
	
	if(UDP->UDP_ISR & UDP_IMR_EP0INT) {
		ep_callback(&ep_control);
		UDP->UDP_ICR |= UDP_IMR_EP0INT;
	}
	
	if(UDP->UDP_ISR & UDP_IMR_EP4INT) {
		ep_callback(&ep_in);
		UDP->UDP_ICR |= UDP_IMR_EP1INT;
	}
	
	if(UDP->UDP_ISR & UDP_IMR_EP2INT) {
		ep_callback(&ep_out);
		UDP->UDP_ICR |= UDP_IMR_EP2INT;
	}
	
	if(UDP->UDP_ISR & (UDP_ISR_EXTRSM | UDP_ISR_RXRSM | UDP_ISR_RXSUSP | UDP_ISR_SOFINT | UDP_ISR_WAKEUP)) {
		UDP->UDP_ICR |= UDP_ISR_EXTRSM | UDP_ISR_RXRSM | UDP_ISR_RXSUSP | UDP_ISR_SOFINT | UDP_ISR_WAKEUP;
		UDP->UDP_ICR |= UDP_ICR_SOFINT;
	}
}