/*
 * udp_audio.h
 *
 * Created: 14.03.2016 14:03:29
 *  Author: ale
 */ 


#ifndef UDP_AUDIO_H_
#define UDP_AUDIO_H_

#define UDP_DESCRIPTOR_DEVICE_SIZE		0x12
//#define UDP_DESCRIPTOR_CONF_SIZE		0xd9
#define UDP_DESCRIPTOR_CONF_SIZE		0xf8
//#define UDP_DESCRIPTOR_CONF_SIZE		0xdd
#define UDP_DESCRIPTOR_HID_REPORT_SIZE	45

#define UDP_AC_INTERFACE				0
#define UDP_AS_IN_INTERFACE				1
#define UDP_AS_OUT_INTERFACE			2
#define UDP_HID_INTERFACE				3

#define UDP_AC_PHONE_FU_ID					6
#define UDP_AC_MIC_FU_ID					5
#define UDP_AC_MIX_ID						7
#define UDP_AC_MIC_PRE_FU_ID				8
#define UDP_AC_MIC_AMP_FU_ID				9




#endif /* UDP-AUDIO_H_ */