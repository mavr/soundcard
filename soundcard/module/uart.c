/*
 * uart.c
 *
 * Created: 24.02.2015 15:57:25
 *  Author: ale
 */ 

#include "sam.h"
#include "uart.h"

void uart_system() {
	/* For uart0 : */
	/* Baudrate 57600 : CD = 763.(8) / 16 = 47.74 */
	UART0->UART_MR = UART_MR_PAR_MARK | UART_MR_CHMODE_NORMAL;
	UART0->UART_BRGR = 300; //361
	
}

void uart_tx_enable() {
	UART0->UART_CR |= UART_CR_TXEN;
}

void uart_rx_enable() {
	UART0->UART_CR |= UART_CR_RXEN;
}