#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>

#define BTN 18
#define LED_TIMER 2
#define LED_BTN 4

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_BMP280 bmp;

hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile bool readBMP = false;
volatile uint32_t pressStart = 0;
volatile uint32_t pressDuration = 0;
volatile bool buttonReleased = false;

bool ledBtnActive = false;
uint32_t ledBtnStart = 0;
uint32_t ledBtnDuration = 0;

void IRAM_ATTR onTimer()
{
    portENTER_CRITICAL_ISR(&timerMux);
    readBMP = true;
    portEXIT_CRITICAL_ISR(&timerMux);
}

void IRAM_ATTR handleButton()
{
    if (digitalRead(BTN) == LOW)
    {
        pressStart = millis();
    }
    else
    {
        pressDuration = millis() - pressStart;
        buttonReleased = true;
    }
}

void setup()
{
    Serial.begin(115200);

    pinMode(LED_TIMER, OUTPUT);
    pinMode(LED_BTN, OUTPUT);
    pinMode(BTN, INPUT_PULLUP);

    Wire.begin(SDA_PIN, SCL_PIN);

    if (!bmp.begin(0x76))
    {
        Serial.println("BMP280 not found!");
        while (1)
            ;
    }

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 2000000, true);
    timerAlarmEnable(timer);

    attachInterrupt(BTN, handleButton, CHANGE);
}

void loop()
{
    if (readBMP)
    {
        portENTER_CRITICAL(&timerMux);
        readBMP = false;
        portEXIT_CRITICAL(&timerMux);

        float temp = bmp.readTemperature();
        float pressure = bmp.readPressure() / 100.0F;

        Serial.printf("Temp: %.2f C | Pressure: %.2f hPa\n", temp, pressure);

        digitalWrite(LED_TIMER, HIGH);
        delay(100);
        digitalWrite(LED_TIMER, LOW);
    }

    if (buttonReleased)
    {
        buttonReleased = false;

        Serial.printf("Button press duration: %lu ms\n", pressDuration);

        ledBtnActive = true;
        ledBtnStart = millis();
        ledBtnDuration = pressDuration;

        digitalWrite(LED_BTN, HIGH);
    }

    if (ledBtnActive)
    {
        if (millis() - ledBtnStart >= ledBtnDuration)
        {
            digitalWrite(LED_BTN, LOW);
            ledBtnActive = false;
        }
    }
}