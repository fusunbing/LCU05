
#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__


#include "stm32f4xx.h"


enum PIN_NAME
{ 
    SLOT_ID1 = 0,
    SLOT_ID2,
    SLOT_ID3,
    SLOT_ID4,
    SLOT_ID5,
    SLOT_ID6,
    BOARD_TYPE_ID1,
    BOARD_TYPE_ID2,
    BOARD_TYPE_ID3,
	BOARD_TYPE_ID4,
    PIN_IN1,
    PIN_IN2,
    PIN_IN3,
    PIN_IN4,
    PIN_IN5,
    PIN_IN6,
    PIN_IN7,
    PIN_IN8,
    PIN_IN9,
    PIN_IN10,
    PIN_IN11,
    PIN_IN12,
    PIN_OU1_FB1,
    PIN_OU1_FB2,
    PIN_OU2_FB1,
    PIN_OU2_FB2,
    PIN_OU3_FB1,
    PIN_OU3_FB2,
    PIN_OU4_FB1,
    PIN_OU4_FB2,
    PIN_OU5_FB1,
    PIN_OU5_FB2,
    PIN_OU6_FB1,
    PIN_OU6_FB2,
    PIN_SW1_FB1,
    PIN_SW1_FB2,
    PIN_SW1_FB3,
    PIN_OU1,
    PIN_OU2,
    PIN_OU3,
    PIN_OU4,
    PIN_OU5,
    PIN_OU6,
    PIN_SW1,
};


void stmPinsInit(void);
void SetPin(uint32_t index, uint32_t val);
uint8_t GetPin(uint32_t index);


#endif


