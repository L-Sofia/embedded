#ifndef LIGHT_EFFECTS_H
#define LIGHT_EFFECTS_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    EFFECT_RIGHT,
    EFFECT_LEFT,
    EFFECT_BOUNCE,
    EFFECT_FILL,
} EffectMode;

void LightEffects_Init(void);
void LightEffects_Update(void);
void LightEffects_SetRunning(bool state);
void LightEffects_SetEffect(EffectMode mode);
void LightEffects_SetSpeed(uint16_t speed_ms);

#endif
