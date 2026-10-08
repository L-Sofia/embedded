/*
    1. Зробити читання стану кнопки з внутрішнім підтягуючим резистором,
    натиск - діод світиться, відпускання - гасне.
    2. Зробити зміну стану кнопки і діода. Одне відпускання засвічує діод,
    наступне його загасає.
    3. Повторити 2, одне натискання засвічує діод, наступне - загасає.
    4. Врахувати брязкіт контактів кнопки.
    5. BCD лічильник. Перший натиск стартує, другий зупиняє, третій очищає.
*/
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#define START 0
#define RUNNING 1
#define STOPPED 2
#define CLEARED 3

volatile bool led_on = false;
volatile uint8_t bcd_state = START;
volatile uint32_t ms = 0;

void tick(void);
uint8_t upd(uint32_t *pLast, uint16_t period);
void leds_init(void);
void btn_init(uint8_t pin);
void btn_read(void);
void led_handler(void);
void bcd_handler(void);
void bcd_counter(void);

int main(void)
{
    leds_init();
    btn_init(PB1);
    while (1)
    {
        tick();
        btn_read();
        bcd_counter();
    }
    return 0;
}

void tick(void)
{
    _delay_ms(1);
    ms++;
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

void leds_init(void)
{
    DDRD = 0xff;
    PORTD = 0x00;
}

void btn_init(uint8_t pin)
{
    DDRB &= ~(1 << pin);
    PORTB |= (1 << pin);
}

void btn_read(void)
{
    static bool btn_pressed = false;
    static uint32_t last = 0;
    uint32_t curr = ms;
    if (!(PINB & (1 << PB1)))
    {
        if (curr - last > 50)
        {
            last = curr;
            if (!btn_pressed)
            {
                btn_pressed = true;
            }
        }
    }
    else
    {
        if (btn_pressed && curr - last > 50)
        {
            btn_pressed = false;
            bcd_handler();
        }
    }
}

void led_handler(void)
{
    led_on = !led_on;
    if (led_on)
    {
        PORTD |= (1 << PD7);
    }
    else
        PORTD &= ~(1 << PD7);
}

void bcd_handler(void)
{
    if (bcd_state == START)
        bcd_state = RUNNING;
    else if (bcd_state == RUNNING)
        bcd_state = STOPPED;
    else if (bcd_state == STOPPED)
        bcd_state = CLEARED;
    else if (bcd_state == CLEARED)
        bcd_state = RUNNING;
}

void bcd_counter(void)
{
    uint16_t period = 1000;
    static uint32_t last_bcd = 0;
    static uint8_t i = 0;

    if (bcd_state == CLEARED)
    {
        i = 0;
        PORTD = 0x00;
        return;
    }

    if (bcd_state != RUNNING)
    {
        return;
    }

    if (upd(&last_bcd, period))
    {
        uint8_t tens = i / 10;
        uint8_t uts = i % 10;
        PORTD = uts | (tens << 4);
        i++;
    }
}