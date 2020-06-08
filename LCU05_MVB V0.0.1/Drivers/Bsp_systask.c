
#include <rtthread.h>
#include "stm32f4xx.h"
#include "Bsp_systask.h"

#include "Bsp_led.h"
#include "usart.h"
#include "Bsp_canTask.h"
#include "HwWDog_Max706.h"
#include "Bsp_gpio.h"
#include "userApp.h"
//#include "Mvb_service.h"
#include "MVB_Cfg.h"

//------------------------------------------------------------------
//  底层任务
//------------------------------------------------------------------
ALIGN(RT_ALIGN_SIZE)
static rt_uint32_t bsp_sys_stack[BSP_TSYS_STACKSIZE];
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread bsp_sys_thread;

//------------------------------------------------------------------
//  eclr task
//------------------------------------------------------------------
//ALIGN(RT_ALIGN_SIZE)
//static char thread_eclr_stack[2048];
//ALIGN(RT_ALIGN_SIZE)
//struct rt_thread thread_eclr;


extern struct rt_semaphore sem_systask_10ms;

static void  Bsp_system_task(void* parameter);



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
		BSP_TSYS_PRIORITY, 8
        
		);
        
	rt_thread_startup(&bsp_sys_thread);
		
    Bsp_MvbTask_CreateApp();

	// CAN
    Bsp_CanInit();
    
	
}

//static void flt_clear(void)
//{
//    uint8_t i;
//    uint8_t temp = 0;

//    for(i = 0; i < pKW_SHM->ds.canNodesCnt; i++)
//    {
//        if(pKW_SHM->ds.canNodes[i].can1_fault != 0 && pKW_SHM->ds.canNodes[i].can2_fault != 0 )
//        {
//            temp++;
//        }
//    }

//    if(pKW_SHM->ds.dc5v_detect == PIN_HIGH && pKW_SHM->ds.dc5v_detect == PIN_HIGH && temp == 0)
//    {
//        System_Led_SetMode(LED_FLT, MODE_OFF);
//    }
//}    

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
        IWDG_ReloadCounter();   // 复位软件狗
		HwWDog_Feed();  // 复位硬件狗
#endif

		if(TimingDelay % 5 == 0)
		{
			System_Led_Logic(); // LED指示灯
		}
	}
}


/*******************************************************************************
* Function Name  : CheckSystemRst
* Description    : Checks which reset happened
* Input          : - RCC_FLAG: specifies the flag to check.
*                    This parameter can be one of the following values:
*                       - RCC_FLAG_PINRST: Pin reset
*                       - RCC_FLAG_PORRST: POR/PDR reset
*                       - RCC_FLAG_SFTRST: Software reset
*                       - RCC_FLAG_IWDGRST: Independent Watchdog reset
*                       - RCC_FLAG_WWDGRST: Window Watchdog reset
*                       - RCC_FLAG_LPWRRST: Low Power reset
*当上电复位时候，RCC->CSR的值为0x24000000，即RCC_FLAG_PINRST和RCC_FLAG_PORRST置位。
当Iwdg复位时候，发现同时RCC->CSR的值为0x24000000，即RCC_FLAG_PINRST和RCC_FLAG_IWDGRST
同时置位，但是检测波形并没有外部管脚复位信号。因此在处理复位原因的时候，需要注意这些细
节。
* Output         : None
* Return         : The new state of RCC_FLAG (SET or RESET).
*******************************************************************************/
#define POR_RST			1
#define PIN_RST			2
#define LPW_RST			4
#define WWDG_RST		8
#define IWWDG_RST		16
#define SFT_RST			32

u8 Rst_RegInfo = 0;
u32 Csr_register = 0 ;
u32 Csr_register1 = 0 ;
void CheckSystemRst(uint32_t rccReg)
{
	Csr_register = RCC->CSR ;
	/* Check if the system has resumed from each reset */
	if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
	{	
		Rst_RegInfo = POR_RST;
	}
	if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
	{		
		Rst_RegInfo |= PIN_RST;
	}
	if (RCC_GetFlagStatus(RCC_FLAG_LPWRRST) != RESET)
	{	
		Rst_RegInfo |= LPW_RST;		
	}
	if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
	{		
		Rst_RegInfo |= WWDG_RST;
	}
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		Rst_RegInfo |= IWWDG_RST;	
	}
	if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET)
	{		
		Rst_RegInfo |= SFT_RST;		
	}

	/* Clear reset flags */
	RCC_ClearFlag();

	Csr_register1 = RCC->CSR ;

	rt_kprintf("\r\n+ SysRST Info: ");
	rt_kprintf("\r\n  Csr_register: 0x%x", Csr_register);
	rt_kprintf("\r\n  rst_semphore: 0x%x", Rst_RegInfo);

	rt_kprintf("\r\n+ SYS_RST: ");
	rt_kprintf("\r\n +    BOR: %d", ((rccReg >> 25) & 0x1) > 0 ? 1: 0);
	rt_kprintf("\r\n +    PIN: %d", ((rccReg >> 26) & 0x1) > 0 ? 1: 0);
	rt_kprintf("\r\n +    POR: %d", ((rccReg >> 27) & 0x1) > 0 ? 1: 0);
	rt_kprintf("\r\n +    SFT: %d", ((rccReg >> 28) & 0x1) > 0 ? 1: 0);
	rt_kprintf("\r\n +    IWD: %d", ((rccReg >> 29) & 0x1) > 0 ? 1: 0);
	rt_kprintf("\r\n +    WWD: %d", ((rccReg >> 30) & 0x1) > 0 ? 1: 0);
	rt_kprintf("\r\n +    LPW: %d", ((rccReg >> 31) & 0x1) > 0 ? 1: 0);

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

