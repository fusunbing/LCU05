/*
 *File: Bsp_canApp.c
 *This file is part of can serve.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-10-21     fusunbing    first version
 *
 */


#include "userApp.h"
#include "Bsp_canApp.h"
#include "Bsp_canPrint.h"
#include "Can_fault_detect.h"
#include "Bsp_led.h"


//����can�������ݴ���
static void can1_rx_handle(CAN_RX_DATA_RAM * pDate)
{
    CanTxMsg txMsg = {0};
    
    CAN_EXTID_INFO info = {0};
    
    info.value = pDate->rxMsg.ExtId;

    CanNode_Clear(1, info.id.src);
    CanNode_Clear(1, ds.slotID);
    
    if(info.id.funID >= 0x80 || info.id.funID == CAN_FUN_MVB_RTC)
    {
        if(info.id.dst == CAN_ADDR_BROADCAST && pDate->rxMsg.DLC == 8)
        {
            info.id.src = ds.carID;  //�����ַ
            info.id.dst = CAN_ADDR_BROADCAST; //�㲥֡
            
            if(info.id.funID == CAN_FUN_MVB_RTC)
            {
                info.id.funID = ds.carID * 16 + 0x89;
            }
            
            txMsg.ExtId = info.value;
            txMsg.IDE = CAN_ID_EXT;
            txMsg.RTR = 0;
            txMsg.DLC = pDate->rxMsg.DLC;
            
            rt_memcpy(txMsg.Data, pDate->rxMsg.Data, pDate->rxMsg.DLC);
            Can2Q_Push_Tx_Msg(2, rt_tick_get(), &txMsg);
        }
    }
}


//����can2��������ת������can1
static void can2_rx_handle(CAN_RX_DATA_RAM * pDate)
{
    CanTxMsg txMsg = {0};
    CAN_EXTID_INFO info = {0};
    
    info.value = pDate->rxMsg.ExtId;

    CanNode_Clear(2, info.id.src);
    CanNode_Clear(2, ds.carID);
    
    info.id.src = ds.slotID; //���ںŸ�Ϊ��λ��
    
    txMsg.ExtId = info.value;
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = pDate->rxMsg.DLC;
    rt_memcpy(txMsg.Data, pDate->rxMsg.Data, pDate->rxMsg.DLC);
    
    Can1Q_Push_Tx_Msg(1, rt_tick_get(), &txMsg);
}


//�ϵ緢�Ͱ忨״̬��Ϣ
static void can1_send_lifesign(void)
{
    CanTxMsg txMsg = {0};
    CAN_EXTID_INFO info = {0};
    static uint8_t lifesign = 0;
    
    info.id.funID = CAN_FUN_POWER_ON;
    info.id.src = ds.slotID;
    info.id.dst = CAN_ADDR_BROADCAST;
    info.id.pri = CAN_PRI_M;

    txMsg.ExtId = info.value;
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 4;
    
    txMsg.Data[0] = lifesign++;     //�����ź�
    txMsg.Data[1] = ds.version;     //����汾��
    txMsg.Data[2] = ds.carID;       //���ں�
    txMsg.Data[3] = (uint8_t)ds.can2_sts;   //����CAN2״̬
    
    Can1Q_Push_Tx_Msg(1, rt_tick_get(), &txMsg);
}


void can_rx_serve()
{   
    uint32_t can_rxBuf;
    PCAN_RX_DATA_RAM pCan_rxBuf;

    //�Ӷ��е�����������֡��ַ
    can_rxBuf = CanQ_Pop_Rx_Msg();
    
    //����ָ�벻Ϊ��, ���ʾ���в����ɹ�
    if (can_rxBuf != CAN_QUEUE_NULL)
    {
        //ת��ָ������: RAM�������ݿ�
        pCan_rxBuf = (PCAN_RX_DATA_RAM)can_rxBuf; 

        //can�������ݴ���
        switch(pCan_rxBuf->canPort)
        {
            case 1:
                can1_rx_handle(pCan_rxBuf);
                break;        
            case 2:
                can2_rx_handle(pCan_rxBuf);
                break;
            default:
                break;
        }
        
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
    PCAN_TX_DATA_RAM pCan1_txBuf;

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
        
        pCan1_txBuf = (PCAN_TX_DATA_RAM)can1_txBuf;
        
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
    PCAN_TX_DATA_RAM pCan2_txBuf;

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
        
        pCan2_txBuf = (PCAN_TX_DATA_RAM)can2_txBuf;
        
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


void canApp_serve(void)
{
    CanNode_Update();
    
    ds.can1_sts = Get_CanSts(1, ds.slotID);
    ds.can2_sts = Get_CanSts(2, ds.carID);
    
    can1_send_lifesign();

    if(ds.can1_sts == RT_EOK && ds.can2_sts == RT_EOK)
    {
        System_Led_SetMode(MODE_FLASH_FAST);
    }
    else if(ds.can1_sts == RT_ERROR && ds.can2_sts == RT_EOK)
    {
        System_Led_SetMode(MODE_ON);
    }
    else if(ds.can1_sts == RT_EOK && ds.can2_sts == RT_ERROR)
    {
        System_Led_SetMode(MODE_FLASH_SLOW);
    }
    else
    {
        System_Led_SetMode(MODE_OFF);
    }

}

