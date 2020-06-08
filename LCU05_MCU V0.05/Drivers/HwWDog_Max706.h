

#ifndef __HWWDOG_MAX706_H
#define __HWWDOG_MAX706_H

#ifdef __cplusplus
extern "C" {
#endif 

    
#include "rtthread.h"
#include "stm32f4xx.h"


void HwWDog_Init(void);
void HwWDog_Feed(void);


#ifdef __cplusplus
}
#endif

#endif 

