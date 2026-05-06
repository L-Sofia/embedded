#include "Arduino.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 32
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const int readingsCount = 100; // кількість зчитувань
const int intervalMs = 300;    // інтервал між зчитуваннями (мс)

float readings[readingsCount]; // сирі дані
float filtered[readingsCount]; // відфільтровані дані

// ======== Налаштування Кальмана ========
float Q = 0.01; // процес шуму
float R = 0.1;  // вимірювальний шум
float P = 1, X = 0, K;

void setup()
{
  Serial.begin(115200);
  sensors.begin();
  Serial.println("DS18B20 стартує...");
}

void loop()
{
  // 1️⃣ Зчитування даних
  for (int i = 0; i < readingsCount; i++)
  {
    sensors.requestTemperatures();
    readings[i] = sensors.getTempCByIndex(0);
    Serial.print("Читання ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(readings[i]);
    delay(intervalMs);
  }

  // 2️⃣ Застосування фільтру Кальмана
  X = readings[0]; // початкове значення фільтру
  filtered[0] = X;

  for (int i = 1; i < readingsCount; i++)
  {
    // Оновлення передбачення
    P = P + Q;

    // Обчислення коефіцієнта Кальмана
    K = P / (P + R);

    // Оновлення оцінки
    X = X + K * (readings[i] - X);

    // Оновлення помилки оцінки
    P = (1 - K) * P;

    filtered[i] = X;
  }

  // 3️⃣ Вивід відфільтрованих значень
  Serial.println("Відфільтровані значення (Кальман):");
  for (int i = 0; i < readingsCount; i++)
  {
    Serial.print("Фільтр ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(filtered[i]);
  }

  while (1)
    ; // зупинка після одного циклу
}