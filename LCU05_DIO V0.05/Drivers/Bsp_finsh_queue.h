

#ifndef __BSP_FINSH_QUEUE_H__
#define __BSP_FINSH_QUEUE_H__

#ifdef __cplusplus
    extern "C" {
#endif


#include "stm32f4xx.h"
#include "rtthread.h"


#define  RS485_MASTER                (1)
#define  RS485_SLAVE                 (2)
#define  PWR_MASTER                  (3)
#define  PWR_SLAVE                   (4)
#define  CAN_TX                      (5)
#define  CAN_RX                      (6)
#define  STRING_MODE_4MAX_PARAM      (7)

//------------------------------------------------------------------------
// ���г�ʼ����־�Ĵ���
//------------------------------------------------------------------------
#define FINSH_INIT_NOT               (0x00)
#define FINSH_INIT_OK                (0x01)

//------------------------------------------------------------------------
// FIFO������־�Ĵ���
//------------------------------------------------------------------------
#define FINSH_FIFO_EMPTY_MASK        (0x00)
#define FINSH_FIFO_FULL_MASK         (0x01)

//------------------------------------------------------------------------
// Queue����Ϊ��
//------------------------------------------------------------------------
#define  FINSH_QUEUE_NULL            (0)

//------------------------------------------------------------------------
// Queue������ÿ����Ϣ���ݳ���
//------------------------------------------------------------------------
#define  FINSN_BUF_SIZE              (32)

typedef struct _Finsh_Frame_Info_
{
	uint32_t  FrameTick;   //rt_tick
	uint32_t  FrameID;     //֡���ݱ��
	uint32_t  Port;        //�˿���Ϣ
	uint32_t  BoardId;     //ͨѶ��ID
	uint32_t  DataSize;    //��ӡ���ݴ�С
    //2014-4-18 
    const char* fmt;       //print string format table pointer.
    uint32_t  val1;        //param 1
    uint32_t  val2;        //param 2
    uint32_t  val3;        //param 3
    uint32_t  val4;        //param 4
}FINSH_FRAME_INFO, *PFINSH_FRAME_INFO;

//------------------------------------------------------------------------
// Queue����ά���ṹ��
//------------------------------------------------------------------------
typedef  struct  _finsh_queue_    
{
	uint32_t  QEntriesMax;
	uint32_t *QStart;                          
	uint32_t *QEnd;                 /* Ptr to end   of finsh Q data                       */
	uint32_t *QIn;                  /* Ptr where next msg will be inserted  in   finsh Q  */
	uint32_t *QOut;                 /* Ptr where next msg will be extracted from finsh Q  */
	uint32_t  QSize;                /* Size of finsh Q (maximum number of entries)        */
	uint32_t  QEntries;             /* Current number of entries in the finsh Q           */

} FINSH_Q;

//------------------------------------------------------------------------
// Queue����ÿ����Ϣ���ݽṹ��
//------------------------------------------------------------------------
typedef struct _Finsh_Ram_
{
	uint32_t          frameType;                //��������
	FINSH_FRAME_INFO  frameInfo;                //����֡��Ϣ
	uint8_t           dataRam[FINSN_BUF_SIZE];  //����

}FINSH_DATA_RAM, *PFINSH_DATA_RAM;




void mBsp_Finsh_Queue_Init(void);
void mBsp_Finsh_Queue_Print(void);
void FinshQ_Push_Data(uint32_t frameType, PFINSH_FRAME_INFO pframeInfo, uint8_t *buf);
uint16_t FinshQ_GetFifoReg(void);
void FinshQ_SetFifoReg(uint16_t val);



#ifdef __cplusplus
}
#endif

#endif 

