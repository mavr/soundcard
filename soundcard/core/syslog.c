/*
 * syslog.c
 *
 * Created: 23.06.2015 13:42:16
 *  Author: ale
 */ 

#include "syslog.h"
#include "uart/uart.h"
#include <string.h>

void syslog_start(char *msg) {
	uart_system();
	uart_tx_enable();
	
	stream8_init(&syslog.stream, _syslog_buffer, SYSLOG_BUFFER_SIZE);
	
	// Set the lowest priority for uart0 irq.
	// see NVIC_IPR register in sam4s manual for more information.
	
	syslog_send(msg);
	
	NVIC_SetPriority(UART0_IRQn, 0x0f);
	NVIC_EnableIRQ(UART0_IRQn);
}

void syslog_send(char *msg) {
	uint32_t i;
	for(i = 0; i < strlen(msg); i++) {
		stream8_put(&syslog.stream, *(msg + i));
	}
	
	stream8_put(&syslog.stream, 0x0d);
	stream8_put(&syslog.stream, 0x0a);
}

void syslog_prefix(char *msg) {
	uint32_t i;
	for(i = 0; i < strlen(msg); i++) {
		stream8_put(&syslog.stream, *(msg + i));
	}
}

void UART0_Handler() {
	if(UART0->UART_SR & UART_SR_TXRDY) {
		if(stream8_get_avalable_data_size(&syslog.stream))
			*serial_dbg.THR = stream8_get(&syslog.stream);
	}
}