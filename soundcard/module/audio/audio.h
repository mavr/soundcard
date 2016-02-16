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
#define AUDIO_PHONE_VOL_MAX			0x600
#define AUDIO_PHONE_VOL_MIN			0xba00
#define AUDIO_PHONE_VOL_RES			0x0100
#define AUDIO_PHONE_VOL_CUR_DEFAULT	0x00

/* Mute volume unit parameters. */
#define AUDIO_PHONE_MUTE_MAX			0x01
#define AUDIO_PHONE_MUTE_MIN			0x00
#define AUDIO_PHONE_MUTE_RES			0x01
#define AUDIO_PHONE_MUTE_CUR_DEFAULT	0x00

/* Microphone  */
/* Volume unit parameters. */
#define AUDIO_MIC_VOL_MAX			0x0ff0
#define AUDIO_MIC_VOL_MIN			0x00
#define AUDIO_MIC_VOL_RES			0x01
#define AUDIO_MIC_VOL_CUR_DEFAULT	0x00

/* Mute volume unit parameters. */
#define AUDIO_MIC_MUTE_MAX			0x01
#define AUDIO_MIC_MUTE_MIN			0x00
#define AUDIO_MIC_MUTE_RES			0x01
#define AUDIO_MIC_MUTE_CUR_DEFAULT	0x00


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

/** Possible elements **/ 
/* Feature  unit ID5 */
audio_unit_controller_t audio_unit_mic_fu_conf_vol;
	audio_controller_conf_uint16_t audio_unit_ctrl_mic_fu_conf_vol;
	void _audio_unit_mic_fu_vol_get_min();
	void _audio_unit_mic_fu_vol_set_min();
	void _audio_unit_mic_fu_vol_get_max();
	void _audio_unit_mic_fu_vol_set_max();
	void _audio_unit_mic_fu_vol_get_cur();
	void _audio_unit_mic_fu_vol_set_cur();
audio_unit_controller_t audio_unit_mic_fu_conf_mute;
	audio_controller_conf_uint8_t audio_unit_ctrl_mic_fu_conf_mute;
	void _audio_unit_mic_fu_mute_get_min();
	void _audio_unit_mic_fu_mute_set_min();
	void _audio_unit_mic_fu_mute_get_max();
	void _audio_unit_mic_fu_mute_set_max();
	void _audio_unit_mic_fu_mute_get_cur();
	void _audio_unit_mic_fu_mute_set_cur();

/* Feature  unit ID6 */
audio_unit_controller_t audio_unit_phone_fu_conf_vol;
	audio_controller_conf_uint16_t audio_unit_ctrl_phone_fu_conf_vol;
	void _audio_unit_phone_fu_vol_get_min();
	void _audio_unit_phone_fu_vol_set_min();
	void _audio_unit_phone_fu_vol_get_max();
	void _audio_unit_phone_fu_vol_set_max();
	void _audio_unit_phone_fu_vol_get_cur();
	void _audio_unit_phone_fu_vol_set_cur();
audio_unit_controller_t audio_unit_phone_fu_conf_mute;
	audio_controller_conf_uint8_t audio_unit_ctrl_phone_fu_conf_mute;
	void _audio_unit_phone_fu_mute_get_min();
	void _audio_unit_phone_fu_mute_set_min();
	void _audio_unit_phone_fu_mute_get_max();
	void _audio_unit_phone_fu_mute_set_max();
	void _audio_unit_phone_fu_mute_get_cur();
	void _audio_unit_phone_fu_mute_set_cur();

/* Mixer unit ID7 */
audio_unit_controller_t audio_unit_mixer_conf;
	

/** Elements routing **/
/* This is head of the elements list. */
struct audio_unit_elist_t *audio_unit_list;

/* Describing Feature Unit id 5. */
struct audio_unit_elist_t audio_mic_fu;  // FU for phone (volume, mute)
/* Controllers: */
	/* Volume */
	struct audio_unit_ctrl_elist_t audio_unit_phone_fu_ctrl_vol;
	/* Mute */
	struct audio_unit_ctrl_elist_t audio_unit_phone_fu_ctrl_mute;

/* Describing Feature Unit id 6. */	
struct audio_unit_elist_t audio_phone_fu;	// FU for mic	(volume, mute)
/* Controllers and configurations: */
	/* Volume */
	struct audio_unit_ctrl_elist_t audio_unit_mic_fu_ctrl_vol;
	/* Mute */
	struct audio_unit_ctrl_elist_t audio_unit_mic_fu_ctrl_mute;

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

/* Speaker feature */
void _audio_unit_phone_vol_set_cur(void *unit_conf, void *data);
void _audio_unit_phone_mute_set_cur(void *unit_conf, void *data);

/* Microphone feature */
void _audio_unit_mic_vol_set_cur(void *unit_conf, void *data);
void _audio_unit_mic_mic_set_cur(void *unit_conf, void *data);

#endif /* AUDIO_H_ */