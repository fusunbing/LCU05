

#include "Bsp_can_QueApi.h"


//------------------------------------------------------------------------
//
//                             ͨ�ö��й���ģ��            
//
//
//             uint32_t type -->   CAN_OP_Q  <------ Write FIFO ָ������
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
// ����Tx�ź���
//------------------------------------------------------------------------
struct rt_semaphore Can2_Tx_OPSem;

//------------------------------------------------------------------------
// CAN TX��Ϣ����RAM��
//------------------------------------------------------------------------
static uint32_t CanTxQueueRam[CAN_TX_QUEUE_SIZE] = {0};
//------------------------------------------------------------------------
// ����������FIFO    ���QUEUE ʹ�� ʹ���ⲿRAM����ռ�
// �� mBsp_Queue_Init() �����з������ڴ�ռ�
//------------------------------------------------------------------------
static CAN_TX_DATA_RAM CanTxDataFIFO[CAN_TX_QUEUE_SIZE] = {0};
//------------------------------------------------------------------------
// FIFO������ָ�� ����FIFO
//------------------------------------------------------------------------
static uint32_t CanTxFifoRamPtr = 0;
//------------------------------------------------------------------------
// ���г�ʼ��״̬�Ĵ���
//------------------------------------------------------------------------
static __IO uint16_t CanTxQInitRegister = CAN_INIT_NOT;
//------------------------------------------------------------------------
// ������״̬�Ĵ���
//------------------------------------------------------------------------
static __IO uint16_t CanTxRegister = CAN_FIFO_EMPTY_MASK;

//------------------------------------------------------------------------
// ����������  ģ���ڲ�����
//------------------------------------------------------------------------
static void     rt_Q_Add(uint32_t msg);     //����API : ѹ����� 
static uint32_t rt_Q_Remove(void);          //����API : ��������
static void     rt_Q_Reset(void);           //����API : ��λ����
static void     rt_Q_Init(void);            //����API : ��ʼ������


void mBsp_Can2_Tx_Queue_Init(void)
{
	// �����ź��� 
	rt_err_t rtv = rt_sem_init(&Can2_Tx_OPSem,  "SQTCan2",  0,  RT_IPC_FLAG_FIFO);

	// ��ʼ��FIFO����ָ��, ��ָ�����㴦�� 
	CanTxFifoRamPtr = 0;
    
	// ���г�ʼ��
	rt_Q_Init();
    
	// ���г�ʼ����ɣ���λ��ʼ��״̬�Ĵ���
	CanTxQInitRegister = CAN_INIT_OK;
}


static void rt_Q_Add(uint32_t msg)
{
	uint32_t entries;
	register rt_base_t temp;

	//����δ��ʼ����������ѹջ����
	if(CanTxQInitRegister != CAN_INIT_OK) return;

	/* lock interrupt */
	temp = rt_hw_interrupt_disable();

	if (pTQ->QEntries >= pTQ->QSize) {          /* Make sure queue is not full.                         */
		/* unlock interrupt */
		rt_hw_interrupt_enable(temp);

		//������������λ�Ĵ���
		CanTxRegister = CAN_FIFO_FULL_MASK;

		return;
	}

	//ѹ�����
	*pTQ->QIn++ = msg;                                  /* Insert message into queue.                           */
	//����Ϊ�´�ѹ�����ָ��ά��
	pTQ->QEntries++;                                    /* Update the nbr of entries in the queue.              */
	if (pTQ->QEntries >= pTQ->QSize)
	{
		//������������λ�Ĵ��� 2014-1-2
		CanTxRegister = CAN_FIFO_FULL_MASK;
	}
	
	if (pTQ->QIn == pTQ->QEnd) {              /* Wrap IN ptr when at end of queue.                    */
		pTQ->QIn  = pTQ->QStart;
	}
	//���¶���ѹ������ֵ
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
	//��������
	msg = *pTQ->QOut++;                                 /* Extract oldest message from the queue.               */
	//����Ϊ�´ε�������ָ��ά��
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
	//���ص���������
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
	// �ж϶����Ƿ����ˣ�û����ʱ�����Լ���ѹջ
	//-------------------------------------------------------------------
	if(CanTxRegister != CAN_FIFO_FULL_MASK)
	{
        //------------------------------------------------------------------------
        // Bug Fix Code 2013-12-16
        //------------------------------------------------------------------------
        temp = rt_hw_interrupt_disable();
        _curr_ptr = CanTxFifoRamPtr;
		//------------------------------------------------------------------------
		// ά��FIFO������ָ��
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
		// ��FIFO��ǰ���ݿ��ַѹ�����
		//------------------------------------------------------------------------
		rt_Q_Add((uint32_t)(&CanTxDataFIFO[_curr_ptr]));
		//------------------------------------------------------------------------
		// �����ź���, �������ݲ���
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

