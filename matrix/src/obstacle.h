#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_OBSTACLES 2

typedef struct
{
    uint8_t x;      // current column
    uint8_t prev_x; // previous column (for clearing)
    uint8_t y;      // row above ground
    bool active;
} Obstacle;

void obstacles_init(Obstacle obs[], uint8_t count);
void obstacles_update(Obstacle obs[], uint8_t count, uint32_t dt_ms);
void obstacles_draw(Obstacle obs[], uint8_t count, uint8_t *buffer);

bool check_collision(Obstacle obs[], uint8_t count, uint8_t dino_x, uint8_t dino_y);

#endif
