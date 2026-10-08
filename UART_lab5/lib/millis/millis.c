#include "millis.h"

volatile uint32_t timer0_millis = 0;
static uint8_t timer0_fract = 0;

ISR(TIMER0_OVF_vect)
{
    uint32_t m = timer0_millis;
    uint8_t f = timer0_fract;

    m += MILLIS_INC;
    f += FRACT_INC;
    if (f >= FRACT_MAX)
    {
        f -= FRACT_MAX;
        m += 1;
    }

    timer0_fract = f;
    timer0_millis = m;
}

uint32_t millis(void)
{
    uint32_t m;
    uint8_t oldSREG = SREG;

    cli();
    m = timer0_millis;
    SREG = oldSREG;

    return m;
}

void InitTimer0(void)
{
    TCCR0A = (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS01) | (1 << CS00);
    TIMSK0 = (1 << TOIE0);
    sei();
}
