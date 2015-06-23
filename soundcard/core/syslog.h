/*
 * syslog.h
 *
 * Created: 23.06.2015 13:42:01
 *  Author: ale
 */ 


#ifndef SYSLOG_H_
#define SYSLOG_H_

#include <sam.h>

/* Array of the possible levels of logging. */
#define LOG_LVL_LOW		0x01
#define LOG_LVL_MED		0x02
#define LOG_LVL_HIGH	0x03

/* Define here one of the log level or comment this. */
#define UART_DEBUG			LOG_LVL_HIGH

//#ifdef UART_DEBUG
	#define __DEBUG(lvl,msg) if( lvl <= UART_DEBUG ) { uart_write(msg); uart_write("\r\n"); }
//#else
//	#define DEBUG(lvl,msg) 
//#endif
			
void syslog_uart_start(char *msg);





#endif /* SYSLOG_H_ */