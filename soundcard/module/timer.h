/*
 * timer.h
 *
 * Created: 12.02.2015 18:23:14
 *  Author: ale
 */ 


#ifndef TIMER_H_
#define TIMER_H_

/* Configure timer 0 in wave mode 
	Timer use TIMER_CLOCK_1 (MCK / 2)
	Line UP with compare RA and (DOWN and RESET) with compare RC.
	The values of RA, RC : 1, 2
*/
	
//PMC->PMC_PCER0 |= (1UL << ID_TC0);
//
//timer_mode_wave_t tc0_wave_config;
//tc0_wave_config.clk = TC_CMR_TCCLKS_TIMER_CLOCK1;
//tc0_wave_config.a_action = TC_CMR_ACPA_SET;
//tc0_wave_config.c_action = TC_CMR_ACPC_CLEAR;
//tc0_wave_config.ra = 1;
//tc0_wave_config.rb = 0;
//tc0_wave_config.rc = 2;
///* PMC =) */
//PMC->PMC_PCER0 |= (1UL << ID_TC0);
//timer_configure(TC0, 0, WAVE, (void *) &tc0_wave_config);
//// TODO: delay
//for(delay = 0; delay < 15000; delay++);
//timer_start(TC0, 0);

#include "sam.h"
#include "timer.h"

typedef enum { WAVE, COUNTER } timer_mode_t;
	
typedef struct {
	uint32_t clk;
	uint32_t ra;
	uint32_t rb;
	uint32_t rc;
	uint32_t a_action;
	uint32_t b_action;
	uint32_t c_action;
} timer_mode_wave_t;

int timer_configure(Tc *timer, short channel, timer_mode_t mode, void * settings);
void timer_start(Tc *timer, short channel);
void timer_disable(Tc *timer, short channel);

void wdt_enable(void *ptr);
void wdt_disable(void);
void wdt_reset(void);


#endif /* TIMER_H_ */