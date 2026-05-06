#include <OneWire.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

// ====== ПІНИ (ЗАМІНИ) ======
#define ONE_WIRE_BUS 15
#define LDR_PIN 32
#define SDA_PIN 21
#define SCL_PIN 22
// ===========================

OneWire ds(ONE_WIRE_BUS);
Adafruit_BMP280 bmp;

// ВСТАВ СЮДИ СВІЙ ROM (якщо 1 датчик)
byte addr[8] = {0x28, 0x61, 0x64, 0x35, 0xF9, 0x4D, 0x31, 0xDC};
float readDS18B20()
{
  byte data[9];

  ds.reset();
  ds.select(addr);
  ds.write(0x44); // start conversion
  delay(750);

  ds.reset();
  ds.select(addr);
  ds.write(0xBE); // read scratchpad

  for (int i = 0; i < 9; i++)
  {
    data[i] = ds.read();
  }

  int16_t raw = (data[1] << 8) | data[0];
  return (float)raw / 16.0;
}

void setup()
{
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!bmp.begin(0x76))
  {
    Serial.println("BMP280 не знайдено!");
    while (1)
      ;
  }
}

void loop()
{

  // ===== DS18B20 =====
  float tempDS = readDS18B20();

  // ===== BMP280 =====
  float tempBMP = bmp.readTemperature();
  float pressure = bmp.readPressure();

  // ===== LDR =====
  int light = analogRead(LDR_PIN);

  // ===== ВИВІД =====
  Serial.println("------");

  Serial.print("DS18B20 Temp: ");
  Serial.print(tempDS);
  Serial.println(" C");

  Serial.print("BMP280 Temp: ");
  Serial.print(tempBMP);
  Serial.println(" C");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" Pa");

  Serial.print("Light: ");
  Serial.println(light);

  delay(2000);
}