#include <Arduino.h>

#define ADC_PIN 32
#define LED_PIN 25

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  uint16_t value = analogRead(ADC_PIN);

  int dac_val = map(value, 0, 4095, 255, 0);

  dacWrite(LED_PIN, dac_val);

  Serial.print("ADC value: ");
  Serial.print(value);
  Serial.print(" brightness: ");
  Serial.println(dac_val);

  delay(500);
}