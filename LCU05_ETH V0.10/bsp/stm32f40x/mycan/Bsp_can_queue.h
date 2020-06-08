

#ifndef __BSP_CAN_QUEUE_H__
#define __BSP_CAN_QUEUE_H__


#include <rthw.h>
#include <rtthread.h>
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>


//------------------------------------------------------------------------
// CAN队列为空
//------------------------------------------------------------------------
#define CAN_QUEUE_NULL                  (0)

//------------------------------------------------------------------------
// 队列初始化标志寄存器
//------------------------------------------------------------------------
#define CAN_INIT_NOT                    (0x00)
#define CAN_INIT_OK                     (0x01)

//------------------------------------------------------------------------
// FIFO操作标志寄存器
//------------------------------------------------------------------------
#define CAN_FIFO_EMPTY_MASK             (0x00)
#define CAN_FIFO_FULL_MASK              (0x01)


//CAN comm pri
#define CAN_PRI_VH                      (0x00)
#define CAN_PRI_H                       (0x01)
#define CAN_PRI_M                       (0x02)
#define CAN_PRI_L                       (0x04)


//
#define CAN_FUN_SUB_SEQ                 (0x01)
#define CAN_FUN_SUB_ACK                 (0x02)
#define CAN_FUN_MAIN_SEQ                (0x03)
#define CAN_FUN_MAIN_ACK                (0x04)

#define CAN_FUN_CYCLE_OUTPUT_SEQ        (0x20)
#define CAN_FUN_CYCLE_INPUT_ACK         (0x21)
#define CAN_FUN_EVENT_INPUT_ACK         (0x22)
#define CAN_FUN_EVENT_INPUT_SEQ         (0x23)

#define CAN_FUN_CYCLE_STS_SEQ           (0x24)
#define CAN_FUN_CYCLE_STS_ACK           (0x25)

#define CAN_FUN_EVENT_IN_FLT_ACK        (0x80)
#define CAN_FUN_EVENT_IN_FLT_SEQ        (0x81)
#define CAN_FUN_EVENT_OU_FLT_ACK        (0x82)
#define CAN_FUN_EVENT_OU_FLT_SEQ        (0x83)


#define CAN2_MANAGE_SUB_SCAN            (0x00)
#define CAN1_MANAGE_MAIN_SCAN           (0x01)
#define CAN2_MANAGE_SUB_NORM            (0x02)
#define CAN1_MANAGE_MAIN_NORM           (0x03)


#define GROUP_TYPE_A        1
#define GROUP_TYPE_B        2
#define GROUP_TYPE_C        3

#define GROUP_TYPE_MAX      3
#define GROUP_INDEX_MAX     4

//------------------------------------------------------------------------
// Queue队列每条信息数据结构体
//------------------------------------------------------------------------
typedef struct
{
	uint32_t  Tick;
	uint16_t  CanPort;
	uint16_t  BoardId;
}CAN_MARK, *PCAN_MARK;


typedef union
{
    struct
    {
        uint32_t funID      :9;     //destination address
        uint32_t src        :8;     //source address
        uint32_t dst        :8;     //function id
        uint32_t pri        :4;     //priority
        uint32_t res        :3;
    } id;
    uint32_t value;
}CAN_EXTID_INFO, *PCAN_EXTID_INFO;


typedef struct
{
    CAN_MARK        parent;
    CanRxMsg        rxMsg;
    CAN_EXTID_INFO  msg_Id;
}CAN_RX_DATA_RAM, *PCAN_RX_DATA_RAM;


typedef struct
{
    CAN_MARK     parent;
    CanTxMsg     txMsg;
}CAN_TX_DATA_RAM, *PCAN_TX_DATA_RAM;


//------------------------------------------------------------------------
// Queue队列维护结构体
//------------------------------------------------------------------------
typedef struct    
{
    uint32_t    QEntriesMax;
    uint32_t   *QStart;                          
    uint32_t   *QEnd;            /* Ptr to end   of finsh Q data                            */
    uint32_t   *QIn;             /* Ptr where next msg will be inserted  in   finsh Q       */
    uint32_t   *QOut;            /* Ptr where next msg will be extracted from finsh Q       */
    uint32_t    QSize;           /* Size of finsh Q (maximum number of entries)             */
    uint32_t    QEntries;        /* Current number of entries in the finsh Q                */

}CAN_Q;


typedef union
{
    struct
    {
        uint8_t can1_main       :1;
        uint8_t can2_sub        :1;
        uint8_t sci_AB          :1;
        uint8_t sci_BC          :1;
        uint8_t sci_CA          :1;
        uint8_t online_A        :1;
        uint8_t online_B        :1;
        uint8_t online_C        :1;
    } sts;
    
    uint8_t value;
    
}LINK_STS, *PLINK_STS;


typedef union
{
    struct
    {
        uint8_t date_A          :1;        
        uint8_t SW_A            :1;        
        uint8_t date_B          :1;      
        uint8_t SW_B            :1;        
        uint8_t date_C          :1;  
        uint8_t SW_C            :1;        
        uint8_t res             :2;
    } io;
    uint8_t value;
}REMOTE_IO_STS, *PREMOTE_IO_STS;


typedef union
{
    struct
    {
        uint8_t arbFlt_A        :1;
        uint8_t arbFlt_B        :1;
        uint8_t arbFlt_C        :1;
        uint8_t res             :5;
    } io;
    uint8_t value;
}REMOTE_IO_FLT, *PREMOTE_IO_FLT;


//typedef struct 
//{
//    //uint8_t boardType;
//    //uint8_t boardIndex;
//    //uint8_t boardGroup;    
//    
//    uint8_t slotID;
//    LINK_STS link;
//    REMOTE_IO_STS sts;
//    REMOTE_IO_FLT flt;
//    
//    uint8_t in[4];
//    uint8_t ou[4];
//    uint8_t fb[4];

//    uint8_t fltIn[4];
//    uint8_t fltOu[4];
//    uint8_t fltSts[4];
//    
//}REMOTE_BOARD_STATE, *PREMOTE_BOARD_STATE;


#endif 

