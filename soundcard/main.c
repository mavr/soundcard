/*
 * soundcard.c
 *
 * Created: 21.01.2015 15:17:23
 *  Author: ale
 */ 

#include "sam.h"
#include "module/ssc.h"
#include "module/ad74111.h"
#include "init.h"

/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void) {
	/* Initialize the SAM system */
	Init();
	
	while(1) {
		PIOA->PIO_SODR |= PIO_SODR_P1;
		PIOA->PIO_CODR |= PIO_SODR_P1;
		//if(UART0->UART_SR & UART_SR_ENDTX) 
			//UART0->UART_THR = 0xFF;
//			TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
	}
}



volatile uint32_t sound_tmp;
void SSC_Handler() {
	static long counter_t, counter_r;
	uint16_t tmp;
	if(SSC->SSC_SR & SSC_SR_TXRDY) {
		/*  Init codec block */
		if(ad74111.mode == AD74111_MIXED) {
			SSC->SSC_THR = (uint16_t) *( ((uint16_t *) &ad74111.registers ) + ad74111.tdata_counter / 2) | AD74111_CR_W;
			if(ad74111.tdata_counter++ == sizeof(ad74111.registers)) {
				ad74111.mode = AD74111_DATA;
				counter_r = 0;
				counter_t = 0;
			}
		} else {
			counter_t++;
			
			/* Here need transmit digital data to headphone */
			ad74111.tdata_counter = ((ad74111.tdata_counter + 1) & 0x0FFF) | 0x001F;
//			SSC->SSC_THR = ad74111.tdata_counter++ & 0x0FFF;
			sound_tmp = SSC->SSC_RHR;
			SSC->SSC_THR = sound_tmp;
		}
	}

	//if(SSC->SSC_SR & SSC_IER_RXRDY) {
		//counter_r++;
		//if(ad74111.mode == AD74111_DATA) {
			//sound_tmp = SSC->SSC_RHR;
			//if(sound_tmp != 0x00) SSC->SSC_THR = sound_tmp;
			////SSC->SSC_THR = sound_tmp;
		//}
	//}
}

void PIOC_Handler() {
	
	
}