

#include "userApp.h"
#include "Bsp_canApp.h"
#include "Bsp_canPrint.h"
#include "Can_fault_detect.h"
#include "Bsp_mvb.h"


#define CAN_MANAGE_PWRON            (0)
#define CAN_MANAGE_SELF_CHECK       (1)
#define CAN_MANAGE_SCAN             (2)
#define CAN_MANAGE_REMOTEIN         (3)


CanRx_hook canRxHandle[256] = {RT_NULL};

static uint32_t can_manage_sts = CAN_MANAGE_PWRON;


static void can_input_handle(CanRxMsg* pMsg)
{
    CAN_EXTID_INFO info = {0};

    info.value = pMsg->ExtId;

    if(info.id.dst == CAN_ADDR_MCU_GROUP && pMsg->DLC == 2)
    {
        if(info.id.src >= SLOT_ID_IO_MIN && info.id.src <= SLOT_ID_IO_MAX)
        {
            rt_memcpy(ds.DIO[info.id.src - SLOT_ID_IO_MIN].in, pMsg->Data, pMsg->DLC);
        }
    }
}


static void can_output_handle(CanRxMsg* pMsg)
{
    CAN_EXTID_INFO info = {0};

    info.value = pMsg->ExtId;
    
    if(info.id.dst == CAN_ADDR_BROADCAST && pMsg->DLC == 8)
    {
        switch(info.id.src)
        {
            case SLOT_ID_MCU_A:
                rt_memcpy(ds.MCU[0].ou, pMsg->Data, pMsg->DLC);
                break;

            case SLOT_ID_MCU_B:
                rt_memcpy(ds.MCU[1].ou, pMsg->Data, pMsg->DLC);
                break;

            case SLOT_ID_MCU_C:
                rt_memcpy(ds.MCU[2].ou, pMsg->Data, pMsg->DLC);
                break;

            default:
                break;

        }
    }
}


static void can_remoteIn_handle(CanRxMsg* pMsg)
{
    CAN_EXTID_INFO info = {0};

    info.value = pMsg->ExtId;
    
    if(info.id.dst == CAN_ADDR_BROADCAST && pMsg->DLC == 8)
    {
        switch(info.id.src)
        {
            case SLOT_ID_MCU_A:
                rt_memcpy(ds.MCU[0].remote_in, pMsg->Data, pMsg->DLC);
                break;

            case SLOT_ID_MCU_B:
                rt_memcpy(ds.MCU[1].remote_in, pMsg->Data, pMsg->DLC);
                break;

            case SLOT_ID_MCU_C:
                rt_memcpy(ds.MCU[2].remote_in, pMsg->Data, pMsg->DLC);
                break;

            default:
                break;

        }
        remoteIn_2oo3();
    }
}


static void can_io_sts_handle(CanRxMsg* pMsg)
{
    CAN_EXTID_INFO info = {0};

    info.value = pMsg->ExtId;

    if(info.id.dst == CAN_ADDR_MCU_GROUP && pMsg->DLC == 8)
    {
        if(info.id.src >= SLOT_ID_IO_MIN && info.id.src <= SLOT_ID_IO_MAX)
        {
            rt_memcpy(ds.DIO[info.id.src - SLOT_ID_IO_MIN].in, pMsg->Data, pMsg->DLC);
        }
    }
}


static void can_powerOn_handle(CanRxMsg* pMsg)
{
    CAN_EXTID_INFO info = {0};
    
    info.value = pMsg->ExtId;
    
    if(info.id.dst == CAN_ADDR_BROADCAST)
    {
        switch(info.id.src)
        {
            case SLOT_ID_MVB:
                if(pMsg->DLC == 3)
                {
                    ds.mvb_Lifesign = pMsg->Data[0];
                    ds.mvb_Version = pMsg->Data[1];
                    ds.mvb_CarID = pMsg->Data[2];
                }
                break;

            case SLOT_ID_CAN:
                if(info.id.port == 1 && pMsg->DLC == 4)
                {
                    ds.can1_Lifesign = pMsg->Data[0];
                    ds.can1_Version = pMsg->Data[1];
                    ds.can1_CarID = pMsg->Data[2];
                    ds.can1_ExtCan = pMsg->Data[3];
                }
                else if(info.id.port == 2 && pMsg->DLC == 4)
                {
                    ds.can2_Lifesign = pMsg->Data[0];
                    ds.can2_Version = pMsg->Data[1];
                    ds.can2_CarID = pMsg->Data[2];
                    ds.can2_ExtCan = pMsg->Data[3];
                }
                break;

            case SLOT_ID_ETU:
                if(pMsg->DLC == 3)
                {
                    ds.etu_Lifesign = pMsg->Data[0];
                    ds.etu_Version = pMsg->Data[1];
                    ds.etu_CarID = pMsg->Data[2];
                }
                break;

            case SLOT_ID_MCU_A:
                if(pMsg->DLC == 6)
                {
                    rt_memcpy(&ds.MCU[0].lifesign, pMsg->Data, pMsg->DLC);
                }
                break;

            case SLOT_ID_MCU_B:
                if(pMsg->DLC == 6)
                {
                    rt_memcpy(&ds.MCU[1].lifesign, pMsg->Data, pMsg->DLC);
                }
                break;

            case SLOT_ID_MCU_C:
                if(pMsg->DLC == 6)
                {
                    rt_memcpy(&ds.MCU[2].lifesign, pMsg->Data, pMsg->DLC);
                }
                break;

            default:
                if(info.id.src >= SLOT_ID_IO_MIN && info.id.src <= SLOT_ID_IO_MAX && pMsg->DLC == 2)
                {
                    ds.DIO[info.id.src - SLOT_ID_IO_MIN].lifesign = pMsg->Data[0];
                    ds.DIO[info.id.src - SLOT_ID_IO_MIN].version = pMsg->Data[1];
                }
                break;

        }
    }
}



static void can_lifesign_handle(CanRxMsg* pMsg)
{
    CAN_EXTID_INFO info = {0};
    BYTE_TO_BIT sts = { 0 };
    
    info.value = pMsg->ExtId;

    if(info.id.dst == CAN_ADDR_BROADCAST && pMsg->DLC == 2)
    {
        switch(info.id.src)
        {
            case SLOT_ID_MCU_A:
                ds.MCU[0].lifesign = pMsg->Data[0];
                sts.value = pMsg->Data[1];
                ds.MCU[0].dc110v = sts.Bits.bit0;
                ds.MCU[0].dc5v = sts.Bits.bit1;
                break;

            case SLOT_ID_MCU_B:
                ds.MCU[1].lifesign = pMsg->Data[0];
                sts.value = pMsg->Data[1];
                ds.MCU[1].dc110v = sts.Bits.bit0;
                ds.MCU[1].dc5v = sts.Bits.bit1;
                break;

            case SLOT_ID_MCU_C:
                ds.MCU[2].lifesign = pMsg->Data[0];
                sts.value = pMsg->Data[1];
                ds.MCU[2].dc110v = sts.Bits.bit0;
                ds.MCU[2].dc5v = sts.Bits.bit1;
                break;

            default:
                break;
        }
    }
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

    info.value = pMsg->ExtId;
    
    if(info.id.src == SLOT_ID_CAN || info.id.src == SLOT_ID_MVB)
    {
        if(info.id.dst == CAN_ADDR_BROADCAST)
        {
            pKW_SHM->lifeSign =(pMsg->Data[0] << 8) + pMsg->Data[1]; //�����źţ����ģʽ
            
            if(RT_EOK == isRtc_Valid(pMsg->Data[2], pMsg->Data[3], pMsg->Data[4], pMsg->Data[5], pMsg->Data[6], pMsg->Data[7]))
            {
                pKW_SHM->year = pMsg->Data[2];
                pKW_SHM->mon = pMsg->Data[3];
                pKW_SHM->day = pMsg->Data[4];
                pKW_SHM->hour = pMsg->Data[5];
                pKW_SHM->min = pMsg->Data[6];
                pKW_SHM->sec = pMsg->Data[7];
                
                setRtc(pKW_SHM->year, pKW_SHM->mon, pKW_SHM->day, pKW_SHM->hour, pKW_SHM->min, pKW_SHM->sec);
            }
        }
    }
}


//Զ�������źŴ洢
static void can_lcu_sts_handle(CanRxMsg* pMsg)
{
    CAN_EXTID_INFO info = { 0 };
    uint32_t offset = 0;
    uint32_t carID = 0;

    info.value = pMsg->ExtId;
    
    if(info.id.dst == CAN_ADDR_BROADCAST && info.id.src == SLOT_ID_CAN && pMsg->DLC == 8)
    {
        carID = (info.id.funID - 0x80) / 16;
        offset = info.id.funID % 16;

        ExtCanNode_Clear(info.id.port, carID);
        ExtCanNode_Clear(info.id.port, ds.carID);

        if(offset == 0)
        {
            rt_memcpy(&ds.inBuf[carID * 8 + 64], pMsg->Data, pMsg->DLC);
        }
        else if(offset < 5 && offset > 0)
        {
            carID = carID + 2;
            offset = (offset - 1) * 8;
            rt_memcpy(&pKW_SHM->mvb_port[carID].date[offset], pMsg->Data, pMsg->DLC);
        }
        else if(offset < 9 && offset > 4)
        {
            carID = carID + 6;
            offset = (offset - 5) * 8;
            rt_memcpy(&pKW_SHM->mvb_port[carID].date[offset], pMsg->Data, pMsg->DLC);
        }
        else if(offset == 9)
        {
            pKW_SHM->lifeSign =(pMsg->Data[0] << 8) + pMsg->Data[1]; //�����źţ����ģʽ
            
            if(RT_EOK == isRtc_Valid(pMsg->Data[2], pMsg->Data[3], pMsg->Data[4], pMsg->Data[5], pMsg->Data[6], pMsg->Data[7]))
            {
                pKW_SHM->year = pMsg->Data[2];
                pKW_SHM->mon = pMsg->Data[3];
                pKW_SHM->day = pMsg->Data[4];
                pKW_SHM->hour = pMsg->Data[5];
                pKW_SHM->min = pMsg->Data[6];
                pKW_SHM->sec = pMsg->Data[7];

                setRtc(pKW_SHM->year, pKW_SHM->mon, pKW_SHM->day, pKW_SHM->hour, pKW_SHM->min, pKW_SHM->sec);
            }
        }
    }
}


void can_rx_handle_register(void)
{
    uint32_t i;
    
    canRxHandle[CAN_FUN_INPUT] = can_input_handle;
    canRxHandle[CAN_FUN_OUTPUT] = can_output_handle;
    canRxHandle[CAN_FUN_REMOTE_IN] = can_remoteIn_handle;
    canRxHandle[CAN_FUN_MVB_RTC] = can_mvb_rtc_handle;
    canRxHandle[CAN_FUN_IO_STS] = can_io_sts_handle;
    canRxHandle[CAN_FUN_POWER_ON] = can_powerOn_handle;
    canRxHandle[CAN_FUN_LIFESIGN] = can_lifesign_handle;
    
    for(i = CAN_FUN_CAR1_DATA; i < 256; i++)
    {
        canRxHandle[i] = can_lcu_sts_handle;
    }
}


void can_rx_serve(CanRxMsg* pMsg)
{
    CAN_EXTID_INFO info = {0};
    
    info.value = pMsg->ExtId;
    
    CanNode_Clear(info.id.port, info.id.src);
    CanNode_Clear(info.id.port, ds.slotID);
    
    //������յ�Դ��ַ���Լ��ĵ�ַһ�£����¶�ȡslot_id
    if(info.id.src == ds.slotID)
    {
        userApp_init();
    }
    
    if(info.id.funID < 256)
    {
        //can�������ݴ���
        if(canRxHandle[info.id.funID] != RT_NULL)
        {
            canRxHandle[info.id.funID](pMsg);
        }
    }
}


/************************************************** CAN TX Serve **************************************************/


static uint32_t GetExtId(uint32_t funId, uint32_t pri)
{
    CAN_EXTID_INFO info = { 0 };

    info.id.funID = funId;
    info.id.src = ds.slotID;
    info.id.dst = CAN_ADDR_BROADCAST;
    info.id.pri = pri;
    info.id.port = 0;

    return info.value;
}


void can_send_output(void)
{
    CanTxMsg txMsg = { 0 };

    txMsg.ExtId = GetExtId(CAN_FUN_OUTPUT, CAN_PRI_H);
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 8;

    rt_memcpy(txMsg.Data, ds.ouBuf, txMsg.DLC);

    if(can_manage_sts == CAN_MANAGE_SCAN || can_manage_sts == CAN_MANAGE_REMOTEIN)
    {
        CANx_Send(&txMsg);
    }
}


static void can_send_PwrOn(void)
{
    CanTxMsg txMsg = { 0 };
    static uint8_t lifesign = 0;

    txMsg.ExtId = GetExtId(CAN_FUN_POWER_ON, CAN_PRI_L);
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 6;

    txMsg.Data[0] = lifesign++;         //�����ź�
    txMsg.Data[1] = MCU_VERSION;        //�ײ�����汾��
    txMsg.Data[2] = pKW_SHM->kwVer;     //KW����汾��
    txMsg.Data[3] = ds.carID;           //���ں�
    txMsg.Data[4] = ds.dc110v;          //״̬��Ϣ
    txMsg.Data[5] = ds.dc5v;            //״̬��Ϣ

    CANx_Send(&txMsg);
}


static void can_send_Lifesign(void)
{
    CanTxMsg txMsg = { 0 };
    BYTE_TO_BIT sts = { 0 };
    static uint8_t lifesign = 0;

    txMsg.ExtId = GetExtId(CAN_FUN_LIFESIGN, CAN_PRI_L);
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 2;

    sts.Bits.bit0 = ds.dc110v;
    sts.Bits.bit1 = ds.dc5v;
    sts.Bits.bit2 = ds.MCU[ds.slotID - SLOT_ID_MCU_A].flt.logic;

    txMsg.Data[0] = lifesign++;         //�����ź�
    txMsg.Data[1] = sts.value;          //״̬��Ϣ

    CANx_Send(&txMsg);
}


static void can_send_selfCheck(uint8_t index)
{
    CanTxMsg txMsg = { 0 };

    txMsg.ExtId = GetExtId(CAN_FUN_SELF_CHECK, CAN_PRI_H);
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 1;

    txMsg.Data[0] = index;

    CANx_Send(&txMsg);
}


void can_send_remoteIn(void)
{
    CanTxMsg txMsg = { 0 };

    txMsg.ExtId = GetExtId(CAN_FUN_REMOTE_IN, CAN_PRI_H);
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 8;

    rt_memcpy(txMsg.Data, &ds.ouBuf[64], txMsg.DLC);

    CANx_Send(&txMsg);
    CANx_Send(&txMsg);
}


static rt_err_t isSendValid(void)
{
    rt_err_t ret = RT_ERROR;

    switch(ds.slotID)
    {
        case SLOT_ID_MCU_A:
            ret = RT_EOK;
            break;

        case SLOT_ID_MCU_B:
            if(ds.MCU[0].flt.lost == RT_ERROR)
            {
                ret = RT_EOK;
            }
            break;

        case SLOT_ID_MCU_C:
            if(ds.MCU[0].flt.lost == RT_ERROR && ds.MCU[1].flt.lost == RT_ERROR)
            {
                ret = RT_EOK;
            }
            break;

        default:
            break;

    }

    return ret;
}


void ExtCan_send_remoteIn(void)
{
    CanTxMsg txMsg = { 0 };

    if(RT_ERROR == isSendValid())
    {
        return;
    }

    txMsg.ExtId = GetExtId( (ds.carID * 16 + 0x80), CAN_PRI_H);
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 8;

    rt_memcpy(txMsg.Data, ds.remoteIn, txMsg.DLC);

    CANx_Send(&txMsg);
    CANx_Send(&txMsg);
}


static void ExtCan_send_lcuFlt(void)
{
    CanTxMsg txMsg = { 0 };
    uint32_t i;

    if(RT_ERROR == isSendValid())
    {
        return;
    }

    for(i = 0; i < 4; i++)
    {
        txMsg.ExtId = GetExtId( (ds.carID * 16 + 0x81 + i), CAN_PRI_L);
        txMsg.IDE = CAN_ID_EXT;
        txMsg.RTR = 0;
        txMsg.DLC = 8;

        rt_memcpy(txMsg.Data, &pKW_SHM->mvb_port[0].date[i * 8], txMsg.DLC);
        CANx_Send(&txMsg);
        CANx_Send(&txMsg);
        rt_thread_delay(5);
    }
}


static void ExtCan_send_lcuSts(void)
{
    CanTxMsg txMsg = { 0 };
    uint32_t i;
    
    if(RT_ERROR == isSendValid())
    {
        return;
    }

    for(i = 0; i < 4; i++)
    {
        txMsg.ExtId = GetExtId( (ds.carID * 16 + 0x85 + i), CAN_PRI_L);
        txMsg.IDE = CAN_ID_EXT;
        txMsg.RTR = 0;
        txMsg.DLC = 8;
        
        rt_memcpy(txMsg.Data, &pKW_SHM->mvb_port[1].date[i * 8], txMsg.DLC);
        CANx_Send(&txMsg);
        CANx_Send(&txMsg);
        rt_thread_delay(5);
    }
}


void can_tx_serve(void)
{
    static uint8_t index = 0;

    rt_thread_delay(ds.slotID);

    //can���ϼ��
    CanNode_Update();

    switch(can_manage_sts)
    {
        case CAN_MANAGE_PWRON:
            can_send_PwrOn();
            if(ds.MCU[0].dc110v == 1 && ds.MCU[1].dc110v == 1 && ds.MCU[2].dc110v == 1)
            {
                can_manage_sts = CAN_MANAGE_SELF_CHECK;
            }
            else
            {
                if(rt_tick_get() > 2000) //���������ͨ���Լ��������ϵ糬��2sֱ�ӽ���ɨ��̬
                {
                    can_manage_sts = CAN_MANAGE_SCAN;
                }
            }

            break;

        case CAN_MANAGE_SELF_CHECK:
            
            if(ds.slotID == SLOT_ID_MCU_A)
            {
                can_send_selfCheck(index);
            }

            index++;

            if(index > 2)
            {
                can_manage_sts = CAN_MANAGE_SCAN;
            }

            break;

        case CAN_MANAGE_SCAN:

            can_send_output();
            rt_thread_delay(5);

            can_send_Lifesign();
            rt_thread_delay(5);

            ExtCan_send_lcuSts();

            can_manage_sts = CAN_MANAGE_REMOTEIN;
            break;

        case CAN_MANAGE_REMOTEIN:

            can_send_remoteIn();
            rt_thread_delay(5);

            ExtCan_send_remoteIn();
            rt_thread_delay(5);

            ExtCan_send_lcuFlt();

            can_manage_sts = CAN_MANAGE_SCAN;
            break;

        default:
            can_manage_sts = CAN_MANAGE_SCAN;
            break;

    }
}

