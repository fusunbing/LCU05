
#ifndef __COMMON_H__
#define __COMMON_H__

#include "stm32f4xx.h"

//MCU组播地址 
#define MCU_BROADCAST_ADDR (0x30)

//槽位ID
#define IO_SLOT_ID_MIN     (7)
#define IO_SLOT_ID_MAX     (33)
#define MCUA_SLOT_ID       (4)
#define MCUC_SLOT_ID       (6)
#define ETH_SLOT_ID        (3)
#define CAN_SLOT_ID        (2)
#define MVB_SLOT_ID        (1)

//功能号
#define POWER_ON_CMD       (0x10)
#define INPUT_SEND_CMD     (0x01)
#define OUTPUT_RECEIVE_CMD  (0X02)


#define MCU_BOARD_COUNT      (3)
#define MCU_BOARD_A          (0)
#define MCU_BOARD_B          (1)
#define MCU_BOARD_C          (2)

//can消息优先级


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
	  uint8_t boxID;
    uint8_t version;
    
    uint32_t sts;
    uint32_t CAN1_STS;
	  uint32_t CAN2_STS;
	
	  uint16_t inputOriginal;
	  uint16_t inputFilter;
	  uint16_t inputold;
	  uint8_t  output;
	  uint16_t outfbOriginal;
	  uint16_t outfbFilter;
	
	  uint8_t MCU_online_cnt[3];   //是否需要初始化为6？
}DS_STU,*PDS_STU;


extern DS_STU ds;


void Board_Information_Init(void);


#endif



