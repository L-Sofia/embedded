#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <uart.h>
#include <adc.h>

#define VREF_MV 1100U
#define pin 0

int main(void)
{
  USART_Init(9600);
  ADC_Init();
  printf("ADC ready!\r\n");

  for (;;)
  {
    uint32_t val = ADC_Read(pin);
    // uint32_t mv = (val / 1023) * VREF_MV;
    uint32_t mv = (val * VREF_MV) / 1023;
    uint32_t cel = mv / 10;
    printf("ADC: %lu, Vout: %lu V, Temp: %lu C\r\n", val, mv, cel);
    // uint32_t mv = (uint32_t)adc * VREF_MV / 1023U;

    // uint32_t temp_x10 = mv;

    // uint16_t temp_int = temp_x10 / 10U;
    // uint16_t temp_dec = temp_x10 % 10U;

    // printf("ADC: %u, Vout: %u.%03u V, Temp: %u.%u C\r\n",
    //        (unsigned)adc,
    //        (unsigned)(mv / 1000U),
    //        (unsigned)(mv % 1000U),
    //        (unsigned)temp_int,
    //        (unsigned)temp_dec);

    _delay_ms(500);
  }
  return 0;
}