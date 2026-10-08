/*
    1. Реалізувати зовнішнє переривання INTx кнопкою. Спробувати різні режими EICRA.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#define BTN PD2
volatile uint8_t on = 0;

ISR(INT0_vect)
{
    on = !on;
}

int main(void)
{
    DDRD &= ~(1 << BTN);
    PORTD |= (1 << BTN);
    //------------------
    DDRD |= (1 << PD7) | (1 << PD0) | (1 << PD1);
    //------------------
    // EICRA |= (1 << ISC01);
    EICRA &= ~(1 << ISC00);

    // EICRA |= (1 << ISC00);
    EICRA &= ~(1 << ISC01);

    EIFR = (1 << INTF0) | (1 << INTF1);
    EIMSK |= (1 << INT0);
    //------------------
    sei();

    while (1)
    {
        if (on)
        {
            PORTD |= (1 << PD7);
        }
        else
        {
            PORTD &= ~(1 << PD7);
        }
    }
    return 0;
}