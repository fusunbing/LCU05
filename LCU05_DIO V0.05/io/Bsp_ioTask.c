

#include "Bsp_ioTask.h"
#include "Bsp_ioTimer.h"
#include "Bsp_ioPwm.h"
#include "Bsp_io.h"
#include <rtthread.h>
#include "stm32f4xx.h"

#define IO_THREAD_PRIORITY      (7)
#define IO_THREAD_STACK_SIZE    (1024)

ALIGN(RT_ALIGN_SIZE)
static uint8_t io_thread_stack[IO_THREAD_STACK_SIZE];
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread io_thread;

static struct rt_semaphore sem2ms;
static rt_err_t semIoTask_Init;


static void io_thread_entry(void* paramemter)
{
    Bsp_ioPwm_init();
    
    Bsp_ioTimer_Init();
    
    for (;;)
    {
        rt_sem_take(&sem2ms, RT_WAITING_FOREVER);
        inputProcess(&ds.DIO[0]);
        outputProcess(&ds.DIO[0], &ds.MCU[0], &ds.MCU[1], &ds.MCU[2]);
    }
}


void Bsp_ioTask_Init(void)
{
    rt_err_t rtv;
    
	rt_kprintf("\r\n[*] IO Thread Init ...... \r\n");
    
    semIoTask_Init = rt_sem_init(&sem2ms, "Sem2ms", 0, RT_IPC_FLAG_FIFO);

    rtv = rt_thread_init(&io_thread,
            "IO_Thread",
            io_thread_entry,
            RT_NULL,
            io_thread_stack,
            IO_THREAD_STACK_SIZE,
            IO_THREAD_PRIORITY,
            8);
            
    if(rtv == RT_EOK)
    {
        rt_thread_startup(&io_thread);
    }
}


void send_IoTaskSem(void)
{
    if(semIoTask_Init == RT_EOK)
    {
        rt_sem_release(&sem2ms);
    }
}

