/*
 * init.c
 *
 * Created: 06.02.2015 14:45:18
 *  Author: ale
 */ 

#include "sam.h"
#include "init.h"

// modules
#include "ssc/ssc.h"
#include "codec/ad74111.h"
#include "timer/timer.h"
#include "core/syslog.h"
#include "pio/pio.h"
#include "uart/uart.h"

#include "include/udp.h"


void Init() {
	/* First configuring Flash Mode register.
		Set number of wait states for read and write operations. (1) */
	EFC0->EEFC_FMR = EEFC_FMR_FWS(1);
	
	/*	Configuring clock and ports	*/
	pmc_system();
	pio_system();
	
	/* Starting syslog system via uart port. And dont't forgive draw logo :) */
	syslog_start("\r\nRadioAvionica. Usb soundcard device starting. Syslog system.\r\n");
	
	/* Configure watchdog ( disable ) */
	wdt_disable();
		
	ssc_system();
	
	/* After initializing ssc interface and enable codec's clock turn on codec. */
	codec_init();
	ssc_irq();

	/* Starting usb system. */	
	udp_system();
}

/* power management controller */
void pmc_system() {
	system_init_flash(120000000);
	PMC->PMC_WPMR = 0x504D43;
	
	PMC->CKGR_MOR = CKGR_MOR_MOSCXTEN | CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCEN;
	while(!(PMC->PMC_SR & PMC_SR_MOSCXTS));
//	PMC->CKGR_MOR = CKGR_MOR_MOSCXTBY | CKGR_MOR_MOSCXTEN;
	PMC->CKGR_MOR |= CKGR_MOR_MOSCSEL | CKGR_MOR_KEY_PASSWD ;
	while(!(PMC->PMC_SR & PMC_SR_MOSCSELS)); 
//	PMC->CKGR_MOR = CKGR_MOR_MOSCXTEN | CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCSEL;
//	while(!(PMC->PMC_SR & PMC_SR_MOSCXTS));
	
	PMC->CKGR_PLLBR = CKGR_PLLBR_MULB(0);
	PMC->CKGR_PLLBR = CKGR_PLLBR_PLLBCOUNT(50) | CKGR_PLLBR_DIVB(2) | CKGR_PLLBR_MULB(15);
	while(!(PMC->PMC_SR & PMC_SR_LOCKB));
	
	PMC->PMC_MCKR = PMC_MCKR_CSS_PLLB_CLK;
//	PMC->PMC_MCKR = PMC_MCKR_CSS_MAIN_CLK | PMC_MCKR_PRES_CLK_1;
	while(!(PMC->PMC_SR & PMC_SR_MCKRDY));
	
	PMC->CKGR_PLLAR = CKGR_PLLAR_MULA(0);
	PMC->CKGR_PLLAR = CKGR_PLLBR_PLLBCOUNT(10) | CKGR_PLLAR_DIVA(1) | CKGR_PLLAR_MULA(7) | CKGR_PLLAR_ONE; 
	
	PMC->PMC_USB = PMC_USB_USBDIV(0); // PLLA and div 1
	
	
		//pck - p13
	PIOA->PIO_IDR |= PIO_IDR_P6;
	PIOA->PIO_OER |= PIO_OER_P6;
	//uint32_t last = PIOB->PIO_ABCDSR[0];
	//PIOB->PIO_ABCDSR[0] &= (~PIO_ABCDSR_P13 & last);
	//last = PIOB->PIO_ABCDSR[1];
	//PIOB->PIO_ABCDSR[1] &= (~PIO_ABCDSR_P13 & last);
	PIOA->PIO_ABCDSR[0] |= PIO_PA6;
	PIOA->PIO_ABCDSR[1] &= ~PIO_PA6;
	PIOA->PIO_PDR |= PIO_PDR_P6;
	
	PMC->PMC_PCK[0] = PMC_PCK_CSS_PLLB_CLK | PMC_PCK_PRES_CLK_4; 
	PMC->PMC_SCER |= PMC_SCER_PCK0;
	while(!(PMC->PMC_SR & PMC_SR_PCKRDY0));
//
//	SystemCoreClockUpdate();
}