#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <stdint.h>

#define MOSI PB3
#define MISO PB4
#define CLK PB5
#define CS PB2

void spi_init(void);
void spi_transmit(uint8_t data);
uint8_t spi_receive(void);

#endif
