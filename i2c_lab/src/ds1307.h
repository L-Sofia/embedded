#ifndef DS1307_H
#define DS1307_H

#include <stdint.h>

#define DS1307_ADDR 0xD0 // write address

typedef struct
{
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
} ds1307_time_t;

void DS1307_Init(void);
void DS1307_GetTime(ds1307_time_t *t);
void DS1307_SetTime(uint8_t h, uint8_t m, uint8_t s);

#endif
