#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"
#include "max7219.h"

int main(void)
{
  spi_init();
  max7219_init();
  while (1)
  {
    max7219_heart();
  }
  return 0;
}