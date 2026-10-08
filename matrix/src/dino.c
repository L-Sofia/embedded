#include "dino.h"

// Simple 2x1 dino
void dino_init(Dino *d)
{
    d->x = 3;
    d->y_ground = 6; // bottom row of dino
    d->y = d->y_ground;
    d->jump_timer = 0;
    d->state = DINO_STATE_IDLE;
}

// Draw dino on buffer
void dino_draw(Dino *d, uint8_t *buffer)
{
    for (uint8_t row = 0; row < 7; row++)
    {
        buffer[row] &= ~(1 << d->x);
    }
    // Clear dino area first
    // buffer[d->y] &= ~(1 << d->x);
    // buffer[d->y - 1] &= ~(1 << d->x);

    // Draw dino
    buffer[d->y] |= (1 << d->x);
    buffer[d->y - 1] |= (1 << d->x);
}
#define DINO_GROUND_Y 6
#define DINO_JUMP_TOP 3
void dino_update(Dino *d, uint32_t dt_ms, bool jump_pressed)
{
    switch (d->state)
    {
    case DINO_STATE_IDLE:
        if (jump_pressed)
        {
            d->state = DINO_STATE_JUMP_UP;
            d->jump_timer = 0;
        }
        break;

    case DINO_STATE_JUMP_UP:
        d->jump_timer += dt_ms;
        if (d->jump_timer >= 100) // rise every 50ms
        {
            d->y--; // move up 1 pixel
            d->jump_timer = 0;

            if (d->y <= DINO_JUMP_TOP)
                d->state = DINO_STATE_FALLING; // start falling
        }
        break;

    case DINO_STATE_FALLING:
        d->jump_timer += dt_ms;
        if (d->jump_timer >= LAND_SPEED)
        {
            d->y++; // move down 1 pixel
            d->jump_timer = 0;

            if (d->y >= DINO_GROUND_Y)
                d->state = DINO_STATE_IDLE; // landed
        }
        break;
    }
}