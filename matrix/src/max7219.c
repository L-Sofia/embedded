#include "max7219.h"

void max7219_send(uint8_t address, uint8_t data)
{
    PORTB &= ~(1 << CS);
    spi_transmit(address);
    spi_transmit(data);
    PORTB |= (1 << CS);
}

void max7219_clear(void)
{
    for (uint8_t i = 1; i <= 8; i++)
    {
        max7219_send(i, 0x00);
    }
}

void max7219_init(void)
{
    max7219_send(0x0F, 0x00); // Display test: off
    max7219_send(0x0C, 0x01); // Shutdown: normal operation
    max7219_send(0x0B, 0x07); // Scan limit: display all 8 digits
    max7219_send(0x09, 0x00); // Decode mode: none (use raw data)
    max7219_send(0x0A, 0x01); // Brightness
    max7219_clear();
}

void max7219_display(uint8_t *pattern, uint8_t row_count, uint8_t col_count)
{
    if (row_count > 8)
        row_count = 8;
    if (col_count > 8)
        col_count = 8;

    for (uint8_t col = 0; col < col_count; col++)
    {
        uint8_t column_data = 0;
        for (uint8_t row = 0; row < row_count; row++)
        {
            if (pattern[row] & (1 << (col_count - 1 - col)))
                column_data |= (1 << (7 - row));
        }
        max7219_send(col + 1, column_data);
    }

    for (uint8_t col = col_count; col < 8; col++)
    {
        max7219_send(col + 1, 0x00);
    }
}