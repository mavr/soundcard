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
	
	pcm3793_analog_in(PCM_R87_AIL_AIN2L | PCM_R87_AIL_DIS );
	
	pcm3793_pg3_gain(PCM_R79_ALV(0x27));
	
	
	//
	//
	//
	//
	//
	///*  */
	//pcm3793_pg3_gain(PCM_R79_ALV(0x27)); // 0x0c = 0dB
	//
	//
	///* Register 73 */
	//pcm3793_pbis_up();
	//pcm3793_pdar_up();
	//
	///* Register 72 */
	//pcm3793_pmxr_on(); // reg 72
	//
	///* Switchers */
//
	//
	////pcm3793_zero_cross_enable();
	//
	//pcm3793_write(0x49, 0xfc);
	////	pcm3793_phpc_up();
	////	pcm3793_phpr_up();
	//
	 //// reg 74
		//
	///* Mic input */
	//pcm3793_mic_inc(PCM_R82_PADL | PCM_R82_PAIL | PCM_R82_PMCB ); //
	//pcm3793_analog_in(PCM_R87_AIL_AIN2L | PCM_R87_AIL_DIS );
	//
	//// trash
	////pg5
	//pcm3793_write(PCM_R89, PCM_R89_GML_9dB );
	//
	//// MBST
////	pcm3793_write(PCM_R86, PCM_R86_MBST | PCM_R86_MSR(0x07));
	//
	//pcm3793_write(0x56, PCM_R86_MBST | PCM_R86_MSR(0x07) | 0x01);
	////pcm3793_zero_cross_enable();
	//
////	pcm3793_mode_master();
	//
	//__DEBUG(LOG_LVL_HIGH, "[audio]\tConfigured pcm3793");
	///* wait 450 ms */
	////TODO: remade this shit
	volatile unsigned long long i = 0;
	for(i = 0; i < 100000; i++) {
		__NOP;
	}
	pcm3793_write(0x49, 0xff);
	
	__DEBUG(LOG_LVL_HIGH, "[audio]\tStarted in master mode");
	
}
