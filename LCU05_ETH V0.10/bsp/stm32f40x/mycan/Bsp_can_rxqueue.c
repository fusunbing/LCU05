

#include "Bsp_can_QueApi.h"


//------------------------------------------------------------------------
// CAN���л�������С
//------------------------------------------------------------------------
#define  CAN_RX_QUEUE_SIZE        (60)   
//------------------------------------------------------------------------
// Queue��������С
//------------------------------------------------------------------------
#define CAN_RX_DRAM_SIZE          (sizeof(CAN_RX_DATA_RAM) * CAN_RX_QUEUE_SIZE)
//------------------------------------------------------------------------
// ��������̨finsh��ӡ����������ź���
//------------------------------------------------------------------------
struct rt_semaphore Can_Rx_OPSem;
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
static CAN_Q  CanRxdataQ;
static CAN_Q  *pCanRxDataQ = &CanRxdataQ; 
//------------------------------------------------------------------------
// ������ϢRAM��
//------------------------------------------------------------------------
static uint32_t CanRxQueueRam[CAN_RX_QUEUE_SIZE] = {0};
//------------------------------------------------------------------------
// ����������FIFO    ���QUEUE ʹ�� ʹ���ⲿRAM����ռ�
// �� mBsp_Queue_Init() �����з������ڴ�ռ�
//------------------------------------------------------------------------
//static PCAN_RX_DATA_RAM CanRxDataFIFO;
static CAN_RX_DATA_RAM CanRxDataFIFO[CAN_RX_QUEUE_SIZE] = {0};
//------------------------------------------------------------------------
// FIFO������ָ�� ����FIFO
//------------------------------------------------------------------------
static uint32_t CanRxFifoRamPtr = 0;
//------------------------------------------------------------------------
// ���г�ʼ��״̬�Ĵ���
//------------------------------------------------------------------------
static __IO uint16_t CanRxQInitRegister = CAN_INIT_NOT;
//------------------------------------------------------------------------
// ������״̬�Ĵ���
//------------------------------------------------------------------------
static __IO uint16_t CanRxRegister = CAN_FIFO_EMPTY_MASK;
//------------------------------------------------------------------------
// ����������
//------------------------------------------------------------------------
void mBsp_Can_Rx_Queue_Init(void);
void CanQ_Push_Rx_Msg(uint8_t port, uint32_t tick, CanRxMsg* msg);
void CanQ_Rx_SetFifoReg(uint16_t val);
uint16_t CanQ_Rx_GetFifoReg(void);
uint32_t CanQ_Pop_Rx_Msg(void);
//------------------------------------------------------------------------
// ����������  ģ���ڲ�����
//------------------------------------------------------------------------
static void rt_Q_Add(uint32_t msg);     //����API : ѹ����� 
static uint32_t rt_Q_Remove(void);      //����API : ��������
static void rt_Q_Reset(void);           //����API : ��λ����
static void rt_Q_Init(void);            //����API : ��ʼ������


void mBsp_Can_Rx_Queue_Init(void)
{
	//-------------------------------------------------------
	// �����ź��� 
	//-------------------------------------------------------
	rt_err_t rtv    = rt_sem_init(&Can_Rx_OPSem,  "SemCanQRx",  0,  RT_IPC_FLAG_FIFO);
	//-------------------------------------------------------
	// ������л����ڴ� תΪ��̬����ռ� [6/26/2014 -3550]
	//-------------------------------------------------------
	//CanRxDataFIFO   = rt_malloc(CAN_RX_DRAM_SIZE);
	//-------------------------------------------------------
	// ��ʼ��FIFO����ָ��, ��ָ�����㴦�� 
	//-------------------------------------------------------
	CanRxFifoRamPtr = 0;
	//-------------------------------------------------------
	// ���г�ʼ��
	//-------------------------------------------------------
	rt_Q_Init();
	//-------------------------------------------------------
	// ���г�ʼ����ɣ���λ��ʼ��״̬�Ĵ���
	//-------------------------------------------------------
	CanRxQInitRegister = CAN_INIT_OK;
}


//����API : ѹ�����
static void rt_Q_Add(uint32_t msg)
{
	uint32_t entries;
	register rt_base_t temp;

	//����δ��ʼ����������ѹջ����
	if(CanRxQInitRegister != CAN_INIT_OK) return;

	/* lock interrupt */
	temp = rt_hw_interrupt_disable();

	if (pCanRxDataQ->QEntries >= pCanRxDataQ->QSize) {          /* Make sure queue is not full.                         */
		/* unlock interrupt */
		rt_hw_interrupt_enable(temp);

		//������������λ�Ĵ���
		CanRxRegister = CAN_FIFO_FULL_MASK;
		return;
	}

	//ѹ�����
	*pCanRxDataQ->QIn++ = msg;                                  /* Insert message into queue.                           */
	//����Ϊ�´�ѹ�����ָ��ά��
	pCanRxDataQ->QEntries++;                                    /* Update the nbr of entries in the queue.              */
	if (pCanRxDataQ->QIn == pCanRxDataQ->QEnd) {              /* Wrap IN ptr when at end of queue.                    */
		pCanRxDataQ->QIn  = pCanRxDataQ->QStart;
	}
	//���¶���ѹ������ֵ
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


//����API : ��������  
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
	//��������
	msg = *pCanRxDataQ->QOut++;                                 /* Extract oldest message from the queue.               */
	//����Ϊ�´ε�������ָ��ά��
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
	//���ص���������
	return (msg);                                               /* Return message received.                             */
}


//����API : ��λ����
static void rt_Q_Reset(void)
{
	pCanRxDataQ->QIn      = pCanRxDataQ->QStart;
	pCanRxDataQ->QOut     = pCanRxDataQ->QStart;
	pCanRxDataQ->QEntries = 0;
}


//����API : ��ʼ������
static void rt_Q_Init(void)
{
	pCanRxDataQ->QStart      =  CanRxQueueRam;     /*  Initialize Finsh Q.   */
	pCanRxDataQ->QEnd        = &CanRxQueueRam[CAN_RX_QUEUE_SIZE - 1];
	pCanRxDataQ->QSize       =  CAN_RX_QUEUE_SIZE;
	pCanRxDataQ->QEntriesMax = 0;

	rt_Q_Reset();    /* Reset Rx status words queue.    */
}


//������ѹ��FIFO
void CanQ_Push_Rx_Msg(uint8_t port, uint32_t tick, CanRxMsg* msg)
{
	//-------------------------------------------------------------------
	// �ж϶����Ƿ����ˣ�û����ʱ�����Լ���ѹջ
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
		// ��FIFO��ǰ���ݿ��ַѹ�����
		//------------------------------------------------------------------------
		rt_Q_Add((uint32_t)(&CanRxDataFIFO[CanRxFifoRamPtr]));
		//------------------------------------------------------------------------
		// ά��FIFO������ָ��
		//------------------------------------------------------------------------
		CanRxFifoRamPtr++;
		if(CanRxFifoRamPtr >= CAN_RX_QUEUE_SIZE) CanRxFifoRamPtr = 0;
		//------------------------------------------------------------------------
		// �����ź���, �������ݲ���
		//------------------------------------------------------------------------
		rt_sem_release(&Can_Rx_OPSem);
	}
}


void Can_CheckRxSem(void)
{
    rt_sem_take(&Can_Rx_OPSem, RT_WAITING_FOREVER);
}


//�����ݵ���FIFO
uint32_t CanQ_Pop_Rx_Msg(void)
{
    return rt_Q_Remove();
}


//��ȡ����״̬�Ĵ��� 
uint16_t CanQ_Rx_GetFifoReg(void)
{
	return CanRxRegister;
}


//���ö���״̬�Ĵ��� 
void CanQ_Rx_SetFifoReg(uint16_t val)
{
    CanRxRegister = val;
}


//����Queue״̬
void* CanQ_Rx_GetQPtr(void)
{
    return (void*)pCanRxDataQ;
}

