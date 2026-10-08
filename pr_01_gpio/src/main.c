/*
    1. Блимати світлодіодом без блокування програми.
    2. Реалізувати біжучий вогонь неблокуючою функцією.
    3. Реалізувати симуляцію таймера окремою функцією.
    4. Зробити бінарний лічильник.
    5. Реалізувати bcd лічильник.
*/
#include <avr/io.h>
#include <util/delay.h>

static uint32_t ms = 0;

uint8_t upd(uint32_t *pLast, uint16_t period);

void led_init(void);
void blink(uint8_t pin);
void running_fire(void);
void bin_count(void);
void bcd_count(void);
void tick(void);

int main(void)
{
    led_init();
    while (1)
    {
        tick();
        // blink(PD4);
        //  running_fire();
        //  bin_count();
        bcd_count();
    }
    return 0;
}

void tick(void)
{
    ms++;
    _delay_ms(1);
}

void led_init(void)
{
    DDRD = 0xff;
    PORTD = 0xff;
    _delay_ms(500);
    PORTD = 0x00;
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

void blink(uint8_t pin)
{
    static uint32_t last_bl = 0;
    static uint8_t state = 0;
    uint16_t period_bl = 0;

    period_bl = (state == 0) ? 1000 : 2000;

    if (upd(&last_bl, period_bl))
    {
        PORTD ^= (1 << pin);
        state = !state;
    }
}

void running_fire(void)
{
    static uint32_t last_rn = 0;
    static uint8_t dt = 1;
    uint16_t period_rn = 500;
    static uint8_t i = 0;
    if (upd(&last_rn, period_rn))
    {
        PORTD = dt << i;
        i++;
        if (i > 7)
            i = 0;
    }
}

void bin_count(void)
{
    static uint32_t last_bin = 0;
    uint16_t period_bin = 500;
    static uint16_t i = 0;
    if (upd(&last_bin, period_bin))
    {
        PORTD = i;
        i++;
        if (i > 255)
            PORTD = 0x00;
    }
}

void bcd_count(void)
{
    static uint8_t i = 0;
    static uint32_t last_bcd = 0;
    uint16_t period_bcd = 1000;

    if (upd(&last_bcd, period_bcd))
    {
        uint8_t uts = i % 10;
        uint8_t tens = i / 10;
        PORTD = uts | (tens << 4);
        i++;
        if (i > 99)
            i = 0;
    }
}