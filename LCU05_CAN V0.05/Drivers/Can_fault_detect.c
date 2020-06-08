

#include "Can_fault_detect.h"


#define NODES_MAX       (40)
#define FLT_TIMEOUT     (5)


static uint32_t can1_cnt[NODES_MAX] = { 0 };
static uint32_t can2_cnt[NODES_MAX] = { 0 };


void CanNode_Update(void)
{
    uint32_t i;

    for(i = 0; i < NODES_MAX; i++)
    {
        can1_cnt[i]++;
        can2_cnt[i]++;
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


uint32_t Get_CanSts(uint32_t port, uint32_t src)
{
    uint32_t ret = RT_ERROR;
    
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

