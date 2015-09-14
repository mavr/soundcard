/*
 * pio.c
 *
 * Created: 06.02.2015 14:58:46
 *  Author: ale
 */ 

#include "sam.h"
#include "pio.h"
#include "ssc/ssc.h"
#include "spi/spi.h"

void pio_system() {
	PMC->PMC_PCER0 |= (1UL << ID_PIOA);
	
	/* Timer 0 pin which run in wave mode. Using peripheral B ( TIOA0 ) */
	PIOA->PIO_WPMR = PIO_WPMR_WPKEY_PASSWD; // | PIO_WPMR_WPEN;
	PIOA->PIO_PER |= PIO_PA0;
	PIOA->PIO_OER |= PIO_PA0;
	PIOA->PIO_PUDR |= PIO_PA0;
	PIOA->PIO_IDR |= PIO_IDR_P0;
	PIOA->PIO_ABCDSR[0] |= PIO_PA0;
	PIOA->PIO_ABCDSR[1] &= ~PIO_PA0;
	PIOA->PIO_PDR |= PIO_PDR_P0;
	

	/* Configure SSC interface */
	RESET_LINE->PIO_PER |= RESET_PIN;
	RESET_LINE->PIO_OER |= RESET_PIN;
	PIOA->PIO_PDR |= PIO_PA17;
	
	PIOA->PIO_PER |= PIO_PA1;
	PIOA->PIO_OER |= PIO_PA1;
	PIOA->PIO_SODR |= PIO_PA1;
	
	PIOA->PIO_WPMR = PIO_WPMR_WPKEY_PASSWD;
	PIOA->PIO_PDR |= SSC_PIO_Msk;
	PIOA->PIO_ODR |= SSC_PIO_Msk;
	PIOA->PIO_IFDR |= SSC_PIO_Msk;
	PIOA->PIO_CODR |= SSC_PIO_Msk;
	PIOA->PIO_IDR |= SSC_PIO_Msk;
	PIOA->PIO_MDDR |= SSC_PIO_Msk;
	PIOA->PIO_PUER |= SSC_PIO_Msk;
	PIOA->PIO_PPDDR |= SSC_PIO_Msk;
	PIOA->PIO_OWDR |= SSC_PIO_Msk;
	
	/* UART0 interface (PA9, PA10) */
	//PIOA->PIO_OER |= PIO_PA9 | PIO_PA10;
	PIOA->PIO_PDR |= PIO_PA9 | PIO_PA10;
	
	//PMC->PMC_PCER0 |= (1UL << ID_PIOC);
	//PIOC->PIO_PER |= PIO_PC24;
	//PIOC->PIO_IER |= PIO_PC24;
	//NVIC_EnableIRQ(PIOC_IRQn);
	
	
	/*
	*	PA11, PA12(x), PA13, PA14
	*/
	spi_pio();
}
