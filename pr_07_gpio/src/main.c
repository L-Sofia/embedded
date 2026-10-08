/*
  1. Реалізувати JK тригер, PB0 - K, PB1 - J, PB2 - C, PD0 - Q і PF7 - !Q
*/
#include <avr/io.h>

void led_init(void)
{
  DDRD = 0xFF;
}

void btn_init(uint8_t pin)
{
  DDRB &= ~(1 << pin);
  PORTB |= (1 << pin);
}

uint8_t btn_get(uint8_t pin)
{
  return !(PINB & (1 << pin));
}

uint8_t btn_handle(void)
{
  static uint8_t Q = 0;
  uint8_t J = btn_get(PB1);
  uint8_t K = btn_get(PB0);
  uint8_t C = btn_get(PB2);

  if (C)
  {
    if (J && !K)
    {
      Q = 1;
    }
    if (!J && K)
    {
      Q = 0;
    }
    if (J && K)
    {
      Q = !Q;
    }
  }
  return Q;
}

void flip_jk(void)
{
  uint8_t val = btn_handle();

  if (val == 1)
  {
    PORTD |= (1 << PD0);
    PORTD &= ~(1 << PD7);
  }
  else
  {
    PORTD &= ~(1 << PD0);
    PORTD |= (1 << PD7);
  }
}

int main(void)
{
  led_init();
  btn_init(PB0); // K
  btn_init(PB1); // J
  btn_init(PB2); // C
  while (1)
  {
    flip_jk();
  }
  return 0;
}