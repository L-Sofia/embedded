#include "ds18b20.h"
#include "one_wire.h"
#include "uart.h"
#include <util/delay.h>
#include <stdio.h>

int16_t ds18b20_read_temp(const uint8_t *rom)
{
    uint8_t scratchpad[9];
    int16_t raw;

    if (!ow_reset_pulse())
        return 0;

    ow_write_byte(ONEWIRE_MATCH_ROM);
    for (uint8_t i = 0; i < 8; i++)
        ow_write_byte(rom[i]);

    ow_write_byte(DS18B20_CMD_CONVERT_T);

    _delay_ms(750);

    if (!ow_reset_pulse())
        return 0;

    ow_write_byte(ONEWIRE_MATCH_ROM);
    for (uint8_t i = 0; i < 8; i++)
        ow_write_byte(rom[i]);

    ow_write_byte(DS18B20_CMD_READ_SCRATCH);
    for (uint8_t i = 0; i < 9; i++)
        scratchpad[i] = ow_read_byte();

    raw = (int16_t)((scratchpad[1] << 8) | scratchpad[0]);

    return raw;
}

void ds18b20_print_temp(int16_t raw)
{
    int16_t temp_int = raw >> 4;
    uint16_t frac = (raw & 0x0F) * 625;

    printf("Temperature: %d.%04u C\r\n", temp_int, frac);
}

void ds18b20_lcd_print(int16_t raw)
{
    int16_t temp_int = raw >> 4;
    uint16_t frac = (raw & 0x0F) * 625;

    char buf[17];
    sprintf(buf, "%d.%02u C   ", temp_int, frac / 100);

    LCD_Print(0, 1, "                ");
    LCD_Print(0, 1, buf);
}

void ds18b20_lcd_print_rom(uint8_t *rom)
{
    char buf[17];

    sprintf(buf, "%02X-%02X-%02X-%02X",
            rom[0], rom[1], rom[2], rom[3]);
    LCD_Print(0, 0, buf);

    sprintf(buf, "%02X-%02X-%02X-%02X",
            rom[4], rom[5], rom[6], rom[7]);
    LCD_Print(0, 1, buf);
}