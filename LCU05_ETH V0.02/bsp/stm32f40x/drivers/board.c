/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      first implementation
 */

#include <rthw.h>
#include <rtthread.h>

#include "stm32f4xx.h"
#include "board.h"

#include "rtc.h"
#include "version.h"
#include "Bsp_bakram.h"
#include "common.h"


#define GPIO_SIZE               ( GPIOB_BASE - GPIOA_BASE )
#define GPIO_PORT_INDEX(port)   ( ( ##port##_BASE - GPIOA_BASE ) / GPIO_SIZE  )

static void IWDG_Initialization(void);
static void fsmc_gpio_init(void);
static void fsmc_bank4_init(void);
static void _do_conf_fsmc(uint8_t port_index,
                          uint8_t GPIO_PinSource,
                          GPIO_InitTypeDef *GPIO_InitStructure);

static const struct
{
    uint8_t port_index;
    uint8_t PinSource;
} _fsmc_conf[] =
{
    /* D0~D7 */
    {GPIO_PORT_INDEX(GPIOD), 14}, // D0
    {GPIO_PORT_INDEX(GPIOD), 15}, // D1
    {GPIO_PORT_INDEX(GPIOD),  0}, // D2
    {GPIO_PORT_INDEX(GPIOD),  1}, // D3
    {GPIO_PORT_INDEX(GPIOE),  7}, // D4
    {GPIO_PORT_INDEX(GPIOE),  8}, // D5
    {GPIO_PORT_INDEX(GPIOE),  9}, // D6
    {GPIO_PORT_INDEX(GPIOE), 10}, // D7

    /* D8~D15 */
    {GPIO_PORT_INDEX(GPIOE), 11}, // D8
    {GPIO_PORT_INDEX(GPIOE), 12}, // D9
    {GPIO_PORT_INDEX(GPIOE), 13}, // D10
    {GPIO_PORT_INDEX(GPIOE), 14}, // D11
    {GPIO_PORT_INDEX(GPIOE), 15}, // D12
    {GPIO_PORT_INDEX(GPIOD),  8}, // D13
    {GPIO_PORT_INDEX(GPIOD),  9}, // D14
    {GPIO_PORT_INDEX(GPIOD), 10}, // D15

    /* A0~A25 */
    {GPIO_PORT_INDEX(GPIOF),  0}, // A0
    {GPIO_PORT_INDEX(GPIOF),  1}, // A1
    {GPIO_PORT_INDEX(GPIOF),  2}, // A2
    {GPIO_PORT_INDEX(GPIOF),  3}, // A3
    {GPIO_PORT_INDEX(GPIOF),  4}, // A4
    {GPIO_PORT_INDEX(GPIOF),  5}, // A5
    {GPIO_PORT_INDEX(GPIOF), 12}, // A6
    {GPIO_PORT_INDEX(GPIOF), 13}, // A7
    {GPIO_PORT_INDEX(GPIOF), 14}, // A8
    {GPIO_PORT_INDEX(GPIOF), 15}, // A9
    {GPIO_PORT_INDEX(GPIOG),  0}, // A10
    {GPIO_PORT_INDEX(GPIOG),  1}, // A11
    {GPIO_PORT_INDEX(GPIOG),  2}, // A12
    {GPIO_PORT_INDEX(GPIOG),  3}, // A13
    {GPIO_PORT_INDEX(GPIOG),  4}, // A14
    {GPIO_PORT_INDEX(GPIOG),  5}, // A15
    {GPIO_PORT_INDEX(GPIOD), 11}, // A16
    {GPIO_PORT_INDEX(GPIOD), 12}, // A17
    {GPIO_PORT_INDEX(GPIOD), 13}, // A18
    {GPIO_PORT_INDEX(GPIOE),  3}, // A19
    {GPIO_PORT_INDEX(GPIOE),  4}, // A20
    {GPIO_PORT_INDEX(GPIOE),  5}, // A21
    {GPIO_PORT_INDEX(GPIOE),  6}, // A22
    {GPIO_PORT_INDEX(GPIOE),  2}, // A23
//      {GPIO_PORT_INDEX(GPIOG), 13}, // A24
//      {GPIO_PORT_INDEX(GPIOG), 14}, // A25

    /* R/W */
    {GPIO_PORT_INDEX(GPIOD),  4}, // NOE
    {GPIO_PORT_INDEX(GPIOD),  5}, // NWE

    /* CS */
//    {GPIO_PORT_INDEX(GPIOD),  7}, // NE1, 0x60000000,
//    {GPIO_PORT_INDEX(GPIOG),  9}, // NE2, 0x64000000,
    {GPIO_PORT_INDEX(GPIOG),  10}, // NE3, SRAM_CS, 0x68000000, wiscom: SRAM, A0-A18
    {GPIO_PORT_INDEX(GPIOG),  12}, // NE4, M_LAN_CS,0x6C000000, wiscom: LED, A0-A18

    {GPIO_PORT_INDEX(GPIOE),  0}, // NBL0
    {GPIO_PORT_INDEX(GPIOE),  1}, // NBL1

    {GPIO_PORT_INDEX(GPIOD),  6}, // NWAIT
};
/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
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

/*******************************************************************************
 * Function Name  : SysTick_Configuration
 * Description    : Configures the SysTick for OS tick.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void  SysTick_Configuration(void)
{
    RCC_ClocksTypeDef  rcc_clocks;
    rt_uint32_t         cnts;

    RCC_GetClocksFreq(&rcc_clocks);

    cnts = (rt_uint32_t)rcc_clocks.HCLK_Frequency / RT_TICK_PER_SECOND;
    cnts = cnts / 8;

    SysTick_Config(cnts);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}



/**
 * This function will initial STM32 board.
 */
void rt_hw_board_init()
{
    IWDG_Initialization(); /* 恢复内部独立看门狗 2019-4-17*/

    /* NVIC Configuration */
    NVIC_Configuration();

    /* Configure the SysTick */
    SysTick_Configuration();

    rt_hw_usart_init();

    #ifdef RT_USING_CONSOLE
    rt_console_set_device(CONSOLE_DEVICE);
    #endif



    Board_Information_Init();
     rt_kprintf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
    //-----------------------------------------------------------------------------------------
    // MCU信息
    //-----------------------------------------------------------------------------------------
    rt_kprintf("\r\n\r\n+++++++++++++++++++++++++++++++++++++++\r\n");
    rt_kprintf        ("+            SystemInit \r\n");
    rt_kprintf        ("+                       \r\n");
    rt_kprintf        ("+ MCU           : STM32F407  \r\n");

    rt_kprintf        ("+ OS            : RT Thread Operating System\r\n");
    rt_kprintf        ("+ OS Ver        : %d.%d.%d\r\n", RT_VERSION, RT_SUBVERSION, RT_REVISION);
    rt_kprintf        ("+ BspVer        : %d.%d.%d  \r\n", MAJOR_VERSION_NUMBER, MINOR_VERSION_NUMBER, REVISION_NUMBER);
    rt_kprintf        ("+ BuildCounts   : %d  \r\n", BUILD_NUMBER);
    rt_kprintf        ("+ COPYRIGHT     : TongYe \r\n");
    rt_kprintf        ("+++++++++++++++++++++++++++++++++++++++\r\n");
    rt_kprintf("\r\n[*] BSP Module Init Start ...... \r\n");


    fsmc_gpio_init();
    // EXT_RAM 初始化
    fsmc_bank4_init();

    // Backup SRAM 模块初始化
    System_hw_bakramInit();

    // rtc模块初始化
    rt_hw_rtc_init();
    rt_kprintf("\r\nRTC INFO: RTC Module Init success!");
}


static void fsmc_gpio_init(void)
{
    int i;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOs clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD
                           | RCC_AHB1Periph_GPIOE
                           | RCC_AHB1Periph_GPIOF
                           | RCC_AHB1Periph_GPIOG, ENABLE);

    /* Enable FSMC clock */
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    for (i = 0; i < sizeof(_fsmc_conf) / sizeof(_fsmc_conf[0]); i++)
    {
       _do_conf_fsmc(_fsmc_conf[i].port_index, _fsmc_conf[i].PinSource, &GPIO_InitStructure);
    }
}



static void _do_conf_fsmc(uint8_t port_index,
                          uint8_t GPIO_PinSource,
                          GPIO_InitTypeDef *GPIO_InitStructure)
{
    GPIO_TypeDef * GPIO;

    GPIO = (GPIO_TypeDef *)(GPIO_SIZE * port_index + GPIOA_BASE);

    GPIO_PinAFConfig(GPIO, GPIO_PinSource, GPIO_AF_FSMC);

    GPIO_InitStructure->GPIO_Pin = (1 << GPIO_PinSource); // GPIO_Pin
    GPIO_Init(GPIO, GPIO_InitStructure);
}


static void fsmc_bank4_init(void)
{
   FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  Timing_read, Timing_write;

    uint32_t FSMC_Bank = FSMC_Bank1_NORSRAM4;

    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &Timing_read;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &Timing_write;

    FSMC_NORSRAMStructInit(&FSMC_NORSRAMInitStructure);

    /*--------------------- read timings configuration ---------------------*/
//    Timing_read.FSMC_AddressSetupTime = 0x01;  /* [3:0] F2/F4 1~15 HCLK */
//    Timing_read.FSMC_AddressHoldTime = 0x00;   /* [7:4] keep 0x00 in SRAM mode */
//    Timing_read.FSMC_DataSetupTime = 0x02;     /* [15:8] F2/F4 0~255 HCLK */
//    /* [19:16] Time between NEx high to NEx low (BUSTURN HCLK) */
//    Timing_read.FSMC_BusTurnAroundDuration = 1;
//    Timing_read.FSMC_CLKDivision = 0; /* [24:20] keep 0x00 in SRAM mode  */
//    Timing_read.FSMC_DataLatency = 0; /* [27:25] keep 0x00 in SRAM mode  */
//    Timing_read.FSMC_AccessMode = FSMC_AccessMode_A;
    Timing_read.FSMC_AddressSetupTime = 0x04;  /* [3:0] F2/F4 1~15 HCLK */
    Timing_read.FSMC_AddressHoldTime = 0x01;   /* [7:4] keep 0x00 in SRAM mode */
    Timing_read.FSMC_DataSetupTime = 0x04;     /* [15:8] F2/F4 0~255 HCLK */
    /* [19:16] Time between NEx high to NEx low (BUSTURN HCLK) */
    Timing_read.FSMC_BusTurnAroundDuration = 0;
    Timing_read.FSMC_CLKDivision = 0; /* [24:20] keep 0x00 in SRAM mode  */
    Timing_read.FSMC_DataLatency = 2; /* [27:25] keep 0x00 in SRAM mode  */
    Timing_read.FSMC_AccessMode = FSMC_AccessMode_A;

    /*--------------------- write timings configuration ---------------------*/
//    Timing_write.FSMC_AddressSetupTime = 0x01;  /* [3:0] F2/F4 1~15 HCLK */
//    Timing_write.FSMC_AddressHoldTime = 0x00;   /* [7:4] keep 0x00 in SRAM mode */
//    Timing_write.FSMC_DataSetupTime = 0x01;     /* [15:8] F2/F4 0~255 HCLK */
//    /* [19:16] Time between NEx high to NEx low (BUSTURN HCLK) */
//    Timing_write.FSMC_BusTurnAroundDuration = 1;
//    Timing_write.FSMC_CLKDivision = 0; /* [24:20] keep 0x00 in SRAM mode  */
//    Timing_write.FSMC_DataLatency = 0; /* [27:25] keep 0x00 in SRAM mode  */
//    Timing_write.FSMC_AccessMode = FSMC_AccessMode_A;
    Timing_write.FSMC_AddressSetupTime = 0x04;  /* [3:0] F2/F4 1~15 HCLK */
    Timing_write.FSMC_AddressHoldTime = 0x01;   /* [7:4] keep 0x00 in SRAM mode */
    Timing_write.FSMC_DataSetupTime = 0x04;     /* [15:8] F2/F4 0~255 HCLK */
    /* [19:16] Time between NEx high to NEx low (BUSTURN HCLK) */
    Timing_write.FSMC_BusTurnAroundDuration = 0;
    Timing_write.FSMC_CLKDivision = 0; /* [24:20] keep 0x00 in SRAM mode  */
    Timing_write.FSMC_DataLatency = 2; /* [27:25] keep 0x00 in SRAM mode  */
    Timing_write.FSMC_AccessMode = FSMC_AccessMode_A;

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
    FSMC_NORSRAMCmd(FSMC_Bank, ENABLE);
}


static void IWDG_Initialization(void)
{
    uint8_t obVal;

    //仿真模式下，IWDG不起作用
    DBGMCU_APB1PeriphConfig(DBGMCU_IWDG_STOP, ENABLE);

    //读取flash中USER区WDG_SW寄存器标志;1：SW 0:HW
    //若为软件模式，则修改为硬件模式
    obVal = FLASH_OB_GetUser();
    if (obVal & 0x01)
    {
        FLASH_OB_Unlock();
        FLASH_OB_UserConfig(OB_IWDG_HW, OB_STOP_NoRST, OB_STDBY_NoRST);
        while (FLASH_OB_Launch() != FLASH_COMPLETE);
        FLASH_OB_Lock();
    }

    //操作寄存器前的解锁操作
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    IWDG_SetPrescaler(IWDG_Prescaler_64);//精度为2ms

    //设置超时时间约8s （2ms*0xFFF）
    IWDG_SetReload(0xFFF);
    //喂狗操作
    IWDG_ReloadCounter();
}

