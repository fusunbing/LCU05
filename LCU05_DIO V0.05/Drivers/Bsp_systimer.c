

#include "Bsp_systimer.h"


#define  BSP_TIM_RCC           (RCC_APB1Periph_TIM4)
#define  BSP_TIM               (TIM4)
#define  BSP_TIM_IRQ           (TIM4_IRQn)


static void TimerConfiguration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};

	//---------------------------------------------------------------------------
	// 初始化定时器TIM
	//---------------------------------------------------------------------------
	TIM_DeInit( BSP_TIM );//复位TIM定时器
	/* ---------------------------------------------------------------
	TIM Configuration: Output Compare Timing Mode:
	TIMCLK = 84 MHz, Prescaler = 8400, TIM counter clock = 10 KHz
	--------------------------------------------------------------- */
	/* TIM configuration */
	TIM_TimeBaseStructure.TIM_Period        = 100;
	TIM_TimeBaseStructure.TIM_Prescaler     = 0;      
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   
	TIM_TimeBaseInit(BSP_TIM, &TIM_TimeBaseStructure);

	/*--------------------------------------------------------------------- */
	/* Prescaler configuration TIM 的时钟设定为10KHz，计数器的计量单位：10ms*/
	/*   注意：分频数不能超过65535 （0xFFFF）                               */
	/*--------------------------------------------------------------------- */
	TIM_PrescalerConfig(BSP_TIM, (8400 - 1), TIM_PSCReloadMode_Immediate);

	/*  */
	TIM_ARRPreloadConfig(BSP_TIM, ENABLE);
	/* Clear TIM update pending flag[清除TIM 溢出中断标志] */
	TIM_ClearFlag(BSP_TIM, TIM_FLAG_Update);

	/* Enable TIM Update interrupt [TIM 溢出中断允许]*/
	TIM_ITConfig(BSP_TIM, TIM_IT_Update, ENABLE);  

	/* TIM enable counter [允许tim计数]*/
	TIM_Cmd(BSP_TIM, ENABLE);
}


static void RCC_Configuration(void)
{
	/* TIM clock enable [TIM定时器允许]*/
	RCC_APB1PeriphClockCmd(BSP_TIM_RCC, ENABLE); 
}


static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the TIM Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel                   = BSP_TIM_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void Bsp_systimer_Init(void)
{
    RCC_Configuration();
	NVIC_Configuration();
	TimerConfiguration();
}

