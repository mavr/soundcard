/*
 * audio.h
 *
 * Created: 29.01.2016 15:56:54
 *  Author: ale
 */ 


#ifndef _M_AUDIO_H_
#define _M_AUDIO_H_

#include <sam.h>
#include "core/syslog.h"

#ifdef UART_DEBUG
	#define __AUDIO_DEBUG(lvl,msg) {\
		syslog_prefix("[audio]\t"); \
	__DEBUG(lvl,msg); }
#else
	#define __AUDIO_DEBUG(lvl,msg)
#endif

/* Phone */
/* Volume unit parameters. */
#define AUDIO_PHONE_VOL_MAX			0x3f00
#define AUDIO_PHONE_VOL_MIN			0x0000
#define AUDIO_PHONE_VOL_RES			0x0100
#define AUDIO_PHONE_VOL_CUR_DEFAULT	0x1a00

/* Mute volume unit parameters. */
#define AUDIO_PHONE_MUTE_MAX			0x01
#define AUDIO_PHONE_MUTE_MIN			0x00
#define AUDIO_PHONE_MUTE_RES			0x01
#define AUDIO_PHONE_MUTE_CUR_DEFAULT	0x00

/* Microphone  */
/* Volume unit parameters. */
#define AUDIO_MIC_VOL_MAX			0x2a00
#define AUDIO_MIC_VOL_MIN			0x00
#define AUDIO_MIC_VOL_RES			0x100
#define AUDIO_MIC_VOL_CUR_DEFAULT	0x40

/* Mute volume unit parameters. */
#define AUDIO_MIC_MUTE_MAX			0x01
#define AUDIO_MIC_MUTE_MIN			0x00
#define AUDIO_MIC_MUTE_RES			0x01
#define AUDIO_MIC_MUTE_CUR_DEFAULT	0x00

/* Mixer */
#define AUDIO_MIX_OUT_SPEAK			0x01
#define AUDIO_MIX_IN_PHONE			0x02
#define AUDIO_MIX_IN_MIC			0x03

#define AUDIO_MIX_IN_PHONE_MAX		0x00
#define AUDIO_MIX_IN_PHONE_MIN		0x8001
#define AUDIO_MIX_IN_PHONE_CUR		0x00
#define AUDIO_MIX_IN_PHONE_RES		0x7fff

#define AUDIO_MIX_IN_MIC_MAX		0x00
#define AUDIO_MIX_IN_MIC_MIN		0x8001
#define AUDIO_MIX_IN_MIC_CUR		0x00
#define AUDIO_MIX_IN_MIC_RES		0x7fff

#define AUDIO_MIX_OUT_SPEAK_MAX		0x00
#define AUDIO_MIX_OUT_SPEAK_MIN		0x8001
#define AUDIO_MIX_OUT_SPEAK_CUR		0x00
#define AUDIO_MIX_OUT_SPEAK_RES		0x7fff



#ifndef AUDIO_UNIT_COUNT
	//#error "You must define number of audio units in AUDIO_UNIT_COUNT variable"
	#define AUDIO_UNIT_COUNT 0
#endif

typedef struct {
	uint8_t cur;
	uint8_t res;
	uint8_t min;
	uint8_t max;
	// ? stat;
} audio_controller_conf_uint8_t;

typedef struct {
	uint16_t cur;
	uint16_t res;
	uint16_t min;
	uint16_t max;
	// ? stat;
} audio_controller_conf_uint16_t;

typedef struct {
	void *attribution;

	void (*__get_min)(const void *);
	void (*__set_min)(const void *);

	void (*__get_max)(const void *);
	void (*__set_max)(const void *);

	void (*__get_res)(const void *);
	void (*__set_res)(const void *);

	void (*__get_cur)(const void *);
	void (*__set_cur)(void *, void *);

	void (*__get_mem)(const void *);
	void (*__set_mem)(const void *);
	
	void (*__get_stat)(const void *);
} audio_unit_controller_t;

struct audio_unit_ctrl_elist_t {
	struct audio_unit_ctrl_elist_t *next;
	uint8_t type;
	audio_unit_controller_t *controller;
};

struct audio_unit_elist_t {
	struct audio_unit_elist_t *next;
	uint8_t id;
	struct audio_unit_ctrl_elist_t *__controller;
} ;

/** Elements routing **/
/* This is head of the elements list. */
struct audio_unit_elist_t *audio_unit_list;

/* Describing Feature Unit ID 5. */
struct audio_unit_elist_t audio_mic_fu;  // FU for phone (volume, mute)
	/* Controllers: */
	/* Volume */
	struct audio_unit_ctrl_elist_t audio_unit_mic_fu_ctrl_vol;
		audio_unit_controller_t audio_unit_mic_fu_conf_vol;
		audio_controller_conf_uint16_t audio_unit_ctrl_mic_fu_conf_vol;
		void _audio_unit_mic_vol_set_cur(void *unit_conf, void *data);
	/* Mute */
	struct audio_unit_ctrl_elist_t audio_unit_mic_fu_ctrl_mute;
		audio_unit_controller_t audio_unit_mic_fu_conf_mute;
		audio_controller_conf_uint8_t audio_unit_ctrl_mic_fu_conf_mute;
		void _audio_unit_mic_mute_set_cur(void *unit_conf, void *data);

/* Describing Feature Unit ID 6. */
struct audio_unit_elist_t audio_phone_fu;	// FU for mic	(volume, mute)
	/* Controllers and configurations: */
	/* Volume */
	struct audio_unit_ctrl_elist_t audio_unit_phone_fu_ctrl_vol;
		audio_unit_controller_t audio_unit_phone_fu_conf_vol;
		audio_controller_conf_uint16_t audio_unit_ctrl_phone_fu_conf_vol;
			void _audio_unit_phone_vol_set_cur(void *unit_conf, void *data);
	/* Mute */
	struct audio_unit_ctrl_elist_t audio_unit_phone_fu_ctrl_mute;
		audio_unit_controller_t audio_unit_phone_fu_conf_mute;
		audio_controller_conf_uint8_t audio_unit_ctrl_phone_fu_conf_mute;
			void _audio_unit_phone_mute_set_cur(void *unit_conf, void *data);

/* Describing Feature Unit id 7. */
struct audio_unit_elist_t audio_mix;
/* Controllers and configurations: */
	/* Incoming line phone from DAC. */
	struct audio_unit_ctrl_elist_t audio_mix_in_phone;
		audio_unit_controller_t audio_unit_mixer_in_phone_conf;
		audio_controller_conf_uint16_t audio_unit_mixer_ctrl_in_phone_conf;
	struct audio_unit_ctrl_elist_t audio_mix_in_mic;
	/* Incoming line form analog microphone. */
		audio_unit_controller_t audio_unit_mixer_in_mic_conf;
		audio_controller_conf_uint16_t audio_unit_mixer_ctrl_in_mic_conf;
	/* Outcoming line to speaker. */
	struct audio_unit_ctrl_elist_t audio_mix_out_speaker;
		audio_unit_controller_t audio_unit_mixer_out_speak_conf;
		audio_controller_conf_uint16_t audio_unit_mixer_out_ctrl_speak_conf;
		/* General _set_current() callback function. */
		void _audio_unit_mix_set_cur(void *unit_conf, void *data);

/* Configuring, starting ssc, spi and pcm systems. */
int audio_system(void);
void audio_controls_set_list(void);

void audio_volume_up(void);
void audio_volume_down(void);

uint16_t audio_get_current(void);
void audio_set_current(uint16_t value);

/* Audio system struct. */
typedef struct {
	// Head of units list.
	struct audio_unit_elist_t *unit;
} audio_t;

audio_t audio;

/** new ideas **/
/* create api */
void audio_unit_add(uint8_t id, struct audio_unit_elist_t *unit);
void audio_unit_ctrl_add(uint8_t unit_id, uint8_t ctrl_id, struct audio_unit_ctrl_elist_t *controller);
void audio_unit_ctrl_init(uint8_t unit_id, uint8_t ctrl_id, audio_unit_controller_t *conf);

struct audio_unit_elist_t * audio_unit_elist_get(uint8_t id);
struct audio_unit_ctrl_elist_t * audio_unit_ctrl_elist_get(struct audio_unit_elist_t *unit, uint8_t id);
audio_unit_controller_t * audio_unit_ctrl_get(uint8_t unit_id, uint8_t ctrl_id);

/** Callbacks **/
/* Audio units functions. */
/* Common */
void _audio_unit_common_u16_get_min(const void *unit_conf);
void _audio_unit_common_u16_set_min(const void *unit_conf);

void _audio_unit_common_u16_get_max(const void *unit_conf);
void _audio_unit_common_u16_set_max(const void *unit_conf);

void _audio_unit_common_u16_get_res(const void *unit_conf);
void _audio_unit_common_u16_set_res(const void *unit_conf);

void _audio_unit_common_u16_get_cur(const void *unit_conf);
void _audio_unit_common_u16_set_cur(void *unit_conf, void *data);

void _audio_unit_common_u8_get_min(const void *unit_conf);
void _audio_unit_common_u8_set_min(const void *unit_conf);

void _audio_unit_common_u8_get_max(const void *unit_conf);
void _audio_unit_common_u8_set_max(const void *unit_conf);

void _audio_unit_common_u8_get_res(const void *unit_conf);
void _audio_unit_common_u8_set_res(const void *unit_conf);

void _audio_unit_common_u8_get_cur(const void *unit_conf);
void _audio_unit_common_u8_set_cur(void *unit_conf, void *);

void _audio_unit_common_unsupport(const void *unit_conf);


/* Converters */
uint8_t _audio_phone_vol_usb_to_pcm(uint16_t value);

#endif /* AUDIO_H_ */