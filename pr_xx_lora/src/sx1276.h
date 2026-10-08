#ifndef SX1276_H
#define SX1276_H

#include <Arduino.h>

#define SX_MISO 19
#define SX_MOSI 27
#define SX_CS 18
#define SX_CLK 5
#define SX_RESET 23

void sx1276_init(void);
void sx1276_reset(void);
void sx1276_enable(void);
void sx1276_disable(void);

void sx1276_lora_mode(void);

uint8_t get_version(void);
void get_op_reg(void);

void sx1276_write_reg(uint8_t addr, uint8_t val);
uint8_t sx1276_read_reg(uint8_t addr);

#endif