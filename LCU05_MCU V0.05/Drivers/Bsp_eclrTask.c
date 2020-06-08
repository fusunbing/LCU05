

#include "Bsp_eclrTask.h"


/* ECLR线程池大小 */
#define ECLR_TASK_STACK_SIZE        (2048)
/* ECLR线程优先级 */
#define ECLR_TASK_PRIORITY          (13)

ALIGN(RT_ALIGN_SIZE)
static uint8_t eclr_thread_stack[ECLR_TASK_STACK_SIZE];
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread eclr_thread;


extern int eclr_main();
static void eclr_thread_entry(void* parameter)
{
    eclr_main();
}


void Bsp_eclrTask_Init(void)
{
    rt_err_t ret;
    
	// eclr application
    ret = rt_thread_init(&eclr_thread,
            "eclrTask",
            eclr_thread_entry,
            RT_NULL,
            eclr_thread_stack,
            ECLR_TASK_STACK_SIZE, 
            ECLR_TASK_PRIORITY, 
            10);
        
    if(RT_EOK == ret)
    {
        rt_thread_startup(&eclr_thread);
    }
}

