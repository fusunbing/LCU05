

#include "Bsp_canTask.h"
#include "Bsp_canPrint.h"
#include "Bsp_canApp.h"

/* CAN�̳߳ش�С */
#define CAN_RX_TASK_STACK_SIZE      (1024)

/* CAN�߳����ȼ� */
#define CAN_RX_TASK_PRIORITY        (8)

/* ������Ϣ���д�С */
#define CAN_RX_MQ_SIZE              (128)

/* ��Ϣ�����ڴ�� */
static CanRxMsg can_RxMq_pool[CAN_RX_MQ_SIZE];

/* ��Ϣ���п��ƿ� */
static struct rt_messagequeue can_RxMq;

/* can�����߳̿��ƿ� */
static struct rt_thread can_rx_thread;

/* can�����߳��ڴ�� */
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
    
    /* ��Ϣ���г�ʼ�� */
    can_RxMq_init = rt_mq_init(&can_RxMq,
                        "can_rx_mq",
                        can_RxMq_pool,                      /* �ڴ��ָ�� */
                        sizeof(CanRxMsg),                   /* ÿ����Ϣ�Ĵ�С */
                        sizeof(can_RxMq_pool),              /* �ڴ�ش�С */
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
		// ��ȡCAN1���������ձ���
        CAN_Receive(CAN1, CAN_FIFO0, &msg);
        
        msg.ExtId |= 0x20000000;
        
        // �����յı���ѹ�����
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
        // ��ȡCAN2���������ձ���
        CAN_Receive(CAN2, CAN_FIFO0, &msg);
        
        msg.ExtId |= 0x40000000;

        // �����յı���ѹ�����
        if(can_RxMq_init == RT_EOK)
        {
            rt_mq_send(&can_RxMq, &msg, sizeof(CanRxMsg));
        }
    }
}

