#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include "SPIFFS.h"

// ======== Налаштування пінів ========
#define ONE_WIRE_BUS 32 // DS18B20
#define LDR_PIN 34      // Фоторезистор (аналоговий)

// ======== Ініціалізація сенсорів ========
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
Adafruit_BMP280 bmp; // I2C: SDA=21, SCL=22

// ======== Таймер ========
unsigned long previousMillis = 0;
const long interval = 5000; // 5 секунд

// ======== Функція для запису даних ========
void saveData(float temp, float pressure, float altitude, int lux)
{
  File file = SPIFFS.open("/data.csv", FILE_APPEND);
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }
  String dataString = String(millis()) + "," + String(temp, 2) + "," + String(pressure, 2) + "," + String(altitude, 2) + "," + String(lux) + "\n";
  file.print(dataString);
  file.close();
}

void readData()
{
  // Перевіряємо, чи файл існує
  if (!SPIFFS.exists("/data.csv"))
  {
    Serial.println("Файл не знайдено");
    return;
  }

  // Відкриваємо файл для читання
  File file = SPIFFS.open("/data.csv", FILE_READ);
  if (!file)
  {
    Serial.println("Не вдалося відкрити файл для читання");
    return;
  }

  Serial.println("=== Вміст файлу data.csv ===");

  // Читаємо файл рядок за рядком
  while (file.available())
  {
    String line = file.readStringUntil('\n'); // зчитуємо до кінця рядка
    Serial.println(line);                     // виводимо у Serial
  }

  file.close();
  Serial.println("=== Кінець файлу ===");
}

void setup()
{
  Serial.begin(115200);

  // ======== SPIFFS ========
  if (!SPIFFS.begin(true))
  {
    Serial.println("An error has occurred while mounting SPIFFS");
    return;
  }
  Serial.println("SPIFFS mounted successfully");

  // ======== DS18B20 ========
  sensors.begin();

  // ======== BMP280 ========
  if (!bmp.begin(0x76))
  { // Адрес BMP280: 0x76 або 0x77
    Serial.println("Could not find BMP280 sensor!");
    while (1)
      ;
  }

  // ======== CSV заголовок (якщо файл пустий) ========
  if (!SPIFFS.exists("/data.csv"))
  {
    File file = SPIFFS.open("/data.csv", FILE_WRITE);
    file.println("timestamp_ms,temperature_C,pressure_Pa,altitude_m,lux");
    file.close();
  }
}

void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    // ======== Збір даних ========
    sensors.requestTemperatures();
    float temperature = sensors.getTempCByIndex(0);

    float pressure = bmp.readPressure();        // Па
    float altitude = bmp.readAltitude(1013.25); // м (за стандартним атмосферним тиском)

    int lux = analogRead(LDR_PIN); // 0-4095 (ESP32 ADC 12 біт)

    // ======== Вивід у Serial ========
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" C, ");
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.print(" Pa, ");
    Serial.print("Altitude: ");
    Serial.print(altitude);
    Serial.print(" m, ");
    Serial.print("Light: ");
    Serial.println(lux);

    // ======== Запис у SPIFFS ========
    saveData(temperature, pressure, altitude, lux);
  }
  if (Serial.available())
  {
    char c = Serial.read();
    if (c == 'r') // якщо ввели 'r'
      readData();
  }
}