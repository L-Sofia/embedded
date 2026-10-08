/*
    1. Лічильник на діодах, кнопка 1 - додає одиницю, 2 - віднімає, 3 - очищує.
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
    static uint8_t i = 0;
    switch (num)
    {
    case 0:
        i++;
        break;
    case 1:
        i--;
        break;
    case 2:
        i = 0;
        break;
    default:
        break;
    }
    PORTD = i;
}

void btn_scan(uint8_t pin, uint8_t num)
{
    static uint8_t is_pressed[3] = {0, 0, 0};
    static uint32_t last = 0;
    uint32_t curr = ms;

    if (!(PINB & (1 << pin)))
    {
        if (curr - last > 50)
        {
            last = curr;
            if (!is_pressed[num])
            {
                is_pressed[num] = 1;
                btn_handler(num);
            }
        }
    }
    else
    {
        if (is_pressed[num] && curr - last > 50)
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
        tick();
        btn_scan(PB0, 0);
        btn_scan(PB1, 1);
        btn_scan(PB2, 2);
    }
    return 0;
}