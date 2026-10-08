#include "ds1820.h"
#include "one_wire.h"
#include "lcd.h"
#include <util/delay.h>
#include <stdio.h>

int16_t ds1820_read_temp_x100(const uint8_t *rom)
{
    uint8_t scratchpad[9];
    int16_t raw;
    int16_t temp_x100;
    uint8_t count_remain;
    uint8_t count_per_c;

    if (!ow_reset_pulse())
        return 0;

    ow_write_byte(0x55); // MATCH ROM
    for (uint8_t i = 0; i < 8; i++)
        ow_write_byte(rom[i]);

    ow_write_byte(DS1820_CMD_CONVERT_T);
    _delay_ms(750);

    if (!ow_reset_pulse())
        return 0;

    ow_write_byte(0x55); // MATCH ROM
    for (uint8_t i = 0; i < 8; i++)
        ow_write_byte(rom[i]);

    ow_write_byte(DS1820_CMD_READ_SCRATCH);
    for (uint8_t i = 0; i < 9; i++)
        scratchpad[i] = ow_read_byte();

    raw = (int16_t)((scratchpad[1] << 8) | scratchpad[0]);
    count_remain = scratchpad[6];
    count_per_c = scratchpad[7];

    /* Температура ×100 (без float) */
    temp_x100 = (raw >> 1) * 100;
    temp_x100 -= 25;
    temp_x100 += (int16_t)((count_per_c - count_remain) * 100 / count_per_c);

    return temp_x100;
}
void ds1820_lcd_print(int16_t temp_x100)
{
    int16_t temp_int = temp_x100 / 100;
    uint16_t frac = (temp_x100 >= 0) ? (temp_x100 % 100) : (-temp_x100 % 100);

    char buf[17];
    sprintf(buf, "%d.%02u C   ", temp_int, frac);

    LCD_Print(0, 1, "                ");
    LCD_Print(0, 1, buf);
}
void ds1820_print_temp(int16_t temp_x100)
{
    int16_t temp_int = temp_x100 / 100;
    uint16_t frac = (temp_x100 >= 0) ? (temp_x100 % 100) : (-temp_x100 % 100);

    printf("Temperature: %d.%02u C\r\n", temp_int, frac);
}
