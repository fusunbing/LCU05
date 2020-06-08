

#ifndef _BSP_CAN_APP_H_
#define _BSP_CAN_APP_H_

#ifdef __cplusplus
extern "C" {
#endif 


#include "Bsp_canTask.h"

#define CAN_SEND_TRYTIMES       		(3)
	
//CAN comm pri
#define CAN_PRI_VH                      (0x00)
#define CAN_PRI_H                       (0x01)
#define CAN_PRI_M                       (0x02)
#define CAN_PRI_L                       (0x04)

#define CAN_ADDR_MCU_GROUP             	(0x30)
#define CAN_ADDR_NET_GROUP             	(0x3F)
#define CAN_ADDR_BROADCAST             	(0xFF)
	
#define CAN_FUN_INPUT                   (0x01)
#define CAN_FUN_OUTPUT                  (0x02)
#define CAN_FUN_REMOTE_IN               (0x03)
    
#define CAN_FUN_POWER_ON                (0x10)
#define CAN_FUN_DIO_FLT                 (0x11)

#define CAN_FUN_MVB_LIFESIGNAL          (0x20)
#define CAN_FUN_MVB_RTC                 (0x20)



typedef union
{
    struct
    {
        uint32_t funID      :9;     //function id
        uint32_t src        :8;     //source address
        uint32_t dst        :8;     //destination address
        uint32_t pri        :4;     //priority
        uint32_t res        :3;     //can port
    } id;
    uint32_t value;
}CAN_EXTID_INFO, *PCAN_EXTID_INFO;


void can_rx_serve(void);
void can1_tx_serve(void);
void can2_tx_serve(void);
void canApp_serve(void);


#ifdef __cplusplus
}
#endif

#endif

