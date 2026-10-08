#include "max7219.h"
#include "spi.h"

void max7219_init(void)
{
    max7219_send(REG_DISPLAY, 0x00);
    max7219_send(REG_DECODE, 0x00);
    max7219_send(REG_SCAN_LIM, 0x07);
    max7219_send(REG_INTENSITY, 0x01);
    max7219_send(REG_SHUTDOWN, 0x01);

    for (uint8_t i = 0; i < 8; i++)
    {
        max7219_send(i + 1, 0);
    }
}
void max7219_send(uint8_t addr, uint8_t data)
{
    PORTB &= ~(1 << CS);
    spi_send(addr);
    while (!(SPSR & (1 << SPIF)))
        ;
    spi_send(data);
    while (!(SPSR & (1 << SPIF)))
        ;
    PORTB |= (1 << CS);
}
uint8_t patt[8] = {
    0b00011000,
    0b00111100,
    0b01111110,
    0b11111111,
    0b11111111,
    0b01111110,
    0b00111100,
    0b00011000};

void max7219_heart(void)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        max7219_send(8 - i, 0b00000001 << i);
        _delay_ms(350);
    }
}