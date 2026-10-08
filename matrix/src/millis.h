#ifndef MILLIS_H
#define MILLIS_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define MICSEC_T0_OVF ((64UL * 256UL) / (F_CPU / 1000000L))
#define MILLIS_INC (MICSEC_T0_OVF / 1000)
#define FRACT_INC ((MICSEC_T0_OVF % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

void InitTimer0(void);
uint32_t millis(void);

#endif