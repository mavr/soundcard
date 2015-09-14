/*
 * pcm3793.h
 *
 * Created: 08.09.2015 16:30:06
 *  Author: ale
 */ 


#ifndef PCM3793_H_
#define PCM3793_H_

/* codec registers */

#define PCM_VOLUME_HPA_Lch		0x40
#define PCM_VOLUME_HPA_Rch		0x41
#define PCM_VOLUME_SPA_Lch		0x42
#define PCM_VOLUME_SPA_Rch		0x43
#define PCM_DAC_SOFT_MUTE_Lch	0x44
#define PCM_DAC_SOFT_MUTE_Rch	0x45
#define PCM_DAC_OVER_SAMPLING	0x46
#define PCM_SPA					0x47
#define PCM_ANALOG_MIXER		0x48
#define PCM_DAC_SPA_HPA			0x49
#define PCM_ANALOG_OUTPUT		0x4a
#define PCM_HPA					0x4b
#define PCM_SPA_SHUTDOWN		0x4c
#define PCM_VOLUME_ADC_IN_Lch	0x4f
#define PCM_VOLUME_ADC_IN_Rch	0x50
#define PCM_ADC_1				0x51
#define PCM_ADC_2				0x52
#define PCM_BCK					0x56
#define PCM_ANALOG_IN_SEL		0x57
#define PCM_ANALOG_MIX_SW		0x58
#define PCM_MIC_BOOST			0x5a

void pcm3793_init(void);

void pcm3793_write(uint8_t register, uint8_t value);




#endif /* PCM3793_H_ */