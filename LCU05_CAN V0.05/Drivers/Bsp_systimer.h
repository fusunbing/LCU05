

#ifndef _BSP_SYSTIMER_H_
#define _BSP_SYSTIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

        
#include <stm32f4xx.h>
#include <rtthread.h>
        

void Bsp_sysTimerInit(void);

void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);

        
#ifdef __cplusplus
}
#endif

#endif

