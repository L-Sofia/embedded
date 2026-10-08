#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "one_wire.h"
#include "uart.h"
#include "lcd.h"
#include "i2c.h"
#include "ds18b20.h"
#include "ds1820.h"

int main(void)
{
    USART_Init(9600);
    ow_init();
    I2C_Init();
    LCD_Init(0x4E);

    uint8_t rom[8];
    uint8_t scratchpad[9];

    // uint8_t rom[8];
    int16_t temp_raw;

    printf("ONE WIRE LAB.");
    printf("Starting search ROM...\r\n");
    ow_reset_search();

    while (ow_search(rom))
    {
        printf("Found ROM: ");
        for (uint8_t i = 0; i < 8; ++i)
        {
            printf("%02X", rom[i]);
            if (i < 7)
                printf("-");
        }

        printf("\r\n");

        _delay_ms(500);
    }
    LCD_Clear();
    ds18b20_lcd_print_rom(rom);
    printf("Search finished\r\n");

    int16_t temp;

    for (;;)
    {
        // temp_raw = ds18b20_read_temp(rom);
        // ds18b20_print_temp(temp_raw);
        // ds18b20_lcd_print(temp_raw);
        // _delay_ms(2000);

        temp = ds1820_read_temp_x100(rom);
        ds1820_print_temp(temp);
        ds1820_lcd_print(temp);
    }
    return 0;
}