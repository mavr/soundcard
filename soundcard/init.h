/*
 * init.h
 *
 * Created: 06.02.2015 14:45:31
 *  Author: ale
 */ 


#ifndef INIT_H_
#define INIT_H_

#define __DEBUG_SYSTEM_PREFIX "system"

/* Arise, and go! */
void Init(void);

/* This function initialize power manager controller */
void pmc_system(void);

/* Here is configured input and output lines. */
void pio_system(void);


#endif /* INIT_H_ */