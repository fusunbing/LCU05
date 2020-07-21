

#include "Bsp_systask.h"
#include "Bsp_led.h"
#include "usart.h"
#include "Bsp_finsh_queue.h"
#include "Bsp_canTask.h"
#include "Bsp_ioTask.h"
#include "HwWDog_Max706.h"
#include "userApp.h"
#include "Bsp_ioFault.h"


#define SYS_TASK_STACK_SIZE         (1024)
#define SYS_TASK_PRIORITY           (10)


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
        //�ȴ�ϵͳ�����ź���
        rt_sem_take(&sem_10ms, RT_WAITING_FOREVER);

        TimingDelay++;
        
#if IWDG_EN
    IWDG_ReloadCounter();       // ��λ�����
    HwWDog_Feed();              // ��λӲ����
#endif

        if(TimingDelay % 4 == 0)
        {
            if(rt_tick_get() > 2000)
            {
                //IO���ϼ��
                io_fault_detect();
            }
            
            // LEDָʾ��
            System_Led_Logic(); 
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
            SYS_TASK_PRIORITY, 
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

    //��ʼ��Ӳ�����Ź�
    HwWDog_Init();

    console_services_init();

    //BSPϵͳ����Ĵ���
    Bsp_sysTask_Init();

    //��ʼ��IO�߼������߳�
    Bsp_ioTask_Init();

    Bsp_canTask_Init();
}

