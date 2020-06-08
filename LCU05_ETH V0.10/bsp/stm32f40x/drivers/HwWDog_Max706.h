
/***************************************************************************

Copyright (C), 1999-2012, Tongye Tech. Co., Ltd.

* @file           HwWDog_Max706.h
* @author
* @version        V1.0.0
* @date           2017-03-04
* @brief          (1)
(2)
(3)
History:          // Revision Records
<Author>             <time>        <version >            <desc>
life £¬  Lily         2017-03-04       V1.0.0              ³õ´Î½¨Á¢
***************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HWWDOG_MAX706_H__
#define __HWWDOG_MAX706_H__


#ifdef __cplusplus
extern "C" {
#endif 

	/* Includes ------------------------------------------------------------------*/
#include "rtthread.h"
#include "stm32f4xx.h"


	/* function -----------------------------------------------------------*/
	void HwWDog_Init(void);
	void HwWDog_Feed(void);
#ifdef __cplusplus
}
#endif

#endif 

/******************* (C) COPYRIGHT 2010 Shenzhen Tongye *****END OF FILE****/
