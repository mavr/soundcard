/*
 * audio.h
 *
 * Created: 05.02.2016 14:25:35
 *  Author: ale
 */ 


#ifndef UDP_CLASS_AUDIO_H_
#define UDP_CLASS_AUDIO_H_

#include <sam.h>
#include "udp/urb.h"

#define UDP_AUDIO_FEATURE_UNIT_CS(value) ( (uint8_t) (value >> 8) )

#define UDP_AUDIO_CS_FU_CONTROL_UNDEFINED		0x00
#define UDP_AUDIO_CS_MUTE_CONTROL				0x01
#define UDP_AUDIO_CS_VOLUME_CONTROL				0x02
#define UDP_AUDIO_CS_BASS_CONTROL				0x03
#define UDP_AUDIO_CS_MID_CONTROL				0x04
#define UDP_AUDIO_CS_TREBLE_CONTROL				0x05
#define UDP_AUDIO_CS_GRAPHIC_EQUALIZER_CONTROL	0x06
#define UDP_AUDIO_CS_AUTOMATIC_GAIN_CONTROL		0x07
#define UDP_AUDIO_CS_DELAY_CONTROL				0x08
#define UDP_AUDIO_CS_BASS_BOOST_CONTROL			0x09
#define UDP_AUDIO_CS_LOUDNESS_CONTROL			0x0a

/* Routing requests for different control selectors. */
void udp_audio_controllers_proc(udp_setup_activity_t *udp_setup_pkg);

/* Feature Unit. */
void udp_audio_fu_volume_cs(udp_setup_activity_t *udp_setup_pkg);
void udp_audio_fu_mute_cs(udp_setup_activity_t *udp_setup_pkg);

/** Callbacks **/
void _udp_ac_set_res(void);

void _udp_ac_set_cur(udp_request_callback_t *);





#endif /* AUDIO_H_ */