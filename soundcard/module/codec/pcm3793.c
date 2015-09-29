/*
 * pcm3793.c
 *
 * Created: 08.09.2015 16:30:26
 *  Author: ale
 */ 

#include "sam.h"
#include "core/syslog.h"

#include "pcm3793.h"

#include "spi/spi.h"


void pcm3793_init() {
	pcm3793_write(PCM_VOLUME_HPA_Lch, 0x35); // mute disabled; 1 dB 
	pcm3793_write(PCM_VOLUME_HPA_Rch, 0x35); // mute disabled; 1 dB 
//	pcm3793_write(PCM_VOLUME_SPA_Lch, 0x27);
//	pcm3793_write(PCM_VOLUME_SPA_Rch, 0x27);
	pcm3793_write(PCM_DAC_SOFT_MUTE_Lch, 0x3f);
	pcm3793_write(PCM_DAC_SOFT_MUTE_Rch, 0x3f);
	pcm3793_write(PCM_DAC_OVER_SAMPLING, 0x60); // left justed; 32kHz
	
	pcm3793_write(PCM_HPA, 0xc0);
	pcm3793_write(PCM_ADC_1, 0x02);   // mic
	
	pcm3793_write(PCM_MIC_BOOST, 0x13);  // mic
	pcm3793_write(PCM_DAC_SPA_HPA, 0xec);
	pcm3793_write(PCM_BCK, 0x71);
	pcm3793_write(PCM_HPA, 0x00);
	pcm3793_write(PCM_ANALOG_MIXER, 0x03);
	pcm3793_write(PCM_ANALOG_MIX_SW, 0x11); // 0x11
	pcm3793_write(PCM_DAC_SPA_HPA, 0xFC);
	pcm3793_write(PCM_SPA_SHUTDOWN, 0x03);
	
	pcm3793_write(PCM_ANALOG_OUTPUT, 0x01);
	pcm3793_write(PCM_ADC_2, 0x37);
	pcm3793_write(PCM_ANALOG_IN_SEL, 0x22); // 0x11 - ain1L / ain1R
	pcm3793_write(PCM_VOLUME_ADC_IN_Lch, 0x0c); // pg3
	pcm3793_write(PCM_VOLUME_ADC_IN_Rch, 0x0c); // pg4
	pcm3793_write(PCM_MASTER_MODE, 0x04);
	
	__DEBUG(LOG_LVL_HIGH, "[audio]\tConfigured pcm3793");
	/* wait 450 ms */
	//TODO: remade this shit
	volatile unsigned long long i = 0;
	for(i = 0; i < 100000; i++) {
		__NOP;
	}
	pcm3793_write(PCM_DAC_SPA_HPA, 0xff);
	
	__DEBUG(LOG_LVL_HIGH, "[audio]\tStarted in master mode");
	
	
	
}

void pcm3793_write(uint8_t reg, uint8_t value) {
	spi_send( ( ((uint16_t) reg) << 8 ) | value );
}