#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#include <button.h>
#include "spi.h"

volatile uint32_t millis = 0;

const uint8_t digit_map[10] = {
    0b11000000, // 0
    0b11111001, // 1
    0b10100100, // 2
    0b10110000, // 3
    0b10011001, // 4
    0b10010010, // 5
    0b10000010, // 6
    0b11111000, // 7
    0b10000000, // 8
    0b10010000  // 9
};

const uint8_t letters_map[] = {};

const uint8_t reg_select[4] = {
    0b00000001, // D1
    0b00000010, // D2
    0b00000100, // D3
    0b00001000  // D4
};

uint8_t do_millis(uint32_t *last_millis, uint32_t period)
{
  if ((millis - *last_millis) >= period)
  {
    *last_millis = millis;
    return 1;
  }
  return 0;
}

void shift_out(uint8_t segments, uint8_t reg)
{
  PORTB &= ~(1 << LATCH);
  spi_send(segments);
  spi_send(reg);
  PORTB |= (1 << LATCH);
}

uint8_t nums[4];

void disp_print_num(uint16_t val)
{
  nums[0] = val % 10;
  nums[1] = (val / 10) % 10;
  nums[2] = (val / 100) % 10;
  nums[3] = (val / 1000) % 10;
}

void update_display(uint8_t period)
{
  static uint8_t dl1 = 0;
  static uint8_t pos = 0;

  if (++dl1 > period)
  {
    shift_out(digit_map[nums[pos]], reg_select[pos]);
    pos++;
    if (pos >= 4)
      pos = 0;
    dl1 = 0;
  }
}

void update_counter(uint32_t *counter, uint32_t *last_ms, uint32_t speed_ms)
{
  if (do_millis(last_ms, speed_ms))
  {
    (*counter)++;
    if (*counter > 9999)
      *counter = 0;
  }
}

static const uint8_t seg_mask[7] = {
    0b11111110, // a
    0b11111101, // b
    0b11111011, // c
    0b11110111, // d
    0b11101111, // e
    0b11011111, // f
    0b10111111  // g
};

static const uint8_t circle_digits[] = {
    0, // D1 a
    1, // D2 a
    2, // D3 a
    3, // D4 a
    3, // D4 b
    3, // D4 c
    3, // D4 d
    2, // D3 d
    1, // D2 d
    0, // D1 d
    0, // D1 e
    0, // D1 f
    0  // D1 g
};

static const uint8_t circle_segs[] = {
    0, // a
    0, // a
    0, // a
    0, // a
    5, // f
    4, // e
    3, // d
    3, // d
    3, // d
    3, // d
    2, // c
    1, // b
    0  // a
};

#define CIRCLE_STEPS (sizeof(circle_digits) / sizeof(circle_digits[0]))

void segment_circle(uint16_t delay_ms, uint16_t cycles)
{
  uint16_t step_index = 0;
  uint16_t completed_cycles = 0;

  while (cycles == 0 || completed_cycles < cycles)
  {
    uint8_t target_digit = circle_digits[step_index];
    uint8_t target_seg = circle_segs[step_index];

    uint16_t elapsed = 0;
    const uint8_t refresh_slot_ms = 3;
    while (elapsed < delay_ms)
    {
      for (uint8_t d = 0; d < 4; d++)
      {
        uint8_t seg_pattern;
        if (d == target_digit)
          seg_pattern = seg_mask[target_seg];
        else
          seg_pattern = 0xFF;

        shift_out(seg_pattern, reg_select[d]);
        _delay_ms(refresh_slot_ms);
        millis += refresh_slot_ms;
      }
      elapsed += (4 * refresh_slot_ms);
    }

    step_index++;
    if (step_index >= CIRCLE_STEPS)
    {
      step_index = 0;
      completed_cycles++;
    }
  }
}

int main(void)
{
  spi_init();
  button_init();

  bool running = false;
  uint32_t counter = 0;
  uint32_t last_ms = 0;
  uint32_t speed_ms = 930;
  for (;;)
  {
    button_fsm_pool(2);
    update_display(5);

    if (button.pressed)
    {
      running = !running;
    }

    if (button.long_pressed)
    {
      counter = 0;
      disp_print_num(counter);
    }

    if (running)
    {
      // segment_circle(100, 0);
      update_counter(&counter, &last_ms, speed_ms);
      disp_print_num(counter); // if counter changet
    }

    _delay_ms(1);
    millis++;
  }
  return 0;
}