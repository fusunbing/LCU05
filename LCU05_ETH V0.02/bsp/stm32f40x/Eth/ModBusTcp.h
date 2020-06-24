/***************************************************************************

Copyright (C), 1999-2019, Tongye Tech. Co., Ltd.

* @file           ModBusTcp.h
* @author        
* @version        V1.0.0
* @date           2019-04-23
* @brief          (1)  
                  (2) 
                  (3) 
History:          // Revision Records

<Author>             <time>       <version >            <desc>

Lily, Life           2019-04-23       V1.0.0               

***************************************************************************/
/* Includes --------------------------------------------------------------*/

#ifndef __BSP__MODBUSTCP_H
#define __BSP__MODBUSTCP_H
/* Includes --------------------------------------------------------------*/
#include "rtthread.h"
#include "stm32f4xx.h"



//------------------------------------------------------------------
//                      functions
//------------------------------------------------------------------
//void  EthService_CreateApp(void);
int  ModBusTcp_Int(void);
int  ModBusTcp_Generate(void* buf, int buf_size);
#endif 
