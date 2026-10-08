#include "spi.h"

void spi_init(void)
{
    DDRB |= (1 << CLK) | (1 << CS) | (1 << MOSI);
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_send(uint8_t byte)
{
    // PORTB &= ~(1 << CS);
    SPDR = byte;
    while (!(SPSR & (1 << SPIF)))
        ;
    // PORTB |= (1 << CS);
}

uint8_t spi_recieve(void)
{
    spi_send(0);
    while (!(SPSR & (1 << SPIF)))
        ;
    return SPDR;
}