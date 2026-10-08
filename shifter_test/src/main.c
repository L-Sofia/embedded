
#include <avr/io.h>
#include <util/delay.h>

#define DATA_PIN PB3  // D11 -> DS
#define LATCH_PIN PB2 // D10 -> ST_CP
#define CLOCK_PIN PB5 // D13 -> SH_CP

void shiftOut(uint8_t data)
{
  for (int i = 0; i < 8; i++)
  {
    if (data & 0x80)
      PORTB |= (1 << DATA_PIN);
    else
      PORTB &= ~(1 << DATA_PIN);

    PORTB |= (1 << CLOCK_PIN);
    _delay_us(1);
    PORTB &= ~(1 << CLOCK_PIN);

    data <<= 1;
  }
}

int main(void)
{
  // Налаштування виводів як виходи
  DDRB |= (1 << DATA_PIN) | (1 << LATCH_PIN) | (1 << CLOCK_PIN);

  uint8_t value = 0;

  while (1)
  {
    // Лічимо від 0 до 255
    for (value = 0; value < 255; value++)
    {
      // Готуємо передавання
      PORTB &= ~(1 << LATCH_PIN); // LATCH LOW
      shiftOut(value);
      PORTB |= (1 << LATCH_PIN); // LATCH HIGH — оновлення виводів
      _delay_ms(200);
    }
  }
}