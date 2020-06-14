

#ifndef __BSP_TASK_H__
#define __BSP_TASK_H__

#include "stm32f4xx.h"

#define BSP_TSYS_STACKSIZE               (1 * 1024 / 4)  //1K  32bit 对齐
#define BSP_TAD_STACKSIZE                (1 * 512  / 4)  //512 32bit 对齐
#define BSP_TRS232_COMM_STACKSIZE        (1 * 1024 / 4)  //1K  32bit 对齐	
#define BSP_TRESEXP_COMM_STACKSIZE       (1 * 1024 / 4)  //1K  32bit 对齐	 
#define BSP_TRESCOMM_BUS_STACKSIZE       (1 * 512  / 4)  //512 32bit 对齐
#define BSP_TEXP_COMM_STACKSIZE          (1 * 1024 / 4)  //1K  32bit 对齐	 
#define BSP_TCOMM_BUS_STACKSIZE          (1 * 512  / 4)  //512 32bit 对齐
#define BSP_TAPP_STACKSIZE               (1 * 1024 / 4)  //1K  32bit 对齐
#define BSP_TLED_STACKSIZE               (1 * 512  / 4)  //512 32bit 对齐
#define BSP_PWM_STACKSIZE                (1 * 512  / 4)  //512 32bit 对齐
#define BSP_FINSH_Q_STACKSIZE            (1 * 1024 / 4)  //1K  32bit 对齐
#define BSP_RD_JUDGE_STACKSIZE           (1 * 1024 / 4)  //1K  32bit 对齐
#define BSP_CAN_CMTX_STACKSIZE           (1 * 1024 / 4)  //1K  32bit 对齐

//#define BSP_CAN_Q_STACKSIZE              (1 * 1024 / 4)  //1K  32bit 对齐
//#define BSP_TCAN_STACKSIZE               (1 * 1024 / 4)  //1K  32bit 对齐

#define BSP_TSYS_PRIORITY                (10)
#define BSP_TAD_PRIORITY                 (16)
#define BSP_TRS232_COMM_PRIORITY         (14)
#define BSP_TRESEXP_COMM_PRIORITY        (11)
#define BSP_TRESCOMM_BUS_PRIORITY        (6)
#define BSP_TEXP_COMM_PRIORITY           (12)
#define BSP_TCOMM_BUS_PRIORITY           (7)
#define BSP_TAPP_PRIORITY                (15)
#define BSP_TLED_PRIORITY                (16)
#define BSP_FINSH_Q_PRIORITY             (19)
#define BSP_RD_JUDGE_PRIORITY            (10)
#define BSP_CAN_CMTX_PRIORITY            (13)

//#define BSP_CAN_T_Q_PRIORITY             (8)
//#define BSP_CAN_R_Q_PRIORITY             (9)
#define BSP_PWM_PRIORITY                 (19)
//#define BSP_TCAN_PRIORITY                (13)
#define IWDG_EN		(1)


void System_bsptask_create(void);


#endif

