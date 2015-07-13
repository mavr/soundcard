/*
 * syslog.h
 *
 * Created: 23.06.2015 13:42:01
 *  Author: ale
 */ 


#ifndef SYSLOG_H_
#define SYSLOG_H_

#include <sam.h>
#include "core/stream.h"

// 1k syslog buffer
#define SYSLOG_BUFFER_SIZE 0x400

/* Array of the possible levels of logging. */
#define LOG_LVL_LOW		0x01
#define LOG_LVL_MED		0x02
#define LOG_LVL_HIGH	0x03

/* Define here one of the log level or comment this. */
#define UART_DEBUG			LOG_LVL_HIGH

#ifdef UART_DEBUG
	#define __DEBUG(lvl,msg) if( lvl <= UART_DEBUG ) syslog_send(msg);
#else
	#define DEBUG(lvl,msg) 
#endif

/** Just syslog_t **/
typedef struct {
	stream8_t stream;
} syslog_t;

syslog_t syslog;

uint8_t _syslog_buffer[SYSLOG_BUFFER_SIZE];

/**
* Warning!
* Syslog provided by uart0 port and always run in interrupt.
* In order not to be interfere with primary system, syslog has 
* the lowest priority in irq system. 
* As result as in case main system want to send any message, it 
* will be processed and transfer when syslog take his time slice.
**/			
void syslog_start(char *msg);

void syslog_send(char *msg);

void syslog_prefix(char *msg);


#endif /* SYSLOG_H_ */