#ifndef __PLATFORM_H__
#define __PLATFORM_H__
 
// define the appropriated platform description 
#define PLATFORM_OSTYPE  EclrOS_FreeRTOS71
#define PLATFORM_TARGET_ENDIANESS   TARGET_LITTLE_ENDIAN    
  
// general header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <stdarg.h>


#define EXECUTION_INPLACE_SUPPORT           //PLC Code execute in flash
#define PLATFORM_THREADING_SUPPORT          // enable/disable platform threading support
#define PLATFORM_CALLSTACK_SUPPORT
#define PLATFORM_LOGICANALYZER_SUPPORT      // enable/disable logicanalyser support

#ifdef EXECUTION_INPLACE_SUPPORT
#define NO_STD_IO_SUPPORT
#else  // #ifndef EXECUTION_INPLACE_SUPPORT
#define PLATFORM_BUILTIN_LINKER_SUPPORT
#define PLATFORM_DOWNLOADCHANGE_SUPPORT     // enable/diable platform download-change support
#define PLATFORM_BREAKPOINT_SUPPORT
#define PLATFORM_BREAKPOINT_REGISTER_MAPPING
#endif // #ifndef EXECUTION_INPLACE_SUPPORT

#include "etarget.h"

#endif //#ifndef __PLATFORM_H__
