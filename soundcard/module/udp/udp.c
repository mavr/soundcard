/*
 * udp.c
 *
 * Created: 11.03.2015 14:29:24
 *  Author: ale
 */ 

#include "include/udp.h"
#include "udp/udp.h"
#include "udp/usb.h"

void udp_system() {
	// make power for UDP
	PMC->PMC_PCER1 |= 1 << (ID_UDP - 32); // UDP id = 34
	PMC->PMC_SCER |= PMC_SCER_UDP;
	
	udp_set_state(UDP_STATE_POWERED);
	
	NVIC_EnableIRQ(UDP_IRQn);
	__UDP_DEBUG(LOG_LVL_LOW, "Started.");
}

void udp_set_interrupt() {
//	UDP->UDP_IER = UDP_IER_SOFINT | UDP_IER_RXSUSP;
	UDP->UDP_IER = UDP_IER_RXSUSP | UDP_IER_EP0INT | UDP_IER_EP4INT 
					| UDP_IER_EP5INT | UDP_IER_SOFINT;
}

void udp_ddp_pull_up() {
	UDP->UDP_TXVC =	UDP_TXVC_PUON;
}

//void udp_set_dev_addr(uint8_t address) {}

void udp_set_state(udp_state state) {
	#ifdef UART_DEBUG
		switch(state) {
			case UDP_STATE_ATTACHED :		__UDP_DEBUG(LOG_LVL_LOW, "State -> attached."); break;
			case UDP_STATE_POWERED :		__UDP_DEBUG(LOG_LVL_LOW, "State -> powered."); break;
			case UDP_STATE_DEFAULT :		__UDP_DEBUG(LOG_LVL_LOW, "State -> default."); break;
			case UDP_STATE_ADDRESS :		__UDP_DEBUG(LOG_LVL_LOW, "State -> address."); break;
			case UDP_STATE_CONFIGURED :		__UDP_DEBUG(LOG_LVL_LOW, "State -> configure."); break;
			case UDP_STATE_SUSPENDED :		__UDP_DEBUG(LOG_LVL_LOW, "State -> suspend."); break;
			default:	__UDP_DEBUG(LOG_LVL_LOW, "Error! State -> uknown."); break;
		}
	#endif
	
	// TODO: I dont know what about suspend mode and remote wake up control
	uint32_t reg = UDP->UDP_GLB_STAT & 0xfffc;
	switch(state) {
		case UDP_STATE_ADDRESS : reg |= UDP_GLB_STAT_FADDEN; break;
		case UDP_STATE_CONFIGURED : reg |= UDP_GLB_STAT_CONFG; break;
		default: break;
	};
	
	UDP->UDP_GLB_STAT = reg;
	_udp.state = state;
}

inline udp_state udp_get_state() {
	return _udp.state;
}

inline uint8_t udp_ready() {
	return (udp_get_state() == UDP_STATE_CONFIGURED);
}

void UDP_Handler() {

	/* Go go go from powered to default state */
	if(UDP->UDP_ISR & UDP_ISR_ENDBUSRES) {
		ep_reset(&ep_control, UDP_EP_CONTROL, UDP_EP_TYPE_CONTROL, UDP_EP0_SIZE);
		ep_reset(&ep_in, UDP_EP_IN, UDP_EP_TYPE_ISO_IN, UDP_EP4_SIZE);
		ep_reset(&ep_out, UDP_EP_OUT, UDP_EP_TYPE_ISO_OUT, UDP_EP4_SIZE);
		
		udp_set_interrupt();
		udp_ddp_pull_up();
		__UDP_DEBUG(LOG_LVL_LOW, "Get Reset signal");
		
		UDP->UDP_ICR |= UDP_ICR_ENDBUSRES | UDP_ICR_SOFINT;
		udp_set_state(UDP_STATE_DEFAULT);
		
		return;
	}
	
	if(UDP->UDP_ISR & UDP_IMR_EP0INT) {
		ep_callback_setup(&ep_control);
		UDP->UDP_ICR |= UDP_IMR_EP0INT;
	}
	
	if(UDP->UDP_ISR & UDP_IMR_EP4INT) {
		ep_callback(&ep_in);
		UDP->UDP_ICR |= UDP_IMR_EP4INT;
	}
	
	if(UDP->UDP_ISR & UDP_IMR_EP5INT) {
		ep_callback(&ep_out);
		UDP->UDP_ICR |= UDP_IMR_EP5INT;
	}
	
	if(UDP->UDP_ISR & (UDP_ISR_EXTRSM | UDP_ISR_RXRSM | UDP_ISR_RXSUSP | UDP_ISR_SOFINT | UDP_ISR_WAKEUP)) {
		UDP->UDP_ICR |= UDP_ISR_EXTRSM | UDP_ISR_RXRSM | UDP_ISR_RXSUSP | UDP_ISR_SOFINT | UDP_ISR_WAKEUP;
		UDP->UDP_ICR |= UDP_ICR_SOFINT;
	}
}