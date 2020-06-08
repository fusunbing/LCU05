#ifndef _MVB_CFG_H
#define _MVB_CFG_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "rtthread.h"
#include "stm32f4xx.h"

#define BSP_MVB_SERVICE_STACKSIZE   (1 * 1024)  //1.0K  32bit pack
#define BSP_MVB_SERVICE_PRIORITY    (0x12)
#define MVB_PORT_ADDR_NUM           (32)
#define MVB_PORT_NUM_MAX            (32)
#define MVB_PORT_SIZE_MAX           (32)
#define MVB_PORT_TYPE_SINK          (1)     //���ն˿�
#define MVB_PORT_TYPE_SRCE          (2)     //���Ͷ˿�

/*
���ں�����
*/
#define CAR_A1           (0x00)
#define CAR_B1           (0x01)
#define CAR_C1           (0x02)
#define CAR_C2           (0x03)
#define CAR_B2           (0x04)
#define CAR_A2           (0x05)


/*
MVB״̬����
*/
#define MVB_POWER_ON     (0x00)
#define MVB_NOMAL        (0x01)
#define MVB_ERROR        (0X02)

/*
�˿ڳ��Ⱥ궨��
*/
#define MVB_RECV_2BYTE         (0x00)
#define MVB_RECV_4BYTE         (0x01)
#define MVB_RECV_8BYTE         (0X02)
#define MVB_RECV_16BYTE        (0X03)
#define MVB_RECV_32BYTE        (0X04)


typedef struct
{
	uint16_t MvbPortAddr;              //mvb�˿ڵ�ַ
	uint8_t portnum;                   //mvb�˿�����
	uint16_t direction;                //�˿ڷ���0��Ŀ��˿ڣ�1:Դ�˿�
	uint16_t length;                   //�˿����ݳ���
	
	//��Ϊ������������ѭ��Ԥ��
	uint16_t  CycleTime;     
    uint16_t  warning : 1;
    uint16_t  error   : 1;
    uint32_t  cycleCounts;
	
}MVB_PORT_INFO,*MVB_PPORT_INFO;


typedef struct
{
	uint8_t  soucPortCount;
    uint8_t  sinkPortCount;
	uint16_t mvb_device_addr;
	MVB_PORT_INFO source_port[MVB_PORT_ADDR_NUM];
	MVB_PORT_INFO sink_port[MVB_PORT_ADDR_NUM];
}MVB_PORT_CFG,*MVB_PPORT_CFG;

typedef struct
{
	uint8_t fresh_data;
	uint8_t *data;
}MVB_SEND_DATA_BUF,*DMVB_SEND_DATA_BUF;

extern MVB_SEND_DATA_BUF  Mvb_Send_Data[MVB_PORT_ADDR_NUM];
extern MVB_SEND_DATA_BUF  Mvb_Recv_Data[MVB_PORT_ADDR_NUM];

void  Bsp_MvbTask_CreateApp(void);


#ifdef __cplusplus
}
#endif

#endif

