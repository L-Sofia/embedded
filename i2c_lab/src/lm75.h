#ifndef LM75_H_
#define LM75_H_

#include <avr/io.h>
#include <stdint.h>

#define LM75_ADDR (0x90)

uint8_t lm75_init(void);

// int16_t lm75_get_temp_x10(void);
void lm75_get_temp(int16_t *a, int16_t *b);

#endif