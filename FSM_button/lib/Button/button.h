#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>

#define BTN PB0
#define DEBOUNCE_MS 20
#define LONG_HOLD_MS 1000
#define DOUBLE_CLICK_MS 300

typedef enum
{
    BUTTON_STATE_IDLE,
    BUTTON_STATE_DEBOUNCE,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_LONG_PRESS,
    BUTTON_STATE_RELEASE_DEBOUNCE
} ButtonState_t;

typedef struct
{
    ButtonState_t state;
    uint32_t counter;
    bool pressed;                // short click event
    bool long_pressed;           // long press event
    bool double_pressed;         // double click event
    bool waiting_second_click;   // waiting for second click
    uint32_t double_click_timer; // ms since first release
    bool was_long;               // did this press reach long threshold
    bool pending_single;         // waiting to confirm single click
} ButtonFSM_t;

void button_init(void);
void button_fsm_update(uint32_t dt_ms);
extern ButtonFSM_t button;

#endif