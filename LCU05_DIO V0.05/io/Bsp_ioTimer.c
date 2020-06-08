

#include "Bsp_ioTimer.h"
#include "stm32f4xx.h"


#define  IO_TIM             (TIM7)
#define  IO_TIM_RCC         (RCC_APB1Periph_TIM7)
#define  IO_TIM_IRQ         (TIM7_IRQn)


static void ioTask_TimerConfiguration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};

	TIM_DeInit(IO_TIM);//复位TIM定时器
	/* ---------------------------------------------------------------
	Modify BSP_SYNC_TIM clk = 100KHz 2014-6-10
	TIM Configuration: Output Compare Timing Mode:
	TIMCLK = 84 MHz, Prescaler = 8400, TIM counter clock = 100 KHz
	--------------------------------------------------------------- */
	/* TIM configuration */
	TIM_TimeBaseStructure.TIM_Period = 200; //200对应2ms
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(IO_TIM, &TIM_TimeBaseStructure);

	/* Prescaler configuration TIM 的时钟设定为100KHz，计数器的计量单位：2ms*/
	/*   注意：分频数不能超过65535 （0xFFFF）                               */
	TIM_PrescalerConfig(IO_TIM, (840 - 1), TIM_PSCReloadMode_Immediate);

	TIM_ARRPreloadConfig(IO_TIM, ENABLE);
	/* Clear TIM update pending flag[清除TIM 溢出中断标志] */
	TIM_ClearFlag(IO_TIM, TIM_FLAG_Update);

	/* Enable TIM Update interrupt [TIM 溢出中断允许]*/
	TIM_ITConfig(IO_TIM, TIM_IT_Update, ENABLE);

	/* TIM enable counter [允许tim计数]*/
	TIM_Cmd(IO_TIM, ENABLE);
}


/* TIM clock enable */
static void ioTask_RCC_Configuration(void)
{
    RCC_APB1PeriphClockCmd(IO_TIM_RCC, ENABLE); 
}


/* Enable the TIM Interrupt */
static void ioTask_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel                   = IO_TIM_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    
	NVIC_Init(&NVIC_InitStructure);
}


//定时器复位，重新计时API
void Bsp_ioSync_TimClear(void)
{
	IO_TIM->CR1 &= (uint16_t)~TIM_CR1_CEN;
	IO_TIM->SR   = (uint16_t)~TIM_FLAG_Update;
	IO_TIM->CNT  = 0;
	IO_TIM->CR1 |= TIM_CR1_CEN;
}


void Bsp_ioTimer_Init(void)
{
    ioTask_RCC_Configuration();
    ioTask_TimerConfiguration();
    ioTask_NVIC_Configuration();
}

