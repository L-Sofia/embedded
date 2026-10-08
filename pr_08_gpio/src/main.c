/*
  1. Зчитати кнопку, порахувати скільки часу вона була затиснута.
  2. Натискання > 2000 мс - перемкнути ефект. Менше 2000 мс - зупинити/почати.
  3. Перемикати ефект можна тільки на паузі.
*/
#include <avr/io.h>
#include <util/delay.h>

uint32_t ms = 0;
//----------------------------------
static uint8_t is_released = 0;
static uint32_t dur = 0;
static uint8_t is_pressed = 0;
static uint32_t press_st = 0;
static uint8_t curr_eff = 0;
static uint8_t run = 0;

void tick(void);
void led_init(void);
void btn_init(uint8_t pin);
void blink_handler(uint8_t eff_num);
uint32_t get_btn_dur(void);
void btn_handler(void);
void btn_scan(void);

void eff1(void);
void eff2(void);
void eff3(void);

int main(void)
{
  led_init();
  btn_init(PB0);
  while (1)
  {
    tick();
    btn_scan();
    if (run)
    {
      blink_handler(curr_eff);
    }
  }
  return 0;
}

void tick(void)
{
  ms++;
  _delay_ms(1);
}

void led_init(void)
{
  DDRD = 0xFF;
}

void btn_init(uint8_t pin)
{
  DDRB &= ~(1 << pin);
  PORTB |= (1 << pin);
}

void btn_handler(void)
{
  uint32_t val = get_btn_dur();
  if (val > 2000)
  {
    if (!run)
    {
      curr_eff = (curr_eff >= 2) ? 0 : curr_eff + 1;
    }
  }
  else
  {
    run = !run;
  }
}

void btn_scan(void)
{
  if (!(PINB & (1 << PB0)) && !is_pressed)
  {
    _delay_ms(35);
    if (!(PINB & (1 << PB0)))
    {
      press_st = ms;
      is_pressed = 1;
    }
  }

  if ((PINB & (1 << PB0)) && is_pressed)
  {
    _delay_ms(35);
    if (PINB & (1 << PB0))
    {
      is_pressed = 0;
      is_released = 1;
      dur = ms - press_st;
      btn_handler();
    }
  }
}

uint32_t get_btn_dur(void)
{
  if (is_released)
  {
    is_released = 0;
    return dur;
  }
  return 0;
}

void eff1(void)
{
  static uint8_t i1 = 0;
  static uint32_t last_1 = 0;
  if (ms - last_1 >= 450)
  {
    last_1 = ms;
    PORTD = (1 << i1);
    i1++;
    if (i1 > 7)
      i1 = 0;
  }
}

void eff2(void)
{
  static uint8_t i2 = 7;
  static uint32_t last_2 = 0;
  if (ms - last_2 >= 450)
  {
    last_2 = ms;
    PORTD = (1 << i2);
    if (i2 == 0)
      i2 = 7;
    else
      i2--;
  }
}

void eff3(void)
{
  static uint8_t i3 = 0;
  static uint32_t last_3 = 0;
  static uint8_t dir = 1;
  static uint8_t mask = 0;
  if (ms - last_3 >= 450)
  {
    last_3 = ms;
    PORTD = mask;

    if (dir)
    {
      mask = (mask << 1) | 1;
      if (mask == 0xff)
        dir = 0;
    }
    else
    {
      mask <<= 1;
      if (mask == 0x00)
        dir = 1;
    }

    i3++;
    if (i3 > 7)
      i3 = 0;
  }
}

void blink_handler(uint8_t eff_num)
{
  switch (eff_num)
  {
  case 0:
    eff1();
    break;
  case 1:
    eff2();
    break;
  case 2:
    eff3();
    break;
  default:
    break;
  }
}