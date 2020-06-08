

#ifndef _BSP_MVBTASK_H_
#define _BSP_MVBTASK_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <rtthread.h>
#include "stm32f4xx.h"


//MVB事件集宏定义
#define MVB_EVENT_WRITE       (1)
#define MVB_EVENT_READ        (2)
#define MVB_EVENT_POWERON     (4)



void MVB_Event_Init();
rt_uint32_t MVB_Check_Event();
void MVB_Send_Write();
void MVB_Send_Read();




#ifdef __cplusplus
}
#endif

#endif

