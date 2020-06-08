

#include "rtthread.h"
#include "stm32f4xx.h"
#include "rthw.h"
#include <stdio.h>
#include <string.h>
#include "Bsp_can_queue.h"

/* Private typedef -----------------------------------------------------------*/
//------------------------------------------------------------------------
// CAN���л�������С
//------------------------------------------------------------------------
#define CAN_TX_QUEUE_SIZE             (60)
//------------------------------------------------------------------------
// Queue��������С
//------------------------------------------------------------------------
#define CAN_TX_DRAM_SIZE              (sizeof(CAN_TX_DATA_RAM) * CAN_TX_QUEUE_SIZE)
//------------------------------------------------------------------------
// ��������̨finsh��ӡ����������ź���
//------------------------------------------------------------------------
struct rt_semaphore Can_Tx_OPSem;
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
static CAN_Q  *pCanTxDataQ = &CanTxdataQ; 
//------------------------------------------------------------------------
// CAN TX��Ϣ����RAM��
//------------------------------------------------------------------------
static uint32_t CanTxQueueRam[CAN_TX_QUEUE_SIZE] = {0};
//------------------------------------------------------------------------
// ����������FIFO    ���QUEUE ʹ�� ʹ���ⲿRAM����ռ�
// �� mBsp_Queue_Init() �����з������ڴ�ռ�
//------------------------------------------------------------------------
static PCAN_TX_DATA_RAM CanTxDataFIFO;
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
// ����������
//------------------------------------------------------------------------
void mBsp_Can_Tx_Queue_Init(void);
void CanQ_Push_Tx_Msg(uint8_t port, uint8_t board, uint32_t tick, CanTxMsg* msg);
uint16_t CanQ_Tx_GetFifoReg(void);
void CanQ_Tx_SetFifoReg(uint16_t val);
uint32_t CanQ_Pop_Tx_Msg(void);
//------------------------------------------------------------------------
// ����������  ģ���ڲ�����
//------------------------------------------------------------------------
static void rt_Q_Add(uint32_t msg);     //����API : ѹ����� 
static uint32_t rt_Q_Remove(void);      //����API : ��������
static void rt_Q_Reset(void);           //����API : ��λ����
static void rt_Q_Init(void);            //����API : ��ʼ������

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
	// �����ź��� 
	//-------------------------------------------------------
	rt_err_t rtv    = rt_sem_init(&Can_Tx_OPSem,  "SemCanQTx",  1,  RT_IPC_FLAG_FIFO);
	//-------------------------------------------------------
	// ������л����ڴ� 
	//-------------------------------------------------------
	CanTxDataFIFO   = rt_malloc(CAN_TX_DRAM_SIZE);
	//-------------------------------------------------------
	// ��ʼ��FIFO����ָ��, ��ָ�����㴦�� 
	//-------------------------------------------------------
	CanTxFifoRamPtr = 0;
	//-------------------------------------------------------
	// ���г�ʼ��
	//-------------------------------------------------------
	rt_Q_Init();
	//-------------------------------------------------------
	// ���г�ʼ����ɣ���λ��ʼ��״̬�Ĵ���
	//-------------------------------------------------------
	CanTxQInitRegister = CAN_INIT_OK;
}

/*******************************************************************************
* Function Name  : static void rt_Q_Add(uint32_t msg)
* Description    : ����API : ѹ�����  
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

	//����δ��ʼ����������ѹջ����
	if(CanTxQInitRegister != CAN_INIT_OK) return;

	/* lock interrupt */
	temp = rt_hw_interrupt_disable();

	if (pCanTxDataQ->QEntries >= pCanTxDataQ->QSize) {          /* Make sure queue is not full.                         */
		/* unlock interrupt */
		rt_hw_interrupt_enable(temp);

		//������������λ�Ĵ���
		CanTxRegister = CAN_FIFO_FULL_MASK;

		return;
	}

	//ѹ�����
	*pCanTxDataQ->QIn++ = msg;                                  /* Insert message into queue.                           */
	//����Ϊ�´�ѹ�����ָ��ά��
	pCanTxDataQ->QEntries++;                                    /* Update the nbr of entries in the queue.              */
	if (pCanTxDataQ->QIn == pCanTxDataQ->QEnd) {              /* Wrap IN ptr when at end of queue.                    */
		pCanTxDataQ->QIn  = pCanTxDataQ->QStart;
	}
	//���¶���ѹ������ֵ
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
* Description    : ����API : ��������  
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
	//��������
	msg = *pCanTxDataQ->QOut++;                                 /* Extract oldest message from the queue.               */
	//����Ϊ�´ε�������ָ��ά��
	pCanTxDataQ->QEntries--;                                    /* Update the number of entries in the queue            */
	if (pCanTxDataQ->QOut == pCanTxDataQ->QEnd) {             /* Wrap OUT ptr when at the end of the queue.           */
		pCanTxDataQ->QOut  = pCanTxDataQ->QStart;
	}

	/* unlock interrupt */
	rt_hw_interrupt_enable(temp);
	//���ص���������
	return (msg);                                               /* Return message received.                             */
}

/*******************************************************************************
* Function Name  : static void rt_Q_Reset(void)
* Description    : ����API : ��λ���� 
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
* Description    : ����API : ��ʼ������
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
* Description    : ������ѹ��FIFO
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/

void CanQ_Push_Tx_Msg(uint8_t port, uint8_t board, uint32_t tick, CanTxMsg* msg)
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
		// ��FIFO��ǰ���ݿ��ַѹ�����
		//------------------------------------------------------------------------
		rt_Q_Add((uint32_t)(&CanTxDataFIFO[_curr_ptr]));
		//------------------------------------------------------------------------
		// �����ź���, �������ݲ���
		//------------------------------------------------------------------------
		rt_sem_release(&Can_Tx_OPSem);
	}
}

/*******************************************************************************
* Function Name  : CanQ_Pop_Msg
* Description    : �����ݵ���FIFO
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
* Description    : ��ȡ����״̬�Ĵ��� 
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
* Description    : ���ö���״̬�Ĵ��� 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void CanQ_Tx_SetFifoReg(uint16_t val)
{
    CanTxRegister = val;
}

