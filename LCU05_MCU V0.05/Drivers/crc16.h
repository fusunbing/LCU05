

#ifndef __CRC16_H__
#define __CRC16_H__


#include "stm32f4xx.h"


//校验CRC值
u8 Crc8_CheckResult( u8 *p, u8 n );

//计算CRC值
unsigned short getCRC16(volatile unsigned char *ptr,unsigned char len);
unsigned short CRC16_Dn_Cal(unsigned char * dat, unsigned long len, unsigned short startCrc);
unsigned short CRC16_Up_Cal(unsigned char * dat, unsigned char len, unsigned short startCrc); 


#endif

