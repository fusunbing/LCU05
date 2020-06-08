
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


OUTPUT_RECEIVE_MSG receive_data[3] = {0};

static CAN_MSG_ID Receive_Id;
static uint8_t can_manage_sts = CAN_MANAGE_POWER_ON;



static void CanApp_ExID2MsgID(uint32_t _extid, PCAN_MSG_ID _id);
static uint32_t CanApp_MsgID2ExID(uint8_t pri, uint8_t dest, uint8_t source, uint16_t fun);
static void Can_Proto_Process(uint8_t netPort, uint8_t srcAddr, uint16_t funcId, uint8_t* pBuf, uint8_t dlc);
static void Output_Data_Process(uint8_t netPort, uint8_t srcAddr, uint16_t funcId, uint8_t* pBuf, uint8_t dlc);
static void Can_Send_Outfb_Data(void);

static CanSniffNode_TypeDef canSnSaveFrame = {0};

static void CanSlave_SniffSave(CAN_RX_DATA_RAM *rBuff)
{
    //static uint16_t pSave01 = 0;
    
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
	  static uint8_t IO_broadcast_addr;
    CAN_RX_DATA_RAM* _p = pbuf;
	
//	  IWDG_ReloadCounter(); /* 恢复内部独立看门狗 2019-4-17*/
//		HwWDog_Feed();

	  IO_broadcast_addr = ((ds.slotID -4) / 3) + MCU_BROADCAST_ADDR;
	  CanApp_ExID2MsgID(_p->rxMsg.ExtId, &Receive_Id);
	
//	  SlotID_check(Receive_Id.Source_Addr);

    if(Receive_Id.Source_Addr>=4 && Receive_Id.Source_Addr<=6)
		{
				/* 送交日志记录处理 */
				CanSlave_SniffSave(_p);
		}
}


static void Can_Proto_Process(uint8_t netPort, uint8_t srcAddr, uint16_t funcId, uint8_t* pBuf, uint8_t dlc)
{
	  uint8_t process_mode = 0;
    if(0x02 == funcId)
		{
		    process_mode = 1;
		}
		else if(0x05 == funcId)
		{
		    process_mode = 2;
		}
		
		switch(process_mode)
		{
			case 1:
				Output_Data_Process(netPort, srcAddr, funcId, pBuf, dlc);
			  break;
			case 2:

			default:
				break;
		}
}


static void Fault_Data_Process(uint8_t netPort, uint8_t srcAddr, uint16_t funcId, uint8_t* pBuf, uint8_t dlc)
{
    if(srcAddr >= 4 && srcAddr <= 6)
		{
//        led_flt_flag = 1;
		}
}


static void Output_Data_Process(uint8_t netPort, uint8_t srcAddr, uint16_t funcId, uint8_t* pBuf, uint8_t dlc)
{
    if(srcAddr >= 4 && srcAddr <= 6)
		{
			  ds.MCU_online_cnt[srcAddr-4] = 0;
		    receive_data[srcAddr-4].output = pBuf[0];
		}
}
	

static void CanApp_ExID2MsgID(uint32_t _extid, PCAN_MSG_ID _id)
{
    _id->Function      = _extid & 0x1FF;
    _id->Source_Addr   = (_extid >>  9) & 0xFF;
    _id->Target_Addr   = (_extid >> 17) & 0xFF;
    _id->Priority      = (_extid >> 25) & 0x0F;
}


static uint32_t CanApp_MsgID2ExID(uint8_t pri, uint8_t dest, uint8_t source, uint16_t fun)
{

    uint32_t extid;

    extid = (pri << 25 ) |
        (dest << 17) |
        (source << 9) |
        fun;

    return (uint32_t)extid;
}


void Can_TXData_Fill(uint8_t pri, uint8_t dest, uint8_t source, uint16_t fun, uint8_t* data, uint8_t len, CanTxMsg* msg)
{
	  uint32_t extid;
	
    if (len >= 8)
    {
        len = 8;
    }
		
		extid = CanApp_MsgID2ExID(pri, dest, source, fun);
		
		msg->ExtId = extid;
		msg->IDE = CAN_ID_EXT;
		msg->RTR = 0;
		msg->DLC = len;
		
		msg->Data[0] = data[0];
		msg->Data[1] = data[1];
		msg->Data[2] = data[2];
		msg->Data[3] = data[3];
		msg->Data[4] = data[4];
		msg->Data[5] = data[5];
		msg->Data[6] = data[6];
		msg->Data[7] = data[7];
}

void Power_on_Request(void)
{
    static uint8_t power_on_flag = 1;
	  uint8_t buffer[8] = {1};
		CanTxMsg power_on_buf;
	
	  if(1 == power_on_flag)
		{
		    power_on_flag = 0;
			  Can_TXData_Fill(0x01, MCU_BROADCAST_ADDR, ds.slotID, POWER_ON_CMD, buffer, 1, &power_on_buf);
				Can1Q_Push_Tx_Msg(1,ds.slotID,rt_tick_get(),&power_on_buf);
			  Can2Q_Push_Tx_Msg(2,ds.slotID,rt_tick_get(),&power_on_buf);
		}

}


void SlotID_check(uint8_t rcv_sourceid)
{
    if(ds.slotID == rcv_sourceid)
		{
		    __set_FAULTMASK(1);
        NVIC_SystemReset();
		}
}


void Can_Cycle_Service(void)
{
	  ds.MCU_online_cnt[MCU_BOARD_A]++;
		ds.MCU_online_cnt[MCU_BOARD_B]++;
		ds.MCU_online_cnt[MCU_BOARD_C]++;
	  switch(can_manage_sts)
		{
			case CAN_MANAGE_POWER_ON:
				can_manage_sts = CAN_MANAGE_SCAN;
				Power_on_Request();
			  break;
			case CAN_MANAGE_SCAN:
//				Can_Send_Outfb_Data();
			  break;
			default:
				break;
		}
}


static void Can_Send_Outfb_Data(void)
{
    CanTxMsg txMsg;
    CAN_EXTID_INFO info;
    
    info.id.funID = 0x01;
    info.id.src = ds.slotID;
    info.id.dst = MCU_BROADCAST_ADDR;
    info.id.pri = CAN_PRI_M;
    info.id.res = 0;

    txMsg.ExtId = info.value;
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 5;
    
    txMsg.Data[0] = ds.inputFilter & 0xFF;
	  txMsg.Data[1] = (ds.inputFilter >> 8) & 0xF;
	  txMsg.Data[2] = ds.output & 0x7F;
	  txMsg.Data[3] = ds.outfbFilter & 0xFF;
	  txMsg.Data[4] = (ds.outfbFilter >> 8) & 0x7F;

	  Can1Q_Push_Tx_Msg(1,ds.boardType,rt_tick_get(),&txMsg);
		Can2Q_Push_Tx_Msg(2,ds.boardType,rt_tick_get(),&txMsg);
}


