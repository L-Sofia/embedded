#include "ground.h"

// Fill the buffer with a simple bottom row
void ground_init(uint8_t *buffer)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        buffer[i] = 0x00;
    }
    buffer[7] = 0xFF; // bottom row fully lit
}

// Draw ground on buffer (for now it just leaves the bottom row)
void ground_draw(uint8_t *buffer)
{
    // Already initialized in buffer; nothing dynamic yet
}
