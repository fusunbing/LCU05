

#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__

#ifdef __cplusplus
    extern "C" {
#endif


#include "stm32f4xx.h"
#include "rtthread.h"

        
void System_HW_Can_Init(void);
void CAN1_RX0_IRQ(void);
void CAN2_RX0_IRQ(void);

        
#ifdef __cplusplus
}
#endif

#endif

