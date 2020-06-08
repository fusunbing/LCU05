

#include "Bsp_mvbTask.h"


#define MVB_TASK_STACKSIZE   (1024)
#define MVB_TASK_PRIORITY    (0x12)


//mvb Task
ALIGN(RT_ALIGN_SIZE)
static rt_uint32_t mvbTask_stack[MVB_TASK_STACKSIZE];
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread mvbTask;


static void Bsp_mvbTask(void* parameter)
{
    for(;;)
    {

        

    }
}


void  Bsp_mvbTask_Init(void)
{
    rt_err_t mvbTask_init;
    
	//MVB_Card_RST_GPIO_Init();
	
	//MVB_Data_Init();
    
	MVB_Event_Init();
    
    
    mvbTask_init = rt_thread_init(&mvbTask,
                    "Bsp_mvbTask",
                    Bsp_mvbTask,
                    RT_NULL,
                    mvbTask_stack, 
                    sizeof(mvbTask_stack),
                    MVB_TASK_PRIORITY, 
                    8
                    );
    
    if(mvbTask_init == RT_EOK)
	{
		rt_thread_startup(&mvbTask);
	}
}



















