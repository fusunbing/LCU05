

#ifndef _USER_APP_H_
#define _USER_APP_H_

#ifdef __cplusplus
extern "C" {
#endif 

    
#include <rtthread.h>
#include "stm32f4xx.h"
	
#include "Bsp_led.h"

    



#define	SLOT_ID_MVB			(1)
#define	SLOT_ID_CAN			(2)
#define	SLOT_ID_ETU			(3)
#define	SLOT_ID_MCU_A		(4)
#define	SLOT_ID_MCU_B		(5)
#define	SLOT_ID_MCU_C		(6)
#define	SLOT_ID_IO_MIN		(7)
#define	SLOT_ID_IO_MAX		(33)

#define	BOARD_TYPE_ID_MVB	(1)
#define	BOARD_TYPE_ID_CAN	(3)
#define	BOARD_TYPE_ID_ETU	(4)
#define	BOARD_TYPE_ID_MCU	(2)
#define	BOARD_TYPE_ID_IO	(5)


//kw box id
#define	KW_BOX_ID_A1		(0x81)
#define	KW_BOX_ID_B1		(KW_BOX_ID_A1+1)
#define	KW_BOX_ID_C1		(KW_BOX_ID_A1+2)
#define	KW_BOX_ID_C2        (KW_BOX_ID_A1+3)
#define	KW_BOX_ID_B2        (KW_BOX_ID_A1+4)
#define	KW_BOX_ID_A2        (KW_BOX_ID_A1+5)
    
#define IO_GROUP_MAX        (9)
#define IO_BOARD_MAX        (IO_GROUP_MAX*3)
#define DI_COUNT_MAX        (IO_GROUP_MAX*12)
#define DO_COUNT_MAX        (IO_GROUP_MAX*7)

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
    uint8_t boxID;
    uint8_t version;
    
    uint32_t sts;
    uint32_t CAN1_STS;
	uint32_t CAN2_STS;
}BOARD_MVB, *PBOARD_MVB;


extern  BOARD_MVB mvb_board;


void userApp_init(void);


uint32_t* bitBand(uint32_t* addr, uint8_t bit_num);


#ifdef __cplusplus
}
#endif

#endif

