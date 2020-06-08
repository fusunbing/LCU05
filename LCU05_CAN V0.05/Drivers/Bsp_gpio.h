

#ifndef _BSP_GPIO_H_
#define _BSP_GPIO_H_

#ifdef __cplusplus
    extern "C" {
#endif
        
        
#include "userApp.h"
        
        
#define PIN_LOW                 0x00
#define PIN_HIGH                0x01

        
enum PIN_MODE
{ 
    PIN_MODE_OUTPUT = 1,
    PIN_MODE_INPUT,
    PIN_MODE_INPUT_PULLUP,
};


typedef struct
{
    uint32_t mode;
    uint32_t rcc;
    GPIO_TypeDef *gpio;
    uint32_t pin;
}PIN_INDEX, *pPIN_INDEX;


void stmPinsInit(void);

        
#ifdef __cplusplus
}
#endif

#endif

