

#include "Bsp_sysTask.h"
#include "Bsp_systimer.h"
#include "Bsp_eclrTask.h"
#include "Bsp_ITempMonit.h"
#include "Bsp_led.h"
#include "usart.h"
#include "Bsp_canTask.h"
#include "HwWDog_Max706.h"
#include "Bsp_gpio.h"
#include "userApp.h"
#include "Can_fault_detect.h"


#define SYS_TASK_STACK_SIZE         (1024)
#define SYS_TASK_PRIORITY           (10)


ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t sys_thread_stack[SYS_TASK_STACK_SIZE];
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread sys_thread;

static struct rt_semaphore sem_10ms;
static rt_err_t sem_10ms_init;


static void canNodeFlt_detect(void)
{
    uint32_t i;
    
    for(i = 0; i < IO_BOARD_MAX; i++)
    {
        ds.DIO[i].flt.can1 = (Get_CanSts(1, (SLOT_ID_IO_MIN + i)) == RT_ERROR) ? RT_ERROR : RT_EOK;
        ds.DIO[i].flt.can2 = (Get_CanSts(2, (SLOT_ID_IO_MIN + i)) == RT_ERROR) ? RT_ERROR : RT_EOK;        
        ds.DIO[i].flt.lost = (ds.DIO[i].flt.can1 == RT_ERROR && ds.DIO[i].flt.can2 == RT_ERROR) ? RT_ERROR : RT_EOK;
        
        if(ds.DIO[i].flt.lost == RT_ERROR)
        {
            rt_memset(ds.DIO[i].in, 0, 8);
        }
    }

    ds.MCU[0].flt.can1 = (Get_CanSts(1, SLOT_ID_MCU_A) == RT_ERROR) ? RT_ERROR : RT_EOK;
    ds.MCU[0].flt.can2 = (Get_CanSts(2, SLOT_ID_MCU_A) == RT_ERROR) ? RT_ERROR : RT_EOK;
    ds.MCU[0].flt.lost = (ds.MCU[0].flt.can1 == RT_ERROR && ds.MCU[0].flt.can2 == RT_ERROR) ? RT_ERROR : RT_EOK;
    
    ds.MCU[1].flt.can1 = (Get_CanSts(1, SLOT_ID_MCU_B) == RT_ERROR) ? RT_ERROR : RT_EOK;
    ds.MCU[1].flt.can2 = (Get_CanSts(2, SLOT_ID_MCU_B) == RT_ERROR) ? RT_ERROR : RT_EOK;
    ds.MCU[1].flt.lost = (ds.MCU[0].flt.can1 == RT_ERROR && ds.MCU[1].flt.can2 == RT_ERROR) ? RT_ERROR : RT_EOK;
    
    ds.MCU[2].flt.can1 = (Get_CanSts(1, SLOT_ID_MCU_C) == RT_ERROR) ? RT_ERROR : RT_EOK;
    ds.MCU[2].flt.can2 = (Get_CanSts(2, SLOT_ID_MCU_C) == RT_ERROR) ? RT_ERROR : RT_EOK;
    ds.MCU[2].flt.lost = (ds.MCU[2].flt.can1 == RT_ERROR && ds.MCU[2].flt.can2 == RT_ERROR) ? RT_ERROR : RT_EOK;
}


static void powerFlt_detect(void)
{
    static uint32_t dc110v_cnt;
    static uint32_t dc5v_cnt;
    
    ds.dc5v = GetPin(DC5V_DETECT);
    ds.dc110v = GetPin(DC110V_DETECT);
    
    if(ds.dc110v == 0)
    {
        dc110v_cnt++;
    }
    else
    {
        dc110v_cnt = 0;
    }    
    
    if(ds.dc5v == 0 && ds.dc110v == 1)
    {
        dc5v_cnt++;
    }
    else
    {
        dc5v_cnt = 0;
    }
    
    ds.dc110v_flt = (dc110v_cnt > 5) ? RT_ERROR : RT_EOK;
    ds.dc5v_flt = (dc5v_cnt > 5) ? RT_ERROR : RT_EOK;
}


static rt_err_t canNodeSts(void)
{
    rt_err_t ret;
    
    if(ds.carID == 0 || ds.carID == 3)
    {
        if(ds.DIO[0].flt.lost == RT_ERROR || ds.DIO[1].flt.lost == RT_ERROR || ds.DIO[2].flt.lost == RT_ERROR ||
            ds.DIO[3].flt.lost == RT_ERROR || ds.DIO[4].flt.lost == RT_ERROR || ds.DIO[5].flt.lost == RT_ERROR ||
            ds.DIO[6].flt.lost == RT_ERROR || ds.DIO[7].flt.lost == RT_ERROR || ds.DIO[8].flt.lost == RT_ERROR ||
            ds.DIO[9].flt.lost == RT_ERROR || ds.DIO[10].flt.lost == RT_ERROR || ds.DIO[11].flt.lost == RT_ERROR ||
            ds.DIO[12].flt.lost == RT_ERROR || ds.DIO[13].flt.lost == RT_ERROR || ds.DIO[14].flt.lost == RT_ERROR ||
            ds.DIO[15].flt.lost == RT_ERROR || ds.DIO[16].flt.lost == RT_ERROR || ds.DIO[17].flt.lost == RT_ERROR ||
            ds.DIO[18].flt.lost == RT_ERROR || ds.DIO[19].flt.lost == RT_ERROR || ds.DIO[20].flt.lost == RT_ERROR ||
            ds.DIO[21].flt.lost == RT_ERROR || ds.DIO[22].flt.lost == RT_ERROR || ds.DIO[23].flt.lost == RT_ERROR ||
            ds.DIO[24].flt.lost == RT_ERROR || ds.DIO[25].flt.lost == RT_ERROR || ds.DIO[26].flt.lost == RT_ERROR )
        {
            ret = RT_ERROR;
        }
        else
        {
            ret = RT_EOK;
        }
    }
    else if(ds.carID == 1 || ds.carID == 2)
    {
        if(ds.DIO[0].flt.lost == RT_ERROR || ds.DIO[1].flt.lost == RT_ERROR || ds.DIO[2].flt.lost == RT_ERROR ||
            ds.DIO[3].flt.lost == RT_ERROR || ds.DIO[4].flt.lost == RT_ERROR || ds.DIO[5].flt.lost == RT_ERROR ||
            ds.DIO[6].flt.lost == RT_ERROR || ds.DIO[7].flt.lost == RT_ERROR || ds.DIO[8].flt.lost == RT_ERROR )
        {
            ret = RT_ERROR;
        }
        else
        {
            ret = RT_EOK;
        }
    }
    else
    {
        ret = RT_ERROR;
    }
    
    return ret;
}    





static void sys_thread_entry(void* parameter)
{
	static  rt_uint32_t  TimingDelay = 0;
    
//    Bsp_ITemp_Config();
    
	rt_kprintf("\r\n+ BSP TASK INFO : BSP Task Init Success!\r\n");

	for(;;)
	{
		//等待系统操作信号量
		rt_sem_take(&sem_10ms, RT_WAITING_FOREVER);

		TimingDelay++;

//        if(TimingDelay % 100 == 0)
//        {
//            Bsp_ITemp_Process();
//        }


#if IWDG_EN
        IWDG_ReloadCounter();       // 复位软件狗
		HwWDog_Feed();              // 复位硬件狗
#endif
        if(TimingDelay % 10 == 0)
		{
            canNodeFlt_detect();
            
            powerFlt_detect();
            
            if(ds.dc110v_flt == RT_ERROR || ds.dc5v_flt == RT_ERROR)
            {
                ds.fltLevel = 1;
            }
            else if(ds.MCU[0].flt.lost == RT_ERROR || ds.MCU[1].flt.lost == RT_ERROR || ds.MCU[2].flt.lost == RT_ERROR)
            {
                ds.fltLevel = 2;
            }
            else if(canNodeSts() == RT_ERROR)
            {
                ds.fltLevel = 4;
            }
            else
            {
                ds.fltLevel = 0;
            }
            
            switch(ds.fltLevel)
            {
                case 0:
                    System_Led_SetMode(LED_FLT, MODE_OFF);
                    break;
                case 1:
                    System_Led_SetMode(LED_FLT, MODE_ON);
                    break;
                case 2:
                    System_Led_SetMode(LED_FLT, MODE_FLASH_FAST);
                    break;
                case 4:
                    System_Led_SetMode(LED_FLT, MODE_FLASH_SLOW);
                    break;
                default:
                    System_Led_SetMode(LED_FLT, MODE_ON);
                    break;
            }
        }
        

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

void System_bsptask_create(void)
{
	rt_kprintf("\r\n[*] System all task Init Start ...... \r\n");
	rt_kprintf(" + \r\n");
	rt_kprintf(" + \r\n");
    
    console_services_init();
    
    /* 系统定时器硬件初始化 */
    Bsp_systimer_Init();    
    rt_kprintf("\r\n+ TIM INFO: Init success!\r\n");
    
    /* 系统任务线程初始化 */
    Bsp_sysTask_Init();
    
    /* CAN硬件及线程初始化 */
    Bsp_CanInit();
    
    /* KW线程初始化 */
    Bsp_eclrTask_Init();
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

