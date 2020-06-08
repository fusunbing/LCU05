

#ifndef _USER_APP_H_
#define _USER_APP_H_

#ifdef __cplusplus
extern "C" {
#endif 

    
#include <rtthread.h>
#include "stm32f4xx.h"
#include "Bsp_led.h"

//软件版本号
#define MAJOR_VERSION		(0) //主版本号
#define MINOR_VERSION		(5) //次版本号

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


//kw box id
#define	KW_BOX_ID_A1		(0x81)
#define	KW_BOX_ID_B1		(KW_BOX_ID_A1+1)
#define	KW_BOX_ID_C1		(KW_BOX_ID_A1+2)
#define	KW_BOX_ID_C2        (KW_BOX_ID_A1+3)
#define	KW_BOX_ID_B2        (KW_BOX_ID_A1+4)
#define	KW_BOX_ID_A2        (KW_BOX_ID_A1+5)

#define BOARD_IN_COUNT      (12)
#define BOARD_OU_COUNT      (7)
#define IO_GROUP_MAX        (9) //6U box
#define IO_BOARD_MAX        (IO_GROUP_MAX * 3)
#define DI_COUNT_MAX        (IO_GROUP_MAX * BOARD_IN_COUNT)
#define DO_COUNT_MAX        (IO_GROUP_MAX * BOARD_OU_COUNT)


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
    uint8_t A_DC5V          :1;     //A组电源故障
    uint8_t A_DC110V        :1;     //A组供电故障
    uint8_t B_DC5V          :1;     //B组电源故障
    uint8_t B_DC110V        :1;     //B组供电故障
    uint8_t C_DC5V          :1;     //C组电源故障
    uint8_t C_DC110V        :1;     //C组供电故障
    uint8_t res             :2;
}BOARD_PWR_FLT, *PBOARD_PWR_FLT;


typedef struct
{
    uint8_t ext_can1        :1;     //外网CAN1故障
    uint8_t ext_can2        :1;     //外网CAN2故障
    uint8_t can1            :1;     //内网CAN1故障
    uint8_t can2            :1;     //内网CAN2故障
    uint8_t res             :4;
}BOARD_CAN_FLT, *PBOARD_CAN_FLT;


typedef struct
{
    uint8_t mvb_module      :1;     //MVB板故障
    uint8_t mvb_can1        :1;     //MVB板CAN1故障
    uint8_t mvb_can2        :1;     //MVB板CAN2故障
    uint8_t res1            :1;        
    uint8_t eth_module      :1;     //ETH板故障
    uint8_t eth_can1        :1;     //ETH板CAN2故障
    uint8_t eth_can2        :1;     //ETH板CAN2故障
    uint8_t res2            :1;
}BOARD_NET_FLT, *PBOARD_NET_FLT;


typedef struct
{
    uint8_t board           :1;     //MCU板故障
    uint8_t can1            :1;     //MCU板CAN1故障
    uint8_t can2            :1;     //MCU板CAN2故障
    uint8_t lost            :1;     //MCU板丢失
    uint8_t res             :4;
}BOARD_MCU_FLT, *PBOARD_MCU_FLT;


typedef struct
{
    uint8_t              :1;     //DIO板故障
    uint8_t can1            :1;     //DIO板CAN1故障
    uint8_t can2            :1;     //DIO板CAN2故障
    uint8_t in              :1;     //DIO板输入故障
    uint8_t ou              :1;     //DIO板输出故障
    uint8_t lost            :1;     //DIO板丢失
    uint8_t res             :2;
}BOARD_DIO_FLT, *PBOARD_DIO_FLT;


typedef struct
{
    uint16_t addr;  //端口地址
    uint16_t cycle; //端口更新周期
    
    uint8_t index;  //端口号
    uint8_t len;    //端口数据长度：2，4，8，16，32
    uint8_t direct; //0：源端口，1：宿端口
    uint8_t flag;
    
    uint8_t data[32];
}MVB_PORT_INFO_STU, *PMVB_PORT_INFO_STU;


typedef struct
{
    uint8_t date[32];
}MVB_DATA_STU, *PMVB_DATA_STU;


//typedef struct 
//{
//    uint16_t lifeSignal;
//    BOARD_PWR_FLT pwr;
//    BOARD_CAN_FLT can;
//    
//    BOARD_NET_FLT net;
//    BOARD_MCU_FLT mcu[3];
//    
//    BOARD_DIO_FLT dio[27];
//    
//    uint8_t inBuf[18];
//    uint8_t ouBuf[9];
//    
//    uint16_t res;
//}CAN_DATA_STU, *PCAN_DATA_STU;


typedef struct
{
    uint8_t date[96];
}CAN_DATA_STU, *PCAN_DATA_STU;


typedef struct
{
    MVB_DATA_STU    mvb_port[32];   //MVB端口数据 %MB3.1000-%MB3.2023
    CAN_DATA_STU    lcu[16];        //各车LCU状态数据 %MB3.2024-%MB3.3559
    
    uint16_t lifeSign;
    uint8_t year;
    uint8_t mon;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    
    uint8_t info[16]; //
}KW_SHM_STU, *PKW_SHM_STU;


typedef struct 
{
    uint8_t lifesign;
    uint8_t version;
    BOARD_DIO_FLT flt;
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
    uint8_t armVersion;
    uint8_t kwVersion;
    uint8_t carID;
    
    uint8_t dc110v;
    uint8_t dc5v;
    BOARD_MCU_FLT flt;
    uint8_t res;
    
    uint8_t ou[8];
    uint32_t *Bits_ou;
}BOARD_MCU_STU, *PBOARD_MCU_STU;


typedef struct
{
    BOARD_DIO_STU DIO[IO_BOARD_MAX];
    BOARD_MCU_STU MCU[3];
    
    uint8_t inBuf[128];
    uint8_t ouBuf[128];
    uint32_t *Bits_inBuf;
    uint32_t *Bits_ouBuf;
    
    uint8_t carID;
    uint8_t slotID;
    uint8_t boardTypeID;    
    uint8_t fltLevel;       //故障等级

    uint8_t dc5v;
    uint8_t dc110v;
    uint8_t dc5v_flt;
    uint8_t dc110v_flt;
}DS_STU,*PDS_STU;


extern DS_STU ds;
extern PKW_SHM_STU pKW_SHM;


void userApp_init(void);
void Input_KW(uint8_t *buf);
void KW_Output(uint8_t *buf);


#ifdef __cplusplus
}
#endif

#endif

