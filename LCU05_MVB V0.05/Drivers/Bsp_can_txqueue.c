

#include "rtthread.h"
#include "stm32f4xx.h"
#include "rthw.h"
#include <stdio.h>
#include <string.h>
#include "Bsp_can_queue.h"

/* Private typedef -----------------------------------------------------------*/
//------------------------------------------------------------------------
// CAN队列缓冲区大小
//------------------------------------------------------------------------
#define CAN_TX_QUEUE_SIZE             (60)
//------------------------------------------------------------------------
// Queue缓冲区大小
//------------------------------------------------------------------------
#define CAN_TX_DRAM_SIZE              (sizeof(CAN_TX_DATA_RAM) * CAN_TX_QUEUE_SIZE)
//------------------------------------------------------------------------
// 创建控制台finsh打印操作任务的信号量
//------------------------------------------------------------------------
struct rt_semaphore Can_Tx_OPSem;
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
static CAN_Q  *pCanTxDataQ = &CanTxdataQ; 
//------------------------------------------------------------------------
// CAN TX消息队列RAM区
//------------------------------------------------------------------------
static uint32_t CanTxQueueRam[CAN_TX_QUEUE_SIZE] = {0};
//------------------------------------------------------------------------
// 真正的数据FIFO    配合QUEUE 使用 使用外部RAM分配空间
// 在 mBsp_Queue_Init() 函数中分配了内存空间
//------------------------------------------------------------------------
static PCAN_TX_DATA_RAM CanTxDataFIFO;
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
// 函数声明区
//------------------------------------------------------------------------
void mBsp_Can_Tx_Queue_Init(void);
void CanQ_Push_Tx_Msg(uint8_t port, uint8_t board, uint32_t tick, CanTxMsg* msg);
uint16_t CanQ_Tx_GetFifoReg(void);
void CanQ_Tx_SetFifoReg(uint16_t val);
uint32_t CanQ_Pop_Tx_Msg(void);
//------------------------------------------------------------------------
// 函数声明区  模块内部函数
//------------------------------------------------------------------------
static void rt_Q_Add(uint32_t msg);     //队列API : 压入操作 
static uint32_t rt_Q_Remove(void);      //队列API : 弹出操作
static void rt_Q_Reset(void);           //队列API : 复位操作
static void rt_Q_Init(void);            //队列API : 初始化操作

/*******************************************************************************
* Function Name  : void mBsp_Can_Queue_Init(void)
* Description    :   
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void mBsp_Can_Tx_Queue_Init(void)
{
	//-------------------------------------------------------
	// 创建信号量 
	//-------------------------------------------------------
	rt_err_t rtv    = rt_sem_init(&Can_Tx_OPSem,  "SemCanQTx",  1,  RT_IPC_FLAG_FIFO);
	//-------------------------------------------------------
	// 申请队列缓冲内存 
	//-------------------------------------------------------
	CanTxDataFIFO   = rt_malloc(CAN_TX_DRAM_SIZE);
	//-------------------------------------------------------
	// 初始化FIFO操作指针, 该指针清零处理 
	//-------------------------------------------------------
	CanTxFifoRamPtr = 0;
	//-------------------------------------------------------
	// 队列初始化
	//-------------------------------------------------------
	rt_Q_Init();
	//-------------------------------------------------------
	// 队列初始化完成，置位初始化状态寄存器
	//-------------------------------------------------------
	CanTxQInitRegister = CAN_INIT_OK;
}

/*******************************************************************************
* Function Name  : static void rt_Q_Add(uint32_t msg)
* Description    : 队列API : 压入操作  
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

static void rt_Q_Add(uint32_t msg)
{
	uint32_t entries;
	register rt_base_t temp;

	//队列未初始化，不进行压栈操作
	if(CanTxQInitRegister != CAN_INIT_OK) return;

	/* lock interrupt */
	temp = rt_hw_interrupt_disable();

	if (pCanTxDataQ->QEntries >= pCanTxDataQ->QSize) {          /* Make sure queue is not full.                         */
		/* unlock interrupt */
		rt_hw_interrupt_enable(temp);

		//队列已满，置位寄存器
		CanTxRegister = CAN_FIFO_FULL_MASK;

		return;
	}

	//压入队列
	*pCanTxDataQ->QIn++ = msg;                                  /* Insert message into queue.                           */
	//队列为下次压入进行指针维护
	pCanTxDataQ->QEntries++;                                    /* Update the nbr of entries in the queue.              */
	if (pCanTxDataQ->QIn == pCanTxDataQ->QEnd) {              /* Wrap IN ptr when at end of queue.                    */
		pCanTxDataQ->QIn  = pCanTxDataQ->QStart;
	}
	//更新队列压入的最大值
	entries = pCanTxDataQ->QEntries;
	if (pCanTxDataQ->QEntriesMax < entries) {
		pCanTxDataQ->QEntriesMax = entries;
	}

	if(pCanTxDataQ->QEntriesMax == pCanTxDataQ->QSize)
	{
		for(;;)
			break;
	}

	/* unlock interrupt */
	rt_hw_interrupt_enable(temp);
}

/*******************************************************************************
* Function Name  : static uint32_t rt_Q_Remove(void)
* Description    : 队列API : 弹出操作  
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

static uint32_t rt_Q_Remove(void)
{
	uint32_t  msg;
	register rt_base_t temp;
	/* lock interrupt */
	temp = rt_hw_interrupt_disable();

	if (pCanTxDataQ->QEntries == 0) {                           /* Chk for queue empty.                                 */
		msg  = CAN_QUEUE_NULL;
		/* unlock interrupt */
		rt_hw_interrupt_enable(temp);

		return(msg);
	}
	//弹出数据
	msg = *pCanTxDataQ->QOut++;                                 /* Extract oldest message from the queue.               */
	//队列为下次弹出进行指针维护
	pCanTxDataQ->QEntries--;                                    /* Update the number of entries in the queue            */
	if (pCanTxDataQ->QOut == pCanTxDataQ->QEnd) {             /* Wrap OUT ptr when at the end of the queue.           */
		pCanTxDataQ->QOut  = pCanTxDataQ->QStart;
	}

	/* unlock interrupt */
	rt_hw_interrupt_enable(temp);
	//返回弹出的数据
	return (msg);                                               /* Return message received.                             */
}

/*******************************************************************************
* Function Name  : static void rt_Q_Reset(void)
* Description    : 队列API : 复位操作 
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

static void rt_Q_Reset(void)
{
	pCanTxDataQ->QIn      = pCanTxDataQ->QStart;
	pCanTxDataQ->QOut     = pCanTxDataQ->QStart;
	pCanTxDataQ->QEntries = 0;
}

/*******************************************************************************
* Function Name  : static void rt_Q_Init(void)
* Description    : 队列API : 初始化操作
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

static void rt_Q_Init(void)
{
	pCanTxDataQ->QStart      =  CanTxQueueRam;     /*  Initialize Finsh Q.   */
	pCanTxDataQ->QEnd        = &CanTxQueueRam[CAN_TX_QUEUE_SIZE];
	pCanTxDataQ->QSize       =  CAN_TX_QUEUE_SIZE;
	pCanTxDataQ->QEntriesMax = 0;

	rt_Q_Reset();    /* Reset Rx status words queue.    */
}

/*******************************************************************************
* Function Name  : void CanQ_Push_Tx_Msg(uint8_t port, uint8_t card, uint32_t tick, CanTxMsg* msg)
* Description    : 将数据压入FIFO
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/

void CanQ_Push_Tx_Msg(uint8_t port, uint8_t board, uint32_t tick, CanTxMsg* msg)
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

		CanTxDataFIFO[_curr_ptr].parent.CanPort = port;
		CanTxDataFIFO[_curr_ptr].parent.Tick    = tick;
		CanTxDataFIFO[_curr_ptr].parent.BoardId = board;
		CanTxDataFIFO[_curr_ptr].data.StdId     = msg->StdId;
		CanTxDataFIFO[_curr_ptr].data.ExtId     = msg->ExtId;
		CanTxDataFIFO[_curr_ptr].data.IDE       = msg->IDE;
		CanTxDataFIFO[_curr_ptr].data.RTR       = msg->RTR;
		CanTxDataFIFO[_curr_ptr].data.DLC       = msg->DLC;
		CanTxDataFIFO[_curr_ptr].data.Data[0]   = msg->Data[0];
		CanTxDataFIFO[_curr_ptr].data.Data[1]   = msg->Data[1];
		CanTxDataFIFO[_curr_ptr].data.Data[2]   = msg->Data[2];
		CanTxDataFIFO[_curr_ptr].data.Data[3]   = msg->Data[3];
		CanTxDataFIFO[_curr_ptr].data.Data[4]   = msg->Data[4];
		CanTxDataFIFO[_curr_ptr].data.Data[5]   = msg->Data[5];
		CanTxDataFIFO[_curr_ptr].data.Data[6]   = msg->Data[6];
		CanTxDataFIFO[_curr_ptr].data.Data[7]   = msg->Data[7];
		//------------------------------------------------------------------------
		// 将FIFO当前数据块地址压入队列
		//------------------------------------------------------------------------
		rt_Q_Add((uint32_t)(&CanTxDataFIFO[_curr_ptr]));
		//------------------------------------------------------------------------
		// 弹出信号量, 进行数据操作
		//------------------------------------------------------------------------
		rt_sem_release(&Can_Tx_OPSem);
	}
}

/*******************************************************************************
* Function Name  : CanQ_Pop_Msg
* Description    : 将数据弹出FIFO
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
uint32_t CanQ_Pop_Tx_Msg(void)
{
    return rt_Q_Remove();
}

/*******************************************************************************
* Function Name  : uint16_t CanQ_Tx_GetFifoReg(void)
* Description    : 获取队列状态寄存器 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t CanQ_Tx_GetFifoReg(void)
{
	return CanTxRegister;
}

/*******************************************************************************
* Function Name  : void CanQ_SetFifoReg(uint16_t val)
* Description    : 设置队列状态寄存器 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void CanQ_Tx_SetFifoReg(uint16_t val)
{
    CanTxRegister = val;
}

