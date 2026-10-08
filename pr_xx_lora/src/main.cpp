#include <Arduino.h>
#include "sx1276.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("uart is booted");
  sx1276_init();

  uint8_t version = get_version();
  Serial.print("SX1276 version: 0x");
  Serial.println(version, HEX);

  get_op_reg();

  sx1276_lora_mode();
}

void loop()
{
  Serial.println("working every 1 second...");
  delay(1000);
}