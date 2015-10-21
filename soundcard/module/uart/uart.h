/*
 * uart.h
 *
 * Created: 24.02.2015 15:57:48
 *  Author: ale
 */ 


#ifndef UART_H_
#define UART_H_

#include "sam.h"

#define UART0_TX_SIZE	128
#define UART0_RX_SIZE	16
//#define UART1_TX_SIZE	16
//#define UART1_RX_SIZE	20480

enum uart_write_error { UART_SEND_OK = 0, UART_SEND_PROC = 1, UART_TX_BUSY = 2, UART_WRITE_BUSY = 3 };
	
typedef struct {
	uint8_t *buffer;
	// Following values must be less than buffer size
	uint32_t counter;
} uart_buffer_t;

typedef struct {
	uint8_t lock;
	
	// hardware IO registers.
	__O  uint32_t *THR;
	__I  uint32_t *RHR;
	
	uart_buffer_t tx;
	uart_buffer_t rx;
	
} uart_t;

uint8_t uart0_tx_buffer[UART0_TX_SIZE];

uart_t serial_dbg;

void uart_system(void);

void uart_interrupt(void);
void uart_tx_isr_en(void);
void uart_tx_isr_dis(void);

void uart_rx_enable(void);
void uart_tx_enable(void);
void uart_rx_disable(void);
void uart_tx_disable(void);

uint32_t uart_write(char *msg);
void uart_writeln(char *msg);

uint8_t write_16b(uint16_t value);
uint8_t write_32b(uint32_t value);

uint8_t read(void);

#endif /* UART_H_ */