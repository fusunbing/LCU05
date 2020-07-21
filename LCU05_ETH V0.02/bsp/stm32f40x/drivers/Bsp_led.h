

#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#ifdef __cplusplus
    extern "C" {
#endif


#include "Bsp_gpio.h"

typedef enum
{
    MODE_OFF = (uint8_t)1,    
    MODE_ON,
    MODE_FLASH_SLOW,
    MODE_FLASH_FAST
}Led_Mode;


void System_Led_Logic(void);
void System_Led_SetMode(uint32_t lt, Led_Mode lm);


#ifdef __cplusplus
}
#endif

#endif

