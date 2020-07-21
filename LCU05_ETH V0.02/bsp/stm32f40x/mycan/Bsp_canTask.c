

#include "Bsp_can.h"
#include "Bsp_canTask.h"
#include "Bsp_can_QueApi.h"

#include "Bsp_can_app.h"
#include "common.h"
#include "Bsp_systimer.h"



#define CanTask_Event_Cycle     ((uint32_t)0x01)


#define CAN_SEND_TRYTIMES       (3)


static uint32_t can_rxBuf;
static uint32_t can1_txBuf;
static uint32_t can2_txBuf;

static CAN_RX_DATA_RAM * pCan_rxBuf;
static CAN_TX_DATA_RAM * pCan1_txBuf;
static CAN_TX_DATA_RAM * pCan2_txBuf;

static volatile uint32_t Can1_bus_fault = 0;
static volatile uint32_t Can2_bus_fault = 0;

static __IO uint8_t  canPrintFlag = 0;


// 控制主逻辑运算的事件变量
static struct rt_event EventCanSend;

// 收取事件的变量
static rt_uint32_t _ev_cansend_flag = 0;

// 事件变量 初始化结果  只有EventCanSend正确初始化后，才可以Send Event
static __IO rt_err_t _ev_cansend_init = 0xFF;


//------------------------------------------------------------------
//  Can Rx Queue Task
//------------------------------------------------------------------
ALIGN(RT_ALIGN_SIZE)
static rt_uint32_t bsp_can_rx_queue_stack[BSP_CAN_Q_STACKSIZE];
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread bsp_can_rx_queue_thread;


//------------------------------------------------------------------
//  Can1 Tx Queue Task
//------------------------------------------------------------------
ALIGN(RT_ALIGN_SIZE)
static rt_uint32_t bsp_can1_tx_queue_stack[BSP_CAN_Q_STACKSIZE];
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread bsp_can1_tx_queue_thread;


//------------------------------------------------------------------
//  Can2 Tx Queue Task
//------------------------------------------------------------------
ALIGN(RT_ALIGN_SIZE)
static rt_uint32_t bsp_can2_tx_queue_stack[BSP_CAN_Q_STACKSIZE];
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread bsp_can2_tx_queue_thread;


//------------------------------------------------------------------
//  Can Task
//------------------------------------------------------------------
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
static void Can_RX_Print(CAN_RX_DATA_RAM * pDate);
static void Can_TX_Print(CAN_TX_DATA_RAM * pDate);


void  Bsp_CanInit(void)
{
    /* 通信事件机制初始化  */
    _ev_cansend_init = rt_event_init(&EventCanSend, "E_Can", RT_IPC_FLAG_FIFO);

    /* Can rx Queue队列初始化  */
    mBsp_Can_Rx_Queue_Init();
    /* Can tx1 Queue队列初始化  */
    mBsp_Can1_Tx_Queue_Init();
    /* Can tx2 Queue队列初始化  */
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


void CanTask_Send_Event_Cycle(void)
{
    if (_ev_cansend_init == RT_EOK)
    {
        rt_event_send(&EventCanSend, CanTask_Event_Cycle);
    }
}


static void CanTask_Check_Event(void)
{
    rt_event_recv(&EventCanSend,
        CanTask_Event_Cycle,
        (RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR),
        RT_WAITING_FOREVER,
        &_ev_cansend_flag);
}


static void  Bsp_can_rx_task(void* parameter)
{
    rt_thread_delay(500);
    System_HW_Can_Init();
    rt_kprintf("\r\n+ CAN INFO: System CAN HW Init success!\r\n");
    Bsp_canTimer_Init();

    for(;;) 
    {
        Can_CheckRxSem();
        
        //------------------------------------------------------------
        // 从队列弹出 接收数据帧 地址
        //------------------------------------------------------------
        can_rxBuf = CanQ_Pop_Rx_Msg();
        
        //------------------------------------------------------------
        // 弹出指针不为空, 则表示队列操作成功
        //------------------------------------------------------------
        if (can_rxBuf != CAN_QUEUE_NULL)
        {
            //--------------------------------------------------------
            // 转换指针类型: RAM操作数据块
            //--------------------------------------------------------
            pCan_rxBuf = (CAN_RX_DATA_RAM *)can_rxBuf; 
            
            Can_Rx_Data_Process(pCan_rxBuf);
            Can_RX_Print(pCan_rxBuf);
        }

    }
}


static void  Bsp_can1_tx_task(void* parameter)
{
    uint8_t mailFull = 0;
    uint8_t tryTimes = CAN_SEND_TRYTIMES;
    
    for(;;) 
    {
        Can1_CheckTxSem();
        
        can1_txBuf = Can1Q_Pop_Tx_Msg();
        
        if (can1_txBuf != CAN_QUEUE_NULL)
        {            
            pCan1_txBuf = (CAN_TX_DATA_RAM * )can1_txBuf;
            
            do
            {
                mailFull = 0;

                if (CAN_Transmit(CAN1, &(pCan1_txBuf->txMsg)) == CAN_TxStatus_NoMailBox)
                {
                    mailFull = 1;
                    rt_thread_delay(1);                    
                    
                    if (tryTimes-- == 0)
                    {
                        mailFull = 0;
                        rt_thread_delay(1);
                        
                        if (CAN_GetFlagStatus(CAN1, CAN_FLAG_BOF) == SET)
                        {
                            Can1_bus_fault = 1;
                        }
                    }
                }

            } while (mailFull);
            
            Can_TX_Print(pCan1_txBuf);
        }      
    }
}


static void  Bsp_can2_tx_task(void* parameter)
{
    uint8_t mailFull = 0;
    uint8_t tryTimes = CAN_SEND_TRYTIMES;
    
    for(;;) 
    {
        Can2_CheckTxSem();
        
        can2_txBuf = Can2Q_Pop_Tx_Msg();
        
        if (can2_txBuf != CAN_QUEUE_NULL)
        {            
            pCan2_txBuf = (CAN_TX_DATA_RAM * )can2_txBuf;
            
            do
            {
                mailFull = 0;

                if (CAN_Transmit(CAN2, &(pCan2_txBuf->txMsg)) == CAN_TxStatus_NoMailBox)
                {
                    mailFull = 1;
                    rt_thread_delay(1);                    
                    
                    if (tryTimes-- == 0)
                    {
                        mailFull = 0;
                        rt_thread_delay(1);
                        
                        if (CAN_GetFlagStatus(CAN2, CAN_FLAG_BOF) == SET)
                        {
                            Can2_bus_fault = 1;
                        }
                    }
                }

            } while (mailFull);
            
            Can_TX_Print(pCan2_txBuf);
        }
    }
}


static void Bsp_can_task(void* parameter)
{
    rt_thread_delay(500);
    for(;;)
    {
        CanTask_Check_Event();

        if (_ev_cansend_flag & CanTask_Event_Cycle)
        {
            Can_Cycle_Service();
        }
    }
}


//for print
static void Can_RX_Print(CAN_RX_DATA_RAM * pDate)
{
    uint32_t tick = 0; 
    uint16_t port = 0;
    uint8_t  len  = 0; 
    uint8_t  temp = 0;    
    CAN_EXTID_INFO info;    

    if (canPrintFlag)
    {
        tick = pDate->parent.Tick;
        port = pDate->parent.CanPort;
        len = pDate->rxMsg.DLC;        
        info.value = pDate->rxMsg.ExtId;       
        
        rt_kprintf("CAN%d, R, %10d, %2d -> %2d, %2x, %d, %d, [", port, tick, info.id.src, info.id.dst, info.id.funID, info.id.pri, len);
        
        for (temp = 0; temp < len; temp++)
        {
            rt_kprintf(" %2x,", pDate->rxMsg.Data[temp]);
        }
        
        rt_kprintf("]\r\n");
    }
}


static void Can_TX_Print(CAN_TX_DATA_RAM * pDate)
{
    uint32_t tick = 0; 
    uint16_t port = 0;
    uint8_t  len  = 0; 
    uint8_t  temp = 0;
    CAN_EXTID_INFO info;
    
    if (canPrintFlag)
    {
        tick = pDate->parent.Tick;
        port = pDate->parent.CanPort;
        len = pDate->txMsg.DLC;        
        info.value = pDate->txMsg.ExtId;       
        
        rt_kprintf("CAN%d, T, %10d, %2d -> %2d, %2x, %d, %d, [", port, tick, info.id.src, info.id.dst, info.id.funID, info.id.pri, len);
        
        for (temp = 0; temp < len; temp++)
        {
            rt_kprintf(" %2x,", pDate->txMsg.Data[temp]);
        }
        
        rt_kprintf("]\r\n");
    }
}


void can_show(void)
{
    if (canPrintFlag)
        canPrintFlag = 0;
    else
        canPrintFlag = 1;
}


#include "finsh.h"
FINSH_FUNCTION_EXPORT(can_show, show system Can data)

