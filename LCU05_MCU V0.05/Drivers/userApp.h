

#ifndef _USER_APP_H_
#define _USER_APP_H_

#ifdef __cplusplus
extern "C" {
#endif 


#include <rtthread.h>
#include "stm32f4xx.h"
#include "Bsp_led.h"


//����汾��
#define MAJOR_VERSION		(0) //���汾��
#define MINOR_VERSION		(5) //�ΰ汾��

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
    uint8_t dc110v_a        :1;     //A�鹩�����    
    uint8_t dc5v_a          :1;     //A���Դ����
    uint8_t dc110v_b        :1;     //B�鹩�����
    uint8_t dc5v_b          :1;     //B���Դ����
    uint8_t dc110v_c        :1;     //C�鹩�����    
    uint8_t dc5v_c          :1;     //C���Դ����
    uint8_t res             :2;
}BOARD_PWR_FLT, *PBOARD_PWR_FLT;


typedef struct
{
    uint8_t ser             :1;     //��΢����
    uint8_t med             :1;     //�еȹ���
    uint8_t sli             :1;     //���ع���
    uint8_t res1            :5;
}LCU_FLT_LEVEL, *PLCU_FLT_LEVEL;


typedef struct
{
    uint8_t mcuVer          :1;     //MCU��ײ�����汾��
    uint8_t kwVer           :1;     //MCU��KW����汾��
    uint8_t dioVer          :1;     //DIO��ײ�����汾��
    uint8_t res             :5;
}LCU_VER_FLT, *PLCU_VER_FLT;


typedef struct
{
    uint8_t board           :1;     //MVB�����
    uint8_t can1            :1;     //MVB��CAN1����
    uint8_t can2            :1;     //MVB��CAN2����
    uint8_t lost            :1;     //MVB�嶪ʧ
    uint8_t res             :4;
}BOARD_MVB_FLT, *PBOARD_MVB_FLT;


typedef struct
{
    uint8_t board           :1;     //CAN�����
    uint8_t can1            :1;     //����CAN1����
    uint8_t can2            :1;     //����CAN2����
    uint8_t lost            :1;     //CAN�嶪ʧ
    
    uint8_t ext_can1        :1;     //����CAN1����
    uint8_t ext_can2        :1;     //����CAN2����
    uint8_t res             :2;
}BOARD_CAN_FLT, *PBOARD_CAN_FLT;


typedef struct
{      
    uint8_t board           :1;     //ETH�����
    uint8_t can1            :1;     //ETH��CAN2����
    uint8_t can2            :1;     //ETH��CAN2����
    uint8_t lost            :1;     //ETU�嶪ʧ
    uint8_t eth             :1;     //ETU����̫��ͨѶ����
    uint8_t res             :4;
}BOARD_ETU_FLT, *PBOARD_ETU_FLT;


typedef struct
{
    uint8_t board           :1;     //MCU�����
    uint8_t can1            :1;     //MCU��CAN1����
    uint8_t can2            :1;     //MCU��CAN2����
    uint8_t lost            :1;     //MCU�嶪ʧ
    uint8_t res             :4;
}BOARD_MCU_FLT, *PBOARD_MCU_FLT;


typedef struct
{
    uint8_t board           :1;     //DIO�����
    uint8_t can1            :1;     //DIO��CAN1����
    uint8_t can2            :1;     //DIO��CAN2����
    uint8_t in              :1;     //DIO���������
    uint8_t ou              :1;     //DIO���������
    uint8_t lost            :1;     //DIO�嶪ʧ
    uint8_t res             :2;
}BOARD_DIO_FLT, *PBOARD_DIO_FLT;


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
    uint16_t lifeSignal; //�����ź�
    
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
    
    uint32_t res;
}LCU_STS_STU, *PLCU_STS_STU;


typedef struct
{
    MVB_DATA_STU    mvb_port[32];   //MVB�˿����� %MB3.1000-%MB3.2023
    CAN_DATA_STU    lcu[8];        //����LCU״̬���� %MB3.2024-%MB3.2047
    
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
    
    uint8_t res3[1024];
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
    uint8_t fltLevel;       //���ϵȼ�

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


#ifdef __cplusplus
}
#endif

#endif

