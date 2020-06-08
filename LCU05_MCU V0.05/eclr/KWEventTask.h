/**
  *************************************************************************
  * This file is part of Platform
  * COPYRIGHT (C) 1999-2015, Lanp R&D Team
  * @file    KWEventTask.h
  * @brief   This file can be used to trigger the event task with the ID.   
  *          
  * Change Logs:
  * Date           Author       Notes
  * 2015/03/05         the first version
  *************************************************************************
  */
	
#ifndef __KWEVENTTASK__H__
#define __KWEVENTTASK__H__


#ifdef __cplusplus 
extern "C" 
{ 
#endif

void TriggerEventTask();
void KW_SetEventId(unsigned int id);

#ifdef __cplusplus 
}
#endif

#endif
/******************* (C) COPYRIGHT 2015 Lanp **********END OF FILE****/