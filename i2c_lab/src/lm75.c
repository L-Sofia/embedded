#include "lm75.h"
#include "i2c.h"
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"

uint8_t lm75_init(void)
{
    I2C_Start();
    if (I2C_Write(LM75_ADDR | I2C_WRITE) == I2C_NAK)
    {
        I2C_Stop();
        return 1;
    }

    I2C_Write(0x01);
    I2C_Write(0);
    I2C_Stop();
    return 0;
}

//------------------------------------------test
int16_t lm75_get_temp_x10(void)
{
    uint8_t res_raw[2];

    I2C_Start();
    if (I2C_Write(LM75_ADDR | I2C_WRITE) == I2C_NAK)
    {
        I2C_Stop();
        return 0xFFFF;
    }
    I2C_Write(0x00);
    I2C_Stop();

    I2C_Start();
    if (I2C_Write(LM75_ADDR | I2C_READ) == I2C_NAK)
    {
        I2C_Stop();
        return 0xFFFF;
    }

    res_raw[0] = I2C_Read(I2C_ACK); // MSB
    res_raw[1] = I2C_Read(I2C_NAK); // LSB
    I2C_Stop();

    // int16_t a = (int8_t)res_raw[0] / 100;
    // int16_t b = (int8_t)res_raw[1] % 100;

    int16_t temp_x10 = ((int8_t)res_raw[0]) * 10;

    if (res_raw[1] & 0x80)
        temp_x10 += 5;

    return temp_x10;
}

void lm75_get_temp(int16_t *a, int16_t *b)
{
    uint8_t msb, lsb;

    I2C_Start();
    if (I2C_Write(LM75_ADDR | I2C_WRITE) == I2C_NAK)
    {
        I2C_Stop();
        *a = 0;
        *b = 0;
        return;
    }
    I2C_Write(0x00);
    I2C_Stop();

    I2C_Start();
    if (I2C_Write(LM75_ADDR | I2C_READ) == I2C_NAK)
    {
        I2C_Stop();
        *a = 0;
        *b = 0;
        return;
    }

    msb = I2C_Read(I2C_ACK);
    lsb = I2C_Read(I2C_NAK);
    I2C_Stop();

    int16_t t100 = ((int8_t)msb) * 100;
    if (lsb & 0x80)
        t100 += 50;

    *a = t100 / 100;
    *b = abs(t100 % 100);

    LOG_DEBUG("LM75", "(Hi:Lo) 0x%X 0x%X", msb, lsb);
}

//-----------------------------------------------------

// int16_t lm75_get_temp_x10(void)
// {
//     uint8_t res_raw[2];

//     I2C_Start();
//     if (I2C_Write(LM75_ADDR | I2C_WRITE) == I2C_NAK)
//     {
//         I2C_Stop();
//         return 0xFFFF;
//     }
//     I2C_Write(0);
//     I2C_Stop();

//     I2C_Start();
//     if (I2C_Write(LM75_ADDR | I2C_READ) == I2C_NAK)
//     {
//         I2C_Stop();
//         return 0xFFFF;
//     }
//     res_raw[0] = I2C_Read(I2C_ACK); // msb
//     res_raw[1] = I2C_Read(I2C_NAK); // >> 7; // kcb
//     I2C_Stop();

//     int16_t temp_x10 = res_raw[0] * 10;
//     //---------------------------------------------------------------
//     // float tt = (res_raw[0] + (res_raw[1] >> 7) * 0.5);

//     // return (int16_t)(tt * 10);

//     if (res_raw[1] & 0x80)
//         temp_x10 += 5;

//     printf("RAW: %02X %02X\r\n", res_raw[0], res_raw[1]);

//     printf("T = %d.%d\r\n", temp_x10 / 10, abs(temp_x10 % 10));
//     return temp_x10;
// }

// int16_t lm75_get_temp_x10(void)
// {
//     uint8_t msb, lsb;

//     // Вибираємо регістр температури
//     I2C_Start();
//     if (I2C_Write(LM75_ADDR | I2C_WRITE) == I2C_NAK)
//     {
//         I2C_Stop();
//         return 0xFFFF;
//     }
//     I2C_Write(0x00); // Temperature register
//     I2C_Stop();

//     // Читаємо два байти
//     I2C_Start();
//     if (I2C_Write(LM75_ADDR | I2C_READ) == I2C_NAK)
//     {
//         I2C_Stop();
//         return 0xFFFF;
//     }

//     msb = I2C_Read(I2C_ACK);
//     lsb = I2C_Read(I2C_NAK);
//     I2C_Stop();

//     // Обробка: цілі градуси *10 + 0.5°C якщо встановлено біт7
//     int16_t temp_x10 = (int8_t)msb * 10;

//     if (lsb & 0x80)
//         temp_x10 += 5;

//     return temp_x10; // повертає 225, 223 і т.д.
// }
