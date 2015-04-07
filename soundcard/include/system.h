/*
 * system.h
 *
 * Created: 10.03.2015 16:19:58
 *  Author: ale
 */ 


#ifndef SYSTEM_H_
#define SYSTEM_H_

#define nop() __asm __volatile ("nop"); 

uint32_t dbg_tx_counter;


#endif /* SYSTEM_H_ */