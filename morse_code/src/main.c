#include <avr/io.h>
#include <util/delay.h>
#define LED PD3

void morse_blink(char *msg)
{
  while (*msg)
  {
    if (*msg == '.')
    {
      PORTD |= (1 << LED);
      _delay_ms(200);
      PORTD &= ~(1 << LED);
    }
    else if (*msg == '-')
    {
      PORTD |= (1 << LED);
      _delay_ms(600);
      PORTD &= ~(1 << LED);
    }
    _delay_ms(1000);
    msg++;
  }
  _delay_ms(2000);
}

int main(void)
{
  DDRD |= (1 << LED);

  for (;;)
  {
    morse_blink("...---...");
  }
  return 0;
}