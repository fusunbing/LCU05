
#include "Bsp_led.h"

#define LED_TIME_COUNT (40)

const uint8_t LED_ON[LED_TIME_COUNT] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };  //灯模式，常亮（周期）
const uint8_t LED_OFF[LED_TIME_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //灯模式，常灭（周期）
const uint8_t LED_FLASH_SLOW[LED_TIME_COUNT] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
                                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //灯模式，闪烁（周期1Hz）
const uint8_t LED_FLASH_FAST[LED_TIME_COUNT] = { 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
                                                  1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0 };

const uint8_t* p_led_act;
const uint8_t* p_led_flt;

void System_Led_SetMode(uint32_t lt, Led_Mode lm)
{
    const uint8_t *p;

    switch(lm)
    {
        case MODE_ON:
            p = LED_ON;
            break;
        case MODE_OFF:
            p = LED_OFF;
            break;
        case MODE_FLASH_SLOW:
            p = LED_FLASH_SLOW;
            break;
        case MODE_FLASH_FAST:
            p = LED_FLASH_FAST;
            break;
        default:
            p = LED_OFF;
            break;
    }

    switch(lt)
    {
        case 0:
            p_led_act = p;
            break;
        case 1:
            p_led_flt = p;
            break;
        default:
            break;
    }
}


void System_Led_Logic(void)
{
    static uint32_t cnt = 0;

    SetPin(12, p_led_act[cnt]);
    SetPin(13, p_led_flt[cnt]);

    cnt++;
    cnt = cnt % LED_TIME_COUNT;
}



