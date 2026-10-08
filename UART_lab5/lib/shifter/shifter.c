#include <avr/io.h>
#include <util/delay.h>
#include "shifter.h"

#define DATA PB3  // D11
#define CLK PB5   // D13
#define LATCH PB2 // D10

void shifter_init(void)
{
    DDRB |= (1 << DATA) | (1 << CLK) | (1 << LATCH);
}

void shift_out(uint8_t data)
{
    for (int i = 0; i < 8; i++)
    {
        if (data & 0x80)
            PORTB |= (1 << DATA);
        else
            PORTB &= ~(1 << DATA);

        PORTB |= (1 << CLK);
        _delay_us(1);
        PORTB &= ~(1 << CLK);

        data <<= 1;
    }

    PORTB |= (1 << LATCH);
    _delay_us(1);
    PORTB &= ~(1 << LATCH);
}
