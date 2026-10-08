#include <avr/io.h>
#include "adc.h"

void ADC_Init(void)
{
    ADMUX = (1 << REFS0) | (1 << REFS1);// | (1 << ADLAR);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t ADC_Read(uint8_t inp)
{
    ADMUX = (ADMUX & 0xF0) | (inp & 0x0F);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
        ;
    return ADC;
}
