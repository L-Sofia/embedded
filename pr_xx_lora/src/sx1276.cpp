#include "sx1276.h"
#include "sx1276_regs.h"
#include <Arduino.h>
#include <SPI.h>

void sx1276_init(void)
{
    SPI.begin(SX_CLK, SX_MISO, SX_MOSI, SX_CS);

    pinMode(SX_CS, OUTPUT);
    digitalWrite(SX_CS, HIGH);

    pinMode(SX_RESET, OUTPUT);
    sx1276_enable();
}

void sx1276_enable(void)
{
    digitalWrite(SX_RESET, HIGH);
}

void sx1276_disable(void)
{
    digitalWrite(SX_RESET, LOW);
}

void sx1276_reset(void)
{
    digitalWrite(SX_RESET, LOW);
    delay(10);
    digitalWrite(SX_RESET, HIGH);
    delay(10);
}

void sx1276_write_reg(uint8_t addr, uint8_t val)
{
    digitalWrite(SX_CS, LOW);
    SPI.transfer(addr | 0x80); // старший біт 1 - запис
    SPI.transfer(val);
    digitalWrite(SX_CS, HIGH);
}

uint8_t sx1276_read_reg(uint8_t addr)
{
    digitalWrite(SX_CS, LOW);
    SPI.transfer(addr & 0x7F); // старший біт 0 - читання
    uint8_t reg_val = SPI.transfer(0);
    digitalWrite(SX_CS, HIGH);

    return reg_val;
}

uint8_t get_version(void)
{
    uint8_t ver = sx1276_read_reg(REG_VERSION);
    return ver;
}

void get_op_reg(void)
{
    uint8_t reg_val = sx1276_read_reg(REG_OP_MODE);

    Serial.print("register 0x");
    Serial.print(REG_OP_MODE, HEX);
    Serial.print(" = 0x");
    Serial.println(REG_OP_MODE, HEX);

    Serial.print("Bits: ");

    for (int8_t i = 7; i >= 0; i--)
    {
        Serial.print((REG_OP_MODE >> i) & 1);
    }

    Serial.println();
}

void sx1276_lora_mode(void)
{
    uint8_t reg = sx1276_read_reg(REG_OP_MODE);
    reg |= REG_LORA_MODE;
    sx1276_write_reg(REG_OP_MODE, reg);
}