/*
 * pio.h
 *
 * Created: 06.02.2015 14:58:31
 *  Author: ale
 */ 


#ifndef PIO_H_
#define PIO_H_

#include "core/syslog.h"

#define __DEBUG_PIO_PREFIX	"kbd"

#define __KBD_DEBUG(msg) {\
		__DEBUG(LOG_LVL_HIGH, __DEBUG_PIO_PREFIX, msg);\
	}

/* Key and pin matching */
#define KBD_BUTTON_MEDIC		(21)
#define KBD_BUTTON_TRANS		(22)
#define KBD_BUTTON_FUNC			(26)
#define KBD_BUTTON_PTT			(25)
#define KBD_BUTTON_ABN			(23)

/* Key code for report register of hiddev. */
#define KBD_BUTTON_MEDIC_CODE	(30)
#define KBD_BUTTON_TRANS_CODE	(31)
#define KBD_BUTTON_FUNC_CODE	(32)
#define KBD_BUTTON_PTT_CODE		(33)
#define KBD_BUTTON_ABN_CODE		(34)

/* Struct for each button */
typedef struct {
	uint8_t pin;
	uint8_t code;
	char label[10];
} kbd_key_t;

/* Keyboard struct */
typedef struct {
	kbd_key_t key[5];
} kbd_t;

void pio_system();
void pio_enable_kdb();




#endif /* PIO_H_ */