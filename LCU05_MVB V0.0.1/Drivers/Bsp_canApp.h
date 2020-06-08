

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
#define CAN_ADDR_DIO1_GROUP             (0x31)
#define CAN_ADDR_BROADCAST             	(0xFF)
	
#define CAN_FUN_INPUT                   (0x01)
#define CAN_FUN_OUTPUT                  (0x02)
    
#define CAN_FUN_POWER_ON                (0x10)
#define CAN_FUN_DIO_FLT                 (0x11)

#define CAN_MANAGE_POWER_ON             (0)
#define CAN_MANAGE_SCAN                 (1)
    
#define CAN_FLT_TIMEOUT_CNT             (10)
#define CAN_NODES_CNT_MAX               (40)

#define CAN_FUN_FLT_DATA                (0x04)
#define CAN_FUN_ST_DATA                 (0x05)

#define CAN_FUN_MCU_DATA                 (0x05)

#define MVB_DATE_SEND_MCU               (0x40)
#define MVB_CARTYPE_SEND_MCU            (0x41)

#define SLOT_ID_MCU                     (0x04)
#define SLOT_ID_MVB_                     (0x01)

    
    
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


typedef struct
{
    uint8_t  cnt1;
    uint8_t  cnt2;
}CAN_NODE_FAULT, *PCAN_NODE_FAULT;


#define CAN_FUN_SUB_SEQ                 (0x01)
    
    
void can_rx_serve(void);
void can1_tx_serve(void);
void can2_tx_serve(void);
void canApp_serve(void);
void can_send_output(uint8_t index, uint8_t date);
void MVB_Date(uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif

