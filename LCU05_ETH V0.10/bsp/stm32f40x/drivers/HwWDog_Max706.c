
/***************************************************************************

Copyright (C), 1999-2012, Tongye Tech. Co., Ltd.

* @file           HwWDog_Max706.c
* @author          
* @version        V1.0.0
* @date           2017-03-04
* @brief          (1)  
                  (2) 
                  (3) 
History:          // Revision Records
   <Author>             <time>        <version >            <desc>
life £¬  Lily         2017-03-04       V1.0.0              ³õ´Î½¨Á¢
***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "HwWDog_Max706.h"



//typedef void(*CmDo_FuncPtr)(uint8_t canPort, uint8_t srcAddr, uint16_t funcId, uint8_t* pBuf);
/* define-----------------------------------------------------------*/
#define HWDOG_Pin    (GPIO_Pin_2)
#define	HWDOG_PT     (GPIOE)

static void HwWDog_Disable(void);
static void HwWDog_Enable(void);


/*******************************************************************************
* Function Name  :void HWDog_Init(void)
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HwWDog_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    rt_kprintf("\r\nHardWear dog Chanals GPIO configure......");

    /* GPIOA Periph clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    /* Configure  in intput pushpull mode */
    //  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_InitStructure.GPIO_Pin = HWDOG_Pin;
    GPIO_Init(HWDOG_PT, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : 
*
* Description    :
* Input          :
* Output         : None
* Return         : None
*******************************************************************************/
static void HwWDog_Enable(void)
{
    GPIO_SetBits(HWDOG_PT, HWDOG_Pin);
}

/*******************************************************************************
* Function Name  :
*
* Description    :
* Input          :
* Output         : None
* Return         : None
*******************************************************************************/
static void HwWDog_Disable(void)
{
    GPIO_ResetBits(HWDOG_PT, HWDOG_Pin);
}

/*******************************************************************************
* Function Name  :
*
* Description    :
* Input          :
* Output         : None
* Return         : None
*******************************************************************************/
void HwWDog_Feed(void)
{
	static uint8_t dogEn = 1;
	
	if(dogEn)
	{
		HwWDog_Enable();
		dogEn = 0;
	}
	else
	{
		HwWDog_Disable();
		dogEn = 1;
	}
}



#include "finsh.h"
//FINSH_FUNCTION_EXPORT(cancm, show system Can Rx data)

/******************* (C) COPYRIGHT 2016 Shenzhen Tongye *****END OF FILE****/
