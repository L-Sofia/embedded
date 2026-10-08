/*
    1. Реалізувати RS тригер, PB1 – S, PB0 – R, PD0 – Q і PD7 - !Q
*/
#include <avr/io.h>

void led_init(uint8_t pin)
{
    DDRD |= (1 << pin);
}

void btn_init(uint8_t pin)
{
    DDRB &= ~(1 << pin);
    PORTB |= (1 << pin);
}

uint8_t btn_res(void)
{
    static uint8_t val = 0;
    uint8_t R = !(PINB & (1 << PB0));
    uint8_t S = !(PINB & (1 << PB1));

    if (S && !R)
    {
        val = 1;
    }
    else if (!S && R)
    {
        val = 0;
    }
    return val;
}

void flip_rs(void)
{
    uint8_t Q = btn_res();

    if (Q == 1)
    {
        PORTD |= (1 << PD0);
        PORTD &= ~(1 << PD7);
    }
    else
    {
        PORTD &= ~(1 << PD0);
        PORTD |= (1 << PD7);
    }
}

int main(void)
{
    DDRD |= (1 << PD1);
    led_init(PD0); //  Q
    led_init(PD7); // !Q
    btn_init(PB0); // R
    btn_init(PB1); // S

    while (1)
    {
        flip_rs();
    }
    return 0;
}