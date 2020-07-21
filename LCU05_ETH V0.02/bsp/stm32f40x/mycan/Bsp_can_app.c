
#include "Bsp_can_app.h"
#include "common.h"
#include "Bsp_can_QueApi.h"
//#include "Bsp_led.h"
#include "EventRecordService.h"
#include "HwWDog_Max706.h"


/* Sniffer dataset */
typedef struct 
{
    uint32_t  canId;
    uint8_t   canData[8];
    uint8_t   canPort;
    uint8_t   DLC;
    uint8_t   res1;
    uint8_t   res2;
}CanSniffNode_TypeDef;


static CanSniffNode_TypeDef canSnSaveFrame = {0};


static void CanSlave_SniffSave(CAN_RX_DATA_RAM *rBuff)
{
    /* 送交日志记录处理 */
    canSnSaveFrame.canId = rBuff->rxMsg.ExtId;
    canSnSaveFrame.canPort = rBuff->parent.CanPort;
    canSnSaveFrame.DLC = rBuff->rxMsg.DLC;
    canSnSaveFrame.canData[0] = rBuff->rxMsg.Data[0];
    canSnSaveFrame.canData[1] = rBuff->rxMsg.Data[1];
    canSnSaveFrame.canData[2] = rBuff->rxMsg.Data[2];
    canSnSaveFrame.canData[3] = rBuff->rxMsg.Data[3];
    canSnSaveFrame.canData[4] = rBuff->rxMsg.Data[4];
    canSnSaveFrame.canData[5] = rBuff->rxMsg.Data[5];
    canSnSaveFrame.canData[6] = rBuff->rxMsg.Data[6];
    canSnSaveFrame.canData[7] = rBuff->rxMsg.Data[7];

    ERSRV_Write(&canSnSaveFrame, sizeof(CanSniffNode_TypeDef), ERD_DATATYPE_ETHIO);
}


#include <time.h>
extern void set_date(rt_uint32_t year, rt_uint32_t month, rt_uint32_t day);
extern void set_time(rt_uint32_t hour, rt_uint32_t minute, rt_uint32_t second);

static rt_err_t isRtc_Valid(uint8_t year, uint8_t mon,uint8_t day, uint8_t hour,uint8_t min, uint8_t sec)
{
    if (year < 20 || year > 99 || mon == 0 || mon > 12 || day == 0 || day > 31
        || hour > 23 || min > 59 || sec > 59)
    {
        return RT_ERROR;
    }
    else
    {
        return RT_EOK;
    }
}


static void setRtc(uint8_t year, uint8_t mon,uint8_t day, uint8_t hour,uint8_t min, uint8_t sec)
{
    time_t now;
    struct tm* ti = RT_NULL;

    /* get current time */
    time(&now);
    ti = localtime(&now);
    ti->tm_year = (ti->tm_year + 1900) % 100;
    ti->tm_mon = ti->tm_mon + 1;
    
    if (ti->tm_year != year || ti->tm_mon != mon || ti->tm_mday != day || ti->tm_hour != hour || ti->tm_min != min)
    {
        set_date((rt_uint32_t)(year + 2000), (rt_uint32_t)mon, (rt_uint32_t)day);
        set_time((rt_uint32_t)hour, (rt_uint32_t)min, (rt_uint32_t)sec);
    }
}


static void can_mvb_rtc_handle(CanRxMsg* pMsg)
{
    CAN_EXTID_INFO info = {0};
    
    if(pMsg == RT_NULL)
    {
        return;
    }

    info.value = pMsg->ExtId;
    
    if(info.id.src == SLOT_ID_CAN || info.id.src == SLOT_ID_MVB)
    {
        if(info.id.dst == CAN_ADDR_BROADCAST)
        {
            if(RT_EOK == isRtc_Valid(pMsg->Data[2], pMsg->Data[3], pMsg->Data[4], pMsg->Data[5], pMsg->Data[6], pMsg->Data[7]))
            {
                setRtc(pMsg->Data[2], pMsg->Data[3], pMsg->Data[4], pMsg->Data[5], pMsg->Data[6], pMsg->Data[7]);
            }
        }
    }
}


void Can_Rx_Data_Process(CAN_RX_DATA_RAM* p)
{
    CAN_EXTID_INFO info = { 0 };

    info.value = p->rxMsg.ExtId;

    switch(info.id.funID)
    {
        case 5:
            can_mvb_rtc_handle(&p->rxMsg);
            break;

        case 3:
            
        
        
            break;

        default:
            
            break;
    }

    /* 送交日志记录处理 */
    CanSlave_SniffSave(p);
}


static void Can_Send_Lifesign(void)
{
    CanTxMsg txMsg;
    CAN_EXTID_INFO info;
    static uint8_t lifesign = 0;
    
    info.id.funID = CAN_FUN_POWER_ON;
    info.id.src = ds.slotID;
    info.id.dst = CAN_ADDR_BROADCAST;
    info.id.pri = CAN_PRI_L;
    info.id.res = 0;

    txMsg.ExtId = info.value;
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 3;
    
    txMsg.Data[0] = lifesign++;
    txMsg.Data[1] = ds.version;
    txMsg.Data[2] = ds.carID;
    
    Can1Q_Push_Tx_Msg(1,ds.boardType,rt_tick_get(),&txMsg);
    Can2Q_Push_Tx_Msg(2,ds.boardType,rt_tick_get(),&txMsg);
}


void Can_Cycle_Service(void)
{
    Can_Send_Lifesign();
}




