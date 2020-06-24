

#ifndef __BSP_CANTASK_H__
#define __BSP_CANTASK_H__

#ifdef __cplusplus
extern "C" {
#endif
    
#include "stm32f4xx.h"
#include "rtthread.h"
#include <stdio.h>
#include <string.h>
#include "Bsp_can_queue.h"
    
    
#define BSP_CAN_Q_STACKSIZE             (1 * 1024 / 4)  //1.0K  32bit ¶ÔÆë
#define BSP_TCAN_STACKSIZE              (1 * 1024 / 4)  //1.0K  32bit ¶ÔÆë


#define BSP_CAN_T_Q_PRIORITY            (8)
#define BSP_CAN_R_Q_PRIORITY            (8)
#define BSP_TCAN_PRIORITY               (8)


void Bsp_CanInit(void);
void CanTask_Send_Event_Cycle(void);
    
void RemoteIn_To_Shm(uint8_t *p);
void ShmOut_To_Remote(uint8_t *p);
    
    

#ifdef __cplusplus
}
#endif

#endif

