#include "1wire.h"
#include "ds18b20.h"
#include <util/delay.h>
#include <stdio.h>

void ds18b20PrintTemperature(void)
{
    if (!OW_Reset())
        return;

    OW_RW_byte(0xCC); // Skip ROM
    OW_RW_byte(0x44); // Start conversion
    _delay_ms(750);

    if (!OW_Reset())
        return;

    OW_RW_byte(0xCC);
    OW_RW_byte(0xBE);

    uint8_t lsb = OW_RW_byte(0xFF);
    uint8_t msb = OW_RW_byte(0xFF);

    printf("lsb=%d msb=%d\n", lsb, msb);

    int16_t raw = (msb << 8) | lsb;

    printf("raw=%d\n", raw);

    // температура * 100
    int16_t temp100 = (raw * 100) / 16;

    int16_t a = temp100 / 100; // ціла частина
    int16_t b = temp100 % 100; // дробова

    printf("T = %d.%02d C\n", a, b);
}
