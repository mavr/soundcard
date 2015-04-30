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
	0x10,
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
	0x3a, // wTotalLength
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
		0x00, // bNumEndpoints (none)
		0x01, // bInterfaceClass (AUDIO)
		0x01, // bInterfaceSubClass (AUDIO_CONTROL)
		0x00, // bInterfaceProtocol (none)
		0x00, // iInterface - address of string descriptor for describe this interface (none)
		
	/* Interface Header Audio Class Descriptor */
			0x09, // bLenght
			0x24, // bDescriptorType (CS_INTERFACE)
			0x01, // bDescriptorSubType (HEADER)
			0x00, // bcdADC (1.0)
			0x01, 
			0x00, // wTotalLenght
			0x00,
			0x01, // bInCollection (1 streaming interface)
			0x01, // baInterfacer (interface in 1 stream)
			
// ---------------			
	/* Input terminal Audio Class Descriptor */
			0x0C, // bLength (12)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x02, // bDescriptorSubtype (INPUT_TERMINAL)
			0x01, // bTerminalID (1)
			0x10, // wTerminalType (radio receiver)
			0x07,
			0x00, // bAssocTerminal (none)
			0x02, // bNrChannels (2)
			0x03, // wChannelConfig (left, right)
			0x00,
			0x00, // iChannelNames (none)
			0x00, // iTerminal (none)
			
	/* Output terminal Audio Class Descriptor */
			0x0A, // bLength (9)
			0x24, // bDescriptorType (CS_INTERFACE)
			0x03, // bDescriptorSubtype (OUTPUT_TERMINAL)
			0x03, // bTerminalID (3)
			0x01, // wTerminalType (USB streaming)
			0x01,
			0x00, // bAssocTerminal (none)
			0x02, // bSourceID (feature unit 2)
			0x00, // iTerminal (none)
			0x03, // bTerminalID (3)
			
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
		0x01, // bInterfaceNumber
		0x00, // bAlternateSetting
		
			
		
			
		
};





#endif /* UDP-AUDIO_H_ */