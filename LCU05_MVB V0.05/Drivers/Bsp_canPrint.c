

#include "Bsp_canPrint.h"


static __IO uint8_t  canPrintFlag = 0;

void Can_RX_Print(CAN_RX_DATA_RAM * pDate)
{
    uint8_t  i = 0;
    CAN_EXTID_INFO info;
    
    if (canPrintFlag)
    {     
        info.value = pDate->rxMsg.ExtId;       
        
        rt_kprintf("CAN%d, R, %10d, %2d -> %2d, %2x, %d, %d, [", pDate->canPort, pDate->tick, info.id.src, info.id.dst, info.id.funID, info.id.pri, pDate->rxMsg.DLC);
        
        for (i = 0; i < pDate->rxMsg.DLC; i++)
        {
            rt_kprintf(" %2x,", pDate->rxMsg.Data[i]);
        }
        
        rt_kprintf("]\r\n");
    }
}


void Can_TX_Print(CAN_TX_DATA_RAM * pDate)
{
    uint8_t  i = 0;
    CAN_EXTID_INFO info;
    
    if (canPrintFlag)
    {
        info.value = pDate->txMsg.ExtId;
        
        rt_kprintf("CAN%d, T, %10d, %2d -> %2d, %2x, %d, %d, [", pDate->canPort, pDate->tick, info.id.src, info.id.dst, info.id.funID, info.id.pri, pDate->txMsg.DLC);
        
        for (i = 0; i < pDate->txMsg.DLC; i++)
        {
            rt_kprintf(" %2x,", pDate->txMsg.Data[i]);
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

