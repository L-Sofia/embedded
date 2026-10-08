/*
    1. BCD лічильник, кнопка 1 - старт і стоп, кнопка 2 - занулення
*/
#include <avr/io.h>
#include <util/delay.h>

volatile uint32_t ms = 0;

typedef enum
{
    STOP,
    RUNNING,
    CLEAR
} bcd_states;

static bcd_states state = STOP;

void led_init(void)
{
    DDRD = 0xff;
}

void btn_init(uint8_t pin)
{
    DDRB &= ~(1 << pin);
    PORTB |= (1 << pin);
}

uint8_t upd(uint32_t *pLast, uint16_t period)
{
    if (ms - *pLast >= period)
    {
        *pLast = ms;
        return 1;
    }
    return 0;
}

void tick(void)
{
    ms++;
    _delay_ms(1);
}

void bcd_counter(void)
{
    static uint8_t i = 0;
    static uint32_t last_bcd = 0;
    uint16_t period = 1000;

    switch (state)
    {
    case CLEAR:
        PORTD = 0x00;
        i = 0;
        state = STOP;
        break;
    case RUNNING:
        if (upd(&last_bcd, period))
        {
            uint8_t uts = i % 10;
            uint8_t tens = i / 10;
            PORTD = (tens << 4) | uts;
            i++;
        }
    case STOP:
    default:
        break;
    }
}

void bcd_handler(uint8_t num)
{
    switch (num)
    {
    case 0:
        if (state == STOP)
        {
            state = RUNNING;
        }
        else if (state == RUNNING)
        {
            state = STOP;
        }
        break;
    case 1:
        state = CLEAR;
        break;
    default:
        break;
    }
}

void btn_read(uint8_t pin, uint8_t num)
{
    static uint8_t is_pressed[2] = {0, 0};
    static uint32_t last = 0;
    uint32_t curr = ms;

    if (!(PINB & (1 << pin)))
    {
        if (curr - last > 50)
        {
            last = curr;
            if (!is_pressed[num])
            {
                is_pressed[num] = !is_pressed[num];
            }
        }
    }
    else
    {
        if (is_pressed[num] && curr - last > 50)
        {
            is_pressed[num] = !is_pressed[num];
            bcd_handler(num);
        }
    }
}

int main(void)
{
    led_init();
    btn_init(PB0);
    btn_init(PB1);
    while (1)
    {
        tick();
        btn_read(PB0, 0);
        btn_read(PB1, 1);
        bcd_counter();
    }
    return 0;
}