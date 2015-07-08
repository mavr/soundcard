/*
 * udp_audio.h
 *
 * Created: 14.04.2015 15:55:28
 *  Author: ale
 */ 


#ifndef UDP_AUDIO_H_
#define UDP_AUDIO_H_

typedef struct { // __attribute__((__packed))
	uint8_t bLenght;
	
} udp_configuration_desc_t;

typedef struct {
	
} udp_audio_conf_desc_t;

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

static uint8_t udp_dev_descriptor[] = {
	/* Device Descriptor */
	0x12, // bLength
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

static uint8_t udp_conf_descriptor[] = {
	/* Configuration Descriptor */
	0x09, // bLength
	0x02, // bDescriptorType
	0xc0, // wTotalLength (83)
	0x00,
	0x03, // bNumInterface
	0x01, // bConfigurationValue
	0x00, // iConfiguration
	0x80, // bmAttributes
	0xFA,  // bMaxPower
	
	/* Audio Control Interface Descriptor */
		0x09, // bLenght
		0x04, // bDescriptorType
		0x00, // bInterfaceNumber
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
			
	/** Microphone **/
	/* Input terminal Audio Class Descriptor */
			0x0C, // bLength (12)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x02, // bDescriptorSubtype (INPUT_TERMINAL)
			0x01, // bTerminalID (1)
			0x01, // wTerminalType (microphone)
			0x02,
			0x00, // bAssocTerminal (none)
			0x01, // bNrChannels (1)
			0x00, // wChannelConfig (mono)
			0x00,
			0x00, // iChannelNames (none)
			0x00, // iTerminal (none)
			
	/* Output terminal Audio Class Descriptor */
			0x09, // bLength (9)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x03, // bDescriptorSubtype (OUTPUT_TERMINAL)
			0x02, // bTerminalID (2)
			0x01, // wTerminalType (usb streaming)
			0x01,
			0x00, // bAssocTerminal (none)
			0x01, // bSourceID (1)
			0x00, // iTerminal (none)			

	/** Speaker **/
	/* Input terminal Audio Class Descriptor */
			0x0C, // bLength (12)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x02, // bDescriptorSubtype (INPUT_TERMINAL)
			0x03, // bTerminalID (3)
			0x01, // wTerminalType (usb streaming)
			0x01,
			0x00, // bAssocTerminal (none)
			0x01, // bNrChannels (1)
			0x00, // wChannelConfig (mono)
			0x00,
			0x00, // iChannelNames (none)
			0x00, // iTerminal (none)

	/* Output terminal Audio Class Descriptor */
			0x09, // bLength (9)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x03, // bDescriptorSubtype (OUTPUT_TERMINAL)
			0x04, // bTerminalID (4)
			0x01, // wTerminalType (speaker)
			0x03,
			0x00, // bAssocTerminal (none)
			0x03, // bSourceID (3)
			0x00, // iTerminal (none)
			
	/* Feature unit Audio Class Descriptor */
			0x09, // bLength (9)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x06, // bDescriptorSubtype (FEATURE_UNIT)
			0x05, // bUnitID (5)
			0x01, // bSourceID (1)
			0x01, // bControlSize (1)
			0x03, // bmaContorls(0) : Mute, Volume
			0x00, // bmaContorls(1)
			0x00, // iTerminal (none)		

	/* Feature unit Audio Class Descriptor */
			0x09, // bLength (9)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x06, // bDescriptorSubtype (FEATURE_UNIT)
			0x06, // bUnitID (6)
			0x03, // bSourceID (3)
			0x01, // bControlSize (1)
			0x03, // bmaContorls(0) : Mute, Volume
			0x00, // bmaContorls(1)
			0x00, // iTerminal (none)
			
	/** Interface number 1 **/
	/* Audio Stream interface descriptor */
		0x09, // bLenght
		0x04, // bDescriptorType (interface)
		0x01, // bInterfaceNumber
		0x00, // bAlternateSettings
		0x00, // bNumEndpoints
		0x01, // bInterfaceClass (audio)
		0x02, // bInterfaceSubClass (audio_streaming)
		0x00, // bInterfaceProtocol (none)
		0x00, // iInterface (none)
		
	/* Alternate Audio Interface Descriptor */
		0x09, // bLenght
		0x04, // bDescriptorType (interface)
		0x01, // bInterfaceNumber
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
			0x02, // bTerminalLink (terminal 1)
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
			0x10, // wMaxPacketSize (16) 
			0x00,
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
		0x02, // bInterfaceNumber
		0x00, // bAlternateSettings
		0x00, // bNumEndpoints
		0x01, // bInterfaceClass (audio)
		0x02, // bInterfaceSubClass (audio_streaming)
		0x00, // bInterfaceProtocol (none)
		0x00, // iInterface (none)
	
		/* Alternate Audio Interface Descriptor */
		0x09, // bLenght
		0x04, // bDescriptorType (interface)
		0x02, // bInterfaceNumber
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
			0x03, // bTerminalLink (terminal 1)
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
			0x10, // wMaxPacketSize (16)
			0x00,
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
};





#endif /* UDP-AUDIO_H_ */