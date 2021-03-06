/*
 * soundcard.c
 *
 * Created: 21.01.2015 15:17:23
 *  Author: ale
 */ 

#include "sam.h"
#include "init.h"
#include "include/udp.h"
#include "core/syslog.h"
#include "pio/pio.h"
#include <stdio.h>

int main(void) {
	/* Initialize the SAM system */
	Init();

	while(!udp_ready());

	__DEBUG(LOG_LVL_HIGH, __DEBUG_SYSTEM_PREFIX, "System during operation.");

	while(1) {
		// Watchdog.
	}
}
