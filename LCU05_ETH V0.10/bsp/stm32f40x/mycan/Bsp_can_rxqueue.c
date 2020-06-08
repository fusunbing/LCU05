

#include "Bsp_can_QueApi.h"


//------------------------------------------------------------------------
// CAN队列缓冲区大小
//------------------------------------------------------------------------
#define  CAN_RX_QUEUE_SIZE        (60)   
//------------------------------------------------------------------------
// Queue缓冲区大小
//------------------------------------------------------------------------
#define CAN_RX_DRAM_SIZE          (sizeof(CAN_RX_DATA_RAM) * CAN_RX_QUEUE_SIZE)
//------------------------------------------------------------------------
// 创建控制台finsh打印操作任务的信号量
//------------------------------------------------------------------------
struct rt_semaphore Can_Rx_OPSem;
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
static CAN_Q  CanRxdataQ;
static CAN_Q  *pCanRxDataQ = &CanRxdataQ; 
//------------------------------------------------------------------------
// 队列消息RAM区
//------------------------------------------------------------------------
static uint32_t CanRxQueueRam[CAN_RX_QUEUE_SIZE] = {0};
//------------------------------------------------------------------------
// 真正的数据FIFO    配合QUEUE 使用 使用外部RAM分配空间
// 在 mBsp_Queue_Init() 函数中分配了内存空间
//------------------------------------------------------------------------
//static PCAN_RX_DATA_RAM CanRxDataFIFO;
static CAN_RX_DATA_RAM CanRxDataFIFO[CAN_RX_QUEUE_SIZE] = {0};
//------------------------------------------------------------------------
// FIFO操作用指针 环形FIFO
//------------------------------------------------------------------------
static uint32_t CanRxFifoRamPtr = 0;
//------------------------------------------------------------------------
// 队列初始化状态寄存器
//------------------------------------------------------------------------
static __IO uint16_t CanRxQInitRegister = CAN_INIT_NOT;
//------------------------------------------------------------------------
// 缓冲区状态寄存器
//------------------------------------------------------------------------
static __IO uint16_t CanRxRegister = CAN_FIFO_EMPTY_MASK;
//------------------------------------------------------------------------
// 函数声明区
//------------------------------------------------------------------------
void mBsp_Can_Rx_Queue_Init(void);
void CanQ_Push_Rx_Msg(uint8_t port, uint32_t tick, CanRxMsg* msg);
void CanQ_Rx_SetFifoReg(uint16_t val);
uint16_t CanQ_Rx_GetFifoReg(void);
uint32_t CanQ_Pop_Rx_Msg(void);
//------------------------------------------------------------------------
// 函数声明区  模块内部函数
//------------------------------------------------------------------------
static void rt_Q_Add(uint32_t msg);     //队列API : 压入操作 
static uint32_t rt_Q_Remove(void);      //队列API : 弹出操作
static void rt_Q_Reset(void);           //队列API : 复位操作
static void rt_Q_Init(void);            //队列API : 初始化操作


void mBsp_Can_Rx_Queue_Init(void)
{
	//-------------------------------------------------------
	// 创建信号量 
	//-------------------------------------------------------
	rt_err_t rtv    = rt_sem_init(&Can_Rx_OPSem,  "SemCanQRx",  0,  RT_IPC_FLAG_FIFO);
	//-------------------------------------------------------
	// 申请队列缓冲内存 转为静态分配空间 [6/26/2014 -3550]
	//-------------------------------------------------------
	//CanRxDataFIFO   = rt_malloc(CAN_RX_DRAM_SIZE);
	//-------------------------------------------------------
	// 初始化FIFO操作指针, 该指针清零处理 
	//-------------------------------------------------------
	CanRxFifoRamPtr = 0;
	//-------------------------------------------------------
	// 队列初始化
	//-------------------------------------------------------
	rt_Q_Init();
	//-------------------------------------------------------
	// 队列初始化完成，置位初始化状态寄存器
	//-------------------------------------------------------
	CanRxQInitRegister = CAN_INIT_OK;
}


//队列API : 压入操作
static void rt_Q_Add(uint32_t msg)
{
	uint32_t entries;
	register rt_base_t temp;

	//队列未初始化，不进行压栈操作
	if(CanRxQInitRegister != CAN_INIT_OK) return;

	/* lock interrupt */
	temp = rt_hw_interrupt_disable();

	if (pCanRxDataQ->QEntries >= pCanRxDataQ->QSize) {          /* Make sure queue is not full.                         */
		/* unlock interrupt */
		rt_hw_interrupt_enable(temp);

		//队列已满，置位寄存器
		CanRxRegister = CAN_FIFO_FULL_MASK;
		return;
	}

	//压入队列
	*pCanRxDataQ->QIn++ = msg;                                  /* Insert message into queue.                           */
	//队列为下次压入进行指针维护
	pCanRxDataQ->QEntries++;                                    /* Update the nbr of entries in the queue.              */
	if (pCanRxDataQ->QIn == pCanRxDataQ->QEnd) {              /* Wrap IN ptr when at end of queue.                    */
		pCanRxDataQ->QIn  = pCanRxDataQ->QStart;
	}
	//更新队列压入的最大值
	entries = pCanRxDataQ->QEntries;
	if (pCanRxDataQ->QEntriesMax < entries) {
		pCanRxDataQ->QEntriesMax = entries;
	}

	if(pCanRxDataQ->QEntriesMax == pCanRxDataQ->QSize)
	{
		for(;;)
			break;
	}

	/* unlock interrupt */
	rt_hw_interrupt_enable(temp);
}


//队列API : 弹出操作  
static uint32_t rt_Q_Remove(void)
{
	uint32_t  msg;
	register rt_base_t temp;
	/* lock interrupt */
	temp = rt_hw_interrupt_disable();

	if (pCanRxDataQ->QEntries == 0) {                           /* Chk for queue empty.                                 */
		msg  = CAN_QUEUE_NULL;
		/* unlock interrupt */
		rt_hw_interrupt_enable(temp);

		return(msg);
	}
	//弹出数据
	msg = *pCanRxDataQ->QOut++;                                 /* Extract oldest message from the queue.               */
	//队列为下次弹出进行指针维护
	pCanRxDataQ->QEntries--;                                    /* Update the number of entries in the queue            */
	if (pCanRxDataQ->QOut == pCanRxDataQ->QEnd) {               /* Wrap OUT ptr when at the end of the queue.           */
		pCanRxDataQ->QOut  = pCanRxDataQ->QStart;
	}
    //  [5/4/2015 ]
    if (pCanRxDataQ->QEntries == 0)
    {
        CanRxRegister = CAN_FIFO_EMPTY_MASK;
    }
	/* unlock interrupt */
	rt_hw_interrupt_enable(temp);
	//返回弹出的数据
	return (msg);                                               /* Return message received.                             */
}


//队列API : 复位操作
static void rt_Q_Reset(void)
{
	pCanRxDataQ->QIn      = pCanRxDataQ->QStart;
	pCanRxDataQ->QOut     = pCanRxDataQ->QStart;
	pCanRxDataQ->QEntries = 0;
}


//队列API : 初始化操作
static void rt_Q_Init(void)
{
	pCanRxDataQ->QStart      =  CanRxQueueRam;     /*  Initialize Finsh Q.   */
	pCanRxDataQ->QEnd        = &CanRxQueueRam[CAN_RX_QUEUE_SIZE - 1];
	pCanRxDataQ->QSize       =  CAN_RX_QUEUE_SIZE;
	pCanRxDataQ->QEntriesMax = 0;

	rt_Q_Reset();    /* Reset Rx status words queue.    */
}


//将数据压入FIFO
void CanQ_Push_Rx_Msg(uint8_t port, uint32_t tick, CanRxMsg* msg)
{
	//-------------------------------------------------------------------
	// 判断队列是否满了，没有满时，可以继续压栈
	//-------------------------------------------------------------------
	if(CanRxRegister != CAN_FIFO_FULL_MASK)
	{
		CanRxDataFIFO[CanRxFifoRamPtr].parent.CanPort = port;
		CanRxDataFIFO[CanRxFifoRamPtr].parent.Tick    = tick;
		CanRxDataFIFO[CanRxFifoRamPtr].parent.BoardId = (msg->ExtId >> 9) & 0xFF;
		CanRxDataFIFO[CanRxFifoRamPtr].rxMsg.StdId     = msg->StdId;
		CanRxDataFIFO[CanRxFifoRamPtr].rxMsg.ExtId     = msg->ExtId;
		CanRxDataFIFO[CanRxFifoRamPtr].rxMsg.IDE       = msg->IDE;
		CanRxDataFIFO[CanRxFifoRamPtr].rxMsg.RTR       = msg->RTR;
		CanRxDataFIFO[CanRxFifoRamPtr].rxMsg.DLC       = msg->DLC;
		CanRxDataFIFO[CanRxFifoRamPtr].rxMsg.Data[0]   = msg->Data[0];
		CanRxDataFIFO[CanRxFifoRamPtr].rxMsg.Data[1]   = msg->Data[1];
		CanRxDataFIFO[CanRxFifoRamPtr].rxMsg.Data[2]   = msg->Data[2];
		CanRxDataFIFO[CanRxFifoRamPtr].rxMsg.Data[3]   = msg->Data[3];
		CanRxDataFIFO[CanRxFifoRamPtr].rxMsg.Data[4]   = msg->Data[4];
		CanRxDataFIFO[CanRxFifoRamPtr].rxMsg.Data[5]   = msg->Data[5];
		CanRxDataFIFO[CanRxFifoRamPtr].rxMsg.Data[6]   = msg->Data[6];
		CanRxDataFIFO[CanRxFifoRamPtr].rxMsg.Data[7]   = msg->Data[7];
		
		CanRxDataFIFO[CanRxFifoRamPtr].rxMsg.FMI       = msg->FMI;
		//------------------------------------------------------------------------
		// 将FIFO当前数据块地址压入队列
		//------------------------------------------------------------------------
		rt_Q_Add((uint32_t)(&CanRxDataFIFO[CanRxFifoRamPtr]));
		//------------------------------------------------------------------------
		// 维护FIFO区操作指针
		//------------------------------------------------------------------------
		CanRxFifoRamPtr++;
		if(CanRxFifoRamPtr >= CAN_RX_QUEUE_SIZE) CanRxFifoRamPtr = 0;
		//------------------------------------------------------------------------
		// 弹出信号量, 进行数据操作
		//------------------------------------------------------------------------
		rt_sem_release(&Can_Rx_OPSem);
	}
}


void Can_CheckRxSem(void)
{
    rt_sem_take(&Can_Rx_OPSem, RT_WAITING_FOREVER);
}


//将数据弹出FIFO
uint32_t CanQ_Pop_Rx_Msg(void)
{
    return rt_Q_Remove();
}


//获取队列状态寄存器 
uint16_t CanQ_Rx_GetFifoReg(void)
{
	return CanRxRegister;
}


//设置队列状态寄存器 
void CanQ_Rx_SetFifoReg(uint16_t val)
{
    CanRxRegister = val;
}


//返回Queue状态
void* CanQ_Rx_GetQPtr(void)
{
    return (void*)pCanRxDataQ;
}

