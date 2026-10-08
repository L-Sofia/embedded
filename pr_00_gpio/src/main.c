/*
  1. Засвітити світлодіод на порті D.
  2. Блимати цим світлодіодом кожні 1.5 сек, загасати на 0.5 сек.
  3. Блимати слово SOS азбукою морзе світлодіодом, між словами пауза 5 сек, між буквами 1 сек.
  4. Блимати 4 світлодіодами на порті D.
  5. Зробити біжучий вогник 1 -> 4. Реалізувати 0.1 сек тінь.
*/
#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000L

void led_init(void);
void morse(char *str);
void blink(void);
void running_fire(void);

int main(void)
{
  led_init();
  char text[] = "...---...";
  while (1)
  {
    morse(text);
    // blink();
    // running_fire();
  }
  return 0;
}

void led_init(void)
{
  DDRD |= (1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD1) | (1 << PD7);
}

void running_fire(void)
{
  for (int8_t i = 1; i < 5; i++)
  {
    PORTD |= (1 << i);
    _delay_ms(500);

    int8_t next = ((i != 4) ? (i + 1) : 1);

    PORTD |= (1 << next);
    _delay_ms(100);
    PORTD &= ~(1 << i);
  }
}

void blink(void)
{
  PORTD |= (1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD1);
  _delay_ms(500);
  PORTD &= ~((1 << PD4) | (1 << PD3) | (1 << PD2) | (1 << PD1));
  _delay_ms(500);
}

void morse(char *str)
{
  for (int8_t i = 0; str[i] != '\0'; i++)
  {
    if (str[i] == '.')
    {
      PORTD |= (1 << PD4);
      _delay_ms(250);
      PORTD &= ~(1 << PD4);
    }
    else
    {
      PORTD |= (1 << PD4);
      _delay_ms(1500);
      PORTD &= ~(1 << PD4);
    }
    if (str[i + 1] != '\0')
    {
      _delay_ms(1000);
    }
  }
  _delay_ms(5000);
}