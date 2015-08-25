/*
 * soundcard.c
 *
 * Created: 21.01.2015 15:17:23
 *  Author: ale
 */ 

#include "sam.h"
#include "ssc/ssc.h"
#include "codec/ad74111.h"
#include "uart/uart.h"
#include "include/udp.h"
#include "init.h"
#include "include/system.h"
#include "core/syslog.h"

/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void) {
	/* Initialize the SAM system */
	Init();
	
	while(!udp_ready());
	
	__DEBUG(LOG_LVL_LOW, "System during operation.");

	while(1) {
		// Warning! 
		// Watchdog may not work here. Because syslog.
	}
}



