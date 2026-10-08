#include "spi.h"

void spi_init(void)
{
    DDRB |= (1 << MOSI) | (1 << LATCH) | (1 << CLK);
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_send(uint8_t data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF)))
        ;
}

uint8_t spi_read(uint8_t data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF)))
        ;
    return SPDR;
}
