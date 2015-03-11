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
/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void) {
	/* Initialize the SAM system */
	Init();
	
	while(1) {

	}
}



volatile uint16_t sound_tmp, count_tmp = 0;
void SSC_Handler() {
	if(SSC->SSC_SR & SSC_SR_TXRDY) {
		/*  Init codec block */
		if(ad74111.mode == AD74111_MIXED) {
			SSC->SSC_THR = (uint16_t) *( ((uint16_t *) &ad74111.registers ) + ad74111.tdata_counter / 2) | AD74111_CR_W;
			if(ad74111.tdata_counter++ == sizeof(ad74111.registers)) {
				ad74111.mode = AD74111_DATA;
				sound_tmp = 0;
			}
		} else {			
			/* Here need transmit digital data to headphone */
			sound_tmp = SSC->SSC_RHR;
			SSC->SSC_THR = sound_tmp;
		}
	}

//	if(SSC->SSC_SR & SSC_IER_RXRDY) {
//	}
}

void UART0_Handler() {
	if(UART0->UART_SR & UART_SR_RXRDY) {
		
	}
	
}