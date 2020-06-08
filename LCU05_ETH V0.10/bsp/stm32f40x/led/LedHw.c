/***************************************************************************

Copyright (C), 1999-2012, Tongye Tech. Co., Ltd.

* @file           LedHw.c
* @author
* @version        V1.0.0
* @date           
* @brief          (1)
(2)
(3)
History:          // Revision Records

<Author>             <time>       <version >            <desc>
lily              2017-05-24       V1.0.0            初次建立文件
***************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include "stm32f4xx.h"
#include "LedHw.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define LD0_P  (GPIOB)
#define LD0_I  (GPIO_Pin_14)
#define LD1_P  (GPIOB)
#define LD1_I  (GPIO_Pin_15)

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/



/*******************************************************************************
* Function Name  : All_led_init(void)
* Description    :  
*                   
* Input          : - none.
* Output         : None
* Return         : None
*******************************************************************************/
void LedHw_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	rt_kprintf("\r\nLED GPIO configure......");

	/* GPIOA Periph clock enable */
    //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	/* Configure PC7 ~ PC12 , PD0 ~ PD2 in output pushpull mode */
	//  
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin   = LD0_I;
	GPIO_Init(LD0_P, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = LD1_I;
    GPIO_Init(LD1_P, &GPIO_InitStructure);

    
	_led_00_off();
	_led_01_off();
}


/*******************************************************************************
* Function Name  : _led_00_on(void)  |  _led_00_off(void)
* Description    : led_00 : gpiof.pin0 
*                   
*******************************************************************************/
void _led_00_on(void)
{
	LD0_P->BSRRH = LD0_I;
}

void _led_00_off(void)
{
	LD0_P->BSRRL = LD0_I;
}

/*******************************************************************************
* Function Name  : _led_01_on(void)  |  _led_01_off(void)
* Description    : led_01 : gpiof.pin1 
*                   
*******************************************************************************/
void _led_01_on(void)
{
	LD1_P->BSRRH = LD1_I;
}

void _led_01_off(void)
{
	LD1_P->BSRRL = LD1_I;
}


/******************* (C) COPYRIGHT 2012 Group *****END OF FILE****/


