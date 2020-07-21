

#include "Bsp_systimer.h"
#include "Bsp_canTask.h"

#define  BSP_CAN_TIM_RCC       (RCC_APB1Periph_TIM3)
#define  BSP_CAN_TIM           (TIM3)
#define  BSP_CAN_TIM_IRQ       (TIM3_IRQn)


void BSP_CAN_TIM_IRQHandler(void)
{
    CanTask_Send_Event_Cycle();
}


static void System_TimerConfiguration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};

    //---------------------------------------------------------------------------
    // ��ʼ����ʱ��
    //---------------------------------------------------------------------------
    TIM_DeInit( BSP_CAN_TIM );//��λTIM��ʱ��
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
    /* Prescaler configuration TIM ��ʱ���趨Ϊ10KHz���������ļ�����λ��10ms*/
    /*   ע�⣺��Ƶ�����ܳ���65535 ��0xFFFF��                               */
    /*--------------------------------------------------------------------- */
    TIM_PrescalerConfig(BSP_CAN_TIM, (8400 - 1), TIM_PSCReloadMode_Immediate);

    TIM_ARRPreloadConfig(BSP_CAN_TIM, ENABLE);
    /* Clear TIM update pending flag[���TIM ����жϱ�־] */
    TIM_ClearFlag(BSP_CAN_TIM, TIM_FLAG_Update);

    /* Enable TIM Update interrupt [TIM ����ж�����]*/
    TIM_ITConfig(BSP_CAN_TIM, TIM_IT_Update, ENABLE);  

    /* TIM enable counter [����tim����]*/
    TIM_Cmd(BSP_CAN_TIM, ENABLE);
}


static void RCC_Configuration(void)
{
    /* TIM clock enable [TIM CAN ��ʱ������]*/
    RCC_APB1PeriphClockCmd(BSP_CAN_TIM_RCC, ENABLE); 
}


static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the TIM Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = BSP_CAN_TIM_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void Bsp_canTimer_Init(void)
{
    RCC_Configuration();
    NVIC_Configuration();
    System_TimerConfiguration();
}

