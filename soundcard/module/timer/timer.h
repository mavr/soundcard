/*
 * timer.h
 *
 * Created: 12.02.2015 18:23:14
 *  Author: ale
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include "sam.h"

#define __DEBUG_WATCHDOG_PREFIX	"wdt"
#define __DEBUG_TIMER_PREFIX	"tc"

#define __TIMER_DEBUG(lvl, msg) {\
		__DEBUG(lvl, __DEBUG_TIMER_PREFIX, msg);\
	}

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