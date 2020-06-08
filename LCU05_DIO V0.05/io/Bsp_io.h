

#ifndef __BSP_IO_H
#define __BSP_IO_H


#ifdef __cplusplus
extern "C" {
#endif 


#include "userApp.h"


void inputProcess(PBOARD_DIO_STU pDIO);
void outputProcess(PBOARD_DIO_STU pDio, PBOARD_MCU_STU pMcu_A, PBOARD_MCU_STU pMcu_B, PBOARD_MCU_STU pMcu_C);


#ifdef __cplusplus
}
#endif


#endif

