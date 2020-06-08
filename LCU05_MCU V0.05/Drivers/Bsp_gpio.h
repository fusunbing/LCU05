

#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

#ifdef __cplusplus
    extern "C" {
#endif
        
#include "stm32f4xx.h"
#include <rtthread.h>
        
#define PIN_LOW                 0x00
#define PIN_HIGH                0x01
        
enum PIN_MODE
{ 
    PIN_MODE_OUTPUT = 1,
    PIN_MODE_INPUT,
    PIN_MODE_INPUT_PULLUP,
};


enum PIN_NAME
{ 
    SLOT_ID1 = 0,
    SLOT_ID2,
    SLOT_ID3,
    SLOT_ID4,
    BOX_ID1,
    BOX_ID2,
    BOX_ID3,
    BOX_ID4,
    BOARD_TYPE_ID1,
    BOARD_TYPE_ID2,
    BOARD_TYPE_ID3,
    BOARD_TYPE_ID4,
    DC5V_DETECT,
    DC110V_DETECT
};


typedef struct
{
    uint32_t mode;
    uint32_t rcc;
    GPIO_TypeDef *gpio;
    uint32_t pin;
}PIN_INDEX, *pPIN_INDEX;


void stmPinsInit(void);
void SetPin(uint32_t index, uint8_t val);
uint8_t GetPin(uint32_t index);

        
#ifdef __cplusplus
}
#endif

#endif

