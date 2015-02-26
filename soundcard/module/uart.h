/*
 * uart.h
 *
 * Created: 24.02.2015 15:57:48
 *  Author: ale
 */ 


#ifndef UART_H_
#define UART_H_

#include "sam4.h"

#define UART0_TX_SIZE	16
#define UART0_RX_SIZE	16
#define UART1_TX_SIZE	16
#define UART1_RX_SIZE	16


typedef struct {
	uint8_t tx_index, rx_index;
	uint8_t tx_aval, rx_aval;
	uint8_t tx[UART0_TX_SIZE];
	uint8_t	rx[UART0_RX_SIZE];
} uart_buffer_t;


void uart_system(void);

void uart_rx_enable(void);
void uart_tx_enable(void);
void uart_rx_disable(void);
void uart_tx_disable(void);

uint8_t write(uint8_t value);

uint8_t read(void);





#endif /* UART_H_ */