/*
 * pcm3793.h
 *
 * Created: 08.09.2015 16:30:06
 *  Author: ale
 */ 


#ifndef PCM3793_H_
#define PCM3793_H_

#include "audio/audio.h"

/* Registers */
#define PCM_R64					0x40
#define PCM_R65					0x41
#define PCM_R66					0x42
#define PCM_R67					0x43
#define PCM_R68					0x44
#define PCM_R69					0x45
#define PCM_R70					0x46
#define PCM_R71					0x47
#define PCM_R72					0x48
#define PCM_R73					0x49
#define PCM_R74					0x4a
#define PCM_R75					0x4b
#define PCM_R76					0x4c
#define PCM_R77					0x4d
#define PCM_R79					0x4f
#define PCM_R80					0x50
#define PCM_R81					0x51
#define PCM_R82					0x52
#define PCM_R83					0x53
#define PCM_R84					0x54
#define PCM_R85					0x55
#define PCM_R86					0x56
#define PCM_R87					0x57
#define PCM_R88					0x58
#define PCM_R89					0x59
#define PCM_R90					0x5a
#define PCM_R91					0x5b
#define PCM_R92					0x5c
#define PCM_R93					0x5d
#define PCM_R94					0x5e
#define PCM_R95					0x5f
#define PCM_R96					0x60
#define PCM_R97					0x61
#define PCM_R98					0x62
#define PCM_R99					0x63
#define PCM_R100				0x64
#define PCM_R101				0x65
#define PCM_R102				0x66
#define PCM_R104				0x67
#define PCM_R105				0x68
#define PCM_R125				0x7d

/* Register 64 */
#define PCM_R64_HMUL			(0x01 << 6)
#define PCM_R64_HLV_Msk			0x3f
#define PCM_R64_HLV_Pos			0x0
#define PCM_R64_HLV(value)		(PCM_R64_HLV_Msk & ((value) << PCM_R64_HLV_Pos))

/* Register 65 */
#define PCM_R65_HMUR			(0x01 << 6)
#define PCM_R65_HRV_Msk			0x3f
#define PCM_R65_HRV_Pos			0x0
#define PCM_R65_HRV(value)		(PCM_R65_HRV_Msk & ((value) << PCM_R65_HRV_Pos))

/* Register 66 */
#define PCM_R66_SMUL			(0x01 << 6)
#define PCM_R66_SLV_Msk			0x3f
#define PCM_R66_SLV_Pos			0x00
#define PCM_R66_SLV(value)		(PCM_R66_SLV_Msk & ((value) << PCM_R66_SLV_Pos))

/* Register 67 */
#define PCM_R67_SMUR			(0x01 << 6)
#define PCM_R67_SRV_Msk			0x3f
#define PCM_R67_SRV_Pos			0x00
#define PCM_R67_SRV(value)		(PCM_R67_SRV_Msk & ((value) << PCM_R67_SRV_Pos))

/* Register 68 */
#define PCM_R68_PMUL			(0x01 << 6)
#define PCM_R68_ATL_Msk			0x3f
#define PCM_R68_ATL_Pos			0x00
#define PCM_R68_ATL(value)		(PCM_R68_ATL_Msk & ((value) << PCM_R68_ATL_Pos))

/* Register 69 */
#define PCM_R69_PMUR			(0x01 << 6)
#define PCM_R69_ATR_Msk			0x3f
#define PCM_R69_ATR_Pos			0x00
#define PCM_R69_ATR(value)		(PCM_R69_ATR_Msk & ((value) << PCM_R69_ATR_Pos))

/* Register 70 */
#define PCM_R70_DEM_Msk			0xc0
#define PCM_R70_DEM_Pos			0x06
#define PCM_R70_DEM_OFF			(PCM_R70_DEM_Msk & ((0x00) << PCM_R70_DEM_Pos))
#define PCM_R70_DEM_32kHz		(PCM_R70_DEM_Msk & ((0x01) << PCM_R70_DEM_Pos))
#define PCM_R70_DEM_44kHz		(PCM_R70_DEM_Msk & ((0x02) << PCM_R70_DEM_Pos))
#define PCM_R70_DEM_48kHz		(PCM_R70_DEM_Msk & ((0x03) << PCM_R70_DEM_Pos))

#define PCM_R70_PFM_Msk			0x30
#define PCM_R70_PFM_Pos			0x04
#define PCM_R70_PFM_I2S			(PCM_R70_PFM_Msk & ((0x00) << PCM_R70_PFM_Pos))
#define PCM_R70_PFM_RJust		(PCM_R70_PFM_Msk & ((0x01) << PCM_R70_PFM_Pos))
#define PCM_R70_PFM_LJust		(PCM_R70_PFM_Msk & ((0x02) << PCM_R70_PFM_Pos))
#define PCM_R70_PFM_DSP			(PCM_R70_PFM_Msk & ((0x03) << PCM_R70_PFM_Pos))

#define PCM_R70_SPX_Msk			0x0c
#define PCM_R70_SPX_Pos			0x02
#define PCM_R70_SPX_0dB			(PCM_R70_SPX_Msk & ((0x00) << PCM_R70_SPX_Pos))
#define PCM_R70_SPX_6dB			(PCM_R70_SPX_Msk & ((0x01) << PCM_R70_SPX_Pos))
#define PCM_R70_SPX_12dB		(PCM_R70_SPX_Msk & ((0x02) << PCM_R70_SPX_Pos))
#define PCM_R70_SPX_18dB		(PCM_R70_SPX_Msk & ((0x03) << PCM_R70_SPX_Pos))

#define PCM_R70_OVER			(0x01 << 0)

/* Register 71 */
#define PCM_R71_SPSE			(0x01 << 4)

#define PCM_R71_SPS_Msk			0x0c
#define PCM_R71_SPS_Pos			2
#define PCM_R71_SPS_LOW			(PCM_R71_SPS_Msk & ((0x00) << PCM_R71_SPS_Pos))
#define PCM_R71_SPS_MED			(PCM_R71_SPS_Msk & ((0x01) << PCM_R71_SPS_Pos))
#define PCM_R71_SPS_HIGH		(PCM_R71_SPS_Msk & ((0x02) << PCM_R71_SPS_Pos))

#define PCM_R71_DFQ_Msk			0x03
#define PCM_R71_DFQ_Pos			0x00
#define PCM_R71_DFQ_1_5MHz		(PCM_R71_DFQ_Msk & ((0x00) << PCM_R71_DFQ_Pos))
#define PCM_R71_DFQ_2_25MHz		(PCM_R71_DFQ_Msk & ((0x01) << PCM_R71_DFQ_Pos))
#define PCM_R71_DFQ_2_65MHz		(PCM_R71_DFQ_Msk & ((0x02) << PCM_R71_DFQ_Pos))
#define PCM_R71_DFQ_3MHz		(PCM_R71_DFQ_Msk & ((0x03) << PCM_R71_DFQ_Pos))

/* Register 72 */
#define PCM_R72_PMXR			(0x01 << 1)
#define PCM_R72_PMXL			(0x01 << 0)

/* Register 73 */
#define PCM_R73_PBIS			(0x01 << 7)
#define PCM_R73_PDAR			(0x01 << 6)
#define PCM_R73_PDAL			(0x01 << 5)
#define PCM_R73_PHPC			(0x01 << 4)
#define PCM_R73_PHPR			(0x01 << 3)
#define PCM_R73_PHPL			(0x01 << 2)
#define PCM_R73_PSPR			(0x01 << 1)
#define PCM_R73_PSPL			(0x01 << 0)

/* Register 74 */
#define PCM_R74_CMS_Msk			0xe0
#define PCM_R74_CMS_Pos			0x04
#define PCM_R74_CMS_COMV		(PCM_R74_CMS_Msk & ((0x00) << PCM_R74_CMS_Pos))
#define PCM_R74_CMS_MONAUR		(PCM_R74_CMS_Msk & ((0x01) << PCM_R74_CMS_Pos))
#define PCM_R74_CMS_INV_HPOL	(PCM_R74_CMS_Msk & ((0x02) << PCM_R74_CMS_Pos))
#define PCM_R74_CMS_INV_HPOR	(PCM_R74_CMS_Msk & ((0x04) << PCM_R74_CMS_Pos))

#define PCM_R74_HPS_Msk			0x0c
#define PCM_R74_HPS_Pos			0x02
#define PCM_R74_HPS_STEREO		(PCM_R74_HPS_Msk & ((0x00) << PCM_R74_HPS_Pos))
#define PCM_R74_HPS_SINGLE		(PCM_R74_HPS_Msk & ((0x01) << PCM_R74_HPS_Pos))
#define PCM_R74_HPS_DIFF		(PCM_R74_HPS_Msk & ((0x02) << PCM_R74_HPS_Pos))

#define PCM_R74_SPKS			(0x01 << 1)
#define PCM_R74_PCOM			(0x01 << 0)

/* Register 75 */
#define PCM_R75_HPDP			(0x01 << 7)
#define PCM_R75_HPDE			(0x01 << 6)
#define PCM_R75_SDHC			(0x01 << 4)
#define PCM_R75_SDHR			(0x01 << 3)
#define PCM_R75_SDHL			(0x01 << 2)
#define PCM_R75_SDSR			(0x01 << 1)
#define PCM_R75_SDSL			(0x01 << 0)

/* Register 76 */
#define PCM_R76_RLSR			(0x01 << 1)
#define PCM_R76_RLSL			(0x01 << 0)

/* Register 77 */
#define PCM_R77_HDPS			(0x01 << 7)
#define PCM_R77_STHC			(0x01 << 4)
#define PCM_R77_STHR			(0x01 << 3)
#define PCM_R77_STHL			(0x01 << 2)
#define PCM_R77_STSR			(0x01 << 1)
#define PCM_R77_STSL			(0x01 << 0)

/* Register 79 */
#define PCM_R79_ALV_Msk			0x1F
#define PCM_R79_ALV_Pos			0x00
#define PCM_R79_ALV(value)		(PCM_R79_ALV_Msk & ((value) << PCM_R79_ALV_Pos))

/* Register 80 */
#define PCM_R80_ARV_Msk			0x1F
#define PCM_R80_ARV_Pos			0x00
#define PCM_R80_ARV(value)		(PCM_R80_ARV_Msk & ((value) << PCM_R80_ARV_Pos))

/* Register 81 */
#define PCM_R81_HPF_Msk			0xc0
#define PCM_R81_HPF_Pos			0x06
#define PCM_R81_HPF_4Hz			(PCM_R81_HPF_Msk & ((0x00) << PCM_R81_HPF_Pos))
#define PCM_R81_HPF_240Hz		(PCM_R81_HPF_Msk & ((0x01) << PCM_R81_HPF_Pos))
#define PCM_R81_HPF_120Hz		(PCM_R81_HPF_Msk & ((0x02) << PCM_R81_HPF_Pos))
#define PCM_R81_HPF_OFF			(PCM_R81_HPF_Msk & ((0x03) << PCM_R81_HPF_Pos))

#define PCM_R81_RMUL			(0x01 << 5)
#define PCM_R81_RMUR			(0x01 << 4)

#define PCM_R81_DSMC			(0x01 << 2)

#define PCM_R81_RFM_Msk			0x03
#define PCM_R81_RFM_Pos			0x00
#define PCM_R81_RFM_I2S			(PCM_R81_RFM_Msk & ((0x00) << PCM_R81_RFM_Pos))
#define PCM_R81_RFM_RJust		(PCM_R81_RFM_Msk & ((0x01) << PCM_R81_RFM_Pos))
#define PCM_R81_RFM_LJust		(PCM_R81_RFM_Msk & ((0x02) << PCM_R81_RFM_Pos))
#define PCM_R81_RFM_DSP			(PCM_R81_RFM_Msk & ((0x03) << PCM_R81_RFM_Pos))

/* Register 82 */
#define PCM_R82_PAIR			(0x01 << 5)
#define PCM_R82_PAIL			(0x01 << 4)
#define PCM_R82_PADS			(0x01 << 3)
#define PCM_R82_PMCB			(0x01 << 2)
#define PCM_R82_PADR			(0x01 << 1)
#define PCM_R82_PADL			(0x01 << 0)

/* Register 83 */
/* Recording */

/* Register 84 */
#define PCM_R84_MSTR			(0x01 << 2)
#define PCM_R84_BIT0			(0x01 << 0)

/* Register 85 */
#define PCM_R85_SRST			(0x01 << 7)

#define PCM_R85_NPR_Msk			0x3f
#define PCM_R85_NPR_Pos			0
#define PCM_R85_NPR(value)		(PCM_R85_NPR_Msk & ((value) << PCM_R85_NPR_Pos))

/* Register 86 */
#define PCM_R86_MBST			(0x01 << 7)

#define PCM_R86_MSR_Msk			0x70
#define PCM_R86_MSR_Pos			0x04
#define PCM_R86_MSR(value)		(PCM_R86_MSR_Msk & ((value) << PCM_R86_MSR_Pos))

#define PCM_R86_ATOD			(0x01 << 3)
#define PCM_R86_ZCRS			(0x01 << 0)

/* Register 87 */
#define PCM_R87_AD2S			(0x01 << 7)

#define PCM_R87_AIR_Msk			0x18
#define PCM_R87_AIR_Pos			0x04
#define PCM_R87_AIR_DIS			(PCM_R87_AIR_Msk & ((0x00) << PCM_R87_AIR_Pos))
#define PCM_R87_AIR_AIN1R		(PCM_R87_AIR_Msk & ((0x01) << PCM_R87_AIR_Pos))
#define PCM_R87_AIR_AIN2R		(PCM_R87_AIR_Msk & ((0x02) << PCM_R87_AIR_Pos))
#define PCM_R87_AIR_AIN3R		(PCM_R87_AIR_Msk & ((0x03) << PCM_R87_AIR_Pos))

#define PCM_R87_AIL_Msk			0x03
#define PCM_R87_AIL_Pos			0x00
#define PCM_R87_AIL_DIS			(PCM_R87_AIL_Msk & ((0x00) << PCM_R87_AIL_Pos))
#define PCM_R87_AIL_AIN1L		(PCM_R87_AIL_Msk & ((0x01) << PCM_R87_AIL_Pos))
#define PCM_R87_AIL_AIN2L		(PCM_R87_AIL_Msk & ((0x02) << PCM_R87_AIL_Pos))
#define PCM_R87_AIL_AIN3L		(PCM_R87_AIL_Msk & ((0x03) << PCM_R87_AIL_Pos))

/* Register 88 */
#define PCM_R88_MXR2			(0x01 << 6)
#define PCM_R88_SW6				PCM_R88_MXR2
#define PCM_R88_MXR1			(0x01 << 5)
#define PCM_R88_SW4				PCM_R88_MXR1
#define PCM_R88_MXR0			(0x01 << 4)
#define PCM_R88_SW5				PCM_R88_MXR0
#define PCM_R88_MXL2			(0x01 << 2)
#define PCM_R88_SW3				PCM_R88_MXL2
#define PCM_R88_MXL1			(0x01 << 1)
#define PCM_R88_SW1				PCM_R88_MXL1
#define PCM_R88_MXL0			(0x01 << 0)
#define PCM_R88_SW2				PCM_R88_MXL0

/* Register 89 */
#define PCM_R89_GMR_Msk			0x70
#define PCM_R89_GMR_Pos			0x04
#define PCM_R89_GMR_21dB		(PCM_R89_GMR_Msk & (0x00 << PCM_R89_GMR_Pos))
#define PCM_R89_GMR_18dB		(PCM_R89_GMR_Msk & (0x01 << PCM_R89_GMR_Pos))
#define PCM_R89_GMR_15dB		(PCM_R89_GMR_Msk & (0x02 << PCM_R89_GMR_Pos))
#define PCM_R89_GMR_12dB		(PCM_R89_GMR_Msk & (0x03 << PCM_R89_GMR_Pos))
#define PCM_R89_GMR_9dB			(PCM_R89_GMR_Msk & (0x04 << PCM_R89_GMR_Pos))
#define PCM_R89_GMR_6dB			(PCM_R89_GMR_Msk & (0x05 << PCM_R89_GMR_Pos))
#define PCM_R89_GMR_3dB			(PCM_R89_GMR_Msk & (0x06 << PCM_R89_GMR_Pos))
#define PCM_R89_GMR_0dB			(PCM_R89_GMR_Msk & (0x07 << PCM_R89_GMR_Pos))

#define PCM_R89_GML_Msk			0x03
#define PCM_R89_GML_Pos			0x00
#define PCM_R89_GML_21dB		(PCM_R89_GML_Msk & (0x00 << PCM_R89_GML_Pos))
#define PCM_R89_GML_18dB		(PCM_R89_GML_Msk & (0x01 << PCM_R89_GML_Pos))
#define PCM_R89_GML_15dB		(PCM_R89_GML_Msk & (0x02 << PCM_R89_GML_Pos))
#define PCM_R89_GML_12dB		(PCM_R89_GML_Msk & (0x03 << PCM_R89_GML_Pos))
#define PCM_R89_GML_9dB			(PCM_R89_GML_Msk & (0x04 << PCM_R89_GML_Pos))
#define PCM_R89_GML_6dB			(PCM_R89_GML_Msk & (0x05 << PCM_R89_GML_Pos))
#define PCM_R89_GML_3dB			(PCM_R89_GML_Msk & (0x06 << PCM_R89_GML_Pos))
#define PCM_R89_GML_0dB			(PCM_R89_GML_Msk & (0x07 << PCM_R89_GML_Pos))

/* Register 90 */
#define PCM_R90_G20R			(0x01 << 1)
#define PCM_R90_G20L			(0x01 << 0)

/* Register 92 */
#define PCM_R92_LPAE			(0x01 << 7)

#define PCM_R92_LGA_Msk			0x1f
#define PCM_R92_LGA_Pos			0x00
#define PCM_R92_LGA(value)		(PCM_R92_LGA_Msk & ((value) << PCM_R92_LGA_Pos))

typedef struct {
	uint8_t r64;
	uint8_t r65;
	uint8_t r66;
	uint8_t r67;
	uint8_t r68;
	uint8_t r69;
	uint8_t r70;
	uint8_t r71;
	uint8_t r72;
	uint8_t r73;
	uint8_t r74;
	uint8_t r75;
	uint8_t r76;
	uint8_t r77;
	uint8_t r78;
	uint8_t r79;
	uint8_t r80;
	uint8_t r81;
	uint8_t r82;
	uint8_t r83;
	uint8_t r84;
	uint8_t r85;
	uint8_t r86;
	uint8_t r87;
	uint8_t r88;
	uint8_t r89;
	uint8_t r90;
} pcm3793_reg_t;

typedef struct {
	pcm3793_reg_t reg;

} pcm3793_codec_t;

pcm3793_codec_t codec;

/* Set value to codec register via spi. */
void pcm3793_write(uint8_t register, uint8_t value);

/* Configuring an turn on codec. */
void pcm3793_init(void);


/** Headphone amp **/
/* Disable analog mute and set volume. */
void pcm3793_hpl_vol(uint8_t value);
void pcm3793_hpr_vol(uint8_t value);


/** Speaker output **/
void pcm3793_spl(uint8_t value);
void pcm3793_spr(uint8_t value);


/** DAC block. **/
/* Disable digital mute and set volume. */
void pcm3793_dal(uint8_t value);
void pcm3793_dar(uint8_t value);	// R69
/* Set DAC config. */
void pcm3793_dac_format(uint8_t value);	//R70
void pcm3793_dac_gain(uint8_t value);
void pcm3793_dac_over(void);
void pcm3793_dac_filter_dem(uint8_t value);


/** ADC block. **/
/* Set ADC format. */
void pcm3793_adc_format(uint8_t value);	// R81
/* High-Pass Filter Selection */
void pcm3793_adc_filter_hp(uint8_t value);


/** PG[1..6] block. */
/* 20dB boost for PG1/PG2. */
void pcm3793_pg1_m20dB(void);	// R90
void pcm3793_pg2_m20dB(void);	// R90


/** Inside power system. **/
/** Register 73 **/
/* Mic Bias */
void pcm3793_pbis_up(void);
/* Digital filtres for DAC */
void pcm3793_pdar_up(void);
void pcm3793_pdal_up(void);
/* Control for headphone output */
void pcm3793_phpc_up(void);
void pcm3793_phpr_up(void);
void pcm3793_phpl_up(void);
/* Control for speaker output */
void pcm3793_pspr_up(void);
void pcm3793_pspl_up(void);
/** Register 82 **/
void pcm3793_pg2pg6_up(void);
void pcm3793_pg1pg5_up(void);
void pcm3793_d2s_up(void);
void pcm3793_mic_bias_source(void);
void pcm3793_adl_up(void);
void pcm3793_adr_up(void);


/** Mixers **/
void pcm3793_pmxr_on(void);
void pcm3793_pmxl_on(void);

/** Switchers **/
void pcm3793_switch(uint8_t value);

/** Analog output configuration. **/
void pcm3793_vcom(void); // R74

/** MUX **/
void pcm3793_analog_in(uint8_t value);


/** Codec system **/
void pcm3793_sysclk_div(uint8_t value);





void pcm3793_dac_filter_dem(uint8_t value);




void pcm3793_pg3_gain(uint8_t value);
void pcm3793_pg4_gain(uint8_t value);




void pcm3793_analog_out(uint8_t value);

void pcm3793_switch(uint8_t value);





void pcm3793_zero_cross_enable(void);

void pcm3793_mode_master(void);


#endif /* PCM3793_H_ */