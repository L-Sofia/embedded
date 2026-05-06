#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 32
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const int readingsCount = 100;
const int intervalMs = 300;

float readings[readingsCount];

float medianFilter(float arr[], int size)
{
  float sorted[size];
  for (int i = 0; i < size; i++)
    sorted[i] = arr[i];

  for (int i = 0; i < size - 1; i++) // bubble sort
  {
    for (int j = 0; j < size - i - 1; j++)
    {
      if (sorted[j] > sorted[j + 1])
      {
        float temp = sorted[j];
        sorted[j] = sorted[j + 1];
        sorted[j + 1] = temp;
      }
    }
  }

  // кількість елементів непарна
  if (size % 2 == 1)
    return sorted[size / 2];
  // парна
  else
    return (sorted[size / 2 - 1] + sorted[size / 2]) / 2.0;
}

void setup()
{
  Serial.begin(115200);
  sensors.begin();
  Serial.println("DS18B20 стартує...");
}

void loop()
{
  for (int i = 0; i < readingsCount; i++) // зчитування даних
  {
    sensors.requestTemperatures();
    readings[i] = sensors.getTempCByIndex(0);
    Serial.print("Читання ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(readings[i]);
    delay(intervalMs);
  }

  const int windowSize = 5;      // розмір вікна для медіани
  float filtered[readingsCount]; // застосування медіанного фільтру

  for (int i = 0; i < readingsCount; i++)
  {
    int start = max(0, i - windowSize / 2); // Створюємо масив для вікна
    int end = min(readingsCount - 1, i + windowSize / 2);
    int wSize = end - start + 1;
    float window[wSize];
    for (int j = 0; j < wSize; j++)
      window[j] = readings[start + j];

    filtered[i] = medianFilter(window, wSize);
  }

  Serial.println("Відфільтровані значення:"); // Вивід відфільтрованих значень
  for (int i = 0; i < readingsCount; i++)
  {
    Serial.print("Фільтр ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(filtered[i]);
  }

  while (1)
    ;
}