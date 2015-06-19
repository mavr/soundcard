/*
 * cfifo.c
 *
 * Created: 22.05.2015 16:30:45
 *  Author: ale
 */ 

#include "include/udp.h"
#include "udp_stream.h"

void udp_audio_stream_in(uint16_t value) {
	static uint16_t count = 0;
	
	count++;
	// Add value to the buffer
	_codec_stream_t *stream = &(ep_in.stream);
	stream_put(stream, value);
	
	if(count == 16) {
		
	}
}

uint16_t udp_audio_stream_out() {
	return stream_get(&ep_out.stream);
}

void stream_init(_codec_stream_t *stream, uint16_t *stream_buffer, uint32_t size_buffer) {
	stream->__in = 0;
	stream->__out = 0;
	stream->__size_msk = size_buffer - 1;
	
	stream->__buffer = stream_buffer;
	
}

inline uint32_t udp_stream_get_avalable_data_size(_codec_stream_t *stream) {
	if(stream->__in == stream->__out)		return 0;
	if(stream->__in > stream->__out)		return (stream->__in - stream->__out);
	else /*stream->__in < stream->__out*/	return (stream->__in + (stream->__size_msk + 1 - stream->__out) );
}

inline void stream_put(_codec_stream_t *stream, uint16_t value) {
	stream->__in = (stream->__in + 1) & stream->__size_msk;
	*( stream->__buffer + stream->__in ) = value;
	
	if(stream->__in == stream->__out)
	stream->__out = (stream->__out + 1) & stream->__size_msk;
}

inline uint16_t stream_get(_codec_stream_t *stream) {
	uint16_t value = stream->__out;
	if(stream->__in != stream->__out) {
		stream->__out = (stream->__out + 1) & stream->__size_msk;
	}
	
	return value;
}