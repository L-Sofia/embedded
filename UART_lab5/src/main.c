#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "uart.h"
#include "shifter.h"
#include "millis.h"
#include "light_effects.h"

void uart_command_handler(uint8_t *line)
{
  char *cmd = (char *)line;

  for (int i = 0; cmd[i]; i++)
  {
    if (cmd[i] == '\r' || cmd[i] == '\n')
    {
      cmd[i] = 0;
      break;
    }
  }

  if (cmd[0] == 0)
    return;

  printf("Got command: %s\r\n", line);

  if (strcmp(cmd, "start") == 0)
  {
    LightEffects_SetRunning(true);
    printf("Effect started\r\n");
  }
  else if (strcmp(cmd, "stop") == 0)
  {
    LightEffects_SetRunning(false);
    printf("Effect stopped\r\n");
  }
  else if (strcmp(cmd, "right") == 0)
  {
    LightEffects_SetEffect(EFFECT_RIGHT);
    printf("Effect: right\r\n");
  }
  else if (strcmp(cmd, "left") == 0)
  {
    LightEffects_SetEffect(EFFECT_LEFT);
    printf("Effect: left\r\n");
  }
  else if (strcmp(cmd, "bounce") == 0)
  {
    LightEffects_SetEffect(EFFECT_BOUNCE);
    printf("Effect: bounce\r\n");
  }
  else if (strcmp(cmd, "fill") == 0)
  {
    LightEffects_SetEffect(EFFECT_FILL);
    printf("Effect: fill\r\n");
  }
  else if (strncmp(cmd, "speed ", 6) == 0)
  {
    uint16_t spd = atoi(&cmd[6]);
    if (spd >= 50 && spd <= 2000)
    {
      LightEffects_SetSpeed(spd);
      printf("Speed set to %u ms\r\n", spd);
    }
    else
    {
      printf("Invalid speed! Use 50-2000 ms\r\n");
    }
  }
  else if (strcmp(cmd, "help") == 0)
  {
    printf("Help: start - starts effect;\n stop - stops effect;\n set speed (value 50 - 1000);\n right - change effect;\n left - change effect;\n bpunce - change effect;\n fill - change effect;\r\n");
  }
  else
  {
    printf("Unknown command: %s\r\n", cmd);
  }
}

int main(void)
{
  USART_Init(115200);
  shifter_init();
  InitTimer0();

  printf("System ready. Commands: start, stop, right, left, bounce, fill...\r\n");
  // LOG_DEBUG("Main", "Hello debug!");

  USART_set_callback(uart_command_handler);
  printf("$ ");
  for (;;)
  {
    USART_poll();
    LightEffects_Update();
  }
  return 0;
}