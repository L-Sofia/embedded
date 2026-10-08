#include "button.h"
#include <avr/io.h>
#include <stdbool.h>

ButtonFSM_t button = {
    .state = BUTTON_STATE_IDLE,
    .counter = 0,
    .pressed = false,
    .long_pressed = false,
    .double_pressed = false,
    .waiting_second_click = false,
    .double_click_timer = 0,
    .was_long = false,
    .pending_single = false};

void button_init(void)
{
    DDRB &= ~(1 << BTN);
    PORTB |= (1 << BTN);
}

static bool button_read(void)
{
    return !(PINB & (1 << BTN)); // returns true when pressed
}

void button_fsm_update(uint32_t dt_ms)
{
    bool btn = button_read();

    button.pressed = false;
    button.long_pressed = false;
    button.double_pressed = false;

    // process pending single click (delayed single-click emission)
    if (button.pending_single)
    {
        button.double_click_timer += dt_ms;
        if (button.double_click_timer >= DOUBLE_CLICK_MS)
        {
            button.pending_single = false;
            button.waiting_second_click = false;
            button.double_click_timer = 0;
            button.pressed = true; // emit the single click now
        }
    }

    switch (button.state)
    {
    case BUTTON_STATE_IDLE: //-------------IDLE---------------
        if (btn)
        {
            button.state = BUTTON_STATE_DEBOUNCE;
            button.counter = 0;
        }
        break;

    case BUTTON_STATE_DEBOUNCE: //-------------CHECKING PRESS ON BUTTON---------------
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

    case BUTTON_STATE_PRESSED: //-------------BUTTON PRESSED, CHECKING TIMERS---------------
        if (btn)
        {
            button.counter += dt_ms;
            if (!button.was_long && button.counter >= LONG_HOLD_MS)
            {
                button.state = BUTTON_STATE_LONG_PRESS;
                button.long_pressed = true; // emit immediately
                button.was_long = true;
                button.waiting_second_click = false;
                button.pending_single = false;
                button.double_click_timer = 0;
            }
        }
        else
        {
            button.state = BUTTON_STATE_RELEASE_DEBOUNCE;
            button.counter = 0;
        }
        break;

    case BUTTON_STATE_LONG_PRESS: //-------------LONG PRESS---------------
        if (!btn)
        {
            button.state = BUTTON_STATE_RELEASE_DEBOUNCE;
            button.counter = 0;
        }
        break;

    case BUTTON_STATE_RELEASE_DEBOUNCE: //-------------CHECKING BUTTON RELEASE---------------
        if (!btn)
        {
            button.counter += dt_ms;
            if (button.counter >= DEBOUNCE_MS)
            {
                if (!button.was_long)
                {
                    if (!button.waiting_second_click)
                    {
                        // First short click -> start waiting for possible double click
                        button.waiting_second_click = true;
                        button.pending_single = true; // delay single-click emit
                        button.double_click_timer = 0;
                    }
                    else
                    {
                        if (button.double_click_timer <= DOUBLE_CLICK_MS)
                        {
                            // Double click detected
                            button.double_pressed = true;
                            button.waiting_second_click = false;
                            button.pending_single = false; // cancel pending single
                            button.double_click_timer = 0;
                        }
                        else
                        {
                            button.waiting_second_click = true;
                            button.pending_single = true;
                            button.double_click_timer = 0;
                        }
                    }
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