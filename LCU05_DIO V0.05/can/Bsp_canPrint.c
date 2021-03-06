 

#include "Bsp_canPrint.h"
#include "Bsp_canApp.h"


static __IO uint8_t  canPrintFlag = 0;


void Can_RX_Print(CanRxMsg* pMsg, uint32_t port)
{
    uint8_t  i = 0;
    CAN_EXTID_INFO info;
    
    if (canPrintFlag)
    {     
        info.value = pMsg->ExtId;
        
        rt_kprintf("CAN%d, R, %10d, %2d -> %2d, %2x, %d, %d, [", port, rt_tick_get(), info.id.src, info.id.dst, info.id.funID, info.id.pri, pMsg->DLC);
        
        for (i = 0; i < pMsg->DLC; i++)
        {
            rt_kprintf(" %2x,", pMsg->Data[i]);
        }
        
        rt_kprintf("]\r\n");
    }
}


void Can_TX_Print(CanTxMsg* pMsg, uint32_t port)
{
    uint8_t  i = 0;
    CAN_EXTID_INFO info;
    
    if (canPrintFlag)
    {
        info.value = pMsg->ExtId;
        
        rt_kprintf("CAN%d, T, %10d, %2d -> %2d, %2x, %d, %d, [", port, rt_tick_get(), info.id.src, info.id.dst, info.id.funID, info.id.pri, pMsg->DLC);
        
        for (i = 0; i < pMsg->DLC; i++)
        {
            rt_kprintf(" %2x,", pMsg->Data[i]);
        }
        
        rt_kprintf("]\r\n");
    }
}


void can_show(void)
{
    if (canPrintFlag)
        canPrintFlag = 0;
    else
        canPrintFlag = 1;
}


#include "finsh.h"
FINSH_FUNCTION_EXPORT(can_show, show system Can data)

