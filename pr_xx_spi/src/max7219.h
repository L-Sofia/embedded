#ifndef MAX7219_H
#define MAX7219_H
#include <avr/io.h>
#include <util/delay.h>
#define REG_SHUTDOWN 0x0C
#define REG_INTENSITY 0x0A
#define REG_SCAN_LIM 0x0B
#define REG_DECODE 0x09
#define REG_DISPLAY 0x0F

void max7219_init(void);
void max7219_send(uint8_t addr, uint8_t data);
void max7219_heart(void);

#endif