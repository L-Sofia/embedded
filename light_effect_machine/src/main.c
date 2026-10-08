#include <avr/io.h>
#include <util/delay.h>
#include "button.h"
#include "light_effects.h"

uint8_t effect_running = 0;
uint8_t effect_index = 0;

void my_button_hnd(button_event_t button_event)
{
    switch (button_event)
    {
    case BUTTON_SHORT:
        effect_running = !effect_running;
        break;

    case BUTTON_DOUBLE:
        light_effect_speed_task();
        break;

    case BUTTON_LONG:
        if (!effect_running)
        {
            effect_index++;
            if (effect_index >= NUM_EFFECTS)
                effect_index = 0;

            light_effects_reset();
        }
        break;
    default:
        break;
    }
}

int main(void)
{
    DDRD = 0xff;
    PORTD = 0x00;

    button_init(&my_button_hnd);

    for (;;)
    {
        button_scan_task();

        if (effect_running)
            light_effects_set_task(effect_index);
        _delay_ms(5);
    }
    return 0;
}
