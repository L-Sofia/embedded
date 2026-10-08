#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <stdint.h>

#define LATCH PB2
#define MOSI PB3
#define CLK PB5

void spi_init(void);
void spi_send(uint8_t data);
uint8_t spi_read(uint8_t data);

#endif
