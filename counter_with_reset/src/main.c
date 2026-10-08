#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#define period_ms 1000
#define BTN1 PB0
#define BTN2 PB1

static uint32_t millis = 0;
static uint32_t last_time = 0;
static uint8_t i = 0;
static bool running = false;
static uint8_t seconds[10] = {
    0b0111111,
    0b0000110,
    0b1011011,
    0b1001111,
    0b1100110,
    0b1101101,
    0b1111101,
    0b0000111,
    0b1111111,
    0b1101111};

uint8_t update_time(uint32_t *last_time_ms);
void btn1_scan();
void btn2_scan();
void btn1_handler();
void btn2_handler();
void counter();

uint8_t update_time(uint32_t *last_time_ms)
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
  DDRB &= ~((1 << BTN1) | (1 << BTN2));
  PORTB |= (1 << BTN1) | (1 << BTN2);

  for (;;)
  {
    btn1_scan();
    btn2_scan();

    if (running && update_time(&last_time))
    {
      counter();
    }

    millis += 5;
    _delay_ms(5);
  }
  return 0;
}

void btn1_scan()
{
  static bool button_pressed = false;
  static uint32_t last_button_press_ms = 0;
  uint32_t current_ms = millis;
  uint8_t pressed = PINB & (1 << BTN1);

  if (!pressed)
  {
    if (current_ms - last_button_press_ms > 50)
    {
      last_button_press_ms = current_ms;
      if (!button_pressed)
      {
        button_pressed = true;
      }
    }
  }
  else
  {
    if (button_pressed && current_ms - last_button_press_ms > 50)
    {
      button_pressed = false;
      btn1_handler();
    }
  }
}

void btn2_scan()
{
  static bool button_pressed = false;
  static uint32_t last_button_press_ms = 0;
  uint32_t current_ms = millis;
  uint8_t pressed = PINB & (1 << BTN2);

  if (!pressed)
  {
    if (current_ms - last_button_press_ms > 50)
    {
      last_button_press_ms = current_ms;
      if (!button_pressed)
      {
        button_pressed = true;
      }
    }
  }
  else
  {
    if (button_pressed && current_ms - last_button_press_ms > 50)
    {
      button_pressed = false;
      btn2_handler();
    }
  }
}

void btn1_handler()
{
  running = !running;
}

void btn2_handler()
{
  PORTD = 0x00;
  i = 0;
}

void counter()
{
  PORTD = seconds[i];
  i++;
  if (i == 10)
    i = 0;
}
