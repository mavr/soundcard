/*
 * ad74111.c
 *
 * Created: 30.01.2015 13:14:47
 *  Author: ale
 */ 

#include "sam.h"
#include "ssc/ssc.h"
#include "codec/ad74111.h"

void codec_init() {
	ad74111.mode = AD74111_MIXED;
	ad74111.con_mode = AD74111_MASTER;
	ad74111.tdata_counter = 0;
	codec_init_cr(&ad74111.registers);
	codec_reset();
}

void codec_reset() {
	RESET_LINE->PIO_CODR |= RESET_PIN;
	
	if(ad74111.con_mode == AD74111_MASTER) 
		PIOA->PIO_CODR |= PIO_PA17; // set DIN low 
	else 
		PIOA->PIO_SODR |= PIO_PA17; // or high
	
	ad74111.tdata_counter = 0;
	RESET_LINE->PIO_SODR |= RESET_PIN;
}

void codec_init_cr(ad74111_registers *registers) {
	//registers->crA = AD74111_CRA_ADDRESS | AD74111_CRA_ADC | AD74111_CRA_DAC | 
	//	AD74111_CRA_ADC_INPUT_AMP | AD74111_CRA_REFERENCE | AD74111_CRA_REFERENCE_AMPLIFIER;
	registers->crA = AD74111_CRA_ADDRESS | AD74111_CRA_REFERENCE | AD74111_CRA_REFERENCE_AMPLIFIER | AD74111_CRA_DAC | AD74111_CRA_ADC | AD74111_CRA_ADC_INPUT_AMP;
	registers->crB = AD74111_CRB_ADDRESS | AD74111_CRB_FIRSTDIV_DIV_3 | AD74111_CRB_SECONDIV_DIV_2;
	registers->crC = AD74111_CRC_ADDRESS | AD74111_CRC_WORD_WIDTH_16bit;
	registers->crD = AD74111_CRD_ADDRESS | AD74111_CRD_MASTER | AD74111_CRD_DSP_16Bits | AD74111_CRD_MODE_DATA;
	registers->crE = AD74111_CRE_ADDRESS | AD74111_CRE_ADC_GAIN_6dB;
	registers->crG = AD74111_CRG_ADDRESS;
	
}

void codec_write_cr(uint16_t *cr) {
	
}

void codec_adc_enable() {
	ad74111.registers.crA |= AD74111_CRA_ADC;
	codec_reset();
}

void codec_adc_disable() {
	ad74111.registers.crA &= ~AD74111_CRA_ADC;
	codec_reset();
}

void codec_dac_enable() {
	ad74111.registers.crA |= AD74111_CRA_DAC;
	codec_reset();
}

void codec_dac_disable() {
	ad74111.registers.crA &= ~AD74111_CRA_DAC;
	codec_reset();
}

void codec_dac_mute() {
	codec_dac_volume(0xffff);
	codec_reset();
}

void codec_dac_volume(uint16_t value) {
	ad74111.registers.crG |= AD74111_DAC_VOLUME(value & 0x03ff);
	codec_reset();
}