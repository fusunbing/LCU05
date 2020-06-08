/*****************************************************************************
*
* Copyright (c) 2005,深圳通业科技有限公司 .All rights reserved.
*
* -----------------------------------------------------------------------------
*
* 文件摘要:MVB-RVS驱动程序头文件
*
* -----------------------------------------------------------------------------
*
* 文件名称:	Mvbdrv_zz.h
* 软件设计:  hcx
* 编译环境:	mdk
* 版本控制:  kpc
*
*****************************************************************************/
/***修改及版本记录*****************************************************************
*
*
*
**********************************************************************************/

#ifndef _MVBDRV_H_
#define _MVBDRV_H_

#include "stm32f4xx.h"



#define MAXPORTNUM            (16)
#define MAXMVBBYTE            (32)
#define SINK                  (0)
#define SOURCE                (1)
#define LINEAB                (3)  

//UART ADDRESS BASE
#define UART_ABASE            (0x301001f8)

//UART emulation寄存器地址定义 
#define UART_RBR              (0x6C000000 + 0x0201f8)
#define UART_THR              (0x6C000000 + 0x0201f8)//0x680201f8
#define UART_TEST             (0x6C000000 + 0x0201fa)//0x680201fa
#define UART_LSR              (0x6C000000 + 0x0201fd)//0x680201fd


//UART emulation寄存器状态定义
#define UART_DR_OK            (0x01)
#define UART_THRE_OK          (0x20)
#define UART_TEMT_OK          (0x40)

//常量定义
#define MVB_DR_CHECK_TIMES    (500)
#define MVB_THRE_CHECK_TIMES  (500)
#define RESET_BYTE_COUNTE     (64)


#define RET_OK                (1)
#define RET_ERROR             (0)





u8 ClearMvb(void);
u8 gf_put_pd(u8 PortNum,u8 *PointerSend,u8 Len);
u8 gf_get_pd(u8 PortNum,u8 *PointerReceive,u8 Len);
u8 ConfigMvbPort(u16 MvbPortAddr,u8 PortNum,u16 direction,u16 length);
u8 ConfigMvbcheck(u16 MvbPortAddr,u8 PortNum,u16 direction,u16 length);
u8 ConfigMvbDevice(u16 MvbDeviceAddr,u16 InputLine);
u8 StarMvb(void);
u8 CheckUartStauts(void);




#endif
