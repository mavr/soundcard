/*
 * cfifo.c
 *
 * Created: 22.05.2015 16:30:45
 *  Author: ale
 */ 

#include "include/udp.h"
#include "udp_stream.h"

void codec_stream_init(udp_ep_audio_t *ep) {
	switch(ep->ep.type) {
		case UDP_EP_TYPE_ISO_OUT:
			break;
		
		case UDP_EP_TYPE_ISO_IN:
			break;
			
		case UDP_EP_TYPE_CONTROL:
			break;
	}
}

void udp_audio_stream_in(uint16_t value) {
	// Add value to the buffer
	_codec_stream_t *stream = &(ep_in.stream);
	stream->__in = (stream->__in + 1) & stream->__size_msk;
	*( stream->__buffer + stream->__in ) = value;
	
	if(stream->__in == stream->__out) 
		stream->__out = (stream->__out + 1) & stream->__size_msk;
	
	//TODO: need to transmit to the host
	return;
}

uint16_t udp_audio_stream_out() {
	_codec_stream_t *stream = &ep_out.stream;
	
	uint16_t sound = stream->__out;
	if(stream->__in != stream->__out) {
		stream->__out = (stream->__out + 1) & stream->__size_msk;		
	}
	
	return sound;
}

void udp_stream_init(_codec_stream_t *stream, uint16_t *stream_buffer, uint32_t size_buffer) {
	stream->__in = 0;
	stream->__out = 0;
	stream->__size_msk = size_buffer - 1;
	
	stream->__buffer = stream_buffer;
	
}