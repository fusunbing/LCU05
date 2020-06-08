/***************************************************************************

Copyright (C), 1999-2018, Tongye Tech. Co., Ltd.

* @file           fat_service.h
* @author        
* @version        V1.0.0
* @date           2018-08-17
* @brief          (1)  
                  (2) 
                  (3) 
History:          // Revision Records

<Author>             <time>       <version >            <desc>

Lily, Life           2018-08-17       V1.0.0               

***************************************************************************/
#ifndef __BSP__FATSERVICE_H
#define __BSP__FATSERVICE_H

/* Includes --------------------------------------------------------------*/
#include "rtthread.h"
#include "stm32f4xx.h"
#include "Sdatatype.h"

//------------------------------------------------------------------
//                      functions
//------------------------------------------------------------------
void  Fat_Service_CreateApp(void);
long  FatSrv_Write(void *buf, uint32_t size, uint32_t type, fatsrv_callback cbk);
void  FatPtu_DownloadEvt(uint8_t* input, uint8_t* outbuff, uint32_t* size);
#endif 
