
/***************************************************************************

Copyright (C), 1999-2012, Tech. Co., Ltd.

* @file           Bsp_bakram.c
* @author      
* @version        V1.0.0
* @date           2012-04-28
* @brief          ���ݼĴ����������

History:          // Revision Records

<Author>             <time>       <version >            <desc>


***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include "Bsp_bakram.h"
#include "crc16.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void RCC_Configuration(void);
static uint16_t DataGetCRC(void* dataAddr, uint32_t dataSize);
static uint8_t write_bram(__IO void* pbuf, uint16_t addr, uint16_t size);
static uint8_t read_bram(__IO void* pbuf, uint16_t addr, uint16_t size);
//-------------------------------------------------------------------------
// ����BakRAMָ��
//-------------------------------------------------------------------------
__IO uint8_t* _pbram;

/*******************************************************************************
* Function Name  : System_hw_bakramInit
* Description    : ��ʼ����SRAM
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

int System_hw_bakramInit(void)
{
	if (RTC_ReadBackupRegister(RTC_BKP_DR2) != 0x1975)
	{
		RCC_Configuration();
		//---------------------------------------------------------
		// ��ʼ������BakRAMָ��
		//---------------------------------------------------------
		_pbram = (__IO uint8_t *)BKPSRAM_BASE;
		//---------------------------------------------------------
		// ��0���ݼĴ�����
		//---------------------------------------------------------
		rt_memset((void *)(_pbram), 0, 4096);

		/* Enable the Backup SRAM low power Regulator to retain it's content in VBAT mode */
		PWR_BackupRegulatorCmd(ENABLE);

		/* Wait until the Backup SRAM low power Regulator is ready */
		while(PWR_GetFlagStatus(PWR_FLAG_BRR) == RESET)
		{
		}

		RTC_WriteBackupRegister(RTC_BKP_DR2, 0x1975);

		//PDS(BI)->BakRamInit = 1;
	}
	else
	{
		RCC_Configuration();
		//---------------------------------------------------------
		// ��ʼ������BakRAMָ��
		//---------------------------------------------------------
		_pbram = (__IO uint8_t *)BKPSRAM_BASE;
	}

	//�ҽ� �������� ����ģ��
	//PDS(BRam)->pWrite = write_bram;
	//�ҽ� �������� ����ģ��
	//PDS(BRam)->pRead = read_bram;

	rt_kprintf ("\r\nBKP SRAM (4K) Init OK!");

	return 0;
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

static void RCC_Configuration(void)
{
	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to RTC */
	PWR_BackupAccessCmd(ENABLE);

	/* Enable BKPRAM Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);
}


/*********************************************************************************************************
** Function name:     uint16_t DataGetCRC(void* dataAddr, uint32_t dataSize)
** Descriptions:      ��ȡ����CRC16У����
** input parameters:  ��
** output parameters: �� 
** Returned value:    ��
*********************************************************************************************************/

static uint16_t DataGetCRC(void* dataAddr, uint32_t dataSize)
{
	uint16_t crc = 0xFFFF;

	return CRC16_Dn_Cal((uint8_t*)dataAddr, dataSize, crc);
}

/*******************************************************************************
* Function Name  : static uint8_t read_bram(void* pbuf, uint16_t addr, uint16_t size)
* Description    : ��ȡ���ݼĴ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

static uint8_t read_bram(__IO void* pbuf, uint16_t addr, uint16_t size)
{
	uint16_t _crc;
	uint8_t _crch,_crcl;
	//-------------------------------------------------------
	// ���󱳼Ĵ���
	//-------------------------------------------------------
	rt_memcpy((void *)pbuf, (void *)(_pbram + addr), size);
	//-------------------------------------------------------
	// ����CRC16
	//-------------------------------------------------------
	_crc = DataGetCRC((void *)pbuf, size);
	_crch = (uint8_t)((_crc & 0xff00) >> 8);
	_crcl = (uint8_t)(_crc & 0x00ff);
	if( (_crch == *(_pbram + addr + size))
	 && (_crcl == *(_pbram + addr + size + 1)))
	{
		return 1;
	}
	return 0;
}

/*******************************************************************************
* Function Name  : static uint8_t write_bram(void* pbuf, uint16_t addr, uint16_t size)
* Description    : �洢���ݼĴ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

static uint8_t write_bram(__IO void* pbuf, uint16_t addr, uint16_t size)
{
	uint16_t _crc;
	//-------------------------------------------------------
	// ����CRC16
	//-------------------------------------------------------
	_crc = DataGetCRC((void *)pbuf, size);
	//-------------------------------------------------------
	// д�󱳼Ĵ���
	//-------------------------------------------------------
	rt_memcpy((void *)(_pbram + addr), (void *)pbuf, size);
	*(_pbram + addr + size) = (uint8_t)((_crc & 0xff00) >> 8);
	*(_pbram + addr + size + 1) = (uint8_t)(_crc & 0x00ff);

	return 1;
}

/*******************************************************************************
* Function Name  : void pbram(void)
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void pbram(void)
{
	uint32_t i;

	rt_kprintf("\r\n  ============ Print Backup SRAM Data ============");
	rt_kprintf("\r\n  Backup SRAM BaseAddr  : 0x%08x", BKPSRAM_BASE);
	rt_kprintf("\r\n  Backup SRAM Size(Byte): %08d", 0x1000);
	rt_kprintf("\r\n\r\n");

	/* 16���ֽ�һ�У���ӡ4K Backup SRAM ������  */
	for (i = 0x0; i < 0x1000; i += 0x10)
	{
		rt_kprintf("0x%08x   ", (BKPSRAM_BASE + i));
		rt_kprintf("%02x  %02x  %02x  %02x  %02x  %02x  %02x  %02x  ",
			(*(__IO uint8_t *) (BKPSRAM_BASE + i + 0)),
			(*(__IO uint8_t *) (BKPSRAM_BASE + i + 1)),
			(*(__IO uint8_t *) (BKPSRAM_BASE + i + 2)),
			(*(__IO uint8_t *) (BKPSRAM_BASE + i + 3)),
			(*(__IO uint8_t *) (BKPSRAM_BASE + i + 4)),
			(*(__IO uint8_t *) (BKPSRAM_BASE + i + 5)),
			(*(__IO uint8_t *) (BKPSRAM_BASE + i + 6)),
			(*(__IO uint8_t *) (BKPSRAM_BASE + i + 7))
			);
        rt_kprintf("%02x  %02x  %02x  %02x  %02x  %02x  %02x  %02x",
			(*(__IO uint8_t *) (BKPSRAM_BASE + i + 8)),
			(*(__IO uint8_t *) (BKPSRAM_BASE + i + 9)),
			(*(__IO uint8_t *) (BKPSRAM_BASE + i + 10)),
			(*(__IO uint8_t *) (BKPSRAM_BASE + i + 11)),
			(*(__IO uint8_t *) (BKPSRAM_BASE + i + 12)),
			(*(__IO uint8_t *) (BKPSRAM_BASE + i + 13)),
			(*(__IO uint8_t *) (BKPSRAM_BASE + i + 14)),
			(*(__IO uint8_t *) (BKPSRAM_BASE + i + 15))
			);
		rt_kprintf("\r\n");
	}
}


#include "finsh.h"
FINSH_FUNCTION_EXPORT(pbram, Print Backup SRAM Data)

/******************* (C) COPYRIGHT 2012  *****END OF FILE****/


