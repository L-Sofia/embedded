#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t toggle_rq = 0;
volatile uint8_t is_pressed = 0;
volatile uint8_t led_on = 0;

void led_init(void);
void btn_init(void);
void led_task(void);

ISR(PCINT0_vect)
{
  if (!(PINB & (1 << PB0)))
  {
    if (!is_pressed)
    {
      is_pressed = 1;
    }
  }
  else
  {
    is_pressed = 0;
    toggle_rq = 1;
  }
}

int main(void)
{
  btn_init();
  led_init();

  PCICR |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT0);
  sei();
  while (1)
  {
    if (toggle_rq)
    {
      toggle_rq = 0;
      led_on = !led_on;
      led_task();
    }
  }
  return 0;
}

void btn_init(void)
{
  DDRB &= ~(1 << PB0);
  PORTB |= (1 << PB0);
}

void led_init(void)
{
  DDRD = 0xFF;
}

void led_task(void)
{
  if (led_on)
  {
    PORTD |= (1 << PD4);
  }
  else
  {
    PORTD &= ~(1 << PD4);
  }
}