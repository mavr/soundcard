/*
 * uart.h
 *
 * Created: 24.02.2015 15:57:48
 *  Author: ale
 */ 


#ifndef UART_H_
#define UART_H_

/* Initialiazing uart system in Init */
/* Example */
//	PMC->PMC_PCER0 |= (1UL << ID_UART0);
//	uart_system();
//	uart_tx_enable();
//	uart_rx_enable();
//	NVIC_EnableIRQ(UART0_IRQn);

#include "sam4.h"
#include "../core/fifo.h"

#define UART0_TX_SIZE	16
#define UART0_RX_SIZE	10240
//#define UART1_TX_SIZE	16
//#define UART1_RX_SIZE	20480

enum uart_write_error { UART_SEND_OK = 0, UART_SEND_PROC = 0, UART_TX_BUSY = 1, UART_WRITE_BUSY = 2 };

//uint8_t tx[UART0_TX_SIZE];
uint16_t	uart0_rx_buf[UART0_RX_SIZE];

fifo_t uart0_rx_data;

void uart_system(void);

void uart_interrupt(void);

void uart_rx_enable(void);
void uart_tx_enable(void);
void uart_rx_disable(void);
void uart_tx_disable(void);

uint8_t write(uint8_t value);
uint8_t write_16b(uint16_t value);
uint8_t write_32b(uint32_t value);

uint8_t read(void);





#endif /* UART_H_ */