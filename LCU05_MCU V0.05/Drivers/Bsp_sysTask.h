

#ifndef _BSP_SYSTASK_H_
#define _BSP_SYSTASK_H_

#include <rtthread.h>
#include "stm32f4xx.h"


#define IWDG_EN		(1)

void System_bsptask_create(void);
void CheckSystemRst(uint32_t rccReg);
void sem_10ms_Release(void);


#endif

