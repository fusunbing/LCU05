

#ifndef _BSP_TIM_H_
#define _BSP_TIM_H_


#ifdef __cplusplus
    extern "C" {
#endif

        
#include <stm32f4xx.h>
#include <rtthread.h>
        
        
#define  BSP_TIM_RCC           (RCC_APB1Periph_TIM4)
#define  BSP_TIM               (TIM4)
#define  BSP_TIM_IRQ           (TIM4_IRQn)

#define  BSP_CAN_TIM_RCC       (RCC_APB1Periph_TIM3)
#define  BSP_CAN_TIM           (TIM3)
#define  BSP_CAN_TIM_IRQ       (TIM3_IRQn)

        
int  System_hw_timerInit(void);
void BSP_TIM_IRQHandler(void);
void BSP_CAN_TIM_IRQHandler(void);
void BSP_CAN_TIM_CLC(void);

        
#ifdef __cplusplus
}
#endif


#endif

