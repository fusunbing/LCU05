/***************************************************************************

Copyright (C), 1999-2019, Tongye Tech. Co., Ltd.

* @file           ModBusTcp.c
* @author        
* @version        V1.0.0
* @date           2019-04-23
* @brief          (1)  
                  (2) 
                  (3) 
History:          // Revision Records

<Author>             <time>       <version >            <desc>

Lily, Life           2019-04-23       V1.0.0               

***************************************************************************/
/* Includes --------------------------------------------------------------*/

#include "ModBusTcp.h"
#include "static_mem.h"
//#include "CanStack_Slave.h"
//#include "Sdatatype.h"

#define MODBUS_TRANSBUFFER_SIZE  (2048U)
#define MODBUS_TRANSOPLIST_SIZE  (1U)

typedef struct
{
    uint8_t  opType;
    uint8_t  localAddr;
    uint16_t  regStartAddress;
    uint16_t  regSize;
    uint16_t* dataMemPtr;
}ModBusOpDescription_TypeDef;

typedef struct
{
	uint16_t EventID;
	uint16_t ProtoID;
	uint16_t length;
    uint8_t  DevId;
    uint8_t  FuncId;
    uint16_t regStart;
    uint16_t regSize;
}ModBusTcpHeader_Tyepdef;



static ModBusOpDescription_TypeDef modbus_opList[MODBUS_TRANSOPLIST_SIZE] = {
    /* opType,localAddr,regStartAddress,regSize,dataMemPtr */
    {0x10, 0x01, 0x0000, 0x0010, RT_NULL}
    };

static uint8_t* writeBuf = RT_NULL;
/*******************************************************************************
* Description    : 
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int  ModBusTcp_Int(void)
{
	writeBuf = static_malloc(MODBUS_TRANSBUFFER_SIZE);
//    modbus_opList[0].dataMemPtr = (uint16_t*)CanSlave_get_hmiErr();
}


static uint16_t ModBusTcp_ToBigEnd(uint16_t val)
{
    uint16_t v;

    v = (uint16_t)(((val & 0x00FF) << 8) + ((val & 0xFF00) >> 8));

    return v;
}

static int doListPtr = 0;
static uint16_t EventID = 0;
int  ModBusTcp_Generate(void* buf, int buf_size)
{
    int retBytes = -1;
    uint16_t modLength;  
    uint8_t* writeDataPtr;    
    uint16_t* outDataPtr;    
    ModBusTcpHeader_Tyepdef* mbtHeader;
    int i;
    
	if(writeBuf != RT_NULL)
    {
        EventID +=1;

        mbtHeader = (ModBusTcpHeader_Tyepdef*)writeBuf;
        mbtHeader->EventID = ModBusTcp_ToBigEnd(EventID);
        mbtHeader->ProtoID = 0x0000U;
        mbtHeader->DevId = modbus_opList[doListPtr].localAddr;
        mbtHeader->FuncId = modbus_opList[doListPtr].opType;
        mbtHeader->regStart = ModBusTcp_ToBigEnd(modbus_opList[doListPtr].regStartAddress);
        mbtHeader->regSize = ModBusTcp_ToBigEnd(modbus_opList[doListPtr].regSize);
        
        switch(modbus_opList[doListPtr].opType)
        {
            case 0x10:
                modLength = 6 + 1 + modbus_opList[doListPtr].regSize * 2;
                mbtHeader->length = ModBusTcp_ToBigEnd(modLength);
                /* Ð´×Ö½ÚÊý */
                writeDataPtr = writeBuf + sizeof(ModBusTcpHeader_Tyepdef);
                writeDataPtr[0] = (uint8_t)(modbus_opList[doListPtr].regSize * 2);
                outDataPtr = (uint16_t *)(writeDataPtr + 1);
            
                for(i = 0; i < modbus_opList[doListPtr].regSize; i++)
                {
                    outDataPtr[i] = ModBusTcp_ToBigEnd(modbus_opList[doListPtr].dataMemPtr[i]);
                }
            
                retBytes = sizeof(ModBusTcpHeader_Tyepdef) + 1 + modbus_opList[doListPtr].regSize * 2;
                break;
            case 0x03:
                modLength = 6;
                mbtHeader->length = ModBusTcp_ToBigEnd(modLength);
                retBytes = sizeof(ModBusTcpHeader_Tyepdef);
                break;
            default:
                retBytes = -1;
                break;
        }
        
        rt_memcpy(buf, writeBuf, retBytes);
        doListPtr = doListPtr + 1;
        doListPtr %= MODBUS_TRANSOPLIST_SIZE;
    }
    
    return retBytes;
}











