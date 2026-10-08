#include <avr/io.h>
#include <util/delay.h>
#include "button.h"

#define LED1 PD2
#define LED2 PD3
#define LED3 PD4

int main(void)
{
  DDRD |= (1 << LED1) | (1 << LED2) | (1 << LED3);
  button_init();

  while (1)
  {
    button_fsm_update(10);
    _delay_ms(10);

    if (button.pressed)
    {
      PORTD |= (1 << LED1);
      PORTD &= ~((1 << LED2) | (1 << LED3));
    }

    if (button.long_pressed)
    {
      PORTD |= (1 << LED2);
      PORTD &= ~((1 << LED1) | (1 << LED3));
    }

    if (button.double_pressed)
    {
      PORTD |= (1 << LED3);
      PORTD &= ~((1 << LED1) | (1 << LED2));
    }
  }
}
