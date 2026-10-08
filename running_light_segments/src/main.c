#include <avr/io.h>
#include <util/delay.h>
#define period_ms 150

static uint16_t millis = 0;

uint8_t update_time(uint16_t *last_time_ms)
{
  if (millis - *last_time_ms >= period_ms)
  {
    *last_time_ms = millis;
    return 1;
  }
  return 0;
}

int main(void)
{
  DDRD = 0xff;
  uint8_t i = 0;
  uint16_t last_time = 0;
  const uint8_t segments[6] = {
      0b0000001,
      0b0000010,
      0b0000100,
      0b0001000,
      0b0010000,
      0b0100000};

  for (;;)
  {
    if (update_time(&last_time))
    {
      PORTD = segments[i];
      i++;
      if (i == 6)
      {
        i = 0;
      }
    }
    millis += 5;
    _delay_ms(5);
  }
  return 0;
}