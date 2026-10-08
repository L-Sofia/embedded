#ifndef SPI_H
#define SPI_H
#include <avr/io.h>

#define CLK PB5
#define CS PB2
#define MOSI PB3
#define MISO PB4

void spi_init(void);
void spi_send(uint8_t byte);
uint8_t spi_recieve(void);

#endif