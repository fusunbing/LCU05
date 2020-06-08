

#include "Bsp_canTask.h"
#include "Bsp_can.h"
#include "Bsp_canTimer.h"
#include "Bsp_canPrint.h"
#include "Bsp_canApp.h"


/* 线程池大小 */
#define CAN_TASK_STACK_SIZE     (1024)
/* 线程优先级 */
#define CAN_TASK_PRIORITY       (13)

#define CanTask_Event_Cycle     (1)
#define CanTask_Event_Logic     (2)


static struct rt_thread can_thread;
static uint8_t can_thread_stack[CAN_TASK_STACK_SIZE];


/* CAN发送事件控制块 */
static struct rt_event canSend_Event;
static __IO rt_err_t canSend_Event_init = 0xFF;


static void can_thread_entry(void* parameter)
{
    rt_uint32_t flag = 0;
    
    for(;;)
    {
        rt_event_recv(&canSend_Event, (CanTask_Event_Cycle | CanTask_Event_Logic), 
                        (RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR),
                        RT_WAITING_FOREVER, &flag);
        
        if(flag & CanTask_Event_Cycle)
        {
            can_tx_serve();
        }
        else if(flag & CanTask_Event_Logic)
        {
            
        }
    }
}


void Bsp_canTask_Init(void)
{
    rt_err_t rtv;
    
    canSend_Event_init = rt_event_init(&canSend_Event, "CanSend_Event", RT_IPC_FLAG_FIFO);
    
    rtv = rt_thread_init(&can_thread,
            "CanTask",
            can_thread_entry,
            RT_NULL,
            can_thread_stack, 
            CAN_TASK_STACK_SIZE,
            CAN_TASK_PRIORITY,
            8);
                            
    if (rtv == RT_EOK)
    {
        rt_thread_startup(&can_thread);
    }
}


void CanTask_Send_Event_Cycle(void)
{
    if (canSend_Event_init == RT_EOK)
    {
        rt_event_send(&canSend_Event, CanTask_Event_Cycle);
    }
}


void CanTask_Send_Event_Logic(void)
{
    if (canSend_Event_init == RT_EOK)
    {
        rt_event_send(&canSend_Event, CanTask_Event_Logic);
    }
}


void  Bsp_CanInit(void)
{
    /* CAN控制器初始化 */
    Bsp_canHW_Init();
    
    /* CAN线程定时器初始化 */
    Bsp_canTimer_Init();
    
    rt_kprintf("\r\n+ CAN INFO: System CAN HW Init success!\r\n");
    
    /* CAN接收线程初始化 */
    Bsp_can_rx_Task_Init();
    
    /* CAN发送线程初始化 */
    Bsp_can_tx_Task_Init();
    
    /* CAN线程初始化 */
    Bsp_canTask_Init();
    
    /* CAN接收处理函数注册 */
    can_rx_handle_register();
}

