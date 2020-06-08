/*
+--------------------------------------------------------------------------------+
|                       StandardServices.h
+--------------------------------------------------------------------------------+
|                       Copyright 2005 by KW Software GmbH                       |
|                                                                                |
|                       KW Software GmbH                                         |
|                       Lagesche Str. 32                                         |
|                       D-32657 Lemgo                                            |
|                       Germany                                                  |
|                                                                                |
+--------------------------------------------------------------------------------+
| Communication service classes for server implementations. 
| Generated at 23.01.2008 10:43:06. 
| The types are generated from the following DLL:
| Ade.Remoting.Services, Version=1.1.2944.19128, Culture=neutral, PublicKeyToken=null
+--------------------------------------------------------------------------------+
| THIS IS AN AUTOMATICALLY GENERATED FILE. DO CHANGES ONLY IN THE C# SOURCES!    |
+--------------------------------------------------------------------------------+
*/        
#ifndef __STANDARDSERVICES_H__
#define __STANDARDSERVICES_H__

#include "Services.h"

  typedef enum _TriggerConditionOperation
  {
      TriggerCondition_None=0x0,
      TriggerCondition_Equals=0x1,
      TriggerCondition_NotEqual=0x2,
      TriggerCondition_GreaterThan=0x3,
      TriggerCondition_GreaterEqual=0x4,
      TriggerCondition_LessThan=0x5,
      TriggerCondition_LessEqual=0x6,      
      TriggerCondition_Modified=0x7,
      TriggerCondition_RisingEdge=0x8,
      TriggerCondition_FalingEdge=0x9,
  } TriggerConditionOperation;

  typedef enum _LAState
  {
      LAState_None=0x0,
      LAState_Inactive=0x1,
      LAState_Active=0x2,
      LAState_Active_Cont=0x3,
  } LAState;

  typedef enum _NotificationPeriod
  {
	  None = 0x0,
	  PreTriggerEvent = 0x1,
	  TriggerEvent = 0x2,
	  PostTriggerEvent = 0x3,
  }NotificationPeriod;

  typedef struct _Notification
  {
      sint32 SubscriptionHandle;
	  //sint32 RecordedPreCycles;
	  //sint32 RecordedCycles;
	  _NotificationPeriod NotificationPeriod;
	  sint32 TimeStamp;
      uint32* Values;
  } Notification;

  typedef enum _RpNItemType
  {
      Variable=0x0,
      Constant=0x1,
      Operation=0x2,
  }RpNItemType;

  typedef struct _TriggerRpnItem
  {
      RpNItemType Type;
      uint32* Values;
  }TriggerRpnItem;

  typedef struct _Subscription
  {
      sint32 ClientHandle;
      sint32 SubscriptionHandle;
      uint16 TaskName[32];
      unsigned TaskNameLength;
      TriggerRpnItem TriggerCondition;
    //  _TriggerConditionOperation TriggerCondition;
  //    int DataTypeTrigger1;
  //    ClrInt64 TriggerVar1;
  //    char TriggerVariable1[128];
      //int DataTypeTrigger2;
      //ClrInt64 TriggerVar2;
  //    char TriggerVariable2[128];
      sint32 Interval;
      sint32 CyclesBeforeTransmission;
      sint32 Cycles;
      sint32 PostCycles;
      const char* const* NamesTokens;
  } Subscription;


#endif // __STANDARDSERVICES_H__

