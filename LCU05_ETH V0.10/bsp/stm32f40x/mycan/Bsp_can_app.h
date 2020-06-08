
#ifndef __BSP_CAN_APP_H__
#define __BSP_CAN_APP_H__

#include "stm32f4xx.h"
#include "Bsp_can_queue.h"


#define CAN_MANAGE_POWER_ON             (0)
#define CAN_MANAGE_SCAN                 (1)


typedef struct _CAN_ID_MSG_ 
{
	uint32_t  Function      : 9;        //功能号
	uint32_t  Source_Addr   : 8;		//源地址
	uint32_t  Target_Addr   : 8;		//目标地址
	uint32_t  Priority      : 4;		//优先级
	uint32_t  res           : 3;
}CAN_MSG_ID, *PCAN_MSG_ID;

typedef struct output_receive_msg
{
    uint8_t output;
}OUTPUT_RECEIVE_MSG;


void Can_Rx_Data_Process(CAN_RX_DATA_RAM* pbuf);
void Can_TXData_Fill(uint8_t pri, uint8_t dest, uint8_t source, uint16_t fun, uint8_t* data, uint8_t len, CanTxMsg* msg);
void Power_on_Request(void);
void SlotID_check(uint8_t rcv_sourceid);
void Can_Cycle_Service(void);

#endif



