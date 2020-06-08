

#include "Bsp_can_QueApi.h"


//------------------------------------------------------------------------
//
//                             通用队列管理模块            
//
//
//             uint32_t type -->   CAN_OP_Q  <------ Write FIFO 指针存放区
//                                     |
//                                     |
//                                     V
// +--------+--------+--------+--------+--------+--------+--------+--------+
// | pFIFO* | pFIFO* | pFIFO* | pFIFO* | ...... | pFIFO* | pFIFO* | pFIFO* |
// +--------+--------+--------+--------+--------+--------+--------+--------+
// ^        ^                 ^                                            ^
// |        |                 |                                            |
// |        |               [QIn]                                          |
// |     [QOut]                                                            |
//[QStart]                                                              [QEnd]
//
// ^                                                                       ^
// |                                                                       |
// +-----------------------------------------------------------------------+
//                                  [QSize]
//
//
//------------------------------------------------------------------------
static CAN_Q  CanTxdataQ;
static CAN_Q  *pTQ = &CanTxdataQ; 
//------------------------------------------------------------------------
// 创建Tx信号量
//------------------------------------------------------------------------
struct rt_semaphore Can2_Tx_OPSem;

//------------------------------------------------------------------------
// CAN TX消息队列RAM区
//------------------------------------------------------------------------
static uint32_t CanTxQueueRam[CAN_TX_QUEUE_SIZE] = {0};
//------------------------------------------------------------------------
// 真正的数据FIFO    配合QUEUE 使用 使用外部RAM分配空间
// 在 mBsp_Queue_Init() 函数中分配了内存空间
//------------------------------------------------------------------------
static CAN_TX_DATA_RAM CanTxDataFIFO[CAN_TX_QUEUE_SIZE] = {0};
//------------------------------------------------------------------------
// FIFO操作用指针 环形FIFO
//------------------------------------------------------------------------
static uint32_t CanTxFifoRamPtr = 0;
//------------------------------------------------------------------------
// 队列初始化状态寄存器
//------------------------------------------------------------------------
static __IO uint16_t CanTxQInitRegister = CAN_INIT_NOT;
//------------------------------------------------------------------------
// 缓冲区状态寄存器
//------------------------------------------------------------------------
static __IO uint16_t CanTxRegister = CAN_FIFO_EMPTY_MASK;

//------------------------------------------------------------------------
// 函数声明区  模块内部函数
//------------------------------------------------------------------------
static void     rt_Q_Add(uint32_t msg);     //队列API : 压入操作 
static uint32_t rt_Q_Remove(void);          //队列API : 弹出操作
static void     rt_Q_Reset(void);           //队列API : 复位操作
static void     rt_Q_Init(void);            //队列API : 初始化操作


void mBsp_Can2_Tx_Queue_Init(void)
{
	// 创建信号量 
	rt_err_t rtv = rt_sem_init(&Can2_Tx_OPSem,  "SQTCan2",  0,  RT_IPC_FLAG_FIFO);

	// 初始化FIFO操作指针, 该指针清零处理 
	CanTxFifoRamPtr = 0;
    
	// 队列初始化
	rt_Q_Init();
    
	// 队列初始化完成，置位初始化状态寄存器
	CanTxQInitRegister = CAN_INIT_OK;
}


static void rt_Q_Add(uint32_t msg)
{
	uint32_t entries;
	register rt_base_t temp;

	//队列未初始化，不进行压栈操作
	if(CanTxQInitRegister != CAN_INIT_OK) return;

	/* lock interrupt */
	temp = rt_hw_interrupt_disable();

	if (pTQ->QEntries >= pTQ->QSize) {          /* Make sure queue is not full.                         */
		/* unlock interrupt */
		rt_hw_interrupt_enable(temp);

		//队列已满，置位寄存器
		CanTxRegister = CAN_FIFO_FULL_MASK;

		return;
	}

	//压入队列
	*pTQ->QIn++ = msg;                                  /* Insert message into queue.                           */
	//队列为下次压入进行指针维护
	pTQ->QEntries++;                                    /* Update the nbr of entries in the queue.              */
	if (pTQ->QEntries >= pTQ->QSize)
	{
		//队列已满，置位寄存器 2014-1-2
		CanTxRegister = CAN_FIFO_FULL_MASK;
	}
	
	if (pTQ->QIn == pTQ->QEnd) {              /* Wrap IN ptr when at end of queue.                    */
		pTQ->QIn  = pTQ->QStart;
	}
	//更新队列压入的最大值
	entries = pTQ->QEntries;
	if (pTQ->QEntriesMax < entries) {
		pTQ->QEntriesMax = entries;
	}

	if(pTQ->QEntriesMax == pTQ->QSize)
	{
		for(;;)
			break;
	}

	/* unlock interrupt */
	rt_hw_interrupt_enable(temp);
}


static uint32_t rt_Q_Remove(void)
{
	uint32_t  msg;
	register rt_base_t temp;
	/* lock interrupt */
	temp = rt_hw_interrupt_disable();

	if (pTQ->QEntries == 0) {                           /* Chk for queue empty.                                 */
		msg  = CAN_QUEUE_NULL;
		/* unlock interrupt */
		rt_hw_interrupt_enable(temp);

		return(msg);
	}
	//弹出数据
	msg = *pTQ->QOut++;                                 /* Extract oldest message from the queue.               */
	//队列为下次弹出进行指针维护
	pTQ->QEntries--;                                    /* Update the number of entries in the queue            */
	if (pTQ->QOut == pTQ->QEnd) {             /* Wrap OUT ptr when at the end of the queue.           */
		pTQ->QOut  = pTQ->QStart;
	}

    //  [5/4/2015 ]
    if (pTQ->QEntries == 0)
    {
        CanTxRegister = CAN_FIFO_EMPTY_MASK;
    }

	/* unlock interrupt */
	rt_hw_interrupt_enable(temp);
	//返回弹出的数据
	return (msg);                                               /* Return message received.                             */
}


static void rt_Q_Reset(void)
{
	pTQ->QIn      = pTQ->QStart;
	pTQ->QOut     = pTQ->QStart;
	pTQ->QEntries = 0;
}


static void rt_Q_Init(void)
{
	pTQ->QStart      =  CanTxQueueRam;     /*  Initialize Finsh Q.   */
	pTQ->QEnd        = &CanTxQueueRam[CAN_TX_QUEUE_SIZE - 1];
	pTQ->QSize       =  CAN_TX_QUEUE_SIZE;
	pTQ->QEntriesMax = 0;

	rt_Q_Reset();    /* Reset Rx status words queue.    */
}


uint8_t Can2Q_Push_Tx_Msg(uint32_t port, uint32_t tick, CanTxMsg* msg)
{
    uint32_t _curr_ptr;
    register rt_base_t temp;

	//-------------------------------------------------------------------
	// 判断队列是否满了，没有满时，可以继续压栈
	//-------------------------------------------------------------------
	if(CanTxRegister != CAN_FIFO_FULL_MASK)
	{
        //------------------------------------------------------------------------
        // Bug Fix Code 2013-12-16
        //------------------------------------------------------------------------
        temp = rt_hw_interrupt_disable();
        _curr_ptr = CanTxFifoRamPtr;
		//------------------------------------------------------------------------
		// 维护FIFO区操作指针
		//------------------------------------------------------------------------
		CanTxFifoRamPtr++;
		if(CanTxFifoRamPtr >= CAN_TX_QUEUE_SIZE) CanTxFifoRamPtr = 0;
        rt_hw_interrupt_enable(temp);

		CanTxDataFIFO[_curr_ptr].canPort         = port;
		CanTxDataFIFO[_curr_ptr].tick            = tick;
        
		CanTxDataFIFO[_curr_ptr].txMsg.StdId     = msg->StdId;
		CanTxDataFIFO[_curr_ptr].txMsg.ExtId     = msg->ExtId;
		CanTxDataFIFO[_curr_ptr].txMsg.IDE       = msg->IDE;
		CanTxDataFIFO[_curr_ptr].txMsg.RTR       = msg->RTR;
		CanTxDataFIFO[_curr_ptr].txMsg.DLC       = msg->DLC;
		CanTxDataFIFO[_curr_ptr].txMsg.Data[0]   = msg->Data[0];
		CanTxDataFIFO[_curr_ptr].txMsg.Data[1]   = msg->Data[1];
		CanTxDataFIFO[_curr_ptr].txMsg.Data[2]   = msg->Data[2];
		CanTxDataFIFO[_curr_ptr].txMsg.Data[3]   = msg->Data[3];
		CanTxDataFIFO[_curr_ptr].txMsg.Data[4]   = msg->Data[4];
		CanTxDataFIFO[_curr_ptr].txMsg.Data[5]   = msg->Data[5];
		CanTxDataFIFO[_curr_ptr].txMsg.Data[6]   = msg->Data[6];
		CanTxDataFIFO[_curr_ptr].txMsg.Data[7]   = msg->Data[7];
		//------------------------------------------------------------------------
		// 将FIFO当前数据块地址压入队列
		//------------------------------------------------------------------------
		rt_Q_Add((uint32_t)(&CanTxDataFIFO[_curr_ptr]));
		//------------------------------------------------------------------------
		// 弹出信号量, 进行数据操作
		//------------------------------------------------------------------------
		rt_sem_release(&Can2_Tx_OPSem);
		
		return (uint8_t)(!0);
	}
	
	return 0;
}


void Can2_CheckTxSem(void)
{
    rt_sem_take(&Can2_Tx_OPSem, RT_WAITING_FOREVER);
}


uint32_t Can2Q_Pop_Tx_Msg(void)
{
    return rt_Q_Remove();
}

