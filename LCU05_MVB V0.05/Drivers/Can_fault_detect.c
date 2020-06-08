

#include "rtthread.h"
#include "Can_fault_detect.h"

#define CAN_NODES_CNT_MAX   (33)
#define CAN_FLT_TIMEOUT_CNT  (10)

typedef struct
{
    uint8_t cnt1;
    uint8_t cnt2;
    uint8_t can1_fault;
    uint8_t can2_fault;
}CAN_NODE_FAULT, *PCAN_NODE_FAULT;


static CAN_NODE_FAULT canNodesFault[CAN_NODES_CNT_MAX] = { 0 };


//收到CAN数据，清除故障标志位
void Can_Nodes_Flt_Clear(uint8_t port, uint8_t src)
{
    uint8_t canNodeIndex = 0;
    
    canNodeIndex = src - 1;
    
    if(canNodeIndex >= 0 && canNodeIndex < CAN_NODES_CNT_MAX)
    {
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
        rt_kprintf("+Can receive slot ID err! \r\n");
    }
}


void Can_Nodes_Flt_Update(void)
{
    uint8_t i;

    for(i = 0; i < CAN_NODES_CNT_MAX; i++)
    {
        canNodesFault[i].cnt1++;
        canNodesFault[i].cnt2++;

        //can1故障判断
        if(canNodesFault[i].cnt1 >= CAN_FLT_TIMEOUT_CNT)
        {
            canNodesFault[i].can1_fault = 1;	
            canNodesFault[i].cnt1 = CAN_FLT_TIMEOUT_CNT;
        }
        else
        {
            canNodesFault[i].can1_fault = 0;	
        }
                
        //can2故障判断
        if(canNodesFault[i].cnt2 >= CAN_FLT_TIMEOUT_CNT)
        {
            canNodesFault[i].can2_fault = 1;
            canNodesFault[i].cnt2 = CAN_FLT_TIMEOUT_CNT;
        }
        else
        {
            canNodesFault[i].can2_fault = 0;
        }
    }
}


uint8_t Get_Can_Nodes_Flt(uint8_t port, uint8_t index)
{
    uint8_t temp;
    
    switch(port)
    {
        case 1:
            temp = canNodesFault[index].can1_fault;
            break;
        case 2:
            temp = canNodesFault[index].can2_fault;
            break;
        default:
            break;
    }
    
    return temp;
}


