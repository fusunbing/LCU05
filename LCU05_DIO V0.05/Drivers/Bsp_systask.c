

#include "Bsp_systask.h"
#include "Bsp_led.h"
#include "usart.h"
#include "Bsp_finsh_queue.h"
#include "Bsp_canTask.h"
#include "Bsp_ioTask.h"
#include "HwWDog_Max706.h"
#include "userApp.h"


#define SYS_TASK_STACK_SIZE         (1024)
#define SYS_TASK__PRIORITY          (10)


ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t sys_thread_stack[SYS_TASK_STACK_SIZE];
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread sys_thread;

static struct rt_semaphore sem_10ms;
static rt_err_t sem_10ms_init;


static void sys_thread_entry(void* parameter)
{
	static  rt_uint32_t  TimingDelay = 0;

	rt_kprintf("\r\n+ BSP TASK INFO : BSP Task Init Success!\r\n");

	for(;;)
	{
		//等待系统操作信号量
		rt_sem_take(&sem_10ms, RT_WAITING_FOREVER);

		TimingDelay++;
        
#if IWDG_EN
        IWDG_ReloadCounter();       // 复位软件狗
		HwWDog_Feed();              // 复位硬件狗
#endif

		if(TimingDelay % 4 == 0)
		{
			System_Led_Logic(); // LED指示灯
		}
	}
}


static void Bsp_sysTask_Init(void)
{
    rt_err_t ret;
    
    sem_10ms_init  = rt_sem_init(&sem_10ms, "Sem10ms", 1, RT_IPC_FLAG_FIFO);

	ret = rt_thread_init(&sys_thread,
            "BspSysTask",
            sys_thread_entry,
            RT_NULL,
            sys_thread_stack, 
            SYS_TASK_STACK_SIZE,
            SYS_TASK__PRIORITY, 
            8);
        
    if(RT_EOK == ret)
    {
        rt_thread_startup(&sys_thread);
    }
}


void sem_10ms_Release(void)
{
    if(sem_10ms_init == RT_EOK)
    {
        rt_sem_release(&sem_10ms);
    }
}


void System_task_create(void)
{
    rt_kprintf("\r\n[*] System all task Init Start ...... \r\n");
	
    //初始化硬件看门狗
    HwWDog_Init();
	
    console_services_init();

	//BSP系统任务的创建
    Bsp_sysTask_Init();
    
    //初始化IO逻辑运算线程
    Bsp_ioTask_Init();

    Bsp_canTask_Init();
}

