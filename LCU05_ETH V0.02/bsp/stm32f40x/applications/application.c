

#include <stdio.h>

#include "stm32f4xx.h"
#include <board.h>
#include <rtthread.h>

#include "sdio_sdcard.h"
#include "Eth_Service.h"
#include "fat_service.h"
#include "EventRecordService.h"

#include "Bsp_led.h"

#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "HwWDog_Max706.h"

#include "Bsp_canTask.h"


void rt_init_thread_entry(void* parameter)
{
    System_Led_SetMode(0, MODE_FLASH_FAST);
    System_Led_SetMode(1, MODE_ON);

    for(;;)
    {
        IWDG_ReloadCounter();
        HwWDog_Feed();

        System_Led_Logic();
        rt_thread_delay(40);
    }
}


int rt_application_init()
{
    rt_thread_t init_thread;


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

    HwWDog_Init();

    ERSRV_CreateApp();

    Fat_Service_CreateApp();

    EthService_CreateApp();

    Bsp_CanInit();

    return 0;
}

