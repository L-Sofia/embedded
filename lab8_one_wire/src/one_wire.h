#ifndef ONE_WIRE_H
#define ONE_WIRE_H

#include <stdint.h>

#include <avr/io.h>
#define ONEWIRE_PORT PORTD
#define ONEWIRE_DDR DDRD
#define ONEWIRE_PIN PIND
#define ONEWIRE_PIN_NUM PD2

void ow_init(void);

uint8_t ow_reset_pulse(void);

void ow_write_bit(uint8_t bit);
uint8_t ow_read_bit(void);

void ow_write_byte(uint8_t byte);
uint8_t ow_read_byte(void);

void ow_write_bytes(const uint8_t *data, uint16_t len);
void ow_read_bytes(uint8_t *buf, uint16_t len);

void ow_reset_search(void);
uint8_t ow_search(uint8_t *newAddr);
uint8_t ow_crc8(const uint8_t *data, uint8_t len);

#endif
