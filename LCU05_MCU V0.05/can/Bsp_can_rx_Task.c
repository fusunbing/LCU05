

#include "Bsp_canTask.h"
#include "Bsp_canPrint.h"
#include "Bsp_canApp.h"

/* CAN线程池大小 */
#define CAN_RX_TASK_STACK_SIZE      (1024)

/* CAN线程优先级 */
#define CAN_RX_TASK_PRIORITY        (8)

/* 接收消息队列大小 */
#define CAN_RX_MQ_SIZE              (128)

/* 消息队列内存池 */
static CanRxMsg can_RxMq_pool[CAN_RX_MQ_SIZE];

/* 消息队列控制块 */
static struct rt_messagequeue can_RxMq;

/* can接收线程控制块 */
static struct rt_thread can_rx_thread;

/* can接收线程内存池 */
static uint8_t can_rx_thread_stack[CAN_RX_TASK_STACK_SIZE];

static __IO rt_err_t can_RxMq_init = 0xFF;


static void  can_rx_thread_entry(void* parameter)
{
    CanRxMsg msg;

    for(;;) 
    {
        if(RT_EOK == rt_mq_recv(&can_RxMq, &msg, sizeof(CanRxMsg), RT_WAITING_FOREVER))
        {
            can_rx_serve(&msg);
            Can_RX_Print(&msg, msg.ExtId >> 29);
        }
    }
}


void  Bsp_can_rx_Task_Init(void)
{
    rt_err_t ret;
    
    /* 消息队列初始化 */
    can_RxMq_init = rt_mq_init(&can_RxMq,
                        "can_rx_mq",
                        can_RxMq_pool,                      /* 内存池指针 */
                        sizeof(CanRxMsg),                   /* 每个消息的大小 */
                        sizeof(can_RxMq_pool),              /* 内存池大小 */
                        RT_IPC_FLAG_FIFO);
    
    ret = rt_thread_init(&can_rx_thread,
            "can_rx_task",
            can_rx_thread_entry,
            RT_NULL,
            can_rx_thread_stack,
            CAN_RX_TASK_STACK_SIZE,
            CAN_RX_TASK_PRIORITY, 
            20);
                                
    if(RT_EOK == ret)
    {
        rt_thread_startup(&can_rx_thread);
    }
}


void CAN1_RX0_IRQ(void)
{
    CanRxMsg msg = { 0 };
    
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0))
	{
		// 收取CAN1控制器接收报文
        CAN_Receive(CAN1, CAN_FIFO0, &msg);
        
        msg.ExtId |= 0x20000000;
        
        // 将接收的报文压入队列
        if(can_RxMq_init == RT_EOK)
        {
            rt_mq_send(&can_RxMq, &msg, sizeof(CanRxMsg));
        }
    }
}


void CAN2_RX0_IRQ(void)
{
    CanRxMsg msg = { 0 };
    
    if(CAN_GetITStatus(CAN2, CAN_IT_FMP0))
    {
        // 收取CAN2控制器接收报文
        CAN_Receive(CAN2, CAN_FIFO0, &msg);
        
        msg.ExtId |= 0x40000000;

        // 将接收的报文压入队列
        if(can_RxMq_init == RT_EOK)
        {
            rt_mq_send(&can_RxMq, &msg, sizeof(CanRxMsg));
        }
    }
}

