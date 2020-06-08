/*
+------------------------------------------------------------------------------+
| ******************************* SERVICES.H ********************************* |
+------------------------------------------------------------------------------+
|                       Copyright 2005 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| Abstact service base                                                         |
+------------------------------------------------------------------------------+
| Author: Jörg Kraemer, started at: 29.09.2005                                 |
| Changes have to be marked with an user time stamp (e.g. '// NN, 08.11.2003') |
+------------------------------------------------------------------------------+
*/

#ifndef __SERVICES_H__
#define __SERVICES_H__

#include "pcosdomain.h"

#include "BinaryReader.h"
#include "BinaryWriter.h"

#ifdef PLATFORM_REMOTING_SECURITY_SUPPORT
#include "SecurityContext.h"
#endif
class CRemotingServer;
class CServiceImplBase;

class CServiceListItem : public eListNode<CServiceListItem>
{
public:
    CServiceImplBase* m_pServiceImpl;
};

class CServiceImplBase : public ClrAllocator
{
public:
    CServiceImplBase();
    virtual ~CServiceImplBase();

    virtual void Invoke(short methodHandle, char* const pDataBuffer) = 0;
    virtual void Continue(int methodHandle);
protected: 
    CBinaryWriter*  m_pBinaryWriter;
    CBinaryReader*  m_pBinaryReader;
    // remoting reader/writer newstyle
    CBinaryReader2* m_pBinaryReader2;
    CBinaryWriter2* m_pBinaryWriter2;
    const char*     m_pName;
#ifdef PLATFORM_REMOTING_SECURITY_SUPPORT
	CSecurityContext* m_pSecurityContext;
#endif // PLATFORM_REMOTING_SECURITY_SUPPORT
public:
    // __TODO__ move handle to the private section
    sint16 m_Handle;
    // oldstyle , useless with newstyle services
    sint16 m_LastMethodHandle;
    
    // remoting reader/writer newstyle
    inline void setReaderWriterNewstyle()
    {   
        m_isReaderWriteNewstyle = true;
    }
    inline bool isReaderWriterNewstyle()
    {   
        return m_isReaderWriteNewstyle;
    }
    inline void setConnection(CBinaryReader2* pBinaryReader, CBinaryWriter2* pBinaryWriter)
    {
        m_pBinaryReader2 = pBinaryReader;
        m_pBinaryWriter2 = pBinaryWriter;
    }
    // __TODO__ skip useless virtual, only DummyTests supports this method !
    virtual void setBinaryWriter(CBinaryWriter* pBinaryWriter) 
    {
        m_pBinaryWriter = pBinaryWriter;
    }
    inline void setBinaryReader(CBinaryReader* pBinaryReader)
    {   
        m_pBinaryReader = pBinaryReader;
    }
    inline const char* getServiceName() 
    { 
        return m_pName; 
    }
    inline void setSeviceName(const char* pServiceName)
    {
        m_pName = pServiceName;
    }
    inline int getServiceHandle() 
    { 
        return m_Handle; 
    }
    inline void setSeviceHandle(int handle) 
    {   
        m_Handle = (sint16)handle;  
    }

    inline sint16 getLastMethodHandle() { return(m_LastMethodHandle); };
    inline void setLastMethodHandle(int handle) {   m_LastMethodHandle = (sint16)handle;    };

    inline CServiceImplBase* getNext()
    {
        return m_pNext;
    }
    inline void setNext(CServiceImplBase* pNext)
    {
        m_pNext = pNext;
    }
#ifdef PLATFORM_REMOTING_SECURITY_SUPPORT
	inline void setSecurityContext(CSecurityContext *pSecurityContext)
	{
		m_pSecurityContext = pSecurityContext;
	}
#endif // PLATFORM_REMOTING_SECURITY_SUPPORT
private:
    CServiceImplBase* m_pNext;
    // remoting reader/writer newstyle
    bool m_isReaderWriteNewstyle;
};




#endif //__SERVICES_H__
