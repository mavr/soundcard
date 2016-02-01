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

#include <string.h>

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
	PIOA->PIO_SCDR |= 0x6e00000;
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
	NVIC_SetPriority(PIOA_IRQn, 0x01);
	NVIC_EnableIRQ(PIOA_IRQn);
}

kbd_t kbd = {
		.key = {
					{ .pin = KBD_BUTTON_MEDIC,	.code = KBD_BUTTON_MEDIC_CODE, .label = "Medic" },
					{ .pin = KBD_BUTTON_TRANS,	.code = KBD_BUTTON_TRANS_CODE, .label = "Transmit" },
					{ .pin = KBD_BUTTON_FUNC,	.code = KBD_BUTTON_FUNC_CODE, .label = "Function" },
					{ .pin = KBD_BUTTON_PTT,	.code = KBD_BUTTON_PTT_CODE, .label = "PTT" },
					{ .pin = KBD_BUTTON_ABN,	.code = KBD_BUTTON_ABN_CODE, .label = "Abonent" },
				}
};

void PIOA_Handler() {
	uint8_t report_pkg[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t i, counter = 0;
	
	static int lock = 0;
	if(lock++ == 1) return;
	
	// need for clear interrupt
	volatile uint32_t temp = PIOA->PIO_ISR;

	for(i = 0; i < 5; i++) {
		if((PIOA->PIO_PDSR & (1 << kbd.key[i].pin)) == 0) {
			report_pkg[counter++] = kbd.key[i].code;
			__KBD_DEBUG(kbd.key[i].label);
		}
	}
	
//	for(i = counter; i < 6; i++) 
//		report_pkg[i] = 0;
		
	/* Ctrl, Alt, Shift etc */
	UDP->UDP_FDR[2] = 0x00;
	/* Reserved byte */
	UDP->UDP_FDR[2] = 0x00;
	
	/* Buttons */
	for(int i = 0; i < 5; i++) {
		//ep_int.ep.FDR = report_pkg[i];
		UDP->UDP_FDR[2] = report_pkg[i];
	}
		
	while((UDP->UDP_CSR[2] & UDP_CSR_TXPKTRDY) != 0);
	__ep_ctrl_set(&ep_int.ep, UDP_CSR_TXPKTRDY);
	
	lock = 0;

}
