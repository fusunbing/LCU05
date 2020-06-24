

#include "Bsp_can.h"
#include "Bsp_can_QueApi.h"


// 接收CAN中断报文
static CanRxMsg can1_RxMsg;
static CanRxMsg can2_RxMsg;


static void Can_GPIO_Configuration(void);
static void Can_Configuration(void);
static void Can_Filter_Configuration(void);
static void Can_NVIC_Configuration(void);


static void Can_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};

	// 开启GPIO时钟，CAN1时钟和CAN2时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

	// 配置CAN1收发GPIO管脚
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1); 
    
	// 配置CAN2收发GPIO管脚
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2); 
}


static void Can_Configuration(void)
{
    CAN_InitTypeDef CAN_InitStructure = {0};

    // 反初始化CAN1,CAN2
    CAN_DeInit(CAN1);
    CAN_DeInit(CAN2);
    
    // 置缺省值至相关寄存器
    CAN_StructInit(&CAN_InitStructure);
    
    // 初始化CAN1,CAN2, 置相关寄存器，
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = ENABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    
    // CAN Baudrate = 1Mbps (CAN clocked at 42 MHz) 
    // 42M/(7*(1+2+3)) = 1Mbps

    //采样点计算方法：(1 + CAN_BS1)/(1 + CAN_BS1 + CAN_BS2), 建议设置为CiA推荐值
    //   采样点  |  波特率
    //    75%    |  >800k
    //    80%    |  >500k
    //    87.5%  |  <=500k
    
    // 配置CAN1控制器的波特率    
    CAN_InitStructure.CAN_SJW  = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1  = CAN_BS1_2tq;
    CAN_InitStructure.CAN_BS2  = CAN_BS2_3tq;
    CAN_InitStructure.CAN_Prescaler = 7;  
    CAN_Init(CAN1, &CAN_InitStructure);
    
    // 配置CAN2控制器的波特率    
    CAN_InitStructure.CAN_SJW  = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1  = CAN_BS1_13tq;
    CAN_InitStructure.CAN_BS2  = CAN_BS2_2tq;
    CAN_InitStructure.CAN_Prescaler = 21;  
    CAN_Init(CAN2, &CAN_InitStructure);	
}


static void Can_Filter_Configuration(void)
{
    CAN_FilterInitTypeDef CAN_FilterInitStructure = {0};

    // CAN的滤波器设定
    CAN_FilterInitStructure.CAN_FilterMode           = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale          = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow      = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation     = ENABLE;

    // CAN1滤波器设定	
    CAN_FilterInitStructure.CAN_FilterNumber         = 0;
    CAN_FilterInitStructure.CAN_FilterIdHigh         = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0000;
    CAN_FilterInit(&CAN_FilterInitStructure);

    // CAN2滤波器设定	
    CAN_FilterInitStructure.CAN_FilterNumber         = 14;
    CAN_FilterInitStructure.CAN_FilterIdHigh         = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0000;
    CAN_FilterInit(&CAN_FilterInitStructure); 
}


static void Can_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure = {0};

	// 配置CAN1中断向量表	
	NVIC_InitStructure.NVIC_IRQChannel                   = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// 配置CAN2中断向量表	
	NVIC_InitStructure.NVIC_IRQChannel                   = CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void CAN1_RX0_IRQHandler(void)
{
	rt_interrupt_enter();
	
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0))
	{
		// 收取CAN1控制器接收报文
        CAN_Receive(CAN1, CAN_FIFO0, &can1_RxMsg);

        // 将接收的报文压入队列
        CanQ_Push_Rx_Msg(1, rt_tick_get(), &can1_RxMsg);        
    }
    
	rt_interrupt_leave();
}


void CAN2_RX0_IRQHandler(void)
{
	rt_interrupt_enter();

    if(CAN_GetITStatus(CAN2, CAN_IT_FMP0))
    {
        // 收取CAN2控制器接收报文
        CAN_Receive(CAN2, CAN_FIFO0, &can2_RxMsg);

        // 将接收的报文压入队列
        CanQ_Push_Rx_Msg(2, rt_tick_get(), &can2_RxMsg);
    }
	
	rt_interrupt_leave();
}


void System_HW_Can_Init(void)
{
    // CAN GPIO管脚配置
    Can_GPIO_Configuration();

    // CAN控制器配置
	Can_Configuration();

	// CAN滤波器配置
	Can_Filter_Configuration();

	// CAN中断向量表配置
	Can_NVIC_Configuration();

	// 打开CAN控制器中断
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
}

