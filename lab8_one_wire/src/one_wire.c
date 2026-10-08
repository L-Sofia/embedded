#include "one_wire.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

static void ow_low(void)
{
    ONEWIRE_PORT &= ~(1 << ONEWIRE_PIN_NUM);
    ONEWIRE_DDR |= (1 << ONEWIRE_PIN_NUM);
}

static void ow_release(void)
{
    ONEWIRE_DDR &= ~(1 << ONEWIRE_PIN_NUM);
    ONEWIRE_PORT |= (1 << ONEWIRE_PIN_NUM);
}

static uint8_t ow_read_pin(void)
{
    return (ONEWIRE_PIN & (1 << ONEWIRE_PIN_NUM)) ? 1 : 0;
}

void ow_init(void)
{
    ONEWIRE_PORT |= (1 << ONEWIRE_PIN_NUM);
    ONEWIRE_DDR &= ~(1 << ONEWIRE_PIN_NUM);
}

uint8_t ow_reset_pulse(void)
{
    uint8_t presence = 0;
    uint8_t sreg = SREG;
    cli();

    ow_low();
    _delay_us(480);
    ow_release();
    _delay_us(70);

    if (ow_read_pin() == 0)
    {
        presence = 1;
    }
    else
    {
        presence = 0;
    }

    _delay_us(410);
    SREG = sreg;
    return presence;
}

void ow_write_bit(uint8_t bit)
{
    uint8_t sreg = SREG;
    cli();

    if (bit)
    {
        ow_low();
        _delay_us(6);
        ow_release();
        _delay_us(64);
    }
    else
    {
        ow_low();
        _delay_us(60);
        ow_release();
        _delay_us(10);
    }

    SREG = sreg;
}

uint8_t ow_read_bit(void)
{
    uint8_t bit;
    uint8_t sreg = SREG;
    cli();

    ow_low();
    _delay_us(6);
    ow_release();
    _delay_us(9);
    bit = ow_read_pin();
    _delay_us(55);

    SREG = sreg;
    return bit ? 1 : 0;
}

void ow_write_byte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; ++i)
    {
        ow_write_bit((byte >> i) & 0x01); // lsb first
    }
}

uint8_t ow_read_byte(void)
{
    uint8_t res = 0;
    for (uint8_t i = 0; i < 8; ++i)
    {
        if (ow_read_bit())
        {
            res |= (1 << i);
        }
    }
    return res;
}

void ow_write_bytes(const uint8_t *data, uint16_t len)
{
    while (len--)
    {
        ow_write_byte(*data++);
    }
}

void ow_read_bytes(uint8_t *buf, uint16_t len)
{
    while (len--)
    {
        *buf++ = ow_read_byte();
    }
}

//-----------------------SEARCH ROM
static uint8_t ROM_NO[8];
static uint8_t LastDiscrepancy;
static uint8_t LastFamilyDiscrepancy;
static uint8_t LastDeviceFlag;

uint8_t ow_crc8(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0;
    uint8_t i, j;
    for (i = 0; i < len; i++)
    {
        uint8_t inbyte = data[i];
        for (j = 0; j < 8; j++)
        {
            uint8_t mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix)
                crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}

void ow_reset_search(void)
{
    LastDiscrepancy = 0;
    LastFamilyDiscrepancy = 0;
    LastDeviceFlag = 0;
    memset(ROM_NO, 0, sizeof(ROM_NO));
}

uint8_t ow_search(uint8_t *newAddr)
{
    uint8_t id_bit_number = 1;
    uint8_t last_zero = 0;
    uint8_t rom_byte_number = 0;
    uint8_t rom_byte_mask = 1;
    uint8_t search_direction;
    uint8_t id_bit, cmp_id_bit;
    uint8_t i;

    if (LastDeviceFlag)
    {
        return 0;
    }

    if (!ow_reset_pulse())
    {
        LastDiscrepancy = 0;
        LastDeviceFlag = 0;
        LastFamilyDiscrepancy = 0;
        return 0;
    }

    ow_write_byte(0xF0);

    while (rom_byte_number < 8)
    {
        id_bit = ow_read_bit();
        cmp_id_bit = ow_read_bit();

        if ((id_bit == 1) && (cmp_id_bit == 1))
        {
            return 0;
        }
        else
        {
            if (id_bit != cmp_id_bit)
            {
                search_direction = id_bit;
            }
            else
            {
                if (id_bit_number < LastDiscrepancy)
                {
                    search_direction = (ROM_NO[rom_byte_number] & rom_byte_mask) ? 1 : 0;
                }
                else
                {
                    search_direction = (id_bit_number == LastDiscrepancy) ? 1 : 0;
                }

                if (search_direction == 0)
                {
                    last_zero = id_bit_number;
                    if (last_zero <= 8)
                    {
                        LastFamilyDiscrepancy = last_zero;
                    }
                }
            }

            if (search_direction == 1)
            {
                ROM_NO[rom_byte_number] |= rom_byte_mask;
            }
            else
            {
                ROM_NO[rom_byte_number] &= ~rom_byte_mask;
            }

            ow_write_bit(search_direction);

            id_bit_number++;
            rom_byte_mask <<= 1;
            if (rom_byte_mask == 0)
            {
                rom_byte_number++;
                rom_byte_mask = 1;
            }
        }
    }

    LastDiscrepancy = last_zero;

    if (LastDiscrepancy == 0)
    {
        LastDeviceFlag = 1;
    }

    if (ow_crc8(ROM_NO, 8) != 0)
    {
        return 0;
    }

    for (i = 0; i < 8; i++)
    {
        newAddr[i] = ROM_NO[i];
    }

    return 1;
}