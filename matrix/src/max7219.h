#ifndef MAX7219_H
#define MAX7219_H

#include <avr/io.h>
#include <stdint.h>
#include "spi.h"

void max7219_init(void);
void max7219_send(uint8_t address, uint8_t data);
void max7219_clear(void);
void max7219_display(uint8_t *pattern, uint8_t row_count, uint8_t col_count);

#endif
