#include "obstacle.h"

#define OBSTACLE_MOVE_INTERVAL 350   // ms per pixel
#define OBSTACLE_SPAWN_INTERVAL 2100 // ms between spawns

const uint16_t spawn_pattern[] = {
    2000, 2000, 2000, 1000, 2000, 2000, 2000, 1000};
#define PATTERN_LEN (sizeof(spawn_pattern) / sizeof(spawn_pattern[0]))

static uint8_t pattern_index = 0;
static uint32_t spawn_timer = 0;

static uint32_t move_timer = 0;

void obstacles_init(Obstacle obs[], uint8_t count)
{
    for (uint8_t i = 0; i < count; i++)
    {
        obs[i].active = false;
        obs[i].x = 0;
        obs[i].prev_x = 0;
        obs[i].y = 6; // just above ground row
    }
    spawn_timer = 0;
    move_timer = 0;
}

void obstacles_update(Obstacle obs[], uint8_t count, uint32_t dt_ms)
{
    move_timer += dt_ms;
    spawn_timer += dt_ms;

    // Move obstacles left
    if (move_timer >= OBSTACLE_MOVE_INTERVAL)
    {
        move_timer = 0;
        for (uint8_t i = 0; i < count; i++)
        {
            if (obs[i].active)
            {
                // Save previous x to clear later
                obs[i].prev_x = obs[i].x;

                if (obs[i].x > 0)
                    obs[i].x--;
                else
                    obs[i].active = false; // off-screen
            }
        }
    }

    // Spawn new obstacle according to pattern
    if (spawn_timer >= spawn_pattern[pattern_index])
    {
        spawn_timer = 0;

        // Find first inactive obstacle to spawn
        for (uint8_t i = 0; i < count; i++)
        {
            if (!obs[i].active)
            {
                obs[i].active = true;
                obs[i].x = 7;        // rightmost
                obs[i].prev_x = 255; // nothing to clear yet
                obs[i].y = 6;        // above ground
                break;
            }
        }

        // Move to next interval in the pattern
        pattern_index = (pattern_index + 1) % PATTERN_LEN;
    }
}

void obstacles_draw(Obstacle obs[], uint8_t count, uint8_t *buffer)
{
    for (uint8_t i = 0; i < count; i++)
    {
        // Clear previous pixel regardless of active status
        if (obs[i].prev_x < 8)
            buffer[obs[i].y] &= ~(1 << obs[i].prev_x);

        // Draw current pixel only if active
        if (obs[i].active)
            buffer[obs[i].y] |= (1 << obs[i].x);
    }
}

bool check_collision(Obstacle obs[], uint8_t count, uint8_t dino_x, uint8_t dino_y)
{
    for (uint8_t i = 0; i < count; i++)
    {
        if (obs[i].active)
        {
            if (obs[i].x == dino_x && (obs[i].y == dino_y || obs[i].y == dino_y - 1))
                return true;
        }
    }
    return false;
}
