/*
 * stream.c
 *
 * Created: 13.07.2015 14:38:22
 *  Author: ale
 */ 

#include "stream.h"

void stream8_init(stream8_t *stream, uint8_t *stream_buffer, uint32_t size_buffer) {
	stream->__in = 0;
	stream->__out = 0;
	stream->__size_msk = size_buffer - 1;
	
	stream->__buffer = stream_buffer;
}

uint32_t stream8_get_avalable_data_size(stream8_t *stream) {
	if(stream->__in == stream->__out)		return 0;
	if(stream->__in > stream->__out)		return (stream->__in - stream->__out);
	else /*stream->__in < stream->__out*/	return (stream->__in + (stream->__size_msk + 1 - stream->__out) );
}

inline void stream8_put(stream8_t *stream, uint8_t value) {
	stream->__in = (stream->__in + 1) & stream->__size_msk;
	*( stream->__buffer + stream->__in ) = value;
	
	if(stream->__in == stream->__out) {
		stream->__out = (stream->__out + 1) & stream->__size_msk;
	}
}

inline uint8_t stream8_get(stream8_t *stream) {
	uint8_t value = *( stream->__buffer + stream->__out );
	if(stream->__in != stream->__out) {
		stream->__out = (stream->__out + 1) & stream->__size_msk;
	}
	
	return value;
}
