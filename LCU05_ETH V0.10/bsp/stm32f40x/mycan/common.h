
#ifndef __COMMON_H__
#define __COMMON_H__

#include "stm32f4xx.h"


#define	SLOT_ID_MVB			(1)
#define	SLOT_ID_CAN			(2)
#define	SLOT_ID_ETU			(3)
#define	SLOT_ID_MCU_A		(4)
#define	SLOT_ID_MCU_B		(5)
#define	SLOT_ID_MCU_C		(6)
#define	SLOT_ID_IO_MIN		(7)
#define	SLOT_ID_IO_MAX		(33)

#define	BOARD_TYPE_ID_MVB	(1)
#define	BOARD_TYPE_ID_MCU	(2)
#define	BOARD_TYPE_ID_CAN	(3)
#define	BOARD_TYPE_ID_ETU	(4)
#define	BOARD_TYPE_ID_IO	(5)


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

#define CAN_FUN_MVB_RTC                 (5)

#define CAN_FUN_IO_STS                  (9)
#define CAN_FUN_POWER_ON                (10)
#define CAN_FUN_SELF_CHECK              (11)

#define CAN_FUN_CAR1_DATA               (0x80)
#define CAN_FUN_CAR2_DATA               (0x90)
#define CAN_FUN_CAR3_DATA               (0xA0)
#define CAN_FUN_CAR4_DATA               (0xB0)
#define CAN_FUN_CAR5_DATA               (0xC0)
#define CAN_FUN_CAR6_DATA               (0xD0)
#define CAN_FUN_CAR7_DATA               (0xE0)
#define CAN_FUN_CAR8_DATA               (0xF0)


//结构体重定义
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
    uint8_t slotID;
    uint8_t boardType;
    uint8_t carID;
    uint8_t version;	
}DS_STU,*PDS_STU;


extern DS_STU ds;


void Board_Information_Init(void);


#endif



