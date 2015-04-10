/*
 * fifo.h
 *
 * Created: 27.02.2015 17:31:29
 *  Author: ale
 */ 


#ifndef FIFO_H_
#define FIFO_H_

typedef struct {
	// TODO : typeof
	void *buffer;
	
	uint32_t _in;
	uint32_t _out;
	
	uint32_t size;
} fifo_t;


/**
* Function puts values to fifo buffer
* Return : size of copied data
**/
uint32_t fifo_put(fifo_t *fifo, void *buffer, uint32_t size);



#endif /* FIFO_H_ */