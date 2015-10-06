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
	/* Set analog volume for right Tel. channel
		and disable analog mute. */
//	pcm3793_hpl(0x23);
	pcm3793_hpr(PCM_R65_HRV(0x3f));
	
	/* SPL/R disable */
	
	/* DAC block */
	pcm3793_da_r(PCM_R69_ATR(0x27));
	pcm3793_dac_set_format(PCM_R70_PFM_LJust);
	
	/* ADC block */
	pcm3793_adc_set_format(PCM_R81_RFM_LJust );
	pcm3793_pg4_gain(PCM_R80_ARV(0x00));
	
	/* Mic input */
	pcm3793_mic_inc(PCM_R82_PADL | PCM_R82_PAIL | PCM_R82_PMCB ); //
	pcm3793_analog_in(PCM_R87_AIL_AIN1L | PCM_R87_AIL_DIS );
	
	/* Switchers */
	pcm3793_switch(PCM_R88_SW1 | PCM_R88_SW4);
	
	// trash
	//pg5
	pcm3793_write(PCM_R89, PCM_R89_GML_21dB);
	
	// MBST
	pcm3793_write(PCM_R86, PCM_R86_MBST | PCM_R86_MSR(0x07));
	
	
	pcm3793_write(0x48, 0x03);
	pcm3793_write(0x5a, 0x10);
	pcm3793_write(0x56, 0x01);
	pcm3793_write(0x49, 0xfc);
	pcm3793_write(PCM_MASTER_MODE, 0x05);
	
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

void pcm3793_hpl(uint8_t value) {
	codec.r64 |= value;
	pcm3793_write(PCM_R64, codec.r64);
}

void pcm3793_hpr(uint8_t value) {
	codec.r65 |= value;
	pcm3793_write(PCM_R65, codec.r65);
}

void pcm3793_spl(uint8_t value) {
	
}

void pcm3793_spr(uint8_t value) {
	
}

void pcm3793_dal(uint8_t value) {
	codec.r68 |= value;
	pcm3793_write(PCM_R68, codec.r68);
}

void pcm3793_da_r(uint8_t value) {
	codec.r69 |= value;
	pcm3793_write(PCM_R69, codec.r69);
}

void pcm3793_dac_set_format(uint8_t value) {
	codec.r70 |= value;
	pcm3793_write(PCM_R70, codec.r70);
}

void pcm3793_set_dac_digital_in_gain(uint8_t value) {
	codec.r70 |= value;
	pcm3793_write(PCM_R70, codec.r70);
}

void pcm3793_set_dac_over(void) {
	codec.r70 |= PCM_R70_OVER;
	pcm3793_write(PCM_R70, codec.r70);
}

void pcm3793_set_dem(uint8_t value) {
	codec.r70 |= value;
	pcm3793_write(PCM_R70, codec.r70);
}

void pcm3793_adc_set_format(uint8_t value) {
	codec.r81 |= value;
	pcm3793_write(PCM_R81, codec.r81);
}

void pcm3793_pg4_gain(uint8_t value) {
	codec.r80 = value;
	pcm3793_write(PCM_R80, codec.r80);
}

void pcm3793_mic_inc(uint8_t value) {
	codec.r82 |= value;
	pcm3793_write(PCM_R82, codec.r82);
}

void pcm3793_analog_in(uint8_t value) {
	codec.r87 |= value;
	pcm3793_write(PCM_R87, codec.r87);
}

void pcm3793_switch(uint8_t value) {
	codec.r88 = value;
	pcm3793_write(PCM_R88, codec.r88);
}