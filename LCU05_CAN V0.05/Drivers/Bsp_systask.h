

#ifndef _BSP_SYSTASK_H_
#define _BSP_SYSTASK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <rtthread.h>
#include "stm32f4xx.h"


#define IWDG_EN		(1)


void System_bsptask_create(void);
void sysSem_Send(void);
    
    
#ifdef __cplusplus
}
#endif

#endif

