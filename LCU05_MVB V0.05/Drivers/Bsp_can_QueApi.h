

#ifndef __BSP_CAN_QUEUE_API_H__
#define __BSP_CAN_QUEUE_API_H__

#ifdef __cplusplus
    extern "C" {
#endif


#include <rthw.h>
#include <stdio.h>
#include <string.h>
#include "Bsp_can.h"


// CAN����Ϊ��
#define CAN_QUEUE_NULL                  (0)

// ���г�ʼ����־�Ĵ���
#define CAN_INIT_NOT                    (0x00)
#define CAN_INIT_OK                     (0x01)

// FIFO������־�Ĵ���
#define CAN_FIFO_EMPTY_MASK             (0x00)
#define CAN_FIFO_FULL_MASK              (0x01)


// CAN���л�������С
#define CAN_RX_QUEUE_SIZE               (60)  

// CAN���л�������С
#define CAN_TX_QUEUE_SIZE               (255)     

// Queue��������С
#define CAN_RX_DRAM_SIZE                (sizeof(CAN_RX_DATA_RAM) * CAN_RX_QUEUE_SIZE)
    
// Queue��������С
#define CAN_TX_DRAM_SIZE                (sizeof(CAN_TX_DATA_RAM) * CAN_TX_QUEUE_SIZE)

 
// Queue����ÿ����Ϣ���ݽṹ��
typedef struct
{
    uint32_t        canPort;
    uint32_t        tick;
    CanRxMsg        rxMsg;
}CAN_RX_DATA_RAM, *PCAN_RX_DATA_RAM;


typedef struct
{
    uint32_t        canPort;
    uint32_t        tick;
    CanTxMsg        txMsg;
}CAN_TX_DATA_RAM, *PCAN_TX_DATA_RAM;


// Queue����ά���ṹ��
typedef struct    
{
    uint32_t    QEntriesMax;
    uint32_t   *QStart;                          
    uint32_t   *QEnd;            /* Ptr to end   of finsh Q data                            */
    uint32_t   *QIn;             /* Ptr where next msg will be inserted  in   finsh Q       */
    uint32_t   *QOut;            /* Ptr where next msg will be extracted from finsh Q       */
    uint32_t    QSize;           /* Size of finsh Q (maximum number of entries)             */
    uint32_t    QEntries;        /* Current number of entries in the finsh Q                */

}CAN_Q;


void     mBsp_Can_Rx_Queue_Init(void);
void     CanQ_Push_Rx_Msg(uint32_t port, uint32_t tick, CanRxMsg* msg);
uint16_t CanQ_Rx_GetFifoReg(void);
void     CanQ_Rx_SetFifoReg(uint16_t val);
uint32_t CanQ_Pop_Rx_Msg(void);
void     Can_CheckRxSem(void);

void     mBsp_Can1_Tx_Queue_Init(void);
uint8_t  Can1Q_Push_Tx_Msg(uint32_t port, uint32_t tick, CanTxMsg* msg);
void     Can1Q_Tx_SetEmpty(void);
uint16_t Can1Q_Tx_IsFull(void);
uint32_t Can1Q_Pop_Tx_Msg(void);
void     Can1_CheckTxSem(void);


void     mBsp_Can2_Tx_Queue_Init(void);
uint8_t  Can2Q_Push_Tx_Msg(uint32_t port, uint32_t tick, CanTxMsg* msg);
void     Can2Q_Tx_SetEmpty(void);
uint16_t Can2Q_Tx_IsFull(void);
uint32_t Can2Q_Pop_Tx_Msg(void);
void     Can2_CheckTxSem(void);


#ifdef __cplusplus
}
#endif

#endif 

