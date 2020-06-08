

#ifndef __CAN_FAULT_DETECT_H
#define __CAN_FAULT_DETECT_H

#ifdef __cplusplus
extern "C" {
#endif 


#include "stm32f4xx.h"
#include "rtthread.h"


void Can_Nodes_Flt_Clear(uint8_t port, uint8_t src);
void Can_Nodes_Flt_Update(void);
uint8_t Get_Can_Nodes_Flt(uint8_t port, uint8_t index);

#ifdef __cplusplus
}
#endif

#endif

