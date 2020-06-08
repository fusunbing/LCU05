

#ifndef __BSP_CAN_PRINT_H
#define __BSP_CAN_PRINT_H

#ifdef __cplusplus
extern "C" {
#endif 

    
#include "Bsp_canApp.h"

    
void Can_RX_Print(CAN_RX_DATA_RAM * pDate);
void Can_TX_Print(CAN_TX_DATA_RAM * pDate);


#ifdef __cplusplus
}
#endif

#endif

