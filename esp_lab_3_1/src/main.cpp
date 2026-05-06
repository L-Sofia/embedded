#include <Arduino.h>

#define BTN1 18
#define BTN2 19
#define LED 2
#define LDR 32

volatile uint32_t last_ml1 = 0;
volatile uint32_t last_ml2 = 0;
bool led_on = false;

struct Button
{
  const uint8_t pin;
  uint32_t key_press_count;
  bool pressed;
};

Button btn1 = {BTN1, 0, false};
Button btn2 = {BTN2, 0, false};

void IRAM_ATTR isr_btn1()
{
  uint32_t now_ml = millis();

  if (now_ml - last_ml1 > 200)
  {
    btn1.key_press_count++;
    btn1.pressed = true;
    last_ml1 = now_ml;
  }
}

void IRAM_ATTR isr_btn2()
{
  uint32_t now_ml = millis();

  if (now_ml - last_ml2 > 200)
  {
    btn2.key_press_count++;
    btn2.pressed = true;
    last_ml2 = now_ml;
  }
}

uint8_t level = 0;
const uint8_t led_val[3] = {0, 100, 240};
const int channel = 0;
const int freq = 5000;
const int res = 8;

void setup()
{
  Serial.begin(115200);

  pinMode(btn1.pin, INPUT_PULLUP);
  pinMode(btn2.pin, INPUT_PULLUP);

  attachInterrupt(btn1.pin, isr_btn1, RISING);
  attachInterrupt(btn2.pin, isr_btn2, RISING);

  ledcSetup(channel, freq, res);
  ledcAttachPin(LED, channel);
}

void loop()
{
  if (btn1.pressed)
  {
    uint16_t val = analogRead(LDR);

    Serial.printf("BTN1 pressed: %u time(s)\n", btn1.key_press_count);
    Serial.printf("LDR value: %d\n", val);

    btn1.pressed = false;
  }

  if (btn2.pressed)
  {
    level = (level + 1) % 3;
    uint8_t brightness = led_val[level];
    ledcWrite(channel, brightness);

    Serial.printf("BTN2 pressed: %u time(s)\n", btn2.key_press_count);
    Serial.printf("Brightness: %u\n", brightness);

    btn2.pressed = false;
  }
}