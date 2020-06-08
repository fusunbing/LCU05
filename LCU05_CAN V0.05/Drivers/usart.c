/*
 * File      : usart.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2010-03-29     Bernard      remove interrupt Tx and DMA Rx mode
 * 2012-02-08     aozima       update for F4.
 */


#include "usart.h"
#include "serial.h"
#include "board.h"


typedef enum
{
    use_console,
    use_freemaster,
    use_ptu,
}console_type;

uint8_t is_reconnected = 1;

/*
* Use UART1 as console output and finsh input
* interrupt Rx and poll Tx (stream mode)
*
* Use UART2 with interrupt Rx and poll Tx
* Use UART3 with DMA Tx and interrupt Rx -- DMA channel 2
*
* USART DMA setting on STM32
* USART1 Tx --> DMA Channel 4
* USART1 Rx --> DMA Channel 5
*/

#ifdef RT_USING_UART1
struct stm32_serial_int_rx uart1_int_rx;
struct stm32_serial_dma_tx uart1_dma_tx;
struct stm32_serial_device uart1 =
{
    USART1,
    &uart1_int_rx,
    &uart1_dma_tx
};
struct rt_device uart1_device;
#endif

/************************************************************************/
//                                                                 
/************************************************************************/
/* USART1_REMAP = 0 */
#define UART1_GPIO_TX		GPIO_Pin_9
#define UART1_TX_PIN_SOURCE GPIO_PinSource9
#define UART1_GPIO_RX		GPIO_Pin_10
#define UART1_RX_PIN_SOURCE GPIO_PinSource10
#define UART1_GPIO			GPIOA
#define UART1_GPIO_RCC      RCC_AHB1Periph_GPIOA
#define RCC_APBPeriph_UART1	RCC_APB2Periph_USART1

#define USART1_RX_DMA_CHANNEL            DMA_Channel_4
#define USART1_RX_DMA_STREAM             DMA2_Stream5
#define USART1_RX_DMA_FLAG_FEIF          DMA_FLAG_FEIF5
#define USART1_RX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF5
#define USART1_RX_DMA_FLAG_TEIF          DMA_FLAG_TEIF5
#define USART1_RX_DMA_FLAG_HTIF          DMA_FLAG_HTIF5
#define USART1_RX_DMA_FLAG_TCIF          DMA_FLAG_TCIF5

#define USART1_TX_DMA_STREAM             DMA2_Stream7
#define USART1_TX_DMA_CHANNEL            DMA_Channel_4
#define USART1_TX_DMA_FLAG_FEIF          DMA_FLAG_FEIF7
#define USART1_TX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF7
#define USART1_TX_DMA_FLAG_TEIF          DMA_FLAG_TEIF7
#define USART1_TX_DMA_FLAG_HTIF          DMA_FLAG_HTIF7
#define USART1_TX_DMA_FLAG_TCIF          DMA_FLAG_TCIF7


uint8_t txBuffer[256] = { 0 };
uint8_t serial1_rx_buffer[512] = { 0 };


static void serial_services_entry( void* parameter);
struct rt_semaphore sem_serial_rx;

static void USART2_Init(void);

static void RCC_Configuration(void)
{
#ifdef RT_USING_UART1
    /* Enable USART2 GPIO clocks */
    RCC_AHB1PeriphClockCmd(UART1_GPIO_RCC, ENABLE);
    /* Enable USART2 clock */
    RCC_APB2PeriphClockCmd(RCC_APBPeriph_UART1, ENABLE);
    /* DMA clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);    
    /* TIM clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
#endif

}


static void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

#ifdef RT_USING_UART1
    /* Configure USART1 Rx/tx PIN */
    GPIO_InitStructure.GPIO_Pin = UART1_GPIO_RX | UART1_GPIO_TX;
    GPIO_Init(UART1_GPIO, &GPIO_InitStructure);

    /* Connect alternate function */
    GPIO_PinAFConfig(UART1_GPIO, UART1_TX_PIN_SOURCE, GPIO_AF_USART1);
    GPIO_PinAFConfig(UART1_GPIO, UART1_RX_PIN_SOURCE, GPIO_AF_USART1);
#endif

}


static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

#ifdef RT_USING_UART1
    /* Enable the USART1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the TIM7 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM8_BRK_TIM12_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

}


DMA_InitTypeDef  DMA_InitStructure;
static void DMA_Configuration(void)
{
    DMA_DeInit(USART1_TX_DMA_STREAM);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ((uint32_t)USART1 + 0x04);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_InitStructure.DMA_Channel = USART1_TX_DMA_CHANNEL;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;  
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)txBuffer;
    DMA_InitStructure.DMA_BufferSize = (uint16_t)1;
    DMA_Init(USART1_TX_DMA_STREAM, &DMA_InitStructure); 

    DMA_DeInit(USART1_RX_DMA_STREAM);
    DMA_InitStructure.DMA_Channel = USART1_RX_DMA_CHANNEL;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; 
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)serial1_rx_buffer;
    DMA_InitStructure.DMA_BufferSize = (uint16_t)sizeof(serial1_rx_buffer);
    DMA_Init(USART1_RX_DMA_STREAM, &DMA_InitStructure);
}


static void Timer_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_DeInit(TIM12);
    TIM_TimeBaseStructure.TIM_Period = 200; 
    TIM_TimeBaseStructure.TIM_Prescaler = (84-1);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM12, TIM_FLAG_Update);
    TIM_ITConfig(TIM12, TIM_IT_Update, ENABLE);

}


void stm32_hw_usart_init()
{
    USART_InitTypeDef USART_InitStructure;

    RCC_Configuration();

    GPIO_Configuration();

    NVIC_Configuration();

    DMA_Configuration();

    Timer_Configuration();

    /* uart init */
#ifdef RT_USING_UART1
    USART_InitStructure.USART_BaudRate = 380400;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    /* register uart1 */
    rt_hw_serial_register(&uart1_device, "uart1",
        RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_STREAM,
        &uart1);
    uart1_dma_tx.dma_channel = USART1_TX_DMA_STREAM;
    /* Enable USART1 DMA Tx request */
    USART_DMACmd(USART1, USART_DMAReq_Tx , ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Rx , ENABLE);
    DMA_Cmd(USART1_RX_DMA_STREAM, ENABLE);
    /* enable interrupt */
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
#endif

    //for eclr
    USART2_Init();
}


void mem_printf(const char *buffer, rt_size_t size)
{
    RT_ASSERT(uart1_dma_tx.dma_channel != RT_NULL);
    DMA_ClearFlag(uart1_dma_tx.dma_channel, USART1_TX_DMA_FLAG_HTIF | USART1_TX_DMA_FLAG_TCIF);    
    /* disable DMA */
    DMA_Cmd(uart1_dma_tx.dma_channel, DISABLE);

    /* set buffer address */
    uart1_dma_tx.dma_channel->M0AR = (rt_uint32_t)buffer;
    /* set size */
    uart1_dma_tx.dma_channel->NDTR = size;

    /* enable DMA */
    DMA_Cmd(uart1_dma_tx.dma_channel, ENABLE);
}


void reset_dma_address(void)
{
    USART_DMACmd(USART1, USART_DMAReq_Rx , DISABLE);
    DMA_Cmd(USART1_RX_DMA_STREAM, DISABLE);
    DMA_DeInit(USART1_RX_DMA_STREAM);
    DMA_Init(USART1_RX_DMA_STREAM, &DMA_InitStructure);
    USART1_RX_DMA_STREAM->NDTR = sizeof(serial1_rx_buffer);
    DMA_Cmd(USART1_RX_DMA_STREAM, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Rx , ENABLE);
}


void USART1_IRQHandler(void)
{
#ifdef RT_USING_UART1

    /* enter interrupt */
    rt_interrupt_enter();

    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        USART_ReceiveData(USART1);
        TIM_SetCounter(TIM12, 0);
        TIM_Cmd(TIM12, ENABLE);
    }
    else
    {
        rt_hw_serial_isr(&uart1_device);
    }

    /* leave interrupt */
    rt_interrupt_leave();
#endif
}


console_type function_switch = use_console;
int rx_length = 0;

void TIM8_BRK_TIM12_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rx_length = sizeof(serial1_rx_buffer) - USART1_RX_DMA_STREAM->NDTR;
    
    reset_dma_address();
    if(sem_serial_rx.parent.parent.flag != 0)
    {
        rt_sem_release(&sem_serial_rx);
    }

    TIM_Cmd(TIM12, DISABLE);
    TIM_ClearFlag(TIM12, TIM_FLAG_Update);
    /* leave interrupt */
    rt_interrupt_leave();
}


static void serial_services_entry( void* parameter)
{
    int len = rx_length;
    u8 * buffer_address = serial1_rx_buffer;
    while(1)
    {
        if(-RT_ETIMEOUT == rt_sem_take(&sem_serial_rx, 2000))
        {
            //需要重新确认连接的客户端类型
            is_reconnected = 1;
            continue;
        }
        else
        {
            if(is_reconnected)
            {
                is_reconnected = 0;
                
                if(serial1_rx_buffer[0] == '+')//freemaster起始符+
                {
                    function_switch = use_freemaster;
                }
                else if(serial1_rx_buffer[0] == '*')//通业上位机通信协议起始符*
                {
                    function_switch = use_ptu;
                }
                else if(serial1_rx_buffer[rx_length - 1] == 0x0D)//控制台指令以换行结束
                {
                    function_switch = use_console;
                }
            }
        }
        
        len = rx_length;
        buffer_address = serial1_rx_buffer;
        switch (function_switch)
        {
            case use_console:
                uart1.int_rx->save_index = 0;
                uart1.int_rx->read_index = 0;
                while (len--)
                {
                    uart1.int_rx->rx_buffer[uart1.int_rx->save_index++] = *buffer_address++;
                }

                if (uart1_device.rx_indicate != RT_NULL)
                {
                    uart1_device.rx_indicate(&uart1_device, len);
                }
                break;
            case use_freemaster:
                //PutData((const char *)buffer_address, 0, len);//--shiliangbao 140221  ????
                break;
            case use_ptu:
                //protocol.put_buffer(&protocol, (void *)buffer_address, len);
                break;
        }
    }
}


void console_services_init()
{
    struct rt_thread * serial_services_thread = rt_thread_create("console",
                                    serial_services_entry, RT_NULL,
                                    1024, 15, 1);
    if (serial_services_thread != RT_NULL)
    {
        rt_thread_startup(serial_services_thread);
    }
    rt_sem_init(&sem_serial_rx, "usart_rx", 0, 1);
}



static void USART2_Init(void)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* Enable UART clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_Init(&NVIC_InitStructure);

    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);

    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART configuration */
    USART_Init(USART2, &USART_InitStructure);

    /* Enable USART */
    USART_Cmd(USART2, ENABLE);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

