#include "spi.h"

void spi_init(void)
{
    DDRB |= (1 << MOSI) | (1 << CLK) | (1 << CS);
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_transmit(uint8_t data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF)))
        ;
}

uint8_t spi_receive(void)
{
    while (!(SPSR & (1 << SPIF)))
        ;
    return SPDR;
}