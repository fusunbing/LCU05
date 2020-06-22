
#include "userApp.h"
#include "Bsp_canApp.h"
#include "Bsp_canPrint.h"
#include "crc16.h"
#include "mvb.h"

#define CAN_SEND_TRYTIMES   (3)


static void can_rx_handle(CAN_RX_DATA_RAM * pDate)
{
    CAN_EXTID_INFO info = {0};
    uint32_t offset = 0;
    uint32_t carID = 0;
    
    info.value = pDate->rxMsg.ExtId;
    
    //���ݹ��ܺŽ��д���
    if(info.id.funID >= 0x80 && info.id.dst == CAN_ADDR_BROADCAST && pDate->rxMsg.DLC == 8)
    {
        if(info.id.src == SLOT_ID_CAN || info.id.src == SLOT_ID_MCU_A || info.id.src == SLOT_ID_MCU_B || info.id.src == SLOT_ID_MCU_C)
        {
            carID = (info.id.funID - 0x80) / 16;
            offset = info.id.funID % 16;
            
            if(offset > 0 && offset < 5)
            {
                carID = carID + 2;
                offset = (offset - 1) * 8;                
                rt_memcpy(&ds.mvb_port[carID].data[offset], pDate->rxMsg.Data, pDate->rxMsg.DLC);
            }
            else if(offset > 4 && offset < 9)
            {
                carID = carID + 6;
                offset = (offset - 5) * 8;
                rt_memcpy(&ds.mvb_port[carID].data[offset], pDate->rxMsg.Data, pDate->rxMsg.DLC);
            }
        }
    }
}


void can_rx_serve()
{   
    uint32_t can_rxBuf;
    CAN_RX_DATA_RAM * pCan_rxBuf;

    //�Ӷ��е�����������֡��ַ
    can_rxBuf = CanQ_Pop_Rx_Msg();
    
    //����ָ�벻Ϊ��, ���ʾ���в����ɹ�
    if (can_rxBuf != CAN_QUEUE_NULL)
    {
        //ת��ָ������: RAM�������ݿ�
        pCan_rxBuf = (CAN_RX_DATA_RAM *)can_rxBuf; 

        //can�������ݴ���
        can_rx_handle(pCan_rxBuf);        
        
        //can�������ݴ�ӡ
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


static void canSend_Lifesign(void)
{
    CanTxMsg txMsg;
    CAN_EXTID_INFO info;
    
    info.id.funID = CAN_FUN_MVB_RTC;
    info.id.src = ds.slotID;
    info.id.dst = CAN_ADDR_BROADCAST;
    info.id.pri = CAN_PRI_L;
    info.id.res = 0;

    txMsg.ExtId = info.value;
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 8;
    
    txMsg.Data[0] = ds.mvb_port[0].data[0];     //�����ź�H
    txMsg.Data[1] = ds.mvb_port[0].data[1];     //�����ź�L
    txMsg.Data[2] = ds.mvb_port[1].data[2];     //�꣨��ֵ��Χ0x00��0x63��Ӧ2000�ꡫ2099�꣩
	txMsg.Data[3] = ds.mvb_port[1].data[3];     //�£���ֵ��Χ0x01��0x0C��Ӧ   1�¡�12��  �� 
	txMsg.Data[4] = ds.mvb_port[1].data[4];	    //�գ���ֵ��Χ0x01��0x1F��Ӧ   1�ա�31��  ��
    txMsg.Data[5] = ds.mvb_port[1].data[5];     //ʱ����ֵ��Χ0x00��0x17��Ӧ   0ʱ��23ʱ  ��
	txMsg.Data[6] = ds.mvb_port[1].data[6];     //�֣���ֵ��Χ0x00��0x3B��Ӧ   0�֡�59��  ��
	txMsg.Data[7] = ds.mvb_port[1].data[7];	    //�루��ֵ��Χ0x00��0x3B��Ӧ   0�롫59��  ��
    
    Can1Q_Push_Tx_Msg(1, rt_tick_get(), &txMsg);
    Can2Q_Push_Tx_Msg(2, rt_tick_get(), &txMsg);
}

static void canSend_PowerOn(void)
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
    
    txMsg.Data[0] = lifesign++;     //�����ź�
    txMsg.Data[1] = ds.version;     //MVB������汾��
    txMsg.Data[2] = ds.carID;       //MVB���ȡ���ں�
    
    Can1Q_Push_Tx_Msg(1, rt_tick_get(), &txMsg);
    Can2Q_Push_Tx_Msg(2, rt_tick_get(), &txMsg);
}


void canApp_serve(void)
{
    if(rt_tick_get() < 2000)
    {
        canSend_PowerOn();
    }
    else
    {
        canSend_Lifesign();
    }

}

