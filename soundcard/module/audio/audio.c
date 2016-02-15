/*
 * audio.c
 *
 * Created: 29.01.2016 16:07:40
 *  Author: ale
 */ 

#include <sam.h>
#include <stddef.h>
#include "audio/audio.h"
#include "udp/udp-audio.h"
#include "udp/audio.h"
#include "ssc/ssc.h"
#include "spi/spi.h"
#include "codec/pcm3793.h"

int audio_system() {
	/* Turn on spi for control pcm device. */
	spi_system();
	
	/* Turn on ssc interface for transmit audio data to and from pcm device. */
	ssc_system();
	
	/* Configuring default codec state. */
	audio_controls_set_list();
	
	/* pcm.. wake up guy. */
	pcm3793_init();
	
	/* Starting ssc line for transmit playback and capture interfaces. */
	ssc_irq();
	
	return 0;
}

void audio_controls_set_list(void) {
	audio_unit_add(UDP_AC_MIC_FU_ID, &audio_mic_fu);
	audio_unit_add(UDP_AC_PHONE_FU_ID, &audio_phone_fu);
	
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

		audio_unit_mic_fu_conf_mute.configuration = (void *) &audio_unit_ctrl_mic_fu_conf_mute;
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

		audio_unit_mic_fu_conf_vol.configuration = (void *) &audio_unit_ctrl_mic_fu_conf_vol;
		audio_unit_mic_fu_conf_vol.__get_min = &_audio_unit_common_u16_get_min;
		audio_unit_mic_fu_conf_vol.__get_max = &_audio_unit_common_u16_get_max;
		audio_unit_mic_fu_conf_vol.__get_res = &_audio_unit_common_u16_get_res;
		audio_unit_mic_fu_conf_vol.__get_cur = &_audio_unit_common_u16_get_cur;
		audio_unit_mic_fu_conf_vol.__set_cur = &_audio_unit_common_u16_set_cur;
		audio_unit_ctrl_init(UDP_AC_MIC_FU_ID, UDP_AUDIO_CS_VOLUME_CONTROL, &audio_unit_mic_fu_conf_vol);

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

		audio_unit_phone_fu_conf_mute.configuration = (void *) &audio_unit_ctrl_phone_fu_conf_mute;
		audio_unit_phone_fu_conf_mute.__get_min = &_audio_unit_common_u8_get_min;
		audio_unit_phone_fu_conf_mute.__get_max = &_audio_unit_common_u8_get_max;
		audio_unit_phone_fu_conf_mute.__get_res = &_audio_unit_common_u8_get_res;
		audio_unit_phone_fu_conf_mute.__get_cur = &_audio_unit_common_u8_get_cur;
		audio_unit_phone_fu_conf_mute.__set_cur = &_audio_unit_phone_mute_set_cur;
		
		audio_unit_ctrl_init(UDP_AC_PHONE_FU_ID, UDP_AUDIO_CS_MUTE_CONTROL, &audio_unit_phone_fu_conf_mute);

	/* Adding phone volume unit control */
	audio_unit_ctrl_add(UDP_AC_PHONE_FU_ID, UDP_AUDIO_CS_VOLUME_CONTROL, &audio_unit_phone_fu_ctrl_vol);
		audio_unit_ctrl_phone_fu_conf_vol.cur = AUDIO_PHONE_MUTE_CUR_DEFAULT;
		audio_unit_ctrl_phone_fu_conf_vol.max = AUDIO_PHONE_VOL_MAX;
		audio_unit_ctrl_phone_fu_conf_vol.min = AUDIO_PHONE_VOL_MIN;
		audio_unit_ctrl_phone_fu_conf_vol.res = AUDIO_PHONE_VOL_RES;

		audio_unit_phone_fu_conf_vol.configuration = (void *) &audio_unit_ctrl_phone_fu_conf_vol;
		audio_unit_phone_fu_conf_vol.__get_min = &_audio_unit_common_u16_get_min;
		audio_unit_phone_fu_conf_vol.__get_max = &_audio_unit_common_u16_get_max;
		audio_unit_phone_fu_conf_vol.__get_res = &_audio_unit_common_u16_get_res;
		audio_unit_phone_fu_conf_vol.__get_cur = &_audio_unit_common_u16_get_cur;
		audio_unit_phone_fu_conf_vol.__set_cur = &_audio_unit_phone_vol_set_cur;
		audio_unit_phone_fu_conf_vol.__set_res = &_audio_unit_common_u16_set_res;
		audio_unit_ctrl_init(UDP_AC_PHONE_FU_ID, UDP_AUDIO_CS_VOLUME_CONTROL, &audio_unit_phone_fu_conf_vol);
}

inline void _audio_unit_common_u16_get_min(const void * unit_conf) {
	audio_controller_conf_uint16_t *conf = ((audio_unit_controller_t *) unit_conf)->configuration;
	udp_send_setup((const uint8_t*) &conf->min, 2);
}

inline void _audio_unit_common_u16_get_max(const void * unit_conf) {
	audio_controller_conf_uint16_t *conf = ((audio_unit_controller_t *) unit_conf)->configuration;
	udp_send_setup((const uint8_t*) &conf->max, 2);
}

inline void _audio_unit_common_u16_get_cur(const void * unit_conf) {
	audio_controller_conf_uint16_t *conf = ((audio_unit_controller_t *) unit_conf)->configuration;
	udp_send_setup((const uint8_t*) &conf->cur, 2);
}

inline void _audio_unit_common_u16_get_res(const void * unit_conf) {
	audio_controller_conf_uint16_t *conf = ((audio_unit_controller_t *) unit_conf)->configuration;
	udp_send_setup((const uint8_t*) &conf->res, 2);
}

inline void _audio_unit_common_u16_set_res(const void * unit_conf) {
	udp_send_setup_zlp();
}

inline void _audio_unit_common_u16_set_cur(void *unit_conf, void *data) {
	audio_controller_conf_uint16_t *conf = ((audio_unit_controller_t *) unit_conf)->configuration;
	conf->cur = (uint16_t) *( (uint16_t *) data);
	udp_send_setup_zlp();
}

inline void _audio_unit_common_u8_get_min(const void * unit_conf) {
	audio_controller_conf_uint8_t *conf = ((audio_unit_controller_t *) unit_conf)->configuration;
	udp_send_setup((const uint8_t*) &conf->min, 1);
}

inline void _audio_unit_common_u8_get_max(const void * unit_conf) {
	audio_controller_conf_uint8_t *conf = ((audio_unit_controller_t *) unit_conf)->configuration;
	udp_send_setup((const uint8_t*) &conf->max, 1);
}

inline void _audio_unit_common_u8_get_cur(const void * unit_conf) {
	audio_controller_conf_uint8_t *conf = ((audio_unit_controller_t *) unit_conf)->configuration;
	udp_send_setup((const uint8_t*) &conf->cur, 1);
}

inline void _audio_unit_common_u8_get_res(const void * unit_conf) {
	audio_controller_conf_uint8_t *conf = ((audio_unit_controller_t *) unit_conf)->configuration;
	udp_send_setup((const uint8_t*) &conf->res, 1);
}

inline void _audio_unit_common_u8_set_res(const void * unit_conf) {
	udp_send_setup_zlp();
}

inline void _audio_unit_common_u8_set_cur(void *unit_conf, void *data) {
	audio_controller_conf_uint8_t *conf = ((audio_unit_controller_t *) unit_conf)->configuration;
	conf->cur = (uint8_t) *( (uint8_t *) data);
	udp_send_setup_zlp();
}

inline void _audio_unit_common_unsupport(const void * unit_conf) {
	udp_send_setup_stall();
}

inline void _audio_unit_phone_vol_set_cur(void *unit_conf, void *data) {
	_audio_unit_common_u16_set_cur(unit_conf, data);
	pcm3793_dar(PCM_R65_HRV((int8_t) (*((uint16_t *) data) / 0x100)));
}

inline void _audio_unit_phone_mute_set_cur(void *unit_conf, void *data) {
	_audio_unit_common_u8_set_cur(unit_conf, data);
	if(*((uint8_t *) data) != 0) pcm3793_dar(PCM_R65_HMUR);
//	else pcm3793_dar(PCM_R65_HMUR);
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
