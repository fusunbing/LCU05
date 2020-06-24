

#ifndef _USER_APP_H_
#define _USER_APP_H_

#ifdef __cplusplus
extern "C" {
#endif 

    
#include <rtthread.h>
#include "stm32f4xx.h"

#include "Bsp_led.h"


#define MVB_VERSION         (5)
   
#define SLOT_ID_MVB         (1)
#define SLOT_ID_CAN         (2)
#define SLOT_ID_ETU         (3)
#define SLOT_ID_MCU_A       (4)
#define SLOT_ID_MCU_B       (5)
#define SLOT_ID_MCU_C       (6)
#define SLOT_ID_IO_MIN      (7)
#define SLOT_ID_IO_MAX      (33)

#define BOARD_TYPE_ID_MVB   (1)
#define BOARD_TYPE_ID_MCU   (2)
#define BOARD_TYPE_ID_CAN   (3)
#define BOARD_TYPE_ID_ETU   (4)
#define BOARD_TYPE_ID_IO    (5)

#define CAR_ID_MC1          (0)     //MC1车节号
#define CAR_ID_TP1          (1)     //TP1车节号
#define CAR_ID_TP2          (2)     //TP2车节号
#define CAR_ID_MC2          (3)     //MC2车节号

#define CAR_NUM             (4)

typedef union
{
    struct
    {
        uint8_t bit0    :1;
        uint8_t bit1    :1;
        uint8_t bit2    :1;
        uint8_t bit3    :1;
        uint8_t bit4    :1;
        uint8_t bit5    :1;
        uint8_t bit6    :1;
        uint8_t bit7    :1;
    } Bits;
    
    uint8_t value;
    
}BYTE_TO_BIT, *PBYTE_TO_BIT;


typedef struct
{
    uint8_t data[32];      //端口索引
}MVB_PORT_DATA_STU, *PMVB_PORT_DATA_STU;


typedef struct
{
    uint8_t can1            :1;     //机箱CAN1故障
    uint8_t can2            :1;     //机箱CAN2故障
    uint8_t lost            :1;     //机箱丢失
    uint8_t res             :5;
}CAR_FLT_STU, *PCAR_FLT_STU;


typedef struct 
{
    uint8_t version;   
    uint8_t boardType;    
    uint8_t slotID;
    uint8_t carID;
    
    uint32_t mvbCnt;
    
    CAR_FLT_STU car[CAR_NUM];

    MVB_PORT_DATA_STU mvb_port[32];
}DS_STU, *PDS_STU;


extern DS_STU ds;


void userApp_init(void);


#ifdef __cplusplus
}
#endif

#endif

