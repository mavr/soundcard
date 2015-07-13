/*
 * stream.h
 *
 * Created: 13.07.2015 14:38:06
 *  Author: ale
 */ 


#ifndef STREAM_H_
#define STREAM_H_

#include "sam.h"

typedef struct {
	uint8_t *__buffer;
	
	volatile uint8_t __lock;
	
	// Warning! __size value must be mask type. ( ex 0001(1)b )
	uint32_t __size_msk;
	
	uint32_t __in;
	uint32_t __out;
	uint32_t __avalable;
	
} stream8_t;

void stream8_init(stream8_t *stream, uint8_t *stream_buffer, uint32_t size_buffer);

uint32_t stream8_get_avalable_data_size(stream8_t *stream);

uint8_t stream8_get(stream8_t *stream);

void stream8_put(stream8_t *stream, uint8_t value);



#endif /* STREAM_H_ */