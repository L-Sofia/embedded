#ifndef DS1820_H
#define DS1820_H

#include <stdint.h>

#define DS1820_CMD_CONVERT_T 0x44
#define DS1820_CMD_READ_SCRATCH 0xBE

int16_t ds1820_read_temp_x100(const uint8_t *rom);
void ds1820_print_temp(int16_t temp_x100);
void ds1820_lcd_print(int16_t temp_x100);

#endif
