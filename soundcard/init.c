/*
 * init.c
 *
 * Created: 06.02.2015 14:45:18
 *  Author: ale
 */ 

#include "sam.h"
#include "init.h"
#include "module/ssc.h"
#include "module/ad74111.h"
#include "module/timer.h"
#include "module/uart.h"
#include "udp.h"

void Init() {
	/* From system_init() */
	EFC0->EEFC_FMR = EEFC_FMR_FWS(1);
	
	pmc_system();
	pio_system();
	
	/* Configure watchdog ( disable ) */
	wdt_disable();
		
	volatile unsigned long long delay;
	
// TODO: delay
	for(delay = 0; delay < 150000; delay++);
	ssc_system();
	
	/* Configure timer 0 in wave mode 
		Timer use TIMER_CLOCK_1 (MCK / 2)
		Line UP with compare RA and (DOWN and RESET) with compare RC.
		The values of RA, RC : 1, 2
	*/
	/* PMC =) */
	PMC->PMC_PCER0 |= (1UL << ID_TC0);
	
	timer_mode_wave_t tc0_wave_config;
	tc0_wave_config.clk = TC_CMR_TCCLKS_TIMER_CLOCK1;
	tc0_wave_config.a_action = TC_CMR_ACPA_SET;
	tc0_wave_config.c_action = TC_CMR_ACPC_CLEAR;
	tc0_wave_config.ra = 1;
	tc0_wave_config.rb = 0;
	tc0_wave_config.rc = 2;
	/* PMC =) */
	PMC->PMC_PCER0 |= (1UL << ID_TC0);
	timer_configure(TC0, 0, WAVE, (void *) &tc0_wave_config);
	// TODO: delay
	for(delay = 0; delay < 15000; delay++);
	timer_start(TC0, 0);


	/* After initializing ssc interface and enable codec's clock turn on codec. */
	codec_init();
	
	NVIC_EnableIRQ(SSC_IRQn);
	
//	PMC->PMC_PCER0 |= (1UL << ID_UART0);
//	uart_system();
//	uart_tx_enable();
//	uart_rx_enable();
//	NVIC_EnableIRQ(UART0_IRQn);

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
}
