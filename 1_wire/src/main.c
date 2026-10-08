#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "uart.h"
#define DS18B20_PIN PD2

void ds18b20WriteBit(uint8_t bit);
void ds18b20WriteByte(uint8_t byte);
uint8_t ds18b20ReadBit();
uint8_t ds18b20ReadByte();
uint8_t ds18b20Reset();
float ds18b20ReadTemperature();
int main(void)
{
    USART_Init(9600);
    _delay_ms(1000);
    while (1)
    {
        if (!ds18b20Reset())
        {
            printf("Error: DS18B20 not detected.\r\n");
            _delay_ms(1000);
            continue;
        }
        float temperature = ds18b20ReadTemperature();
        int temp_int = (int)temperature;
        int temp_frac = (int)((temperature - temp_int) * 100);
        if (temp_frac < 0)
            temp_frac = -temp_frac;

        char buffer[30];
        sprintf(buffer, "Temperature: %d.%02d\r\n", temp_int, temp_frac);
        printf(buffer);
        _delay_ms(1000);
    }
    return 0;
}

void ds18b20WriteBit(uint8_t bit)
{
    DDRD |= (1 << DS18B20_PIN);
    PORTD &= ~(1 << DS18B20_PIN);
    _delay_us(1);
    if (bit)
    {
        DDRD &= ~(1 << DS18B20_PIN);
    }
    _delay_us(60);
    DDRD &= ~(1 << DS18B20_PIN);
}
void ds18b20WriteByte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        ds18b20WriteBit(byte & 1);
        byte >>= 1;
    }
}
uint8_t ds18b20ReadBit()
{
    uint8_t bit = 0;
    DDRD |= (1 << DS18B20_PIN);
    PORTD &= ~(1 << DS18B20_PIN);
    _delay_us(1);
    DDRD &= ~(1 << DS18B20_PIN);
    _delay_us(10);
    if (PIND & (1 << DS18B20_PIN))
    {
        bit = 1;
    }
    _delay_us(50);
    return bit;
}
uint8_t ds18b20ReadByte()
{
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        byte |= (ds18b20ReadBit() << i);
    }
    return byte;
}
uint8_t ds18b20Reset()
{
    DDRD |= (1 << DS18B20_PIN);
    PORTD &= ~(1 << DS18B20_PIN);
    _delay_us(480);
    DDRD &= ~(1 << DS18B20_PIN);
    _delay_us(70);
    uint8_t presence = !(PIND & (1 << DS18B20_PIN));
    _delay_us(410);
    return presence;
}
float ds18b20ReadTemperature()
{
    if (!ds18b20Reset())
    {
        return 0;
    }
    ds18b20WriteByte(0xCC); // Skip ROM command
    ds18b20WriteByte(0x44); // Start temperature conversion
    _delay_ms(750);         // Wait for conversion to complete
    ds18b20Reset();
    ds18b20WriteByte(0xCC); // Skip ROM command
    ds18b20WriteByte(0xBE); // Read Scratchpad command
    uint8_t lsb = ds18b20ReadByte();
    uint8_t msb = ds18b20ReadByte();
    int16_t temperature = (msb << 8) | lsb;
    return temperature * 0.0625;
}
