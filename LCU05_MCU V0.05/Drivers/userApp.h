

#ifndef _USER_APP_H_
#define _USER_APP_H_

#ifdef __cplusplus
extern "C" {
#endif 


#include <rtthread.h>
#include "stm32f4xx.h"
#include "Bsp_led.h"


//软件版本号
#define MCU_VERSION         (5)     //MCU板底层软件版本号
#define KW_VERSION          (3)     //MCU板KW软件版本号
#define DIO_VERSION         (5)     //DIO板底层软件版本号

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

#define DIO_CNT_6U          (27)    //6U机箱DIO板数量
#define DIO_CNT_3U          (9)     //3U机箱DIO板数量

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
    uint8_t dc110v_a        :1;     //A组供电故障    
    uint8_t dc5v_a          :1;     //A组电源故障
    uint8_t dc110v_b        :1;     //B组供电故障
    uint8_t dc5v_b          :1;     //B组电源故障
    uint8_t dc110v_c        :1;     //C组供电故障    
    uint8_t dc5v_c          :1;     //C组电源故障
    uint8_t res             :2;
}BOARD_PWR_FLT, *PBOARD_PWR_FLT;


typedef struct
{
    uint8_t ser             :1;     //轻微故障
    uint8_t med             :1;     //中等故障
    uint8_t sli             :1;     //严重故障
    uint8_t res             :5;
}LCU_FLT_LEVEL, *PLCU_FLT_LEVEL;


typedef struct
{
    uint8_t mcuVer          :1;     //MCU板底层软件版本号
    uint8_t kwVer           :1;     //MCU板KW软件版本号
    uint8_t dioVer          :1;     //DIO板底层软件版本号
    uint8_t res             :5;
}LCU_VER_FLT, *PLCU_VER_FLT;


typedef struct
{
    uint8_t board           :1;     //MVB板故障
    uint8_t can1            :1;     //MVB板CAN1故障
    uint8_t can2            :1;     //MVB板CAN2故障
    uint8_t lost            :1;     //MVB板丢失
    uint8_t mc1             :1;     //MC1车TCMS生命信号丢失
    uint8_t mc2             :1;     //MC2车TCMS生命信号丢失
    uint8_t res             :2;
}BOARD_MVB_FLT, *PBOARD_MVB_FLT;


typedef struct
{
    uint8_t board           :1;     //CAN板故障
    uint8_t can1            :1;     //内网CAN1故障
    uint8_t can2            :1;     //内网CAN2故障
    uint8_t lost            :1;     //CAN板丢失
    
    uint8_t ext_can1        :1;     //外网CAN1故障
    uint8_t ext_can2        :1;     //外网CAN2故障
    uint8_t res             :2;
}BOARD_CAN_FLT, *PBOARD_CAN_FLT;


typedef struct
{      
    uint8_t board           :1;     //ETH板故障
    uint8_t can1            :1;     //ETH板CAN2故障
    uint8_t can2            :1;     //ETH板CAN2故障
    uint8_t lost            :1;     //ETU板丢失
    uint8_t eth             :1;     //ETU板以太网通讯故障
    uint8_t res             :4;
}BOARD_ETU_FLT, *PBOARD_ETU_FLT;


typedef struct
{
    uint8_t board           :1;     //MCU板故障
    uint8_t can1            :1;     //MCU板CAN1故障
    uint8_t can2            :1;     //MCU板CAN2故障
    uint8_t lost            :1;     //MCU板丢失
    uint8_t logic           :1;     //MCU板逻辑运算错误
    uint8_t remoteIn        :1;     //MCU板列车线信号错误
    uint8_t res             :2;
}BOARD_MCU_FLT, *PBOARD_MCU_FLT;


typedef struct
{
    uint8_t board           :1;     //DIO板故障
    uint8_t can1            :1;     //DIO板CAN1故障
    uint8_t can2            :1;     //DIO板CAN2故障
    uint8_t in              :1;     //DIO板输入故障
    uint8_t ou              :1;     //DIO板输出故障
    uint8_t lost            :1;     //DIO板丢失
    uint8_t res             :2;
}BOARD_DIO_FLT, *PBOARD_DIO_FLT;


typedef struct
{
    uint8_t can1            :1;     //机箱CAN1故障
    uint8_t can2            :1;     //机箱CAN2故障
    uint8_t lost            :1;     //机箱丢失
    uint8_t res             :5;
}BOX_FLT, *PBOX_FLT;


typedef struct
{
    uint8_t date[32];
}MVB_DATA_STU, *PMVB_DATA_STU;


typedef struct
{
    uint8_t date[128];
}CAN_DATA_STU, *PCAN_DATA_STU;


typedef struct 
{
    uint16_t lifeSignal; //生命信号
    
    LCU_FLT_LEVEL fltLevel;
    LCU_VER_FLT   verFlt;
    
    BOARD_PWR_FLT pwr;
    BOARD_CAN_FLT can;
    BOARD_MVB_FLT mvb;
    BOARD_ETU_FLT etu;
    BOARD_MCU_FLT mcu[3];
    
    BOARD_DIO_FLT dio[IO_BOARD_MAX];
    
    uint8_t inBuf[14];
    uint8_t ouBuf[8];
    
    BOX_FLT car[4];
}LCU_STS_STU, *PLCU_STS_STU;


typedef struct
{
    MVB_DATA_STU    mvb_port[32];   //MVB端口数据 %MB3.1000-%MB3.2023
    CAN_DATA_STU    lcu[8];        //各车LCU状态数据 %MB3.2024-%MB3.2047
    
    uint16_t lifeSign;
    uint8_t year;
    uint8_t mon;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t res1[24];

    uint8_t carID;
    uint8_t kwVer;
    uint8_t res2[30];

    LCU_STS_STU me;
}KW_SHM_STU, *PKW_SHM_STU;

typedef struct 
{
    uint8_t lifesign;
    uint8_t version;
    uint8_t carID;
    BOARD_DIO_FLT flt;
}BOARD_MVB_STU, *PBOARD_MVB_STU;


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
    uint8_t remote_in[8];
    uint32_t *Bits_ou;
    uint32_t *Bits_remote_in;
}BOARD_MCU_STU, *PBOARD_MCU_STU;


typedef struct
{
    BOARD_DIO_STU DIO[IO_BOARD_MAX];
    BOARD_MCU_STU MCU[3];
    
    uint8_t inBuf[128];
    uint8_t ouBuf[128];
    uint32_t *Bits_inBuf;
    uint32_t *Bits_ouBuf;
    uint8_t remoteIn[8];
    
    
    uint8_t carID;
    uint8_t slotID;
    uint8_t boardTypeID;    
    uint8_t fltLevel;       //故障等级

    uint8_t dc5v;
    uint8_t dc110v;
    uint8_t dc5v_flt;
    uint8_t dc110v_flt;
    
    uint8_t mvb_Lifesign;
    uint8_t mvb_Version;
    uint8_t mvb_CarID;
    uint8_t mvb_res;
    
    uint8_t can1_Lifesign;
    uint8_t can1_Version;
    uint8_t can1_CarID;
    uint8_t can1_ExtCan;
    
    uint8_t can2_Lifesign;
    uint8_t can2_Version;
    uint8_t can2_CarID;
    uint8_t can2_ExtCan;
    
    uint8_t etu_Lifesign;
    uint8_t etu_Version;
    uint8_t etu_CarID;
    uint8_t etu_res;

}DS_STU,*PDS_STU;


extern DS_STU ds;
extern PKW_SHM_STU pKW_SHM;


void userApp_init(void);
void Input_KW(uint8_t *buf);
void KW_Output(uint8_t *buf);
void remoteIn_2oo3(void);

#ifdef __cplusplus
}
#endif

#endif

