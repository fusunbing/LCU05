

#include "Bsp_systimer.h"
#include "Bsp_canTask.h"

struct rt_semaphore sem_systask_10ms;
struct rt_semaphore sem_systask_20ms;
struct rt_semaphore sem_systask_1s;

static rt_uint16_t tim_1s_cnt = 0;
static rt_uint16_t tim_20ms_cnt = 0;


static void System_TimerConfiguration(void);
static void RCC_Configuration(void);
static void NVIC_Configuration(void);


void BSP_TIM_IRQHandler(void)
{
	rt_sem_release(&sem_systask_10ms);

    //借用20ms信号量变量， 实际为50ms
	if ((++tim_20ms_cnt % 5) == 0)
	{
		tim_20ms_cnt = 0;
		rt_sem_release(&sem_systask_20ms);
	}

	if ((++tim_1s_cnt % 100) == 0)
	{
		tim_1s_cnt = 0;
		//rt_sem_release(&sem_systask_1s);
	}
}


void BSP_CAN_TIM_IRQHandler(void)
{
    CanTask_Send_Event_Cycle();
}


void BSP_CAN_TIM_CLC(void)
{
	BSP_CAN_TIM->CNT = 0;
}


int System_hw_timerInit(void)
{
//	rt_err_t rtv, rtv1, rtv2, rtv3;

//	rtv  = rt_sem_init(&sem_systask_10ms,  "Sem10ms",   1,  RT_IPC_FLAG_FIFO);
//	rtv1 = rt_sem_init(&sem_systask_1s,    "Sem_1s",    1,  RT_IPC_FLAG_FIFO);
//	rtv2 = rt_sem_init(&sem_systask_20ms,  "Sem20ms",   1,  RT_IPC_FLAG_FIFO);
//  rtv3 = RT_EOK;

  RCC_Configuration();
	NVIC_Configuration();
	System_TimerConfiguration();

//	if ((rtv == RT_EOK) && (rtv1 == RT_EOK) && (rtv2 == RT_EOK) && (rtv3 == RT_EOK))
//	{
//		return 0;
//	} 
//	else
//	{
//		return -1;
//	}

}


static void System_TimerConfiguration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};

//	//---------------------------------------------------------------------------
//	// 初始化定时器TIM
//	//---------------------------------------------------------------------------
//	TIM_DeInit( BSP_TIM );//复位TIM定时器
//	/* ---------------------------------------------------------------
//	TIM Configuration: Output Compare Timing Mode:
//	TIMCLK = 84 MHz, Prescaler = 8400, TIM counter clock = 10 KHz
//	--------------------------------------------------------------- */
//	/* TIM configuration */
//	TIM_TimeBaseStructure.TIM_Period        = 100;
//	TIM_TimeBaseStructure.TIM_Prescaler     = 0;      
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
//	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   
//	TIM_TimeBaseInit(BSP_TIM, &TIM_TimeBaseStructure);

//	/*--------------------------------------------------------------------- */
//	/* Prescaler configuration TIM 的时钟设定为10KHz，计数器的计量单位：200ms*/
//	/*   注意：分频数不能超过65535 （0xFFFF）                               */
//	/*--------------------------------------------------------------------- */
//	TIM_PrescalerConfig(BSP_TIM, (8400 - 1), TIM_PSCReloadMode_Immediate);

//	/*  */
//	TIM_ARRPreloadConfig(BSP_TIM, ENABLE);
//	/* Clear TIM update pending flag[清除TIM 溢出中断标志] */
//	TIM_ClearFlag(BSP_TIM, TIM_FLAG_Update);

//	/* Enable TIM Update interrupt [TIM 溢出中断允许]*/
//	TIM_ITConfig(BSP_TIM, TIM_IT_Update, ENABLE);  

//	/* TIM enable counter [允许tim计数]*/
//	TIM_Cmd(BSP_TIM, ENABLE);


	//---------------------------------------------------------------------------
	// 初始化定时器
	//---------------------------------------------------------------------------
	TIM_DeInit( BSP_CAN_TIM );//复位TIM定时器
	/* ---------------------------------------------------------------
	TIM Configuration: Output Compare Timing Mode:
	TIMCLK = 84 MHz, Prescaler = 8400, TIM counter clock = 10 KHz
	--------------------------------------------------------------- */
	/* TIM configuration */
	TIM_TimeBaseStructure.TIM_Period        = 2000;
	TIM_TimeBaseStructure.TIM_Prescaler     = 0;      
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   
	TIM_TimeBaseInit(BSP_CAN_TIM, &TIM_TimeBaseStructure);

	/*--------------------------------------------------------------------- */
	/* Prescaler configuration TIM 的时钟设定为10KHz，计数器的计量单位：10ms*/
	/*   注意：分频数不能超过65535 （0xFFFF）                               */
	/*--------------------------------------------------------------------- */
	TIM_PrescalerConfig(BSP_CAN_TIM, (8400 - 1), TIM_PSCReloadMode_Immediate);

	/*  */
	TIM_ARRPreloadConfig(BSP_CAN_TIM, ENABLE);
	/* Clear TIM update pending flag[清除TIM 溢出中断标志] */
	TIM_ClearFlag(BSP_CAN_TIM, TIM_FLAG_Update);

	/* Enable TIM Update interrupt [TIM 溢出中断允许]*/
	TIM_ITConfig(BSP_CAN_TIM, TIM_IT_Update, ENABLE);  

	/* TIM enable counter [允许tim计数]*/
	TIM_Cmd(BSP_CAN_TIM, ENABLE);
}


static void RCC_Configuration(void)
{
	/* TIM clock enable [TIM定时器允许]*/
	RCC_APB1PeriphClockCmd(BSP_TIM_RCC, ENABLE); 
    
	/* TIM clock enable [TIM CAN 定时器允许]*/
	RCC_APB1PeriphClockCmd(BSP_CAN_TIM_RCC, ENABLE); 
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

	/* Enable the TIM Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel                   = BSP_CAN_TIM_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

