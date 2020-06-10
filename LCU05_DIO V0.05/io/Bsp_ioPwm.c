

#include "Bsp_ioPwm.h"
#include "stm32f4xx.h"


#define PWM_PULSE       (18)        //占空比18%


void Bsp_ioPwm_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); //初始化时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;  //配置GPIO口
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOE,&GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);

    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //配置基本定时器
    TIM_TimeBaseInitStruct.TIM_Period = (336 - 1);  //计算出计数周期（决定输出频率）
    TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);  //配置ug位

    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;   
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;  //输出通道使能
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;   //选择极性 高电平有效
    TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OCInitStruct.TIM_Pulse = 336 * PWM_PULSE / 100;  //计算出脉宽值（决定PWM占空比）
    TIM_OC3Init(TIM1, &TIM_OCInitStruct);    //配置通道

    //打开影子寄存器
    TIM_ARRPreloadConfig(TIM1, ENABLE);//打开自动重装载寄存器的影子寄存器
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

