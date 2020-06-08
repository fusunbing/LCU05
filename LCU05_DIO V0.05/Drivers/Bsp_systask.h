

#ifndef __BSP_SYSTASK_H__
#define __BSP_SYSTASK_H__

#include <rtthread.h>
#include "stm32f4xx.h"


#define IWDG_EN		(1)


void System_task_create(void);
void sem_10ms_Release(void);


#endif

