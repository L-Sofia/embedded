#ifndef __ONE_WIRE_H__
#define __ONE_WIRE_H__

#define OW_PIN PD2
#define OW_IN PINB
#define OW_OUT PORTB
#define OW_DDR DDRB

#include <stdint.h>
#include "millis.h"

uint8_t OW_Reset(void);
uint8_t OW_IO_Bit(uint8_t b);
uint8_t OW_RW_byte(uint8_t b);
uint8_t OW_UpdateCRC(uint8_t crc, uint8_t b);
uint8_t OW_Get_ROM(uint8_t *id);

#endif