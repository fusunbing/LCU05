
#include <rtthread.h>
#include "stm32f4xx.h"
#include "Bsp_systask.h"

#include "Bsp_led.h"
#include "usart.h"
#include "Bsp_canTask.h"
#include "HwWDog_Max706.h"
#include "Bsp_gpio.h"
#include "userApp.h"
#include "Bsp_mvbService.h"


ALIGN(RT_ALIGN_SIZE)
static rt_uint32_t bsp_sys_stack[BSP_TSYS_STACKSIZE];
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread bsp_sys_thread;

extern struct rt_semaphore sem_systask_10ms;


static void Bsp_system_task(void* parameter)
{
	static  rt_uint32_t  TimingDelay     = 0;

	rt_kprintf("\r\n+ BSP TASK INFO : BSP Task Init Success!\r\n");

	for(;;)
	{
		//等待系统操作信号量
		rt_sem_take(&sem_systask_10ms, RT_WAITING_FOREVER);

		TimingDelay++;
        
#if IWDG_EN
        IWDG_ReloadCounter();       // 复位软件狗
		HwWDog_Feed();              // 复位硬件狗
#endif

		if(TimingDelay % 4 == 0)
		{
			System_Led_Logic();     // LED指示灯
		}
	}
}


void System_bsptask_create(void)
{
	rt_kprintf("\r\n[*] System all task Init Start ...... \r\n");
	rt_kprintf(" + \r\n");
	rt_kprintf(" + \r\n");
    
    console_services_init();
    
	// BSP 系统任务的创建
	rt_thread_init(&bsp_sys_thread,
		"BspSysTask",
		Bsp_system_task,
		RT_NULL,
		&bsp_sys_stack[0], 
		sizeof(bsp_sys_stack),
		BSP_TSYS_PRIORITY, 
        8);
        
	rt_thread_startup(&bsp_sys_thread);
		
    Bsp_mvbService_Init();

	//CAN
    Bsp_CanInit();
}


void time_tick(void)
{
	rt_uint32_t tick = 0;
	tick = rt_tick_get();

	rt_kprintf("the rt_tick is : %d\r\n", tick);
}

void running_time(void)
{
	rt_uint8_t  hour = 0;
	rt_uint8_t  min  = 0;
	rt_uint8_t  sec  = 0;
	rt_uint32_t tick = 0;

	tick = rt_tick_get();

	hour = (rt_uint8_t)(tick/3600000);
	min  = (rt_uint8_t)((tick/60000)%60);
	sec  = (rt_uint8_t)((tick/1000)%60);

	rt_kprintf("the system running total time is : %3d:",hour);
	rt_kprintf("%02d:",min);
	rt_kprintf("%02d\r\n",sec);
}

#include "finsh.h"
FINSH_FUNCTION_EXPORT(time_tick, rt_tick show)
FINSH_FUNCTION_EXPORT(running_time, running time show)

