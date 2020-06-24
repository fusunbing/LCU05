/***************************************************************************

Copyright (C), 1999-2012, Tongye Tech. Co., Ltd.

* @file           LedHw.h
* @author
* @version        V1.0.0
* @date
* @brief          (1)
(2)
(3)
History:          // Revision Records

<Author>             <time>       <version >            <desc>
lily              2017-05-24       V1.0.0            初次建立文件
***************************************************************************/


#ifndef __BSP_LEDS_H__
#define __BSP_LEDS_H__


#ifdef __cplusplus
    extern "C" {
#endif

#include "stm32f4xx.h"
//#include "ds.h"
#define LED_DRV_CNT_MAX (10)

/* Private typedef -----------------------------------------------------------*/


//		int LedHw_Init(void);
	void LedHw_Init(void);
		void _led_00_on(void);
		void _led_00_off(void);
		void _led_01_on(void);
		void _led_01_off(void);

//void System_Led_Logic(void);
//void System_Led_SetMode(Led_Type lt, Led_Mode lm);




#ifdef __cplusplus
}
#endif

#endif


/******************* (C) COPYRIGHT 2012 Group *****END OF FILE****/

