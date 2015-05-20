/*
 * udp.c
 *
 * Created: 11.03.2015 14:29:24
 *  Author: ale
 */ 

#include "udp.h"
#include "usb.h"

void udp_system() {
	// make power for UDP
	PMC->PMC_PCER1 |= 1 << (ID_UDP - 32); // UDP id = 34
	PMC->PMC_SCER |= PMC_SCER_UDP;
	
	udp_set_state(UDP_STATE_POWERED);
	
	// interrupt enable.
	NVIC_EnableIRQ(UDP_IRQn);
}


inline enum udp_state udp_get_state() {
// TODO:udp_get_state
// may be possible to compare with UDP_GLB_STAT ?
// UDP->UDP_GLB_STAT & 0x0003
	return _udp.state;	
}

int udp_audio_stream_in(uint16_t value) {
	if(ep_in.tx_count == 16) return 0;
	
	
	ep_in.tx_buffer[ep_in.tx_count] = (uint8_t) (value >> 8);
	ep_in.tx_count++;
	
	ep_in.tx_buffer[ep_in.tx_count] = (uint8_t) value;
	ep_in.tx_count++;
	
	//if(ep_in.tx_count++) {
		//ep_control_set(&ep_in, UDP_CSR_TXPKTRDY);
		//ep_control_clr(&ep_in, UDP_CSR_TXCOMP);
		//count = 0;
	//}
			
	return;
}

uint16_t udp_audio_stream_out() {
	static uint16_t sound_tmp = 0;
	return (sound_tmp++ & 0x0fff) | 0x00ff ;
}
