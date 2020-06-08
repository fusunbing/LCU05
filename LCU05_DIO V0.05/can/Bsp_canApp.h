

#ifndef _BSP_CAN_APP_H_
#define _BSP_CAN_APP_H_

#ifdef __cplusplus
extern "C" {
#endif 


#include "Bsp_canTask.h"


//CAN comm pri
#define CAN_PRI_VH                      (0x00)
#define CAN_PRI_H                       (0x01)
#define CAN_PRI_M                       (0x02)
#define CAN_PRI_L                       (0x04)

//address
#define CAN_ADDR_MCU_GROUP             	(0x30)
#define CAN_ADDR_NET_GROUP             	(0x3F)
#define CAN_ADDR_BROADCAST             	(0xFF)

//function id
#define CAN_FUN_INPUT                   (1)
#define CAN_FUN_OUTPUT                  (2)
#define CAN_FUN_REMOTE_IN               (3)

#define CAN_FUN_IO_STS                  (9)
#define CAN_FUN_LIFESIGN                (10)
#define CAN_FUN_SELF_CHECK              (11)

#define CAN_FUN_CAR1_IN                 (20)
#define CAN_FUN_CAR2_IN                 (21)
#define CAN_FUN_CAR3_IN                 (22)
#define CAN_FUN_CAR4_IN                 (23)
#define CAN_FUN_CAR5_IN                 (24)
#define CAN_FUN_CAR6_IN                 (25)
#define CAN_FUN_CAR7_IN                 (26)
#define CAN_FUN_CAR8_IN                 (27)


typedef union
{
    struct
    {
        uint32_t funID      :9;     //function id
        uint32_t src        :8;     //source address
        uint32_t dst        :8;     //destination address
        uint32_t pri        :4;     //priority
        uint32_t port       :3;
    }id;
    uint32_t value;
}CAN_EXTID_INFO, *PCAN_EXTID_INFO;


typedef void (*CanRx_hook)(CanRxMsg* msg);


void can_rx_handle_register(void);
void can_rx_serve(CanRxMsg* pMsg);
void Can_Cycle_Service(void);
void Can_Event_Service(void);


#ifdef __cplusplus
}
#endif

#endif

