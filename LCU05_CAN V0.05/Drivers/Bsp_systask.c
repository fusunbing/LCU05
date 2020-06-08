

#include "Bsp_systask.h"
#include "Bsp_led.h"
#include "usart.h"
#include "Bsp_canTask.h"
#include "HwWDog_Max706.h"

#define BSP_TSYS_STACKSIZE      (1 * 1024 / 4)  //1K  32bit 对齐
#define BSP_TSYS_PRIORITY       (10)

//系统任务
ALIGN(RT_ALIGN_SIZE)
static rt_uint32_t bsp_sys_stack[BSP_TSYS_STACKSIZE];
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread bsp_sys_thread;

static struct rt_semaphore sem_systask_10ms;
static rt_err_t rtv_sysSem;


static void Bsp_system_task(void* parameter)
{
	rt_kprintf("\r\n+ BSP TASK INFO : BSP Task Init Success!\r\n");

	for(;;)
	{
		//等待系统操作信号量
        rt_sem_take(&sem_systask_10ms, RT_WAITING_FOREVER);
        
#if IWDG_EN
        IWDG_ReloadCounter();       // 复位软件狗
		HwWDog_Feed();              // 复位硬件狗
#endif

        System_Led_Logic(); // LED指示灯
	}
}


void System_bsptask_create(void)
{
    rt_err_t rtv;
    
	rt_kprintf("\r\n[*] System all task Init Start ...... \r\n");
	rt_kprintf(" + \r\n");
	rt_kprintf(" + \r\n");
    
    console_services_init();

    rtv_sysSem = rt_sem_init(&sem_systask_10ms, "Sem10ms", 1, RT_IPC_FLAG_FIFO);
    
	//系统任务的创建
    rtv = rt_thread_init(&bsp_sys_thread,
		"BspSysTask",
		Bsp_system_task,
		RT_NULL,
		&bsp_sys_stack[0], 
		sizeof(bsp_sys_stack),
		BSP_TSYS_PRIORITY, 
        8);
        
    if(rtv == RT_EOK)
    {
        rt_thread_startup(&bsp_sys_thread);
    }
    
	//CAN硬件与线程初始化
    Bsp_CanInit();
}


void sysSem_Send(void)
{
    if(rtv_sysSem == RT_EOK)
    {
        rt_sem_release(&sem_systask_10ms);
    }
}


void time_tick(void)
{
	rt_kprintf("the rt_tick is : %d\r\n", rt_tick_get());
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

