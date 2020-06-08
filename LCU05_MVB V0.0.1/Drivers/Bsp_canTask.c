

#include "Bsp_canTask.h"
#include "Bsp_canApp.h"


//控制主逻辑运算的事件变量
static struct rt_event EventCanSend;

//事件变量初始化结果
static __IO rt_err_t _ev_cansend_init = 0xFF;

//收取事件的变量
static rt_uint32_t _ev_cansend_flag = 0;


//  Can Rx Queue Task
ALIGN(RT_ALIGN_SIZE)
static rt_uint32_t bsp_can_rx_queue_stack[BSP_CAN_Q_STACKSIZE];
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread bsp_can_rx_queue_thread;


//  Can1 Tx Queue Task
ALIGN(RT_ALIGN_SIZE)
static rt_uint32_t bsp_can1_tx_queue_stack[BSP_CAN_Q_STACKSIZE];
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread bsp_can1_tx_queue_thread;


//  Can2 Tx Queue Task
ALIGN(RT_ALIGN_SIZE)
static rt_uint32_t bsp_can2_tx_queue_stack[BSP_CAN_Q_STACKSIZE];
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread bsp_can2_tx_queue_thread;


//  Can Task
ALIGN(RT_ALIGN_SIZE)
static rt_uint32_t bsp_can_stack[BSP_TCAN_STACKSIZE];
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread bsp_can_thread;


//------------------------------------------------------------------
// 内部函数声明
//------------------------------------------------------------------
static void Bsp_can_rx_task(void* parameter);
static void Bsp_can1_tx_task(void* parameter);
static void Bsp_can2_tx_task(void* parameter);
static void Bsp_can_task(void* parameter);


void  Bsp_CanInit(void)
{    
    System_HW_Can_Init();
    rt_kprintf("\r\n+ CAN INFO: System CAN HW Init success!\r\n");

    //-------------------------------------------------------------------------
    // 通信事件机制初始化
    //-------------------------------------------------------------------------
    _ev_cansend_init = rt_event_init(&EventCanSend, "E_Can", RT_IPC_FLAG_FIFO);
    
	//--------------------------------------------------------------
	// Can rx Queue队列初始化
	//--------------------------------------------------------------
	mBsp_Can_Rx_Queue_Init();
	//--------------------------------------------------------------
    // Can tx1 Queue队列初始化
    //--------------------------------------------------------------
    mBsp_Can1_Tx_Queue_Init();
	//--------------------------------------------------------------
	// Can tx2 Queue队列初始化
	//--------------------------------------------------------------
	mBsp_Can2_Tx_Queue_Init();

    
    // can rx task
    rt_thread_init(&bsp_can_rx_queue_thread,
        "BspCanQueRx",
        Bsp_can_rx_task,
        RT_NULL,
        &bsp_can_rx_queue_stack[0], 
        sizeof(bsp_can_rx_queue_stack),
        BSP_CAN_R_Q_PRIORITY, 
        20
        );

    // can1 tx task
    rt_thread_init(&bsp_can1_tx_queue_thread,
        "BspCan1QueTx",
        Bsp_can1_tx_task,
        RT_NULL,
        &bsp_can1_tx_queue_stack[0], 
        sizeof(bsp_can1_tx_queue_stack),
        BSP_CAN_T_Q_PRIORITY, 
        4
        );


	// can2 tx task
	rt_thread_init(&bsp_can2_tx_queue_thread,
		"BspCan2QueTx",
		Bsp_can2_tx_task,
		RT_NULL,
		&bsp_can2_tx_queue_stack[0], 
		sizeof(bsp_can2_tx_queue_stack),
		BSP_CAN_T_Q_PRIORITY, 
		4
		);

    // CAN 周期发送任务的创建
    rt_thread_init(&bsp_can_thread,
        "BspCanTask",
        Bsp_can_task,
        RT_NULL,
        &bsp_can_stack[0], 
        sizeof(bsp_can_stack),
        BSP_TCAN_PRIORITY, 
        8
        );
        
    rt_thread_startup(&bsp_can_rx_queue_thread);
    rt_thread_startup(&bsp_can1_tx_queue_thread);
	rt_thread_startup(&bsp_can2_tx_queue_thread);
    rt_thread_startup(&bsp_can_thread);
}


static void  Bsp_can_rx_task(void* parameter)
{
    for(;;) 
    {
        Can_CheckRxSem();        
        can_rx_serve();
		
    }
}


static void  Bsp_can1_tx_task(void* parameter)
{
    for(;;) 
    {
		Can1_CheckTxSem();
        can1_tx_serve();
		
    }
}


static void  Bsp_can2_tx_task(void* parameter)
{    
	for(;;) 
	{
		Can2_CheckTxSem();        
        can2_tx_serve();
	}
}

void CanTask_Send_Event_Cycle(void)
{
    if (_ev_cansend_init == RT_EOK)
    {
        rt_event_send(&EventCanSend, CanTask_Event_Cycle);
    }
}


void CanTask_Send_Event_Logic(void)
{
    if (_ev_cansend_init == RT_EOK)
    {
        rt_event_send(&EventCanSend, CanTask_Event_Logic);
    }
}


static void CanTask_Check_Event(void)
{
    rt_event_recv(&EventCanSend,
        (CanTask_Event_Cycle | CanTask_Event_Logic),
        (RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR),
        RT_WAITING_FOREVER,
        &_ev_cansend_flag);
}


static void Bsp_can_task(void* parameter)
{
	
    for(;;)
    {
        CanTask_Check_Event();
        
        if (_ev_cansend_flag & CanTask_Event_Cycle)
        {
            canApp_serve();
        }
    }
}

