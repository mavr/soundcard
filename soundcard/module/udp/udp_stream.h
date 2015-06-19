/*
 * cfifo.h
 *
 * Created: 22.05.2015 16:30:34
 *  Author: ale
 */ 


#ifndef CFIFO_H_
#define CFIFO_H_

#include "sam.h"
#include "../../core/fifo.h"

//#define _codec_stream_t fifo_t

typedef struct {
	uint16_t *__buffer;
	
	volatile uint8_t __lock;
	
	// Warning! __size value must be mask type. ( ex 0001(1)b )
	uint32_t __size_msk;
	
	uint32_t __in; 
	uint32_t __out;
	uint32_t __avalable; // ?
	
} _codec_stream_t;

void stream_init(_codec_stream_t *stream, uint16_t *stream_buffer, uint32_t size_buffer);

uint32_t udp_stream_get_avalable_data_size(_codec_stream_t *stream);

uint16_t stream_get(_codec_stream_t *stream);

void stream_put(_codec_stream_t *stream, uint16_t value);


#endif /* CFIFO_H_ */