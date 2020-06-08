/***************************************************************************

Copyright (C), 1999-2018, Tongye Tech. Co., Ltd.

* @file           fat_service.c
* @author        
* @version        V1.0.0
* @date           2018-08-17
* @brief          (1)  
                  (2) 
                  (3) 
History:          // Revision Records

<Author>             <time>       <version >            <desc>

Lily, Life           2018-08-17       V1.0.0               

***************************************************************************/
/* Includes --------------------------------------------------------------*/

#include "fat_service.h"
//#include "Sdatatype.h"
#include "crc16.h"
#include "sdio_sd.h"
#include "dfs.h"
#include "dfs_fs.h"
#include "ff.h"
#include <dfs_elm.h>


//---------------------------------------------------------------------------
//                        Task Macro 
//---------------------------------------------------------------------------
#define BSP_FAT_SERVICE_STACKSIZE   (4 * 1024 / 4)  
#define BSP_FAT_SERVICE_PRIORITY    (0x1C)

#define  FATSERV_ST_INIT        (0x00)
#define  FATSERV_ST_CFGWAIT     (0x01)
#define  FATSERV_ST_CFG         (0x02)
#define  FATSERV_ST_SERVECREAT  (0x03)
#define  FATSERV_ST_NORMAL      (0x04)
#define  FATSERV_ST_PTU         (0x05)
#define  FATSERV_ST_FAULT       (0xFF)

#define  FATSERV_ENABLE         (0xF34576)

typedef struct _fat_service_controlData_
{
	uint8_t  serviceSt;
	uint8_t  cfgvalidFlag;
}FATSRVICE_CONTROL;
//---------------------------------------------------------------------------
//                        Task Object
//---------------------------------------------------------------------------
static  FATSRVICE_CONTROL  fatServ_control = { 0 };
static char fat_log_buf[RT_CONSOLEBUF_SIZE];

//---------------------------------------------------------------------------
//   Task stack memory static allocate
//   Task object variable define
//---------------------------------------------------------------------------
static uint32_t Bsp_FatService_Stack[BSP_FAT_SERVICE_STACKSIZE];
static struct rt_thread Bsp_FatService_thread;
/**/
/*邮箱控制块*/
static struct rt_mailbox fat_mb;
/*用于放邮件的内存池*/
static uint8_t fat_mb_pool[20 * 4];
/*收取邮件的变量*/
static uint32_t rec_fat_mbox = 0u;
static uint32_t fat_mb_init = 0xFFu;

/* 可以提供服务的标志 */
static volatile uint32_t fatSrvEnable = 0;

/* system event dir config file.*/
static FSEVTConfg_TypeDef sysEcfg;
//---------------------------------------------------------------------------
//                      Private functions 
//---------------------------------------------------------------------------
static void FatService_Task(void* parameter);
static int FatService_Init(void);
static int FatService_InitDirBase(void);
static unsigned long fat_printf(char *logbuf, const char *fmt, ...);
static long fat_CreatEvtCfgFile(FSEVTConfg_TypeDef *ecf);
static long fat_ChkEvtCfgFile(FSEVTConfg_TypeDef *ecf);
static long fat_saveEvtCfgFile(FSEVTConfg_TypeDef *ecf);
static long fat_EventDStoFile(FSEVTConfg_TypeDef *ecf, uint8_t *buf, uint32_t size);

static void FatService_MailboxInit(void);
static long FatService_Notification(uint32_t data);
static long FatService_MailAccess(void);
static long FatService_MailAccessMaintain(void);
//---------------------------------------------------------------------------
//                      External functions 
//---------------------------------------------------------------------------


/*******************************************************************************
* Description    : 
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  Fat_Service_CreateApp(void)
{
    fatSrvEnable = 0;
    
    rt_thread_init(&Bsp_FatService_thread,
        "FatSrv",
        FatService_Task,
        RT_NULL,
        &Bsp_FatService_Stack[0], 
        sizeof(Bsp_FatService_Stack),
        BSP_FAT_SERVICE_PRIORITY, 
        20
        );

    rt_thread_startup(&Bsp_FatService_thread);
	
}
/*******************************************************************************
* Description    : 
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static uint32_t tt = 0u;
static void FatService_Task(void* parameter)
{
    int initResult = -1;
    long retMail = 0;
    
    fatServ_control.serviceSt = FATSERV_ST_INIT;
	
	for (;;)
	{
		switch(fatServ_control.serviceSt)
		{
			case FATSERV_ST_INIT:
                rt_thread_delay(300);
				initResult = FatService_Init();            
                if(initResult >= 0)
                {
                    fatServ_control.serviceSt = FATSERV_ST_CFG;
                }
				break;
			case FATSERV_ST_CFGWAIT:
				break;
			case FATSERV_ST_CFG:                		
                rt_thread_delay(100);
                FatService_InitDirBase();
                fatServ_control.serviceSt = FATSERV_ST_SERVECREAT;
				break;
			case FATSERV_ST_SERVECREAT:
                tt = rt_tick_get();
                
                if(fat_ChkEvtCfgFile(&sysEcfg) != SPD_OPERTION_OK)
                {
                    fat_CreatEvtCfgFile(&sysEcfg);
                    rt_thread_delay(50);
                }
                else
                {
                    fatSrvEnable = FATSERV_ENABLE;
                    fatServ_control.serviceSt = FATSERV_ST_NORMAL;
                }
                tt = rt_tick_get() - tt;
				break;			
			case FATSERV_ST_NORMAL:
                retMail = FatService_MailAccess();
            
                if(retMail == SPD_OPERTION_EXITSRV)
                {
                    fatServ_control.serviceSt = FATSERV_ST_PTU;
                }
                //rt_thread_delay(3000);
				break;
            case FATSERV_ST_PTU:
                retMail = FatService_MailAccessMaintain();
                if(retMail == SPD_OPERTION_EXITSRV)
                {
                    fatServ_control.serviceSt = FATSERV_ST_NORMAL;
                }
                break;
			case FATSERV_ST_FAULT:
				break;
			case 0x11:
				
				break;
			default:
				break;
		}
	}

}


/*******************************************************************************
* Function Name  :
* Description    : [8/17/2018 Lily]
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static int FatService_Init(void)
{
    int retVal = -1;
    
    /* Delay for SD card ready. */
    rt_thread_delay(100);
    /* init the device filesystem */
    dfs_init();

    /* init the elm chan FatFs filesystam*/
    elm_init();

    /* init the SDCart device filesystem */
    rt_hw_sdcard_init();
    
    /* 邮件服务开启 */                
    FatService_MailboxInit();
    
    if (dfs_mount("sd0", "/", "elm", 0, 0) == 0)
    {
        rt_kprintf("File System initialized!\n");
        retVal = 0;
    }
    else
    {
        rt_kprintf("File System Erro!\n");
        retVal = -2;
    }
    
    return retVal;
}

/*******************************************************************************
* Function Name  :
* Description    : [8/17/2018 Lily]
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#include <time.h>
static int FatService_InitDirBase(void)
{
    FIL fp;
    UINT byte_write;
    UINT byte_to_write;
    time_t now;

    time(&now);

    /*Check secure directory , name format dos:8.3*/
    f_mkdir("Scfg");
    f_mkdir("Sevt");
    f_mkdir("Sflt");
    f_mkdir("Slog");
    f_mkdir("Sprg");
    
    f_open(&fp, "Slog/syslog.txt", FA_OPEN_ALWAYS | FA_WRITE);
    byte_write = f_size(&fp);
    f_lseek(&fp, byte_write);
    time(&now);
    byte_to_write = fat_printf(fat_log_buf, "System bootup log at :%s \n",ctime(&now));
    f_write(&fp, fat_log_buf, byte_to_write, &byte_write);
    f_close(&fp);
    
    return 0;
}


/*******************************************************************************
* Function Name  :
* Description    : [8/17/2018 Lily]
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static unsigned long fat_printf(char *logbuf, const char *fmt, ...)
{
    va_list args;
    rt_size_t length;

    va_start(args, fmt);
    /* the return value of vsnprintf is the number of bytes that would be
     * written to buffer had if the size of the buffer been sufficiently
     * large excluding the terminating null byte. If the output string
     * would be larger than the rt_log_buf, we have to adjust the output
     * length. */
    length = rt_vsnprintf(logbuf, (RT_CONSOLEBUF_SIZE - 1), fmt, args);
    va_end(args);
    
    return length;
}



/**
* @ Function    : void FatService_MailboxInit(void)                                      
* @ Descriptions:  邮箱初始化   
* @ input       :                   
* @ output      :                   
* @ Returned    :                     
**/
static void FatService_MailboxInit(void)
{
    //邮箱初始化
    fat_mb_init = rt_mb_init(&fat_mb, //
        "fat_mb",                      //邮箱名称
        fat_mb_pool,                   //邮箱内存池
        sizeof(fat_mb_pool) / 4,       //邮箱大小是内存池的大小除以4，因为每封邮件大小是4个字节
        RT_IPC_FLAG_FIFO                 //采用FIFO方式进行线程等待
        );
}


/**
* @ Function    : FatService_Notification(uint32_t data)                                      
* @ Descriptions:     
* @ input       : data 数据缓存的指针                  
* @ output      :                   
* @ Returned    :                     
**/
static long FatService_Notification(uint32_t data)
{
    long ret = -1;
    
    //发送数据至邮箱
    if (fat_mb_init == RT_EOK)
    {
        ret = rt_mb_send(&fat_mb, data);
        if(ret == RT_EOK)
        {
            ret = SPD_OPERTION_OK;
        }
        else
        {
            ret = SPD_OPERTION_ERRCALL;
        }
    }
    else
    {
        ret = SPD_OPERTION_ERRNOINIT;
    }
    
    return ret;
}




/**
* @ Function    : FatSrv_Write(void *buf, uint32_t size, uint32_t type, fatsrv_callback cbk)                                     
* @ Descriptions:     
* @ input       :                   
* @ output      :                   
* @ Returned    :                     
**/
long  FatSrv_Write(void *buf, uint32_t size, uint32_t type, fatsrv_callback cbk)
{
    long ret = -1;    
    uint32_t bufSize;
    uint32_t dsBlockSize;
    uint16_t tCrc;
    void *mailPtr;
    uint8_t *PtrDataPayload;


    if(fatSrvEnable != FATSERV_ENABLE)
    {
        ret = SPD_OPERTION_ERRSRV;
    }
    else
    {
    
        switch(type)
        {
            case SPD_OPERTION_WREVT:
                dsBlockSize = SPD_DSBLK_4K;
                break;
            case SPD_OPERTION_WRCFG:
                dsBlockSize = (size + sizeof(SDataSetParent_TypeDef));
                if(dsBlockSize > SPD_DSBLK_MAX)
                {
                    dsBlockSize = 0u;
                }
                break;
            case SPD_OPERTION_PTUMEN:
            case SPD_OPERTION_PTUDIS:
                /* 进入和退出PTU服务用的邮件 2019-4-18 */
                dsBlockSize = (size + sizeof(SDataSetParent_TypeDef));
                if(dsBlockSize > SPD_DSBLK_MAX)
                {
                    dsBlockSize = 0u;
                }

                break;
            default:
                /* 通过封锁块申请空间来屏蔽非法操作类型 */
                dsBlockSize = 0u;
                break;
        }
        
        //-------------------------------------------------------------
        // 数据申请空间的校核
        //-------------------------------------------------------------
        bufSize = size + sizeof(SDataSetParent_TypeDef);

        if(dsBlockSize == 0u)
        {
            ret = SPD_OPERTION_ERRTYPE;
        }
        else if(bufSize > dsBlockSize)
        {
            ret = SPD_OPERTION_ERRSIZE;
        }
        else
        {
            /* mailPtr = rt_malloc(bufSize); */
            /* 这里应使用分配的块空间大小，实际的文件一次写入的均为块空间长度。 */
            mailPtr = rt_malloc(dsBlockSize);
            if (mailPtr != RT_NULL)
            {
                /* 数据块的头信息填充 */
                ((SDataSetParent_TypeDef *)mailPtr)->magicFlag = SDATA_MAGIC;
                ((SDataSetParent_TypeDef *)mailPtr)->systick  = rt_tick_get();
                ((SDataSetParent_TypeDef *)mailPtr)->sysdateTime  = get_fattime();
                ((SDataSetParent_TypeDef *)mailPtr)->dataSetSize = size;
                ((SDataSetParent_TypeDef *)mailPtr)->callback = cbk;
                ((SDataSetParent_TypeDef *)mailPtr)->dataType = type;

                /* 有效数据的填充 */
                PtrDataPayload = (uint8_t *)mailPtr;
                PtrDataPayload += sizeof(SDataSetParent_TypeDef);
                rt_memcpy(PtrDataPayload, buf, size);
                /* crc16 update */
                tCrc = CRC16_Dn_Cal((uint8_t*)PtrDataPayload, size, 0xFFFFu);
                ((SDataSetParent_TypeDef *)mailPtr)->crc16 = tCrc;
                /* 通过邮件发送至 Fat Service */
                ret = FatService_Notification((uint32_t)mailPtr);
                if(ret != SPD_OPERTION_OK)
                {
                    /* 发送邮件失败，释放申请的内存 */
                    rt_free(mailPtr);
                    rt_kprintf(" -> evt send mail fault! \r\n");
                }
            } 
            else
            {
                ret = SPD_OPERTION_ERRNULL;
            }
        }
    }
    
    return ret;
}



/**
* @ Function    : FatService_MailAccess(void)                                      
* @ Descriptions:     
* @ input       :                    
* @ output      :                   
* @ Returned    :                     
**/
static long FatService_MailAccess(void)
{
    uint8_t *recBuf;
    SDataSetParent_TypeDef *sdp;
    long ret = 0;
    
    rt_mb_recv(&fat_mb, (rt_uint32_t*)&rec_fat_mbox, RT_WAITING_FOREVER);
    
    recBuf = (uint8_t*)rec_fat_mbox;
    sdp = (SDataSetParent_TypeDef *)recBuf;
	
    if(sdp->magicFlag != SDATA_MAGIC)
    {
        /* 数据严重出错，此处到底释放这个内存块还是进行什么安全处理，
           待日后处理。暂时做释放处理 */
        rt_free((void *)rec_fat_mbox);
        return SPD_OPERTION_ERRPTR;
    }
    
    switch(sdp->dataType)
    {
        case SPD_OPERTION_WREVT:
            ret = fat_EventDStoFile(&sysEcfg, recBuf, SPD_DSBLK_4K);
            rt_kprintf("-fat wr at:%08d\r\n", rt_tick_get());
            break;
        case SPD_OPERTION_WRCFG:            
            break;
        case SPD_OPERTION_PTUMEN:  
            /* 进入PTU维护模式，不进行事件记录 */ 
            ret = SPD_OPERTION_EXITSRV;      
            break;
        default:            
            break;
    }
    /* 释放邮件空间 */            
    rt_free((void*)rec_fat_mbox);
    return ret;
}

/* 进入PTU维护模式后，监视退出通知 */
static long FatService_MailAccessMaintain(void)
{
    uint8_t *recBuf;
    SDataSetParent_TypeDef *sdp;
    long ret = 0;
    
    rt_mb_recv(&fat_mb, (rt_uint32_t*)&rec_fat_mbox, RT_WAITING_FOREVER);
    
    recBuf = (uint8_t*)rec_fat_mbox;
    sdp = (SDataSetParent_TypeDef *)recBuf;
    
    if(sdp->magicFlag != SDATA_MAGIC)
    {
        /* 数据严重出错，此处到底释放这个内存块还是进行什么安全处理，
           待日后处理。暂时做释放处理 */
        rt_free((void *)rec_fat_mbox);
        return SPD_OPERTION_ERRPTR;
    }
    
    switch(sdp->dataType)
    {
        case SPD_OPERTION_WREVT:
            break;
        case SPD_OPERTION_WRCFG:            
            break;
        case SPD_OPERTION_PTUDIS:  
            /* 退出PTU维护模式，返回事件记录 */ 
            ret = SPD_OPERTION_EXITSRV;      
            rt_kprintf("-Ptu maitain exit:%08d\r\n", rt_tick_get());
            break;
        default:            
            break;
    }
    /* 释放邮件空间 */            
    rt_free((void*)rec_fat_mbox);
    
    return ret;
}

/* */
static long fat_EventDStoFile(FSEVTConfg_TypeDef *ecf, uint8_t *buf, uint32_t size)
{
    long ret;
    //uint32_t i;
    //uint32_t tk;
    FIL fp;
    UINT byte_write;
    //UINT byte_to_write;

    if(ecf->currentNum >= ecf->fileMaxNum)
    {
        ret = SPD_OPERTION_ERREVTCFG;
    }
    else
    {
        /* Open eventlog file, seek the end line, ready to write.*/
        if(f_open(&fp, ecf->filNames[ecf->currentNum], FA_OPEN_ALWAYS | FA_WRITE) == FR_OK)
        {
            /* 获取当前操作文件的大小 */
            byte_write = f_size(&fp);
            /* 判断单个文件尺寸是否超标，超标则更换下一个 */
            if(byte_write >= FS_EVTFILE_SIZE)
            {
                /* 换下一个日志文件，使用FA_CREATE_ALWAYS标志创建，清空原有数据 */
                f_close(&fp);
                
                /* 生成下一个文件操作编号，并更新配置文件到SD卡 */
                ecf->currentNum++;
                ecf->currentNum %= ecf->fileMaxNum;
                fat_saveEvtCfgFile(ecf);
                /* 创建新的日志文件，如果存在老的文件，则清空原有的数据记录 */
                f_open(&fp, ecf->filNames[ecf->currentNum], FA_CREATE_ALWAYS | FA_WRITE);
                byte_write = f_size(&fp);
                f_write(&fp, buf, size, &byte_write);
                f_close(&fp);
            }
            else
            {
                /* 单个文件还没写满，继续使用当前文件 */
                f_lseek(&fp, byte_write);
                f_write(&fp, buf, size, &byte_write);
                f_close(&fp);
            }         
            ret = SPD_OPERTION_OK;
        }
        else
        {
            ret = SPD_OPERTION_ERRCREATE;
        }        
    }
    
    return ret;
}





/**
* @ Function    : fat_CreatEvtCfgFile(FSEVTConfg_TypeDef *ecf)                                      
* @ Descriptions:     
* @ input       :                    
* @ output      :                   
* @ Returned    :                     
**/
static long fat_CreatEvtCfgFile(FSEVTConfg_TypeDef *ecf)
{
    FIL fp;
    UINT byte_write = 0;

    long ret;
    FSEVTConfg_TypeDef* eCfg;
    int i;
    
    eCfg = (FSEVTConfg_TypeDef*)rt_malloc(sizeof(FSEVTConfg_TypeDef));
    
    if(eCfg != RT_NULL)
    {
        eCfg->magicFlag = SDATA_MAGIC;
        eCfg->fattime = get_fattime();
        eCfg->fileMaxNum = FS_EVTFILE_NUMS;
        eCfg->currentNum = 0;
        
        for(i = 0;i < FS_EVTFILE_NUMS;i++)
        {
            fat_printf(eCfg->filNames[i], "Sevt/evtD%02d.dat", i);
        }
        
        eCfg->crc16 = CRC16_Dn_Cal((uint8_t*)eCfg, (sizeof(FSEVTConfg_TypeDef) - 4), 0xFFFFu);
        /* Create new evtcfg file */
        if(f_open(&fp, FS_EVFCFG_FILENAME, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
        {
            /* save data to config file. */
            f_write(&fp, eCfg, sizeof(FSEVTConfg_TypeDef), &byte_write);
            f_close(&fp);
            /* Load new file config data to sysConfig var.*/
            rt_memcpy(ecf, eCfg, sizeof(FSEVTConfg_TypeDef));
            ret = SPD_OPERTION_OK;
        }
        else
        {
            ret = SPD_OPERTION_ERRCREATE;
        }
        
        rt_free(eCfg);
    }
    else
    {
        ret = SPD_OPERTION_ERRNULL;
    }
    
    
    return ret;
}

/**
* @ Function    : fat_ChkEvtCfgFile(FSEVTConfg_TypeDef *ecf)                                      
* @ Descriptions:     
* @ input       :                    
* @ output      :                   
* @ Returned    :                     
**/
static long fat_ChkEvtCfgFile(FSEVTConfg_TypeDef *ecf)
{
    FIL fp;
    UINT byte_read = 0;
    DWORD length;
    uint16_t crcTem;

    long ret;
    FSEVTConfg_TypeDef* eCfg;
    
    if(f_open(&fp, FS_EVFCFG_FILENAME, FA_READ) == FR_OK)
    {
        eCfg = (FSEVTConfg_TypeDef*)rt_malloc(sizeof(FSEVTConfg_TypeDef));
        
        if(eCfg != RT_NULL)
        {
            length = f_size(&fp);
            if(length != sizeof(FSEVTConfg_TypeDef))
            {
                /*File size error.*/
                ret = SPD_OPERTION_ERREEFILE;
                f_close(&fp);
            }
            else
            {
                /* load file data to RAM */
                f_read(&fp, eCfg, length, &byte_read);
                f_close(&fp);
                /* check file crc16 code.*/
                crcTem = CRC16_Dn_Cal((uint8_t*)eCfg, (sizeof(FSEVTConfg_TypeDef) - 4), 0xFFFFu);
                if((crcTem != eCfg->crc16) || (eCfg->magicFlag != SDATA_MAGIC))
                {
                    ret = SPD_OPERTION_ERREEFILE;
                }
                else
                {
                    rt_memcpy(ecf, eCfg, sizeof(FSEVTConfg_TypeDef));
                    ret = SPD_OPERTION_OK;
                }
            }
            
            rt_free(eCfg);
        }
        else
        {
            ret = SPD_OPERTION_ERRNULL;
        }
    }
    else
    {
        ret = SPD_OPERTION_ERRNOFILE;
    }

    return ret;
}

/**
* @ Function    : fat_saveEvtCfgFile(FSEVTConfg_TypeDef *ecf)                                      
* @ Descriptions:     
* @ input       :                    
* @ output      :                   
* @ Returned    :                     
**/
static long fat_saveEvtCfgFile(FSEVTConfg_TypeDef *ecf)
{
    FIL fp;
    UINT byte_write = 0;
    long ret;
    
    if(    (ecf->magicFlag != SDATA_MAGIC) 
        || (ecf->fileMaxNum != FS_EVTFILE_NUMS)
        || (ecf->currentNum >= ecf->fileMaxNum)
    )
    {
        ret = SPD_OPERTION_ERREEFILE;
    }
    else
    {
        ecf->fattime = get_fattime();
        ecf->crc16 = CRC16_Dn_Cal((uint8_t*)ecf, (sizeof(FSEVTConfg_TypeDef) - 4), 0xFFFFu);
        /* Create new evtcfg file */
        if(f_open(&fp, FS_EVFCFG_FILENAME, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
        {
            /* save data to config file. */
            f_write(&fp, ecf, sizeof(FSEVTConfg_TypeDef), &byte_write);
            f_close(&fp);
            ret = SPD_OPERTION_OK;
        }
        else
        {
            ret = SPD_OPERTION_ERRCREATE;
        }

    }
    
    return ret;
}



/* PTU下载日志协议处理函数 2019-4-18 */
static uint8_t fb[36] ={
    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
    17,18,19,20,21,22,23,24,25,26,27,28,29,
    30,31,32,33,34,35,36
};
static FIL fp = {0};
//static uint8_t fileTemp[4096] = {0};

void FatPtu_DownloadEvt(uint8_t* input, uint8_t* outbuff, uint32_t* size)
{
    SDataPtu_TypeDef* ptuIn;
    uint8_t* PtrDataPayload;
    uint16_t tCrc;
    uint16_t temp;
    uint16_t crcChkOK = 0;
    

    UINT byte_read = 0;
    int length = 0;
    int lenTemp;
    
    ptuIn = (SDataPtu_TypeDef*)input;
    
    if(ptuIn->magicFlag == SDATA_MAGIC) 
    {
        PtrDataPayload = input;
        PtrDataPayload += sizeof(SDataPtu_TypeDef);
        tCrc = CRC16_Dn_Cal((uint8_t*)PtrDataPayload, ptuIn->dataSetSize, 0xFFFFu);
        if(tCrc == ptuIn->crc16)
        {
            crcChkOK = 0x92;
        }
    }
    
    if(crcChkOK == 0x92)
    {
        switch(ptuIn->cmdType)
        {
            case 0xD0000000U:
                /*通知记录服务退出EVENT记录服务，转到PTU下载服务*/
                FatSrv_Write(fb,8,SPD_OPERTION_PTUMEN,RT_NULL);
                rt_thread_delay(80);
                if(fatServ_control.serviceSt == FATSERV_ST_PTU)
                {
                    fb[0] = 1; //enter ptu
                    
                }
                else
                {
                    fb[0] = 0; //fail enter ptu
                }
                rt_memcpy((outbuff + sizeof(SDataPtuAck_TypeDef)), fb, 4);
                tCrc = CRC16_Dn_Cal((uint8_t*)fb, 4, 0xFFFFu);
                ((SDataPtuAck_TypeDef*)outbuff)->cmdType = ptuIn->cmdType;
                ((SDataPtuAck_TypeDef*)outbuff)->packId = 0;
                ((SDataPtuAck_TypeDef*)outbuff)->dataSetSize = 4;
                ((SDataPtuAck_TypeDef*)outbuff)->crc16 = tCrc;
                /* 更新发送数据长度*/
                *size = sizeof(SDataPtuAck_TypeDef) + 4;
                break;
            case 0xD0000001U:
                rt_memcpy((outbuff + sizeof(SDataPtuAck_TypeDef)), &sysEcfg, sizeof(FSEVTConfg_TypeDef));
                tCrc = CRC16_Dn_Cal((uint8_t*)&sysEcfg, sizeof(FSEVTConfg_TypeDef), 0xFFFFu);
                ((SDataPtuAck_TypeDef*)outbuff)->cmdType = ptuIn->cmdType;
                ((SDataPtuAck_TypeDef*)outbuff)->packId = 0;
                ((SDataPtuAck_TypeDef*)outbuff)->dataSetSize = sizeof(FSEVTConfg_TypeDef);
                ((SDataPtuAck_TypeDef*)outbuff)->crc16 = tCrc;
                /* 更新发送数据长度*/
                *size = sizeof(SDataPtuAck_TypeDef) + sizeof(FSEVTConfg_TypeDef);
                break;
            case 0xD0000002U:
                /*获取指定文件编号*/
                temp = PtrDataPayload[0] + (PtrDataPayload[1] << 8); 
                *(int*)fb = 0;
                if(temp < sysEcfg.fileMaxNum)
                {
                    if(f_open(&fp, sysEcfg.filNames[temp], FA_READ) == FR_OK)
                    {
                        length = f_size(&fp);
                        *(int*)fb = length;
                    }
                }
                rt_memcpy((outbuff + sizeof(SDataPtuAck_TypeDef)), fb, 4);
                tCrc = CRC16_Dn_Cal((uint8_t*)fb, 4, 0xFFFFu);
                ((SDataPtuAck_TypeDef*)outbuff)->cmdType = ptuIn->cmdType;
                ((SDataPtuAck_TypeDef*)outbuff)->packId = 0;
                ((SDataPtuAck_TypeDef*)outbuff)->dataSetSize = 4;
                ((SDataPtuAck_TypeDef*)outbuff)->crc16 = tCrc;
                /* 更新发送数据长度*/
                *size = sizeof(SDataPtuAck_TypeDef) + 4;
                break;
            case 0xD0000003U:
                *(int*)fb = 0;
                if(fp.fs != RT_NULL)
                {
                    temp = f_close(&fp);
                    if(temp == FR_OK)
                    {
                        *(int*)fb = 1;
                    }
                }
            
                rt_memcpy((outbuff + sizeof(SDataPtuAck_TypeDef)), fb, 4);
                tCrc = CRC16_Dn_Cal((uint8_t*)fb, 4, 0xFFFFu);
                ((SDataPtuAck_TypeDef*)outbuff)->cmdType = ptuIn->cmdType;
                ((SDataPtuAck_TypeDef*)outbuff)->packId = 0;
                ((SDataPtuAck_TypeDef*)outbuff)->dataSetSize = 4;
                ((SDataPtuAck_TypeDef*)outbuff)->crc16 = tCrc;
                /* 更新发送数据长度*/
                *size = sizeof(SDataPtuAck_TypeDef) + 4;

                break;
            case 0xD0000004U:
                /*通知记录服务退出PTU服务，转到EVT记录服务*/
                FatSrv_Write(fb, 8, SPD_OPERTION_PTUDIS, RT_NULL);
                rt_thread_delay(80);
                if(fatServ_control.serviceSt == FATSERV_ST_NORMAL)
                {
                    fb[0] = 1; //enter ptu
                    
                }
                else
                {
                    fb[0] = 0; //fail enter ptu
                }
                rt_memcpy((outbuff + sizeof(SDataPtuAck_TypeDef)), fb, 4);
                tCrc = CRC16_Dn_Cal((uint8_t*)fb, 4, 0xFFFFu);
                ((SDataPtuAck_TypeDef*)outbuff)->cmdType = ptuIn->cmdType;
                ((SDataPtuAck_TypeDef*)outbuff)->packId = 0;
                ((SDataPtuAck_TypeDef*)outbuff)->dataSetSize = 4;
                ((SDataPtuAck_TypeDef*)outbuff)->crc16 = tCrc;
                /* 更新发送数据长度*/
                *size = sizeof(SDataPtuAck_TypeDef) + 4;

                break;
            case 0xD0000005U:
                /*获取指定偏移编号*/
                temp = PtrDataPayload[0] + (PtrDataPayload[1] << 8); 
                lenTemp = f_size(&fp);
                lenTemp = lenTemp / 4096;
                if(temp >= lenTemp)
                {
                    *size = 0;
                }
                else
                {
                    if(fp.fs != RT_NULL)
                    {
                        f_lseek(&fp, (temp * 4096));
                        /* f_read(&fp, fileTemp, 4096, &byte_read); */
                        f_read(&fp, (void*)(outbuff + sizeof(SDataPtuAck_TypeDef)), 4096, &byte_read);
                        tCrc = CRC16_Dn_Cal((uint8_t*)(outbuff + sizeof(SDataPtuAck_TypeDef)), 4096, 0xFFFFu);
                        ((SDataPtuAck_TypeDef*)outbuff)->cmdType = ptuIn->cmdType;
                        ((SDataPtuAck_TypeDef*)outbuff)->packId = 0;
                        ((SDataPtuAck_TypeDef*)outbuff)->dataSetSize = 4096;
                        ((SDataPtuAck_TypeDef*)outbuff)->crc16 = tCrc;
                        /* 更新发送数据长度*/
                        *size = sizeof(SDataPtuAck_TypeDef) + 4096;
                    }
                    else
                    {
                        *size = 0;
                    }
                }

                break;
            default:
                *size = 0;
                break;
        }
    }
    else
    {
        *size = 0;
        rt_kprintf("[Ptu] CrcErr!  \r\n");
    }
}

















void tc_evt(void)
{
    //tc_evt void *buf, uint32_t size, uint32_t type, fatsrv_callback cbk, 4096 SPD_OPERTION_WREVT
    FatSrv_Write((void *)(BKPSRAM_BASE), 4000, SPD_OPERTION_WREVT, RT_NULL);
}

void tc_evt1(void)
{
    int i;
    
    for(i = 0; i < 1024; i++)
    {
        FatSrv_Write((void *)(BKPSRAM_BASE), 4000, SPD_OPERTION_WREVT, RT_NULL);
        rt_kprintf("  evt testCase: %04d\r\n", i);
        rt_thread_delay(20);
    }
    rt_kprintf("evt testCase: write 4M data to SD card.\r\n");
}

void tc_evt2(void)
{
    int i;
    
    for(i = 0; i < (1024 * 16); i++)
    {
        FatSrv_Write((void *)(BKPSRAM_BASE), 4000, SPD_OPERTION_WREVT, RT_NULL);
        rt_kprintf("  evt testCase: %04d\r\n", i);
        rt_thread_delay(2);
    }
    rt_kprintf("evt testCase: write 64M data to SD card.\r\n");
}

/* 320KB write , need 183ms .*/
void tc_log1(void)
{
    uint32_t tk;
    FIL fp;
    UINT byte_write;
    /*UINT byte_to_write;*/

    /* Open syslog file, seek the end line, ready to write.*/
    f_open(&fp, "Slog/sysData.dat", FA_OPEN_ALWAYS | FA_WRITE);
    byte_write = f_size(&fp);
    f_lseek(&fp, byte_write);
    
    tk = rt_tick_get();
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);

    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);

    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);

    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);

    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);

    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);

    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);

    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    f_write(&fp, (void *)(BKPSRAM_BASE), 4096, &byte_write);
    tk = rt_tick_get() - tk;
    f_close(&fp);
    rt_kprintf(" -WR 320KB data tick:%04d", tk);
}

void tc_rd01(uint8_t id, uint32_t start, uint32_t size)
{
    FIL fp;
    UINT byte_read = 0;
    int length = 0;
    uint8_t* buf;
    int i;
    
	rt_kprintf("\r\n  ============ Print EventRecord Data ============");
	rt_kprintf("\r\n  SystemEvtConfig current File  : %s", sysEcfg.filNames[sysEcfg.currentNum]);
    
    if(f_open(&fp, sysEcfg.filNames[id], FA_READ) == FR_OK)
    {
        buf = rt_malloc(size);
        length = f_size(&fp);
        rt_kprintf("\r\n  Open File  : %s  filesize : %010d", sysEcfg.filNames[id], length);
        f_lseek(&fp, start);
        f_read(&fp, buf, size, &byte_read);
        rt_kprintf("\r\n\r\n");
        /* 16个字节一行，打印size buf 的内容  */
        for (i = 0x0; i < size; i += 0x10)
        {
            //rt_kprintf("0x%08x   ", (BKPSRAM_BASE + i));
            rt_kprintf("%02x  %02x  %02x  %02x  %02x  %02x  %02x  %02x  ",
                (*(__IO uint8_t *) (buf + i + 0)),
                (*(__IO uint8_t *) (buf + i + 1)),
                (*(__IO uint8_t *) (buf + i + 2)),
                (*(__IO uint8_t *) (buf + i + 3)),
                (*(__IO uint8_t *) (buf + i + 4)),
                (*(__IO uint8_t *) (buf + i + 5)),
                (*(__IO uint8_t *) (buf + i + 6)),
                (*(__IO uint8_t *) (buf + i + 7))
                );
            rt_kprintf("%02x  %02x  %02x  %02x  %02x  %02x  %02x  %02x",
                (*(__IO uint8_t *) (buf + i + 8)),
                (*(__IO uint8_t *) (buf + i + 9)),
                (*(__IO uint8_t *) (buf + i + 10)),
                (*(__IO uint8_t *) (buf + i + 11)),
                (*(__IO uint8_t *) (buf + i + 12)),
                (*(__IO uint8_t *) (buf + i + 13)),
                (*(__IO uint8_t *) (buf + i + 14)),
                (*(__IO uint8_t *) (buf + i + 15))
                );
            rt_kprintf("\r\n");
        }

        rt_free(buf);
        /* Close file */
        f_close(&fp);
    }
}


void updata_start(void)
{
    fatServ_control.serviceSt = 0x11;
}

void updata_end(void)
{
    fatServ_control.serviceSt = FATSERV_ST_NORMAL;
}


#include "finsh.h"
FINSH_FUNCTION_EXPORT(tc_evt, test case00 event log write)
FINSH_FUNCTION_EXPORT(tc_evt1, test case01 event log write)
FINSH_FUNCTION_EXPORT(tc_evt2, test case02 event log write 64M)
FINSH_FUNCTION_EXPORT(tc_log1, test case03 320KB log write)
FINSH_FUNCTION_EXPORT(tc_rd01, test case04 readEvtFile)
FINSH_FUNCTION_EXPORT(updata_start,start updata application)
FINSH_FUNCTION_EXPORT(updata_end,end updata application)


