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


void pcm3793_write(uint8_t reg, uint8_t value) {
	spi_send( ( ((uint16_t) reg) << 8 ) | value );
}

void pcm3793_hpl_vol(uint8_t value) {
	codec.r64 |= value;
	pcm3793_write(PCM_R64, codec.r64);
}

void pcm3793_hpr_vol(uint8_t value) {
	codec.r65 |= value;
	pcm3793_write(PCM_R65, codec.r65);
}

void pcm3793_spl(uint8_t value) {
	//TODO: pcm3793_spl()	
}

void pcm3793_spr(uint8_t value) {
	//TODO: pcm3793_spr()
}

void pcm3793_dal(uint8_t value) {
	codec.r68 = value;
	pcm3793_write(PCM_R68, codec.r68);
}

void pcm3793_dar(uint8_t value) {
	codec.r69 = value;
	pcm3793_write(PCM_R69, codec.r69);
}

void pcm3793_dac_format(uint8_t value) {
	codec.r70 |= value;
	pcm3793_write(PCM_R70, codec.r70);
}

void pcm3793_dac_gain(uint8_t value) {
	codec.r70 |= value;
	pcm3793_write(PCM_R70, codec.r70);
}

void pcm3793_dac_over(void) {
	codec.r70 |= PCM_R70_OVER;
	pcm3793_write(PCM_R70, codec.r70);
}

void pcm3793_dac_filter_dem(uint8_t value) {
	codec.r70 |= value;
	pcm3793_write(PCM_R70, codec.r70);
}

void pcm3793_pmxr_on() {
	codec.r72 |= PCM_R72_PMXR;
	pcm3793_write(PCM_R72, codec.r72);
}

void pcm3793_pmxl_on() {
	codec.r72 |= PCM_R72_PMXL;
	pcm3793_write(PCM_R72, codec.r72);
}

void pcm3793_pbis_up() {
	codec.r73 |= PCM_R73_PBIS;
	pcm3793_write(PCM_R73, codec.r73);
}

void pcm3793_pdar_up() {
	codec.r73 |= PCM_R73_PDAR;
	pcm3793_write(PCM_R73, codec.r73);
}

void pcm3793_pdal_up() {
	codec.r73 |= PCM_R73_PDAL;
	pcm3793_write(PCM_R73, codec.r73);
}

void pcm3793_phpc_up() {
	codec.r73 |= PCM_R73_PHPC;
	pcm3793_write(PCM_R73, codec.r73);
}

void pcm3793_phpr_up() {
	codec.r73 |= PCM_R73_PHPR;
	pcm3793_write(PCM_R73, codec.r73);
}

void pcm3793_phpl_up() {
	codec.r73 |= PCM_R73_PHPL;
	pcm3793_write(PCM_R73, codec.r73);
}

void pcm3793_pspr_up() {
	codec.r73 |= PCM_R73_PSPR;
	pcm3793_write(PCM_R73, codec.r73);
}

void pcm3793_pspl_up() {
	codec.r73 |= PCM_R73_PSPL;
	pcm3793_write(PCM_R73, codec.r73);
}

void pcm3793_vcom() {
	codec.r74 |= PCM_R74_PCOM;
	pcm3793_write(PCM_R74, codec.r74);
}

void pcm3793_pg3_gain(uint8_t value) {
	codec.r79 = value;
	pcm3793_write(PCM_R79, codec.r79);
}

void pcm3793_pg4_gain(uint8_t value) {
	codec.r80 = value;
	pcm3793_write(PCM_R80, codec.r80);
}

void pcm3793_adc_format(uint8_t value) {
	codec.r81 |= value;
	pcm3793_write(PCM_R81, codec.r81);
}

void pcm3793_adc_filter_hp(uint8_t value) {
	codec.r81 |= value;
	pcm3793_write(PCM_R81, codec.r81);
}

//void pcm3793_adc

void pcm3793_mic_inc(uint8_t value) {
	codec.r82 |= value;
	pcm3793_write(PCM_R82, codec.r82);
}

void pcm3793_pg2pg6_up() {
	codec.r82 |= PCM_R82_PAIR;
	pcm3793_write(PCM_R82, codec.r82);
}

void pcm3793_pg1pg5_up() {
	codec.r82 |= PCM_R82_PAIL;
	pcm3793_write(PCM_R82, codec.r82);
}

void pcm3793_d2s_up() {
	codec.r82 |= PCM_R82_PADS;
	pcm3793_write(PCM_R82, codec.r82);
}

void pcm3793_mic_bias_source()  {
	codec.r82 |= PCM_R82_PMCB;
	pcm3793_write(PCM_R82, codec.r82);
}

void pcm3793_adl_up() {
	codec.r82 |= PCM_R82_PADL;
	pcm3793_write(PCM_R82, codec.r82);
}

void pcm3793_adr_up() {
	codec.r82 |= PCM_R82_PADR;
	pcm3793_write(PCM_R82, codec.r82);
}

void pcm3793_mode_master() {
	codec.r84 = PCM_R84_MSTR;
	pcm3793_write(PCM_R84, codec.r84);
}

void pcm3793_zero_cross_enable() {
	codec.r86 |= PCM_R86_ZCRS;
	pcm3793_write(PCM_R86, codec.r86);
}

void pcm3793_sysclk_div(uint8_t value) {
	codec.r86 |= PCM_R86_MSR(value);
	pcm3793_write(PCM_R86, codec.r86);
}


void pcm3793_analog_in(uint8_t value) {
	codec.r87 |= value;
	pcm3793_write(PCM_R87, codec.r87);
}

void pcm3793_switch(uint8_t value) {
	codec.r88 = value;
	pcm3793_write(PCM_R88, codec.r88);
}

void pcm3793_pg1_m20dB() {
	codec.r90 |= PCM_R90_G20L;
	pcm3793_write(PCM_R90, codec.r90);
}

void pcm3793_pg2_m20dB() {
	codec.r90 |= PCM_R90_G20R;
	pcm3793_write(PCM_R90, codec.r90);
}