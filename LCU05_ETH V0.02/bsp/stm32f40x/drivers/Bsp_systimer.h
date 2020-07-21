

#ifndef _BSP_TIM_H_
#define _BSP_TIM_H_


#ifdef __cplusplus
    extern "C" {
#endif

        
#include <stm32f4xx.h>
#include <rtthread.h>


void Bsp_canTimer_Init(void);
void BSP_CAN_TIM_IRQHandler(void);


#ifdef __cplusplus
}
#endif

#endif

