

#include <rthw.h>
#include <rtthread.h>
#include "stm32f4xx.h"
#include "rtc.h"
#include "usart.h"
#include "board.h"

#include "HwWDog_Max706.h"
#include "Bsp_led.h"
#include "Bsp_gpio.h"
#include "Bsp_fsmc.h"
#include "Bsp_bakram.h"
#include "Bsp_systask.h"
#include "userApp.h"





static uint32_t _sysCoreClk;


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
	_sysCoreClk =  rcc_clocks.HCLK_Frequency;
	SysTick_Config(cnts);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}


static void SRAM_Test(void)
{
    unsigned char * p_extram = (unsigned char *)STM32_EXT_SRAM_BEGIN;
    unsigned int temp;

    rt_kprintf("\r\nmem testing....");
    for(temp = 0; temp < (STM32_EXT_SRAM_END - STM32_EXT_SRAM_BEGIN); temp++)
    {
        *p_extram++ = (unsigned char)temp;
    }

    p_extram = (unsigned char *)STM32_EXT_SRAM_BEGIN;
    for(temp = 0; temp < (STM32_EXT_SRAM_END - STM32_EXT_SRAM_BEGIN); temp++)
    {
        if( *p_extram++ != (unsigned char)temp )
        {
            while (1)
            {
                rt_kprintf("memtest fail @ %08Xsystem halt!!!!! \r\n", (unsigned int)p_extram);
            }

        }
    }
    rt_kprintf("\r\nmem test pass!!\r\n");

    for(temp = 0; temp < (STM32_EXT_SRAM_END-STM32_EXT_SRAM_BEGIN); temp++)
    {
        *p_extram++ = 0;
    }
}


void SysTick_Handler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	rt_tick_increase();

	/* leave interrupt */
	rt_interrupt_leave();
}


static uint32_t clockSouc = 0;

void rt_hw_board_init(void)
{
    uint32_t rcc_reg, crystl;
    
	/* NAND_IDTypeDef NAND_ID */
	DBGMCU_APB1PeriphConfig(DBGMCU_IWDG_STOP, ENABLE);
    
#if  IWDG_EN
	/* init IWDG */
	IWDG_Init(20);
#endif
    
	/* Get the ystem reset flag */
	rcc_reg = RCC->CSR;
    
	/* NVIC Configuration */
	NVIC_Configuration();
    
	/* Configure the SysTick */
	SysTick_Configuration();
    
	/* Configure the Console */
    stm32_hw_usart_init();
    
#ifdef RT_USING_CONSOLE
	rt_console_set_device(CONSOLE_DEVICE);
#endif

	//  系统指示灯初始化
	System_Led_Init();
    
    //系统slot ID、110V检测、5V检测gpio初始化
    stmPinsInit();

    rt_kprintf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
    
	// MCU信息
	rt_kprintf("\r\n\r\n+++++++++++++++++++++++++++++++++++++++\r\n");
	rt_kprintf        ("+            SystemInit \r\n");
	rt_kprintf        ("+                       \r\n");
	rt_kprintf        ("+ MCU           : STM32F407  \r\n");
    
    
    crystl = ((RCC->CR & RCC_CR_HSERDY) > 0) ? 1: 0;
    clockSouc = crystl;

	if(crystl != RESET)
	{
		rt_kprintf        ("+ Clock source  : HSE  \r\n");
	}
	else
	{
		rt_kprintf        ("+ Clock source  : HSI  \r\n");
	}
    
	rt_kprintf("+ SPEED         : %d MHz  \r\n", (_sysCoreClk /1000000));
	rt_kprintf("+ OS            : RT Thread Operating System\r\n");
	rt_kprintf("+ OS Ver        : %d.%d.%d\r\n", RT_VERSION, RT_SUBVERSION, RT_REVISION);
	rt_kprintf("+ BspVer        : %d.%d\r\n", MAJOR_VERSION, MAJOR_VERSION);
	rt_kprintf("+ BoardType     : MCU\r\n");
	rt_kprintf("+ COPYRIGHT     : Tongye Team \r\n");
	rt_kprintf("+++++++++++++++++++++++++++++++++++++++\r\n");
	rt_kprintf("\r\n[*] BSP Module Init Start ...... \r\n");
	rt_kprintf(" + \r\n");
    rt_kprintf(" + \r\n");

    
    // SRAM FSMC模块初始化
    Bsp_fsmc_init();
    rt_kprintf("\r\n+ FSMC INFO: FSMC Init success!\r\n");
    rt_kprintf("\r\n+ SRAM INFO: SRAM Init success!\r\n");
    SRAM_Test();
	
	userApp_init();

	// Backup SRAM 模块初始化
	System_hw_bakramInit();
    
    
	// rtc模块初始化
	rt_hw_rtc_init();
	rt_kprintf("\r\n+ RTC INFO: RTC Module Init success!\r\n");
    
    //硬件看门狗初始化
    HwWDog_Init();
    
	//-----------------------------------------------------------------------------------------
	// CPU占用率计算模块初始化
	//-----------------------------------------------------------------------------------------
	cpu_usage_init();
	rt_kprintf("\r\n+ CPU USAGE INFO: Can use console see about the cpu use perecent!\r\n");
    
	//-----------------------------------------------------------------------------------------
	// System Reset Source
	//-----------------------------------------------------------------------------------------
	CheckSystemRst(rcc_reg);
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


void lcu_version(void)
{
    uint32_t i;
    
    rt_kprintf("+  mvb  : V%d.%d  \r\n", ds.mvb_Version>>5, ds.mvb_Version&0x1F);
    rt_kprintf("+  can1 : V%d.%d  \r\n", ds.can1_Version>>5, ds.can1_Version&0x1F);
    rt_kprintf("+  can2 : V%d.%d  \r\n", ds.can2_Version>>5, ds.can2_Version&0x1F);
    rt_kprintf("+  eth  : V%d.%d  \r\n", ds.etu_Version>>5, ds.etu_Version&0x1F);
    
    rt_kprintf("+  MCU A: V%d.%d  \r\n", ds.MCU[0].armVersion >> 5, ds.MCU[0].armVersion & 0x1F);
    rt_kprintf("+  MCU B: V%d.%d  \r\n", ds.MCU[1].armVersion >> 5, ds.MCU[1].armVersion & 0x1F);
    rt_kprintf("+  MCU C: V%d.%d  \r\n", ds.MCU[2].armVersion >> 5, ds.MCU[2].armVersion & 0x1F);
    rt_kprintf("+   KW A: V%d.%d  \r\n", ds.MCU[0].kwVersion >> 5, ds.MCU[0].kwVersion & 0x1F);
    rt_kprintf("+   KW B: V%d.%d  \r\n", ds.MCU[1].kwVersion >> 5, ds.MCU[1].kwVersion & 0x1F);
    rt_kprintf("+   KW C: V%d.%d  \r\n", ds.MCU[2].kwVersion >> 5, ds.MCU[2].kwVersion & 0x1F);

    for(i = 0; i < IO_BOARD_MAX; i++)
    {
        rt_kprintf("+ DIO %2d: V%d.%d  \r\n", (i + 1), ds.DIO[i].version >> 5, ds.DIO[i].version & 0x1F);
    }
}


void list_flt(void)
{
    
    
}


void list_dio(void)
{
    uint32_t i;

    for(i = 0; i < 27; i++)
    {
        rt_kprintf("+ DIO%2d: %2x, %2x, %2x, %2x, %2x, %2x, %2x, %2x\r\n", (i + 1), 
        ds.DIO[i].in[0],
        ds.DIO[i].in[1],
        ds.DIO[i].ou,
        ds.DIO[i].fb[0],
        ds.DIO[i].fb[1],
        ds.DIO[i].inFlt[0],
        ds.DIO[i].inFlt[1],
        ds.DIO[i].ouFlt);
    }
}


#include "finsh.h"
FINSH_FUNCTION_EXPORT(clockSour, show clock source)
FINSH_FUNCTION_EXPORT(lcu_version, show lcu board version)
FINSH_FUNCTION_EXPORT(list_flt, show lcu fault)
FINSH_FUNCTION_EXPORT(list_dio, show lcu dio)
