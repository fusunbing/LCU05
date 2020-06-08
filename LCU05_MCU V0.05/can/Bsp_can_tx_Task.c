

#include "Bsp_canTask.h"
#include "Bsp_canPrint.h"


/* 线程池大小 */
#define CAN_TX_TASK_STACK_SIZE      (1024)

/* 线程优先级 */
#define CAN_TX_TASK_PRIORITY        (8)

/* 接收消息队列大小 */
#define CAN_TX_MQ_SIZE              (128)

/* 消息队列内存池 */
static CanTxMsg can_TxMq_pool[CAN_TX_MQ_SIZE];

/* 消息队列控制块 */
static struct rt_messagequeue can_TxMq;

/* can发送线程控制块 */
static struct rt_thread can_tx_thread;

/* can发送线程内存池 */
static uint8_t can_tx_thread_stack[CAN_TX_TASK_STACK_SIZE];

static __IO rt_err_t can_TxMq_init = RT_ERROR;


static void CanHW_tx(CAN_TypeDef* CANx, CanTxMsg* msg)
{
    uint8_t flag = 0;
    uint8_t cnt = 3;

    if(CAN_GetFlagStatus(CANx, CAN_FLAG_BOF) == SET)
    {
        return;
    }

    do
    {
        flag = 0;
        
        if(CAN_Transmit(CANx, msg) == CAN_TxStatus_NoMailBox)
        {
            flag = 1;
            rt_thread_delay(1);
            
            if (cnt-- == 0)
            {
                flag = 0;
                rt_thread_delay(1);
            }
        }
        
    }while(flag);
}


static void can_tx_thread_entry(void* parameter)
{
    CanTxMsg msg;
    static uint32_t CANx = 1;
    
    for(;;) 
    {
        if(RT_EOK == rt_mq_recv(&can_TxMq, &msg, sizeof(CanTxMsg), RT_WAITING_FOREVER))
        {
            //打印CAN发送数据
            Can_TX_Print(&msg, CANx);
            
            if(CANx == 1)
            {
                CanHW_tx(CAN1, &msg);
                CANx = 2;
            }
            else
            {
                CanHW_tx(CAN2, &msg);
                CANx = 1;
            }
        }
    }
}


void  Bsp_can_tx_Task_Init(void)
{
    rt_err_t ret;
    
    can_TxMq_init = rt_mq_init(&can_TxMq,
                        "can_tx_mq",
                        can_TxMq_pool,                      /* 内存池指针 */
                        sizeof(CanTxMsg),                   /* 每个消息的大小 */
                        sizeof(can_TxMq_pool),              /* 内存池大小 */
                        RT_IPC_FLAG_FIFO);

    ret = rt_thread_init(&can_tx_thread,
            "can_tx_task",
            can_tx_thread_entry,
            RT_NULL,
            can_tx_thread_stack, 
            CAN_TX_TASK_STACK_SIZE,
            CAN_TX_TASK_PRIORITY,
            4);

    if(RT_EOK == ret)
    {
        rt_thread_startup(&can_tx_thread);
    }
}


void CANx_Send(CanTxMsg* msg)
{
    if(can_TxMq_init == RT_EOK)
    {
        rt_mq_send(&can_TxMq, msg, sizeof(CanTxMsg));
    }
}

