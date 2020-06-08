


#include "Bsp_bakram.h"
#include "crc16.h"


static void RCC_Configuration(void);
static uint16_t DataGetCRC(void* dataAddr, uint32_t dataSize);
static uint8_t write_bram(__IO void* pbuf, uint16_t addr, uint16_t size);
static uint8_t read_bram(__IO void* pbuf, uint16_t addr, uint16_t size);


//-------------------------------------------------------------------------
// 访问BakRAM指针
//-------------------------------------------------------------------------
__IO uint8_t* _pbram;


//初始化后备SRAM
int System_hw_bakramInit(void)
{
	if (RTC_ReadBackupRegister(RTC_BKP_DR2) != 0x1975)
	{
		RCC_Configuration();
		//---------------------------------------------------------
		// 初始化访问BakRAM指针
		//---------------------------------------------------------
		_pbram = (__IO uint8_t *)BKPSRAM_BASE;
		//---------------------------------------------------------
		// 清0备份寄存区域
		//---------------------------------------------------------
		rt_memset((void *)(_pbram), 0, 4096);

		/* Enable the Backup SRAM low power Regulator to retain it's content in VBAT mode */
		PWR_BackupRegulatorCmd(ENABLE);

		/* Wait until the Backup SRAM low power Regulator is ready */
		while(PWR_GetFlagStatus(PWR_FLAG_BRR) == RESET)
		{
		}

		RTC_WriteBackupRegister(RTC_BKP_DR2, 0x1975);

	}
	else
	{
		RCC_Configuration();
		//---------------------------------------------------------
		// 初始化访问BakRAM指针
		//---------------------------------------------------------
		_pbram = (__IO uint8_t *)BKPSRAM_BASE;
	}

	rt_kprintf ("\r\n+ BKP SRAM (4K) Init OK! \r\n");

	return 0;
}


static void RCC_Configuration(void)
{
	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to RTC */
	PWR_BackupAccessCmd(ENABLE);

	/* Enable BKPRAM Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);
}


//获取数组CRC16校验结果
static uint16_t DataGetCRC(void* dataAddr, uint32_t dataSize)
{
	uint16_t crc = 0xFFFF;

	return CRC16_Dn_Cal((uint8_t*)dataAddr, dataSize, crc);
}


//读取备份寄存器
static uint8_t read_bram(__IO void* pbuf, uint16_t addr, uint16_t size)
{
	uint16_t _crc;
	uint8_t _crch,_crcl;
	//-------------------------------------------------------
	// 读后背寄存器
	//-------------------------------------------------------
	rt_memcpy((void *)pbuf, (void *)(_pbram + addr), size);
	//-------------------------------------------------------
	// 计算CRC16
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


//存储备份寄存器
static uint8_t write_bram(__IO void* pbuf, uint16_t addr, uint16_t size)
{
	uint16_t _crc;
	//-------------------------------------------------------
	// 计算CRC16
	//-------------------------------------------------------
	_crc = DataGetCRC((void *)pbuf, size);
	//-------------------------------------------------------
	// 写后背寄存器
	//-------------------------------------------------------
	rt_memcpy((void *)(_pbram + addr), (void *)pbuf, size);
	*(_pbram + addr + size) = (uint8_t)((_crc & 0xff00) >> 8);
	*(_pbram + addr + size + 1) = (uint8_t)(_crc & 0x00ff);

	return 1;
}


void pbram(void)
{
	uint32_t i;

	rt_kprintf("\r\n  ============ Print Backup SRAM Data ============");
	rt_kprintf("\r\n  Backup SRAM BaseAddr  : 0x%08x", BKPSRAM_BASE);
	rt_kprintf("\r\n  Backup SRAM Size(Byte): %08d", 0x1000);
	rt_kprintf("\r\n\r\n");

	/* 16个字节一行，打印4K Backup SRAM 的内容  */
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


