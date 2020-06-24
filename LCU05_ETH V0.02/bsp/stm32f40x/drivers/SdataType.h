/***************************************************************************

Copyright (C), 1999-2018, Tongye Tech. Co., Ltd.

* @file           Sdatatype.h
* @author        
* @version        V1.0.0
* @date           2018-08-22
* @brief          (1)  
                  (2) 
                  (3) 
History:          // Revision Records

<Author>             <time>       <version >            <desc>

Lily, Life           2018-08-22       V1.0.0               

***************************************************************************/
#ifndef __SHARP__DATATYPE_H
#define __SHARP__DATATYPE_H

/* Includes --------------------------------------------------------------*/
#include "stm32f4xx.h"

#define SDATA_MAGIC  (0xFEEDBEEF)

/* SharpData operation code definitions */
#define SPD_OPERTION_WREVT  (0xD01u)
#define SPD_OPERTION_WRFLT  (0xD02u)
#define SPD_OPERTION_WRCFG  (0xD03u)
#define SPD_OPERTION_PTUMEN  (0xD04u)
#define SPD_OPERTION_PTUDIS  (0xD05u)
//#define SPD_OPERTION_WREVT  (0x01u)
//#define SPD_OPERTION_WREVT  (0x01u)
#define SPD_OPERTION_CANCLE  (0xF000F000u)

/* SharpData dataset block size definitions */
#define SPD_DSBLK_4K     (4096)
#define SPD_DSBLK_MAX    (4096 * 20)

/* SharpData operation erro definitions */
#define SPD_OPERTION_OK         ((long)0u)
#define SPD_OPERTION_ERRTYPE    ((long)(-999))
#define SPD_OPERTION_ERRSIZE    ((long)(-998))
#define SPD_OPERTION_ERRNULL    ((long)(-997))
#define SPD_OPERTION_ERRNOINIT  ((long)(-996))
#define SPD_OPERTION_ERRCALL    ((long)(-995))
#define SPD_OPERTION_ERRPTR     ((long)(-994))
#define SPD_OPERTION_ERRCREATE  ((long)(-993))
#define SPD_OPERTION_ERRNOFILE  ((long)(-992))
#define SPD_OPERTION_ERREEFILE  ((long)(-991))
#define SPD_OPERTION_ERREVTCFG  ((long)(-990))
#define SPD_OPERTION_ERRSRV     ((long)(-989))
#define SPD_OPERTION_EXITSRV    ((long)(-999))


/* Fat Service config */
#define FS_EVTFILE_SIZE  (32u * 1024u * 1024u)

#define FS_EVTFILE_NUMS  (0x30u)

#define FS_EVFCFG_FILENAME ("Sevt/evtcfg.bin")

typedef struct
{
    uint32_t  magicFlag;
    uint32_t  fattime;
    uint32_t  fileMaxNum;
    uint32_t  currentNum;
    char      filNames[FS_EVTFILE_NUMS][32];
    uint32_t  crc16;
}FSEVTConfg_TypeDef;




//------------------------------------------------------------------
// 回调函数原型
//------------------------------------------------------------------
typedef void(*fatsrv_callback)(uint32_t result);
//------------------------------------------------------------------
//                     
//------------------------------------------------------------------
typedef struct 
{
    uint32_t  magicFlag;
    uint32_t  systick;
    uint32_t  sysdateTime;
    uint32_t  dataType;
    uint32_t  dataSetSize;
    fatsrv_callback callback;
    uint32_t  crc16;
}SDataSetParent_TypeDef;

typedef struct 
{
    uint32_t  magicFlag;
    uint32_t  cmdType;
    uint16_t  dataSetSize;
    uint16_t  crc16;
}SDataPtu_TypeDef;

typedef struct 
{
    uint32_t  cmdType;
    uint32_t  packId;
    uint16_t  dataSetSize;
    uint16_t  crc16;
}SDataPtuAck_TypeDef;

#endif 


