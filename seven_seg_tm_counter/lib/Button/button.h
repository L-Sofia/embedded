#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>

#define BTN PB0
#define DEBOUNCE_MS 20
#define LONG_HOLD_MS 2000

typedef enum
{
    BUTTON_STATE_IDLE,
    BUTTON_STATE_DEBOUNCE,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_LONG_PRESS,
    BUTTON_STATE_RELEASE
} button_state_t;

typedef struct
{
    button_state_t state;
    uint32_t counter;
    bool pressed;
    bool long_pressed;
    bool was_long;
} button_FSM_t;

void button_init(void);
void button_fsm_pool(uint32_t dt_ms);
extern button_FSM_t button;

#endif