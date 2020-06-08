

#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#ifdef __cplusplus
    extern "C" {
#endif

        
#include <rtthread.h>
#include "stm32f4xx.h"


#define LED_ACT_PT      (GPIOB)
#define LED_ACT_PIN     (GPIO_Pin_15)

#define LED_FLT_PT      (GPIOB)
#define LED_FLT_PIN     (GPIO_Pin_14)

        
typedef enum
{
    LED_ACT = (uint8_t)1,
    LED_FLT,
}Led_Type;


typedef enum
{
    MODE_OFF = (uint8_t)1,    
    MODE_ON,
    MODE_FLASH_SLOW,
    MODE_FLASH_FAST,

}Led_Mode;


void System_Led_Init(void);
void System_Led_Logic(void);
void System_Led_SetMode(Led_Type lt, Led_Mode lm);


#ifdef __cplusplus
}
#endif

#endif

