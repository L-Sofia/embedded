#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

ISR(INT0_vect)
{
}

int main(void)
{
  DDRD |= (1 << PD1) | (1 << PD2) | (1 << PD3) | (1 << PD4);
  DDRD &= ~(1 << PD0);
  PORTD |= (1 << PD0);
  sei();
  wdt_enable(WDTO_4S);
  while (1)
  {
  }
  return 0;
}