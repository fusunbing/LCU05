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

life, lily          2014-06-24       V0.1.0             ���ν���

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
// ��̬�����̶߳�ջ
ALIGN(RT_ALIGN_SIZE)
static rt_uint32_t eventRecordService_stack[1 * 1024 / 4];
// ��̬�����̶߳���
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread ersr_thread;

// ������Ϣ����
static __IO uint8_t msg_single[ERSRV_MESSAGE_MAX];
static Event_DatasetObjectTypeDef *pEds_obj;
/**************************************************************************/

static rt_mq_t pMq_Ersv;

//��׼��¼���� ������
static uint8_t standad_record_data[SRD_SIZE_MAX] = {0};
//������Ϣ�������ݵĻ�����
static uint8_t SingleMQRecvBuf[SINGLE_MQ_SIZE_MAX] = {0};


//////////////////////////////////////////////////////////////////////////
//����������
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
* @ �¼���¼���� :: ��ʼ��
* @     ��Ϣ���г�ʼ��
* @     ˫��������ʼ��
* @     �����̳߳�ʼ��
*/
void  ERSRV_CreateApp(void)
{
    //˫��������ʼ��
    ERSRV_DataInit();
    //��Ϣ���г�ʼ��
    ERSRV_Mq_Init();
    //�����̳߳�ʼ��
    ERSRV_CreateThread();
}

/**
* @ �¼���¼���� :: Double Buffer Create.
*/
static void  ERSRV_DataInit(void)
{
    EventRecd_Init();
 
    pEds_obj = EventRecd_CreateObject(SRD_SIZE_MAX, SRD_COUNT_MAX);
}


/**
* @ �¼���¼���� :: �����̳߳�ʼ��������������
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
* @ �¼���¼���� :: ����������
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
// ��110V����Ĺ����£�����д�������
// Ŀ�ģ���ǰ��Buffer��СΪ4K�� ���Ի���62����׼���ݿ飬��ôֻ�е�62��
//       ��׼���ݿ鶼д�뵽�������󣬲ŻὫ��һ������¼��Flash��
//       ��ô����д�뻺�滹δ��¼��62����׼���ݿ�ʱ�����������¼�������
//       Ϊ�˲���ʧ�Ѿ���¼�����ݣ��������������д��API������ǰ������
//       ������д��FLash��
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
* @ �¼���¼���� :: ������Ϣ����
*/
static int ERSRV_Mq_Init(void)
{
    pMq_Ersv = rt_mq_create("ERSRV",            //��Ϣ���е�����
                            SINGLE_MQ_SIZE_MAX, //��Ϣ������һ����Ϣ����󳤶�
                            EVENTRECORD_QUE,    //��Ϣ���е������Ϣ����
                            RT_IPC_FLAG_FIFO    //IPC��������FIFO��ʽ
                            );
    return 0;
}

/**
* @ �¼���¼���� :: 'Write' �ӿ�
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
* @ �¼���¼���� :: ���ݴ������
*/
static void ERSRV_DataRepack(Event_ObjectBaseTypeDef *pEob, uint8_t *srd)
{
    //����׼���ݼ�¼��Ԫ ���
    rt_memset((void*)srd, 0 , SRD_SIZE_MAX);
    //������Ϣ�����л�ȡ������ ����׼��ʽ�����ȣ��������ݼ�¼��Ԫ��
    rt_memcpy((void*)srd, 
        (const void*)pEob, 
        SRD_SIZE_MAX);
    //2019/6/27 �������޸� ԭ������64�ֽ������޸�Ϊ����32�ֽ�����
    //(pEob->size + sizeof(Event_ObjectBaseTypeDef)));
}

/**
* @ �¼���¼���� :: Obtain RTC Stamp.
* @ ��� 00 - 99
* @ �·� 01 - 12
*/
static void ERSRV_GetRTC(struct tm *pDatetime)
{
    time_t now;
    struct tm* ti;

    ti = RT_NULL;
    /* get current time */
    time(&now);

    ti = localtime(&now);

    /*"tm_mon"  - ����еĵڼ����£��� 0 ��ʼ��ʾһ��
      "tm_year" - ��ݣ��� 1900 ��ʼ*/

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




