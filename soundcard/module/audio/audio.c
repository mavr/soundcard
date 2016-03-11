/*
 * audio.c
 *
 * Created: 29.01.2016 16:07:40
 *  Author: ale
 */ 

#include <sam.h>
#include <stddef.h>
#include <string.h>
#include "audio/audio.h"
#include "udp/udp-audio.h"
#include "udp/audio.h"
#include "ssc/ssc.h"
#include "spi/spi.h"
#include "codec/pcm3793.h"

int audio_system() {
	/* Turn on spi for control pcm device. */
	spi_system();
	__DEBUG(LOG_LVL_MED, __DEBUG_AUDIO_PREFIX, "SPI interface configured.");

	/* Turn on ssc interface for transmit audio data to and from pcm device. */
	ssc_system();
	__DEBUG(LOG_LVL_MED, __DEBUG_AUDIO_PREFIX, "SSC interface configured.");

	/* Configuring default codec state. */
	audio_controls_set_list();

	/* pcm.. wake up guy. */
	pcm3793_init();
	__DEBUG(LOG_LVL_MED, __DEBUG_AUDIO_PREFIX, "Codec pcm7393 up.");

	return 0;
}

void audio_start() {
	/* Starting ssc line for transmit playback and capture interfaces. */
	while(!udp_ready());

	ssc_irq();
	__DEBUG(LOG_LVL_HIGH, __DEBUG_AUDIO_PREFIX, "SSC interface started.");

//	__DEBUG(LOG_LVL_HIGH, __DEBUG_AUDIO_PREFIX, "Start failed because udp not ready!");
}

void audio_controls_set_list(void) {
	audio_unit_add(UDP_AC_MIC_FU_ID, &audio_mic_fu);
	audio_unit_add(UDP_AC_PHONE_FU_ID, &audio_phone_fu);
	audio_unit_add(UDP_AC_MIX_ID, &audio_mix);
	audio_unit_add(UDP_AC_MIC_PRE_FU_ID, &audio_mic_preamph_fu);
	audio_unit_add(UDP_AC_MIC_AMP_FU_ID, &audio_mic_dig_amp_fu);
	
	/**
	*	Configuration of Feature Unit ID 5. 
	*	There are controls volume and mute.
	**/
	/* Adding microphone mute unit control */
	audio_unit_ctrl_add(UDP_AC_MIC_FU_ID, UDP_AUDIO_CS_MUTE_CONTROL, &audio_unit_mic_fu_ctrl_mute);
		audio_unit_ctrl_mic_fu_conf_mute.cur = AUDIO_MIC_MUTE_CUR_DEFAULT;
		audio_unit_ctrl_mic_fu_conf_mute.max = AUDIO_MIC_MUTE_MAX;
		audio_unit_ctrl_mic_fu_conf_mute.min = AUDIO_MIC_MUTE_MIN;
		audio_unit_ctrl_mic_fu_conf_mute.res = AUDIO_MIC_MUTE_RES;

		audio_unit_mic_fu_conf_mute.attribution = (void *) &audio_unit_ctrl_mic_fu_conf_mute;
		audio_unit_mic_fu_conf_mute.__get_min = &_audio_unit_common_u8_get_min;
		audio_unit_mic_fu_conf_mute.__get_max = &_audio_unit_common_u8_get_max;
		audio_unit_mic_fu_conf_mute.__get_res = &_audio_unit_common_u8_get_res;
		audio_unit_mic_fu_conf_mute.__get_cur = &_audio_unit_common_u8_get_cur;
		audio_unit_mic_fu_conf_mute.__set_cur = &_audio_unit_common_u8_set_cur;
		audio_unit_ctrl_init(UDP_AC_MIC_FU_ID, UDP_AUDIO_CS_MUTE_CONTROL, &audio_unit_mic_fu_conf_mute);

	/* Adding microphone volume unit control */
	audio_unit_ctrl_add(UDP_AC_MIC_FU_ID, UDP_AUDIO_CS_VOLUME_CONTROL, &audio_unit_mic_fu_ctrl_vol);
		audio_unit_ctrl_mic_fu_conf_vol.cur = AUDIO_MIC_VOL_CUR_DEFAULT;
		audio_unit_ctrl_mic_fu_conf_vol.max = AUDIO_MIC_VOL_MAX;
		audio_unit_ctrl_mic_fu_conf_vol.min = AUDIO_MIC_VOL_MIN;
		audio_unit_ctrl_mic_fu_conf_vol.res = AUDIO_MIC_VOL_RES;

		audio_unit_mic_fu_conf_vol.attribution = (void *) &audio_unit_ctrl_mic_fu_conf_vol;
		audio_unit_mic_fu_conf_vol.__get_min = &_audio_unit_common_u16_get_min;
		audio_unit_mic_fu_conf_vol.__get_max = &_audio_unit_common_u16_get_max;
		audio_unit_mic_fu_conf_vol.__get_res = &_audio_unit_common_u16_get_res;
		audio_unit_mic_fu_conf_vol.__get_cur = &_audio_unit_common_u16_get_cur;
		audio_unit_mic_fu_conf_vol.__set_cur = &_audio_unit_mic_vol_set_cur;
		audio_unit_ctrl_init(UDP_AC_MIC_FU_ID, UDP_AUDIO_CS_VOLUME_CONTROL, &audio_unit_mic_fu_conf_vol);

	/**
	*	Configuration of Feature Unit ID 8. 
	*	There is pream.
	**/
	audio_unit_ctrl_add(UDP_AC_MIC_PRE_FU_ID, UDP_AUDIO_CS_VOLUME_CONTROL, &audio_unit_mic_fu_ctrl_preamph);
		audio_unit_ctrl_mic_fu_preamph.cur = AUDIO_MIC_PRE_CUR_DEFAULT;
		audio_unit_ctrl_mic_fu_preamph.max = AUDIO_MIC_PRE_MAX;
		audio_unit_ctrl_mic_fu_preamph.min = AUDIO_MIC_PRE_MIN;
		audio_unit_ctrl_mic_fu_preamph.res = AUDIO_MIC_PRE_RES;

		audio_unit_mic_fu_conf_preamph.attribution = (void *) &audio_unit_ctrl_mic_fu_preamph;
		audio_unit_mic_fu_conf_preamph.__get_min = &_audio_unit_common_u16_get_min;
		audio_unit_mic_fu_conf_preamph.__get_max = &_audio_unit_common_u16_get_max;
		audio_unit_mic_fu_conf_preamph.__get_res = &_audio_unit_common_u16_get_res;
		audio_unit_mic_fu_conf_preamph.__get_cur = &_audio_unit_common_u16_get_cur;
		audio_unit_mic_fu_conf_preamph.__set_cur = &_audio_unit_mic_preamph_set_cur;
		audio_unit_ctrl_init(UDP_AC_MIC_PRE_FU_ID, UDP_AUDIO_CS_VOLUME_CONTROL, &audio_unit_mic_fu_conf_preamph);

	/**
	*	Configuration of Feature Unit ID 8. 
	*	There is pream.
	**/
	audio_unit_ctrl_add(UDP_AC_MIC_AMP_FU_ID, UDP_AUDIO_CS_VOLUME_CONTROL, &audio_unit_mic_fu_ctrl_dig_amp);
		audio_unit_ctrl_mic_fu_dig_amp.cur = AUDIO_MIC_AMP_CUR_DEFAULT;
		audio_unit_ctrl_mic_fu_dig_amp.max = AUDIO_MIC_AMP_MAX;
		audio_unit_ctrl_mic_fu_dig_amp.min = AUDIO_MIC_AMP_MIN;
		audio_unit_ctrl_mic_fu_dig_amp.res = AUDIO_MIC_AMP_RES;

		audio_unit_mic_fu_conf_dig_amp.attribution = (void *) &audio_unit_ctrl_mic_fu_dig_amp;
		audio_unit_mic_fu_conf_dig_amp.__get_min = &_audio_unit_common_u16_get_min;
		audio_unit_mic_fu_conf_dig_amp.__get_max = &_audio_unit_common_u16_get_max;
		audio_unit_mic_fu_conf_dig_amp.__get_res = &_audio_unit_common_u16_get_res;
		audio_unit_mic_fu_conf_dig_amp.__get_cur = &_audio_unit_common_u16_get_cur;
		audio_unit_mic_fu_conf_dig_amp.__set_cur = &_audio_unit_mic_dig_amp_set_cur;
		audio_unit_ctrl_init(UDP_AC_MIC_AMP_FU_ID, UDP_AUDIO_CS_VOLUME_CONTROL, &audio_unit_mic_fu_conf_dig_amp);

	/**
	*	Configuration of Feature Unit ID 6. 
	*	There are controls volume and mute too.
	**/
	/* Adding phone mute unit control */
	audio_unit_ctrl_add(UDP_AC_PHONE_FU_ID, UDP_AUDIO_CS_MUTE_CONTROL, &audio_unit_phone_fu_ctrl_mute);
		audio_unit_ctrl_phone_fu_conf_mute.cur = AUDIO_PHONE_MUTE_CUR_DEFAULT;
		audio_unit_ctrl_phone_fu_conf_mute.max = AUDIO_PHONE_MUTE_MAX;
		audio_unit_ctrl_phone_fu_conf_mute.min = AUDIO_PHONE_MUTE_MIN;
		audio_unit_ctrl_phone_fu_conf_mute.res = AUDIO_PHONE_MUTE_RES;

		audio_unit_phone_fu_conf_mute.attribution = (void *) &audio_unit_ctrl_phone_fu_conf_mute;
		audio_unit_phone_fu_conf_mute.__get_min = &_audio_unit_common_u8_get_min;
		audio_unit_phone_fu_conf_mute.__get_max = &_audio_unit_common_u8_get_max;
		audio_unit_phone_fu_conf_mute.__get_res = &_audio_unit_common_u8_get_res;
		audio_unit_phone_fu_conf_mute.__get_cur = &_audio_unit_common_u8_get_cur;
		audio_unit_phone_fu_conf_mute.__set_cur = &_audio_unit_phone_mute_set_cur;
		
		audio_unit_ctrl_init(UDP_AC_PHONE_FU_ID, UDP_AUDIO_CS_MUTE_CONTROL, &audio_unit_phone_fu_conf_mute);

	/* Adding phone volume unit control */
	audio_unit_ctrl_add(UDP_AC_PHONE_FU_ID, UDP_AUDIO_CS_VOLUME_CONTROL, &audio_unit_phone_fu_ctrl_vol);
		audio_unit_ctrl_phone_fu_conf_vol.cur = AUDIO_MASTER_VOL_CUR_DEFAULT;
		audio_unit_ctrl_phone_fu_conf_vol.max = AUDIO_MASTER_VOL_MAX;
		audio_unit_ctrl_phone_fu_conf_vol.min = AUDIO_MASTER_VOL_MIN;
		audio_unit_ctrl_phone_fu_conf_vol.res = AUDIO_MASTER_VOL_RES;

		audio_unit_phone_fu_conf_vol.attribution = (void *) &audio_unit_ctrl_phone_fu_conf_vol;
		audio_unit_phone_fu_conf_vol.__get_min = &_audio_unit_common_u16_get_min;
		audio_unit_phone_fu_conf_vol.__get_max = &_audio_unit_common_u16_get_max;
		audio_unit_phone_fu_conf_vol.__get_res = &_audio_unit_common_u16_get_res;
		audio_unit_phone_fu_conf_vol.__get_cur = &_audio_unit_common_u16_get_cur;
		audio_unit_phone_fu_conf_vol.__set_cur = &_audio_unit_phone_vol_set_cur;
		audio_unit_phone_fu_conf_vol.__set_res = &_audio_unit_common_u16_set_res;
		audio_unit_ctrl_init(UDP_AC_PHONE_FU_ID, UDP_AUDIO_CS_VOLUME_CONTROL, &audio_unit_phone_fu_conf_vol);

	/**
	*	Configuration of Feature Unit ID 7. 
	**/
	/*  */
	audio_unit_ctrl_add(UDP_AC_MIX_ID, AUDIO_MIX_IN_PHONE, &audio_mix_in_phone);
		audio_unit_mixer_ctrl_in_phone_conf.max = AUDIO_MIX_OUT_SPEAK_MAX;
		audio_unit_mixer_ctrl_in_phone_conf.min = AUDIO_MIX_OUT_SPEAK_MIN;
		audio_unit_mixer_ctrl_in_phone_conf.cur = AUDIO_MIX_OUT_SPEAK_CUR;
		audio_unit_mixer_ctrl_in_phone_conf.res = AUDIO_MIX_OUT_SPEAK_RES;
		
		audio_unit_mixer_in_phone_conf.attribution = (void *) &audio_unit_mixer_ctrl_in_phone_conf;
		audio_unit_mixer_in_phone_conf.__get_min = &_audio_unit_common_u16_get_min;
		audio_unit_mixer_in_phone_conf.__get_max = &_audio_unit_common_u16_get_max;
		audio_unit_mixer_in_phone_conf.__get_res = &_audio_unit_common_u16_get_res;
		audio_unit_mixer_in_phone_conf.__get_cur = &_audio_unit_common_u16_get_cur;
//		audio_unit_mixer_in_phone_conf.__set_cur = &_audio_unit_mixer_in_phone_set_cur;
		audio_unit_mixer_in_phone_conf.__set_cur = &_audio_unit_mix_set_cur;
		audio_unit_mixer_in_phone_conf.__set_res = &_audio_unit_common_u16_set_res;
		audio_unit_ctrl_init(UDP_AC_MIX_ID, AUDIO_MIX_IN_PHONE, &audio_unit_mixer_in_phone_conf);

	audio_unit_ctrl_add(UDP_AC_MIX_ID, AUDIO_MIX_IN_MIC, &audio_mix_in_mic);
		audio_unit_mixer_ctrl_in_mic_conf.max = AUDIO_MIX_IN_PHONE_MAX;
		audio_unit_mixer_ctrl_in_mic_conf.min = AUDIO_MIX_IN_PHONE_MIN;
		audio_unit_mixer_ctrl_in_mic_conf.cur = AUDIO_MIX_IN_PHONE_CUR;
		audio_unit_mixer_ctrl_in_mic_conf.res = AUDIO_MIX_IN_PHONE_RES;
	
		audio_unit_mixer_in_mic_conf.attribution = (void *) &audio_unit_mixer_ctrl_in_mic_conf;
		audio_unit_mixer_in_mic_conf.__get_min = &_audio_unit_common_u16_get_min;
		audio_unit_mixer_in_mic_conf.__get_max = &_audio_unit_common_u16_get_max;
		audio_unit_mixer_in_mic_conf.__get_res = &_audio_unit_common_u16_get_res;
		audio_unit_mixer_in_mic_conf.__get_cur = &_audio_unit_common_u16_get_cur;
//		audio_unit_mixer_in_mic_conf.__set_cur = &_audio_unit_mixer_in_mic_set_cur;
		audio_unit_mixer_in_mic_conf.__set_cur = &_audio_unit_mix_set_cur;
		audio_unit_mixer_in_mic_conf.__set_res = &_audio_unit_common_u16_set_res;
		audio_unit_ctrl_init(UDP_AC_MIX_ID, AUDIO_MIX_IN_MIC, &audio_unit_mixer_in_mic_conf);

	audio_unit_ctrl_add(UDP_AC_MIX_ID, AUDIO_MIX_OUT_SPEAK, &audio_mix_out_speaker);
		audio_unit_mixer_out_ctrl_speak_conf.max = AUDIO_MIX_IN_MIC_MAX;
		audio_unit_mixer_out_ctrl_speak_conf.min = AUDIO_MIX_IN_MIC_MIN;
		audio_unit_mixer_out_ctrl_speak_conf.cur = AUDIO_MIX_IN_MIC_CUR;
		audio_unit_mixer_out_ctrl_speak_conf.res = AUDIO_MIX_IN_MIC_RES;

		audio_unit_mixer_out_speak_conf.attribution = (void *) &audio_unit_mixer_out_ctrl_speak_conf;
		audio_unit_mixer_out_speak_conf.__get_min = &_audio_unit_common_u16_get_min;
		audio_unit_mixer_out_speak_conf.__get_max = &_audio_unit_common_u16_get_max;
		audio_unit_mixer_out_speak_conf.__get_res = &_audio_unit_common_u16_get_res;
		audio_unit_mixer_out_speak_conf.__get_cur = &_audio_unit_common_u16_get_cur;
//		audio_unit_mixer_out_speak_conf.__set_cur = &_audio_unit_mixer_out_speak_set_cur;
		audio_unit_mixer_out_speak_conf.__set_cur = &_audio_unit_mix_set_cur;
		audio_unit_mixer_out_speak_conf.__set_res = &_audio_unit_common_u16_set_res;
		audio_unit_ctrl_init(UDP_AC_MIX_ID, AUDIO_MIX_OUT_SPEAK, &audio_unit_mixer_out_speak_conf);

}

void audio_unit_add(uint8_t id, struct audio_unit_elist_t *unit) {
	if(audio.unit == NULL) {
		audio.unit = unit;
		} else {
		struct audio_unit_elist_t *ptr = audio.unit;
		while(ptr->next != NULL) ptr = ptr->next;
		ptr->next = unit;
	}

	unit->id = id;
	unit->next = NULL;
	unit->__controller = NULL;
}

void audio_unit_ctrl_add(uint8_t unit_id, uint8_t ctrl_id, struct audio_unit_ctrl_elist_t *controller) {
	struct audio_unit_elist_t *unit = audio_unit_elist_get(unit_id);
	if(unit == NULL) return;

	struct audio_unit_ctrl_elist_t *ctrl = unit->__controller;
	if(ctrl != NULL) {
		while(ctrl->next != NULL) ctrl = ctrl->next;
		ctrl->next = controller;
	} else
	unit->__controller = controller;
	controller->type = ctrl_id;
}

void audio_unit_ctrl_init(uint8_t unit_id, uint8_t ctrl_id, audio_unit_controller_t *conf) {
	struct audio_unit_elist_t *unit = audio_unit_elist_get(unit_id);
	if(unit == NULL) return;

	struct audio_unit_ctrl_elist_t *ctrl = audio_unit_ctrl_elist_get(unit, ctrl_id);
	if(ctrl == NULL) return;

	ctrl->controller = conf;
}

struct audio_unit_elist_t * audio_unit_elist_get(uint8_t id) {
	struct audio_unit_elist_t *ptr;
	if(audio.unit != NULL) ptr = audio.unit;
	else return NULL;
	while(ptr->id != id) {
		if(ptr->next != NULL) ptr = ptr->next;
		else return NULL;
	}

	return ptr;
}

struct audio_unit_ctrl_elist_t * audio_unit_ctrl_elist_get(struct audio_unit_elist_t *unit, uint8_t type) {
	struct audio_unit_ctrl_elist_t *ptr;
	if(unit->__controller != NULL) ptr = unit->__controller;
	else return NULL;
	while(ptr->type != type) {
		if(ptr->next != NULL) ptr = ptr->next;
		else return NULL;
	}

	return ptr;
}

audio_unit_controller_t * audio_unit_ctrl_get(uint8_t unit_id, uint8_t ctrl_id) {
	struct audio_unit_ctrl_elist_t *ctrl;
	struct audio_unit_elist_t *unit = audio_unit_elist_get(unit_id);
	if(unit != NULL) ctrl = audio_unit_ctrl_elist_get(unit, ctrl_id);
	else return NULL;

	if(ctrl != NULL) return ctrl->controller;

	return NULL;
}

/** 
*	Simple description of common callback functions.
**/

inline void _audio_unit_common_u16_get_min(const void * unit_conf) {
	audio_controller_conf_uint16_t *conf = ((audio_unit_controller_t *) unit_conf)->attribution;
	udp_send_setup((const uint8_t*) &conf->min, 2);
}

inline void _audio_unit_common_u16_get_max(const void * unit_conf) {
	audio_controller_conf_uint16_t *conf = ((audio_unit_controller_t *) unit_conf)->attribution;
	udp_send_setup((const uint8_t*) &conf->max, 2);
}

inline void _audio_unit_common_u16_get_cur(const void * unit_conf) {
	audio_controller_conf_uint16_t *conf = ((audio_unit_controller_t *) unit_conf)->attribution;
	udp_send_setup((const uint8_t*) &conf->cur, 2);
}

inline void _audio_unit_common_u16_get_res(const void * unit_conf) {
	audio_controller_conf_uint16_t *conf = ((audio_unit_controller_t *) unit_conf)->attribution;
	udp_send_setup((const uint8_t*) &conf->res, 2);
}

inline void _audio_unit_common_u16_set_res(const void * unit_conf) {
	udp_send_setup_zlp();
}

inline void _audio_unit_common_u16_set_cur(void *unit_conf, void *data) {
	audio_controller_conf_uint16_t *conf = ((audio_unit_controller_t *) unit_conf)->attribution;
	conf->cur = (uint16_t) *( (uint16_t *) data);
	udp_send_setup_zlp();
}

inline void _audio_unit_common_u8_get_min(const void * unit_conf) {
	audio_controller_conf_uint8_t *conf = ((audio_unit_controller_t *) unit_conf)->attribution;
	udp_send_setup((const uint8_t*) &conf->min, 1);
}

inline void _audio_unit_common_u8_get_max(const void * unit_conf) {
	audio_controller_conf_uint8_t *conf = ((audio_unit_controller_t *) unit_conf)->attribution;
	udp_send_setup((const uint8_t*) &conf->max, 1);
}

inline void _audio_unit_common_u8_get_cur(const void * unit_conf) {
	audio_controller_conf_uint8_t *conf = ((audio_unit_controller_t *) unit_conf)->attribution;
	udp_send_setup((const uint8_t*) &conf->cur, 1);
}

inline void _audio_unit_common_u8_get_res(const void * unit_conf) {
	audio_controller_conf_uint8_t *conf = ((audio_unit_controller_t *) unit_conf)->attribution;
	udp_send_setup((const uint8_t*) &conf->res, 1);
}

inline void _audio_unit_common_u8_set_res(const void * unit_conf) {
	udp_send_setup_zlp();
}

inline void _audio_unit_common_u8_set_cur(void *unit_conf, void *data) {
	audio_controller_conf_uint8_t *conf = ((audio_unit_controller_t *) unit_conf)->attribution;
	conf->cur = (uint8_t) *( (uint8_t *) data);
	udp_send_setup_zlp();
}

inline void _audio_unit_common_unsupport(const void * unit_conf) {
	udp_send_setup_stall();
}

/**
*	Local callback functions.
**/

/* Feature Unit ID 5. */
void _audio_unit_mic_vol_set_cur(void *unit_conf, void *data) {
	_audio_unit_common_u16_set_cur(unit_conf, data);
	pcm3793_pg3_gain((int8_t) (audio_unit_ctrl_mic_fu_conf_vol.cur / 0x100));

	_audio_unit_mic_log();
}

/* Feature Unit ID 6. */
inline void _audio_unit_phone_vol_set_cur(void *unit_conf, void *data) {
	uint8_t reg = 0x00;
	_audio_unit_common_u16_set_cur(unit_conf, data);

	if(audio_unit_ctrl_phone_fu_conf_mute.cur == 1) {
		reg |= PCM_R65_HMUR;
	}
	reg |= PCM_R65_HRV((int8_t) (audio_unit_ctrl_phone_fu_conf_vol.cur / 0x100));
	pcm3793_hpr_vol(reg);

	_audio_unit_phone_log();
}

inline void _audio_unit_phone_mute_set_cur(void *unit_conf, void *data) {
	uint8_t reg = 0x00;
	_audio_unit_common_u8_set_cur(unit_conf, data);

	if(audio_unit_ctrl_phone_fu_conf_mute.cur == 1) reg |= PCM_R65_HMUR;
	reg |= PCM_R65_HRV((int8_t) (audio_unit_ctrl_phone_fu_conf_vol.cur / 0x100));
	pcm3793_hpr_vol(reg);

	_audio_unit_phone_log();
}

/* Mixer Unit ID 7. */
inline void _audio_unit_mix_set_cur(void *unit_conf, void *data) {
	char message[48] = "Output channels: ";
	_audio_unit_common_u16_set_cur(unit_conf, data);

	uint8_t reg = 0x00;
	strcat(message, "mic - ");
	if(audio_unit_mixer_ctrl_in_mic_conf.cur != AUDIO_MIX_IN_MIC_MIN) {
//		reg |= PCM_R88_SW1 | PCM_R88_SW6;
		reg |= PCM_R88_SW6;
		strcat(message, "on; ");
	} else 
		strcat(message, "off; ");

	strcat(message, "phone - ");
	if(audio_unit_mixer_ctrl_in_phone_conf.cur != AUDIO_MIX_IN_PHONE_MIN) {
//		reg |= PCM_R88_SW2 | PCM_R88_SW5;
		reg |= PCM_R88_SW5;
		strcat(message, "on; ");
	} else 
		strcat(message, "off;");

	__AUDIO_DEBUG(LOG_LVL_HIGH, message);

	pcm3793_switch(reg);
}

/* Feature Unit ID 8. */
inline void _audio_unit_mic_preamph_set_cur(void *unit_conf, void *data) {
	_audio_unit_common_u16_set_cur(unit_conf, data);

	if(audio_unit_ctrl_mic_fu_preamph.cur == AUDIO_MIC_PRE_MIN) {
		__AUDIO_DEBUG(LOG_LVL_HIGH, "Volume: mic preamp off.");
		pcm3793_pg1pg5_down();
//		pcm3793_pg2pg6_down();
	} else {
		__AUDIO_DEBUG(LOG_LVL_HIGH, "Volume: mic preamp on.");
		pcm3793_pg1pg5_up();
//		pcm3793_pg2pg6_up();
	}
}

/* Feature Unit ID 9. */
inline void _audio_unit_mic_dig_amp_set_cur(void *unit_conf, void *data) {
	_audio_unit_common_u16_set_cur(unit_conf, data);

	uint8_t value = (uint8_t) 0x07 - (audio_unit_ctrl_mic_fu_dig_amp.cur * (-1) >> 8);
	pcm3793_pg5_gain(value);

	_audio_unit_mic_dig_amp_log();
}

inline void _audio_unit_phone_log(void) {
	char message[28] = "Volume: Output - ";
	char value[4];

	if(audio_unit_ctrl_phone_fu_conf_mute.cur == 1) {
		strcat(message, "(mute) ");
	}

	itoa(audio_unit_ctrl_phone_fu_conf_vol.cur * 100 / AUDIO_MASTER_VOL_MAX, value, 10);
	strcat(message, value);
	strcat(message, "%");
	__AUDIO_DEBUG(LOG_LVL_HIGH, message);
}

void _audio_unit_mic_log(void) {
	char message[32] = "Volume: Digital Mic - ";
	char value[4];
	itoa(audio_unit_ctrl_mic_fu_conf_vol.cur * 100 / AUDIO_MIC_VOL_MAX, value, 10);
	strcat(message, value);
	strcat(message, "%");
	__AUDIO_DEBUG(LOG_LVL_HIGH, message);
}

void _audio_unit_mic_dig_amp_log() {
	char message[28] = "Volume: Analog Mic - ";

	char value[6];
	// Do not be afraid of the next line. O_O
	itoa( 100 - ((int8_t) (audio_unit_ctrl_mic_fu_dig_amp.cur  >> 8) * (-1)) * 100 / 7, value, 10);
	strcat(message, value);
	strcat(message, "%");
	__AUDIO_DEBUG(LOG_LVL_HIGH, message);
}
