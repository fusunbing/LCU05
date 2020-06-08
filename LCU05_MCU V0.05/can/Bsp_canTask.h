

#ifndef __BSP_CAN_TASK_H__
#define __BSP_CAN_TASK_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "rtthread.h"
#include "stm32f4xx.h"


void Bsp_CanInit(void);
void CanTask_Send_Event_Logic(void);
void CanTask_Send_Event_Cycle(void);

void Bsp_can_rx_Task_Init(void);
void Bsp_can_tx_Task_Init(void);
void CANx_Send(CanTxMsg* msg);

void CAN1_RX0_IRQ(void);
void CAN2_RX0_IRQ(void);


#ifdef __cplusplus
}
#endif

#endif

