

#ifndef _BSP_CAN_H_
#define _BSP_CAN_H_

#ifdef __cplusplus
    extern "C" {
#endif


#include "stm32f4xx.h"
#include "rtthread.h"


void System_HW_Can_Init(void);
void CAN1_RX0_IRQHandler(void);
void CAN2_RX0_IRQHandler(void);


#ifdef __cplusplus
}
#endif

#endif

