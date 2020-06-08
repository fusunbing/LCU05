

#include <rthw.h>
#include <rtthread.h>
#include "stm32f4xx.h"
#include "usart.h"
#include "board.h"
#include "userApp.h"

#include "HwWDog_Max706.h"
#include "Bsp_led.h"
#include "Bsp_gpio.h"
#include "Bsp_systimer.h"
#include "Bsp_systask.h"

static uint32_t sysCoreClk = 0;
static uint32_t clockSouc = 0;

#if  IWDG_EN
static void IWDG_Init(rt_uint8_t ReloadTime);
#endif

static void NVIC_Configuration(void);
static void SysTick_Configuration(void);

#if IWDG_EN
static void IWDG_Init(rt_uint8_t ReloadTime)
{
	rt_uint16_t reload;

	if(26 <= ReloadTime)
	{
		reload = 0x0FFF;
	}
	else
	{
		reload = (156 * ReloadTime) & 0x0FFF;
	}

	/* IWDG timeout equal to 280 ms (the timeout may varies due to LSI frequency dispersion) */
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	/* IWDG counter clock: 40KHz(LSI) / IWDG_Prescaler */
	IWDG_SetPrescaler(IWDG_Prescaler_256);

	/* Set counter reload value */
	IWDG_SetReload(reload);

	/* Reload IWDG counter */
	IWDG_ReloadCounter();

	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
}
#endif


static void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}


static void SysTick_Configuration(void)
{
	RCC_ClocksTypeDef  rcc_clocks;
	rt_uint32_t         cnts;

	RCC_GetClocksFreq(&rcc_clocks);

	cnts = (rt_uint32_t)rcc_clocks.HCLK_Frequency / RT_TICK_PER_SECOND;
	cnts = cnts / 8;
	sysCoreClk =  rcc_clocks.HCLK_Frequency;
	SysTick_Config(cnts);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}


void SysTick_Handler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	rt_tick_increase();

	/* leave interrupt */
	rt_interrupt_leave();
}


void rt_hw_board_init(void)
{
	/* NAND_IDTypeDef NAND_ID */
	DBGMCU_APB1PeriphConfig(DBGMCU_IWDG_STOP, ENABLE);
    
#if  IWDG_EN
	/* init IWDG */
	IWDG_Init(20);
#endif

	/* NVIC Configuration */
	NVIC_Configuration();
    
	/* Configure the SysTick */
	SysTick_Configuration();
    
	/* Configure the Console */
    stm32_hw_usart_init();
    
#ifdef RT_USING_CONSOLE
	rt_console_set_device(CONSOLE_DEVICE);
#endif

	//系统指示灯初始化
	System_Led_Init();
    
    //系统slot ID、110V检测、5V检测gpio初始化
    stmPinsInit();

    rt_kprintf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
    
	// MCU信息
	rt_kprintf("\r\n\r\n+++++++++++++++++++++++++++++++++++++++\r\n");
	rt_kprintf("+            SystemInit \r\n");
	rt_kprintf("+                       \r\n");
	rt_kprintf("+ MCU           : STM32F407  \r\n");
    
    
    clockSouc = ((RCC->CR & RCC_CR_HSERDY) > 0) ? 1: 0;

	if(clockSouc != RESET)
	{
		rt_kprintf        ("+ Clock source  : HSE  \r\n");
	}
	else
	{
		rt_kprintf        ("+ Clock source  : HSI  \r\n");
	}
    
	rt_kprintf("+ SPEED         : %d MHz  \r\n", (sysCoreClk /1000000));
	rt_kprintf("+ OS            : RT Thread Operating System\r\n");
	rt_kprintf("+ OS Ver        : %d.%d.%d\r\n", RT_VERSION, RT_SUBVERSION, RT_REVISION);
	rt_kprintf("+ BspVer        : %d\r\n", SOFTWARE_VERSION);
	rt_kprintf("+ COPYRIGHT     : Tongye Team\r\n");
	rt_kprintf("+++++++++++++++++++++++++++++++++++++++\r\n");
	rt_kprintf("\r\n[*] BSP Module Init Start ...... \r\n");
	rt_kprintf(" + \r\n");
    rt_kprintf(" + \r\n");

	// TIM 模块初始化
    Bsp_sysTimerInit();
    rt_kprintf("\r\n+ TIM INFO: Init success!\r\n");


    //硬件看门狗初始化
    HwWDog_Init();
    
	//CPU占用率计算模块初始化
	cpu_usage_init();
	rt_kprintf("\r\n+ CPU USAGE INFO: Can use console see about the cpu use perecent!\r\n");
}


void clockSour(void)
{
    rt_kprintf("\r\n\r\n+++++++++++++++++++++++++++++++++++++++\r\n");

    if (clockSouc != RESET)
    {
        rt_kprintf("+ Clock source  : HSE  \r\n");
    }
    else
    {
        rt_kprintf("+ Clock source  : HSI  \r\n");
    }
}

#include "finsh.h"
FINSH_FUNCTION_EXPORT(clockSour,show clock source)

