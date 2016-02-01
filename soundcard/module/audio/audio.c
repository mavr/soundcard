/*
 * audio.c
 *
 * Created: 29.01.2016 16:07:40
 *  Author: ale
 */ 

#include "audio.h"
#include "ssc/ssc.h"
#include "spi/spi.h"
#include "codec/pcm3793.h"

int audio_system() {
	/* Turn on spi for control pcm device. */
	spi_system();
	
	/* Turn on ssc interface for transmit audio data to and from pcm device. */
	ssc_system();
	
	/* Configuring default codec state. */
	codec.mixer.volume.max = AUDIO_MIX_VOL_MAX;
	codec.mixer.volume.min = AUDIO_MIX_VOL_MIN;
	codec.mixer.volume.res = AUDIO_MIX_VOL_RES;
	codec.mixer.volume.cur = AUDIO_MIX_VOL_CUR_DEFAULT;
	
	/* pcm.. wake up guy. */
	pcm3793_init();
	
	/* Starting ssc line for transmit playback and capture interfaces. */
	ssc_irq();
	
	return 0;
}

void audio_volume_up(void) {
	codec.mixer.volume.cur += codec.mixer.volume.res;
}

void audio_volume_down(void) {
	codec.mixer.volume.cur -= codec.mixer.volume.res;
}

uint16_t audio_get_current(void) {
	return codec.mixer.volume.cur;
}

void audio_set_current(uint16_t value) {
	codec.mixer.volume.cur = value;
}
