#include <avr/io.h>
#include "button.h"

static button_handler_t m_bhnd;

void bhnd_hook(button_event_t e) { (void)e; }

void button_init(button_handler_t bhnd)
{
    DDRB &= ~(1 << BTN);
    PORTB |= (1 << BTN);

    if (bhnd != ((void *)0))
    {
        m_bhnd = bhnd;
    }
    else
    {
        m_bhnd = bhnd_hook;
    }
}

void button_scan_task(void)
{
    static uint8_t button_prev = 1;
    static uint16_t hold_time = 0;
    static uint8_t long_triggered = 0;
    uint8_t button_now = PINB & (1 << BTN);
    button_event_t event = BUTTON_NONE;

    if (event != BUTTON_NONE)
    {
        m_bhnd(event);
    }

    if (button_prev && !button_now)
    {
        hold_time = 0;
        long_triggered = 0;
    }

    if (!button_now)
    {
        hold_time += SCAN_PERIOD_MS;
        if (hold_time >= HOLD_TIME_MS && !long_triggered)
        {
            event = BUTTON_LONG;
            long_triggered = 1;
            if (event != BUTTON_NONE)
            {
                m_bhnd(event);
            }
        }
    }
    if (!button_prev && button_now)
    {
        event = BUTTON_SHORT;
        if (event != BUTTON_NONE)
        {
            m_bhnd(event);
        }
    }
    button_prev = button_now;

    // return event;
}