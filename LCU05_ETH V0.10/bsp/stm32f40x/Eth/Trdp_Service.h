/***************************************************************************

Copyright (C), 1999-2014, Tongye Tech. Co., Ltd.

* @file           bsp_trdpservice.h
* @author        
* @version        V1.0.0
* @date           2016-06-29
* @brief          (1)  
                  (2) 
                  (3) 
History:          // Revision Records

<Author>             <time>       <version >            <desc>

care               2016-06-29       V1.0.0               

***************************************************************************/
#ifndef __BSP__TRDPSERVICE_H
#define __BSP__TRDPSERVICE_H
/* Includes --------------------------------------------------------------*/
#include "rtthread.h"
#include "stm32f4xx.h"


//------------------------------------------------------------------
//                      functions
//------------------------------------------------------------------
int pd_init (const char *pDestAddress, uint32_t comID, uint32_t interval);

#endif 
