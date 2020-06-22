
#include "Bsp_canTask.h"
#include "userApp.h"
#include "Bsp_canApp.h"
#include "Bsp_canPrint.h"
#include "Can_fault_detect.h"


#define CAN_MANAGE_PWR_ON           (0)
#define CAN_MANAGE_SELF_CHECK       (1)
#define CAN_MANAGE_SCAN             (2)
#define CAN_MANAGE_LIFESIGN         (3)

static CanRx_hook canRxHandle[16] = {RT_NULL};

static uint32_t can_manage_sts = CAN_MANAGE_PWR_ON;

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
        if(info.id.src == ds.DIO[0].slotID)
        {
            rt_memcpy(ds.DIO[0].in, pMsg->Data, pMsg->DLC);
        }
        else if(info.id.src == ds.DIO[1].slotID)
        {
            rt_memcpy(ds.DIO[1].in, pMsg->Data, pMsg->DLC);
        }
        else if(info.id.src == ds.DIO[2].slotID)
        {
            rt_memcpy(ds.DIO[2].in, pMsg->Data, pMsg->DLC);
        }
    }
}


static void can_powerOn_handle(CanRxMsg* pMsg)
{
    CAN_EXTID_INFO info = {0};
    
    info.value = pMsg->ExtId;
    
    if(info.id.dst == CAN_ADDR_BROADCAST && pMsg->DLC == 6)
    {
        switch(info.id.src)
        {
            case SLOT_ID_MCU_A:
                rt_memcpy(&ds.MCU[0].lifesign, pMsg->Data, pMsg->DLC);
                break;
            case SLOT_ID_MCU_B:
                rt_memcpy(&ds.MCU[1].lifesign, pMsg->Data, pMsg->DLC);
                break;
            case SLOT_ID_MCU_C:
                rt_memcpy(&ds.MCU[2].lifesign, pMsg->Data, pMsg->DLC);
                break;            
            default:

                break;
        }
    }
}


static void can_self_check_Handle(CanRxMsg* pMsg)
{
    CAN_EXTID_INFO info = {0};
    uint32_t i;
    
    info.value = pMsg->ExtId;

    if(rt_tick_get() < 2000)
    {
        if(info.id.src == SLOT_ID_MCU_A && info.id.dst == CAN_ADDR_BROADCAST && pMsg->DLC == 1)
        {
            if(pMsg->Data[0] == (ds.DIO[0].slotID - SLOT_ID_IO_MIN) % 3)
            {
                rt_memset(ds.MCU[0].ou, 0xFF, 8);
                rt_memset(ds.MCU[1].ou, 0xFF, 8);
                rt_memset(ds.MCU[2].ou, 0xFF, 8);
                
                rt_thread_delay(30);
                
                for(i = 0; i < 6; i++)
                {
                    if(ds.DIO[0].Bits_fb[i * 2] == 0)
                    {
                        ds.DIO[0].Bits_ouFlt[i] = RT_ERROR;
                    }
                }
                
                //干节点前级mos故障诊断
                if(ds.DIO[0].Bits_fb[12] == 1 && ds.DIO[0].Bits_fb[13] == 0)
                {
                    ds.DIO[0].Bits_ouFlt[6] = RT_ERROR;
                }
                    
                rt_memset(ds.MCU[0].ou, 0, 8);
                rt_memset(ds.MCU[1].ou, 0, 8);
                rt_memset(ds.MCU[2].ou, 0, 8);
            }
            else if( (pMsg->Data[0] + 1) % 3 == (ds.DIO[0].slotID - SLOT_ID_IO_MIN) % 3 )
            {
                rt_thread_delay(30);
                
                for(i = 0; i < 6; i++)
                {
                    if(ds.DIO[0].Bits_fb[i * 2 + 1] == 1)
                    {
                        ds.DIO[0].Bits_ouFlt[i] = RT_ERROR;
                    }
                }
                
                //干节点后级mos故障诊断
                if(ds.DIO[0].Bits_fb[12] == 1 && ds.DIO[0].Bits_fb[14] == 1)
                {
                    ds.DIO[0].Bits_ouFlt[6] = RT_ERROR;
                }
            }
            
            switch(pMsg->Data[0])
            {
                case 0:
                    can_manage_sts = CAN_MANAGE_SELF_CHECK;
                    break;
                case 1:
                    can_manage_sts = CAN_MANAGE_SELF_CHECK;
                    break;
                case 2:
                    can_manage_sts = CAN_MANAGE_SCAN;
                    break;
                default:
                    break;
            }
            
        }
    }
}


void can_rx_handle_register(void)
{
    canRxHandle[CAN_FUN_OUTPUT] = can_output_handle;
    canRxHandle[CAN_FUN_IO_STS] = can_io_sts_handle;
    canRxHandle[CAN_FUN_POWER_ON] = can_powerOn_handle;
    canRxHandle[CAN_FUN_SELF_CHECK] = can_self_check_Handle; 
}


void can_rx_serve(CanRxMsg* pMsg)
{
    CAN_EXTID_INFO info = {0};
    static uint32_t slotID_Flt_cnt = 0;
    
    info.value = pMsg->ExtId;
    
    CanNode_Clear(info.id.port, info.id.src);
    CanNode_Clear(info.id.port, ds.DIO[0].slotID);
    
    //如果接收到源地址和自己的地址一致，重新读取slot_id
    if(info.id.src == ds.DIO[0].slotID)
    {
        slotID_Flt_cnt++;
        
        GetSlotID();
        return;
    }
    
    if(info.id.funID < 16)
    {
        //can接收数据处理
        if(canRxHandle[info.id.funID] != RT_NULL)
        {
            canRxHandle[info.id.funID](pMsg);
        }
    }
}


/************************************************** CAN TX Serve **************************************************/


static void Can_Send_PwrOn(void)
{
    CanTxMsg txMsg;
    CAN_EXTID_INFO info = {0};
    static uint8_t lifesign = 0;
    
    info.id.funID = CAN_FUN_POWER_ON;
    info.id.src = ds.slotID;
    info.id.dst = CAN_ADDR_BROADCAST;
    info.id.pri = CAN_PRI_H;
    info.id.port = 0;

    txMsg.ExtId = info.value;
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 2;
    
    txMsg.Data[0] = lifesign++;     //生命信号
    txMsg.Data[1] = ds.version;     //软件版本号

    CANx_Send(&txMsg);
}


//Send input data event
static void Can_Send_Input(void)
{
    CanTxMsg txMsg;
    CAN_EXTID_INFO info = {0};
    
    info.id.funID = CAN_FUN_INPUT;
    info.id.src = ds.DIO[0].slotID;
    info.id.dst = CAN_ADDR_MCU_GROUP;
    info.id.pri = CAN_PRI_VH;
    info.id.port = 0;

    txMsg.ExtId = info.value;
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 2;
    
    txMsg.Data[0] = ds.DIO[0].in[0];
    txMsg.Data[1] = ds.DIO[0].in[1];
    
    CANx_Send(&txMsg);
    CANx_Send(&txMsg);
}


//Send board io status
static void Can_Send_Sts(void)
{
    CanTxMsg txMsg;
    CAN_EXTID_INFO info;
    
    info.id.funID = CAN_FUN_IO_STS;
    info.id.src = ds.DIO[0].slotID;
    info.id.dst = CAN_ADDR_MCU_GROUP;
    info.id.pri = CAN_PRI_M;
    info.id.port = 0;

    txMsg.ExtId = info.value;
    txMsg.IDE = CAN_ID_EXT;
    txMsg.RTR = 0;
    txMsg.DLC = 8;
    
    rt_memcpy(txMsg.Data, ds.DIO[0].in, txMsg.DLC);
    
    CANx_Send(&txMsg);
}


void Can_Event_Service(void)
{
    Can_Send_Input();
}


void Can_Cycle_Service(void)
{
    rt_thread_delay(ds.DIO[0].slotID);
    
    if(rt_tick_get() > 2000)
    {
        //can故障检测
        CanNode_Update();
        
        can_manage_sts = CAN_MANAGE_SCAN;
    }

    switch(can_manage_sts)
    {
        case CAN_MANAGE_PWR_ON:
            Can_Send_PwrOn();
            break;
        
        case CAN_MANAGE_SELF_CHECK:
            //进入自检态
            break;
        
        case CAN_MANAGE_SCAN:            
        
            //周期发送状态数据
            Can_Send_Sts();  
        
            break;
        
        default:            
            break;
    }
}




