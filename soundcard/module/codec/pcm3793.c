/*
 * pcm3793.c
 *
 * Created: 08.09.2015 16:30:26
 *  Author: ale
 */ 

#include "sam.h"
//#include "i"
#include "pcm3793.h"

#include "spi/spi.h"

void pcm3793_init() {
	pcm3793_write(PCM_VOLUME_HPA_Lch, 0x27);
	pcm3793_write(PCM_VOLUME_HPA_Rch, 0x27);
	pcm3793_write(PCM_VOLUME_SPA_Lch, 0x27);
	pcm3793_write(PCM_VOLUME_SPA_Rch, 0x27);
	pcm3793_write(PCM_DAC_SOFT_MUTE_Lch, 0x27);
	pcm3793_write(PCM_DAC_SOFT_MUTE_Rch, 0x27);
	pcm3793_write(PCM_DAC_OVER_SAMPLING, 0x20);
	
	pcm3793_write(PCM_HPA, 0xc0);
	pcm3793_write(PCM_ADC_1, 0x02);
	
	pcm3793_write(PCM_MIC_BOOST, 0x10);
	pcm3793_write(PCM_DAC_SPA_HPA, 0xe0);
	pcm3793_write(PCM_BCK, 0x01);
	pcm3793_write(PCM_HPA, 0xc0);
	pcm3793_write(PCM_ANALOG_MIXER, 0x03);
	pcm3793_write(PCM_ANALOG_IN_SEL, 0x11);
	pcm3793_write(PCM_DAC_SPA_HPA, 0xFC);
	pcm3793_write(PCM_SPA_SHUTDOWN, 0x03);
	
	pcm3793_write(PCM_ANALOG_OUTPUT, 0x01);
	pcm3793_write(PCM_ADC_2, 0x3f);
	pcm3793_write(PCM_ANALOG_IN_SEL, 0x11);
	pcm3793_write(PCM_VOLUME_ADC_IN_Lch, 0x0c);
	pcm3793_write(PCM_VOLUME_ADC_IN_Rch, 0x0c);
	pcm3793_write(PCM_MASTER_MODE, 0x04);
	
	/* wait 450 ms */
	//TODO: remade this shit
	volatile unsigned long long i = 0;
	for(i = 0; i < 1000000; i++) {
		__NOP;
	}
	for(i = 0; i < 100000; i++) {
		__NOP;
	}
	pcm3793_write(PCM_DAC_SPA_HPA, 0xff);
	
	
	
}

void pcm3793_write(uint8_t reg, uint8_t value) {
	spi_send( ( ((uint16_t) reg) << 8 ) | value );
}