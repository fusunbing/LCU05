
#include "mvb.h"
#include "Bsp_led.h"
#include "userApp.h"
#include "Bsp_mvbService.h"


#define MVB_TASK_STACK_SIZE     (2048)
#define MVB_TASK_PRIORITY       (10)


/* MVB×´Ì¬¶¨Òå */
#define MVB_STS_CONFIG          (0)
#define MVB_STS_NOMAL           (1)
#define MVB_STS_ERROR           (2)


ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t mvb_thread_stack[MVB_TASK_STACK_SIZE];
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread mvb_thread;


static uint32_t mvb_sts = MVB_STS_CONFIG;


static void mvb_thread_entry(void* parameter)
{
	uint8_t ret = 1;
    
    for (;;)
    {		
		switch(mvb_sts)
		{
			case MVB_STS_CONFIG:				
                ret &= MVB_Device_Init();
                ret &= mvbPort_config(0);
            
                if(ret == 0)
                {
                    mvb_sts = MVB_STS_ERROR;
                }
                mvb_sts = MVB_STS_NOMAL;
				break;
			case MVB_STS_NOMAL:
                mvbPort_Read(0);
                mvbPort_write(0);				
				break;			
			case MVB_STS_ERROR:
				System_Led_SetMode(LED_FLT, MODE_FLASH_SLOW);
				mvb_sts = MVB_STS_CONFIG;
				break;
			default:				
				break;
		}
		
		
		rt_thread_delay(16);
    }
}


void Bsp_mvbService_Init(void)
{
    rt_err_t ret;
    
    //-----------------------------------------------------------------------
    // MVB service task
    //-----------------------------------------------------------------------
	ret = rt_thread_init(&mvb_thread,
            "mvbService",
            mvb_thread_entry,
            RT_NULL,
            mvb_thread_stack, 
            MVB_TASK_STACK_SIZE,
            MVB_TASK_PRIORITY, 
            8);
        
    if(RT_EOK == ret)
    {
        rt_thread_startup(&mvb_thread);
    }
}

