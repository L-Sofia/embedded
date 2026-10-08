#ifndef LIGHT_EFFECTS_H
#define LIGHT_EFFECTS_H
#include <stdint.h>

typedef enum
{
    LF_RUNNING_FIRE = 0,
    LF_RUNNING_SHADOW,
    LF_JOHNSON_COUNTER,
    NUM_EFFECTS
} light_effects_t;
void light_effects_set_task(light_effects_t effect);
void light_effect_speed_task();
void light_effects_reset(void);

#endif