/*
 * spi.c
 *
 * Created: 07.09.2015 13:34:36
 *  Author: ale
 */ 

#include "sam.h"

void spi_system() {
	PMC->PMC_PCER0 |= (1UL << ID_SPI);
	
	SPI->SPI_WPMR = SPI_WPMR_WPKEY_PASSWD;

	// Disable to configure
	SPI->SPI_CR = SPI_CR_SPIDIS;
	// Reset
	SPI->SPI_CR |= SPI_CR_SWRST;
	// Mode Register Master
	SPI->SPI_MR |= SPI_MR_MSTR | SPI_MR_DLYBCS(11);
	// SPCK baudrate = MCK / SCBR = 84 MHz / 40 (0x28) = 2.1Mhz
    SPI->SPI_CSR[0] |= 0x00002800 | SPI_CSR_NCPHA;

    // Enable SPI in SPI controller
    SPI->SPI_CR &= ~SPI_CR_SPIDIS;
	SPI->SPI_CR |= SPI_CR_SPIEN;
	
}

void spi_pio() {
	/* Set PA11 in peripheral mode A - NPCS0 */
	PIOA->PIO_ABCDSR[0] &= ~(PIO_ABCDSR_P11) ;
	PIOA->PIO_ABCDSR[1] &= ~(PIO_ABCDSR_P11) ;
	PIOA->PIO_PER |= PIO_PER_P11;
	
	// PA12 didn't use.
		
	/* Set PA13 in peripheral mode A - MOSI */
	PIOA->PIO_ABCDSR[0] &= ~(PIO_ABCDSR_P13) ;
	PIOA->PIO_ABCDSR[1] &= ~(PIO_ABCDSR_P13) ;
	PIOA->PIO_PER |= PIO_PER_P13;
	
	/* Set PA14 in peripheral mode A - SPCK */
	PIOA->PIO_ABCDSR[0] &= ~(PIO_ABCDSR_P14) ;
	PIOA->PIO_ABCDSR[1] &= ~(PIO_ABCDSR_P14) ;
	PIOA->PIO_PER |= PIO_PER_P14;

}

void spi_send(uint16_t value) {
//	while(!(SPI->SPI_SR & SPI_SR_TDRE));
//	SPI->SPI_TDR = SPI_TDR_TD(value);
	
	while ((SPI->SPI_SR & SPI_SR_TXEMPTY) == 0);
	SPI->SPI_TDR = value | SPI_TDR_PCS(0);
	while ((SPI->SPI_SR & SPI_SR_TDRE) == 0);
}