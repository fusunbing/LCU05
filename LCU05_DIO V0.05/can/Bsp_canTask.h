

#ifndef __BSP_CANTASK_H__
#define __BSP_CANTASK_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "rtthread.h"
#include "stm32f4xx.h"


void Bsp_canTask_Init(void);

void Can_Send_Cycle(void);
void Can_Send_Event(void);

void Bsp_can_rx_Task_Init(void);
void Bsp_can_tx_Task_Init(void);
void CANx_Send(CanTxMsg* msg);

void CAN1_RX0_IRQ(void);
void CAN2_RX0_IRQ(void);


#ifdef __cplusplus
}
#endif

#endif

