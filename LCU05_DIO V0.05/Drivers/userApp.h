
#ifndef __USERAPP_H
#define __USERAPP_H

#ifdef __cplusplus
extern "C" {
#endif 


#include "rtthread.h"
#include "stm32f4xx.h"


//软件版本号
#define MAJOR_VERSION       (0)     //主版本号
#define MINOR_VERSION       (5)     //子版本号

//板类型
#define	BOARD_TYPE_ID_MVB	(1)
#define	BOARD_TYPE_ID_MCU	(2)
#define	BOARD_TYPE_ID_CAN	(3)
#define	BOARD_TYPE_ID_ETU	(4)
#define	BOARD_TYPE_ID_IO	(5)

//槽位ID
#define	SLOT_ID_MVB			(1)
#define	SLOT_ID_CAN			(2)
#define	SLOT_ID_ETU			(3)
#define	SLOT_ID_MCU_A		(4)
#define	SLOT_ID_MCU_B		(5)
#define	SLOT_ID_MCU_C		(6)
#define	SLOT_ID_IO_MIN		(7)
#define	SLOT_ID_IO_MAX		(33)


#define IN_CNT              (12)
#define OU_CNT              (7)
#define OU_FB_CNT           (15)


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
    uint8_t can1        :1;
    uint8_t can2        :1;
    uint8_t isLost      :1;
    uint8_t res         :6;
}CAN_NODE_STU, *PCAN_NODE_STU;


typedef struct
{
    uint8_t slotID;
    uint8_t version;
    CAN_NODE_STU flt;
    uint8_t res;

    uint8_t in[2];
    uint8_t ou;
    uint8_t fb[2];
    uint8_t inFlt[2];
    uint8_t ouFlt;
    uint32_t *Bits_in;
    uint32_t *Bits_ou;
    uint32_t *Bits_fb;
    uint32_t *Bits_inFlt;
    uint32_t *Bits_ouFlt;
}BOARD_DIO_STU, *PBOARD_DIO_STU;


typedef struct
{
    uint8_t lifesign;
    uint8_t armVer;
    uint8_t kwVer;
    uint8_t carID;
    
    uint8_t dc110v;
    uint8_t dc5v;
    CAN_NODE_STU flt;
    uint8_t res;
    
    uint8_t ou[8];
    uint32_t *Bits_ou;
}BOARD_MCU_STU, *PBOARD_MCU_STU;


typedef struct 
{
    uint8_t slotID;
    uint8_t boardType;
    uint8_t version;
    uint8_t offset;

    BOARD_DIO_STU DIO[3];
    BOARD_MCU_STU MCU[3];
}DS_STU,*PDS_STU;

extern DS_STU ds;

void userApp_init(void);
void GetSlotID(void);


#ifdef __cplusplus
}
#endif

#endif

