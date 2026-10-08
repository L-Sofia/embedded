/*
    1. Реалізувати декодер 3 -> 8, де входи це кнопки 1, 2, 3, а виходи це діоди 0-7.
*/
#include <avr/io.h>
#include <util/delay.h>

volatile uint32_t ms = 0;

void tick(void)
{
    ms++;
    _delay_ms(1);
}

void led_init(void)
{
    DDRD = 0xFF;
}

void btn_init(uint8_t pin)
{
    DDRB &= ~(1 << pin);
    PORTB |= (1 << pin);
}

void btn_handler(uint8_t num)
{
    static uint8_t a = 0;
    static uint8_t b = 0;
    static uint8_t c = 0;
    switch (num)
    {
    case 0:
        a = !a;
        break;
    case 1:
        b = !b;
        break;
    case 2:
        c = !c;
        break;
    default:
        break;
    }
    uint8_t addr = (c << 2) | (b << 1) | a;
    PORTD = (1 << addr);
}

// uint8_t btn_in(void)
// {
//     uint8_t a = !(PINB & (1 << PB0));
//     uint8_t b = !(PINB & (1 << PB1));
//     uint8_t c = !(PINB & (1 << PB2));

//     return (c << 2) | (b << 1) | a;
// }

void btn_scan(uint8_t pin, uint8_t num)
{
    static uint8_t is_pressed[3] = {0, 0, 0};
    static uint32_t last[3] = {0, 0, 0};
    uint32_t curr = ms;
    if (!(PINB & (1 << pin)))
    {
        if (curr - last[num] > 50)
        {
            last[num] = curr;
            if (!is_pressed[num])
            {
                is_pressed[num] = 1;
                btn_handler(num);
            }
        }
    }
    else
    {
        if (is_pressed[num] && curr - last[num] > 50)
        {
            is_pressed[num] = 0;
        }
    }
}

int main(void)
{
    led_init();
    btn_init(PB0);
    btn_init(PB1);
    btn_init(PB2);
    while (1)
    {
        // uint8_t addr = btn_in();
        // PORTD = (1 << addr);
        //_delay_ms(20);
        tick();
        btn_scan(PB0, 0);
        btn_scan(PB1, 1);
        btn_scan(PB2, 2);
    }
    return 0;
}