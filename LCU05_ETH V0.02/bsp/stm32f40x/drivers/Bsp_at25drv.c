
/***************************************************************************

Copyright (C), 1999-2012, Tongye Tech. Co., Ltd.

* @file           Bsp_at25drv.c
* @author         �й���Ŀ��
* @version        V1.0.0
* @date           2012-05-15
* @brief          AT25�������

History:          // Revision Records

 <Author>             <time>       <version >            <desc>

  grape             2012-05-15       V1.0.0             ���ν���

***************************************************************************/

#include <rtthread.h>
#include "Bsp_sysspi.h"
#include "Bsp_at25drv.h"
#include "ds.h"

#define  EraseFullTimeOut       (50 * RT_TICK_PER_SECOND)
#define  Erase4KTimeOut         (RT_TICK_PER_SECOND / 2)
#define  Erase32KTimeOut        (RT_TICK_PER_SECOND)
#define  Erase64KTimeOut        (2  * RT_TICK_PER_SECOND)

rt_uint8_t AT25_ST[2] = {0}; 

u8   read1[1024] = {0};
u8   write1[1024] = {0};

/****************************************************************************
** �������ƣ�static rt_uint8_t ICReadByte(rt_uint8_t data) 
** ��    �ܣ���һ���ֽ�����
** ˵    ���� 
****************************************************************************/

static rt_uint8_t ICReadByte(rt_uint8_t data)
{
	return(SPI_SendByte(SPI_DEVICE_PORT, data));
}

/****************************************************************************
** �������ƣ�static void ICWriteByte(rt_uint8_t data) 
** ��    �ܣ�дһ���ֽ�����
** ˵    ���� 
****************************************************************************/

static void ICWriteByte(rt_uint8_t data)
{
	SPI_SendByte(SPI_DEVICE_PORT, data);
}


/****************************************************************************
** �������ƣ�static void Delay(void)
** ��    �ܣ���ʱ
** ˵    ���� 
****************************************************************************/

static void Delay(void)
{	
	u8 cnt = 0;

	cnt = DELAY;
	while(cnt--);
}


/****************************************************************************
** �������ƣ�static void AT25_Configuration(void)
** ��    �ܣ�����AT25DF321A, �����ƬICд����
** ˵    ������
****************************************************************************/

static void AT25_Configuration(void)                  
{
	//��дʹ��
	FLASH_CS_LOW();
	ICWriteByte(CMD_WREN);
	FLASH_CS_HIGH();
	Delay();
	//����������������
	FLASH_CS_LOW();
	ICWriteByte(CMD_WRSR1);
	ICWriteByte(0x00);
	FLASH_CS_HIGH();
	Delay();
}

/****************************************************************************
** �������ƣ�rt_uint8_t at25_read_status(void) 
** ��    �ܣ���ȡ״̬�Ĵ���
** ˵    ������
****************************************************************************/

static void AT25_Status(void)
{
	ICWriteByte(CMD_RDSR);
	
	AT25_ST[0] = ICReadByte(DUMMY);
	AT25_ST[1] = ICReadByte(DUMMY);
	AT25_ST[0] = ICReadByte(DUMMY);
	AT25_ST[1] = ICReadByte(DUMMY);
}

/****************************************************************************
** �������ƣ�rt_uint8_t AT25_Wait_Busy(void)
** ��    �ܣ���ѯ״̬
** ˵    ������
****************************************************************************/

static rt_uint8_t AT25_Wait_Busy(void)
{
	u32 cnt = 100000;

	FLASH_CS_LOW();

	do 
	{
		AT25_Status();

	} while (--cnt && !(!(AT25_ST[0] & 0x01) && !(AT25_ST[1] & 0x01)));

	FLASH_CS_HIGH();

	if(cnt > 0 )  return AT25_TRUE;

	return AT25_FALSE;
}

/****************************************************************************
** �������ƣ�void at25_wait_Erase(void)
** ��    �ܣ���ѯ״̬
** ˵    ������
****************************************************************************/
#define DELAY_LENGTH (10)
static rt_uint8_t AT25_Wait_Erase(rt_uint8_t Erase_Type, rt_uint32_t _tick)
{
	static u32 _tick_temp;
	u8 _out_time = 0;

	FLASH_CS_LOW();

	do 
	{
        rt_thread_delay(DELAY_LENGTH);

		AT25_Status();

		_tick_temp = rt_tick_get();

		if(_tick_temp >= _tick)
			_tick_temp = _tick_temp - _tick;
		else
			_tick_temp = (0xFFFFFFFF - _tick) + _tick_temp;

		switch(Erase_Type)
		{
		case Sector4KBErase:

			if(_tick_temp > Erase4KTimeOut)     _out_time = 1;
			break;

		case Sector32KBErase:

			if(_tick_temp > Erase32KTimeOut)    _out_time = 1;
			break;

		case Sector64KBErase:

			if(_tick_temp > Erase64KTimeOut)    _out_time = 1;
			break;

		case FullErase:

			if(_tick_temp > EraseFullTimeOut)   _out_time = 1;
			break;

		default:
			break;
		}
	} while (!_out_time && !(!(AT25_ST[0] & 0x01) && !(AT25_ST[1] & 0x01)));

	FLASH_CS_HIGH();

	if(_out_time) return AT25_FALSE;
	return AT25_TRUE;

}

/****************************************************************************
** �������ƣ�rt_uint8_t ICSectorErase(rt_uint8_t Sector,EraseSectorProperty EraseSector_Property) 
** ��    �ܣ���������
** ˵    ���� 
****************************************************************************/

rt_uint8_t  AT25_Sector_Erase(rt_uint8_t Erase_Type, rt_uint32_t offset)      
{
	u8  temp  = 0;
	u8  temp1 = 0;
	u8  temp2 = 0;
	u8  temp3 = 0;
	static u32 _tick;

	if(Erase_Type > FullErase) return AT25_FALSE;

	temp1 = (u8)((offset >> 16) & 0xFF);
	temp2 = (u8)((offset >> 8 ) & 0xFF);
	temp3 = (u8)(offset & 0xFF);

	Delay();
	//��дʹ��
	FLASH_CS_LOW();
	ICWriteByte(CMD_WREN);
	FLASH_CS_HIGH();
	Delay();
	
	switch(Erase_Type)
	{
	case Sector4KBErase:

		FLASH_CS_LOW();
		ICWriteByte(CMD_ERASE_4K);
		ICWriteByte(temp1);
		ICWriteByte(temp2);
		ICWriteByte(temp3);
		FLASH_CS_HIGH();

		break;

	case Sector32KBErase:

		FLASH_CS_LOW();
		ICWriteByte(CMD_ERASE_32K);
		ICWriteByte(temp1);
		ICWriteByte(temp2);
		ICWriteByte(temp3);
		FLASH_CS_HIGH();

		break;

	case Sector64KBErase:

		FLASH_CS_LOW();
		ICWriteByte(CMD_ERASE_64K);
		ICWriteByte(temp1);
		ICWriteByte(temp2);
		ICWriteByte(temp3);
		FLASH_CS_HIGH();

		break;

	case FullErase:

		FLASH_CS_LOW();
		ICWriteByte(CMD_ERASE_full);
		FLASH_CS_HIGH();

		break;
	}

	Delay();

	_tick = rt_tick_get();

	temp = AT25_Wait_Erase(Erase_Type, _tick);

	return temp;
}

/****************************************************************************
** �������ƣ�rt_uint32_t at25_page_write(rt_uint32_t page, const rt_uint8_t * buffer, rt_uint32_t size) 
** ��    �ܣ���ҳд
** ˵    ������
****************************************************************************/

rt_uint8_t AT25_Write(rt_uint32_t page, rt_uint8_t * buffer, rt_uint32_t size)
{
	rt_uint32_t offset = 0;
	rt_uint32_t i      = 0;
	u8  temp  = 0;
	u8  temp1 = 0;
	u8  temp2 = 0;
	u8  temp3 = 0;

	if(size >= PAGE_BYTE)       size   = PAGE_BYTE;

	if(page <= PAGE_NUM_MAX)	
		offset = page * PAGE_BYTE;
	else
		return AT25_FALSE;

	temp1 = (u8)((offset >> 16) & 0xFF);
	temp2 = (u8)((offset >> 8 ) & 0xFF);
	temp3 = (u8)(offset & 0xFF);

	FLASH_CS_LOW();
	ICWriteByte(CMD_WREN);
	FLASH_CS_HIGH();
	Delay();
	FLASH_CS_LOW();
	ICWriteByte(CMD_BP);
	ICWriteByte(temp1);
	ICWriteByte(temp2);
	ICWriteByte(temp3);
	for(i=0; i<size; i++)
	{
		ICWriteByte(*buffer++);
	}
	FLASH_CS_HIGH();
	Delay();

	temp = AT25_Wait_Busy();
	return temp;
}

/****************************************************************************
** �������ƣ�rt_size_t AT25_read(rt_off_t page, rt_uint8_t* buffer, rt_size_t size)
** ��    �ܣ�������
** ˵    ������
****************************************************************************/

rt_uint8_t AT25_Read(rt_uint32_t page, rt_uint8_t* buffer, rt_uint32_t size)
{
	rt_uint32_t i      = 0;
	rt_uint32_t offset = 0;
	u8  temp  = 0;
	u8  temp1 = 0;
	u8  temp2 = 0;
	u8  temp3 = 0;

	if(size >= PAGE_BYTE)       size   = PAGE_BYTE;

	if(page <= PAGE_NUM_MAX)	
		offset = page * PAGE_BYTE;
	else
		return AT25_FALSE;
	
	temp1 = (u8)((offset >> 16) & 0xFF);
	temp2 = (u8)((offset >> 8 ) & 0xFF);
	temp3 = (u8)(offset & 0xFF);

	Delay();
	FLASH_CS_LOW();
	ICWriteByte(CMD_WRDI);
	FLASH_CS_HIGH();

	Delay();
	FLASH_CS_LOW();
	ICWriteByte(CMD_READ_MODE3);
	ICWriteByte(temp1);
	ICWriteByte(temp2);
	ICWriteByte(temp3);
	for(i=0;i<size;i++)
	{
		*buffer++ = ICReadByte(DUMMY);
	}

	FLASH_CS_HIGH();

	Delay();
	temp = AT25_Wait_Busy();

	return temp;
}

/****************************************************************************
** �������ƣ�static void AT25_Test(void) 
** ��    �ܣ�
** ˵    ������
****************************************************************************/

void AT25_Test(rt_uint32_t page, rt_uint8_t first_value)
{
	rt_uint32_t offset = 0;
	rt_uint16_t i = 0;

	for(i=0; i<1024; i++)
	{
		write1[i] = (u8)((i + first_value) & 0xFF);
	}

	if(page <= PAGE_NUM_MAX)	offset = page * PAGE_BYTE;

	offset = offset & 0xFFFFF000;

	AT25_Sector_Erase(Sector4KBErase, offset);
	//AT25_Sector_Erase(FullErase, 0);

	AT25_Write( page    , &write1[0]  , PAGE_BYTE);
//	AT25_Write((page+1) , &write1[256], PAGE_BYTE);
//	AT25_Write((page+2) , &write1[512], PAGE_BYTE);
//	AT25_Write((page+3) , &write1[768], PAGE_BYTE);

	AT25_Read( page    , &read1[0]  , PAGE_BYTE);
//	AT25_Read((page+1) , &read1[256], PAGE_BYTE);
//	AT25_Read((page+2) , &read1[512], PAGE_BYTE);
//	AT25_Read((page+3) , &read1[768], PAGE_BYTE);
	
	for(i = 0; i <200 ; i++)
	{
		if( write1[i]!= read1[i] )
		{
				rt_kprintf("\rmemtest fail @ %d\r\nsystem halt!!!!!",i);
				while(1);
		}
	}
	rt_kprintf("\r\nFlash mem test pass!!\r\n");


	
}

/****************************************************************************
** �������ƣ�int AT25_ICInit(void) 
** ��    �ܣ���ʼ��AT25DF321A
** ˵    ������
****************************************************************************/
int AT25_ICInit(void)                  
{
	rt_uint8_t id_recv[4];

	FLASH_CS_LOW();
	ICWriteByte(CMD_DEVICE_ID);
	id_recv[0] = ICReadByte(DUMMY);
	id_recv[1] = ICReadByte(DUMMY);
	id_recv[2] = ICReadByte(DUMMY);
	id_recv[3] = ICReadByte(DUMMY);
	FLASH_CS_HIGH();
  
	
	AT25_Configuration();
	

	
	//�ж�FLASH�������Ƿ�ATMEL��˾��AT25DB32
	if(    (id_recv[0] == MF_ID) 
		&& (id_recv[1] == MT_ID)
		)
	{
		return (FUNC_RET_OK);
	}
	else
	{
		return(FUNC_RET_ERR);
	}
}

/******************* (C) COPYRIGHT 2012 Shenzhen Tongye *****END OF FILE****/

