/*
 * syslog.c
 *
 * Created: 23.06.2015 13:42:16
 *  Author: ale
 */ 

#include "syslog.h"
#include "uart/uart.h"

void syslog_uart_start(char *msg) {
	uart_system();
	uart_tx_enable();
	
	uart_write(msg);
}