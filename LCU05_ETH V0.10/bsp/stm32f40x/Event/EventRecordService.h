/***************************************************************************

Copyright (C), 1999-2014, Tongye Tech. Co., Ltd.

* @file           EventRecordService.h
* @author          
* @version        V0.1.0
* @date           2014-06-24
* @brief          (1)  
(2) 
(3) 
History:          // Revision Records

<Author>             <time>       <version >            <desc>

lily,life           2014-06-24       V0.1.0             ~{3u4N=(A"~}

***************************************************************************/


#ifndef __EVENT_RECORD_SERVICE_H
#define __EVENT_RECORD_SERVICE_H

#ifdef __cplusplus
extern "C" {
#endif 

    /* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

//2019/6/27 王晓信修改 原本的64(字节) * 62(个)形式的数据缓冲区 修改为32(字节) * 124(个)形式的数据缓冲区
#define SRD_SIZE_MAX       (32)
#define SRD_COUNT_MAX      (62*2)
//#define SRD_SIZE_MAX       (64)
//#define SRD_COUNT_MAX      (62)
#define ERSRV_MESSAGE_MAX  (SRD_SIZE_MAX)
#define EVENTRECORD_CELL   (SRD_SIZE_MAX)
// 2015-7-28 ~{<S4s6SAP3_4g#,D?5D#:N*K+;:3eGP;;J1#,Ge?U51G0J}>]2YWwWvW<18~}
#define EVENTRECORD_QUE    (62)   

#define ERD_DATATYPE_DXUEV   ((uint16_t)0x8001)
#define ERD_DATATYPE_MCUERR  ((uint16_t)0x8002)
#define ERD_DATATYPE_MCUIO   ((uint16_t)0x8003)
#define ERD_DATATYPE_MCUIOD  ((uint16_t)0x8004)
#define ERD_DATATYPE_MCUST   ((uint16_t)0x8005)
#define ERD_DATATYPE_MCUTB   ((uint16_t)0x8006)
#define ERD_DATATYPE_MCUEV   ((uint16_t)0x8007)
#define ERD_DATATYPE_SYSERR  ((uint16_t)0x8008)
#define ERD_DATATYPE_ETHIO   ((uint16_t)0x9001)
	//  [10/29/2017 Lily]
#define ERD_DATATYPE_TRAINCANRX  ((uint16_t)0x8009)
#define ERD_DATATYPE_TRAINCANTX  ((uint16_t)0x800A)
#define ERD_DATATYPE_DIOPD  ((uint16_t)0x800B)


    /* Definition for   ********************************************/

    /* Definition for   **********************************************/
    typedef struct  
    {
        uint32_t  type    : 16;
        uint32_t  year    : 8;
        uint32_t  month   : 8;

        uint32_t  day     : 8;
        uint32_t  hour    : 8;
        uint32_t  min     : 8;
        uint32_t  sec     : 8;

        uint32_t  tick;
        uint32_t  size;
    }Event_ObjectBaseTypeDef;



    /* Misc definition ************************************************************/

    int   ERSRV_Write(void*buffer, uint32_t buffsize, uint32_t type);
    void  ERSRV_CreateApp(void);
    void  ERSRV_EmergencyWrite(void);

    void ERSRV_EmergencyRecord(void *buffer, uint32_t buffsize, uint32_t type);
#ifdef __cplusplus
}
#endif

#endif /* __CAN_FAULT_DETECT_H */

/************************ (C) COPYRIGHT Tongye *****END OF FILE****/
