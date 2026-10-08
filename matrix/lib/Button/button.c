#include "button.h"
#include <avr/io.h>
#include <stdbool.h>

button_FSM_t button = {
    .state = BUTTON_STATE_IDLE,
    .counter = 0,
    .pressed = false,
    .long_pressed = false,
    .was_long = false};

void button_init(void)
{
    DDRB &= ~(1 << BTN);
    PORTB |= (1 << BTN);
}

static bool button_read(void)
{
    return !(PINB & (1 << BTN));
}

void button_fsm_pool(uint32_t dt_ms)
{
    bool btn = button_read();
    button.pressed = false;
    button.long_pressed = false;

    switch (button.state)
    {
    case BUTTON_STATE_IDLE:
        if (btn)
        {
            button.state = BUTTON_STATE_DEBOUNCE;
            button.counter = 0;
        }
        break;

    case BUTTON_STATE_DEBOUNCE:
        if (btn)
        {
            button.counter += dt_ms;
            if (button.counter >= DEBOUNCE_MS)
            {
                button.state = BUTTON_STATE_PRESSED;
                button.counter = 0;
                button.was_long = false;
            }
        }
        else
        {
            button.state = BUTTON_STATE_IDLE;
        }
        break;

    case BUTTON_STATE_PRESSED:
        if (btn)
        {
            button.counter += dt_ms;
            if (button.counter >= LONG_HOLD_MS)
            {
                button.state = BUTTON_STATE_LONG_PRESS;
                button.long_pressed = true;
                button.was_long = true;
            }
        }
        else
        {
            button.state = BUTTON_STATE_RELEASE;
            button.counter = 0;
        }
        break;

    case BUTTON_STATE_LONG_PRESS:
        if (!btn)
        {
            button.state = BUTTON_STATE_RELEASE;
            button.counter = 0;
        }
        break;

    case BUTTON_STATE_RELEASE:
        if (!btn)
        {
            button.counter += dt_ms;
            if (button.counter >= DEBOUNCE_MS)
            {
                if (!(button.was_long))
                {
                    button.pressed = true;
                }
                button.state = BUTTON_STATE_IDLE;
                button.counter = 0;
            }
        }
        else
        {
            button.state = BUTTON_STATE_PRESSED;
        }
        break;

    default:
        button.state = BUTTON_STATE_IDLE;
        break;
    }
}