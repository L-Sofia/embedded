#include "ds1307.h"
#include "i2c.h"

static uint8_t bcd_to_dec(uint8_t val)
{
    return ((val >> 4) * 10) + (val & 0x0F);
}

static uint8_t dec_to_bcd(uint8_t val)
{
    return ((val / 10) << 4) | (val % 10);
}

void DS1307_Init(void)
{
    I2C_Start();
    I2C_Write(DS1307_ADDR);
    I2C_Write(0x00);
    I2C_Write(0x00);
    I2C_Stop();
}

void DS1307_GetTime(ds1307_time_t *t)
{
    uint8_t bcd;

    I2C_Start();
    I2C_Write(DS1307_ADDR);
    I2C_Write(0x00);
    I2C_Stop();

    I2C_Start();
    I2C_Write(DS1307_ADDR | 1);

    bcd = I2C_Read(1);
    t->sec = bcd_to_dec(bcd & 0x7F);

    bcd = I2C_Read(1);
    t->min = bcd_to_dec(bcd & 0x7F);

    bcd = I2C_Read(0);
    t->hour = bcd_to_dec(bcd & 0x3F);

    I2C_Stop();
}

void DS1307_SetTime(uint8_t h, uint8_t m, uint8_t s)
{
    I2C_Start();
    I2C_Write(DS1307_ADDR);
    I2C_Write(DS1307_ADDR);
    I2C_Write(dec_to_bcd(s));
    I2C_Write(dec_to_bcd(m));
    I2C_Write(dec_to_bcd(h));
    I2C_Stop();
}
