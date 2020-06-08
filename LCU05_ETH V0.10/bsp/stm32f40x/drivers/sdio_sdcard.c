/***************************************************************************

Copyright (C), 1999-2014, Tongye Tech. Co., Ltd.

* @file           sdio_sdcard.c
* @author        
* @version        V1.0.0
* @date           2016-06-29
* @brief          (1)  
                  (2) 
                  (3) 
History:          // Revision Records

<Author>             <time>       <version >            <desc>

care               2016-06-29       V1.0.0               

***************************************************************************/
/* Includes --------------------------------------------------------------*/
#include "sdio_sdcard.h"
#include "rthw.h"
#include "dfs_def.h"
#include "bsp_fatservice.h"
//---------------------------------------------------------------------------
//                        Task Macro 
//---------------------------------------------------------------------------
#define BSP_SD_DRIVER_STACKSIZE   (1 * 1024 / 4)  
#define BSP_SD_DRIVER_PRIORITY    (0x0a)
//---------------------------------------------------------------------------
//                        Sd Macro 
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//SDIO 指令集
#define SD_CMD_GO_IDLE_STATE                       ((u8)0)
#define SD_CMD_SEND_OP_COND                        ((u8)1)
#define SD_CMD_ALL_SEND_CID                        ((u8)2)
#define SD_CMD_SET_REL_ADDR                        ((u8)3) /*!< SDIO_SEND_REL_ADDR for SD Card */
#define SD_CMD_SET_DSR                             ((u8)4)
#define SD_CMD_SDIO_SEN_OP_COND                    ((u8)5)
#define SD_CMD_HS_SWITCH                           ((u8)6)
#define SD_CMD_SEL_DESEL_CARD                      ((u8)7)
#define SD_CMD_HS_SEND_EXT_CSD                     ((u8)8)
#define SD_CMD_SEND_CSD                            ((u8)9)
#define SD_CMD_SEND_CID                            ((u8)10)
#define SD_CMD_READ_DAT_UNTIL_STOP                 ((u8)11) /*!< SD Card doesn't support it */
#define SD_CMD_STOP_TRANSMISSION                   ((u8)12)
#define SD_CMD_SEND_STATUS                         ((u8)13)
#define SD_CMD_HS_BUSTEST_READ                     ((u8)14)
#define SD_CMD_GO_INACTIVE_STATE                   ((u8)15)
#define SD_CMD_SET_BLOCKLEN                        ((u8)16)
#define SD_CMD_READ_SINGLE_BLOCK                   ((u8)17)
#define SD_CMD_READ_MULT_BLOCK                     ((u8)18)
#define SD_CMD_HS_BUSTEST_WRITE                    ((u8)19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP                ((u8)20) 
#define SD_CMD_SET_BLOCK_COUNT                     ((u8)23) 
#define SD_CMD_WRITE_SINGLE_BLOCK                  ((u8)24)
#define SD_CMD_WRITE_MULT_BLOCK                    ((u8)25)
#define SD_CMD_PROG_CID                            ((u8)26)
#define SD_CMD_PROG_CSD                            ((u8)27)
#define SD_CMD_SET_WRITE_PROT                      ((u8)28)
#define SD_CMD_CLR_WRITE_PROT                      ((u8)29)
#define SD_CMD_SEND_WRITE_PROT                     ((u8)30)
#define SD_CMD_SD_ERASE_GRP_START                  ((u8)32) /*!< To set the address of the first write
                                                                  block to be erased. (For SD card only) */
#define SD_CMD_SD_ERASE_GRP_END                    ((u8)33) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For SD card only) */
#define SD_CMD_ERASE_GRP_START                     ((u8)35) /*!< To set the address of the first write block to be erased.
                                                                  (For MMC card only spec 3.31) */

#define SD_CMD_ERASE_GRP_END                       ((u8)36) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For MMC card only spec 3.31) */

#define SD_CMD_ERASE                               ((u8)38)
#define SD_CMD_FAST_IO                             ((u8)39) /*!< SD Card doesn't support it */
#define SD_CMD_GO_IRQ_STATE                        ((u8)40) /*!< SD Card doesn't support it */
#define SD_CMD_LOCK_UNLOCK                         ((u8)42)
#define SD_CMD_APP_CMD                             ((u8)55)
#define SD_CMD_GEN_CMD                             ((u8)56)
#define SD_CMD_NO_CMD                              ((u8)64)

/** 
  * @brief Following commands are SD Card Specific commands.
  *        SDIO_APP_CMD ：CMD55 should be sent before sending these commands. 
  */
#define SD_CMD_APP_SD_SET_BUSWIDTH                 ((u8)6)  /*!< For SD Card only */
#define SD_CMD_SD_APP_STAUS                        ((u8)13) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((u8)22) /*!< For SD Card only */
#define SD_CMD_SD_APP_OP_COND                      ((u8)41) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT          ((u8)42) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_SCR                     ((u8)51) /*!< For SD Card only */
#define SD_CMD_SDIO_RW_DIRECT                      ((u8)52) /*!< For SD I/O Card only */
#define SD_CMD_SDIO_RW_EXTENDED                    ((u8)53) /*!< For SD I/O Card only */

/** 
  * @brief Following commands are SD Card Specific security commands.
  *        SDIO_APP_CMD should be sent before sending these commands. 
  */
#define SD_CMD_SD_APP_GET_MKB                      ((u8)43) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_MID                      ((u8)44) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RN1                  ((u8)45) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RN2                  ((u8)46) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RES2                 ((u8)47) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RES1                 ((u8)48) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((u8)18) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((u8)25) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_ERASE                 ((u8)38) /*!< For SD Card only */
#define SD_CMD_SD_APP_CHANGE_SECURE_AREA           ((u8)49) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MKB             ((u8)48) /*!< For SD Card only */
  			   
//支持的SD卡定义
#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((u32)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((u32)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((u32)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((u32)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((u32)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((u32)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((u32)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((u32)0x00000007)

//SDIO相关参数定义
#define NULL 0
#define SDIO_STATIC_FLAGS               ((u32)0x000005FF)
#define SDIO_CMD0TIMEOUT                ((u32)0x00010000)	  
#define SDIO_DATATIMEOUT                ((u32)0xFFFFFFFF)	  
#define SDIO_FIFO_Address               ((u32)0x40018080)

//Mask for errors Card Status R1 (OCR Register)  
#define SD_OCR_ADDR_OUT_OF_RANGE        ((u32)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((u32)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((u32)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((u32)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((u32)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((u32)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((u32)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((u32)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((u32)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((u32)0x00200000)
#define SD_OCR_CC_ERROR                 ((u32)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((u32)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((u32)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((u32)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE       ((u32)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((u32)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((u32)0x00004000)
#define SD_OCR_ERASE_RESET              ((u32)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((u32)0x00000008)
#define SD_OCR_ERRORBITS                ((u32)0xFDFFE008)

//Masks for R6 Response 
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((u32)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((u32)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((u32)0x00008000)

#define SD_VOLTAGE_WINDOW_SD            ((u32)0x80100000)
#define SD_HIGH_CAPACITY                ((u32)0x40000000)
#define SD_STD_CAPACITY                 ((u32)0x00000000)
#define SD_CHECK_PATTERN                ((u32)0x000001AA)
#define SD_VOLTAGE_WINDOW_MMC           ((u32)0x80FF8000)

#define SD_MAX_VOLT_TRIAL               ((u32)0x0000FFFF)
#define SD_ALLZERO                      ((u32)0x00000000)

#define SD_WIDE_BUS_SUPPORT             ((u32)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((u32)0x00010000)
#define SD_CARD_LOCKED                  ((u32)0x02000000)
#define SD_CARD_PROGRAMMING             ((u32)0x00000007)
#define SD_CARD_RECEIVING               ((u32)0x00000006)
#define SD_DATATIMEOUT                  ((u32)0xFFFFFFFF)
#define SD_0TO7BITS                     ((u32)0x000000FF)
#define SD_8TO15BITS                    ((u32)0x0000FF00)
#define SD_16TO23BITS                   ((u32)0x00FF0000)
#define SD_24TO31BITS                   ((u32)0xFF000000)
#define SD_MAX_DATA_LENGTH              ((u32)0x01FFFFFF)

#define SD_HALFFIFO                     ((u32)0x00000008)
#define SD_HALFFIFOBYTES                ((u32)0x00000020)

//Command Class Supported  
#define SD_CCCC_LOCK_UNLOCK             ((u32)0x00000080)
#define SD_CCCC_WRITE_PROT              ((u32)0x00000040)
#define SD_CCCC_ERASE                   ((u32)0x00000020)
																	 
//CMD8指令
#define SDIO_SEND_IF_COND               ((u32)0x00000008)
/* 用户配置区	 --------------------------------------------------------------*/
//SDIO时钟计算公式:SDIO_CK时钟=SDIOCLK/[clkdiv+2];其中,SDIOCLK固定为48Mhz
//使用DMA模式的时候,传输速率可以到48Mhz(bypass on时),不过如果你的卡不是高速
//卡,可能也会出错,出错就请降低时钟
#define SDIO_INIT_CLK_DIV        0x76 		//SDIO初始化频率，最大400Kh  
#define SDIO_TRANSFER_CLK_DIV    0x00		//SDIO传输频率,该值太小可能会导致读写文件出错 
/* SDIO工作模式定义,通过SD_SetDeviceMode函数设置.	 --------------------------*/
#define SD_POLLING_MODE    	0  	//查询模式,该模式下,如果读写有问题,建议增大SDIO_TRANSFER_CLK_DIV的设置.
#define SD_DMA_MODE    		1	//DMA模式,该模式下,如果读写有问题,建议增大SDIO_TRANSFER_CLK_DIV的设置.   
//---------------------------------------------------------------------------
//                        Private Typedef Object
//---------------------------------------------------------------------------
/* SDIO 各种错误枚举定义	 -------------------------------------------------*/
typedef enum
{	 
	//特殊错误定义 
	SD_CMD_CRC_FAIL                    = (1), /*!< Command response received (but CRC check failed) */
	SD_DATA_CRC_FAIL                   = (2), /*!< Data bock sent/received (CRC check Failed) */
	SD_CMD_RSP_TIMEOUT                 = (3), /*!< Command response timeout */
	SD_DATA_TIMEOUT                    = (4), /*!< Data time out */
	SD_TX_UNDERRUN                     = (5), /*!< Transmit FIFO under-run */
	SD_RX_OVERRUN                      = (6), /*!< Receive FIFO over-run */
	SD_START_BIT_ERR                   = (7), /*!< Start bit not detected on all data signals in widE bus mode */
	SD_CMD_OUT_OF_RANGE                = (8), /*!< CMD's argument was out of range.*/
	SD_ADDR_MISALIGNED                 = (9), /*!< Misaligned address */
	SD_BLOCK_LEN_ERR                   = (10), /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
	SD_ERASE_SEQ_ERR                   = (11), /*!< An error in the sequence of erase command occurs.*/
	SD_BAD_ERASE_PARAM                 = (12), /*!< An Invalid selection for erase groups */
	SD_WRITE_PROT_VIOLATION            = (13), /*!< Attempt to program a write protect block */
	SD_LOCK_UNLOCK_FAILED              = (14), /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
	SD_COM_CRC_FAILED                  = (15), /*!< CRC check of the previous command failed */
	SD_ILLEGAL_CMD                     = (16), /*!< Command is not legal for the card state */
	SD_CARD_ECC_FAILED                 = (17), /*!< Card internal ECC was applied but failed to correct the data */
	SD_CC_ERROR                        = (18), /*!< Internal card controller error */
	SD_GENERAL_UNKNOWN_ERROR           = (19), /*!< General or Unknown error */
	SD_STREAM_READ_UNDERRUN            = (20), /*!< The card could not sustain data transfer in stream read operation. */
	SD_STREAM_WRITE_OVERRUN            = (21), /*!< The card could not sustain data programming in stream mode */
	SD_CID_CSD_OVERWRITE               = (22), /*!< CID/CSD overwrite error */
	SD_WP_ERASE_SKIP                   = (23), /*!< only partial address space was erased */
	SD_CARD_ECC_DISABLED               = (24), /*!< Command has been executed without using internal ECC */
	SD_ERASE_RESET                     = (25), /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
	SD_AKE_SEQ_ERROR                   = (26), /*!< Error in sequence of authentication. */
	SD_INVALID_VOLTRANGE               = (27),
	SD_ADDR_OUT_OF_RANGE               = (28),
	SD_SWITCH_ERROR                    = (29),
	SD_SDIO_DISABLED                   = (30),
	SD_SDIO_FUNCTION_BUSY              = (31),
	SD_SDIO_FUNCTION_FAILED            = (32),
	SD_SDIO_UNKNOWN_FUNCTION           = (33),
	//标准错误定义
	SD_INTERNAL_ERROR, 
	SD_NOT_CONFIGURED,
	SD_REQUEST_PENDING, 
	SD_REQUEST_NOT_APPLICABLE, 
	SD_INVALID_PARAMETER,  
	SD_UNSUPPORTED_FEATURE,  
	SD_UNSUPPORTED_HW,  
	SD_ERROR,  
	SD_OK = 0 
} SD_Error;	
/* SD卡CSD寄存器数据		 -------------------------------------------------*/	  
typedef struct
{
	u8  CSDStruct;            /*!< CSD structure */
	u8  SysSpecVersion;       /*!< System specification version */
	u8  Reserved1;            /*!< Reserved */
	u8  TAAC;                 /*!< Data read access-time 1 */
	u8  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
	u8  MaxBusClkFrec;        /*!< Max. bus clock frequency */
	u16 CardComdClasses;      /*!< Card command classes */
	u8  RdBlockLen;           /*!< Max. read data block length */
	u8  PartBlockRead;        /*!< Partial blocks for read allowed */
	u8  WrBlockMisalign;      /*!< Write block misalignment */
	u8  RdBlockMisalign;      /*!< Read block misalignment */
	u8  DSRImpl;              /*!< DSR implemented */
	u8  Reserved2;            /*!< Reserved */
	u32 DeviceSize;           /*!< Device Size */
	u8  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
	u8  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
	u8  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
	u8  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
	u8  DeviceSizeMul;        /*!< Device size multiplier */
	u8  EraseGrSize;          /*!< Erase group size */
	u8  EraseGrMul;           /*!< Erase group size multiplier */
	u8  WrProtectGrSize;      /*!< Write protect group size */
	u8  WrProtectGrEnable;    /*!< Write protect group enable */
	u8  ManDeflECC;           /*!< Manufacturer default ECC */
	u8  WrSpeedFact;          /*!< Write speed factor */
	u8  MaxWrBlockLen;        /*!< Max. write data block length */
	u8  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
	u8  Reserved3;            /*!< Reserded */
	u8  ContentProtectAppli;  /*!< Content protection application */
	u8  FileFormatGrouop;     /*!< File format group */
	u8  CopyFlag;             /*!< Copy flag (OTP) */
	u8  PermWrProtect;        /*!< Permanent write protection */
	u8  TempWrProtect;        /*!< Temporary write protection */
	u8  FileFormat;           /*!< File Format */
	u8  ECC;                  /*!< ECC code */
	u8  CSD_CRC;              /*!< CSD CRC */
	u8  Reserved4;            /*!< always 1*/
} SD_CSD;   
/* SD卡CID寄存器数据	 -------------------------------------------------*/
typedef struct
{
	u8  ManufacturerID;       /*!< ManufacturerID */
	u16 OEM_AppliID;          /*!< OEM/Application ID */
	u32 ProdName1;            /*!< Product Name part1 */
	u8  ProdName2;            /*!< Product Name part2*/
	u8  ProdRev;              /*!< Product Revision */
	u32 ProdSN;               /*!< Product Serial Number */
	u8  Reserved1;            /*!< Reserved1 */
	u16 ManufactDate;         /*!< Manufacturing Date */
	u8  CID_CRC;              /*!< CID CRC */
	u8  Reserved2;            /*!< always 1 */
} SD_CID;	 
/* SD卡状态	 -------------------------------------------------*/
typedef enum
{
	SD_CARD_READY                  = ((uint32_t)0x00000001),
	SD_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),
	SD_CARD_STANDBY                = ((uint32_t)0x00000003),
	SD_CARD_TRANSFER               = ((uint32_t)0x00000004),
	SD_CARD_SENDING                = ((uint32_t)0x00000005),
	SD_CARD_RECEIVINGG              = ((uint32_t)0x00000006),
	SD_CARD_PROGRAMMINGG            = ((uint32_t)0x00000007),
	SD_CARD_DISCONNECTED           = ((uint32_t)0x00000008),
	SD_CARD_ERROR                  = ((uint32_t)0x000000FF)
}SDCardState;
/* SD卡信息,包括CSD,CID等数据 -------------------------------------------------*/
typedef struct
{
  SD_CSD SD_csd;
  SD_CID SD_cid;
  long long CardCapacity;  	//SD卡容量,单位:字节,最大支持2^64字节大小的卡.
  u32 CardBlockSize; 		//SD卡块大小	
  u16 RCA;					//卡相对地址
  u8 CardType;				//卡类型
} SD_CardInfo;
//---------------------------------------------------------------------------
//                        Sd Object
//---------------------------------------------------------------------------
static u8 CardType=SDIO_STD_CAPACITY_SD_CARD_V1_1;		//SD卡类型（默认为1.x卡）
static u32 CSD_Tab[4],CID_Tab[4],RCA=0;					//SD卡CSD,CID以及相对地址(RCA)数据
static u8 DeviceMode = SD_DMA_MODE;		   				//工作模式,注意,工作模式必须通过SD_SetDeviceMode,后才算数.这里只是定义一个默认的模式(SD_DMA_MODE)
static u8 StopCondition = 0; 								//是否发送停止传输标志位,DMA多块读写的时候用到
volatile SD_Error TransferError=SD_OK;					//数据传输错误标志,DMA读写时使用	    
volatile u8 TransferEnd=0;								//传输结束标志,DMA读写时使用
static SD_CardInfo SDCardInfo;//SD卡信息	
/*用于sdio初始化的结构体*/
static SDIO_InitTypeDef SDIO_InitStructure;
static SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
static SDIO_DataInitTypeDef SDIO_DataInitStructure;
/*用于FAT系统的设备结构体*/
static struct rt_device sdcard_device;
//SD_ReadDisk/SD_WriteDisk函数专用buf,当这两个函数的数据缓存区地址不是4字节对齐的时候,
//需要用到该数组,确保数据缓存区地址是4字节对齐的.
__align(4) u8 SDIO_DATA_BUFFER[512];	
//---------------------------------------------------------------------------
//                        Task Object
//---------------------------------------------------------------------------
static rt_uint32_t Bsp_SdDriver_Stack[BSP_SD_DRIVER_STACKSIZE];
static struct rt_thread Bsp_SdDriver_thread;
//---------------------------------------------------------------------------
//                        Sem Object
//---------------------------------------------------------------------------
static struct rt_semaphore sd_lock;
//---------------------------------------------------------------------------
//                      Private functions 
//---------------------------------------------------------------------------
static void    Bsp_SdDriver_Task(void* parameter);
static SD_Error SD_Init(void);
static void SDIO_Register_Deinit(void);
static SD_Error SD_PowerON(void);
static SD_Error CmdError(void);
static SD_Error CmdResp7Error(void);
static SD_Error CmdResp1Error(u8);
static SD_Error CmdResp3Error(void);
static SD_Error SD_InitializeCards(void);
static SD_Error CmdResp2Error(void);
static SD_Error CmdResp6Error(u8 cmd,u16*prca);
static SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo);
static SD_Error SD_SelectDeselect(u32 addr);
static SD_Error SD_EnableWideBusOperation(u32 WideMode);
static SD_Error SDEnWideBus(u8 enx);
static SD_Error FindSCR(u16 rca,u32 *pscr);
static void SDIO_Clock_Set(u8 clkdiv);
static SD_Error SD_SetDeviceMode(u32 Mode);
static u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt);
static SD_Error SD_ReadBlock(u8 *buf,long long addr,u16 blksize);
static u8 convert_from_bytes_to_power_of_two(u16 NumberOfBytes);
static void SD_DMA_Config(u32*mbuf,u32 bufsize,u32 dir);
static SD_Error SD_ReadMultiBlocks(u8 *buf,long long addr,u16 blksize,u32 nblks);
static u8 SD_WriteDisk(u8*buf,u32 sector,u8 cnt);
static SD_Error SD_WriteBlock(u8 *buf,long long addr,  u16 blksize);
static SD_Error IsCardProgramming(u8 *pstatus);
static SD_Error SD_WriteMultiBlocks(u8 *buf,long long addr,u16 blksize,u32 nblks);
static void SD_test(u32 num);
/*用于FAT系统的操作函数*/
static rt_err_t rt_sdcard_init(rt_device_t dev);
static rt_err_t rt_sdcard_open(rt_device_t dev, rt_uint16_t oflag);
static rt_err_t rt_sdcard_close(rt_device_t dev);
static rt_size_t rt_sdcard_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size);
static rt_size_t rt_sdcard_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size);
static rt_err_t rt_sdcard_control(rt_device_t dev, rt_uint8_t cmd, void *args);
/*******************************************************************************
* Description    : 
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  Bsp_SdDriver_CreateApp(void)
{

    rt_thread_init(&Bsp_SdDriver_thread,
        "SdDrv",
        Bsp_SdDriver_Task,
        RT_NULL,
        &Bsp_SdDriver_Stack[0], 
        sizeof(Bsp_SdDriver_Stack),
        BSP_SD_DRIVER_PRIORITY, 
        20
        );

    rt_thread_startup(&Bsp_SdDriver_thread);
	
}

static void    Bsp_SdDriver_Task(void* parameter)
{
		while(SD_Init())
		{
        rt_kprintf("SD Card Error!\r\n");	
			  rt_thread_delay(5000);
		}
		
		switch(SDCardInfo.CardType)
		{
		    case SDIO_STD_CAPACITY_SD_CARD_V1_1: 
				    rt_kprintf("Card Type:SDSC V1.1\r\n");
					  break;
			  case SDIO_STD_CAPACITY_SD_CARD_V2_0:
						rt_kprintf("Card Type:SDSC V2.0\r\n");
						break;
			  case SDIO_HIGH_CAPACITY_SD_CARD:
						rt_kprintf("Card Type:SDHC V2.0\r\n");
						break;
			  case SDIO_MULTIMEDIA_CARD:rt_kprintf("Card Type:MMC Card\r\n");
						break;
		}	
		rt_kprintf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);	//制造商ID
		rt_kprintf("Card RCA:%d\r\n",SDCardInfo.RCA);								                //卡相对地址
		rt_kprintf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20)); 	//显示容量
		rt_kprintf("Card BlockSize:%d\r\n\r\n",SDCardInfo.CardBlockSize);			      //显示块大小
		
		//SD_test(1);
#ifdef RT_USING_DFS 
		sdcard_device.type  = RT_Device_Class_Block;
		sdcard_device.init 	= rt_sdcard_init;
		sdcard_device.open 	= rt_sdcard_open;
		sdcard_device.close = rt_sdcard_close;
		sdcard_device.read 	= rt_sdcard_read;
		sdcard_device.write = rt_sdcard_write;
		sdcard_device.control = rt_sdcard_control;
		
		sdcard_device.user_data = &SDCardInfo;

		rt_device_register(&sdcard_device, "sd0",RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_REMOVABLE |
                    	 RT_DEVICE_FLAG_STANDALONE);
#endif
		Bsp_FatService_CreateApp();
}
/*******************************************************************************
* Description    : 
*                   
*
* Input          : None
* Output         : None
* Return         : 错误代码;(0,无错误)
*******************************************************************************/
static SD_Error SD_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		SD_Error errorstatus=SD_OK;	 


		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_DMA2, ENABLE);//使能GPIOC,GPIOD DMA2时钟

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SDIO, ENABLE);//SDIO时钟使能

		RCC_APB2PeriphResetCmd(RCC_APB2Periph_SDIO, ENABLE);//SDIO复位


		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12; 	//PC8,9,10,11,12复用功能输出	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100M
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(GPIOC, &GPIO_InitStructure);// PC8,9,10,11,12复用功能输出


		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;
		GPIO_Init(GPIOD, &GPIO_InitStructure);//PD2复用功能输出

		 //引脚复用映射设置
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_SDIO); //PC8,AF12
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_SDIO);
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_SDIO);
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_SDIO);
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_SDIO);	
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_SDIO);	

		RCC_APB2PeriphResetCmd(RCC_APB2Periph_SDIO, DISABLE);//SDIO结束复位
			
		//SDIO外设寄存器设置为默认值 			   
		SDIO_Register_Deinit();

		NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

			errorstatus=SD_PowerON();			//SD卡上电
		if(errorstatus==SD_OK)errorstatus=SD_InitializeCards();			//初始化SD卡														  
			if(errorstatus==SD_OK)errorstatus=SD_GetCardInfo(&SDCardInfo);	//获取卡信息
		if(errorstatus==SD_OK)errorstatus=SD_SelectDeselect((u32)(SDCardInfo.RCA<<16));//选中SD卡   
			if(errorstatus==SD_OK)errorstatus=SD_EnableWideBusOperation(SDIO_BusWide_4b);	//4位宽度,如果是MMC卡,则不能用4位模式 
			if((errorstatus==SD_OK)||(SDIO_MULTIMEDIA_CARD==CardType))
		{  		    
			SDIO_Clock_Set(SDIO_TRANSFER_CLK_DIV);			//设置时钟频率,SDIO时钟计算公式:SDIO_CK时钟=SDIOCLK/[clkdiv+2];其中,SDIOCLK固定为48Mhz 
			//errorstatus=SD_SetDeviceMode(SD_DMA_MODE);	//设置为DMA模式
			errorstatus=SD_SetDeviceMode(SD_POLLING_MODE);//设置为查询模式
		}
		return errorstatus;		 
}
/*******************************************************************************
* Description    : 
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void SDIO_Register_Deinit(void)
{
	SDIO->POWER=0x00000000;
	SDIO->CLKCR=0x00000000;
	SDIO->ARG=0x00000000;
	SDIO->CMD=0x00000000;
	SDIO->DTIMER=0x00000000;
	SDIO->DLEN=0x00000000;
	SDIO->DCTRL=0x00000000;
	SDIO->ICR=0x00C007FF;
	SDIO->MASK=0x00000000;	 
}
/*******************************************************************************
* Description    : 
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static SD_Error SD_PowerON(void)
{
 	u8 i=0;
	SD_Error errorstatus=SD_OK;
	u32 response=0,count=0,validvoltage=0;
	u32 SDType=SD_STD_CAPACITY;
	
	
	 /*初始化时的时钟不能大于400KHz*/ 
  SDIO_InitStructure.SDIO_ClockDiv = SDIO_INIT_CLK_DIV;	/* HCLK = 72MHz, SDIOCLK = 72MHz, SDIO_CK = HCLK/(178 + 2) = 400 KHz */
  SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
  SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;  //不使用bypass模式，直接用HCLK进行分频得到SDIO_CK
  SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;	// 空闲时不关闭时钟电源
  SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;	 				//1位数据线
  SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;//硬件流
  SDIO_Init(&SDIO_InitStructure);

	SDIO_SetPowerState(SDIO_PowerState_ON);	//上电状态,开启卡时钟   
  SDIO->CLKCR|=1<<8;			//SDIOCK使能  
 
 	for(i=0;i<74;i++)
	{
 
		SDIO_CmdInitStructure.SDIO_Argument = 0x0;//发送CMD0进入IDLE STAGE模式命令.
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_GO_IDLE_STATE; //cmd0
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_No;  //无响应
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;  //则CPSM在开始发送命令之前等待数据传输结束。 
    SDIO_SendCommand(&SDIO_CmdInitStructure);	  		//写命令进命令寄存器
		
		errorstatus=CmdError();
		
		if(errorstatus==SD_OK)break;
 	}
 	if(errorstatus)return errorstatus;//返回错误状态
	
  SDIO_CmdInitStructure.SDIO_Argument = SD_CHECK_PATTERN;	//发送CMD8,短响应,检查SD卡接口特性
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SEND_IF_COND;	//cmd8
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;	 //r7
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;			 //关闭等待中断
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
	
  errorstatus=CmdResp7Error();						//等待R7响应
	
 	if(errorstatus==SD_OK) 								//R7响应正常
	{
		CardType=SDIO_STD_CAPACITY_SD_CARD_V2_0;		//SD 2.0卡
		SDType=SD_HIGH_CAPACITY;			   			//高容量卡
	}
	  
	  SDIO_CmdInitStructure.SDIO_Argument = 0x00;//发送CMD55,短响应	
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);		//发送CMD55,短响应	 
	
	 errorstatus=CmdResp1Error(SD_CMD_APP_CMD); 		 	//等待R1响应   
	
	if(errorstatus==SD_OK)//SD2.0/SD 1.1,否则为MMC卡
	{																  
		//SD卡,发送ACMD41 SD_APP_OP_COND,参数为:0x80100000 
		while((!validvoltage)&&(count<SD_MAX_VOLT_TRIAL))
		{	   										   
		  SDIO_CmdInitStructure.SDIO_Argument = 0x00;//发送CMD55,短响应
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;	  //CMD55
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);			//发送CMD55,短响应	 
			
			errorstatus=CmdResp1Error(SD_CMD_APP_CMD); 	 	//等待R1响应  
			
 			if(errorstatus!=SD_OK)return errorstatus;   	//响应错误

      //acmd41，命令参数由支持的电压范围及HCS位组成，HCS位置一来区分卡是SDSc还是sdhc
      SDIO_CmdInitStructure.SDIO_Argument = SD_VOLTAGE_WINDOW_SD | SDType;	//发送ACMD41,短响应	
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SD_APP_OP_COND;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;  //r3
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);
			
			errorstatus=CmdResp3Error(); 					//等待R3响应   
			
 			if(errorstatus!=SD_OK)return errorstatus;   	//响应错误 
			response=SDIO->RESP1;;			   				//得到响应
			validvoltage=(((response>>31)==1)?1:0);			//判断SD卡上电是否完成
			count++;
		}
		if(count>=SD_MAX_VOLT_TRIAL)
		{
			errorstatus=SD_INVALID_VOLTRANGE;
			return errorstatus;
		}	 
		if(response&=SD_HIGH_CAPACITY)
		{
			CardType=SDIO_HIGH_CAPACITY_SD_CARD;
		}
 	}else//MMC卡
	{
		//MMC卡,发送CMD1 SDIO_SEND_OP_COND,参数为:0x80FF8000 
		while((!validvoltage)&&(count<SD_MAX_VOLT_TRIAL))
		{	   										   				   
			SDIO_CmdInitStructure.SDIO_Argument = SD_VOLTAGE_WINDOW_MMC;//发送CMD1,短响应	   
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEND_OP_COND;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;  //r3
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);
			
			errorstatus=CmdResp3Error(); 					//等待R3响应   
			
 			if(errorstatus!=SD_OK)return errorstatus;   	//响应错误  
			response=SDIO->RESP1;;			   				//得到响应
			validvoltage=(((response>>31)==1)?1:0);
			count++;
		}
		if(count>=SD_MAX_VOLT_TRIAL)
		{
			errorstatus=SD_INVALID_VOLTRANGE;
			return errorstatus;
		}	 			    
		CardType=SDIO_MULTIMEDIA_CARD;	  
  	}  
  	return(errorstatus);		
}

/*******************************************************************************
* Description    : 检查CMD0的执行状态
*                   
*
* Input          : None
* Output         : None
* Return         : sd卡错误码
*******************************************************************************/
static SD_Error CmdError(void)
{
	SD_Error errorstatus = SD_OK;
	u32 timeout=SDIO_CMD0TIMEOUT;	   
	while(timeout--)
	{
		if(SDIO_GetFlagStatus(SDIO_FLAG_CMDSENT) != RESET)break;	//命令已发送(无需响应)	 
	}	    
	if(timeout==0)return SD_CMD_RSP_TIMEOUT;  
	SDIO_ClearFlag(SDIO_STATIC_FLAGS);//清除所有标记
	return errorstatus;
}	 
/*******************************************************************************
* Description    : 检查R7响应的错误状态
*                   
*
* Input          : None
* Output         : None
* Return         : sd卡错误码
*******************************************************************************/
static SD_Error CmdResp7Error(void)
{
	SD_Error errorstatus=SD_OK;
	u32 status;
	u32 timeout=SDIO_CMD0TIMEOUT;
 	while(timeout--)
	{
		status=SDIO->STA;
		if(status&((1<<0)|(1<<2)|(1<<6)))break;//CRC错误/命令响应超时/已经收到响应(CRC校验成功)	
	}
 	if((timeout==0)||(status&(1<<2)))	//响应超时
	{																				    
		errorstatus=SD_CMD_RSP_TIMEOUT;	//当前卡不是2.0兼容卡,或者不支持设定的电压范围
		SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT); 			//清除命令响应超时标志
		return errorstatus;
	}	 
	if(status&1<<6)						//成功接收到响应
	{								   
		errorstatus=SD_OK;
		SDIO_ClearFlag(SDIO_FLAG_CMDREND); 				//清除响应标志
 	}
	return errorstatus;
}	  
/*******************************************************************************
* Description    : 检查R1响应的错误状态
*                   
*
* Input          : 当前命令
* Output         : None
* Return         : sd卡错误码
*******************************************************************************/
static SD_Error CmdResp1Error(u8 cmd)
{	  
   	u32 status; 
	while(1)
	{
		status=SDIO->STA;
		if(status&((1<<0)|(1<<2)|(1<<6)))break;//CRC错误/命令响应超时/已经收到响应(CRC校验成功)
	} 
	if(SDIO_GetFlagStatus(SDIO_FLAG_CTIMEOUT) != RESET)					//响应超时
	{																				    
 		SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT); 				//清除命令响应超时标志
		return SD_CMD_RSP_TIMEOUT;
	}	
 	if(SDIO_GetFlagStatus(SDIO_FLAG_CCRCFAIL) != RESET)					//CRC错误
	{																				    
 		SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL); 				//清除标志
		return SD_CMD_CRC_FAIL;
	}		
	if(SDIO->RESPCMD!=cmd)return SD_ILLEGAL_CMD;//命令不匹配 
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);//清除所有标记
	return (SD_Error)(SDIO->RESP1&SD_OCR_ERRORBITS);//返回卡响应
}
/*******************************************************************************
* Description    : 检查R3响应的错误状态
*                   
*
* Input          : None
* Output         : None
* Return         : sd卡错误码
*******************************************************************************/
static SD_Error CmdResp3Error(void)
{
	u32 status;						 
 	while(1)
	{
		status=SDIO->STA;
		if(status&((1<<0)|(1<<2)|(1<<6)))break;//CRC错误/命令响应超时/已经收到响应(CRC校验成功)	
	}
 	if(SDIO_GetFlagStatus(SDIO_FLAG_CTIMEOUT) != RESET)					//响应超时
	{											 
		SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);			//清除命令响应超时标志
		return SD_CMD_RSP_TIMEOUT;
	}	 
   SDIO_ClearFlag(SDIO_STATIC_FLAGS);//清除所有标记
 	return SD_OK;								  
}
/*******************************************************************************
* Description    : 初始化所有的卡,并让卡进入就绪状态
*                   
*
* Input          : None
* Output         : None
* Return         : sd卡错误码
*******************************************************************************/
SD_Error SD_InitializeCards(void)
{
 	SD_Error errorstatus=SD_OK;
	u16 rca = 0x01;
	
  if (SDIO_GetPowerState() == SDIO_PowerState_OFF)	//检查电源状态,确保为上电状态
  {
    errorstatus = SD_REQUEST_NOT_APPLICABLE;
    return(errorstatus);
  }

 	if(SDIO_SECURE_DIGITAL_IO_CARD!=CardType)			//非SECURE_DIGITAL_IO_CARD
	{
		SDIO_CmdInitStructure.SDIO_Argument = 0x0;//发送CMD2,取得CID,长响应
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_ALL_SEND_CID;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Long;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);//发送CMD2,取得CID,长响应	
		
		errorstatus=CmdResp2Error(); 					//等待R2响应 
		
		if(errorstatus!=SD_OK)return errorstatus;   	//响应错误		 
		
 		CID_Tab[0]=SDIO->RESP1;
		CID_Tab[1]=SDIO->RESP2;
		CID_Tab[2]=SDIO->RESP3;
		CID_Tab[3]=SDIO->RESP4;
	}
	if((SDIO_STD_CAPACITY_SD_CARD_V1_1==CardType)||(SDIO_STD_CAPACITY_SD_CARD_V2_0==CardType)||(SDIO_SECURE_DIGITAL_IO_COMBO_CARD==CardType)||(SDIO_HIGH_CAPACITY_SD_CARD==CardType))//判断卡类型
	{
		SDIO_CmdInitStructure.SDIO_Argument = 0x00;//发送CMD3,短响应 
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_REL_ADDR;	//cmd3
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //r6
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);	//发送CMD3,短响应 
		
		errorstatus=CmdResp6Error(SD_CMD_SET_REL_ADDR,&rca);//等待R6响应 
		
		if(errorstatus!=SD_OK)return errorstatus;   	//响应错误		    
	}   
    if (SDIO_MULTIMEDIA_CARD==CardType)
    {

		  SDIO_CmdInitStructure.SDIO_Argument = (u32)(rca<<16);//发送CMD3,短响应 
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_REL_ADDR;	//cmd3
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //r6
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);	//发送CMD3,短响应 	
			
      errorstatus=CmdResp2Error(); 					//等待R2响应   
			
		  if(errorstatus!=SD_OK)return errorstatus;   	//响应错误	 
    }
	if (SDIO_SECURE_DIGITAL_IO_CARD!=CardType)			//非SECURE_DIGITAL_IO_CARD
	{
		RCA = rca;
		
    SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)(rca << 16);//发送CMD9+卡RCA,取得CSD,长响应 
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEND_CSD;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Long;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);
		
		errorstatus=CmdResp2Error(); 					//等待R2响应   
		if(errorstatus!=SD_OK)return errorstatus;   	//响应错误		    
  		
		CSD_Tab[0]=SDIO->RESP1;
	  CSD_Tab[1]=SDIO->RESP2;
		CSD_Tab[2]=SDIO->RESP3;						
		CSD_Tab[3]=SDIO->RESP4;					    
	}
	return SD_OK;//卡初始化成功
} 
/*******************************************************************************
* Description    : 检查R2响应的错误状态
*                   
*
* Input          : None
* Output         : None
* Return         : sd卡错误码
*******************************************************************************/
static SD_Error CmdResp2Error(void)
{
	SD_Error errorstatus=SD_OK;
	u32 status;
	u32 timeout=SDIO_CMD0TIMEOUT;
 	while(timeout--)
	{
		status=SDIO->STA;
		if(status&((1<<0)|(1<<2)|(1<<6)))break;//CRC错误/命令响应超时/已经收到响应(CRC校验成功)	
	}
  	if((timeout==0)||(status&(1<<2)))	//响应超时
	{																				    
		errorstatus=SD_CMD_RSP_TIMEOUT; 
		SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT); 		//清除命令响应超时标志
		return errorstatus;
	}	 
	if(SDIO_GetFlagStatus(SDIO_FLAG_CCRCFAIL) != RESET)						//CRC错误
	{								   
		errorstatus=SD_CMD_CRC_FAIL;
		SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);		//清除响应标志
 	}
	SDIO_ClearFlag(SDIO_STATIC_FLAGS);//清除所有标记
 	return errorstatus;								    		 
} 
//检查R6响应的错误状态
//cmd:之前发送的命令
//prca:卡返回的RCA地址
//返回值:错误状态
/*******************************************************************************
* Description    : 检查R6响应的错误状态
*                   
*
* Input          : cmd:之前发送的命令
*                  prca:卡返回的RCA地址
* Output         : None
* Return         : sd卡错误码
*******************************************************************************/
static SD_Error CmdResp6Error(u8 cmd,u16*prca)
{
	SD_Error errorstatus=SD_OK;
	u32 status;					    
	u32 rspr1;
 	while(1)
	{
		status=SDIO->STA;
		if(status&((1<<0)|(1<<2)|(1<<6)))break;//CRC错误/命令响应超时/已经收到响应(CRC校验成功)	
	}
	if(SDIO_GetFlagStatus(SDIO_FLAG_CTIMEOUT) != RESET)					//响应超时
	{																				    
 		SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);			//清除命令响应超时标志
		return SD_CMD_RSP_TIMEOUT;
	}	 	 
	if(SDIO_GetFlagStatus(SDIO_FLAG_CCRCFAIL) != RESET)						//CRC错误
	{								   
		SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);					//清除响应标志
 		return SD_CMD_CRC_FAIL;
	}
	if(SDIO->RESPCMD!=cmd)				//判断是否响应cmd命令
	{
 		return SD_ILLEGAL_CMD; 		
	}	    
	SDIO_ClearFlag(SDIO_STATIC_FLAGS);//清除所有标记
	rspr1=SDIO->RESP1;					//得到响应 	 
	if(SD_ALLZERO==(rspr1&(SD_R6_GENERAL_UNKNOWN_ERROR|SD_R6_ILLEGAL_CMD|SD_R6_COM_CRC_FAILED)))
	{
		*prca=(u16)(rspr1>>16);			//右移16位得到,rca
		return errorstatus;
	}
   	if(rspr1&SD_R6_GENERAL_UNKNOWN_ERROR)return SD_GENERAL_UNKNOWN_ERROR;
   	if(rspr1&SD_R6_ILLEGAL_CMD)return SD_ILLEGAL_CMD;
   	if(rspr1&SD_R6_COM_CRC_FAILED)return SD_COM_CRC_FAILED;
	return errorstatus;
}
/*******************************************************************************
* Description    : 得到卡信息
*                   
*
* Input          : 卡信息存储区
* Output         : None
* Return         : sd卡错误码
*******************************************************************************/
static SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo)
{
 	SD_Error errorstatus=SD_OK;
	u8 tmp=0;	   
	cardinfo->CardType=(u8)CardType; 				//卡类型
	cardinfo->RCA=(u16)RCA;							//卡RCA值
	tmp=(u8)((CSD_Tab[0]&0xFF000000)>>24);
	cardinfo->SD_csd.CSDStruct=(tmp&0xC0)>>6;		//CSD结构
	cardinfo->SD_csd.SysSpecVersion=(tmp&0x3C)>>2;	//2.0协议还没定义这部分(为保留),应该是后续协议定义的
	cardinfo->SD_csd.Reserved1=tmp&0x03;			//2个保留位  
	tmp=(u8)((CSD_Tab[0]&0x00FF0000)>>16);			//第1个字节
	cardinfo->SD_csd.TAAC=tmp;				   		//数据读时间1
	tmp=(u8)((CSD_Tab[0]&0x0000FF00)>>8);	  		//第2个字节
	cardinfo->SD_csd.NSAC=tmp;		  				//数据读时间2
	tmp=(u8)(CSD_Tab[0]&0x000000FF);				//第3个字节
	cardinfo->SD_csd.MaxBusClkFrec=tmp;		  		//传输速度	   
	tmp=(u8)((CSD_Tab[1]&0xFF000000)>>24);			//第4个字节
	cardinfo->SD_csd.CardComdClasses=tmp<<4;    	//卡指令类高四位
	tmp=(u8)((CSD_Tab[1]&0x00FF0000)>>16);	 		//第5个字节
	cardinfo->SD_csd.CardComdClasses|=(tmp&0xF0)>>4;//卡指令类低四位
	cardinfo->SD_csd.RdBlockLen=tmp&0x0F;	    	//最大读取数据长度
	tmp=(u8)((CSD_Tab[1]&0x0000FF00)>>8);			//第6个字节
	cardinfo->SD_csd.PartBlockRead=(tmp&0x80)>>7;	//允许分块读
	cardinfo->SD_csd.WrBlockMisalign=(tmp&0x40)>>6;	//写块错位
	cardinfo->SD_csd.RdBlockMisalign=(tmp&0x20)>>5;	//读块错位
	cardinfo->SD_csd.DSRImpl=(tmp&0x10)>>4;
	cardinfo->SD_csd.Reserved2=0; 					//保留
 	if((CardType==SDIO_STD_CAPACITY_SD_CARD_V1_1)||(CardType==SDIO_STD_CAPACITY_SD_CARD_V2_0)||(SDIO_MULTIMEDIA_CARD==CardType))//标准1.1/2.0卡/MMC卡
	{
		cardinfo->SD_csd.DeviceSize=(tmp&0x03)<<10;	//C_SIZE(12位)
	 	tmp=(u8)(CSD_Tab[1]&0x000000FF); 			//第7个字节	
		cardinfo->SD_csd.DeviceSize|=(tmp)<<2;
 		tmp=(u8)((CSD_Tab[2]&0xFF000000)>>24);		//第8个字节	
		cardinfo->SD_csd.DeviceSize|=(tmp&0xC0)>>6;
 		cardinfo->SD_csd.MaxRdCurrentVDDMin=(tmp&0x38)>>3;
		cardinfo->SD_csd.MaxRdCurrentVDDMax=(tmp&0x07);
 		tmp=(u8)((CSD_Tab[2]&0x00FF0000)>>16);		//第9个字节	
		cardinfo->SD_csd.MaxWrCurrentVDDMin=(tmp&0xE0)>>5;
		cardinfo->SD_csd.MaxWrCurrentVDDMax=(tmp&0x1C)>>2;
		cardinfo->SD_csd.DeviceSizeMul=(tmp&0x03)<<1;//C_SIZE_MULT
 		tmp=(u8)((CSD_Tab[2]&0x0000FF00)>>8);	  	//第10个字节	
		cardinfo->SD_csd.DeviceSizeMul|=(tmp&0x80)>>7;
 		cardinfo->CardCapacity=(cardinfo->SD_csd.DeviceSize+1);//计算卡容量
		cardinfo->CardCapacity*=(1<<(cardinfo->SD_csd.DeviceSizeMul+2));
		cardinfo->CardBlockSize=1<<(cardinfo->SD_csd.RdBlockLen);//块大小
		cardinfo->CardCapacity*=cardinfo->CardBlockSize;
	}else if(CardType==SDIO_HIGH_CAPACITY_SD_CARD)	//高容量卡
	{
 		tmp=(u8)(CSD_Tab[1]&0x000000FF); 		//第7个字节	
		cardinfo->SD_csd.DeviceSize=(tmp&0x3F)<<16;//C_SIZE
 		tmp=(u8)((CSD_Tab[2]&0xFF000000)>>24); 	//第8个字节	
 		cardinfo->SD_csd.DeviceSize|=(tmp<<8);
 		tmp=(u8)((CSD_Tab[2]&0x00FF0000)>>16);	//第9个字节	
 		cardinfo->SD_csd.DeviceSize|=(tmp);
 		tmp=(u8)((CSD_Tab[2]&0x0000FF00)>>8); 	//第10个字节	
 		cardinfo->CardCapacity=(long long)(cardinfo->SD_csd.DeviceSize+1)*512*1024;//计算卡容量
		cardinfo->CardBlockSize=512; 			//块大小固定为512字节
	}	  
	cardinfo->SD_csd.EraseGrSize=(tmp&0x40)>>6;
	cardinfo->SD_csd.EraseGrMul=(tmp&0x3F)<<1;	   
	tmp=(u8)(CSD_Tab[2]&0x000000FF);			//第11个字节	
	cardinfo->SD_csd.EraseGrMul|=(tmp&0x80)>>7;
	cardinfo->SD_csd.WrProtectGrSize=(tmp&0x7F);
 	tmp=(u8)((CSD_Tab[3]&0xFF000000)>>24);		//第12个字节	
	cardinfo->SD_csd.WrProtectGrEnable=(tmp&0x80)>>7;
	cardinfo->SD_csd.ManDeflECC=(tmp&0x60)>>5;
	cardinfo->SD_csd.WrSpeedFact=(tmp&0x1C)>>2;
	cardinfo->SD_csd.MaxWrBlockLen=(tmp&0x03)<<2;	 
	tmp=(u8)((CSD_Tab[3]&0x00FF0000)>>16);		//第13个字节
	cardinfo->SD_csd.MaxWrBlockLen|=(tmp&0xC0)>>6;
	cardinfo->SD_csd.WriteBlockPaPartial=(tmp&0x20)>>5;
	cardinfo->SD_csd.Reserved3=0;
	cardinfo->SD_csd.ContentProtectAppli=(tmp&0x01);  
	tmp=(u8)((CSD_Tab[3]&0x0000FF00)>>8);		//第14个字节
	cardinfo->SD_csd.FileFormatGrouop=(tmp&0x80)>>7;
	cardinfo->SD_csd.CopyFlag=(tmp&0x40)>>6;
	cardinfo->SD_csd.PermWrProtect=(tmp&0x20)>>5;
	cardinfo->SD_csd.TempWrProtect=(tmp&0x10)>>4;
	cardinfo->SD_csd.FileFormat=(tmp&0x0C)>>2;
	cardinfo->SD_csd.ECC=(tmp&0x03);  
	tmp=(u8)(CSD_Tab[3]&0x000000FF);			//第15个字节
	cardinfo->SD_csd.CSD_CRC=(tmp&0xFE)>>1;
	cardinfo->SD_csd.Reserved4=1;		 
	tmp=(u8)((CID_Tab[0]&0xFF000000)>>24);		//第0个字节
	cardinfo->SD_cid.ManufacturerID=tmp;		    
	tmp=(u8)((CID_Tab[0]&0x00FF0000)>>16);		//第1个字节
	cardinfo->SD_cid.OEM_AppliID=tmp<<8;	  
	tmp=(u8)((CID_Tab[0]&0x000000FF00)>>8);		//第2个字节
	cardinfo->SD_cid.OEM_AppliID|=tmp;	    
	tmp=(u8)(CID_Tab[0]&0x000000FF);			//第3个字节	
	cardinfo->SD_cid.ProdName1=tmp<<24;				  
	tmp=(u8)((CID_Tab[1]&0xFF000000)>>24); 		//第4个字节
	cardinfo->SD_cid.ProdName1|=tmp<<16;	  
	tmp=(u8)((CID_Tab[1]&0x00FF0000)>>16);	   	//第5个字节
	cardinfo->SD_cid.ProdName1|=tmp<<8;		 
	tmp=(u8)((CID_Tab[1]&0x0000FF00)>>8);		//第6个字节
	cardinfo->SD_cid.ProdName1|=tmp;		   
	tmp=(u8)(CID_Tab[1]&0x000000FF);	  		//第7个字节
	cardinfo->SD_cid.ProdName2=tmp;			  
	tmp=(u8)((CID_Tab[2]&0xFF000000)>>24); 		//第8个字节
	cardinfo->SD_cid.ProdRev=tmp;		 
	tmp=(u8)((CID_Tab[2]&0x00FF0000)>>16);		//第9个字节
	cardinfo->SD_cid.ProdSN=tmp<<24;	   
	tmp=(u8)((CID_Tab[2]&0x0000FF00)>>8); 		//第10个字节
	cardinfo->SD_cid.ProdSN|=tmp<<16;	   
	tmp=(u8)(CID_Tab[2]&0x000000FF);   			//第11个字节
	cardinfo->SD_cid.ProdSN|=tmp<<8;		   
	tmp=(u8)((CID_Tab[3]&0xFF000000)>>24); 		//第12个字节
	cardinfo->SD_cid.ProdSN|=tmp;			     
	tmp=(u8)((CID_Tab[3]&0x00FF0000)>>16);	 	//第13个字节
	cardinfo->SD_cid.Reserved1|=(tmp&0xF0)>>4;
	cardinfo->SD_cid.ManufactDate=(tmp&0x0F)<<8;    
	tmp=(u8)((CID_Tab[3]&0x0000FF00)>>8);		//第14个字节
	cardinfo->SD_cid.ManufactDate|=tmp;		 	  
	tmp=(u8)(CID_Tab[3]&0x000000FF);			//第15个字节
	cardinfo->SD_cid.CID_CRC=(tmp&0xFE)>>1;
	cardinfo->SD_cid.Reserved2=1;	 
	return errorstatus;
}
/*******************************************************************************
* Description    : 发送CMD7,选择相对地址(rca)为addr的卡,取消其他卡.如果为0,则都不选择.
*                   
*
* Input          : 卡的RCA地址
* Output         : None
* Return         : sd卡错误码
*******************************************************************************/
static SD_Error SD_SelectDeselect(u32 addr)
{

  SDIO_CmdInitStructure.SDIO_Argument =  addr;//发送CMD7,选择卡,短响应	
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEL_DESEL_CARD;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);//发送CMD7,选择卡,短响应
	
 	return CmdResp1Error(SD_CMD_SEL_DESEL_CARD);	  
}
//设置SDIO总线宽度(MMC卡不支持4bit模式)

//设置SDIO总线宽度(MMC卡不支持4bit模式)
//   @arg SDIO_BusWide_8b: 8-bit data transfer (Only for MMC)
//   @arg SDIO_BusWide_4b: 4-bit data transfer
//   @arg SDIO_BusWide_1b: 1-bit data transfer (默认)

/*******************************************************************************
* Description    : 设置SDIO总线宽度(MMC卡不支持4bit模式)
* @arg SDIO_BusWide_8b: 8-bit data transfer (Only for MMC)
* @arg SDIO_BusWide_4b: 4-bit data transfer
* @arg SDIO_BusWide_1b: 1-bit data transfer (默认)                 
*
* Input          : 位宽模式.0,1位数据宽度;1,4位数据宽度;2,8位数据宽度
* Output         : None
* Return         : sd卡错误码
*******************************************************************************/
static SD_Error SD_EnableWideBusOperation(u32 WideMode)
{
  	SD_Error errorstatus=SD_OK;
  if (SDIO_MULTIMEDIA_CARD == CardType)
  {
    errorstatus = SD_UNSUPPORTED_FEATURE;
    return(errorstatus);
  }
	
 	else if((SDIO_STD_CAPACITY_SD_CARD_V1_1==CardType)||(SDIO_STD_CAPACITY_SD_CARD_V2_0==CardType)||(SDIO_HIGH_CAPACITY_SD_CARD==CardType))
	{
		 if (SDIO_BusWide_8b == WideMode)   //2.0 sd不支持8bits
    {
      errorstatus = SD_UNSUPPORTED_FEATURE;
      return(errorstatus);
    }
 		else   
		{
			errorstatus=SDEnWideBus(WideMode);
 			if(SD_OK==errorstatus)
			{
				SDIO->CLKCR&=~(3<<11);		//清除之前的位宽设置    
				SDIO->CLKCR|=WideMode;//1位/4位总线宽度 
				SDIO->CLKCR|=0<<14;			//不开启硬件流控制
			}
		}  
	}
	return errorstatus; 
}

/*******************************************************************************
* Description    : SDIO使能宽总线模式
*               
*
* Input          : 0,不使能;1,使能;
* Output         : None
* Return         : sd卡错误码
*******************************************************************************/
static SD_Error SDEnWideBus(u8 enx)
{
	SD_Error errorstatus = SD_OK;
 	u32 scr[2]={0,0};
	u8 arg=0X00;
	if(enx)arg=0X02;
	else arg=0X00;
 	if(SDIO->RESP1&SD_CARD_LOCKED)return SD_LOCK_UNLOCK_FAILED;//SD卡处于LOCKED状态		    
 	errorstatus=FindSCR(RCA,scr);						//得到SCR寄存器数据
 	if(errorstatus!=SD_OK)return errorstatus;
	if((scr[1]&SD_WIDE_BUS_SUPPORT)!=SD_ALLZERO)		//支持宽总线
	{
		  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t) RCA << 16;//发送CMD55+RCA,短响应	
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);
		
	 	errorstatus=CmdResp1Error(SD_CMD_APP_CMD);
		
	 	if(errorstatus!=SD_OK)return errorstatus; 
		
		  SDIO_CmdInitStructure.SDIO_Argument = arg;//发送ACMD6,短响应,参数:10,4位;00,1位.	
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_SD_SET_BUSWIDTH;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);
			
     errorstatus=CmdResp1Error(SD_CMD_APP_SD_SET_BUSWIDTH);
		
		return errorstatus;
	}else return SD_REQUEST_NOT_APPLICABLE;				//不支持宽总线设置 	 
}		 
/*******************************************************************************
* Description    : 查找SD卡的SCR寄存器值
*               
*
* Input          : rca:卡相对地址
									 pscr:数据缓存区(存储SCR内容)
* Output         : None
* Return         : sd卡错误码
*******************************************************************************/
static SD_Error FindSCR(u16 rca,u32 *pscr)
{ 
	u32 index = 0; 
	SD_Error errorstatus = SD_OK;
	u32 tempscr[2]={0,0};  
	
	SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)8;	 //发送CMD16,短响应,设置Block Size为8字节	
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN; //	 cmd16
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;  //r1
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
	
 	errorstatus=CmdResp1Error(SD_CMD_SET_BLOCKLEN);
	
 	if(errorstatus!=SD_OK)return errorstatus;	 
	
  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t) RCA << 16; 
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;//发送CMD55,短响应 	
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
	
 	errorstatus=CmdResp1Error(SD_CMD_APP_CMD);
 	if(errorstatus!=SD_OK)return errorstatus;
	
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = 8;  //8个字节长度,block为8字节,SD卡到SDIO.
  SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_8b  ;  //块大小8byte 
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStructure);		

  SDIO_CmdInitStructure.SDIO_Argument = 0x0;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SD_APP_SEND_SCR;	//发送ACMD51,短响应,参数为0	
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;  //r1
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
	
 	errorstatus=CmdResp1Error(SD_CMD_SD_APP_SEND_SCR);
 	if(errorstatus!=SD_OK)return errorstatus;							   
 	while(!(SDIO->STA&(SDIO_FLAG_RXOVERR|SDIO_FLAG_DCRCFAIL|SDIO_FLAG_DTIMEOUT|SDIO_FLAG_DBCKEND|SDIO_FLAG_STBITERR)))
	{ 
		if(SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET)//接收FIFO数据可用
		{
			*(tempscr+index)=SDIO->FIFO;	//读取FIFO内容
			index++;
			if(index>=2)break;
		}
	}
		if(SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)		//数据超时错误
		{										   
	 		SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT); 	//清错误标志
			return SD_DATA_TIMEOUT;
	 	}else if(SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)	//数据块CRC错误
		{
	 		SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);  		//清错误标志
			return SD_DATA_CRC_FAIL;		   
		}else if(SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET) 	//接收fifo上溢错误
		{
	 		SDIO_ClearFlag(SDIO_FLAG_RXOVERR);		//清错误标志
			return SD_RX_OVERRUN;		 
		}else if(SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET) 	//接收起始位错误
		{
	 		SDIO_ClearFlag(SDIO_FLAG_STBITERR);//清错误标志
			return SD_START_BIT_ERR;		 
		}  
   SDIO_ClearFlag(SDIO_STATIC_FLAGS);//清除所有标记
	//把数据顺序按8位为单位倒过来.   	
	*(pscr+1)=((tempscr[0]&SD_0TO7BITS)<<24)|((tempscr[0]&SD_8TO15BITS)<<8)|((tempscr[0]&SD_16TO23BITS)>>8)|((tempscr[0]&SD_24TO31BITS)>>24);
	*(pscr)=((tempscr[1]&SD_0TO7BITS)<<24)|((tempscr[1]&SD_8TO15BITS)<<8)|((tempscr[1]&SD_16TO23BITS)>>8)|((tempscr[1]&SD_24TO31BITS)>>24);
 	return errorstatus;
}
/*******************************************************************************
* Description    : SDIO时钟初始化设置
*               
*
* Input          : 时钟分频系数
*                  CK时钟=SDIOCLK/[clkdiv+2];(SDIOCLK时钟固定为48Mhz)
* Output         : None
* Return         : sd卡错误码
*******************************************************************************/
static void SDIO_Clock_Set(u8 clkdiv)
{
	u32 tmpreg=SDIO->CLKCR; 
  	tmpreg&=0XFFFFFF00; 
 	tmpreg|=clkdiv;   
	SDIO->CLKCR=tmpreg;
}
/*******************************************************************************
* Description    : 设置SD卡工作模式
*               
*
* Input          : None
* Output         : None
* Return         : sd卡错误码
*******************************************************************************/
static SD_Error SD_SetDeviceMode(u32 Mode)
{
	SD_Error errorstatus = SD_OK;
 	if((Mode==SD_DMA_MODE)||(Mode==SD_POLLING_MODE))
	{
			DeviceMode=Mode;
	}
	else 
	{
			errorstatus = SD_INVALID_PARAMETER;
	}
	return errorstatus;	    
}
/*******************************************************************************
* Description    : 读SD卡
*               
*
* Input          : buf:读数据缓存区
                   sector:扇区地址
* Output         : None
* Return         : 错误状态;0,正常;其他,错误代码
*******************************************************************************/	  				 
static u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt)
{
	u8 sta=SD_OK;
	long long lsector=sector;
	u8 n;
	if(CardType!=SDIO_STD_CAPACITY_SD_CARD_V1_1)lsector<<=9;
	if((u32)buf%4!=0)
	{
	 	for(n=0;n<cnt;n++)
		{
		 	sta=SD_ReadBlock(SDIO_DATA_BUFFER,lsector+512*n,512);//单个sector的读操作
			rt_memcpy(buf,SDIO_DATA_BUFFER,512);
			buf+=512;
		} 
	}else
	{
		if(cnt==1)sta=SD_ReadBlock(buf,lsector,512);    	//单个sector的读操作
		else sta=SD_ReadMultiBlocks(buf,lsector,512,cnt);//多个sector  
	}
	return sta;
}
/*******************************************************************************
* Description    : SD卡读取一个块 
*               
*
* Input          : buf:读数据缓存区(必须4字节对齐!!)
								   addr:读取地址
									 blksize:块大小
* Output         : None
* Return         : sd卡错误码
*******************************************************************************/	
static SD_Error SD_ReadBlock(u8 *buf,long long addr,u16 blksize)
{	  
	SD_Error errorstatus=SD_OK;
	u8 power;
  u32 count=0,*tempbuff=(u32*)buf;//转换为u32指针 
	u32 timeout=SDIO_DATATIMEOUT;  
	register rt_base_t temp;	
  if(NULL==buf)
		return SD_INVALID_PARAMETER; 
  SDIO->DCTRL=0x0;	//数据控制寄存器清零(关DMA) 
  
	if(CardType==SDIO_HIGH_CAPACITY_SD_CARD)//大容量卡
	{
		blksize=512;
		addr>>=9;
	}   
  	SDIO_DataInitStructure.SDIO_DataBlockSize= SDIO_DataBlockSize_1b ;//清除DPSM状态机配置
	  SDIO_DataInitStructure.SDIO_DataLength= 0 ;
	  SDIO_DataInitStructure.SDIO_DataTimeOut=SD_DATATIMEOUT ;
	  SDIO_DataInitStructure.SDIO_DPSM=SDIO_DPSM_Enable;
	  SDIO_DataInitStructure.SDIO_TransferDir=SDIO_TransferDir_ToCard;
	  SDIO_DataInitStructure.SDIO_TransferMode=SDIO_TransferMode_Block;
    SDIO_DataConfig(&SDIO_DataInitStructure);
	
	
	if(SDIO->RESP1&SD_CARD_LOCKED)return SD_LOCK_UNLOCK_FAILED;//卡锁了
	if((blksize>0)&&(blksize<=2048)&&((blksize&(blksize-1))==0))
	{
		power=convert_from_bytes_to_power_of_two(blksize);	
		
   
		SDIO_CmdInitStructure.SDIO_Argument =  blksize;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);//发送CMD16+设置数据长度为blksize,短响应
		
		
		errorstatus=CmdResp1Error(SD_CMD_SET_BLOCKLEN);	//等待R1响应 
		
		if(errorstatus!=SD_OK)return errorstatus;   	//响应错误	
		
	}else return SD_INVALID_PARAMETER;	  	 
	
	  SDIO_DataInitStructure.SDIO_DataBlockSize= power<<4 ;//清除DPSM状态机配置
	  SDIO_DataInitStructure.SDIO_DataLength= blksize ;
	  SDIO_DataInitStructure.SDIO_DataTimeOut=SD_DATATIMEOUT ;
	  SDIO_DataInitStructure.SDIO_DPSM=SDIO_DPSM_Enable;
	  SDIO_DataInitStructure.SDIO_TransferDir=SDIO_TransferDir_ToSDIO;
	  SDIO_DataInitStructure.SDIO_TransferMode=SDIO_TransferMode_Block;
    SDIO_DataConfig(&SDIO_DataInitStructure);
	
	  SDIO_CmdInitStructure.SDIO_Argument =  addr;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_READ_SINGLE_BLOCK;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);//发送CMD17+从addr地址出读取数据,短响应 
	
	errorstatus=CmdResp1Error(SD_CMD_READ_SINGLE_BLOCK);//等待R1响应   
	if(errorstatus!=SD_OK)return errorstatus;   		//响应错误	 
 	if(DeviceMode==SD_POLLING_MODE)						//查询模式,轮询数据	 
	{

		temp = rt_hw_interrupt_disable();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
		while(!(SDIO->STA&((1<<5)|(1<<1)|(1<<3)|(1<<10)|(1<<9))))//无上溢/CRC/超时/完成(标志)/起始位错误
		{
			if(SDIO_GetFlagStatus(SDIO_FLAG_RXFIFOHF) != RESET)						//接收区半满,表示至少存了8个字
			{
				for(count=0;count<8;count++)			//循环读取数据
				{
					*(tempbuff+count)=SDIO->FIFO;
				}
				tempbuff+=8;	 
				timeout=0X7FFFFF; 	//读数据溢出时间
			}else 	//处理超时
			{
				if(timeout==0)return SD_DATA_TIMEOUT;
				timeout--;
			}
		} 
		if(SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)		//数据超时错误
		{										   
	 		SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT); 	//清错误标志
			return SD_DATA_TIMEOUT;
	 	}else if(SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)	//数据块CRC错误
		{
	 		SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);  		//清错误标志
			return SD_DATA_CRC_FAIL;		   
		}else if(SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET) 	//接收fifo上溢错误
		{
	 		SDIO_ClearFlag(SDIO_FLAG_RXOVERR);		//清错误标志
			return SD_RX_OVERRUN;		 
		}else if(SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET) 	//接收起始位错误
		{
	 		SDIO_ClearFlag(SDIO_FLAG_STBITERR);//清错误标志
			return SD_START_BIT_ERR;		 
		}   
		while(SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET)	//FIFO里面,还存在可用数据
		{
			*tempbuff=SDIO->FIFO;	//循环读取数据
			tempbuff++;
		}
		rt_hw_interrupt_enable(temp);//开启总中断
		SDIO_ClearFlag(SDIO_STATIC_FLAGS);//清除所有标记
	 
	}else if(DeviceMode==SD_DMA_MODE)
	{
 		TransferError=SD_OK;
		StopCondition=0;			//单块读,不需要发送停止传输指令
		TransferEnd=0;				//传输结束标置位，在中断服务置1
		SDIO->MASK|=(1<<1)|(1<<3)|(1<<8)|(1<<5)|(1<<9);	//配置需要的中断 
	 	SDIO->DCTRL|=1<<3;		 	//SDIO DMA使能 
 	    SD_DMA_Config((u32*)buf,blksize,DMA_DIR_PeripheralToMemory); 
 		while(((DMA2->LISR&(1<<27))==RESET)&&(TransferEnd==0)&&(TransferError==SD_OK)&&timeout)timeout--;//等待传输完成 
		if(timeout==0)return SD_DATA_TIMEOUT;//超时
		if(TransferError!=SD_OK)errorstatus=TransferError;  
    }   
 	return errorstatus; 
}
/*******************************************************************************
* Description    : 得到NumberOfBytes以2为底的指数.
*               
*
* Input          : 字节数	
* Output         : None
* Return         : 以2为底的指数值
*******************************************************************************/	
static u8 convert_from_bytes_to_power_of_two(u16 NumberOfBytes)
{
	u8 count=0;
	while(NumberOfBytes!=1)
	{
		NumberOfBytes>>=1;
		count++;
	}
	return count;
}
/*******************************************************************************
* Description    : 配置SDIO DMA 
*               
*
* Input          : mbuf:存储器地址
                   bufsize:传输数据量
                   dir:方向;DMA_DIR_MemoryToPeripheral  存储器-->SDIO(写数据);DMA_DIR_PeripheralToMemory SDIO-->存储器(读数据);
* Output         : None
* Return         : None
*******************************************************************************/	
static void SD_DMA_Config(u32*mbuf,u32 bufsize,u32 dir)
{		 

  DMA_InitTypeDef  DMA_InitStructure;
	
	while (DMA_GetCmdStatus(DMA2_Stream3) != DISABLE){}//等待DMA可配置 
		
  DMA_DeInit(DMA2_Stream3);//清空之前该stream3上的所有中断标志
	
 
  DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SDIO->FIFO;//DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)mbuf;//DMA 存储器0地址
  DMA_InitStructure.DMA_DIR = dir;//存储器到外设模式
  DMA_InitStructure.DMA_BufferSize = 0;//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//外设数据长度:32位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//存储器数据长度:32位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//最高优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;   //FIFO使能      
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//全FIFO
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_INC4;//外设突发4次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_INC4;//存储器突发4次传输
  DMA_Init(DMA2_Stream3, &DMA_InitStructure);//初始化DMA Stream

	DMA_FlowControllerConfig(DMA2_Stream3,DMA_FlowCtrl_Peripheral);//外设流控制 
	 
  DMA_Cmd(DMA2_Stream3 ,ENABLE);//开启DMA传输	 

} 
/*******************************************************************************
* Description    : SD卡读取多个块 
*               
*
* Input          : buf:读数据缓存区
							     addr:读取地址
                   blksize:块大小
                   nblks:要读取的块数
* Output         : None
* Return         : 错误状态;0,正常;其他,错误代码
*******************************************************************************/
__align(4) u32 *tempbuff;
static SD_Error SD_ReadMultiBlocks(u8 *buf,long long addr,u16 blksize,u32 nblks)
{
  SD_Error errorstatus=SD_OK;
	u8 power;
  u32 count=0;
	u32 timeout=SDIO_DATATIMEOUT;  
	register rt_base_t temp;	
	
	tempbuff=(u32*)buf;//转换为u32指针

	
  SDIO->DCTRL=0x0;		//数据控制寄存器清零(关DMA)   
	if(CardType==SDIO_HIGH_CAPACITY_SD_CARD)//大容量卡
	{
		blksize=512;
		addr>>=9;
	}  
	
	  SDIO_DataInitStructure.SDIO_DataBlockSize= 0; ;//清除DPSM状态机配置
	  SDIO_DataInitStructure.SDIO_DataLength= 0 ;
	  SDIO_DataInitStructure.SDIO_DataTimeOut=SD_DATATIMEOUT ;
	  SDIO_DataInitStructure.SDIO_DPSM=SDIO_DPSM_Enable;
	  SDIO_DataInitStructure.SDIO_TransferDir=SDIO_TransferDir_ToCard;
	  SDIO_DataInitStructure.SDIO_TransferMode=SDIO_TransferMode_Block;
    SDIO_DataConfig(&SDIO_DataInitStructure);
	
	if(SDIO->RESP1&SD_CARD_LOCKED)return SD_LOCK_UNLOCK_FAILED;//卡锁了
	if((blksize>0)&&(blksize<=2048)&&((blksize&(blksize-1))==0))
	{
		power=convert_from_bytes_to_power_of_two(blksize);	    
		
	  SDIO_CmdInitStructure.SDIO_Argument =  blksize;//发送CMD16+设置数据长度为blksize,短响应 
		SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
		SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
		SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
		SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
		SDIO_SendCommand(&SDIO_CmdInitStructure);
		
		errorstatus=CmdResp1Error(SD_CMD_SET_BLOCKLEN);	//等待R1响应  
		
		if(errorstatus!=SD_OK)return errorstatus;   	//响应错误	 
		
	}else return SD_INVALID_PARAMETER;	  
	
	if(nblks>1)											//多块读  
	{									    
 	  	if(nblks*blksize>SD_MAX_DATA_LENGTH)return SD_INVALID_PARAMETER;//判断是否超过最大接收长度 
		
		   SDIO_DataInitStructure.SDIO_DataBlockSize= power<<4; ;//nblks*blksize,512块大小,卡到控制器
			 SDIO_DataInitStructure.SDIO_DataLength= nblks*blksize ;
			 SDIO_DataInitStructure.SDIO_DataTimeOut=SD_DATATIMEOUT ;
			 SDIO_DataInitStructure.SDIO_DPSM=SDIO_DPSM_Enable;
			 SDIO_DataInitStructure.SDIO_TransferDir=SDIO_TransferDir_ToSDIO;
			 SDIO_DataInitStructure.SDIO_TransferMode=SDIO_TransferMode_Block;
			 SDIO_DataConfig(&SDIO_DataInitStructure);

       SDIO_CmdInitStructure.SDIO_Argument =  addr;//发送CMD18+从addr地址出读取数据,短响应 
	     SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_READ_MULT_BLOCK;
		   SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
		   SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
		   SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
		   SDIO_SendCommand(&SDIO_CmdInitStructure);	
		
		errorstatus=CmdResp1Error(SD_CMD_READ_MULT_BLOCK);//等待R1响应 
		
		if(errorstatus!=SD_OK)return errorstatus;   	//响应错误	 
		
 		if(DeviceMode==SD_POLLING_MODE)
		{
			temp = rt_hw_interrupt_disable();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
			while(!(SDIO->STA&((1<<5)|(1<<1)|(1<<3)|(1<<8)|(1<<9))))//无上溢/CRC/超时/完成(标志)/起始位错误
			{
				if(SDIO_GetFlagStatus(SDIO_FLAG_RXFIFOHF) != RESET)						//接收区半满,表示至少存了8个字
				{
					for(count=0;count<8;count++)			//循环读取数据
					{
						*(tempbuff+count)=SDIO->FIFO;
					}
					tempbuff+=8;	 
					timeout=0X7FFFFF; 	//读数据溢出时间
				}else 	//处理超时
				{
					if(timeout==0)return SD_DATA_TIMEOUT;
					timeout--;
				}
			}  
		if(SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)		//数据超时错误
		{										   
	 		SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT); 	//清错误标志
			return SD_DATA_TIMEOUT;
	 	}else if(SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)	//数据块CRC错误
		{
	 		SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);  		//清错误标志
			return SD_DATA_CRC_FAIL;		   
		}else if(SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET) 	//接收fifo上溢错误
		{
	 		SDIO_ClearFlag(SDIO_FLAG_RXOVERR);		//清错误标志
			return SD_RX_OVERRUN;		 
		}else if(SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET) 	//接收起始位错误
		{
	 		SDIO_ClearFlag(SDIO_FLAG_STBITERR);//清错误标志
			return SD_START_BIT_ERR;		 
		}   
	    
		while(SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET)	//FIFO里面,还存在可用数据
		{
			*tempbuff=SDIO->FIFO;	//循环读取数据
			tempbuff++;
		}
	 		if(SDIO_GetFlagStatus(SDIO_FLAG_DATAEND) != RESET)		//接收结束
			{
				if((SDIO_STD_CAPACITY_SD_CARD_V1_1==CardType)||(SDIO_STD_CAPACITY_SD_CARD_V2_0==CardType)||(SDIO_HIGH_CAPACITY_SD_CARD==CardType))
				{				
					SDIO_CmdInitStructure.SDIO_Argument =  0;//发送CMD12+结束传输
				  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_STOP_TRANSMISSION;
					SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
					SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
					SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
					SDIO_SendCommand(&SDIO_CmdInitStructure);	
					
					errorstatus=CmdResp1Error(SD_CMD_STOP_TRANSMISSION);//等待R1响应   
					
					if(errorstatus!=SD_OK)return errorstatus;	 
				}
 			}
		  rt_hw_interrupt_enable(temp);//开启总中断
	 		SDIO_ClearFlag(SDIO_STATIC_FLAGS);//清除所有标记
 		}else if(DeviceMode==SD_DMA_MODE)
		{
	   		TransferError=SD_OK;
			StopCondition=1;			//多块读,需要发送停止传输指令 
			TransferEnd=0;				//传输结束标置位，在中断服务置1
			SDIO->MASK|=(1<<1)|(1<<3)|(1<<8)|(1<<5)|(1<<9);	//配置需要的中断 
		 	SDIO->DCTRL|=1<<3;		 						//SDIO DMA使能 
	 	    SD_DMA_Config((u32*)buf,nblks*blksize,DMA_DIR_PeripheralToMemory); 
	 		while(((DMA2->LISR&(1<<27))==RESET)&&timeout)timeout--;//等待传输完成 
			if(timeout==0)return SD_DATA_TIMEOUT;//超时
			while((TransferEnd==0)&&(TransferError==SD_OK)); 
			if(TransferError!=SD_OK)errorstatus=TransferError;  	 
		}		 
  	}
	return errorstatus;
}			    							
/*******************************************************************************
* Description    : 写SD卡
*               
*
* Input          : buf:读数据缓存区
                   sector:扇区地址
                   cnt:扇区个数	
* Output         : None
* Return         : 错误状态;0,正常;其他,错误代码
*******************************************************************************/	
static u8 SD_WriteDisk(u8*buf,u32 sector,u8 cnt)
{
	u8 sta=SD_OK;
	u8 n;
	long long lsector=sector;
	if(CardType!=SDIO_STD_CAPACITY_SD_CARD_V1_1)lsector<<=9;
	if((u32)buf%4!=0)
	{
	 	for(n=0;n<cnt;n++)
		{
			rt_memcpy(SDIO_DATA_BUFFER,buf,512);
		 	sta=SD_WriteBlock(SDIO_DATA_BUFFER,lsector+512*n,512);//单个sector的写操作
			buf+=512;
		} 
	}else
	{
		if(cnt==1)sta=SD_WriteBlock(buf,lsector,512);    	//单个sector的写操作
		else sta=SD_WriteMultiBlocks(buf,lsector,512,cnt);	//多个sector  
	}
	return sta;
}
/*******************************************************************************
* Description    : SD卡写1个块 
*               
*
* Input          : buf:数据缓存区
                   addr:写地址
                   blksize:块大小
* Output         : None
* Return         : SD错误代码
*******************************************************************************/	
static SD_Error SD_WriteBlock(u8 *buf,long long addr,  u16 blksize)
{
	SD_Error errorstatus = SD_OK;
	
	u8  power=0,cardstate=0;
	
	u32 timeout=0,bytestransferred=0;
	
	u32 cardstatus=0,count=0,restwords=0;
	
	u32	tlen=blksize;						//总长度(字节)
	
	u32*tempbuff=(u32*)buf;
  
	register rt_base_t temp;	
	
 	if(buf==NULL)return SD_INVALID_PARAMETER;//参数错误  
	
  SDIO->DCTRL=0x0;							//数据控制寄存器清零(关DMA)
	
	SDIO_DataInitStructure.SDIO_DataBlockSize= 0; ;//清除DPSM状态机配置
	SDIO_DataInitStructure.SDIO_DataLength= 0 ;
	SDIO_DataInitStructure.SDIO_DataTimeOut=SD_DATATIMEOUT ;
	SDIO_DataInitStructure.SDIO_DPSM=SDIO_DPSM_Enable;
	SDIO_DataInitStructure.SDIO_TransferDir=SDIO_TransferDir_ToCard;
	SDIO_DataInitStructure.SDIO_TransferMode=SDIO_TransferMode_Block;
  SDIO_DataConfig(&SDIO_DataInitStructure);
	
	
	if(SDIO->RESP1&SD_CARD_LOCKED)return SD_LOCK_UNLOCK_FAILED;//卡锁了
 	if(CardType==SDIO_HIGH_CAPACITY_SD_CARD)	//大容量卡
	{
		blksize=512;
		addr>>=9;
	}    
	if((blksize>0)&&(blksize<=2048)&&((blksize&(blksize-1))==0))
	{
		power=convert_from_bytes_to_power_of_two(blksize);	
		
		SDIO_CmdInitStructure.SDIO_Argument = blksize;//发送CMD16+设置数据长度为blksize,短响应 	
		SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
		SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
		SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
		SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
		SDIO_SendCommand(&SDIO_CmdInitStructure);	
		
		errorstatus=CmdResp1Error(SD_CMD_SET_BLOCKLEN);	//等待R1响应  
		
		if(errorstatus!=SD_OK)return errorstatus;   	//响应错误	 
		
	}else return SD_INVALID_PARAMETER;	
	
			SDIO_CmdInitStructure.SDIO_Argument = (u32)RCA<<16;//发送CMD13,查询卡的状态,短响应 	
		  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEND_STATUS;
			SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
			SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
			SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
			SDIO_SendCommand(&SDIO_CmdInitStructure);	

	  errorstatus=CmdResp1Error(SD_CMD_SEND_STATUS);		//等待R1响应  
	
	if(errorstatus!=SD_OK)return errorstatus;
	cardstatus=SDIO->RESP1;													  
	timeout=SD_DATATIMEOUT;
   	while(((cardstatus&0x00000100)==0)&&(timeout>0)) 	//检查READY_FOR_DATA位是否置位
	{
		timeout--;  
		
		SDIO_CmdInitStructure.SDIO_Argument = (u32)RCA<<16;//发送CMD13,查询卡的状态,短响应
		SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEND_STATUS;
		SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
		SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
		SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
		SDIO_SendCommand(&SDIO_CmdInitStructure);	
		
		errorstatus=CmdResp1Error(SD_CMD_SEND_STATUS);	//等待R1响应   
		
		if(errorstatus!=SD_OK)return errorstatus;		
		
		cardstatus=SDIO->RESP1;													  
	}
	if(timeout==0)return SD_ERROR;

			SDIO_CmdInitStructure.SDIO_Argument = addr;//发送CMD24,写单块指令,短响应 	
			SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_WRITE_SINGLE_BLOCK;
			SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
			SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
			SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
			SDIO_SendCommand(&SDIO_CmdInitStructure);	
	
	errorstatus=CmdResp1Error(SD_CMD_WRITE_SINGLE_BLOCK);//等待R1响应  
	
	if(errorstatus!=SD_OK)return errorstatus;   	 
	
	StopCondition=0;									//单块写,不需要发送停止传输指令 

	SDIO_DataInitStructure.SDIO_DataBlockSize= power<<4; ;	//blksize, 控制器到卡	
	SDIO_DataInitStructure.SDIO_DataLength= blksize ;
	SDIO_DataInitStructure.SDIO_DataTimeOut=SD_DATATIMEOUT ;
	SDIO_DataInitStructure.SDIO_DPSM=SDIO_DPSM_Enable;
	SDIO_DataInitStructure.SDIO_TransferDir=SDIO_TransferDir_ToCard;
	SDIO_DataInitStructure.SDIO_TransferMode=SDIO_TransferMode_Block;
  SDIO_DataConfig(&SDIO_DataInitStructure);
	
	
	timeout=SDIO_DATATIMEOUT;
	
	if (DeviceMode == SD_POLLING_MODE)
	{
		temp = rt_hw_interrupt_disable();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
		while(!(SDIO->STA&((1<<10)|(1<<4)|(1<<1)|(1<<3)|(1<<9))))//数据块发送成功/下溢/CRC/超时/起始位错误
		{
			if(SDIO_GetFlagStatus(SDIO_FLAG_TXFIFOHE) != RESET)							//发送区半空,表示至少存了8个字
			{
				if((tlen-bytestransferred)<SD_HALFFIFOBYTES)//不够32字节了
				{
					restwords=((tlen-bytestransferred)%4==0)?((tlen-bytestransferred)/4):((tlen-bytestransferred)/4+1);
					
					for(count=0;count<restwords;count++,tempbuff++,bytestransferred+=4)
					{
						SDIO->FIFO=*tempbuff;
					}
				}else
				{
					for(count=0;count<8;count++)
					{
						SDIO->FIFO=*(tempbuff+count);
					}
					tempbuff+=8;
					bytestransferred+=32;
				}
				timeout=0X3FFFFFFF;	//写数据溢出时间
			}else
			{
				if(timeout==0)return SD_DATA_TIMEOUT;
				timeout--;
			}
		} 
		if(SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)		//数据超时错误
		{										   
	 		SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT); 	//清错误标志
			return SD_DATA_TIMEOUT;
	 	}else if(SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)	//数据块CRC错误
		{
	 		SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);  		//清错误标志
			return SD_DATA_CRC_FAIL;		   
		}else if(SDIO_GetFlagStatus(SDIO_FLAG_TXUNDERR) != RESET) 	//接收fifo下溢错误
		{
	 		SDIO_ClearFlag(SDIO_FLAG_TXUNDERR);		//清错误标志
			return SD_TX_UNDERRUN;		 
		}else if(SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET) 	//接收起始位错误
		{
	 		SDIO_ClearFlag(SDIO_FLAG_STBITERR);//清错误标志
			return SD_START_BIT_ERR;		 
		}   
	      
		rt_hw_interrupt_enable(temp);//开启总中断
		SDIO_ClearFlag(SDIO_STATIC_FLAGS);//清除所有标记  
	}else if(DeviceMode==SD_DMA_MODE)
	{
   		TransferError=SD_OK;
		StopCondition=0;			//单块写,不需要发送停止传输指令 
		TransferEnd=0;				//传输结束标置位，在中断服务置1
		SDIO->MASK|=(1<<1)|(1<<3)|(1<<8)|(1<<4)|(1<<9);	//配置产生数据接收完成中断
		SD_DMA_Config((u32*)buf,blksize,DMA_DIR_MemoryToPeripheral);				//SDIO DMA配置
 	 	SDIO->DCTRL|=1<<3;								//SDIO DMA使能.  
 		while(((DMA2->LISR&(1<<27))==RESET)&&timeout)timeout--;//等待传输完成 
		if(timeout==0)
		{
  			SD_Init();	 					//重新初始化SD卡,可以解决写入死机的问题
			return SD_DATA_TIMEOUT;			//超时	 
 		}
		timeout=SDIO_DATATIMEOUT;
		while((TransferEnd==0)&&(TransferError==SD_OK)&&timeout)timeout--;
 		if(timeout==0)return SD_DATA_TIMEOUT;			//超时	 
  		if(TransferError!=SD_OK)return TransferError;
 	}  
 	SDIO_ClearFlag(SDIO_STATIC_FLAGS);//清除所有标记
 	errorstatus=IsCardProgramming(&cardstate);
 	while((errorstatus==SD_OK)&&((cardstate==SD_CARD_PROGRAMMING)||(cardstate==SD_CARD_RECEIVING)))
	{
		errorstatus=IsCardProgramming(&cardstate);
	}   
	return errorstatus;
}
/*******************************************************************************
* Description    : 检查卡是否正在执行写操作 
*               
*
* Input          : 当前状态
* Output         : None
* Return         : SD错误代码
*******************************************************************************/	
static SD_Error IsCardProgramming(u8 *pstatus)
{
 	vu32 respR1 = 0, status = 0;  
  
  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t) RCA << 16; //卡相对地址参数
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEND_STATUS;//发送CMD13 	
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);	
 	
	status=SDIO->STA;
	
	while(!(status&((1<<0)|(1<<6)|(1<<2))))status=SDIO->STA;//等待操作完成
   	if(SDIO_GetFlagStatus(SDIO_FLAG_CCRCFAIL) != RESET)			//CRC检测失败
	{  
	  SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);	//清除错误标记
		return SD_CMD_CRC_FAIL;
	}
   	if(SDIO_GetFlagStatus(SDIO_FLAG_CTIMEOUT) != RESET)			//命令超时 
	{
		SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);			//清除错误标记
		return SD_CMD_RSP_TIMEOUT;
	}
 	if(SDIO->RESPCMD!=SD_CMD_SEND_STATUS)return SD_ILLEGAL_CMD;
	SDIO_ClearFlag(SDIO_STATIC_FLAGS);//清除所有标记
	respR1=SDIO->RESP1;
	*pstatus=(u8)((respR1>>9)&0x0000000F);
	return SD_OK;
}		
/*******************************************************************************
* Description    : SD卡写多个块
*               
*
* Input          : buf:数据缓存区
                   addr:写地址
                   blksize:块大小
                   nblks:要写入的块数
* Output         : None
* Return         : SD错误代码
*******************************************************************************/
static SD_Error SD_WriteMultiBlocks(u8 *buf,long long addr,u16 blksize,u32 nblks)
{
	SD_Error errorstatus = SD_OK;
	u8  power = 0, cardstate = 0;
	u32 timeout=0,bytestransferred=0;
	u32 count = 0, restwords = 0;
	u32 tlen=nblks*blksize;				//总长度(字节)
	u32 *tempbuff = (u32*)buf;  
	register rt_base_t temp;
  if(buf==NULL)return SD_INVALID_PARAMETER; //参数错误  
  SDIO->DCTRL=0x0;							//数据控制寄存器清零(关DMA)   
	
	SDIO_DataInitStructure.SDIO_DataBlockSize= 0; ;	//清除DPSM状态机配置	
	SDIO_DataInitStructure.SDIO_DataLength= 0 ;
	SDIO_DataInitStructure.SDIO_DataTimeOut=SD_DATATIMEOUT ;
	SDIO_DataInitStructure.SDIO_DPSM=SDIO_DPSM_Enable;
	SDIO_DataInitStructure.SDIO_TransferDir=SDIO_TransferDir_ToCard;
	SDIO_DataInitStructure.SDIO_TransferMode=SDIO_TransferMode_Block;
  SDIO_DataConfig(&SDIO_DataInitStructure);
	
	if(SDIO->RESP1&SD_CARD_LOCKED)return SD_LOCK_UNLOCK_FAILED;//卡锁了
 	if(CardType==SDIO_HIGH_CAPACITY_SD_CARD)//大容量卡
	{
		blksize=512;
		addr>>=9;
	}    
	if((blksize>0)&&(blksize<=2048)&&((blksize&(blksize-1))==0))
	{
		power=convert_from_bytes_to_power_of_two(blksize);
		
		SDIO_CmdInitStructure.SDIO_Argument = blksize;	//发送CMD16+设置数据长度为blksize,短响应
		SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
		SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
		SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
		SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
		SDIO_SendCommand(&SDIO_CmdInitStructure);	
		
		errorstatus=CmdResp1Error(SD_CMD_SET_BLOCKLEN);	//等待R1响应  
		
		if(errorstatus!=SD_OK)return errorstatus;   	//响应错误	 
		
	}else return SD_INVALID_PARAMETER;	 
	if(nblks>1)
	{					  
		if(nblks*blksize>SD_MAX_DATA_LENGTH)return SD_INVALID_PARAMETER;   
     	if((SDIO_STD_CAPACITY_SD_CARD_V1_1==CardType)||(SDIO_STD_CAPACITY_SD_CARD_V2_0==CardType)||(SDIO_HIGH_CAPACITY_SD_CARD==CardType))
    	{
			//提高性能
				SDIO_CmdInitStructure.SDIO_Argument = (u32)RCA<<16;		//发送ACMD55,短响应 	
				SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
				SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
				SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
				SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
				SDIO_SendCommand(&SDIO_CmdInitStructure);	
				
			errorstatus=CmdResp1Error(SD_CMD_APP_CMD);		//等待R1响应 
				
			if(errorstatus!=SD_OK)return errorstatus;				 
				
				SDIO_CmdInitStructure.SDIO_Argument =nblks;		//发送CMD23,设置块数量,短响应 	 
				SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCK_COUNT;
				SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
				SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
				SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
				SDIO_SendCommand(&SDIO_CmdInitStructure);
			  
				errorstatus=CmdResp1Error(SD_CMD_SET_BLOCK_COUNT);//等待R1响应 
				
			if(errorstatus!=SD_OK)return errorstatus;		
		    
		} 

				SDIO_CmdInitStructure.SDIO_Argument =addr;	//发送CMD25,多块写指令,短响应 	  
				SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_WRITE_MULT_BLOCK;
				SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
				SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
				SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
				SDIO_SendCommand(&SDIO_CmdInitStructure);	

 		errorstatus=CmdResp1Error(SD_CMD_WRITE_MULT_BLOCK);	//等待R1响应   		   
	
		if(errorstatus!=SD_OK)return errorstatus;

        SDIO_DataInitStructure.SDIO_DataBlockSize= power<<4; ;	//blksize, 控制器到卡	
				SDIO_DataInitStructure.SDIO_DataLength= nblks*blksize ;
				SDIO_DataInitStructure.SDIO_DataTimeOut=SD_DATATIMEOUT ;
				SDIO_DataInitStructure.SDIO_DPSM=SDIO_DPSM_Enable;
				SDIO_DataInitStructure.SDIO_TransferDir=SDIO_TransferDir_ToCard;
				SDIO_DataInitStructure.SDIO_TransferMode=SDIO_TransferMode_Block;
				SDIO_DataConfig(&SDIO_DataInitStructure);
				
		if(DeviceMode==SD_POLLING_MODE)
	    {
			timeout=SDIO_DATATIMEOUT;
			temp = rt_hw_interrupt_disable();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
			while(!(SDIO->STA&((1<<4)|(1<<1)|(1<<8)|(1<<3)|(1<<9))))//下溢/CRC/数据结束/超时/起始位错误
			{
				if(SDIO_GetFlagStatus(SDIO_FLAG_TXFIFOHE) != RESET)							//发送区半空,表示至少存了8字(32字节)
				{	  
					if((tlen-bytestransferred)<SD_HALFFIFOBYTES)//不够32字节了
					{
						restwords=((tlen-bytestransferred)%4==0)?((tlen-bytestransferred)/4):((tlen-bytestransferred)/4+1);
						for(count=0;count<restwords;count++,tempbuff++,bytestransferred+=4)
						{
							SDIO->FIFO=*tempbuff;
						}
					}else 										//发送区半空,可以发送至少8字(32字节)数据
					{
						for(count=0;count<SD_HALFFIFO;count++)
						{
							SDIO->FIFO=*(tempbuff+count);
						}
						tempbuff+=SD_HALFFIFO;
						bytestransferred+=SD_HALFFIFOBYTES;
					}
					timeout=0X3FFFFFFF;	//写数据溢出时间
				}else
				{
					if(timeout==0)return SD_DATA_TIMEOUT; 
					timeout--;
				}
			} 
		if(SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)		//数据超时错误
		{										   
	 		SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT); 	//清错误标志
			return SD_DATA_TIMEOUT;
	 	}else if(SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)	//数据块CRC错误
		{
	 		SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);  		//清错误标志
			return SD_DATA_CRC_FAIL;		   
		}else if(SDIO_GetFlagStatus(SDIO_FLAG_TXUNDERR) != RESET) 	//接收fifo下溢错误
		{
	 		SDIO_ClearFlag(SDIO_FLAG_TXUNDERR);		//清错误标志
			return SD_TX_UNDERRUN;		 
		}else if(SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET) 	//接收起始位错误
		{
	 		SDIO_ClearFlag(SDIO_FLAG_STBITERR);//清错误标志
			return SD_START_BIT_ERR;		 
		}   
	      										   
			if(SDIO_GetFlagStatus(SDIO_FLAG_DATAEND) != RESET)		//发送结束
			{															 
				if((SDIO_STD_CAPACITY_SD_CARD_V1_1==CardType)||(SDIO_STD_CAPACITY_SD_CARD_V2_0==CardType)||(SDIO_HIGH_CAPACITY_SD_CARD==CardType))
				{   
					SDIO_CmdInitStructure.SDIO_Argument =0;//发送CMD12+结束传输 	  
					SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_STOP_TRANSMISSION;
					SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
					SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
					SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
					SDIO_SendCommand(&SDIO_CmdInitStructure);	
					
					errorstatus=CmdResp1Error(SD_CMD_STOP_TRANSMISSION);//等待R1响应   
					if(errorstatus!=SD_OK)return errorstatus;	 
				}
			}
			rt_hw_interrupt_enable(temp);//开启总中断
	 		SDIO_ClearFlag(SDIO_STATIC_FLAGS);//清除所有标记
	    }else if(DeviceMode==SD_DMA_MODE)
		{
	   	TransferError=SD_OK;
			StopCondition=1;			//多块写,需要发送停止传输指令 
			TransferEnd=0;				//传输结束标置位，在中断服务置1
			SDIO->MASK|=(1<<1)|(1<<3)|(1<<8)|(1<<4)|(1<<9);	//配置产生数据接收完成中断
			SD_DMA_Config((u32*)buf,nblks*blksize,DMA_DIR_MemoryToPeripheral);		//SDIO DMA配置
	 	 	SDIO->DCTRL|=1<<3;								//SDIO DMA使能. 
			timeout=SDIO_DATATIMEOUT;
	 		while(((DMA2->LISR&(1<<27))==RESET)&&timeout)timeout--;//等待传输完成 
			if(timeout==0)	 								//超时
			{									  
  				SD_Init();	 					//重新初始化SD卡,可以解决写入死机的问题
	 			return SD_DATA_TIMEOUT;			//超时	 
	 		}
			timeout=SDIO_DATATIMEOUT;
			while((TransferEnd==0)&&(TransferError==SD_OK)&&timeout)timeout--;
	 		if(timeout==0)return SD_DATA_TIMEOUT;			//超时	 
	 		if(TransferError!=SD_OK)return TransferError;	 
		}
  	}
 	SDIO_ClearFlag(SDIO_STATIC_FLAGS);//清除所有标记
 	errorstatus=IsCardProgramming(&cardstate);
 	while((errorstatus==SD_OK)&&((cardstate==SD_CARD_PROGRAMMING)||(cardstate==SD_CARD_RECEIVING)))
	{
		errorstatus=IsCardProgramming(&cardstate);
	}   
	return errorstatus;	   
}
/*******************************************************************************
* Description    : SD读写驱动测试
*               
*
* Input          : 测试的扇区数目
* Output         : None
* Return         : None
*******************************************************************************/
static void SD_test(u32 num)
{
	  u32 flag = 0;
	  u32 i = 0;
	  u8 * pWrite = NULL;
	  u8 * pRead = NULL; 
	  pWrite = rt_malloc(512*num);
	  pRead = rt_malloc(512*num);
	  rt_memset(pRead,'0',512*num);
		for( i = 0;i< num*512; i++)
		{
			pWrite[i] = i;
		}
		
		SD_WriteDisk(pWrite,0,num);

		SD_ReadDisk(pRead,0,num);
		
		for( i = 0;i< num*512; i++)
		{
		    if(pRead[i] != pWrite[i])
				{
			      flag = flag + 1;
				}
		}
		
		rt_kprintf("SD Test Error : %d", flag);
		
		rt_free(pWrite);
		rt_free(pRead);
		
}
/*******************************************************************************
* Description    : 设置SD卡工作模式
*               
*
* Input          : None
* Output         : None
* Return         : sd卡错误码
*******************************************************************************/
static rt_err_t rt_sdcard_init(rt_device_t dev)
{
		if (rt_sem_init(&sd_lock, "sdlock", 1, RT_IPC_FLAG_FIFO) != RT_EOK)
	{
		rt_kprintf("init sd lock semaphore failed\n");
	}
	else
		
	rt_kprintf("SD Card init OK\n");
	return RT_EOK;
}
static rt_err_t rt_sdcard_open(rt_device_t dev, rt_uint16_t oflag)
{
	return RT_EOK;
}
static rt_err_t rt_sdcard_close(rt_device_t dev)
{
	return RT_EOK;
}

static uint32_t dma_buffer[SECTOR_SIZE/sizeof(uint32_t)];
static rt_size_t rt_sdcard_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
			
    SD_Error status;

	  
	
  	rt_sem_take(&sd_lock, RT_WAITING_FOREVER);


	
    if(((uint32_t)buffer & 0x03) != 0)
    {
        /* non-aligned. */
        u32 i;
        rt_size_t sector_adr;
        uint8_t* copy_buffer;

        sector_adr = pos*SECTOR_SIZE;
        copy_buffer = (uint8_t*)buffer;

        for(i=0; i<size; i++)
        {
            status = SD_ReadBlock((uint8_t*)dma_buffer,
                                  sector_adr,
                                  SECTOR_SIZE);

            //status = SD_WaitReadOperation();
            //while(SD_GetStatus() != SD_TRANSFER_OK);

            rt_memcpy(copy_buffer, dma_buffer, SECTOR_SIZE);
            sector_adr += SECTOR_SIZE;
            copy_buffer += SECTOR_SIZE;
        }
    }
    else
    {
        if (size == 1)
        {
            status = SD_ReadBlock((uint8_t*)buffer,
                                  pos*SECTOR_SIZE,
                                  SECTOR_SIZE);
        }
        else
        {
            status = SD_ReadMultiBlocks((uint8_t*)buffer,
                                        pos*SECTOR_SIZE,
                                        SECTOR_SIZE,
                                        size);
        }
        //status = SD_WaitReadOperation();
        //while(SD_GetStatus() != SD_TRANSFER_OK);
    }


		rt_sem_release(&sd_lock);
		if (status == SD_OK) 
		{
			return size;
		}
		else
		{
			rt_kprintf("read failed: %d, buffer 0x%08x\n", status, buffer);
			
			return 0;
		}
		
		
}
static rt_size_t rt_sdcard_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
		SD_Error status;


//	if (CardType == SDIO_HIGH_CAPACITY_SD_CARD) factor = 1;
//	else factor = SECTOR_SIZE;

	//rt_kprintf("sd: write 0x%X, sector 0x%X, 0x%X\n", (uint32_t)buffer , pos, size);
	  rt_sem_take(&sd_lock, RT_WAITING_FOREVER);

    if(((uint32_t)buffer & 0x03) != 0)
    {
        /* non-aligned. */
        uint32_t i;
        rt_size_t sector_adr;
        uint8_t* copy_buffer;

        sector_adr = pos*SECTOR_SIZE;
        copy_buffer = (uint8_t*)buffer;

        for(i=0; i<size; i++)
        {
            rt_memcpy(dma_buffer, copy_buffer, SECTOR_SIZE);
            status = SD_WriteBlock((uint8_t*)dma_buffer,
                                  sector_adr,
                                  SECTOR_SIZE);

            sector_adr += SECTOR_SIZE;
            copy_buffer += SECTOR_SIZE;

//            status = SD_WaitWriteOperation();
//            while(SD_GetStatus() != SD_TRANSFER_OK);
        }
    }
    else
    {
        if (size == 1)
        {
            status = SD_WriteBlock((uint8_t*)buffer, pos*SECTOR_SIZE, SECTOR_SIZE);
        }
        else
        {
            status = SD_WriteMultiBlocks((uint8_t*)buffer, pos*SECTOR_SIZE, SECTOR_SIZE, size);
        }
//        status = SD_WaitWriteOperation();
//        while(SD_GetStatus() != SD_TRANSFER_OK);
    }
	// }
	  rt_sem_release(&sd_lock);

		if (status == SD_OK) 
		{
			return size;
		}
		else
		{
			rt_kprintf("write failed: %d, buffer 0x%08x\n", status, buffer);
			
			return 0;
		}
}
static rt_err_t rt_sdcard_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
		RT_ASSERT(dev != RT_NULL);

			if (cmd == RT_DEVICE_CTRL_BLK_GETGEOME)
			{
					struct rt_device_blk_geometry *geometry;

					geometry = (struct rt_device_blk_geometry *)args;
					if (geometry == RT_NULL) return -RT_ERROR;

					geometry->bytes_per_sector = 512;
					geometry->block_size = SDCardInfo.CardBlockSize;
			if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
				geometry->sector_count = (SDCardInfo.SD_csd.DeviceSize + 1)  * 1024;
			else
						geometry->sector_count = SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize;
			}

		return RT_EOK;
}
