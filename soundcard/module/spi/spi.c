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

	// Reset
	SPI->SPI_CR |= SPI_CR_SWRST;
	// Disable to configure
	SPI->SPI_CR = SPI_CR_SPIDIS;
	// Mode Register Master
	SPI->SPI_MR |= SPI_MR_MSTR;
	SPI->SPI_MR |= SPI_MR_PCS(0);
	// SPCK baudrate = MCK / SCBR = 84 MHz / 40 (0x28) = 2.1Mhz
    SPI->SPI_CSR[0] |= SPI_CSR_NCPHA  | SPI_CSR_BITS_16_BIT | SPI_CSR_SCBR(250);

    // Enable SPI in SPI controller
    SPI->SPI_CR &= ~SPI_CR_SPIDIS;
	SPI->SPI_CR |= SPI_CR_SPIEN;
	
}

void spi_pio() {
	/* Set PA11 in peripheral mode A - NPCS0 */
	PIOA->PIO_ABCDSR[0] &= ~(PIO_ABCDSR_P11) ;
	PIOA->PIO_ABCDSR[1] &= ~(PIO_ABCDSR_P11) ;
	PIOA->PIO_PDR |= PIO_PDR_P11;
	
	// PA12 didn't use.
	PIOA->PIO_ABCDSR[0] &= ~(PIO_ABCDSR_P12) ;
	PIOA->PIO_ABCDSR[1] &= ~(PIO_ABCDSR_P12) ;
	PIOA->PIO_PER |= PIO_PDR_P12;
	//TODO: this pin doesnt use. Need to be pull down.
		
	/* Set PA13 in peripheral mode A - MOSI */
	PIOA->PIO_ABCDSR[0] &= ~(PIO_ABCDSR_P13) ;
	PIOA->PIO_ABCDSR[1] &= ~(PIO_ABCDSR_P13) ;
	PIOA->PIO_PDR |= PIO_PDR_P13;
	
	/* Set PA14 in peripheral mode A - SPCK */
	PIOA->PIO_ABCDSR[0] &= ~(PIO_ABCDSR_P14) ;
	PIOA->PIO_ABCDSR[1] &= ~(PIO_ABCDSR_P14) ;
	PIOA->PIO_PDR |= PIO_PDR_P14;

}

void spi_send(uint16_t value) {
//	while(!(SPI->SPI_SR & SPI_SR_TDRE));
//	SPI->SPI_TDR = SPI_TDR_TD(value);
	
	while ((SPI->SPI_SR & SPI_SR_TXEMPTY) == 0);
	SPI->SPI_TDR = value | SPI_TDR_PCS(0);
	while ((SPI->SPI_SR & SPI_SR_TDRE) == 0);
}