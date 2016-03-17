/*
 * udp_audio.h
 *
 * Created: 14.04.2015 15:55:28
 *  Author: ale
 */ 


#include <sam.h>
#include "udp-audio.h"

typedef struct { // __attribute__((__packed))
	uint8_t bLenght;
	
} udp_configuration_desc_t;

typedef struct {
	
} udp_audio_conf_desc_t;



//#define UDP_DESCRIPTOR_HID_REPORT_SIZE	52

/**
	--- Descriptor Hierarchy ---
	Device Descriptor

	Configuration Descriptor
	** interface 0 **
		-- AC interface Descriptor
			-- Header Descriptor
			-- Input Terminal (Microphone)
			-- Output Terminal
	** interface 1 **
		-- AS interface alternate 0
		-- AS interface alternate 1
			-- General
			-- Type I Format
		-- Endpoint Descriptor
			-- Endpoint specific Descriptor
		

**/

const uint8_t udp_dev_descriptor[] = {
	/* Device Descriptor */
	UDP_DESCRIPTOR_DEVICE_SIZE, // bLength
	0x01, // bDescriptorType
	0x00, // bcdUSB
	0x01,
	0x00, // bDeviceClass - vendor specific
	0x00, // bDeviceSubClass - vendor specific
	0x00, // bDeviceProtocol - vendor specific
	0x40, // bMaxPacketSize0 - 64
	0xCA, // idVendor
	0xAD,
	0x08, // idProduct
	0x20,
	0x00, // bcdDevice
	0x01,
	0x01, // iManufacturer
	0x02, // iProduct
	0x03, // iSerialNumber
	0x01  // bNumConfigurations
};

const uint8_t udp_conf_descriptor[] = {
	/* Configuration Descriptor */
	0x09, // bLength
	0x02, // bDescriptorType
	UDP_DESCRIPTOR_CONF_SIZE, // wTotalLength (217)
	0x00,
	0x04, // bNumInterface
	0x01, // bConfigurationValue
	0x00, // iConfiguration
	0x80, // bmAttributes
	0xFA, // bMaxPower

	/* Audio Control Interface Descriptor */
		0x09, // bLenght
		0x04, // bDescriptorType
		UDP_AC_INTERFACE, // bInterfaceNumber
		0x00, // bAlternateSetting (none)
		0x00, // bNumEndpoints (0) - describes in standard interface desc for AC dev.
		0x01, // bInterfaceClass (AUDIO)
		0x01, // bInterfaceSubClass (AUDIO_CONTROL)
		0x00, // bInterfaceProtocol (none)
		0x00, // iInterface - address of string descriptor for describe this interface (none)

	/* Interface Header Audio Class Descriptor */		
			0x0A, // bLength (9)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x01, // bDescriptorSubtype (HEADER)
			0x00, // bcdADC (1.0)
			0x01,
			0x45, // wTotalLength ()
			0x00,
			0x02, // bInCollection (1 streaming interface)
			0x01, // baInterfaceNr (interface 1 is stream)
			0x02, // baInterfaceNr (interface 2 is stream)

	/* Input terminal Audio Class Descriptor */
			0x0C, // bLength (12)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x02, // bDescriptorSubtype (INPUT_TERMINAL)
			UDP_AC_TERM_IN_MIC_ID, // bTerminalID (1)
			0x01, // wTerminalType (microphone)
			0x02,
			0x00, // bAssocTerminal (none)
			0x01, // bNrChannels (1)
			0x01, // wChannelConfig (mono)
			0x00,
			0x00, // iChannelNames (none)
			0x00, // iTerminal (none)

	/* Input terminal Audio Class Descriptor */
			0x0C, // bLength (12)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x02, // bDescriptorSubtype (INPUT_TERMINAL)
			UDP_AC_TERM_IN_USB_ID, // bTerminalID (2)
			0x01, // wTerminalType (usb streaming)
			0x01,
			0x00, // bAssocTerminal (none)
			0x02, // bNrChannels (1)
			0x01, // wChannelConfig (mono)
			0x00,
			0x00, // iChannelNames (none)
			0x00, // iTerminal (none)

	/* Output terminal Audio Class Descriptor */
			0x09, // bLength (9)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x03, // bDescriptorSubtype (OUTPUT_TERMINAL)
			UDP_AC_TERM_OUT_USB_ID, // bTerminalID (3)
			0x01, // wTerminalType (usb streaming)
			0x01,
			0x00, // bAssocTerminal (none)
			UDP_AC_MIC_FU_ID, // bSourceID (5)
			0x00, // iTerminal (none)

	/* Output terminal Audio Class Descriptor */
			0x09, // bLength (9)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x03, // bDescriptorSubtype (OUTPUT_TERMINAL)
			UDP_AC_TERM_OUT_PHONE_ID, // bTerminalID (4)
			0x01, // wTerminalType (speaker)
			0x03,
			0x00, // bAssocTerminal (none)
			UDP_AC_PHONE_FU_ID, // bSourceID (6)
			0x00, // iTerminal (none)

		/* Feature unit Audio Class Descriptor */
		///* Feature unit Audio Class Descriptor */
			//0x09, // bLength (9)
			//0x24, // bDescriptorType (CS_INTERFACE)
			//0x06, // bDescriptorSubtype (FEATURE_UNIT)
			//UDP_AC_MIC_PRE_FU_ID, // bUnitID (8)
			//UDP_AC_TERM_IN_MIC_ID, // bSourceID (1)
			//0x01, // bControlSize (1)
			//0x02, // bmaContorls(0) Volume
			//0x00, // bmaContorls(1)
			//0x06, // iTerminal (none)

	/* Feature unit Audio Class Descriptor */
			0x09, // bLength (9)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x06, // bDescriptorSubtype (FEATURE_UNIT)
			UDP_AC_MIC_FU_ID, // bUnitID (5)
			UDP_AC_TERM_IN_MIC_ID, // bSourceID ()
			0x01, // bControlSize (1)
			0x03, // bmaContorls(0) : Mute, Volume
			0x00, // bmaContorls(1)
			0x00, // iTerminal (none)

	/* Feature unit Audio Class Descriptor */
			0x09, // bLength (9)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x06, // bDescriptorSubtype (FEATURE_UNIT)
			UDP_AC_PHONE_FU_ID, // bUnitID (6)
			UDP_AC_TERM_IN_USB_ID, // bSourceID ()
			0x01, // bControlSize (1)
			0x03, // bmaContorls(0) : Mute, Volume
			0x00, // bmaContorls(1)
			0x05, // iTerminal (none)

	/* Feature unit Audio Class Descriptor */
			//0x09, // bLength (9)
			//0x24, // bDescriptorType (CS_INTERFACE)
			//0x06, // bDescriptorSubtype (FEATURE_UNIT)
			//UDP_AC_MIC_AMP_FU_ID, // bUnitID (9)
			//UDP_AC_MIC_PRE_FU_ID, // bSourceID ()
			//0x01, // bControlSize (1)
			//0x02, // bmaContorls(0) Volume
			//0x00, // bmaContorls(1)
			//0x07, // iTerminal (none)

	///* Mixer unit Audio Class Descriptor */
			//0x0d, // bLenght
			//0x24, // bDescriptorType (CS_INTERFACE)
			//0x04, // bDescriptorSubtype (MIXER_UNIT)
			//UDP_AC_MIX_ID, // bUnitID (7)
			//0x02, // bNrInPins (2)
			//UDP_AC_MIC_PRE_FU_ID, // baSourceID[0] (1)
			//UDP_AC_TERM_IN_USB_ID, // baSourceID[1]
			//0x01, // bNrChannels
			//0x01, // wChanelConfig
			//0x00,
			//0x00, // iChannelNames
			//0x60, // bmControls
			//0x04, // iMixer

	/** Interface number 1 **/
	/* Audio Stream interface descriptor */
		0x09, // bLenght
		0x04, // bDescriptorType (interface)
		UDP_AS_IN_INTERFACE, // bInterfaceNumber
		0x00, // bAlternateSettings
		0x00, // bNumEndpoints
		0x01, // bInterfaceClass (audio)
		0x02, // bInterfaceSubClass (audio_streaming)
		0x00, // bInterfaceProtocol (none)
		0x00, // iInterface (none)

	/* Alternate Audio Interface Descriptor */
		0x09, // bLenght
		0x04, // bDescriptorType (interface)
		UDP_AS_IN_INTERFACE, // bInterfaceNumber
		0x01, // bAlternateSetting
		0x01, // bNumEndpoints
		0x01, // bInterfaceClass
		0x02, // bInterfaceSubClass
		0x00, // bInterfaceProtocol (none) 
		0x00, // iInterface (none)

	/* Audio Stream Audio Class Descriptor */
			0x07, // bLenght
			0x24, // bDescriptorType (CS_INTERFACE)
			0x01, // bDescriptorSubtype (AS_GENERAL)
			UDP_AC_TERM_OUT_USB_ID, // bTerminalLink (terminal 1)
			0x01, // bDelay (none)
			0x01, // wFormatTag (PCM format)
			0x00,

	/* Format Type Audio Descriptor */
			0x0B, // bLength (11)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x02, // bDescriptorSubtype (FORMAT_TYPE)
			0x01, // bFormatType (TYPE_I)
			0x01, // bNrChannels (1)
			0x02, // bSubFrameSize (2)
			// The next field should be 10, but 16 works with more standard software
			0x10, // bBitResolution (16)
			0x01, // bSamFreqType (1 sampling frequency)
			0x40, // 8,000 Hz (byte 0) 0x1f40
			0x1f, // 8,000 Hz (byte 1) 0x841e
			0x00, // 8,000 Hz (byte 2)

	/* Isochronous Endpoint Descriptor */  
			0x09, // bLenght
			0x05, // bDescriptionType (endpoint)
			0x84, // bEndpointAddress (EP4 in)
			0x01, // bmAttributes (asynchronous)
			0x00, // wMaxPacketSize (16)
			0x02,
			0x01, // bInterwal (1 ms)
			0x00, // bRefresh (0)
			0x00, // bSyncAddress (no synchronization)

	/* Isochronous Endpoint Audio Class Descriptor */
				0x07, // bLength (7)
				0x25, // bDescriptorType (CS_ENDPOINT)
				0x01, // bDescriptorSubtype (EP_GENERAL)
				0x00, // bmAttributes (none)
				0x00, // bLockDelayUnits (PCM samples)
				0x00, // wLockDelay (0)	
				0x00, 

	/** Interface number 2 **/			
	/* Audio Stream interface descriptor */
		0x09, // bLenght
		0x04, // bDescriptorType (interface)
		UDP_AS_OUT_INTERFACE, // bInterfaceNumber
		0x00, // bAlternateSettings
		0x00, // bNumEndpoints
		0x01, // bInterfaceClass (audio)
		0x02, // bInterfaceSubClass (audio_streaming)
		0x00, // bInterfaceProtocol (none)
		0x00, // iInterface (none)

		/* Alternate Audio Interface Descriptor */
		0x09, // bLenght
		0x04, // bDescriptorType (interface)
		UDP_AS_OUT_INTERFACE, // bInterfaceNumber
		0x01, // bAlternateSetting
		0x01, // bNumEndpoints
		0x01, // bInterfaceClass
		0x02, // bInterfaceSubClass
		0x00, // bInterfaceProtocol (none)
		0x00, // iInterface (none)

	/* Audio Stream Audio Class Descriptor */
			0x07, // bLenght
			0x24, // bDescriptorType (CS_INTERFACE)
			0x01, // bDescriptorSubtype (AS_GENERAL)
			UDP_AC_TERM_IN_USB_ID, // bTerminalLink (terminal 2)
			0x01, // bDelay (none)
			0x01, // wFormatTag (PCM format)
			0x00,

			/* Format Type Audio Descriptor */
			0x0B, // bLength (11)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x02, // bDescriptorSubtype (FORMAT_TYPE)
			0x01, // bFormatType (TYPE_I)
			0x01, // bNrChannels (1)
			0x02, // bSubFrameSize (2)
			// The next field should be 10, but 16 works with more standard software
			0x10, // bBitResolution (16)
			0x01, // bSamFreqType (1 sampling frequency)
			0x40, // 8,000 Hz (byte 0) 0x1f40
			0x1f, // 8,000 Hz (byte 1) 0x841e
			0x00, // 8,000 Hz (byte 2)

			/* Isochronous Endpoint Descriptor */
			0x09, // bLenght
			0x05, // bDescriptionType (endpoint)
			0x05, // bEndpointAddress (EP5 out)
			0x01, // bmAttributes (asynchronous)
			0x00, // wMaxPacketSize (16)
			0x02,
			0x01, // bInterwal (1 ms)
			0x00, // bRefresh (0)
			0x00, // bSyncAddress (no synchronization)

			/* Isochronous Endpoint Audio Class Descriptor */
				0x07, // bLength (7)
				0x25, // bDescriptorType (CS_ENDPOINT)
				0x01, // bDescriptorSubtype (EP_GENERAL)
				0x00, // bmAttributes (none)
				0x00, // bLockDelayUnits (PCM samples)
				0x00, // wLockDelay (0)
				0x00,

		/* Interface descriptor (HID) */
		0x09, // bLength
		0x04, // bDescriptorType (interface)
		UDP_HID_INTERFACE, // bInterfaceNumber
		0x00, // bAlternateSetting
		0x01, // bNumEndpoints
		0x03, // bInterfaceClass (HID)
		0x01, // bInterfaceSubClass
		0x01, // bInterfaceProtocol (none)
		0x00, // iInterface (none)

		/* HID interface descriptor */
			0x09, // bLength
			0x21, // bDescriptorType (33)
			0x10, // bcdHID
			0x01,
			0x00, // bCountryCode
			0x01, // bNumDescriptors
			0x22, // bDescriptorType
			UDP_DESCRIPTOR_HID_REPORT_SIZE, // bDescriptorLength (39)
			0x00,

	/* EndPoint Descriptor */
	0x07, // bLength (7)
	0x05, // bDescriptorType (CS_ENDPOINT)
	0x82, // bDescriptorSubtype (EP_GENERAL)
	0x03, // bmAttributes (none)
	0x08, // wMaxPacketSize
	0x00,
	0x1a, // bInterval

};

const uint8_t udp_kbd_report_descriptor[] = {
	/* Report descriptor */
	0x05, 0x01, // Usage Page (Generic Desktop)
	0x09, 0x06, // Usage (Keyboard)
	0xa1, 0x01, // Collection (application)
	
	0x05, 0x07, //     Usage Page (Key Codes)
	0x19, 0xe0, //     Usage Minimum (224)
	0x29, 0xe7, //     Usage Maximum (231)
	0x15, 0x00, //     Logical Minimum (0)
	0x25, 0x01, //     Logical Maximum (1)
	0x75, 0x01, //     Report Size (1)
	0x95, 0x08, //     Report Count (8)
	0x81, 0x02, //     Input (Data, Variable, Absolute)

	0x95, 0x01, //     Report Count (1)
	0x75, 0x08, //     Report Size (8)
	0x81, 0x01, //     Input (Constant) reserved byte(1)
	
	0x95, 0x05,                         //     Report Count (6)
	0x75, 0x08,                         //     Report Size (8)
	0x15, 0x00,                         //     Logical Minimum (0)
	0x25, 0x65,                         //     Logical Maximum (101)
	0x05, 0x07,                         //     Usage Page (Key codes)
	0x19, 0x00,                         //     Usage Minimum (0)
	0x29, 0x65,                         //     Usage Maximum (101)
	0x81, 0x00,                         //     Input (Data, Array) Key array(6 bytes)
		
////	0x05, 0x07, //	   Usage Page (key codes)
	//0x81, 0x02,	//	   Input (Data, Variable, Absolute)
	//0x81, 0x01,	//	   Input (Constant)
	//0x19, 0x00,	//     Usage Minimum (0)
	//0x29, 101,	//     Usage Maximum (101)
	//0x15, 0x00,	//     Logical Minimum (0)
	//0x25, 101,	//     Logical Maximum (101)
	//0x95, 0x05,	//     Report Count (6)
	//0x75, 0x08,	//     Report Size (8)
	//0x81, 0x00,	//     Input (Data, Array)
	
	0xc0,     // End collection	
	
	//0x05, 0x01, // Usage Page (Generic Desktop)
	//0x09, 0x02, // Usage (Keyboard)
	//0xa1, 0x01, // Collection (application)
	//0x09, 0x01, // uasge pointer
	//0xa1, 0x00,
	//0x05, 0x09, // Usage Page (key codes)
	//0x19, 0x01,	// Usage Minimum (224)
	//0x29, 0x05,	// Usage Maximum (231)
	//0x15, 0x00,	// Logical Minimum (0)
	//0x25, 0x01,	// Logical Maximum (1)
	//0x95, 0x05,	// Report Size (1)
	//0x75, 0x01,	// Report Count (8)
	//0x81, 0x02,	// Input (Data, Variable, Absolute)
	//0x95, 0x01,
	//0x75, 0x03,
	//0x81, 0x01,
	//0x05, 0x01,
	//0x09, 0x30,
	//0x09, 0x31,
	//0x09, 0x38,
	//0x15, 0x81,
	//0x25, 0x7f,
	//0x75, 0x08,
	//0x95, 0x03,
	//0x81, 0x06,
	//0xc0, 0xc0,
	
};

/* This is duplication from device configuration register. */
const uint8_t udp_hid_interface_descriptor[] = {
			/* Interface descriptor (HID) */
			0x09, // bLength
			0x21, // bDescriptorType (33)
			0x01, // bcdHID
			0x00,
			0x00, // bCountryCode
			0x01, // bNumDescriptors
			0x22, // bDescriptorType
			UDP_DESCRIPTOR_HID_REPORT_SIZE, // bDescriptorLength (39)
			0x00,
};

//#endif /* UDP-AUDIO_H_ */