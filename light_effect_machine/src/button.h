#ifndef BUTTON_H
#define BUTTON_H
#include <stdint.h>
#define BTN PB0
#define HOLD_TIME_MS 2000
#define SCAN_PERIOD_MS 5
#define DOUBLE_CLICK_MS 400

typedef enum
{
    BUTTON_NONE = 0,
    BUTTON_SHORT,
    BUTTON_DOUBLE,
    BUTTON_LONG
} button_event_t;

typedef void (*button_handler_t)(button_event_t);

void button_init(button_handler_t bhnd);

void button_scan_task(void);

#endif