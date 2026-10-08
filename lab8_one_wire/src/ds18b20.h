#ifndef DS18B20_H
#define DS18B20_H

#include <stdint.h>
#include "lcd.h"

#define DS18B20_CMD_CONVERT_T 0x44
#define DS18B20_CMD_READ_SCRATCH 0xBE
#define ONEWIRE_MATCH_ROM 0x55

int16_t ds18b20_read_temp(const uint8_t *rom);
void ds18b20_print_temp(int16_t raw);
void ds18b20_lcd_print(int16_t raw);
void ds18b20_lcd_print_rom(uint8_t *rom);

#endif
