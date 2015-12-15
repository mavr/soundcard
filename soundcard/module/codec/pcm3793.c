/*
 * pcm3793.c
 *
 * Created: 22.10.2015 12:37:56
 *  Author: ale
 */ 

#include "sam.h"
#include "core/syslog.h"

#include "pcm3793.h"

void pcm3793_init() {
	
	pcm3793_sysclk_div(0x07); // 8kHz
	
	/* Set analog volume for right Tel. channel
		and disable analog mute. */
	pcm3793_hpr_vol(PCM_R65_HRV(0x33));
	
	/* SPL/R disable */
	
	/* DAC block */
	pcm3793_dar(PCM_R69_ATR(0x33));
	pcm3793_dac_format(PCM_R70_PFM_LJust);
	pcm3793_dac_over();
	pcm3793_dac_filter_dem(PCM_R70_DEM_OFF);
	pcm3793_dac_gain(PCM_R70_SPX_0dB);
	
	/* ADC block */
	pcm3793_adc_format(PCM_R81_RFM_LJust );
	pcm3793_adc_filter_hp(PCM_R81_HPF_OFF);

	
	/* Set boost for PG1. */
	pcm3793_pg1_m20dB();
	
	/* Power on Mic Bias. */
	pcm3793_pbis_up();
	
	/* Zero-cross ? */
	
	/* Turn on mixer. */
	pcm3793_pmxr_on();
	
	/* Choose switchers. */
//	pcm3793_switch(PCM_R88_SW1 | PCM_R88_SW6);
	pcm3793_switch(PCM_R88_SW5 | PCM_R88_SW2);
	
	/* Turn on HPR. */
	void pcm3793_phpr_up();
	
	/* Power up Vcom. */
	pcm3793_vcom();
	
	/* Power on inside source. */
	pcm3793_mic_bias_source();
	pcm3793_adl_up();
	pcm3793_pg1pg5_up();
	
	/* Switch codec to master mode. */
	pcm3793_mode_master();
	
	pcm3793_analog_in(PCM_R87_AIL_AIN1L | PCM_R87_AIL_DIS );
	
	pcm3793_pg3_gain(PCM_R79_ALV(0x0c));

	///* wait 450 ms */
	////TODO: remade this shit
	volatile unsigned long long i = 0;
	for(i = 0; i < 100000; i++) {
		__NOP;
	}
	pcm3793_write(0x49, 0xff);
	
	__DEBUG(LOG_LVL_HIGH, "[audio]\tStarted in master mode");
	
}
