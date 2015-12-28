/*
 * pio.h
 *
 * Created: 06.02.2015 14:58:31
 *  Author: ale
 */ 


#ifndef PIO_H_
#define PIO_H_

#include "core/syslog.h"

#ifdef UART_DEBUG
#define __PIO_DEBUG(lvl,msg) {\
	syslog_prefix("[pio]\t"); \
__DEBUG(lvl,msg); }
#else
#define __PIO_DEBUG(lvl,msg)
#endif

#define KBD_BUTTONE_MEDIC


void pio_system();
void pio_enable_kdb();




#endif /* PIO_H_ */