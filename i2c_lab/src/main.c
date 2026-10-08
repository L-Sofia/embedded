#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "i2c.h"
#include "lcd.h"
#include "uart.h"
#include "lm75.h"
#include "ds1307.h"

void my_print(const char *s)
{
  printf("%s", s);
}

void print_temp(void)
{
  // char buf[32];
  // int16_t t = lm75_get_temp_x10();
  // sprintf(buf, "T = %d.%d  ", t / 10, t % 10);
  // LCD_Print(1, 1, buf);
  // int16_t a, b;
  // lm75_get_temp(&a, &b);
  // printf("T = %d.%02d\n", a, b);
  // sprintf(buf, "%d.%02d   ", a, b);
  // LCD_Print(1, 1, buf);
}

int main(void)
{
  USART_Init(9600);
  I2C_Init();
  LCD_Init(0x4E);
  lm75_init();

  // LCD_Print(0, 0, "Hello!");

  I2C_ScanConnectedDevices(my_print);
  LCD_Clear();

  DS1307_Init();
  DS1307_SetTime(12, 00, 00);

  LCD_Print(0, 0, "DS1307 RTC");
  _delay_ms(1500);

  ds1307_time_t time;
  char buf[17];

  for (;;)
  {
    // LCD_Print(0, 0, "Hello!");

    // print_temp();
    // _delay_ms(700);

    DS1307_GetTime(&time);

    sprintf(buf, "%02d:%02d:%02d",
            time.hour, time.min, time.sec);

    LCD_Clear();
    LCD_Print(3, 0, "TIME");
    LCD_Print(4, 1, buf);

    _delay_ms(1000);
  }

  return 0;
}