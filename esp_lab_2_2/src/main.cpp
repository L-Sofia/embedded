#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_BMP280 bmp1; // 0x76
Adafruit_BMP280 bmp2; // 0x77

void setup()
{
  Serial.begin(9600);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!bmp1.begin(0x76))
  {
    Serial.println("BMP280 1 (0x76)!");
  }

  if (!bmp2.begin(0x77))
  {
    Serial.println("BMP280 2 (0x77)!");
  }
}

void loop()
{
  Serial.println("----- BMP280 1 -----");
  Serial.print("Temp: ");
  Serial.print(bmp1.readTemperature());
  Serial.println(" C");

  Serial.print("Pressure: ");
  Serial.print(bmp1.readPressure());
  Serial.println(" Pa");

  Serial.println();

  Serial.println("----- BMP280 2 -----");
  Serial.print("Temp: ");
  Serial.print(bmp2.readTemperature());
  Serial.println(" C");

  Serial.print("Pressure: ");
  Serial.print(bmp2.readPressure());
  Serial.println(" Pa");

  Serial.println("----------------------");

  delay(5000);
}