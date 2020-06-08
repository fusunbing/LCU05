

#ifndef __BSP_CAN_QUEUE_API_H__
#define __BSP_CAN_QUEUE_API_H__

#ifdef __cplusplus
    extern "C" {
#endif

        
#include "Bsp_can_queue.h"

			
//------------------------------------------------------------------------
// CAN队列缓冲区大小
//------------------------------------------------------------------------
#define CAN_TX_QUEUE_SIZE             (255)

//------------------------------------------------------------------------
// Queue缓冲区大小
//------------------------------------------------------------------------
#define CAN_TX_DRAM_SIZE              (sizeof(CAN_TX_DATA_RAM) * CAN_TX_QUEUE_SIZE)


void     mBsp_Can_Rx_Queue_Init(void);
void     CanQ_Push_Rx_Msg(uint8_t port, uint32_t tick, CanRxMsg* msg);
uint16_t CanQ_Rx_GetFifoReg(void);
void     CanQ_Rx_SetFifoReg(uint16_t val);
uint32_t CanQ_Pop_Rx_Msg(void);
void     Can_CheckRxSem(void);

void     mBsp_Can1_Tx_Queue_Init(void);
uint8_t  Can1Q_Push_Tx_Msg(uint8_t port, uint8_t board, uint32_t tick, CanTxMsg* msg);
void     Can1Q_Tx_SetEmpty(void);
uint16_t Can1Q_Tx_IsFull(void);
uint32_t Can1Q_Pop_Tx_Msg(void);
void     Can1_CheckTxSem(void);


void     mBsp_Can2_Tx_Queue_Init(void);
uint8_t  Can2Q_Push_Tx_Msg(uint8_t port, uint8_t board, uint32_t tick, CanTxMsg* msg);
void     Can2Q_Tx_SetEmpty(void);
uint16_t Can2Q_Tx_IsFull(void);
uint32_t Can2Q_Pop_Tx_Msg(void);
void     Can2_CheckTxSem(void);


#ifdef __cplusplus
}
#endif

#endif 

