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
#include "include/udp.h"

void pio_system() {
	PMC->PMC_PCER0 |= (1UL << ID_PIOA);
	/* Configure SSC interface */
	RESET_LINE->PIO_PER |= RESET_PIN;
	RESET_LINE->PIO_OER |= RESET_PIN;
	PIOA->PIO_PDR |= PIO_PA17;
	
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
	PIOA->PIO_PDR |= PIO_PA9 | PIO_PA10;
	
	PIOA->PIO_PER |= 0x6e00000;
	PIOA->PIO_CODR |= 0x6e00000;
	PIOA->PIO_IFER |= 0x6e00000;
	PIOA->PIO_REHLSR |= 0x6e00000;
	PIOA->PIO_ODR |= 0x6e00000;
	PIOA->PIO_PUER |= 0x6e00000;
	//PIOA->PIO_PPDDR |= 0x1f;
	//PIOA->PIO_PUER |= 0x1f;
	//PIOA->PIO_REHLSR |= 0x1f;
	//PIOA->PIO_ESR |= 0x1f;
	PIOA->PIO_IER |= 0x6e00000;
	
	
	
	//PMC->PMC_PCER0 |= (1UL << ID_PIOC);
	//PIOC->PIO_PER |= PIO_PC24;
	//PIOC->PIO_IER |= PIO_PC24;
	//NVIC_EnableIRQ(PIOC_IRQn);
	
	
	/*
	*	PA11, PA12(x), PA13, PA14
	*/
	spi_pio();
}

void pio_enable_kdb() {
	NVIC_EnableIRQ(PIOA_IRQn);
}

void PIOA_Handler() {
	__PIO_DEBUG(LOG_LVL_HIGH, "interrupt");
	uint32_t temp = PIOA->PIO_ISR;
	uint8_t report[7];
	if(temp & (0x01 << 23)) {
//		memset(report, 0xaa, 7);
		__PIO_DEBUG(LOG_LVL_HIGH, "sent report");
		
		if((PIOA->PIO_PDSR & (1 << 23)) == 0) {
			UDP->UDP_FDR[2] = 0x00;
			UDP->UDP_FDR[2] = 0;
			UDP->UDP_FDR[2] = 0;
			UDP->UDP_FDR[2] = 31;
			UDP->UDP_FDR[2] = 0;
			UDP->UDP_FDR[2] = 0;
			UDP->UDP_FDR[2] = 0;
		} else {
			UDP->UDP_FDR[2] = 0x00;
			UDP->UDP_FDR[2] = 0;
			UDP->UDP_FDR[2] = 0;
			UDP->UDP_FDR[2] = 0;
			UDP->UDP_FDR[2] = 0;
			UDP->UDP_FDR[2] = 0;
			UDP->UDP_FDR[2] = 0;
		}
		
		
		__ep_ctrl_set(&ep_int.ep, UDP_CSR_TXPKTRDY);
	}
}