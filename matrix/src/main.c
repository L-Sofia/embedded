#include <avr/io.h>
#include "spi.h"
#include "max7219.h"
#include "millis.h"
#include "button.h"
#include "dino.h"
#include "ground.h"
#include "obstacle.h"

Obstacle obstacles[MAX_OBSTACLES];
uint8_t buffer[8];
Dino dino;

typedef enum
{
  GAME_RUNNING,
  GAME_OVER
} GameState;

GameState game_state = GAME_RUNNING;

uint8_t sad_face[8] = {
    0b00111100, // top row
    0b01000010,
    0b10100101,
    0b10000001,
    0b10111101,
    0b10000001,
    0b01000010,
    0b00111100 // bottom row
};

int main(void)
{
  spi_init();
  max7219_init();
  InitTimer0();
  button_init();

  obstacles_init(obstacles, MAX_OBSTACLES);
  ground_init(buffer);
  dino_init(&dino);

  uint32_t dl_ms, tmp;
  dl_ms = millis();

  for (;;)
  {
    tmp = millis();
    uint32_t dt = tmp - dl_ms;
    dl_ms = tmp;

    button_fsm_pool(dt);

    if (game_state == GAME_RUNNING)
    {
      // Update game objects
      dino_update(&dino, dt, button.pressed);
      obstacles_update(obstacles, MAX_OBSTACLES, dt);

      // Draw frame
      ground_draw(buffer);
      dino_draw(&dino, buffer);
      obstacles_draw(obstacles, MAX_OBSTACLES, buffer);
      max7219_display(buffer, 8, 8);

      // Check collision
      if (check_collision(obstacles, MAX_OBSTACLES, dino.x, dino.y))
      {
        game_state = GAME_OVER;
      }
    }
    else if (game_state == GAME_OVER)
    {
      // Display sad face
      max7219_display(sad_face, 8, 8);

      // Restart game on short button press
      if (button.pressed)
      {
        // Reset game objects
        dino_init(&dino);
        obstacles_init(obstacles, MAX_OBSTACLES);
        // Clear buffer
        for (uint8_t i = 0; i < 7; i++)
        {
          buffer[i] = 0;
        }

        game_state = GAME_RUNNING;
      }
    }
  }

  return 0;
}