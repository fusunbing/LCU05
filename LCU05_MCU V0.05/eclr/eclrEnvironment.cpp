//=============================================================================
//
//                      Copyright 2009 by KW Software GmbH                     
//                                                                             
//                      KW Software GmbH                                       
//                      Lagesche Str. 32                                       
//                      D-32657 Lemgo                                          
//                      Germany                                                
//                                                                             
//-----------------------------------------------------------------------------
// Module Name: eCLR Environment interface implementation
//-----------------------------------------------------------------------------
// Author:
//-----------------------------------------------------------------------------
// Change History:
// ----------------------------------------------------------------------------
//
//
// ============================================================================

// ----------------------------------------------------------------------------
// Pragma�s
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes 
// ----------------------------------------------------------------------------
#include "eclrEnvironment.h"

//RT-Thread Kernel
#include <rtthread.h>
// ----------------------------------------------------------------------------
// Definitions
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// User Type Declarations
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Data Object Declarations
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Private Method Declarations
// ----------------------------------------------------------------------------

// ============================================================================
// Start of Implementation
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Private Methods
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Global Methods
// ----------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Name:        EclrEnvironment::GetTickCount
//
// Description: Retrieves the millisecond count.
//
// Return:      uint32 - tick count in milliseconds
//
// Parameters:  None
//
// Note:        STATIC method
// ---------------------------------------------------------------------------

uint32 EclrEnvironment::GetTickCount() //resulotion is millisecond
{
  return rt_tick_get();
}


// ---------------------------------------------------------------------------
// Name:        EclrEnvironment::GetMicroTickCount
//
// Description: Initializes the embedded CLR environment class.
//
// Return:      uint32 - tick count in microseconds
//
// Parameters:  None
//
// Note:        STATIC method
// ---------------------------------------------------------------------------

uint32 EclrEnvironment::GetMicroTickCount() // resolution is microsecond
{
  return rt_tick_get()*1000;
}



/********************* EclrEnvironment plattform specifics ********************/


// ---------------------------------------------------------------------------
// Name:        EclrEnvironment::GetCurrentDateTime
// Description: Gets the current data and time.
// Return:      bool - true = retrieved successfully
//                     false = could not retrieve data or time
// Parameters:  dateTime - DateTime - structure to poulate with current time
//                                    and date.
// Note:        STATIC method
// ---------------------------------------------------------------------------

bool EclrEnvironment::GetCurrentDateTime(DateTime& dateTime)
{
   bool result = false;
/*   RTC_TIME_Type *pt=GetTime();
   if(pt != NULL)
   {
      dateTime.year = pt->YEAR;
      dateTime.month = pt->MONTH;
      dateTime.day = pt->DOM;
      dateTime.hour = pt->HOUR;
      dateTime.minute = pt->MIN;
      dateTime.second = pt->SEC;
      dateTime.millisecond = 0;
      result = true;
   }
  */
   return result;
}

// ---------------------------------------------------------------------------
// Name:        EclrEnvironment::FlushCache
// Description: 
// Return:      Nothing
// Parameters:  None
// Note:        STATIC method 
//              However should be implemented in eclr_main module since it 
//              is strongly dependent on CPU and OS type.
// ---------------------------------------------------------------------------
extern "C" void* eclrInstructionSync();
void EclrEnvironment::FlushCache(void*, unsigned)
{
	eclrInstructionSync();
}      


// ----------------------------------------------------------------------------
// End of Implementation
// ============================================================================
