/**
  *************************************************************************
  * This file is part of Platform
  * COPYRIGHT (C) 1999-2015, Lanp R&D Team
  * @file    KWAppInfo.h
  * @brief   This file can get the KW-App's Info.   
  *          
  * Change Logs:
  * Date           Author       Notes
  * 2015/06/09         the first version
  *************************************************************************
  */
	
#ifndef __KWAPPINFO__H__
#define __KWAPPINFO__H__

typedef unsigned long long  uint64;
typedef unsigned char       uint8;

#ifdef __cplusplus 
extern "C" 
{ 
#endif
	
uint8 GetKWAppState(void);
uint64 GetPLCUnixTimestamp(void);
	
#ifdef __cplusplus 
}
#endif

#endif
/******************* (C) COPYRIGHT 2014 Lanp **********END OF FILE****/
