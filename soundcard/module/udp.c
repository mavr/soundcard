/*
 * udp.c
 *
 * Created: 11.03.2015 14:29:24
 *  Author: ale
 */ 

#include "sam.h"
#include "udp.h"

void udp_system() {
	PMC->PMC_PCER1 |= 1 << 2; // 34 - udb
	PMC->PMC_SCER |= PMC_SCER_UDP;
	NVIC_EnableIRQ(UDP_IRQn);
}

void udp_set_interrupt() {
	UDP->UDP_IER = UDP_IER_SOFINT| UDP_ISR_RXSUSP;
}

void udp_ddp_pull_up() {
	UDP->UDP_TXVC =	UDP_TXVC_PUON;
}