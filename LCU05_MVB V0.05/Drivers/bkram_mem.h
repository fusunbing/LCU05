
/***************************************************************************

Copyright (C), 1999-2014, Lanp Tech. Co., Ltd.

* @file           bkram_mem.h
* @author         
* @version        V1.0.0
* @date           2014-11-13
* @brief          STM32F4xx Backup RAMÇø

History:          // Revision Records

<Author>             <time>       <version >            <desc>

                     2014-11-13       V1.0.0             

***************************************************************************/

#ifndef __BSP_BAKRAM_ALLOCATE_H__
#define __BSP_BAKRAM_ALLOCATE_H__

#include "stm32f4xx.h"
#ifdef STM32F4XX

#ifdef __cplusplus
extern "C" {
#endif

#define BKRAM_SECTION_01	(BKPSRAM_BASE)              //For Flash1 allocate table
#define BKRAM_SECTION_02	(BKRAM_SECTION_01 + 352)    //For Flash1 allocate table
#define BKRAM_SECTION_03	(BKRAM_SECTION_02 + 352)    //For MVB Config Table
#define BKRAM_SECTION_04	(BKRAM_SECTION_03 + 384)    //For ......


#ifdef __cplusplus
}
#endif


#endif
#endif

/******************* (C) COPYRIGHT 2014 Shenzhen Lanp **********END OF FILE****/

/*@}*/

