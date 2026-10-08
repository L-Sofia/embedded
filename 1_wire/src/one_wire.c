#include "one_wire.h"
#include <avr/io.h>
#include <util/delay.h>

uint8_t OW_Reset(void)
{
    uint8_t err;
    OW_OUT &= ~(1 << OW_PIN);
    OW_DDR |= 1 << OW_PIN;
    _delay_us(480);
    cli();
    OW_DDR &= ~(1 << OW_PIN);
    _delay_us(70);
    err = OW_IN & (1 << OW_PIN); // presence detect
    sei();
    _delay_us(410);
    if ((OW_IN & (1 << OW_PIN)) == 0)
    {
        err = 1;
    }
    return err;
}

uint8_t OW_IO_Bit(uint8_t b)
{
    cli();
    OW_DDR |= 1 << OW_PIN;
    _delay_us(6); // 1
    if (b)
    {
        OW_DDR &= ~(1 << OW_PIN);
    }
    _delay_us(9); // 14
    if ((OW_IN & (1 << OW_PIN)) == 0)
    {
        b = 0;
    }
    _delay_us(55);
    OW_DDR &= ~(1 << OW_PIN);
    sei();
    return b;
}

uint8_t OW_RW_byte(uint8_t b)
{
    uint8_t i, t;
    for (i = 0; i < 8; i++)
    {
        t = OW_IO_Bit(b & 1);
        b >>= 1;
        if (t)
            b |= 0x80;
    }
    return b;
}

uint8_t OW_UpdateCRC(uint8_t crc, uint8_t b)
{
    for (uint8_t p = 0; p < 8; p++)
    {
        crc = ((crc ^ b) & 1) ? (crc >> 1) ^ 0b10001100 : (crc >> 1);
        b >>= 1;
    }
    return crc;
}

uint8_t OW_Get_ROM(uint8_t *id)
{
    uint8_t i, crc = 0;
    if (OW_Reset())
    {
        return 0xFF; // error, device not found
    }
    OW_RW_byte(0x33); // command get ROM
    for (i = 0; i < 7; i++)
    {
        id[i] = OW_RW_byte(0xFF);
        crc = OW_UpdateCRC(crc, id[i]);
    }
    id[7] = OW_RW_byte(0xFF);
    if (id[7] != crc)
        return 0xFE; // error CRC
    return 0;
}