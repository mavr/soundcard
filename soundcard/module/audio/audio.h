/*
 * audio.h
 *
 * Created: 29.01.2016 15:56:54
 *  Author: ale
 */ 


#ifndef AUDIO_H_
#define AUDIO_H_

#include <sam.h>

/* Volume unit parameters. */ 
#define AUDIO_MIX_VOL_MAX	0x0ff0
#define AUDIO_MIX_VOL_MIN	0x00
#define AUDIO_MIX_VOL_RES	0x01
#define AUDIO_MIX_VOL_CUR_DEFAULT	0x00

typedef struct {
	uint16_t max;
	uint16_t min;
	uint16_t res;
	uint16_t cur;
} audio_unit_t;

typedef struct {
	audio_unit_t volume;
} audio_mixer_t;

/* Configuring, starting ssc, spi and pcm systems. */
int audio_system(void);

void audio_volume_up(void);
void audio_volume_down(void);

uint16_t audio_get_current(void);
void audio_set_current(uint16_t value);

#endif /* AUDIO_H_ */