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
#define MVB_PORT_TYPE_SINK          (1)     //接收端口
#define MVB_PORT_TYPE_SRCE          (2)     //发送端口

/*
车节号索引
*/
#define CAR_A1           (0x00)
#define CAR_B1           (0x01)
#define CAR_C1           (0x02)
#define CAR_C2           (0x03)
#define CAR_B2           (0x04)
#define CAR_A2           (0x05)


/*
MVB状态定义
*/
#define MVB_POWER_ON     (0x00)
#define MVB_NOMAL        (0x01)
#define MVB_ERROR        (0X02)

/*
端口长度宏定义
*/
#define MVB_RECV_2BYTE         (0x00)
#define MVB_RECV_4BYTE         (0x01)
#define MVB_RECV_8BYTE         (0X02)
#define MVB_RECV_16BYTE        (0X03)
#define MVB_RECV_32BYTE        (0X04)


typedef struct
{
	uint16_t MvbPortAddr;              //mvb端口地址
	uint8_t portnum;                   //mvb端口索引
	uint16_t direction;                //端口方向：0：目标端口；1:源端口
	uint16_t length;                   //端口数据长度
	
	//作为后续加入周期循环预留
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

