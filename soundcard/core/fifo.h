/*
 * fifo.h
 *
 * Created: 27.02.2015 17:31:29
 *  Author: ale
 */ 


#ifndef FIFO_H_
#define FIFO_H_

#include "sam4.h"

/**
* Warning! In current version of buffer there are only values of T_fifo can be used.
* Looking current T_fifo below.
**/
// Some trouble with typedef
#define T_fifo uint16_t

typedef struct {
	T_fifo *buffer;
	
	volatile uint8_t _lock;
	
	uint32_t _in;
	uint32_t _out;
	
	uint32_t _avalable;
	
	uint32_t _size;
} fifo_t;

/**
* Function initializing a new fifo object
**/
void fifo_init(fifo_t *fifo, T_fifo *buffer, uint32_t size);


/**
* Function puts values to fifo buffer
* Return : size of copied data
**/
uint32_t fifo_put(fifo_t *fifo, T_fifo *arry, uint32_t size);

/**
* Function get values from fifo buffer
* Return : size of got data
**/
uint32_t fifo_get(fifo_t *fifo, T_fifo *arry, uint32_t size);

/**
* Function add one value to the fifo buffer
**/
void fifo_push(fifo_t *fifo, T_fifo val);

/**
* Function get one value from the fifo buffer
* val : value ( see on line above :P )
* Return error code. 0 - error.
**/
uint8_t fifo_pull(fifo_t *fifo, T_fifo *val);

/**
* Locking IO operations for current fifo buffer.
* Return : zero if fifo buffer already locking.
**/
uint8_t __fifo_lock(fifo_t *fifo);

/**
* Unlocking fifo buffer;
**/
void __fifo_unlock(fifo_t *fifo);

/**
* Return 0 if fifo lock and 1 if nolock.
**/
uint8_t __fifo_is_locking(fifo_t *fifo);

#endif /* FIFO_H_ */