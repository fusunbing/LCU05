
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
    uint8_t   dlc;
    uint8_t   res1;
    uint8_t   res2;
}CanSniffNode_TypeDef;


static CanSniffNode_TypeDef canSnSaveFrame = {0};


static void CanSlave_SniffSave(CAN_RX_DATA_RAM *rBuff)
{
    /* 送交日志记录处理 */
    canSnSaveFrame.canId = rBuff->rxMsg.ExtId;
    canSnSaveFrame.canPort = rBuff->parent.CanPort;
    canSnSaveFrame.dlc = rBuff->rxMsg.DLC;
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


void Can_Rx_Data_Process(CAN_RX_DATA_RAM* pbuf)
{
    /* 送交日志记录处理 */
    CanSlave_SniffSave(pbuf);
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




