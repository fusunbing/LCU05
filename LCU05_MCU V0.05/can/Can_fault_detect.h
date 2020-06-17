

#ifndef __CAN_FAULT_DETECT_H
#define __CAN_FAULT_DETECT_H

#ifdef __cplusplus
extern "C" {
#endif 


#include "stm32f4xx.h"
#include <rtthread.h>


void CanNode_Update(void);
    
void CanNode_Clear(uint32_t port, uint32_t src);
void ExtCanNode_Clear(uint32_t port, uint32_t funID);
    
rt_err_t Get_CanSts(uint32_t port, uint32_t src);
rt_err_t Get_ExtCanSts(uint32_t port, uint32_t carID);

#ifdef __cplusplus
}
#endif

#endif

