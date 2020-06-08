

#ifndef __BSP_CANTASK_H__
#define __BSP_CANTASK_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "Bsp_can_QueApi.h"

    
#define CanTask_Event_Cycle     ((uint32_t)0x01)
#define CanTask_Event_Logic     ((uint32_t)0x02)
    
#define BSP_CAN_Q_STACKSIZE     (1 * 1024 / 4)  //1.0K  32bit ¶ÔÆë
#define BSP_TCAN_STACKSIZE      (1 * 1024 / 4)  //1.0K  32bit ¶ÔÆë

#define BSP_CAN_T_Q_PRIORITY    (8)
#define BSP_CAN_R_Q_PRIORITY    (8)
#define BSP_TCAN_PRIORITY       (13)


void Bsp_CanInit(void);
void CanTask_Send_Event_Logic(void);
void CanTask_Send_Event_Cycle(void);


#ifdef __cplusplus
}
#endif

#endif

