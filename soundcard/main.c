/*
 * soundcard.c
 *
 * Created: 21.01.2015 15:17:23
 *  Author: ale
 */ 

#include "sam.h"
#include "module/ssc.h"
#include "module/ad74111.h"
#include "module/uart.h"
#include "init.h"
#include "system.h"
#include "udp.h"
/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void) {
	/* Initialize the SAM system */

	dbg_tx_counter = 0;
	Init();
	
	uint8_t buffer[128];
	for(int j = 0, i = 0; j < sizeof(buffer); j++, i++) *(buffer + j) = i;
	while(_udp.state != UDP_STATE_CONFIGURE);
	ep_control_set(&ep_in, UDP_CSR_TXPKTRDY);
//	udp_stream_in(&ep_in, buffer, sizeof(buffer));
//	udp_stream_in(&ep_in, buffer, 35);
	
	
//	udp_stream_in(0x55aa);
	uint16_t value = 0x4445;
	while(1) {
	}
}

void SSC_Handler() {
	if(SSC->SSC_SR & SSC_SR_TXRDY){ 
		/*  Init codec block */
		if(ad74111.mode == AD74111_MIXED) {
			SSC->SSC_THR = (uint16_t) *( ((uint16_t *) &ad74111.registers ) + ad74111.tdata_counter / 2) | AD74111_CR_W;
			if(ad74111.tdata_counter++ == sizeof(ad74111.registers)) {
				ad74111.mode = AD74111_DATA;
			}
		} else {			
			/* Here need transmit digital data to headphone */
			udp_audio_stream_in(SSC->SSC_RHR);
			SSC->SSC_THR = udp_audio_stream_out();
		}
	}
}

void UART0_Handler() {
	if(UART0->UART_SR & UART_SR_RXRDY) {
		
	}
	
}
