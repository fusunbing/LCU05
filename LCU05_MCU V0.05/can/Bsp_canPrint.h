

#ifndef __BSP_CAN_PRINT_H
#define __BSP_CAN_PRINT_H

#ifdef __cplusplus
extern "C" {
#endif 


#include "rtthread.h"
#include "stm32f4xx.h"


void Can_RX_Print(CanRxMsg* pMsg, uint32_t port);
void Can_TX_Print(CanTxMsg* pMsg, uint32_t port);


#ifdef __cplusplus
}
#endif

#endif

