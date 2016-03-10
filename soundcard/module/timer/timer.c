/*
 * timer.c
 *
 * Created: 12.02.2015 18:23:02
 *  Author: ale
 */ 

#include "sam.h"
#include "timer.h"
#include "core/syslog.h"

int timer_configure(Tc *timer, short channel, timer_mode_t mode, void * settings) {
	
	timer->TC_WPMR = TC_WPMR_WPKEY_PASSWD; // | TC_WPMR_WPEN;
	timer->TC_CHANNEL[channel].TC_CCR = TC_CCR_CLKDIS;
	if(mode == WAVE) {
		timer_mode_wave_t *settings_wave = settings;
		timer->TC_CHANNEL[channel].TC_CMR = settings_wave->clk | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_CPCTRG | settings_wave->a_action | settings_wave->b_action | settings_wave->c_action; 
//		timer->TC_CHANNEL[channel].TC_CMR |= settings_wave->a_action | settings_wave->b_action | settings_wave->c_action;
		timer->TC_CHANNEL[channel].TC_RA = settings_wave->ra;
		timer->TC_CHANNEL[channel].TC_RB = settings_wave->rb;
		timer->TC_CHANNEL[channel].TC_RC = settings_wave->rc;
	}	
	
	timer->TC_WPMR |= TC_WPMR_WPEN;
	
	return 0;
}

void timer_start(Tc *timer, short channel) {
	timer->TC_CHANNEL[channel].TC_CCR = TC_CCR_CLKEN;
	timer->TC_CHANNEL[channel].TC_CCR = TC_CCR_SWTRG;
}

void wdt_disable() {
	PMC->PMC_PCER0 |= (1UL << ID_WDT);
	WDT->WDT_MR |= WDT_MR_WDDIS;
	PMC->PMC_PCDR0 |= (1UL << ID_WDT);

	__TIMER_DEBUG(LOG_LVL_HIGH, "Watchdog disabled.");
}