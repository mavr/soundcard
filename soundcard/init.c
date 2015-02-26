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
	
}

/* power management controller */
void pmc_system() {
//	system_init_flash(12000000);
	PMC->PMC_WPMR = 0x504D43;
	
	PMC->CKGR_MOR = CKGR_MOR_MOSCXTEN | CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCSEL;
	while(!(PMC->PMC_SR & PMC_SR_MOSCXTS));
//	PMC->CKGR_MOR |= CKGR_MOR_MOSCSEL | CKGR_MOR_KEY_PASSWD;
//	while(!(PMC->PMC_SR & PMC_SR_MOSCSELS));
	
	//PMC->CKGR_PLLAR = CKGR_PLLAR_PLLACOUNT(0) | CKGR_PLLAR_ONE;
	//PMC->CKGR_PLLAR = CKGR_PLLAR_PLLACOUNT(1) | CKGR_PLLAR_ONE | CKGR_PLLAR_DIVA(1) | CKGR_PLLAR_MULA(1);
	//while(!(PMC->PMC_SR & PMC_SR_LOCKA));
	//
	//PMC->CKGR_PLLBR = CKGR_PLLBR_PLLBCOUNT(0);
	//PMC->CKGR_PLLBR = CKGR_PLLBR_PLLBCOUNT(1) | CKGR_PLLBR_DIVB(1) | CKGR_PLLBR_MULB(1);
	//while(!(PMC->PMC_SR & PMC_SR_LOCKB));
	
//	PMC->PMC_MCKR = PMC_MCKR_CSS_PLLA_CLK;
//	SystemCoreClock = 60000000;
	//while()

	//PMC->CKGR_PLLAR = CKGR_PLLAR_PLLACOUNT(0) | CKGR_PLLAR_ONE;
	//PMC->CKGR_PLLAR = CKGR_PLLAR_PLLACOUNT(10) | CKGR_PLLAR_ONE | CKGR_PLLAR_DIVA(1) |  CKGR_PLLAR_MULA(20);
	//PMC->CKGR_PLLBR = CKGR_PLLBR_DIVB(0); // | CKGR_PLLAR_ONE;
	//PMC->CKGR_PLLBR = CKGR_PLLBR_PLLBCOUNT(10) | CKGR_PLLBR_DIVB(1) | CKGR_PLLBR_MULB(20);
	//PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) | PMC_MCKR_CSS_PLLB_CLK;
	
	PMC->CKGR_PLLBR = CKGR_PLLBR_MULB(0);
//	while(!(PMC->PMC_SR & PMC_SR_LOCKB));
	PMC->CKGR_PLLBR = CKGR_PLLBR_PLLBCOUNT(10) | CKGR_PLLBR_DIVB(10) | CKGR_PLLBR_MULB(40);
	while(!(PMC->PMC_SR & PMC_SR_LOCKB));
	
	PMC->PMC_MCKR = PMC_MCKR_CSS_PLLB_CLK;
	while(!(PMC->PMC_SR & PMC_SR_MCKRDY));
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
