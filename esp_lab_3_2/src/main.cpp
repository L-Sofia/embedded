#include <Arduino.h>

#define LED 2
#define LDR 32

hw_timer_t *timer1 = NULL;
hw_timer_t *timer2 = NULL;

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile bool readFlag1 = false;
volatile bool readFlag2 = false;

void IRAM_ATTR onTimer1()
{
  portENTER_CRITICAL_ISR(&timerMux);
  readFlag1 = true;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void IRAM_ATTR onTimer2()
{
  portENTER_CRITICAL_ISR(&timerMux);
  readFlag2 = true;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup()
{
  Serial.begin(115200);

  pinMode(LDR, INPUT);

  // ===== TIMER 1 (наприклад 1 сек) =====
  timer1 = timerBegin(0, 80, true); // 80 prescaler → 1 tick = 1 мкс
  timerAttachInterrupt(timer1, &onTimer1, true);
  timerAlarmWrite(timer1, 1000000, true); // 1 000 000 мкс = 1 сек
  timerAlarmEnable(timer1);

  // ===== TIMER 2 (наприклад 3 сек) =====
  timer2 = timerBegin(1, 80, true);
  timerAttachInterrupt(timer2, &onTimer2, true);
  timerAlarmWrite(timer2, 3000000, true); // 3 сек
  timerAlarmEnable(timer2);
}

void loop()
{
  if (readFlag1)
  {
    portENTER_CRITICAL(&timerMux);
    readFlag1 = false;
    portEXIT_CRITICAL(&timerMux);

    uint16_t val = analogRead(LDR);
    Serial.printf("[TIMER 1 - 1s] LDR: %d\n", val);
  }

  if (readFlag2)
  {
    portENTER_CRITICAL(&timerMux);
    readFlag2 = false;
    portEXIT_CRITICAL(&timerMux);

    uint16_t val = analogRead(LDR);
    Serial.printf("[TIMER 2 - 3s] LDR: %d\n", val);
  }
}