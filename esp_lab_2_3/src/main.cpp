#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

// Спільні піни для обох датчиків
#define BMP_SCK 18
#define BMP_MISO 19
#define BMP_MOSI 23

// Окремі піни вибору (CS)
#define BMP_CS_1 5
#define BMP_CS_2 15

// Створюємо два об'єкти з різними CS
Adafruit_BMP280 bmp1(BMP_CS_1, BMP_MOSI, BMP_MISO, BMP_SCK);
Adafruit_BMP280 bmp2(BMP_CS_2, BMP_MOSI, BMP_MISO, BMP_SCK);

void setup()
{
  Serial.begin(9600);

  Serial.println("Ініціалізація датчиків...");

  // Запуск першого датчика
  if (!bmp1.begin())
  {
    Serial.println("Помилка: Датчик №1 не знайдено!");
  }

  // Запуск другого датчика
  if (!bmp2.begin())
  {
    Serial.println("Помилка: Датчик №2 не знайдено!");
  }

  Serial.println("Система готова!");
}

void loop()
{
  // Дані з першого датчика
  Serial.print("Датчик 1: ");
  Serial.print(bmp1.readTemperature());
  Serial.print(" *C | ");
  Serial.print(bmp1.readPressure() * 0.00750062); // в мм рт. ст.
  Serial.println(" mmHg");

  // Дані з другого датчика
  Serial.print("Датчик 2: ");
  Serial.print(bmp2.readTemperature());
  Serial.print(" *C | ");
  Serial.print(bmp2.readPressure() * 0.00750062);
  Serial.println(" mmHg");

  Serial.println("------------------------------------");
  delay(2000);
}