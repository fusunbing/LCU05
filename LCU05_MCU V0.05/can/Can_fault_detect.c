

#include "Can_fault_detect.h"


#define NODES_MAX       (40)
#define FLT_TIMEOUT     (10)

#define CAR_NUM         (8)


static uint32_t can1_cnt[NODES_MAX] = { 0 };
static uint32_t can2_cnt[NODES_MAX] = { 0 };

static uint32_t extCan1_cnt[CAR_NUM] = { 0 };
static uint32_t extCan2_cnt[CAR_NUM] = { 0 };


void CanNode_Update(void)
{
    uint32_t i;

    for(i = 0; i < NODES_MAX; i++)
    {
        can1_cnt[i]++;
        can2_cnt[i]++;
    }
    
    for(i = 0; i < CAR_NUM; i++)
    {
        extCan1_cnt[i]++;
        extCan2_cnt[i]++;
    }    
}


void CanNode_Clear(uint32_t port, uint32_t src)
{
    if(src < NODES_MAX)
    {
        switch(port)
        {
            case 1:                
                can1_cnt[src] = 0;
                break;
            case 2:
                can2_cnt[src] = 0;
                break;
            default:
                break;
        }
    }
}



void ExtCanNode_Clear(uint32_t port, uint32_t funID)
{
    uint32_t carID;

    carID = (funID - 0x80) / 16;
    
    if(carID < CAR_NUM)
    {
        switch(port)
        {
            case 1:                
                extCan1_cnt[carID] = 0;
                break;
            case 2:
                extCan2_cnt[carID] = 0;
                break;
            default:
                break;
        }
    }
}



rt_err_t Get_CanSts(uint32_t port, uint32_t src)
{
    rt_err_t ret = RT_ERROR;
    
    if(src < NODES_MAX)
    {
        switch(port)
        {
            case 1:
                ret = (can1_cnt[src] >= FLT_TIMEOUT) ? RT_ERROR : RT_EOK;
                break;
            case 2:
                ret = (can2_cnt[src] >= FLT_TIMEOUT) ? RT_ERROR : RT_EOK;
                break;            
            default:
                break;
        }
        
    }

    return ret;
}


rt_err_t Get_ExtCanSts(uint32_t port, uint32_t carID)
{
    rt_err_t ret = RT_ERROR;
    
    if(carID < CAR_NUM)
    {
        switch(port)
        {
            case 1:
                ret = (extCan1_cnt[carID] >= FLT_TIMEOUT) ? RT_ERROR : RT_EOK;
                break;
            case 2:
                ret = (extCan2_cnt[carID] >= FLT_TIMEOUT) ? RT_ERROR : RT_EOK;
                break;            
            default:
                break;
        }
        
    }

    return ret;
}

