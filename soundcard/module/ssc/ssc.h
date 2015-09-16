/*
 * ssc.h
 *
 * Created: 21.01.2015 17:09:34
 *  Author: ale
 */ 

#include <sam.h>

#ifndef SSC_H_
#define SSC_H_

#define RESET_LINE	PIOA
#define RESET_PIN	PIO_PA14
#define SSC_PIO_Msk (PIO_PA15 | PIO_PA16 | PIO_PA17 | PIO_PA18 | PIO_PA19 | PIO_PA20)


/*	Choose once from SSC_MASTER_MODE / SSC_SLAVE_MODE */
#define SSC_SLAVE_MODE

/* comment this defines if transmit or receive operation never use. */
#define SSC_TX_ENABLE
#define SSC_RX_ENABLE
#define SSC_CLOCK_DIV	4095

typedef struct {
	uint16_t *rx_buffer;
} ssc_t;

ssc_t ssc_interface;

/**
	Synchronous Serial Controller initialization.
	Device configuring by defines.
**/
void ssc_system(void);

void ssc_irq(void);
void ssc_noirq(void);

/**
	Function start ssc interface.
**/
//void ssc_start(void);
//void ssc_reset(void);

void ssc_rx_enable(void);
void ssc_tx_enable(void);
void ssc_rx_disable(void);
void ssc_tx_disable(void);

void ssc_int_enable(void);



#endif /* SSC_H_ */