

#ifndef _BSP_GPIO_H_
#define _BSP_GPIO_H_

#ifdef __cplusplus
    extern "C" {
#endif


#include <rtthread.h>
#include "stm32f4xx.h"


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
};


typedef struct
{
    uint32_t mode;
    uint32_t rcc;
    GPIO_TypeDef *gpio;
    uint32_t pin;
}PIN_INDEX, *pPIN_INDEX;


void stmPinsInit(void);
uint8_t GetPin(uint32_t index);

        
#ifdef __cplusplus
}
#endif

#endif

