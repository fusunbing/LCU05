
#include "userApp.h"
#include "Bsp_canApp.h"
#include "Bsp_canPrint.h"
#include "crc16.h"
#include "MVB_Cfg.h"
#include "mvb_event.h"

#define LCU_FAULT_PORT      (0)
#define LCU_IO_STATUS_PORT  (1)

#define LCU_LIFESIGAL_PORT  (6)

//MVB_SEND_DATA_BUF  Mvb_Send_Data[MVB_PORT_ADDR_NUM];
//MVB_SEND_DATA_BUF  Mvb_Recv_Data[MVB_PORT_ADDR_NUM];

static uint8_t can_manage_sts = CAN_MANAGE_POWER_ON;

static CAN_NODE_FAULT canNodesFault[CAN_NODES_CNT_MAX] = { 0 };


static void can_PowerOn(void);
static void MVB_PowerOn(void);
static void can_send_flt(uint8_t slotID, uint8_t date);


static uint8_t matrix_in[9][3] = { 0 };
static uint8_t CAN_DATA[66] = { 0 };

//static uint8_t CAN_DATA[66] = {0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x20,
//							   0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x30,
//							   0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x40};


static void CanFlt_Node_FaultClear(uint8_t port, uint8_t src)
{
    uint8_t canNodeIndex = 0;
    
    canNodeIndex = src - 1;
    
    if(canNodeIndex < CAN_NODES_CNT_MAX)
    {
        //收到CAN数据，清除故障标志位
        switch(port)
        {
            case 1:
                canNodesFault[canNodeIndex].cnt1 = 0;
                break;
            case 2:
                canNodesFault[canNodeIndex].cnt2 = 0;
                break;
            default:
                rt_kprintf("+can port err! \r\n");
                break;
        }
    }
    else
    {
        rt_kprintf("+can receive slot ID err! \r\n");
    }
}




static void can_rx_handle(CAN_RX_DATA_RAM * pDate)
{
    CAN_EXTID_INFO info = {0};
    uint8_t index = 0;
    uint8_t group = 0;
	uint16_t crc = 0;
	uint8_t CRCL = 0;
	uint8_t CRCH = 0;

    info.value = pDate->rxMsg.ExtId;
    
    //清空can故障标志
    CanFlt_Node_FaultClear(pDate->canPort, info.id.src);

    //根据功能号进行处理
    if(info.id.dst == SLOT_ID_MVB_)
    {
        switch(info.id.funID)
        {
            case CAN_FUN_MCU_DATA:
                if(info.id.src >= 1 && info.id.src <= 5)
                {
                    
					rt_memcpy(CAN_DATA,&pDate->rxMsg.Data[0],8);
					rt_memcpy(Mvb_Send_Data[LCU_LIFESIGAL_PORT].data,CAN_DATA,8);
					Mvb_Send_Data[LCU_LIFESIGAL_PORT].fresh_data = 1;
					MVB_Send_Write();
//                    switch(pDate->rxMsg.Data[0])
//					{
//						case  1:
//							rt_memcpy(CAN_DATA,&pDate->rxMsg.Data[1],7);
//							break;
//						case  2:
//							rt_memcpy(&CAN_DATA[7],&pDate->rxMsg.Data[1],7);
//							break;
//						case  3:
//							rt_memcpy(&CAN_DATA[14],&pDate->rxMsg.Data[1],7);
//							break;
//						case  4:
//							rt_memcpy(&CAN_DATA[21],&pDate->rxMsg.Data[1],7);
//							break;
//						case  5:
//							rt_memcpy(&CAN_DATA[28],&pDate->rxMsg.Data[1],7);
//							break;
//						case  6:
//							rt_memcpy(&CAN_DATA[35],&pDate->rxMsg.Data[1],7);
//							break;
//						case  7:
//							rt_memcpy(&CAN_DATA[42],&pDate->rxMsg.Data[1],7);
//							break;
//						case  8:
//							rt_memcpy(&CAN_DATA[49],&pDate->rxMsg.Data[1],7);
//							break;
//						case  9:
//							rt_memcpy(&CAN_DATA[56],&pDate->rxMsg.Data[1],7);
//							break;
//						case  255:
//							rt_memcpy(&CAN_DATA[63],&pDate->rxMsg.Data[1],3);
//							break;
//						default:
//							break;
//						
//					}
//					crc = getCRC16(CAN_DATA,64);
//					CRCL = crc & 0xFF;
//					CRCH = (crc >> 8) & 0xFF;
//					if(CRCL == CAN_DATA[64] && CRCH == CAN_DATA[65])
//					{
//						rt_memcpy(Mvb_Send_Data[LCU_FAULT_PORT].data,CAN_DATA,32);
//						rt_memcpy(Mvb_Send_Data[LCU_IO_STATUS_PORT].data,&CAN_DATA[32],32);
//						Mvb_Send_Data[LCU_FAULT_PORT].fresh_data = 1;
//						Mvb_Send_Data[LCU_IO_STATUS_PORT].fresh_data = 1;
//						MVB_Send_Write();
//					}

                }
                break;
                
 //           case CAN_FUN_ST_DATA:

 //               break;
            
            default:
                
                break;
        }
    }
    
}


void can_rx_serve()
{   
    uint32_t can_rxBuf;
    CAN_RX_DATA_RAM * pCan_rxBuf;

    //从队列弹出接收数据帧地址
    can_rxBuf = CanQ_Pop_Rx_Msg();
    
    //弹出指针不为空, 则表示队列操作成功
    if (can_rxBuf != CAN_QUEUE_NULL)
    {
        //转换指针类型: RAM操作数据块
        pCan_rxBuf = (CAN_RX_DATA_RAM *)can_rxBuf; 

        //can接收数据处理
        can_rx_handle(pCan_rxBuf);        
        
        //can接收数据打印
        Can_RX_Print(pCan_rxBuf);
    }
}


void can1_tx_serve()
{
    uint8_t sendFault = 0;
    uint8_t mailFull = 0;
    uint8_t tryTimes = CAN_SEND_TRYTIMES;
    uint32_t Can1_bus_fault = 0;
    uint32_t can1_txBuf;
    CAN_TX_DATA_RAM * pCan1_txBuf;

    can1_txBuf = Can1Q_Pop_Tx_Msg();
    
    if (can1_txBuf != CAN_QUEUE_NULL)
    {
        if (Can1_bus_fault != 0)
        {
            if (CAN_GetFlagStatus(CAN1, CAN_FLAG_BOF) == SET)
            {
                return;
            }
            else
            {
                Can1_bus_fault = 0;
            }
        }
        
        pCan1_txBuf = (CAN_TX_DATA_RAM * )can1_txBuf;
        
        do
        {
            mailFull = 0;

            if (CAN_Transmit(CAN1, &(pCan1_txBuf->txMsg)) == CAN_TxStatus_NoMailBox)
            {
                mailFull = 1;
                rt_thread_delay(1);                    
                
                if (tryTimes-- == 0)
                {
                    mailFull = 0;
                    sendFault = 1;
                    rt_thread_delay(1);
                    
                    if (CAN_GetFlagStatus(CAN1, CAN_FLAG_BOF) == SET)
                    {
                        Can1_bus_fault = 1;
                    }
                }
            }

        } while (mailFull);
        
        if (sendFault || Can1_bus_fault)
        {
            rt_kprintf("+ can1 send fault\r\n");
        }
        
        Can_TX_Print(pCan1_txBuf);
    } 
}


void can2_tx_serve()
{
    uint8_t sendFault = 0;
    uint8_t mailFull = 0;
    uint8_t tryTimes = CAN_SEND_TRYTIMES;
    uint32_t Can2_bus_fault = 0;
    uint32_t can2_txBuf;
    CAN_TX_DATA_RAM * pCan2_txBuf;

    can2_txBuf = Can2Q_Pop_Tx_Msg();

    if (can2_txBuf != CAN_QUEUE_NULL)
    {
        if (Can2_bus_fault != 0)
        {
            if (CAN_GetFlagStatus(CAN2, CAN_FLAG_BOF) == SET)
            {
                return;
            }
            else
            {
                Can2_bus_fault = 0;
            }
        }
        
        pCan2_txBuf = (CAN_TX_DATA_RAM*)can2_txBuf;
        
        do
        {
            mailFull = 0;

            if (CAN_Transmit(CAN2, &(pCan2_txBuf->txMsg)) == CAN_TxStatus_NoMailBox)
            {
                mailFull = 1;
                rt_thread_delay(1);
                
                if (tryTimes-- == 0)
                {
                    mailFull = 0;
                    sendFault = 1;
                    rt_thread_delay(1);
                    
                    if (CAN_GetFlagStatus(CAN2, CAN_FLAG_BOF) == SET)
                    {
                        Can2_bus_fault = 1;
                    }
                }
            }

        } while (mailFull);
        
        if (sendFault || Can2_bus_fault)
        {
            rt_kprintf("+ can2 send fault\r\n");
        }
        
        Can_TX_Print(pCan2_txBuf);
    }
}


//static void CanFlt_Node_FaultUpdate(void)
//{
//    uint8_t i;

//    for(i = 0; i < pKW_SHM->ds.canNodesCnt; i++)
//    {
//        if(pKW_SHM->ds.canNodes[i].isValid == 1)
//        {
//            canNodesFault[i].cnt1++;
//            canNodesFault[i].cnt2++;

//            //can1故障判断
//            if(canNodesFault[i].cnt1 >= CAN_FLT_TIMEOUT_CNT)
//            {
//                pKW_SHM->ds.canNodes[i].can1_fault = 1;	
//                canNodesFault[i].cnt1 = CAN_FLT_TIMEOUT_CNT;
//            }
//            else
//            {
//                pKW_SHM->ds.canNodes[i].can1_fault = 0;	
//            }
//                    
//            //can2故障判断
//            if(canNodesFault[i].cnt2 >= CAN_FLT_TIMEOUT_CNT)
//            {
//                pKW_SHM->ds.canNodes[i].can2_fault = 1;
//                canNodesFault[i].cnt2 = CAN_FLT_TIMEOUT_CNT;
//            }
//            else
//            {
//                pKW_SHM->ds.canNodes[i].can2_fault = 0;	
//            }
//            
//            //是否在线判断
//            pKW_SHM->ds.canNodes[i].isLost = (pKW_SHM->ds.canNodes[i].can1_fault == 1 && pKW_SHM->ds.canNodes[i].can2_fault == 1) ? 1 : 0;
//            
//            if(pKW_SHM->ds.canNodes[i].can1_fault != 0 || pKW_SHM->ds.canNodes[i].can2_fault != 0 )
//            {
//                System_Led_SetMode(LED_FLT, MODE_FLASH_SLOW);
//                rt_kprintf("+ can node NO.%d err,slot ID = %d\r\n", i, pKW_SHM->ds.slotID);
//            }
//        }
//    }
//}


static uint8_t IO_GROUP_INDEX = 0;
void canApp_serve(void)
{
//	CanFlt_Node_FaultUpdate();
    
    switch(can_manage_sts)
    {
        case CAN_MANAGE_POWER_ON:
            MVB_PowerOn();
            can_manage_sts = CAN_MANAGE_SCAN;
            break;
        case CAN_MANAGE_SCAN:
            
//            can_send_output(IO_GROUP_INDEX, pKW_SHM->ds.ouBuf[IO_GROUP_INDEX]);
//        
//            IO_GROUP_INDEX++;
//            if(IO_GROUP_INDEX > (pKW_SHM->ds.canNodesCnt-7)/3)
//            {
//                IO_GROUP_INDEX = 0;
//            }
            
            break;
        default:
            can_manage_sts = CAN_MANAGE_SCAN;
            break;
    }
}


static void can_send_flt(uint8_t slotID, uint8_t date)
{
    CanTxMsg txMsg;
    CAN_EXTID_INFO info;
    
    info.id.funID = CAN_FUN_DIO_FLT;
//    info.id.src = pKW_SHM->ds.slotID;
    info.id.dst = CAN_ADDR_BROADCAST;
    info.id.pri = CAN_PRI_M;
    info.id.res = 0;

    txMsg.ExtId = info.value;
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 1;
    
    txMsg.Data[0] = date;

    Can1Q_Push_Tx_Msg(1, rt_tick_get(), &txMsg);
    Can2Q_Push_Tx_Msg(2, rt_tick_get(), &txMsg);
}


void can_send_output(uint8_t index, uint8_t date)
{
    CanTxMsg txMsg;
    CAN_EXTID_INFO info;
    
    info.id.funID = CAN_FUN_OUTPUT;
//    info.id.src = pKW_SHM->ds.slotID;
    info.id.dst = (index + CAN_ADDR_DIO1_GROUP);
    info.id.pri = CAN_PRI_M;
    info.id.res = 0;

    txMsg.ExtId = info.value;
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 1;
    
    txMsg.Data[0] = date;

    Can1Q_Push_Tx_Msg(1, rt_tick_get(), &txMsg);
    Can2Q_Push_Tx_Msg(2, rt_tick_get(), &txMsg);
}


static void can_PowerOn(void)
{
    CanTxMsg txMsg;
    CAN_EXTID_INFO info;
    
    info.id.funID = CAN_FUN_POWER_ON;
//    info.id.src = pKW_SHM->ds.slotID;
    info.id.dst = CAN_ADDR_MCU_GROUP;
    info.id.pri = CAN_PRI_M;
    info.id.res = 0;

    txMsg.ExtId = info.value;
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 3;
    
//    txMsg.Data[0] = pKW_SHM->ds.boardTypeID;
    txMsg.Data[1] = 10;
//    txMsg.Data[2] = pKW_SHM->ds.boxID;
    
    Can1Q_Push_Tx_Msg(1, rt_tick_get(), &txMsg);
    Can2Q_Push_Tx_Msg(2, rt_tick_get(), &txMsg);
}

static void MVB_PowerOn(void)
{
    CanTxMsg txMsg;
    CAN_EXTID_INFO info;
	uint16_t crc16 = 0;
    
    info.id.funID = CAN_FUN_POWER_ON;
    info.id.src = mvb_board.slotID;
    info.id.dst = CAN_ADDR_MCU_GROUP;
    info.id.pri = CAN_PRI_M;
    info.id.res = 0;

    txMsg.ExtId = info.value;
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 3;
    
    txMsg.Data[0] = mvb_board.boardType;
    txMsg.Data[1] = mvb_board.version;
//    txMsg.Data[2] = pKW_SHM->ds.boxID;
	crc16 = getCRC16(txMsg.Data,2);
	txMsg.Data[2] = (crc16 >> 8);
	txMsg.Data[3] = crc16;	
    
    Can1Q_Push_Tx_Msg(1, rt_tick_get(), &txMsg);
    Can2Q_Push_Tx_Msg(2, rt_tick_get(), &txMsg);
}

//待优化
void MVB_Date(uint8_t *data)
{
    CanTxMsg txMsg;
    CAN_EXTID_INFO info;
    
    info.id.funID = MVB_DATE_SEND_MCU;
    info.id.src = mvb_board.slotID;
    info.id.dst = CAN_ADDR_MCU_GROUP;
    info.id.pri = CAN_PRI_M;
    info.id.res = 0;

    txMsg.ExtId = info.value;
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 3;
    
    rt_memcpy(txMsg.Data,data,8);
	    
    Can1Q_Push_Tx_Msg(1, rt_tick_get(), &txMsg);
    Can2Q_Push_Tx_Msg(2, rt_tick_get(), &txMsg);
}

