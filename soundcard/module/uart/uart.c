/*
 * uart.c
 *
 * Created: 24.02.2015 15:57:25
 *  Author: ale
 */ 

#include "sam.h"
#include "uart.h"
#include "core/syslog.h"
#include <string.h>

#include "include/error_code.h"

void uart_system() {

// Initializing serial debug port.
// This port has only transmit function.
	PMC->PMC_PCER0 |= (1UL << ID_UART0);
	/* For uart0 : */
	/* Baudrate 115200, no parity, normal mode. */
	UART0->UART_MR = UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL;
	UART0->UART_BRGR = 75;
	
	memset((void *) &serial_dbg, 0x00, sizeof(serial_dbg));
	serial_dbg.tx.buffer = uart0_tx_buffer;
	serial_dbg.THR = &(UART0->UART_THR);
	serial_dbg.RHR = &(UART0->UART_RHR);

	uart_interrupt();
	
}

void uart_interrupt() {
	UART0->UART_IER = UART_IER_TXRDY;
}

void uart_tx_enable() {
	UART0->UART_CR |= UART_CR_TXEN;
}

void uart_rx_enable() {
	UART0->UART_CR |= UART_CR_RXEN;
}

uint32_t uart_write(char *msg) {
//	syslog_send(msg);
		
	return SUCCESS;
}

void uart_writeln(char *msg) {
	uart_write(msg);
	
	//while(!(UART0->UART_SR & UART_SR_TXRDY));
	//UART0->UART_THR = 0x0d;
	//while(!(UART0->UART_SR & UART_SR_TXRDY));
	//UART0->UART_THR = 0x0a;
}

