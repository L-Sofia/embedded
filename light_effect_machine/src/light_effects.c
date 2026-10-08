#include <avr/io.h>
#include "light_effects.h"

static uint8_t pos_fire;
static uint8_t pos_shadow;
static uint8_t pos_johnson;
uint16_t effect_period = 150;
static uint16_t tick = 0;

uint8_t time_to_update(uint16_t period_ms)
{
    tick += 5;
    if (tick >= period_ms)
    {
        tick = 0;
        return 1;
    }
    return 0;
}

void light_effect_speed_task()
{
    if (effect_period > 50)
    {
        effect_period -= 50;
    }
    else if (effect_period <= 50)
    {
        effect_period = 150;
    }
}

void light_effects_reset(void)
{
    pos_fire = 0;
    pos_shadow = 8;
    pos_johnson = 0b00000000;
}

static void effect_running_fire()
{
    if (time_to_update(effect_period))
    {
        PORTD = (1 << pos_fire);
        pos_fire++;
        if (pos_fire >= 8)
        {
            pos_fire = 0;
        }
    }
}
static void effect_running_shadow()
{
    if (time_to_update(effect_period))
    {
        PORTD = (1 << pos_shadow);
        if (pos_shadow == 0)
            pos_shadow = 8;
        pos_shadow--;
    }
}
static void effect_johnson_counter(void)
{
    static uint8_t pattern = 0;
    static uint8_t filling = 1;
    if (time_to_update(effect_period))
    {
        if (filling)
        {
            pattern |= (1 << pos_johnson);
            pos_johnson++;
            if (pos_johnson >= 8)
            {
                pos_johnson = 0;
                filling = 0;
            }
        }
        else
        {
            pattern &= ~(1 << pos_johnson);
            pos_johnson++;
            if (pos_johnson >= 8)
            {
                pos_johnson = 0;
                filling = 1;
            }
        }
        PORTD = pattern;
    }
}

void light_effects_set_task(light_effects_t effect)
{
    switch (effect)
    {
    case LF_RUNNING_FIRE:
        effect_running_fire();
        break;

    case LF_RUNNING_SHADOW:
        effect_running_shadow();
        break;

    case LF_JOHNSON_COUNTER:
        effect_johnson_counter();
        break;

    default:
        PORTD = 0x00;
        break;
    }
}