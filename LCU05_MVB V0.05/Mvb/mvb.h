#ifndef _MVB_H_
#define _MVB_H_

#ifdef __cplusplus
extern "C" {
#endif 

#include "rtthread.h"
#include "stm32f4xx.h"

uint8_t MVB_Device_Init (void);
uint8_t mvbPort_config(uint8_t carID);
void mvbPort_Read(uint8_t carID);
void mvbPort_write(uint8_t carID);



#ifdef __cplusplus
}
#endif

#endif

