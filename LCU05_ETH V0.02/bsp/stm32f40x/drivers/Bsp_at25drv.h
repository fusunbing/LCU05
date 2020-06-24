

/***************************************************************************

Copyright (C), 1999-2012, Tongye Tech. Co., Ltd.

* @file           Bsp_at25drv.h
* @author         �й���Ŀ��
* @version        V1.0.0
* @date           2012-05-15
* @brief          AT25�������

History:          // Revision Records

       <Author>             <time>       <version >            <desc>
                               
        grape             2012-05-15       V1.0.0             ���ν���
												 
***************************************************************************/

#ifndef __BSP_AT25DRV_H__
#define __BSP_AT25DRV_H__

#include <stm32f4xx.h>

//-----------------------------------------------------------------------------------
// SPI PORT
//-----------------------------------------------------------------------------------
#define SPI_DEVICE_PORT             (SPI2)
//-----------------------------------------------------------------------------------
// Device Information
//-----------------------------------------------------------------------------------
// Manufacturer ID
#define MF_ID                       (0x1F)
// Device ID : Memory Type
#define MT_ID                       (0x48)
//-----------------------------------------------------------------------------------
// Memory Architecture
//-----------------------------------------------------------------------------------
#define PAGE_BYTE                   (0x100)
#define MAX_PAGE_ADDR               (0x3FFF00)
#define PAGE_NUM_MAX                (0x4000)
//-----------------------------------------------------------------------------------
// COMMAND LIST
//-----------------------------------------------------------------------------------
// Read Commands
#define CMD_READ_MODE1              (0x1B)  /* ģʽ1������  Up to 100MHz */
#define CMD_READ_MODE2              (0x0B)  /* ģʽ2������  Up to 85MHz  */
#define CMD_READ_MODE3              (0x03)  /* ģʽ3������  Up to 50MHz  */
#define CMD_ARRAY_READ              (0x3B)  /* �Զ���ַ����������        */
// Program and Erase Commands
#define CMD_ERASE_4K                (0x20)  /* ��������:4K        */
#define CMD_ERASE_32K               (0x52)  /* ��������:32K       */
#define CMD_ERASE_64K               (0xD8)  /* ��������:64K       */
#define CMD_ERASE_full              (0xC7)  /* ȫƬ����           */
#define CMD_BP                      (0x02)  /* �ֽڱ��           */
#define CMD_AAIP                    (0xA2)  /* �Զ���ַ�������   */
#define CMD_PE_SUSPEND              (0xB0)  /* ������д��������   */
#define CMD_PE_RESUME               (0xD0)  /* ������д�����ָ�   */
// Protection Commands
#define CMD_WRDI                    (0x04)  /* �ر�дʹ��         */
#define CMD_WREN                    (0x06)  /* ��дʹ��         */
#define CMD_PTSEC                   (0x36)  /* ��������           */
#define CMD_UPTSEC                  (0x39)  /* ������������       */
#define CMD_RDPR                    (0x3C)  /* ��ȡ���������Ĵ��� */
// Status Register Commands
#define CMD_RDSR                    (0x05)  /* ��״̬�Ĵ���       */
#define CMD_WRSR1                   (0x01)  /* д״̬�Ĵ���1      */
#define CMD_WRSR2                   (0x31)  /* д״̬�Ĵ���2      */
// Miscellaneous Commands
#define CMD_RESET                   (0xF0)  /* ��λ               */
#define CMD_DEVICE_ID               (0x9F)  /* ��ȡоƬID         */
#define DUMMY                       (0xFF)

//-----------------------------------------------------------------------------------
// DELAY
//-----------------------------------------------------------------------------------
#define DELAY                       (10)

//-----------------------------------------------------------------------------------
// Erase Sector Type
//-----------------------------------------------------------------------------------
#define Sector4KBErase              (1)
#define Sector32KBErase             (2)
#define Sector64KBErase             (3)
#define FullErase                   (4)

//-----------------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------------
#define AT25_TRUE                   (1)
#define AT25_FALSE                  (0)

int AT25_ICInit(void);
rt_uint8_t AT25_Write(rt_uint32_t page, rt_uint8_t * buffer, rt_uint32_t size);
rt_uint8_t AT25_Read(rt_uint32_t pos, rt_uint8_t* buffer, rt_uint32_t size);
rt_uint8_t  AT25_Sector_Erase(rt_uint8_t Erase_Type, rt_uint32_t offset);     
void AT25_Test(rt_uint32_t page, rt_uint8_t first_value);

#endif






/******************* (C) COPYRIGHT 2012 Shenzhen Tongye *****END OF FILE****/

