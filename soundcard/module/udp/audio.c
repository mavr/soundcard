/*
 * audio.c
 *
 * Created: 05.02.2016 14:25:51
 *  Author: ale
 */ 

#include <sam.h>
#include <stddef.h>
#include "include/udp.h"
#include "udp/udp_request.h"
#include "udp/audio.h"
#include "audio/audio.h"
#include "codec/pcm3793.h"

#include "udp-audio.h"

void udp_audio_controllers_proc(udp_setup_activity_t *udp_setup_pkg) {
	uint8_t tmp = (uint8_t) (UDP_AUDIO_FEATURE_UNIT_CS(udp_setup_pkg->pkg.wValue));
	audio_unit_controller_t *audio_ctrl = audio_unit_ctrl_get((uint8_t) (udp_setup_pkg->pkg.wIndex >> 8), tmp);
	if(audio_ctrl == NULL) {
		__UDP_DEBUG(LOG_LVL_LOW, "Error! Audio Control : wrong control.");
		udp_send_stall(&ep_control);
	}
	udp_setup_pkg->object.source = (void *) audio_ctrl;
	switch(udp_setup_pkg->pkg.bRequest) {

		case UDP_bRequest_SET_RES :
			udp_send_setup_stall();
			//udp_setup_pkg->callback = audio_ctrl->__set_res;
//			__UDP_DEBUG(LOG_LVL_LOW, "SET_RES");
			break;

		case UDP_bRequest_SET_CUR :
			udp_setup_pkg->callback = &_udp_ac_set_cur;
//			__UDP_DEBUG(LOG_LVL_LOW, "SET_CUR");
			break;

		case UDP_bRequest_GET_CUR :
			//udp_setup_pkg->callback = audio_ctrl->__get_cur;
			audio_ctrl->__get_cur(audio_ctrl);
//			__UDP_DEBUG(LOG_LVL_LOW, "GET_CUR");
			break;

		case UDP_bRequest_GET_MIN :
			//udp_setup_pkg->callback = audio_ctrl->__get_min;
			audio_ctrl->__get_min(audio_ctrl);
//			__UDP_DEBUG(LOG_LVL_LOW, "GET_MIN");
			break;

		case UDP_bRequest_GET_MAX :
			//udp_setup_pkg->callback = audio_ctrl->__get_max;
			audio_ctrl->__get_max(audio_ctrl);
//			__UDP_DEBUG(LOG_LVL_LOW, "GET_MAX");
			break;

		case UDP_bRequest_GET_RES :
			//udp_setup_pkg->callback = audio_ctrl->__get_res;
			audio_ctrl->__get_res(audio_ctrl);
//			__UDP_DEBUG(LOG_LVL_LOW, "GET_RES");
			break;

		case UDP_bRequest_SET_MIN :
		case UDP_bRequest_SET_MAX :
		case UDP_bRequest_SET_MEM :
		case UDP_bRequest_GET_MEM :
		case UDP_bRequest_GET_STAT :
			__UDP_DEBUG(LOG_LVL_LOW, "Audio Control : request doesn't support.");
			udp_send_zlp(&ep_control);
			break;

		default:
			__UDP_DEBUG(LOG_LVL_LOW, "Error! Audio Control : unknown request.");
			udp_send_zlp(&ep_control);
			break;
	}

////TODO: remove this debug info
	//switch((uint8_t) (udp_setup_pkg->pkg.wIndex >> 8)) {
		//case UDP_AC_PHONE_FU_ID :
////			__UDP_DEBUG(LOG_LVL_HIGH, "Request to phone volume.");
			//break;
		//case UDP_AC_MIC_FU_ID :
////			__UDP_DEBUG(LOG_LVL_HIGH, "Request to microphone volume.");
			//break;
		//case UDP_AC_MIX_ID:
////			__UDP_DEBUG(LOG_LVL_HIGH, "Request to mixer.");
			//break;
		//default:
			//__UDP_DEBUG(LOG_LVL_HIGH, "Request to uknown selector.");
			//break;
	//}
}
	
void udp_audio_fu_volume_cs(udp_setup_activity_t *udp_setup_pkg) {

}

void udp_audio_fu_mute_cs(udp_setup_activity_t *udp_setup_pkg) {
	
}

void _udp_ac_set_res(void) {
	udp_send_zlp(&ep_control);
}

void _udp_ac_set_cur(udp_request_callback_t *object) {
//	int16_t volume = *( (int16_t *) udp_setup_pkg.data);
//	static uint8_t volume = 0x00;
//	pcm3793_dar(PCM_R65_HRV((int8_t) (volume / 0x100)));
//	udp_send_zlp(&ep_control);
	audio_unit_controller_t *ptr = (audio_unit_controller_t *) object->source;
	ptr->__set_cur(object->source, object->data);
}
