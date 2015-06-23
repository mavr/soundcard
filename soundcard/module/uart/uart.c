/*
 * uart.c
 *
 * Created: 24.02.2015 15:57:25
 *  Author: ale
 */ 

#include "sam.h"
#include "uart.h"
#include <string.h>

#include "../core/fifo.h"

void uart_system() {
	/* For uart0 : */
	/* Baudrate 115200 */
	
	PMC->PMC_PCER0 |= (1UL << ID_UART0);
	
	UART0->UART_MR = UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL;
	UART0->UART_BRGR = 26; //361
	
	fifo_init(&uart0_rx_data, uart0_rx_buf, UART0_RX_SIZE);
	
	uart_interrupt();
}

void uart_interrupt() {
	UART0->UART_IER = UART_IER_RXRDY;

}

void uart_tx_enable() {
	UART0->UART_CR |= UART_CR_TXEN;
}

void uart_rx_enable() {
	UART0->UART_CR |= UART_CR_RXEN;
}

uint8_t write_16b(uint16_t value) {
	static uint8_t busy = 0;
	
	if(busy) return UART_WRITE_BUSY;
	
	busy = 1;

// TODO : recode this shit	
	while(!(UART0->UART_SR & UART_SR_TXRDY));
	UART0->UART_THR = (uint8_t) value;
	while(!(UART0->UART_SR & UART_SR_TXRDY));
	UART0->UART_THR = value >> 8;
	
	busy = 0;
	
	return UART_SEND_OK;
}

void uart_write(char *msg) {
	uint32_t i;
	for(i = 0; i < strlen((char *) msg); i++) {
		while(!(UART0->UART_SR & UART_SR_TXRDY));
		UART0->UART_THR = *(msg + i);
	}
	
	while(!(UART0->UART_SR & UART_SR_TXRDY));
	UART0->UART_THR = 0x0d;
	while(!(UART0->UART_SR & UART_SR_TXRDY));
	UART0->UART_THR = 0x0a;
}