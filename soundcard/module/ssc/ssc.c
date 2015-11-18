/*
 * ssc.c
 *
 * Created: 21.01.2015 17:09:48
 *  Author: ale
 */ 

#include "sam.h"
#include "codec/ad74111.h"
#include "ssc.h"
#include "include/udp.h"

void ssc_system() {
	
//	enable synchronous serial controller in power management controller.
	PMC->PMC_PCER0 |= (1UL << ID_SSC);
	
	SSC->SSC_WPMR = SSC_WPMR_WPKEY_PASSWD;
	
//	reset ssc control register	
	SSC->SSC_CR |= SSC_CR_SWRST;
	
#ifdef SSC_RX_ENABLE
	SSC->SSC_CR |= SSC_CR_RXEN;
#endif
#ifdef SSC_TX_ENABLE
	SSC->SSC_CR |= SSC_CR_TXEN;
#endif
//	and turn off receive and transmit operations
//	SSC->SSC_CR |= SSC_CR_RXDIS | SSC_CR_TXDIS;

//	set divider value for master clock
//	SSC->SSC_CMR = SSC_CMR_DIV(SSC_CLOCK_DIV);

	
#ifdef SSC_RX_ENABLE
/*	configure receive clock mode */
//	master clock and continuous mode
//	SSC->SSC_RCMR =	 SSC_RCMR_CKS_MCK | SSC_RCMR_CKO_CONTINUOUS;
//	rising edge
//	SSC->SSC_RCMR |= SSC_RCMR_CKI;
//	receive frame synchro at low
//	SSC->SSC_RCMR |= SSC_RCMR_CKG_EN_RF_LOW;
#endif


/* Disable clock driver for slave mode */
	SSC->SSC_CMR |= SSC_CMR_DIV(0);

	
	SSC->SSC_RCMR = SSC_RCMR_CKS_TK | SSC_RCMR_CKO_NONE | SSC_RCMR_CKG_EN_RF_HIGH | SSC_RCMR_START_RF_RISING ;
	SSC->SSC_RFMR = SSC_RFMR_DATLEN(15) | SSC_RFMR_MSBF ;
	
	SSC->SSC_TCMR = SSC_TCMR_CKS_TK | SSC_TCMR_CKO_NONE | SSC_TCMR_CKG_EN_TF_LOW | SSC_TCMR_START_TF_FALLING ;
	SSC->SSC_TFMR = SSC_TFMR_DATLEN(15) | SSC_TFMR_MSBF;


	PDC_SSC->PERIPH_PTCR |= PERIPH_PTCR_RXTEN | PERIPH_PTCR_TXTEN;
//	PDC_SSC->PERIPH_PTSR |= PERIPH_PTSR_RXTEN | PERIPH_PTCR_TXTEN;
	SSC->SSC_CR |= SSC_CR_RXEN | SSC_CR_TXEN;

	ssc_int_enable();
	
	__DEBUG(LOG_LVL_HIGH, "[ssc]\tConfigured");
//	codec_reset();
}

inline void ssc_irq() { 
	NVIC_SetPriority(SSC_IRQn, 0x00);
	NVIC_EnableIRQ(SSC_IRQn);
	__DEBUG(LOG_LVL_HIGH, "[ssc]\tStarted");
}

inline void ssc_noirq() {
	NVIC_DisableIRQ(SSC_IRQn);
}

inline void ssc_tx_enable() {
	SSC->SSC_CR &= ~(SSC_CR_TXDIS);
}

void ssc_rx_enable() {
	SSC->SSC_CR &= ~(SSC_CR_RXDIS);
}

void ssc_tx_disable() {
	SSC->SSC_CR |= SSC_CR_TXDIS;
}

void ssc_rx_disable() {
	SSC->SSC_CR |= SSC_CR_RXDIS;
}

//void ssc_reset() {
//
//}

void ssc_int_enable() {	
	SSC->SSC_IER = SSC_IER_RXRDY ;
}

void SSC_Handler() {
	static volatile uint16_t tmp;
	static uint16_t sound = 0x00;
	//if(SSC->SSC_SR & SSC_SR_TXRDY) {
		//udp_audio_stream_in(SSC->SSC_RHR);
	//}
	
	if(SSC->SSC_SR & SSC_SR_RXRDY) {
		
		/* Here need transmit digital data to headphone */
		tmp = SSC->SSC_RHR;
//		sound = (sound | 0xff3) + 1;
//		SSC->SSC_THR = 0x5555;
		//udp_audio_stream_in((tmp >> 8) | (tmp << 8));
		uint16_t tmp_out = udp_audio_stream_out();
		SSC->SSC_THR = tmp_out;
		
		udp_audio_stream_in(tmp);
	
	}
	
//	if(SSC->SSC_SR & SSC_SR_TXRDY) {
//		SSC->SSC_THR = stream_get(stream);
//		SSC->SSC_THR = tmp;	
//	}
}