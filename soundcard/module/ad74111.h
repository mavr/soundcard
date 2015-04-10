/*
 * ad74111.h
 *
 * Created: 30.01.2015 13:15:14
 *  Author: ale
 */ 


#ifndef AD74111_H_
#define AD74111_H_

#define AD74111_CR_ADDRESS_Pos 11
#define AD74111_CR_W (1 << 15)
#define AD74111_CR_R (0u << 15)

/* Describe register A */
#define AD74111_CRA_ADDRESS (0u << AD74111_CR_ADDRESS_Pos)
#define AD74111_CRA_REFERENCE_AMPLIFIER (1u << 2) 
#define AD74111_CRA_REFERENCE (1u << 3)
#define AD74111_CRA_DAC (1u << 4)
#define AD74111_CRA_ADC (1u << 5)
#define AD74111_CRA_ADC_INPUT_AMP (1u << 6)

/* Describe register B */
#define AD74111_CRB_ADDRESS (1u << AD74111_CR_ADDRESS_Pos)
#define AD74111_CRB_FIRSTDIV_DIV_1 (0x0u << 0)
#define AD74111_CRB_FIRSTDIV_DIV_2 (0x1u << 0)
#define AD74111_CRB_FIRSTDIV_DIV_3 (0x2u << 0)
#define AD74111_CRB_SECONDIV_DIV_1 (0x0u << 2)
#define AD74111_CRB_SECONDIV_DIV_2 (0x1u << 2)
#define AD74111_CRB_SECONDIV_DIV_3 (0x2u << 2)
#define AD74111_CRB_THIRDIV_DIV_1 (0x0u << 4)
#define AD74111_CRB_THIRDIV_DIV_2 (0x1u << 4)
#define AD74111_CRB_THIRDIV_DIV_4 (0x2u << 4)

/* Describe register C */
#define AD74111_CRC_ADDRESS (2u << AD74111_CR_ADDRESS_Pos)
#define AD74111_CRC_ADC_HIGH_PASS_FILTER (1u << 0)
#define AD74111_CRC_DAC_DEEMPHASIS_44kHz (1u << 1) 
#define AD74111_CRC_DAC_DEEMPHASIS_32kHz (2u << 1) 
#define AD74111_CRC_DAC_DEEMPHASIS_48kHz (3u << 1) 
#define AD74111_CRC_LOW_GROUP_DELAY (1u << 3)
#define AD74111_CRC_WORD_WIDTH_16bit (0u << 4)
#define AD74111_CRC_WORD_WIDTH_20bit (1u << 4)
#define AD74111_CRC_WORD_WIDTH_24bit (2u << 4)

/* Describe register D */
#define AD74111_CRD_ADDRESS (3u << AD74111_CR_ADDRESS_Pos)
#define AD74111_CRD_SLAVE (0u << 0)
#define AD74111_CRD_MASTER (1u << 0)
#define AD74111_CRD_FAST_DCKL (1u << 1)
#define AD74111_CRD_DSP_16Bits (0u << 2)
#define AD74111_CRD_DSP_32Bits (1u << 2)
#define AD74111_CRD_MODE_DATA (0u << 3)
#define AD74111_CRD_MODE_MIXED (1u << 3)
#define AD74111_CRD_MULTIFRAME_SYNC_NORMAL (0u << 9)
#define AD74111_CRD_MULTIFRAME_SYNC_MFS (1u << 9)

/* Describe register E */
#define AD74111_CRE_ADDRESS (4u << AD74111_CR_ADDRESS_Pos)
#define AD74111_CRE_DAC_MUTE (1u << 0)
#define AD74111_CRE_ADC_MUTE (1u << 1)
#define AD74111_CRE_ADC_GAIN_3dB (1u << 2)
#define AD74111_CRE_ADC_GAIN_6dB (2u << 2)
#define AD74111_CRE_ADC_GAIN_9dB (3u << 2)
#define AD74111_CRE_ADC_GAIN_12dB (4u << 2)
#define AD74111_CRE_ADCL_PEAK (1u << 5)

/* Describe register F */
#define AD74111_CRF_ADDRESS (5u << AD74111_CR_ADDRESS_Pos)
#define AD74111_ADC_INPUT_PEAK_Mask 0x3F
#define AD74111_CRF_ADC_INPUT_PEAK_LVL(value) ((value & AD74111_ADC_INPUT_PEAK_Mask) << 0)

/* Describe register G */
#define AD74111_CRG_ADDRESS (6u << AD74111_CR_ADDRESS_Pos)
#define AD74111_DAC_VOLUME_Masl 0x3FF
#define AD74111_DAC_VOLUME(value) ((value & AD74111_ADC_INPUT_PEAK_Mask) << 0)


/* Codec's modes */
enum ad74111_mode { AD74111_MIXED, AD74111_DATA };
enum ad74111_connect_mode { AD74111_SLAVE, AD74111_MASTER };


typedef struct { // __attribute__((__packed))
	uint16_t crG;
	uint16_t crC;
	uint16_t crE;
	uint16_t crB;
	uint16_t crA;
	uint16_t crD;
} ad74111_registers;

typedef struct {
	enum ad74111_mode mode;
	enum ad74111_connect_mode con_mode;
	ad74111_registers registers;
	int tdata_counter;
} ad74111_t;

ad74111_t ad74111;


void codec_init(void);
void codec_init_cr(ad74111_registers *registers);
void codec_write_cr(uint16_t *cr);
uint16_t codec_read_cr(uint16_t address);

void codec_reset(void);

/* Codec control functions */
void codec_adc_enable(void);
void codec_dac_enable(void);
void codec_adc_disable(void);
void codec_dac_disable(void);

void codec_dac_mute(void);
void codec_dac_volume(uint16_t value);




#endif /* AD74111_H_ */