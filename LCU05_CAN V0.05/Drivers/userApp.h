

//add by fusunbing

#ifndef _USER_APP_H_
#define _USER_APP_H_

#ifdef __cplusplus
extern "C" {
#endif 

#include <rtthread.h>
#include "stm32f4xx.h"


#define SOFTWARE_VERSION    (5) 	//Èí¼þ°æ±¾ºÅ

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
    uint16_t lifesign;
    uint8_t rtc[8];
    uint8_t remoteIn[8];
    
    uint8_t devSts[32];
    uint8_t ioSts[32];
}CAN_DATE_STU,*PCAN_DATE_STU;


typedef struct
{
    uint8_t carID;
    uint8_t slotID;
    uint8_t boardTypeID;
    uint8_t version;
    
    uint32_t can1_sts;  //ÄÚÍø×´Ì¬
    uint32_t can2_sts;  //ÍâÍø×´Ì¬
    
    CAN_DATE_STU lcu[8];
}DS_STU,*PDS_STU;


extern  DS_STU ds;

void userApp_init(void);
uint32_t* bitBand(uint32_t* addr);


#ifdef __cplusplus
}
#endif

#endif

