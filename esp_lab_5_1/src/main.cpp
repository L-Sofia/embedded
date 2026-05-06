#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include "SPIFFS.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

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

// ======== BLE UUID ========
#define SERVICE_UUID "12345678-1234-1234-1234-1234567890ab"
#define TEMP_CHAR_UUID "12345678-1234-1234-1234-1234567890ac"
#define PRESS_CHAR_UUID "12345678-1234-1234-1234-1234567890ad"
#define ALT_CHAR_UUID "12345678-1234-1234-1234-1234567890ae"
#define LUX_CHAR_UUID "12345678-1234-1234-1234-1234567890af"

// ======== BLE характеристики ========
BLECharacteristic *tempChar;
BLECharacteristic *pressChar;
BLECharacteristic *altChar;
BLECharacteristic *luxChar;

// ======== Функція для запису даних у SPIFFS ========
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

// ======== Функція для читання останніх даних з файлу ========
bool readLastData(float &temp, float &pressure, float &altitude, int &lux)
{
  if (!SPIFFS.exists("/data.csv"))
    return false;

  File file = SPIFFS.open("/data.csv", FILE_READ);
  if (!file)
    return false;

  String lastLine;
  while (file.available())
  {
    String line = file.readStringUntil('\n');
    if (line.length() > 0)
      lastLine = line;
  }
  file.close();

  if (lastLine.length() == 0)
    return false;

  // Розбиваємо CSV
  int index1 = lastLine.indexOf(',');
  int index2 = lastLine.indexOf(',', index1 + 1);
  int index3 = lastLine.indexOf(',', index2 + 1);
  int index4 = lastLine.indexOf(',', index3 + 1);

  temp = lastLine.substring(index1 + 1, index2).toFloat();
  pressure = lastLine.substring(index2 + 1, index3).toFloat();
  altitude = lastLine.substring(index3 + 1, index4).toFloat();
  lux = lastLine.substring(index4 + 1).toInt();

  return true;
}

void setup()
{
  Serial.begin(115200);

  // ======== SPIFFS ========
  if (!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS mount failed");
    return;
  }

  // ======== DS18B20 ========
  sensors.begin();

  // ======== BMP280 ========
  if (!bmp.begin(0x76))
  {
    Serial.println("BMP280 not found!");
    while (1)
      ;
  }

  // ======== CSV заголовок ========
  if (!SPIFFS.exists("/data.csv"))
  {
    File file = SPIFFS.open("/data.csv", FILE_WRITE);
    file.println("timestamp_ms,temperature_C,pressure_Pa,altitude_m,lux");
    file.close();
  }

  // ======== BLE сервер ========
  BLEDevice::init("ESP32_Sensor");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  tempChar = pService->createCharacteristic(TEMP_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  tempChar->addDescriptor(new BLE2902());

  pressChar = pService->createCharacteristic(PRESS_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pressChar->addDescriptor(new BLE2902());

  altChar = pService->createCharacteristic(ALT_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  altChar->addDescriptor(new BLE2902());

  luxChar = pService->createCharacteristic(LUX_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  luxChar->addDescriptor(new BLE2902());

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->start();
  Serial.println("BLE service started!");
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
    float pressure = bmp.readPressure();
    float altitude = bmp.readAltitude(1013.25);
    int lux = analogRead(LDR_PIN);

    // ======== Вивід у Serial ========
    Serial.printf("Temp: %.2f C, Press: %.2f Pa, Alt: %.2f m, Lux: %d\n", temperature, pressure, altitude, lux);

    // ======== Запис у SPIFFS ========
    saveData(temperature, pressure, altitude, lux);

    // ======== Оновлення BLE характеристик (як рядки) ========
    tempChar->setValue(String(temperature, 2).c_str()); // 2 знаки після коми
    tempChar->notify();

    pressChar->setValue(String(pressure, 2).c_str());
    pressChar->notify();

    altChar->setValue(String(altitude, 2).c_str());
    altChar->notify();

    luxChar->setValue(String(lux).c_str());
    luxChar->notify();
  }
}