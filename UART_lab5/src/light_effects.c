#include "light_effects.h"
#include "millis.h"
#include "shifter.h"

static bool running = false;
static EffectMode current_effect = EFFECT_RIGHT;
static uint8_t pattern = 1;
static uint32_t prev_time = 0;
static uint8_t direction = 1;
static uint16_t effect_speed = 150;

void LightEffects_Init(void)
{
    shifter_init();
    pattern = 1;
    direction = 1;
    prev_time = millis();
}

void LightEffects_SetSpeed(uint16_t speed_ms)
{
    effect_speed = speed_ms;
}

void LightEffects_SetRunning(bool state)
{
    running = state;
}

void LightEffects_SetEffect(EffectMode mode)
{
    current_effect = mode;
    pattern = 1;
    direction = 1;
}

void LightEffects_Update(void)
{
    if (!running)
        return;

    if (millis() - prev_time < effect_speed)
        return;

    prev_time = millis();

    switch (current_effect)
    {
    case EFFECT_RIGHT:
        shift_out(pattern);
        pattern <<= 1;
        if (pattern == 0)
            pattern = 1;
        break;

    case EFFECT_LEFT:
        shift_out(pattern);
        pattern >>= 1;
        if (pattern == 0)
            pattern = 0x80;
        break;

    case EFFECT_BOUNCE:
        shift_out(pattern);
        if (direction)
        {
            pattern <<= 1;
            if (pattern == 0x80)
                direction = 0;
        }
        else
        {
            pattern >>= 1;
            if (pattern == 0x01)
                direction = 1;
        }
        break;

    case EFFECT_FILL:
    {
        static uint8_t mask = 0;
        mask = (mask << 1) | 1;
        shift_out(mask);
        if (mask == 0xFF)
            mask = 0;
        break;
    }
    }
}