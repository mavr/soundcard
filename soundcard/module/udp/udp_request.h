/*
 * udp_request.h
 *
 * Created: 13.03.2015 15:51:40
 *  Author: ale
 */ 


#ifndef UDP_REQUEST_H_
#define UDP_REQUEST_H_

/** bmRequestType **/
/* Type */
#define UDP_bmRequestType_Type_STANDART		0x00
#define UDP_bmRequestType_Type_CLASS		0x01
#define UDP_bmRequestType_Type_VENDOR		0x02
#define UDP_bmRequestType_Type_RESERVED		0x03

/* Recipient */
#define UDP_bmRequestType_Recipient_DEV		0x00
#define UDP_bmRequestType_Recipient_INT		0x01
#define UDP_bmRequestType_Recipient_EP		0x02
#define UDP_bmRequestType_Recipient_OTHER	0x03

/* Descriptor */
#define UDP_bmRequestType_Descriptor_HID	0x21
#define UDP_bmRequestType_Descriptor_REPORT	0x22
#define UDP_bmRequestType_Descriptor_PHY	0x23


/** bRequest **/
/* Device */
#define UDP_bRequest_GET_STATUS				0x00
#define UDP_bRequest_CLEAR_FEATURE			0x01
#define UDP_bRequest_SET_FEATURE			0x03
#define UDP_bRequest_SET_ADDRESS			0x05
#define UDP_bRequest_GET_DESCRIPTOR			0x06
#define UDP_bRequest_SET_DESCRIPTOR			0x07
#define UDP_bRequest_GET_CONFIGURATION		0x08
#define UDP_bRequest_SET_CONFIGURATION		0x09
#define UDP_bRequest_GET_INTERFACE			0x0a
#define UDP_bRequest_SET_INTERFACE			0x0b
#define UDP_bRequest_SYNCH_FRAME			0x0c

/* Class */
#define UDP_bRequest_GET_REPORT				0x01
#define UDP_bRequest_SET_REPORT				0x09

/** wValue **/
/* Descriptor types */
#define UDP_wValue_DESCRIPTORT_DEV					0x01
#define UDP_wValue_DESCRIPTORT_CONF					0x02
#define UDP_wValue_DESCRIPTORT_STR					0x03
#define UDP_wValue_DESCRIPTORT_INT					0x04
#define UDP_wValue_DESCRIPTORT_EP					0x05
#define UDP_wValue_DESCRIPTORT_DEV_QUAL				0x06
#define UDP_wValue_DESCRIPTORT_OTHER_SPEED_CONF		0x07
#define UDP_wValue_DESCRIPTORT_INT_POWER			0x08
#define UDP_wValue_DESCRIPTORT_HID					0x21
#define UDP_wValue_DESCRIPTORT_HID_REPORT			0x22

/* Feature Selections */
#define UDP_wValue_FS_DEV_REMOTE_WAKEUP		0x01
#define UDP_wValue_FS_EP_HALT				0x00
#define UDP_wValue_FS_TEST_MODE				0x02

#endif /* UDP_REQUEST_H_ */