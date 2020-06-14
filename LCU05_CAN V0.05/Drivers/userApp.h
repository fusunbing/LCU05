

//add by fusunbing

#ifndef _USER_APP_H_
#define _USER_APP_H_

#ifdef __cplusplus
extern "C" {
#endif 

#include <rtthread.h>
#include "stm32f4xx.h"


#define SOFTWARE_VERSION    (10) 	//����汾��

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
    /* Word 0 */
    uint16_t res01          :1;     /* Ԥ�� */
    uint16_t res02          :1;     /* Ԥ�� */
    uint16_t pwr_c_5v       :1;     /* C���Դ���� */
    uint16_t pwr_c_110v     :1;     /* C�鹩��Ͽ� */
    uint16_t pwr_b_5v       :1;     /* B���Դ���� */
    uint16_t pwr_b_110v     :1;     /* B�鹩��Ͽ� */
    uint16_t pwr_a_5v       :1;     /* A���Դ���� */
    uint16_t pwr_a_110v     :1;     /* A�鹩��Ͽ� */
    
    uint16_t mcu_a_can2     :1;     /* ���ذ�A����CAN2���� */
    uint16_t mcu_a_lost     :1;     /* ���ذ�A��ʧ */
    uint16_t lcu_flt_ser    :1;     /* ���ع��� */
    uint16_t lcu_flt_med    :1;     /* �еȹ��� */
    uint16_t lcu_flt_sli    :1;     /* ��΢���� */
    uint16_t mvb_flt        :1;     /* MVB����� */
    uint16_t mvb_can1       :1;     /* MVB��CAN1���� */
    uint16_t mvb_can2       :1;     /* MVB��CAN2���� */
    
    /* Word 1 */
    uint16_t mcu_b_flt      :1;     /* ���ذ�B���� */
    uint16_t mcu_b_can1     :1;     /* ���ذ�B����CAN1���� */
    uint16_t mcu_b_can2     :1;     /* ���ذ�B����CAN2���� */
    uint16_t mcu_b_lost     :1;     /* ���ذ�B��ʧ */
    uint16_t res11          :1;     /* Ԥ�� */
    uint16_t mcu_a_ou       :1;     /* ���ذ�A������ϣ��˴﷽���� */
    uint16_t mcu_a_flt      :1;     /* ���ذ�B���� */
    uint16_t mcu_a_can1     :1;     /* ���ذ�A����CAN1���� */
    
    uint16_t res12          :1;     /* Ԥ�� */
    uint16_t mcu_c_ou       :1;     /* ���ذ�C������ϣ��˴﷽���� */
    uint16_t mcu_c_flt      :1;     /* ���ذ�C���� */
    uint16_t mcu_c_can1     :1;     /* ���ذ�C����CAN1���� */
    uint16_t mcu_c_can2     :1;     /* ���ذ�C����CAN2���� */
    uint16_t mcu_c_lost     :1;     /* ���ذ�C��ʧ */
    uint16_t res13          :1;     /* Ԥ�� */
    uint16_t mcu_b_ou       :1;     /* ���ذ�B������ϣ��˴﷽���� */
    
    /* Word 2 */
    uint16_t etu_can1       :1;     /* ETU�屳��CAN1���� */
    uint16_t etu_can2       :1;     /* ETU�屳��CAN1���� */
    uint16_t can_flt        :1;     /* CAN����� */
    uint16_t can_lost       :1;     /* CAN�嶪ʧ */
    uint16_t can_ext_can1   :1;     /* CAN���ذ�ǰ���CAN1���� */
    uint16_t can_ext_can2   :1;     /* CAN���ذ�ǰ���CAN2���� */
    uint16_t can_can1       :1;     /* CAN���ذ屳��CAN1���� */
    uint16_t can_can2       :1;     /* CAN���ذ屳��CAN2���� */
    
    uint16_t dio_ver        :1;     /* DIO����汾�Ų�һ�� */
    uint16_t mcu_kw_ver     :1;     /* MCUӦ�ò�����汾�Ų�һ�� */
    uint16_t mcu_ver        :1;     /* MCU�ײ�����汾�Ų�һ�� */
    uint16_t res21          :1;     /* Ԥ�� */
    uint16_t res22          :1;     /* Ԥ�� */
    uint16_t etu_lost       :1;     /* ETU�嶪ʧ */
    uint16_t etu_eth        :1;     /* ETU����̫��ͨѶ���� */
    uint16_t etu_flt        :1;     /* ETU����� */
    
    /* Word 3 */
    uint16_t io1_a_flt      :1;     /* IO1��A���� */
    uint16_t io1_a_can1     :1;     /* IO1��A����CAN1���� */
    uint16_t io1_a_can2     :1;     /* IO1��A����CAN1���� */
    uint16_t io1_a_in       :1;     /* IO1��A������� */
    uint16_t io1_a_ou       :1;     /* IO1��A������� */
    uint16_t io1_a_lost     :1;     /* IO1��A��ʧ */
    uint16_t res31          :1;     /* Ԥ�� */
    uint16_t gio_ver        :1;     /* GIO����汾�Ų�һ�£��˴﷽���� */
    
    uint16_t res32          :1;     /* Ԥ�� */
    uint16_t io1_b_flt      :1;     /* IO1��B���� */
    uint16_t io1_b_can1     :1;     /* IO1��B����CAN1���� */
    uint16_t io1_b_can2     :1;     /* IO1��B����CAN1���� */
    uint16_t io1_b_in       :1;     /* IO1��B������� */
    uint16_t io1_b_ou       :1;     /* IO1��B������� */
    uint16_t io1_b_lost     :1;     /* IO1��B��ʧ */
    uint16_t res33          :1;     /* Ԥ�� */
    
    /* Word 4 */
    uint16_t io2_a_lost     :1;     /* IO2��A��ʧ */
    uint16_t res41          :1;     /* Ԥ�� */
    uint16_t io1_c_flt      :1;     /* IO1��C���� */
    uint16_t io1_c_can1     :1;     /* IO1��C����CAN1���� */
    uint16_t io1_c_can2     :1;     /* IO1��C����CAN1���� */
    uint16_t io1_c_in       :1;     /* IO1��C������� */
    uint16_t io1_c_ou       :1;     /* IO1��C������� */
    uint16_t io1_c_lost     :1;     /* IO1��C��ʧ */
    
    uint16_t io2_b_ou       :1;     /* IO2��B������� */
    uint16_t io2_b_lost     :1;     /* IO2��B��ʧ */
    uint16_t res42          :1;     /* Ԥ�� */    
    uint16_t io2_a_flt      :1;     /* IO2��A���� */    
    uint16_t io2_a_can1     :1;     /* IO2��A����CAN1���� */
    uint16_t io2_a_can2     :1;     /* IO2��A����CAN1���� */
    uint16_t io2_a_in       :1;     /* IO2��A������� */
    uint16_t io2_a_ou       :1;     /* IO2��A������� */
    
    /* Word 5 */
    uint16_t io2_c_in       :1;     /* IO2��C������� */
    uint16_t io2_c_ou       :1;     /* IO2��C������� */
    uint16_t io2_c_lost     :1;     /* IO2��C��ʧ */    
    uint16_t res51          :1;     /* Ԥ�� */    
    uint16_t io2_b_flt      :1;     /* IO2��B���� */
    uint16_t io2_b_can1     :1;     /* IO2��B����CAN1���� */
    uint16_t io2_b_can2     :1;     /* IO2��B����CAN1���� */
    uint16_t io2_b_in       :1;     /* IO2��B������� */
    
    uint16_t io3_a_can2     :1;     /* IO3��A����CAN1���� */
    uint16_t io3_a_in       :1;     /* IO3��A������� */
    uint16_t io3_a_ou       :1;     /* IO3��A������� */
    uint16_t io3_a_lost     :1;     /* IO3��A��ʧ */
    uint16_t res52          :1;     /* Ԥ�� */    
    uint16_t io2_c_flt      :1;     /* IO2��C���� */
    uint16_t io2_c_can1     :1;     /* IO2��C����CAN1���� */
    uint16_t io2_c_can2     :1;     /* IO2��C����CAN1���� */
    
    /* Word 6 */
    uint16_t io3_b_can1     :1;     /* IO3��B����CAN1���� */
    uint16_t io3_b_can2     :1;     /* IO3��B����CAN1���� */
    uint16_t io3_b_in       :1;     /* IO3��B������� */
    uint16_t io3_b_ou       :1;     /* IO3��B������� */
    uint16_t io3_b_lost     :1;     /* IO3��B��ʧ */
    uint16_t res61          :1;     /* Ԥ�� */    
    uint16_t io3_a_flt      :1;     /* IO3��A���� */
    uint16_t io3_a_can1     :1;     /* IO3��A����CAN1���� */

    uint16_t io3_c_flt      :1;     /* IO3��C���� */
    uint16_t io3_c_can1     :1;     /* IO3��C����CAN1���� */
    uint16_t io3_c_can2     :1;     /* IO3��C����CAN1���� */
    uint16_t io3_c_in       :1;     /* IO3��C������� */
    uint16_t io3_c_ou       :1;     /* IO3��C������� */
    uint16_t io3_c_lost     :1;     /* IO3��C��ʧ */    
    uint16_t res62          :1;     /* Ԥ�� */    
    uint16_t io3_b_flt      :1;     /* IO3��B���� */
    
    /* Word 7 */
    uint16_t res71          :1;     /* Ԥ�� */    
    uint16_t res72          :1;     /* Ԥ�� */    
    uint16_t res73          :1;     /* Ԥ�� */    
    uint16_t res74          :1;     /* Ԥ�� */    
    uint16_t res75          :1;     /* Ԥ�� */    
    uint16_t res76          :1;     /* Ԥ�� */    
    uint16_t res77          :1;     /* Ԥ�� */    
    uint16_t res78          :1;     /* Ԥ�� */
    
    uint16_t res79          :1;     /* Ԥ�� */
    uint16_t io4_a_flt      :1;     /* IO4��A���� */
    uint16_t io4_a_can1     :1;     /* IO4��A����CAN1���� */
    uint16_t io4_a_can2     :1;     /* IO4��A����CAN1���� */
    uint16_t io4_a_in       :1;     /* IO4��A������� */
    uint16_t io4_a_ou       :1;     /* IO4��A������� */
    uint16_t io4_a_lost     :1;     /* IO4��A��ʧ */
    uint16_t res710         :1;     /* Ԥ�� */
    
    /* Word 8 */
    uint16_t io4_c_lost     :1;     /* IO4��C��ʧ */    
    uint16_t res81          :1;     /* Ԥ�� */
    uint16_t io4_b_flt      :1;     /* IO4��B���� */
    uint16_t io4_b_can1     :1;     /* IO4��B����CAN1���� */
    uint16_t io4_b_can2     :1;     /* IO4��B����CAN1���� */
    uint16_t io4_b_in       :1;     /* IO4��B������� */
    uint16_t io4_b_ou       :1;     /* IO4��B������� */
    uint16_t io4_b_lost     :1;     /* IO4��B��ʧ */
    
    uint16_t io5_a_ou       :1;     /* IO5��A������� */
    uint16_t io5_a_lost     :1;     /* IO5��A��ʧ */
    uint16_t res82          :1;     /* Ԥ�� */
    uint16_t io4_c_flt      :1;     /* IO4��C���� */
    uint16_t io4_c_can1     :1;     /* IO4��C����CAN1���� */
    uint16_t io4_c_can2     :1;     /* IO4��C����CAN1���� */
    uint16_t io4_c_in       :1;     /* IO4��C������� */
    uint16_t io4_c_ou       :1;     /* IO4��C������� */
    
    /* Word 9 */
    uint16_t io5_b_in       :1;     /* IO5��B������� */
    uint16_t io5_b_ou       :1;     /* IO5��B������� */
    uint16_t io5_b_lost     :1;     /* IO5��B��ʧ */
    uint16_t res91          :1;     /* Ԥ�� */
    uint16_t io5_a_flt      :1;     /* IO5��A���� */
    uint16_t io5_a_can1     :1;     /* IO5��A����CAN1���� */
    uint16_t io5_a_can2     :1;     /* IO5��A����CAN1���� */
    uint16_t io5_a_in       :1;     /* IO5��A������� */
    
    uint16_t io5_c_can2     :1;     /* IO5��C����CAN1���� */
    uint16_t io5_c_in       :1;     /* IO5��C������� */
    uint16_t io5_c_ou       :1;     /* IO5��C������� */
    uint16_t io5_c_lost     :1;     /* IO5��C��ʧ */    
    uint16_t res92          :1;     /* Ԥ�� */
    uint16_t io5_b_flt      :1;     /* IO5��B���� */
    uint16_t io5_b_can1     :1;     /* IO5��B����CAN1���� */
    uint16_t io5_b_can2     :1;     /* IO5��B����CAN1���� */
    
    /* Word 10 */
    uint16_t io6_a_can1     :1;     /* IO6��A����CAN1���� */
    uint16_t io6_a_can2     :1;     /* IO6��A����CAN1���� */
    uint16_t io6_a_in       :1;     /* IO6��A������� */
    uint16_t io6_a_ou       :1;     /* IO6��A������� */
    uint16_t io6_a_lost     :1;     /* IO6��A��ʧ */
    uint16_t res101         :1;     /* Ԥ�� */
    uint16_t io5_c_flt      :1;     /* IO5��C���� */
    uint16_t io5_c_can1     :1;     /* IO5��C����CAN1���� */
    
    uint16_t io6_b_flt      :1;     /* IO6��B���� */
    uint16_t io6_b_can1     :1;     /* IO6��B����CAN1���� */
    uint16_t io6_b_can2     :1;     /* IO6��B����CAN1���� */
    uint16_t io6_b_in       :1;     /* IO6��B������� */
    uint16_t io6_b_ou       :1;     /* IO6��B������� */
    uint16_t io6_b_lost     :1;     /* IO6��B��ʧ */    
    uint16_t res102         :1;     /* Ԥ�� */
    uint16_t io6_a_flt      :1;     /* IO6��A���� */
    
    /* Word 11 */
    uint16_t res111         :1;     /* Ԥ�� */
    uint16_t io6_c_flt      :1;     /* IO6��C���� */
    uint16_t io6_c_can1     :1;     /* IO6��C����CAN1���� */
    uint16_t io6_c_can2     :1;     /* IO6��C����CAN1���� */
    uint16_t io6_c_in       :1;     /* IO6��C������� */
    uint16_t io6_c_ou       :1;     /* IO6��C������� */
    uint16_t io6_c_lost     :1;     /* IO6��C��ʧ */    
    uint16_t res112         :1;     /* Ԥ�� */
    
    uint16_t io7_b_lost     :1;     /* IO7��B��ʧ */
    uint16_t res113         :1;     /* Ԥ�� */
    uint16_t io7_a_flt      :1;     /* IO7��A���� */
    uint16_t io7_a_can1     :1;     /* IO7��A����CAN1���� */
    uint16_t io7_a_can2     :1;     /* IO7��A����CAN1���� */
    uint16_t io7_a_in       :1;     /* IO7��A������� */
    uint16_t io7_a_ou       :1;     /* IO7��A������� */
    uint16_t io7_a_lost     :1;     /* IO7��A��ʧ */
   
    /* Word 12 */
    uint16_t io7_c_ou       :1;     /* IO7��C������� */
    uint16_t io7_c_lost     :1;     /* IO7��C��ʧ */    
    uint16_t res121         :1;     /* Ԥ�� */
    uint16_t io7_b_flt      :1;     /* IO7��B���� */
    uint16_t io7_b_can1     :1;     /* IO7��B����CAN1���� */
    uint16_t io7_b_can2     :1;     /* IO7��B����CAN1���� */
    uint16_t io7_b_in       :1;     /* IO7��B������� */
    uint16_t io7_b_ou       :1;     /* IO7��B������� */
    
    uint16_t io8_a_in       :1;     /* IO8��A������� */
    uint16_t io8_a_ou       :1;     /* IO8��A������� */
    uint16_t io8_a_lost     :1;     /* IO8��A��ʧ */
    uint16_t res122         :1;     /* Ԥ�� */
    uint16_t io7_c_flt      :1;     /* IO7��C���� */
    uint16_t io7_c_can1     :1;     /* IO7��C����CAN1���� */
    uint16_t io7_c_can2     :1;     /* IO7��C����CAN1���� */
    uint16_t io7_c_in       :1;     /* IO7��C������� */
    
    /* Word 13 */    
    uint16_t io8_b_can2     :1;     /* IO8��B����CAN1���� */
    uint16_t io8_b_in       :1;     /* IO8��B������� */
    uint16_t io8_b_ou       :1;     /* IO8��B������� */
    uint16_t io8_b_lost     :1;     /* IO8��B��ʧ */
    uint16_t res131         :1;     /* Ԥ�� */    
    uint16_t io8_a_flt      :1;     /* IO8��A���� */
    uint16_t io8_a_can1     :1;     /* IO8��A����CAN1���� */
    uint16_t io8_a_can2     :1;     /* IO8��A����CAN1���� */
    
    uint16_t io8_c_can1     :1;     /* IO8��C����CAN1���� */
    uint16_t io8_c_can2     :1;     /* IO8��C����CAN1���� */
    uint16_t io8_c_in       :1;     /* IO8��C������� */
    uint16_t io8_c_ou       :1;     /* IO8��C������� */
    uint16_t io8_c_lost     :1;     /* IO8��C��ʧ */
    uint16_t res132         :1;     /* Ԥ�� */   
    uint16_t io8_b_flt      :1;     /* IO8��B���� */
    uint16_t io8_b_can1     :1;     /* IO8��B����CAN1���� */
    
    /* Word 14 */
    uint16_t io9_a_flt      :1;     /* IO9��A���� */
    uint16_t io9_a_can1     :1;     /* IO9��A����CAN1���� */
    uint16_t io9_a_can2     :1;     /* IO9��A����CAN1���� */
    uint16_t io9_a_in       :1;     /* IO9��A������� */
    uint16_t io9_a_ou       :1;     /* IO9��A������� */
    uint16_t io9_a_lost     :1;     /* IO9��A��ʧ */    
    uint16_t res141         :1;     /* Ԥ�� */    
    uint16_t io8_c_flt      :1;     /* IO8��C���� */
    
    uint16_t res142         :1;     /* Ԥ�� */
    uint16_t io9_b_flt      :1;     /* IO9��B���� */
    uint16_t io9_b_can1     :1;     /* IO9��B����CAN1���� */
    uint16_t io9_b_can2     :1;     /* IO9��B����CAN1���� */
    uint16_t io9_b_in       :1;     /* IO9��B������� */
    uint16_t io9_b_ou       :1;     /* IO9��B������� */
    uint16_t io9_b_lost     :1;     /* IO9��B��ʧ */    
    uint16_t res143         :1;     /* Ԥ�� */    
    
    /* Word 15 */
    uint16_t car_mand_ou    :1;     /* ˾����ռ��������� */    
    uint16_t res151         :1;     /* Ԥ�� */    
    uint16_t io9_c_flt      :1;     /* IO9��C���� */
    uint16_t io9_c_can1     :1;     /* IO9��C����CAN1���� */
    uint16_t io9_c_can2     :1;     /* IO9��C����CAN1���� */
    uint16_t io9_c_in       :1;     /* IO9��C������� */
    uint16_t io9_c_ou       :1;     /* IO9��C������� */
    uint16_t io9_c_lost     :1;     /* IO9��C��ʧ */    
    
    uint16_t res152         :1;     /* Ԥ�� */    
    uint16_t res153         :1;     /* Ԥ�� */    
    uint16_t res154         :1;     /* Ԥ�� */    
    uint16_t res155         :1;     /* Ԥ�� */    
    uint16_t res156         :1;     /* Ԥ�� */    
    uint16_t res157         :1;     /* Ԥ�� */    
    uint16_t res158         :1;     /* Ԥ�� */    
    uint16_t res159         :1;     /* Ԥ�� */    
}LCU_FAULT_STU, *PLCU_FAULT_STU;


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
    
    uint32_t can1_sts;  //����״̬
    uint32_t can2_sts;  //����״̬
    
    CAN_DATE_STU lcu[8];
}DS_STU,*PDS_STU;


extern  DS_STU ds;

void userApp_init(void);
uint32_t* bitBand(uint32_t* addr);


#ifdef __cplusplus
}
#endif

#endif

