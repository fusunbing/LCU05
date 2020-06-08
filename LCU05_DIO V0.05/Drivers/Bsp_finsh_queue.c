

#include "Bsp_finsh_queue.h"
#include "rthw.h"
#include <stdio.h>
#include <string.h>


//------------------------------------------------------------------------
// Queue队列缓冲区大小
//------------------------------------------------------------------------
#define  FINSH_QUEUE_SIZE            (512) 
//------------------------------------------------------------------------
// Queue缓冲区大小
//------------------------------------------------------------------------
#define FINSH_DRAM_SIZE              (sizeof(FINSH_DATA_RAM) * FINSH_QUEUE_SIZE)
//------------------------------------------------------------------------
// 创建控制台finsh打印操作任务的信号量
//------------------------------------------------------------------------
struct rt_semaphore finsh_OPSem;
//------------------------------------------------------------------------
//
//                             通用队列管理模块            
//
//
//             U32 type -->       FINSH_OP_Q  <------ Write FIFO 指针存放区
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
FINSH_Q  FinshDataQ;
FINSH_Q  *pFinshDataQ = &FinshDataQ;
//------------------------------------------------------------------------
// 队列消息RAM区
//------------------------------------------------------------------------
//u32 FinshQueueRam[FINSH_QUEUE_SIZE] = { 0 };
u32 *FinshQueueRam;
//------------------------------------------------------------------------
// 真正的数据FIFO    配合QUEUE 使用 使用外部RAM分配空间
// 在 mBsp_Queue_Init() 函数中分配了内存空间
//------------------------------------------------------------------------
PFINSH_DATA_RAM FinshDataFIFO;
//------------------------------------------------------------------------
// FIFO操作用指针 环形FIFO
//------------------------------------------------------------------------
u32 FinshFifoRamPtr = 0;
//------------------------------------------------------------------------
// 队列初始化状态寄存器
//------------------------------------------------------------------------
static __IO uint16_t FinshQInitRegister = FINSH_INIT_NOT;
//------------------------------------------------------------------------
// 缓冲区状态寄存器
//------------------------------------------------------------------------
static __IO uint16_t FinshRegister = FINSH_FIFO_EMPTY_MASK;
//------------------------------------------------------------------------
// 控制台打印开启标志
//------------------------------------------------------------------------
static uint8_t print_flag = 0;
//------------------------------------------------------------------------
// 函数声明区
//------------------------------------------------------------------------
static void rt_Q_Add(uint32_t msg);     //队列API : 压入操作 
static uint32_t rt_Q_Remove(void);      //队列API : 弹出操作
static void rt_Q_Reset(void);           //队列API : 复位操作
static void rt_Q_Init(void);            //队列API : 初始化操作


void mBsp_Finsh_Queue_Init(void)
{
    //-------------------------------------------------------
    // 创建信号量 
    //-------------------------------------------------------
    rt_err_t rtv = rt_sem_init(&finsh_OPSem, "SemFinshQ", 0, RT_IPC_FLAG_FIFO);
    //-------------------------------------------------------
    // 申请队列缓冲内存 
    //-------------------------------------------------------
    FinshDataFIFO = rt_malloc(FINSH_DRAM_SIZE);
    FinshQueueRam = rt_malloc(FINSH_QUEUE_SIZE * 4);
    //-------------------------------------------------------
    // 初始化动态申请缓冲内存数据
    //-------------------------------------------------------
    rt_memset((void*)FinshDataFIFO, 0, FINSH_DRAM_SIZE);
    rt_memset((void*)FinshQueueRam, 0, FINSH_QUEUE_SIZE * 4);
    //-------------------------------------------------------
    // 初始化FIFO操作指针, 该指针清零处理 
    //-------------------------------------------------------
    FinshFifoRamPtr = 0;
    //-------------------------------------------------------
    // 队列初始化
    //-------------------------------------------------------
    rt_Q_Init();
    //-------------------------------------------------------
    // 队列初始化完成，置位初始化状态寄存器
    //-------------------------------------------------------
    FinshQInitRegister = FINSH_INIT_OK;
}


//队列API : 压入操作
static void rt_Q_Add(uint32_t msg)
{
    uint32_t entries;
    register rt_base_t temp;

    //队列未初始化，不进行压栈操作
    if (FinshQInitRegister != FINSH_INIT_OK) return;

    /* lock interrupt */
    temp = rt_hw_interrupt_disable();

    if (pFinshDataQ->QEntries >= pFinshDataQ->QSize) {          /* Make sure queue is not full.                         */
        /* unlock interrupt */
        rt_hw_interrupt_enable(temp);

        FinshRegister = FINSH_FIFO_FULL_MASK;

        return;
    }

    //压入队列
    *pFinshDataQ->QIn++ = msg;                                  /* Insert message into queue.                           */
    //队列为下次压入进行指针维护
    pFinshDataQ->QEntries++;                                    /* Update the nbr of entries in the queue.              */
    if (pFinshDataQ->QIn == pFinshDataQ->QEnd) {              /* Wrap IN ptr when at end of queue.                    */
        pFinshDataQ->QIn = pFinshDataQ->QStart;
    }
    //更新队列压入的最大值
    entries = pFinshDataQ->QEntries;
    if (pFinshDataQ->QEntriesMax < entries) {
        pFinshDataQ->QEntriesMax = entries;
    }

    if (pFinshDataQ->QEntriesMax == pFinshDataQ->QSize)
    {
        for (;;)
            break;
    }

    /* unlock interrupt */
    rt_hw_interrupt_enable(temp);
}


//队列API : 弹出操作
static uint32_t rt_Q_Remove(void)
{
    uint32_t msg;
    register rt_base_t temp;

    /* lock interrupt */
    temp = rt_hw_interrupt_disable();

    if (pFinshDataQ->QEntries == 0) {                           /* Chk for queue empty.                                 */
        msg = FINSH_QUEUE_NULL;
        /* unlock interrupt */
        rt_hw_interrupt_enable(temp);

        return(msg);
    }
    //弹出数据
    msg = *pFinshDataQ->QOut++;                                 /* Extract oldest message from the queue.               */
    //队列为下次弹出进行指针维护
    pFinshDataQ->QEntries--;                                    /* Update the number of entries in the queue            */
    if (pFinshDataQ->QOut == pFinshDataQ->QEnd) {             /* Wrap OUT ptr when at the end of the queue.           */
        pFinshDataQ->QOut = pFinshDataQ->QStart;
    }
    //  [5/4/2015 ]
    if (pFinshDataQ->QEntries == 0)
    {
        FinshRegister = FINSH_FIFO_EMPTY_MASK;
    }
    /* unlock interrupt */
    rt_hw_interrupt_enable(temp);
    //返回弹出的数据
    return (msg);                                               /* Return message received.                             */
}


//队列API : 复位操作
static  void  rt_Q_Reset(void)
{
    pFinshDataQ->QIn = pFinshDataQ->QStart;
    pFinshDataQ->QOut = pFinshDataQ->QStart;
    pFinshDataQ->QEntries = 0;
}


//队列API : 初始化操作
static  void  rt_Q_Init(void)
{
    pFinshDataQ->QStart = FinshQueueRam;     /*  Initialize Finsh Q.   */
    pFinshDataQ->QEnd = &FinshQueueRam[FINSH_QUEUE_SIZE - 1];
    pFinshDataQ->QSize = FINSH_QUEUE_SIZE;
    pFinshDataQ->QEntriesMax = 0;

    rt_Q_Reset();    /* Reset Rx status words queue.    */
}


FINSH_DATA_RAM * pbufram;
CanRxMsg *pcanRxRam;
CanTxMsg *pcanTxRam;

void  mBsp_Finsh_Queue_Print(void)
{
    uint32_t _buffRamPtr;
    uint32_t temp = 0;
    uint32_t _size = 0;
    //uint8_t  buff[FINSN_BUF_SIZE] = {0};
    //----------------------------------------------------------------
    // 将队列中所有需要打印信息通过控制台按顺序打印出来
    //----------------------------------------------------------------
    //------------------------------------------------------------
    // 从队列弹出FIFO首地址
    //------------------------------------------------------------
    _buffRamPtr = rt_Q_Remove();
    //------------------------------------------------------------
    // 弹出指针不为空,则表示队列操作成功
    //------------------------------------------------------------
    if (_buffRamPtr != FINSH_QUEUE_NULL)
    {
        //--------------------------------------------------------
        // 转换指针类型: RAM操作数据块
        //--------------------------------------------------------
        pbufram = (FINSH_DATA_RAM *)_buffRamPtr;
        //------------------------------------------------------------ 
        // 根据数据块操作类型, 确定写入操作类型
        //------------------------------------------------------------
        switch (pbufram->frameType)
        {
        case RS485_SLAVE:
            break;

        case RS485_MASTER:
            break;

        case PWR_SLAVE:
            break;

        case PWR_MASTER:
            break;

        case CAN_RX:

            //--------------------------------------------------------
            // 转换指针类型: RAM操作数据块
            //--------------------------------------------------------
            pcanRxRam = (CanRxMsg *)pbufram->dataRam;
            _size = pbufram->frameInfo.DataSize;
            if (_size >= 8)  _size = 8;

            if (pbufram->frameInfo.Port == 1)
            {
                rt_kprintf("CAN1,");
            }
            else if (pbufram->frameInfo.Port == 2)
            {
                rt_kprintf("CAN2,");
            }

            /*_id->Function      = _extid & 0x1FF;
            _id->Source_Addr   = (_extid >>  9) & 0xFF;
            _id->Target_Addr   = (_extid >> 17) & 0xFF;
            _id->Priority      = (_extid >> 25) & 0x0F;*/
            //接收方向，时间戳，源地址-目的地址，ID，数据长度
            rt_kprintf(" R, %10d, %3d - %3d, %d, %d, %d, [",
                pbufram->frameInfo.FrameTick,
                ((pcanRxRam->ExtId >> 9) & 0xFF),	//源地址
                ((pcanRxRam->ExtId >> 17) & 0xFF),	//目标地址
                (pcanRxRam->ExtId & 0x1FF),			//功能号
                ((pcanRxRam->ExtId >> 25) & 0xFF),	//优先级
                pcanRxRam->DLC);
/*
			rt_kprintf(" R, %10d, %3d - %3d, %8x, %d, [",
                pbufram->frameInfo.FrameTick,
                ((pcanRxRam->ExtId >> 9) & 0xFF),
                ((pcanRxRam->ExtId >> 17) & 0xFF),
                pcanRxRam->ExtId,
                pcanRxRam->DLC);	
*/
            for (temp = 0; temp < _size; temp++)
            {
                rt_kprintf(" %2x,", pcanRxRam->Data[temp]);
            }
            rt_kprintf("]\r\n");

            break;

        case CAN_TX:

            //--------------------------------------------------------
            // 转换指针类型: RAM操作数据块
            //--------------------------------------------------------
            pcanTxRam = (CanTxMsg *)pbufram->dataRam;
            _size = pbufram->frameInfo.DataSize;
            if (_size >= 8)  _size = 8;

            if (pbufram->frameInfo.Port == 1)
            {
                rt_kprintf("CAN1,");
            }
            else if (pbufram->frameInfo.Port == 2)
            {
                rt_kprintf("CAN2,");
            }

            //接收方向，时间戳，源地址-目的地址，ID，数据长度
            rt_kprintf(" T, %10d, %3d - %3d, %8x, %d, [",
                pbufram->frameInfo.FrameTick,
                ((pcanTxRam->ExtId >> 9) & 0xFF),
                ((pcanTxRam->ExtId >> 17) & 0xFF),
                pcanTxRam->ExtId,
                pcanTxRam->DLC);

            for (temp = 0; temp < _size; temp++)
            {
                rt_kprintf(" %2x,", pcanTxRam->Data[temp]);
            }
            rt_kprintf("]\r\n");

            break;

        case STRING_MODE_4MAX_PARAM:
            switch (pbufram->frameInfo.DataSize)
            {
            case 0:
                rt_kprintf(pbufram->frameInfo.fmt);
                break;
            case 1:
                rt_kprintf(pbufram->frameInfo.fmt, pbufram->frameInfo.val1);
                break;
            case 2:
                rt_kprintf(pbufram->frameInfo.fmt, pbufram->frameInfo.val1, pbufram->frameInfo.val2);
                break;
            case 3:
                rt_kprintf(pbufram->frameInfo.fmt, pbufram->frameInfo.val1, pbufram->frameInfo.val2
                    , pbufram->frameInfo.val3);
                break;
            case 4:
                rt_kprintf(pbufram->frameInfo.fmt, pbufram->frameInfo.val1, pbufram->frameInfo.val2
                    , pbufram->frameInfo.val3, (char *)(pbufram->frameInfo.val4));
                break;
            }
            rt_kprintf("\r\n");
            break;
        default:
            break;
        }
    }
}

//处理接收数据, 将数据压入FIFO
void FinshQ_Push_Data(uint32_t frameType, PFINSH_FRAME_INFO pframeInfo, uint8_t *buf)
{
    uint32_t _curr_ptr;
    register rt_base_t temp;

    if ((FinshRegister != FINSH_FIFO_FULL_MASK) && !print_flag)
    {
        //------------------------------------------------------------------------
        // Bug Fix Code
        //------------------------------------------------------------------------
        temp = rt_hw_interrupt_disable();
        _curr_ptr = FinshFifoRamPtr;
        //------------------------------------------------------------------------
        // 维护FIFO区操作指针
        //------------------------------------------------------------------------
        FinshFifoRamPtr++;
        if (FinshFifoRamPtr >= FINSH_QUEUE_SIZE) FinshFifoRamPtr = 0;
        rt_hw_interrupt_enable(temp);

        //------------------------------------------------------------------------
        // 数据帧类型
        //------------------------------------------------------------------------
        FinshDataFIFO[_curr_ptr].frameType = frameType;

        if (FinshDataFIFO[_curr_ptr].frameType == STRING_MODE_4MAX_PARAM)
        {
            //------------------------------------------------------------------------
            // 数据帧信息  2014-4-18
            //------------------------------------------------------------------------
            FinshDataFIFO[_curr_ptr].frameInfo.FrameTick = pframeInfo->FrameTick;
            FinshDataFIFO[_curr_ptr].frameInfo.FrameID = pframeInfo->FrameID;
            FinshDataFIFO[_curr_ptr].frameInfo.Port = pframeInfo->Port;
            FinshDataFIFO[_curr_ptr].frameInfo.DataSize = pframeInfo->DataSize;
            FinshDataFIFO[_curr_ptr].frameInfo.BoardId = pframeInfo->BoardId;
            FinshDataFIFO[_curr_ptr].frameInfo.fmt = pframeInfo->fmt;
            FinshDataFIFO[_curr_ptr].frameInfo.val1 = pframeInfo->val1;
            FinshDataFIFO[_curr_ptr].frameInfo.val2 = pframeInfo->val2;
            FinshDataFIFO[_curr_ptr].frameInfo.val3 = pframeInfo->val3;
            FinshDataFIFO[_curr_ptr].frameInfo.val4 = pframeInfo->val4;

        }
        else
        {
            //------------------------------------------------------------------------
            // 数据帧信息
            //------------------------------------------------------------------------
            FinshDataFIFO[_curr_ptr].frameInfo.FrameTick = pframeInfo->FrameTick;
            FinshDataFIFO[_curr_ptr].frameInfo.FrameID = pframeInfo->FrameID;
            FinshDataFIFO[_curr_ptr].frameInfo.Port = pframeInfo->Port;
            FinshDataFIFO[_curr_ptr].frameInfo.DataSize = pframeInfo->DataSize;
            FinshDataFIFO[_curr_ptr].frameInfo.BoardId = pframeInfo->BoardId;
            FinshDataFIFO[_curr_ptr].frameInfo.fmt = " ";
            FinshDataFIFO[_curr_ptr].frameInfo.val1 = 0;
            FinshDataFIFO[_curr_ptr].frameInfo.val2 = 0;
            FinshDataFIFO[_curr_ptr].frameInfo.val3 = 0;
            FinshDataFIFO[_curr_ptr].frameInfo.val4 = 0;
        }
        //------------------------------------------------------------------------
        // 缓冲区数据更新入FIFO
        //------------------------------------------------------------------------
        rt_memcpy((void*)(FinshDataFIFO[_curr_ptr].dataRam), (void*)(buf), FINSN_BUF_SIZE);
        //------------------------------------------------------------------------
        // 将FIFO当前数据块地址压入队列
        //------------------------------------------------------------------------
        rt_Q_Add((u32)(&FinshDataFIFO[_curr_ptr]));

        //------------------------------------------------------------------------
        // 弹出信号量,指示控制台打印任务, 进行数据操作
        //------------------------------------------------------------------------
        rt_sem_release(&finsh_OPSem);
    }
}


void finsh_q(void)
{
    if (print_flag)
        print_flag = 0;
    else
        print_flag = 1;
}

#include "finsh.h"
FINSH_FUNCTION_EXPORT(finsh_q, finsh queue on / off)

