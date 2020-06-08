/*
 *File: HwWDog_Max706.c
 *This file provides firmware functions to manage Hardware Watchdog.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-21     fusunbing    first version
 *
 */

#include "HwWDog_Max706.h"

#define	HWDOG_PT     (GPIOE)
#define HWDOG_Pin    (GPIO_Pin_2)
#define HWDOG_Rcc    (RCC_AHB1Periph_GPIOE)


void HwWDog_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    rt_kprintf("\r\n Hardware Watchdog Chanals GPIO configure......");

    /* GPIOA Periph clock enable */
    RCC_AHB1PeriphClockCmd(HWDOG_Rcc, ENABLE);

    /* Configure  in intput pushpull mode */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_InitStructure.GPIO_Pin = HWDOG_Pin;
    GPIO_Init(HWDOG_PT, &GPIO_InitStructure);
}


void HwWDog_Feed(void)
{
	if(GPIO_ReadOutputDataBit(HWDOG_PT, HWDOG_Pin) != (uint8_t)Bit_SET)
	{
        GPIO_SetBits(HWDOG_PT, HWDOG_Pin);
	}
	else
	{
        GPIO_ResetBits(HWDOG_PT, HWDOG_Pin);
	}
}

