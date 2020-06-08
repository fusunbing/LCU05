/*
+------------------------------------------------------------------------------+
| ************************* BREAKPOINTSERVICE.H ****************************** |
+------------------------------------------------------------------------------+
|                       Copyright 2007 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| Handling of control services (e.g. start, stop,..)                           |
+------------------------------------------------------------------------------+
| Author: Jürgen Tölke, started at: 31.10.2007                                 |
| Changes have to be marked with an user time stamp (e.g. '// NN, 08.11.2003') |
+------------------------------------------------------------------------------+
*/

#ifndef __BREAKPOINT_SERVICE_H__
#define __BREAKPOINT_SERVICE_H__

#include "Services.h"

typedef enum _BreakpointType
{
  BreakpointType_None=0x0,
  BreakpointType_Permanent=0x1,
  BreakpointType_Temporarily=0x2,
  BreakpointType_TemporarilyAndContinue=0x3,
} BreakpointType;

typedef enum _EclrExceptionType
{
  EclrExceptionType_None=0x0,
  EclrExceptionType_ExcOutOfMemory=0x1,
  EclrExceptionType_ExcDivisionByZero=0x2,
  EclrExceptionType_ExcIndexOutOfRange=0x3,
  EclrExceptionType_ExcIllegalCast=0x4,
  EclrExceptionType_ExcStackOverflow=0x5,
  EclrExceptionType_ExcNullReference=0x6,
  EclrExceptionType_ExcMissingMethod=0x7,
  EclrExceptionType_ExcThreadCreation=0x8,
  EclrExceptionType_ExcThreadAbort=0x9,
  EclrExceptionType_ExcSynchronizationLockException=0xA,
  EclrExceptionType_ExcBreakpointIllegal=0xB,
  EclrExceptionType_ExcBreakpoint=0xC,
  EclrExceptionType_ExcExternal=0x80,
} EclrExceptionType;

typedef struct _MethodFrame
{
  ClrInt64 ThisPointer;
  ClrInt64 FramePointer;
  uint32 MethodHandle;
  uint32 CurrentPosition;
} MethodFrame;

class CDebugService : public CServiceImplBase
{
public:
    CDebugService();
    ~CDebugService();
    void Invoke(sint16 methodHandle, char* const pDataBuffer);
    
    static CServiceImplBase* CreateInstance();

       //Service method [4]: void ClearAllBreakpoints();
    void ClearAllBreakpoints();

   //Service method [5sta]: void Continue();
    void Continue();

   //Service method [1]: void GetCallstackChain(out EclrException& exceptionType, out UInt32& threadHandle, out MethodFrame[]& methodFrames);
    void GetCallstackChain();

   //Service method [3]: void ResetBreakpoint(UInt32 methodHandle, UInt32 nativeAddress);
    //void ResetBreakpoint(uint32 methodHandle, uint32 nativeAddress);
    void ResetBreakpoint();

   //Service method [2]: void SetBreakpoint(UInt32 methodHandle, UInt32 nativeAddress);
    //void SetBreakpoint(uint32 methodHandle, uint32 nativeAddress, uint32 bpType);
    void SetBreakpoint();

};

#endif //__BREAKPOINT_H__
