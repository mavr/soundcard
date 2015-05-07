/*
 * udp_audio.h
 *
 * Created: 14.04.2015 15:55:28
 *  Author: ale
 */ 


#ifndef UDP_AUDIO_H_
#define UDP_AUDIO_H_



static uint8_t udp_dev_descriptor[] = {
	/* Device Descriptor */
	0x12, // bLength
	0x01, // bDescriptorType
	0x10, // bcdUSB
	0x00,
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
	0x64, // wTotalLength (83)
	0x00,
	0x02, // bNumInterface
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
			0x09, // bLength (9)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x01, // bDescriptorSubtype (HEADER)
			0x01, // bcdADC (1.0)
			0x00,
			0x15, // wTotalLength (36)
			0x00,
			0x01, // bInCollection (1 streaming interface)
			0x01, // baInterfaceNr (interface 1 is stream)
			
	/* Input terminal Audio Class Descriptor */
			0x0C, // bLength (12)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x02, // bDescriptorSubtype (INPUT_TERMINAL)
			0x01, // bTerminalID (1)
			0x10, // wTerminalType (radio receiver)
			0x07,
			0x00, // bAssocTerminal (none)
			0x01, // bNrChannels (2)
			0x00, // wChannelConfig (mono)
			0x00,
			0x00, // iChannelNames (none)
			0x00, // iTerminal (none)
			
	/* Feature Unit Descriptor */
			0x09, // bLength (13)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x06, // bDescriptorSubtype (FEATURE_UNIT)
			0x02, // bUnitID (2)
			0x01, // bSourceID (input terminal 1)
			0x02, // bControlSize (2 bytes)
			0xff, // Master controls
			0xff,
			
			0x00, // iFeature (none)
			//0x0e, // bLength (13)
			//0x24, // bDescriptorType (CS_INTERFACE)
			//0x06, // bDescriptorSubtype (FEATURE_UNIT)
			//0x41, // bUnitID (41)
			//0x01, // bSourceID (input terminal 1)
			//0x0F, 0x0f, 0x0f, 0x0f, /* bmaControls(0)(0x0000000F): Master Channel 0
										 //0b11: Mute read/write
										 //0b11: Volume read/write */
			//0x00, 0x00, 0x00, 0x00, /* bmaControls(1)(0x00000000): Logical Channel 1
			//*/
			//0x00, 
			
			
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
		//0x01, // bInterfaceNumber
		//0x00, // bAlternateSetting
		
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
			0x01, // bTerminalLink (terminal 1)
			0x00, // bDelay (none)
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
			0x40, // 8,000 Hz (byte 0)
			0x1f, // 8,000 Hz (byte 1)
			0x00, // 8,000 Hz (byte 2)
				
	/* Isochronous Endpoint Descriptor */
			0x09, // bLenght
			0x05, // bDescriptionType (endpoint)
			0x02, // bEndpointAddress (EP2 out)
			0x05, // bmAttributes (asynchronous)
			0x00, // wMaxPacketSize (512)
			0x02,
			0x01, // bInterwal (1 ms)
			0x00, // bRefresh (0)
			0x00, // bSyncAddress (no synchronization)			
		
	/* Isochronous Endpoint Audio Class Descriptor */
			0x07, // bLength (7)
			0x25, // bDescriptorType (CS_ENDPOINT)
			0x01, // bDescriptorSubtype (EP_GENERAL)
			0x00, // bmAttributes (none)
			0x02, // bLockDelayUnits (PCM samples)
			0x00, // wLockDelay (0)	
			0x00, 
		
};





#endif /* UDP-AUDIO_H_ */