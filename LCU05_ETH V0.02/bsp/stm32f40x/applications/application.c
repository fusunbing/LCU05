/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

/**
 * @addtogroup STM32
 */
/*@{*/



#include <stdio.h>

#include "stm32f4xx.h"
#include <board.h>
#include <rtthread.h>

#include "sdio_sdcard.h"
#include "Eth_Service.h"
#include "fat_service.h"
#include "EventRecordService.h"

#include "LedTask.h"

#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "HwWDog_Max706.h"

//#include "vos_dbgSrv.h"
//#include "CanStack_Slave.h"

#include "Bsp_canTask.h"

extern void System_GetVersion(void);



void rt_init_thread_entry(void* parameter)
{
    for(;;)
    {
        //LedTask_SetMode(LED_FLT, MODE_OFF);
        //LedTask_SetMode(LED_ACT, MODE_FLASH_FAST);
        rt_thread_delay(1000);
    }
}


int rt_application_init()
{
    rt_thread_t init_thread;

	HwWDog_Init();
#if (RT_THREAD_PRIORITY_MAX == 32)
    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   1024, 8, 20);
#else
    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   1024, 80, 20);
#endif
    rt_thread_startup(init_thread);
    
    System_GetVersion();
    
//    VosDbgSrv_CreatApp(100);

	LedTask_CreateApp();

    ERSRV_CreateApp();
    
	Fat_Service_CreateApp();	

    EthService_CreateApp();

//    CanStackSlave_CreateApp();
    Bsp_CanInit();

    return 0;
}

/*@}*/
