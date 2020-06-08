

#include "userApp.h"
#include "Bsp_canApp.h"
#include "Bsp_canPrint.h"
#include "Can_fault_detect.h"


#define CAN_MANAGE_PWRON            (0)
#define CAN_MANAGE_SELF_CHECK       (1)
#define CAN_MANAGE_SCAN             (2)
#define CAN_MANAGE_REMOTE_IN        (3)
#define CAN_MANAGE_LIFESIGN         (4)


CanRx_hook canRxHandle[256] = {RT_NULL};

static uint32_t can_manage_sts = CAN_MANAGE_PWRON;


static void can_input_handle(CanRxMsg* pMsg)
{
    CAN_EXTID_INFO info = {0};
    uint8_t index;
    
    info.value = pMsg->ExtId;
    
    if(info.id.dst == CAN_ADDR_MCU_GROUP && pMsg->DLC == 2)
    {
        if(info.id.src >= SLOT_ID_IO_MIN && info.id.src <= SLOT_ID_IO_MAX)
        {
            //第几块IO板
            index = info.id.src - SLOT_ID_IO_MIN;
            
            //判断CAN输入信号是否与之前输入信号一致
            if(ds.DIO[index].in[0] != pMsg->Data[0] || ds.DIO[index].in[1] != pMsg->Data[1])
            {
                ds.DIO[index].in[0] = pMsg->Data[0];
                ds.DIO[index].in[1] = pMsg->Data[1];
            }
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
                rt_memcpy(ds.MCU[0].ou, pMsg->Data, 8);
                break;
            case SLOT_ID_MCU_B:
                rt_memcpy(ds.MCU[1].ou, pMsg->Data, 8);
                break;
            case SLOT_ID_MCU_C:
                rt_memcpy(ds.MCU[2].ou, pMsg->Data, 8);
                break;
            default:
                break;
        }
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


static void can_lifesign_handle(CanRxMsg* pMsg)
{
    CAN_EXTID_INFO info = {0};
    
    info.value = pMsg->ExtId;
    
    if(info.id.dst == CAN_ADDR_MCU_GROUP || info.id.dst == CAN_ADDR_BROADCAST)
    {
        switch(info.id.src)
        {
            case SLOT_ID_MCU_A:
                rt_memcpy(&ds.MCU[0].lifesign, pMsg->Data, 6);
                break;
            case SLOT_ID_MCU_B:
                rt_memcpy(&ds.MCU[1].lifesign, pMsg->Data, 6);
                break;
            case SLOT_ID_MCU_C:
                rt_memcpy(&ds.MCU[2].lifesign, pMsg->Data, 6);
                break;            
            default:
                if(info.id.src >= SLOT_ID_IO_MIN && info.id.src <= SLOT_ID_IO_MAX && pMsg->DLC == 4)
                {
                    ds.DIO[info.id.src - SLOT_ID_IO_MIN].lifesign = pMsg->Data[0];
                    ds.DIO[info.id.src - SLOT_ID_IO_MIN].version = pMsg->Data[1];
                    ds.DIO[info.id.src - SLOT_ID_IO_MIN].in[0] = pMsg->Data[2];
                    ds.DIO[info.id.src - SLOT_ID_IO_MIN].in[1] = pMsg->Data[3];
                }
                break;
        }
    }
}


//远程输入信号存储
static void can_remoteIn_handle(CanRxMsg* pMsg)
{
    CAN_EXTID_INFO info = {0};
    
    info.value = pMsg->ExtId;
    
    if(info.id.dst == CAN_ADDR_MCU_GROUP && info.id.src == SLOT_ID_CAN && pMsg->DLC == 8)
    {
        if(info.id.funID % 16 == 0)
        {
            //计算偏移地址：64 + (info.id.funID - 128) % 16 * 8
            //rt_memcpy(&ds.inBuf[info.id.funID * 8 - 96], pMsg->Data, pMsg->DLC);
        }
    }
}


void can_rx_handle_register(void)
{
    uint32_t i;
    
    canRxHandle[CAN_FUN_INPUT] = can_input_handle;
    canRxHandle[CAN_FUN_OUTPUT] = can_output_handle;
    canRxHandle[CAN_FUN_IO_STS] = can_io_sts_handle;
    canRxHandle[CAN_FUN_LIFESIGN] = can_lifesign_handle;
    
    for(i = CAN_FUN_CAR1_DATA; i < 256; i++)
    {
        canRxHandle[i] = can_remoteIn_handle;
    }

}


void can_rx_serve(CanRxMsg* pMsg)
{
    CAN_EXTID_INFO info = {0};
    
    info.value = pMsg->ExtId;
    
    CanNode_Clear(info.id.port, info.id.src);
    CanNode_Clear(info.id.port, ds.slotID);
    
    //如果接收到源地址和自己的地址一致，重新读取slot_id
    if(info.id.src == ds.slotID)
    {
        userApp_init();
    }
    
    if(info.id.funID < 32)
    {
        //can接收数据处理
        if(canRxHandle[info.id.funID] != RT_NULL)
        {
            canRxHandle[info.id.funID](pMsg);
        }
    }
}


/************************************************** CAN TX Serve **************************************************/


void can_send_output(void)
{
    CanTxMsg txMsg = { 0 };
    CAN_EXTID_INFO info = { 0 };
    
    info.id.funID = CAN_FUN_OUTPUT;
    info.id.src = ds.slotID;
    info.id.dst = CAN_ADDR_BROADCAST;
    info.id.pri = CAN_PRI_H;
    info.id.port = 0;

    txMsg.ExtId = info.value;
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 8;

    rt_memcpy(txMsg.Data, ds.ouBuf, 8);
    
    if(can_manage_sts == CAN_MANAGE_SCAN || can_manage_sts == CAN_MANAGE_REMOTE_IN)
    {
        CANx_Send(&txMsg);
        CANx_Send(&txMsg);
    }
}


static void can_send_PwrOn(void)
{
    CanTxMsg txMsg = { 0 };
    CAN_EXTID_INFO info = { 0 };
    static uint8_t lifesign = 0;
    
    info.id.funID = CAN_FUN_LIFESIGN;
    info.id.src = ds.slotID;
    info.id.dst = CAN_ADDR_BROADCAST;
    info.id.pri = CAN_PRI_L;
    info.id.port = 0;

    txMsg.ExtId = info.value;
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 6;
    
    txMsg.Data[0] = lifesign++;         //生命信号
    txMsg.Data[1] = MINOR_VERSION;      //底层软件版本号
    txMsg.Data[2] = 3;                  //KW软件版本号
    txMsg.Data[3] = ds.carID;           //车节号
    txMsg.Data[4] = ds.dc110v;          //状态信息
    txMsg.Data[5] = ds.dc5v;            //状态信息
    
    CANx_Send(&txMsg);
}


static void can_send_selfCheck(uint8_t index)
{
    CanTxMsg txMsg = { 0 };
    CAN_EXTID_INFO info = { 0 };
    
    info.id.funID = CAN_FUN_SELF_CHECK;
    info.id.src = ds.slotID;
    info.id.dst = CAN_ADDR_BROADCAST;
    info.id.pri = CAN_PRI_H;
    info.id.port = 0;

    txMsg.ExtId = info.value;
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 1;
    
    txMsg.Data[0] = index;

    CANx_Send(&txMsg);
}


static void can_send_remote_in(void)
{
    CanTxMsg txMsg = { 0 };
    CAN_EXTID_INFO info = { 0 };
    
    info.id.funID = (ds.carID * 16 + 128);
    info.id.src = ds.slotID;
    info.id.dst = SLOT_ID_CAN;
    info.id.pri = CAN_PRI_H;
    info.id.port = 0;

    txMsg.ExtId = info.value;
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 8;

    rt_memcpy(txMsg.Data, &ds.inBuf[ds.carID * 8 + 64], 8);

    CANx_Send(&txMsg);
    CANx_Send(&txMsg);
}


static uint8_t testBuf[64] = {0};

static void can_send_md(void)
{
    CanTxMsg txMsg = { 0 };
    CAN_EXTID_INFO info = { 0 };
    uint32_t i;

    for(i = 0; i < 8; i++)
    {
        info.id.funID = (ds.carID * 16 + 129 + i);
        info.id.src = ds.slotID;
        info.id.dst = CAN_ADDR_NET_GROUP;
        info.id.pri = CAN_PRI_L;
        info.id.port = 0;

        txMsg.ExtId = info.value;
        txMsg.IDE = CAN_ID_EXT;
        txMsg.RTR = 0;
        txMsg.DLC = 8;

        rt_memcpy(txMsg.Data, &testBuf[i * 8], txMsg.DLC);
        CANx_Send(&txMsg);
        CANx_Send(&txMsg);
        rt_thread_delay(1);
    }
}


static void ExtCanData_Send(void)
{
    switch(ds.slotID)
    {
        case SLOT_ID_MCU_A:
            can_send_remote_in();
            rt_thread_delay(1);
            can_send_md();
            break;
   
        case SLOT_ID_MCU_B:
            if(ds.MCU[0].flt.lost == RT_ERROR)
            {
                can_send_remote_in();
                rt_thread_delay(1);
                can_send_md();
            }
            else
            {
                can_send_remote_in();
            }
            
            break;
        case SLOT_ID_MCU_C:
            if(ds.MCU[0].flt.lost == RT_ERROR && ds.MCU[1].flt.lost == RT_ERROR)
            {
                can_send_remote_in();
                rt_thread_delay(1);
                can_send_md();
            }
            else
            {
                can_send_remote_in();
            }
            break;
        default:
            
            break;
    }
}


void can_tx_serve(void)
{
    static uint8_t index = 0;
    
    //can故障检测
	CanNode_Update();
    
    switch(can_manage_sts)
    {
        case CAN_MANAGE_PWRON:
            
            can_send_PwrOn();
        
            if(ds.MCU[0].dc110v == 1 && ds.MCU[1].dc110v == 1 && ds.MCU[2].dc110v == 1) //IO板卡正常进入自检态
            {
                can_manage_sts = CAN_MANAGE_SELF_CHECK;
            }
            else
            {
                if(rt_tick_get() < 2000) //不满足输出通道自检条件且上电超过2s直接进入扫描态
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
            can_manage_sts = CAN_MANAGE_REMOTE_IN;
        
            break;
        case CAN_MANAGE_REMOTE_IN:

            ExtCanData_Send();            
            can_manage_sts = CAN_MANAGE_SCAN;
        
            break;
        
        default:
            
            break;
    }
}

