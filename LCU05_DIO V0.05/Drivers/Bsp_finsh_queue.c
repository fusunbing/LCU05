

#include "Bsp_finsh_queue.h"
#include "rthw.h"
#include <stdio.h>
#include <string.h>


//------------------------------------------------------------------------
// Queue���л�������С
//------------------------------------------------------------------------
#define  FINSH_QUEUE_SIZE            (512) 
//------------------------------------------------------------------------
// Queue��������С
//------------------------------------------------------------------------
#define FINSH_DRAM_SIZE              (sizeof(FINSH_DATA_RAM) * FINSH_QUEUE_SIZE)
//------------------------------------------------------------------------
// ��������̨finsh��ӡ����������ź���
//------------------------------------------------------------------------
struct rt_semaphore finsh_OPSem;
//------------------------------------------------------------------------
//
//                             ͨ�ö��й���ģ��            
//
//
//             U32 type -->       FINSH_OP_Q  <------ Write FIFO ָ������
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
// ������ϢRAM��
//------------------------------------------------------------------------
//u32 FinshQueueRam[FINSH_QUEUE_SIZE] = { 0 };
u32 *FinshQueueRam;
//------------------------------------------------------------------------
// ����������FIFO    ���QUEUE ʹ�� ʹ���ⲿRAM����ռ�
// �� mBsp_Queue_Init() �����з������ڴ�ռ�
//------------------------------------------------------------------------
PFINSH_DATA_RAM FinshDataFIFO;
//------------------------------------------------------------------------
// FIFO������ָ�� ����FIFO
//------------------------------------------------------------------------
u32 FinshFifoRamPtr = 0;
//------------------------------------------------------------------------
// ���г�ʼ��״̬�Ĵ���
//------------------------------------------------------------------------
static __IO uint16_t FinshQInitRegister = FINSH_INIT_NOT;
//------------------------------------------------------------------------
// ������״̬�Ĵ���
//------------------------------------------------------------------------
static __IO uint16_t FinshRegister = FINSH_FIFO_EMPTY_MASK;
//------------------------------------------------------------------------
// ����̨��ӡ������־
//------------------------------------------------------------------------
static uint8_t print_flag = 0;
//------------------------------------------------------------------------
// ����������
//------------------------------------------------------------------------
static void rt_Q_Add(uint32_t msg);     //����API : ѹ����� 
static uint32_t rt_Q_Remove(void);      //����API : ��������
static void rt_Q_Reset(void);           //����API : ��λ����
static void rt_Q_Init(void);            //����API : ��ʼ������


void mBsp_Finsh_Queue_Init(void)
{
    //-------------------------------------------------------
    // �����ź��� 
    //-------------------------------------------------------
    rt_err_t rtv = rt_sem_init(&finsh_OPSem, "SemFinshQ", 0, RT_IPC_FLAG_FIFO);
    //-------------------------------------------------------
    // ������л����ڴ� 
    //-------------------------------------------------------
    FinshDataFIFO = rt_malloc(FINSH_DRAM_SIZE);
    FinshQueueRam = rt_malloc(FINSH_QUEUE_SIZE * 4);
    //-------------------------------------------------------
    // ��ʼ����̬���뻺���ڴ�����
    //-------------------------------------------------------
    rt_memset((void*)FinshDataFIFO, 0, FINSH_DRAM_SIZE);
    rt_memset((void*)FinshQueueRam, 0, FINSH_QUEUE_SIZE * 4);
    //-------------------------------------------------------
    // ��ʼ��FIFO����ָ��, ��ָ�����㴦�� 
    //-------------------------------------------------------
    FinshFifoRamPtr = 0;
    //-------------------------------------------------------
    // ���г�ʼ��
    //-------------------------------------------------------
    rt_Q_Init();
    //-------------------------------------------------------
    // ���г�ʼ����ɣ���λ��ʼ��״̬�Ĵ���
    //-------------------------------------------------------
    FinshQInitRegister = FINSH_INIT_OK;
}


//����API : ѹ�����
static void rt_Q_Add(uint32_t msg)
{
    uint32_t entries;
    register rt_base_t temp;

    //����δ��ʼ����������ѹջ����
    if (FinshQInitRegister != FINSH_INIT_OK) return;

    /* lock interrupt */
    temp = rt_hw_interrupt_disable();

    if (pFinshDataQ->QEntries >= pFinshDataQ->QSize) {          /* Make sure queue is not full.                         */
        /* unlock interrupt */
        rt_hw_interrupt_enable(temp);

        FinshRegister = FINSH_FIFO_FULL_MASK;

        return;
    }

    //ѹ�����
    *pFinshDataQ->QIn++ = msg;                                  /* Insert message into queue.                           */
    //����Ϊ�´�ѹ�����ָ��ά��
    pFinshDataQ->QEntries++;                                    /* Update the nbr of entries in the queue.              */
    if (pFinshDataQ->QIn == pFinshDataQ->QEnd) {              /* Wrap IN ptr when at end of queue.                    */
        pFinshDataQ->QIn = pFinshDataQ->QStart;
    }
    //���¶���ѹ������ֵ
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


//����API : ��������
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
    //��������
    msg = *pFinshDataQ->QOut++;                                 /* Extract oldest message from the queue.               */
    //����Ϊ�´ε�������ָ��ά��
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
    //���ص���������
    return (msg);                                               /* Return message received.                             */
}


//����API : ��λ����
static  void  rt_Q_Reset(void)
{
    pFinshDataQ->QIn = pFinshDataQ->QStart;
    pFinshDataQ->QOut = pFinshDataQ->QStart;
    pFinshDataQ->QEntries = 0;
}


//����API : ��ʼ������
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
    // ��������������Ҫ��ӡ��Ϣͨ������̨��˳���ӡ����
    //----------------------------------------------------------------
    //------------------------------------------------------------
    // �Ӷ��е���FIFO�׵�ַ
    //------------------------------------------------------------
    _buffRamPtr = rt_Q_Remove();
    //------------------------------------------------------------
    // ����ָ�벻Ϊ��,���ʾ���в����ɹ�
    //------------------------------------------------------------
    if (_buffRamPtr != FINSH_QUEUE_NULL)
    {
        //--------------------------------------------------------
        // ת��ָ������: RAM�������ݿ�
        //--------------------------------------------------------
        pbufram = (FINSH_DATA_RAM *)_buffRamPtr;
        //------------------------------------------------------------ 
        // �������ݿ��������, ȷ��д���������
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
            // ת��ָ������: RAM�������ݿ�
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
            //���շ���ʱ�����Դ��ַ-Ŀ�ĵ�ַ��ID�����ݳ���
            rt_kprintf(" R, %10d, %3d - %3d, %d, %d, %d, [",
                pbufram->frameInfo.FrameTick,
                ((pcanRxRam->ExtId >> 9) & 0xFF),	//Դ��ַ
                ((pcanRxRam->ExtId >> 17) & 0xFF),	//Ŀ���ַ
                (pcanRxRam->ExtId & 0x1FF),			//���ܺ�
                ((pcanRxRam->ExtId >> 25) & 0xFF),	//���ȼ�
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
            // ת��ָ������: RAM�������ݿ�
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

            //���շ���ʱ�����Դ��ַ-Ŀ�ĵ�ַ��ID�����ݳ���
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

//�����������, ������ѹ��FIFO
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
        // ά��FIFO������ָ��
        //------------------------------------------------------------------------
        FinshFifoRamPtr++;
        if (FinshFifoRamPtr >= FINSH_QUEUE_SIZE) FinshFifoRamPtr = 0;
        rt_hw_interrupt_enable(temp);

        //------------------------------------------------------------------------
        // ����֡����
        //------------------------------------------------------------------------
        FinshDataFIFO[_curr_ptr].frameType = frameType;

        if (FinshDataFIFO[_curr_ptr].frameType == STRING_MODE_4MAX_PARAM)
        {
            //------------------------------------------------------------------------
            // ����֡��Ϣ  2014-4-18
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
            // ����֡��Ϣ
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
        // ���������ݸ�����FIFO
        //------------------------------------------------------------------------
        rt_memcpy((void*)(FinshDataFIFO[_curr_ptr].dataRam), (void*)(buf), FINSN_BUF_SIZE);
        //------------------------------------------------------------------------
        // ��FIFO��ǰ���ݿ��ַѹ�����
        //------------------------------------------------------------------------
        rt_Q_Add((u32)(&FinshDataFIFO[_curr_ptr]));

        //------------------------------------------------------------------------
        // �����ź���,ָʾ����̨��ӡ����, �������ݲ���
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

