/*
+------------------------------------------------------------------------------+
| ****************************** CALLBACKBASE.H ****************************** |
+------------------------------------------------------------------------------+
|                       Copyright 2005 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| Description: base class for callback method implementations for ADE remoting |
|                                                                              |
| Author: Uwe Thomas, started at: 31.10.2005                                   |
+------------------------------------------------------------------------------+
*/

#ifndef __CALLBACKBASE_H__
#define __CALLBACKBASE_H__

#include "BinaryWriter.h"

class CServiceImplBase;

class CCallbackBase
{
private: 
	void SendErrorProlog(uint32 errId, char* resName, uint32 paramCount);
public:
    CCallbackBase(CServiceImplBase* pServiceImplBase);
    ~CCallbackBase();
    CBinaryWriter* m_pBinaryWriter;
    
    inline void SendError(uint32 errorNumber)
    {
        m_pBinaryWriter->SendError(errorNumber, NULL);
    }

	//Obsolet (handles are not required any more)
    inline void SendError(uint32 errorNumber, uint16 serviceHandle, uint16 methodHandle)
    {
        m_pBinaryWriter->SendError(errorNumber, serviceHandle, methodHandle, NULL);
    }

	
	// localized error messages 
	//errId - number of the Error
	//errorResourceName - name of the .NET resource where the localized message is defined
	bool SendError(uint32 errId, char* errorResourceName);
	bool SendError(uint32 errId, char* errorResourceName, sint32 intPrameter1);
	bool SendError(uint32 errId, char* errorResourceName, sint32 intPrameter1, sint32 intPrameter2);
	bool SendErrorAnsi(uint32 errId, char* errorResourceName, char* ansiStringParameter1);
	bool SendErrorAnsi(uint32 errId, char* errorResourceName, char* ansiStringParameter1, sint32 intPrameter1);
	bool SendErrorAnsi(uint32 errId, char* errorResourceName, char* ansiStringParameter1, char* ansiStringParameter2);
	bool SendErrorUTF8(uint32 errId, char* errorResourceName, char* utf8StringParameter1);
	bool SendErrorUTF8(uint32 errId, char* errorResourceName, char* utf8StringParameter1, sint32 intPrameter1);
	bool SendErrorUTF8(uint32 errId, char* errorResourceName, char* utf8StringParameter1, char* ansiStringParameter2);

protected:
    CServiceImplBase* m_pServiceImplBase;
};

#endif // __CALLBACKBASE_H__
