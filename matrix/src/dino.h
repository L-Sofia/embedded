#ifndef DINO_H
#define DINO_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    DINO_STATE_IDLE,
    DINO_STATE_JUMP_UP,
    DINO_STATE_FALLING
} DinoState;

typedef struct
{
    uint8_t x;
    uint8_t y;           // current bottom row, bottom of dino position
    uint8_t y_ground;    // original ground row
    uint32_t jump_timer; // ms timer for jump
    DinoState state;
} Dino;
#define LAND_SPEED 200
void dino_init(Dino *d);
void dino_draw(Dino *d, uint8_t *buffer);
void dino_update(Dino *d, uint32_t dt_ms, bool jump_pressed);

#endif
