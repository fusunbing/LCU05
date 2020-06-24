/***************************************************************************

Copyright (C), 1999-2014, Tongye Tech. Co., Ltd.

* @file           EventRecordService.c
* @author          
* @version        V0.1.0
* @date           2014-06-24
* @brief          (1)  
(2) 
(3) 
History:          // Revision Records

<Author>             <time>       <version >            <desc>

life, lily          2014-06-24       V0.1.0             初次建立

***************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "rtthread.h"
#include "EventRecorder.h"
#include "EventRecordService.h"
#include "rtc.h"
#include <time.h>

#include "fat_service.h"


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define ERSRV_TASK_PRIORITY  (0x1B)

#define SINGLE_MQ_SIZE_MAX   (SRD_SIZE_MAX)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**************************************************************************/
// 静态创建线程堆栈
ALIGN(RT_ALIGN_SIZE)
static rt_uint32_t eventRecordService_stack[1 * 1024 / 4];
// 静态定义线程对象
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread ersr_thread;

// 单个消息缓冲
static __IO uint8_t msg_single[ERSRV_MESSAGE_MAX];
static Event_DatasetObjectTypeDef *pEds_obj;
/**************************************************************************/

static rt_mq_t pMq_Ersv;

//标准记录数据 缓冲区
static uint8_t standad_record_data[SRD_SIZE_MAX] = {0};
//接收消息队列数据的缓冲区
static uint8_t SingleMQRecvBuf[SINGLE_MQ_SIZE_MAX] = {0};


//////////////////////////////////////////////////////////////////////////
//函数声明区
//////////////////////////////////////////////////////////////////////////

/* Private function prototypes -----------------------------------------------*/
static void  ERSRV_Task(void* parameter);
static void  ERSRV_CreateThread(void);
static void  ERSRV_DataInit(void);
static int   ERSRV_Mq_Init(void);
static void  ERSRV_DataRepack(Event_ObjectBaseTypeDef *pEob, uint8_t *srd);
static void  ERSRV_GetRTC(struct tm *pDatetime);



/* Private functions ---------------------------------------------------------*/

//////////////////////////////////////////////////////////////////////////
/**
* @ 事件记录服务 :: 初始化
* @     消息队列初始化
* @     双缓冲器初始化
* @     服务线程初始化
*/
void  ERSRV_CreateApp(void)
{
    //双缓冲器初始化
    ERSRV_DataInit();
    //消息队列初始化
    ERSRV_Mq_Init();
    //服务线程初始化
    ERSRV_CreateThread();
}

/**
* @ 事件记录服务 :: Double Buffer Create.
*/
static void  ERSRV_DataInit(void)
{
    EventRecd_Init();
 
    pEds_obj = EventRecd_CreateObject(SRD_SIZE_MAX, SRD_COUNT_MAX);
}


/**
* @ 事件记录服务 :: 服务线程初始化，并启动运行
*/
static void  ERSRV_CreateThread(void)
{ 
    rt_thread_init(&ersr_thread,
        "ERSRV",
        ERSRV_Task,
        RT_NULL,
        &eventRecordService_stack[0], 
        sizeof(eventRecordService_stack),
        ERSRV_TASK_PRIORITY, 
        8
        );

    rt_thread_startup(&ersr_thread);
}

/**
* @ 事件记录服务 :: 服务主函数
*/
static void  ERSRV_Task(void* parameter)
{
    for (;;)
    {
        //Message Queue receive API
        rt_mq_recv(pMq_Ersv, (void*)SingleMQRecvBuf, SINGLE_MQ_SIZE_MAX, RT_WAITING_FOREVER);

        //do something : Data Repack.
        ERSRV_DataRepack((Event_ObjectBaseTypeDef *)SingleMQRecvBuf, standad_record_data);

        //Double Buffer Operator : Record one SRD data block.
        if (pEds_obj != RT_NULL)
        {
            //write to double buffer
            pEds_obj->Record(pEds_obj, (void*)standad_record_data);

            //if one buffer full, write to flash.
            if (pEds_obj->ctrlFlag.bits.canRead != 0)
            {
                pEds_obj->ctrlFlag.bits.canRead = 0;
							
                //call IF: SD_Write
                //FlashSrv_Write(pEds_obj->pBufRead, SRD_SIZE_MAX * SRD_COUNT_MAX, DS_EVENT_LOG);
                FatSrv_Write(pEds_obj->pBufRead, SRD_SIZE_MAX * SRD_COUNT_MAX, SPD_OPERTION_WREVT, RT_NULL);
            }
        }

    }
}


//////////////////////////////////////////////////////////////////////////
// 在110V掉电的工况下，紧急写入操作。
// 目的：当前的Buffer大小为4K， 可以缓存62个标准数据块，那么只有当62个
//       标准数据块都写入到缓冲区后，才会将其一起打包记录到Flash。
//       那么，当写入缓存还未记录满62个标准数据块时，发生掉电事件，我们
//       为了不丢失已经记录的数据，就利用这个紧急写入API，将当前缓冲区
//       的内容写入FLash。
//////////////////////////////////////////////////////////////////////////
void ERSRV_EmergencyWrite(void)
{
    if(pEds_obj->loopPoint != 0)
    {
        //////FlashSrv_Write(pEds_obj->pBufWrite, SRD_SIZE_MAX * SRD_COUNT_MAX, DS_EVENT_LOG);
        pEds_obj->loopPoint = 0;
        rt_memset(pEds_obj->pBufWrite, 0, (pEds_obj->cellSize * pEds_obj->loopSize));
    }
}


void ERSRV_EmergencyRecord(void *buffer, uint32_t buffsize, uint32_t type)
{
    struct tm ti;
    Event_ObjectBaseTypeDef* eob;
    uint8_t msg[SINGLE_MQ_SIZE_MAX];

    eob = (Event_ObjectBaseTypeDef*)msg;
    eob->tick = rt_tick_get();
    eob->type = type;
    eob->size = buffsize;
    // update datetime stamp.
    ERSRV_GetRTC(&ti);
    eob->year = ti.tm_year;
    eob->month = ti.tm_mon;
    eob->day = ti.tm_mday;
    eob->hour = ti.tm_hour;
    eob->min = ti.tm_min;
    eob->sec = ti.tm_sec;

    // update payload data.
    rt_memcpy(&msg[sizeof(Event_ObjectBaseTypeDef)], buffer, buffsize);

    //do something : Data Repack.
    //ERSRV_DataRepack(msg, standad_record_data);

    //Double Buffer Operator : Record one SRD data block.
    if (pEds_obj != RT_NULL)
    {
        //write to double buffer
        pEds_obj->Record(pEds_obj, (void*)&msg);
    }
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////


/**
* @ 事件记录服务 :: 创建消息队列
*/
static int ERSRV_Mq_Init(void)
{
    pMq_Ersv = rt_mq_create("ERSRV",            //消息队列的名称
                            SINGLE_MQ_SIZE_MAX, //消息队列中一条消息的最大长度
                            EVENTRECORD_QUE,    //消息队列的最大消息数量
                            RT_IPC_FLAG_FIFO    //IPC参数采用FIFO方式
                            );
    return 0;
}

/**
* @ 事件记录服务 :: 'Write' 接口
*/
int ERSRV_Write(void *buffer, uint32_t buffsize, uint32_t type)
{
    struct tm ti;
    Event_ObjectBaseTypeDef* eob;
    uint8_t msg[SINGLE_MQ_SIZE_MAX];

    eob = (Event_ObjectBaseTypeDef*)msg;
    eob->tick = rt_tick_get();
    eob->type = type;
    eob->size = buffsize;
    // update datetime stamp.
    ERSRV_GetRTC(&ti);
    eob->year  = ti.tm_year;
    eob->month = ti.tm_mon; 
    eob->day   = ti.tm_mday;
    eob->hour  = ti.tm_hour;
    eob->min   = ti.tm_min; 
    eob->sec   = ti.tm_sec; 

    // update payload data.
    rt_memcpy(&msg[sizeof(Event_ObjectBaseTypeDef)], buffer, buffsize);

    // send to message queue.
    return rt_mq_send(pMq_Ersv, msg, (eob->size + sizeof(Event_ObjectBaseTypeDef)));
}


/**
* @ 事件记录服务 :: 数据打包操作
*/
static void ERSRV_DataRepack(Event_ObjectBaseTypeDef *pEob, uint8_t *srd)
{
    //将标准数据记录单元 清空
    rt_memset((void*)srd, 0 , SRD_SIZE_MAX);
    //将从消息队列中获取的数据 按标准格式（长度）放入数据记录单元中
    rt_memcpy((void*)srd, 
        (const void*)pEob, 
        SRD_SIZE_MAX);
    //2019/6/27 王晓信修改 原本复制64字节数据修改为复制32字节数据
    //(pEob->size + sizeof(Event_ObjectBaseTypeDef)));
}

/**
* @ 事件记录服务 :: Obtain RTC Stamp.
* @ 年份 00 - 99
* @ 月份 01 - 12
*/
static void ERSRV_GetRTC(struct tm *pDatetime)
{
    time_t now;
    struct tm* ti;

    ti = RT_NULL;
    /* get current time */
    time(&now);

    ti = localtime(&now);

    /*"tm_mon"  - 年份中的第几个月，从 0 开始表示一月
      "tm_year" - 年份，从 1900 开始*/

    if (ti != RT_NULL)
    {
        pDatetime->tm_year  = (ti->tm_year + 1900) % 100;
        pDatetime->tm_mon   = ti->tm_mon + 1; 
        pDatetime->tm_mday  = ti->tm_mday;

        pDatetime->tm_hour  = ti->tm_hour;
        pDatetime->tm_min   = ti->tm_min;
        pDatetime->tm_sec   = ti->tm_sec;
    }
}


/**
* @}
*/
void ers_prt(void)
{
    rt_kprintf("\r\n  ===== EventRecordService DataObject information ===== \r\n");
    rt_kprintf("EDS->cellSize : %d \r\n", pEds_obj->cellSize);
    rt_kprintf("EDS->loopSize : %d \r\n", pEds_obj->loopSize);
    rt_kprintf("EDS->loopBuf1 : 0x%08X \r\n", (uint32_t)(pEds_obj->loopBuf1));
    rt_kprintf("EDS->loopBuf2 : 0x%08X \r\n", (uint32_t)(pEds_obj->loopBuf2));
    rt_kprintf("EDS->loopPoint : %d \r\n", pEds_obj->loopPoint);
    rt_kprintf("EDS->pBufRead : 0x%08X \r\n", (uint32_t)(pEds_obj->pBufRead));
    rt_kprintf("EDS->pBufWrite : 0x%08X \r\n", (uint32_t)(pEds_obj->pBufWrite));
    rt_kprintf("EDS->Record() : 0x%08X \r\n", (uint32_t)(pEds_obj->Record));
    
}

#include "finsh.h"
FINSH_FUNCTION_EXPORT(ers_prt, EventRecordService DataObject information)

/************************ (C) COPYRIGHT Tongye 2014 *****END OF FILE****/




