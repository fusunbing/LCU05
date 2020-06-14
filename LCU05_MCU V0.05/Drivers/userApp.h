

#ifndef _USER_APP_H_
#define _USER_APP_H_

#ifdef __cplusplus
extern "C" {
#endif 


#include <rtthread.h>
#include "stm32f4xx.h"
#include "Bsp_led.h"


//»Ìº˛∞Ê±æ∫≈
#define MAJOR_VERSION		(0) //÷˜∞Ê±æ∫≈
#define MINOR_VERSION		(5) //¥Œ∞Ê±æ∫≈

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
    uint8_t A_DC5V          :1;     //A◊ÈµÁ‘¥π ’œ
    uint8_t A_DC110V        :1;     //A◊Èπ©µÁπ ’œ
    uint8_t B_DC5V          :1;     //B◊ÈµÁ‘¥π ’œ
    uint8_t B_DC110V        :1;     //B◊Èπ©µÁπ ’œ
    uint8_t C_DC5V          :1;     //C◊ÈµÁ‘¥π ’œ
    uint8_t C_DC110V        :1;     //C◊Èπ©µÁπ ’œ
    uint8_t res             :2;
}BOARD_PWR_FLT, *PBOARD_PWR_FLT;


typedef struct
{
    uint8_t board           :1;     //MVB∞Âπ ’œ
    uint8_t can1            :1;     //MVB∞ÂCAN1π ’œ
    uint8_t can2            :1;     //MVB∞ÂCAN2π ’œ
    uint8_t lost            :1;     //MVB∞Â∂™ ß
    uint8_t res             :4;        
}BOARD_MVB_FLT, *PBOARD_MVB_FLT;


typedef struct
{
    uint8_t ext_can1        :1;     //Õ‚Õ¯CAN1π ’œ
    uint8_t ext_can2        :1;     //Õ‚Õ¯CAN2π ’œ
    uint8_t can1            :1;     //ƒ⁄Õ¯CAN1π ’œ
    uint8_t can2            :1;     //ƒ⁄Õ¯CAN2π ’œ
    uint8_t res             :4;
}BOARD_CAN_FLT, *PBOARD_CAN_FLT;


typedef struct
{      
    uint8_t board           :1;     //ETH∞Âπ ’œ
    uint8_t can1            :1;     //ETH∞ÂCAN2π ’œ
    uint8_t can2            :1;     //ETH∞ÂCAN2π ’œ
    uint8_t lost            :1;     //ETU∞Â∂™ ß
    uint8_t res             :4;
}BOARD_ETU_FLT, *PBOARD_ETU_FLT;


typedef struct
{
    uint8_t board           :1;     //MCU∞Âπ ’œ
    uint8_t can1            :1;     //MCU∞ÂCAN1π ’œ
    uint8_t can2            :1;     //MCU∞ÂCAN2π ’œ
    uint8_t lost            :1;     //MCU∞Â∂™ ß
    uint8_t res             :4;
}BOARD_MCU_FLT, *PBOARD_MCU_FLT;


typedef struct
{
    uint8_t board           :1;     //DIO∞Âπ ’œ
    uint8_t can1            :1;     //DIO∞ÂCAN1π ’œ
    uint8_t can2            :1;     //DIO∞ÂCAN2π ’œ
    uint8_t in              :1;     //DIO∞Â ‰»Îπ ’œ
    uint8_t ou              :1;     //DIO∞Â ‰≥ˆπ ’œ
    uint8_t lost            :1;     //DIO∞Â∂™ ß
    uint8_t res             :2;
}BOARD_DIO_FLT, *PBOARD_DIO_FLT;


typedef struct
{
    uint8_t date[32];
}MVB_DATA_STU, *PMVB_DATA_STU;


typedef struct 
{
    uint16_t lifeSignal; //…˙√¸–≈∫≈
    
    BOARD_PWR_FLT pwr;
    BOARD_CAN_FLT can;    
    BOARD_MVB_FLT mvb;
    BOARD_ETU_FLT etu;
    
    BOARD_MCU_FLT mcu[3];
    
    BOARD_DIO_FLT dio[27];
    
    uint8_t inBuf[14];
    uint8_t ouBuf[8];
    
    uint16_t res;
}CAN_DATA_STU, *PCAN_DATA_STU;


typedef struct
{
    MVB_DATA_STU    mvb_port[32];   //MVB∂Àø⁄ ˝æ› %MB3.1000-%MB3.2023
    CAN_DATA_STU    lcu[16];        //∏˜≥µLCU◊¥Ã¨ ˝æ› %MB3.2024-%MB3.3559
    
    uint16_t lifeSign;
    uint8_t year;
    uint8_t mon;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    
    uint8_t carID;
    uint8_t kwVer;
    
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
    
    uint8_t carID;
    uint8_t slotID;
    uint8_t boardTypeID;    
    uint8_t fltLevel;       //π ’œµ»º∂

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

