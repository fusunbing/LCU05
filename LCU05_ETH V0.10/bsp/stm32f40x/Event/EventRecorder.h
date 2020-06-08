/***************************************************************************

Copyright (C), 1999-2014, Tongye Tech. Co., Ltd.

* @file           EventRecorder.h
* @author          
* @version        V0.1.0
* @date           2014-06-19
* @brief          (1)  
(2) 
(3) 
History:          // Revision Records

<Author>             <time>       <version >            <desc>

lily, life         2014-06-19       V0.1.0             ³õ´Î½¨Á¢

***************************************************************************/


#ifndef __EVENT_RECORDER_H
#define __EVENT_RECORDER_H

#ifdef __cplusplus
extern "C" {
#endif 

    /* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

    typedef struct
    {
        uint32_t tick;
    }Event_RecorderCellBaseTypeDef;


    typedef union
    {
        struct  
        {
            uint32_t isCtreate     : 1;
            uint32_t canRead       : 1;
            uint32_t reserv        : 30;
        }bits;
        uint32_t value;
    }Event_ControlFlag;


    typedef struct  _Event_DatasetObject_
    {
        Event_ControlFlag ctrlFlag;  
        uint32_t  loopPoint;
        uint32_t  loopSize;
        uint32_t  cellSize;
        uint8_t*  loopBuf1;
        uint8_t*  loopBuf2;
        uint8_t*  pBufWrite;
        uint8_t*  pBufRead;

        void  (*Record)(struct _Event_DatasetObject_* _me, void* cell);
    }Event_DatasetObjectTypeDef;

    /* Definition for USARTx resources ********************************************/

    /* Definition for DMAx resources **********************************************/




    /* Misc definition ************************************************************/
    void EventRecd_Init(void);
    Event_DatasetObjectTypeDef* EventRecd_CreateObject(uint32_t cellsize, uint32_t loopsize);

    /* Transmit buffer size */
#ifdef __cplusplus
}
#endif

#endif /* __EVENT_RECORDER_H */

/************************ (C) COPYRIGHT Tongye *****END OF FILE****/
