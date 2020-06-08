/*
+------------------------------------------------------------------------------+
| *************************** REMOTINGSERVER.H ******************************* |
+------------------------------------------------------------------------------+
|                       Copyright 2005 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| Server managment                                                            |
+------------------------------------------------------------------------------+
| Author: Jörg Kraemer, started at: 26.11.2005                                 |
| Changes have to be marked with an user time stamp (e.g. '// NN, 08.11.2003') |
+------------------------------------------------------------------------------+
*/

#ifndef __REMOTINGSERVER_H__
#define __REMOTINGSERVER_H__

#include "eclr.h"
#include "esocket.h"
#include "Services.h"
#include "Tools.h"

#ifdef PLATFORM_REMOTING_SECURITY_SUPPORT
#include "SecurityContext.h"
#endif

//1 min. 10 sec. (10 seconds more than the maximum timout of MP (during download))
#define REMOTING_CONNECTION_LIFETIMEOUT  70000
//#define REMOTING_SEND_TIMEOUT 2000
// disable the lifetimout mechanism, just for debug purposes
// #undef REMOTING_CONNECTION_LIFETIMEOUT

typedef CServiceImplBase* (*ServiceFactoryMethod)();

class ServiceFactory : public ClrAllocator
{
public:
    ServiceFactoryMethod    m_method;
    const char*             m_pName;
    ServiceFactory*         m_pNext;
};

class CRemotingServer : public ClrAllocator
{
public:
    enum RemotingCommands
    {
        GetServiceRequ  = 0,
        GetServiceConf  = 1,
        InvokeRequ      = 2,
        InvokeConf      = 3,
        SocomTest       = 6,
        RemotingVersion = 1
    };
    enum AbortCode
    {
        RecvTimeout = 1,
        RecvFailed  = 2,
        SendTimeout = 3,
        SendFailed  = 4,
        ContinueExecutionFailed = 5,
        SwitchRequest   = 6,
		NotAuthorized	= 7
    };
    CRemotingServer(EclrRemotingSocket* pSocket);
    ~CRemotingServer();

    int Process();

    void SendConfirmation();
    void SendError(uint32 errorNumber, const char* pMessage = NULL);
    void SendError_oldstyle(uint32 errorNumber, const char* pMessage);
    void AbortConnection(int abortCode);
    void Switch();

    static void AddService(ServiceFactoryMethod method, const char* serviceName);

private:
    CServiceImplBase* getService(const char* pServiceName);
    CServiceImplBase* getService(int handle);
    bool readHeader(ExtendedCommandHeader& header);
    void writeHeader(ExtendedCommandHeader& header);
    void testSocomm();
    void handleSoComTests(CommandHeader* pCommunicationHeader);

    CServiceImplBase* m_pServiceList;

    CBinaryWriter2 m_binaryWriter2;
    CBinaryReader2 m_binaryReader2;
    ExtendedCommandHeader m_currHeader;
    EclrRemotingSocket* m_pSocket;
    unsigned m_InvokeControlTime;

    // supports abort of blocking socket interfaces
    jmp_buf     m_abortJump;
    int         m_abortCode;
    CServiceImplBase* m_pContinueService;


    static ServiceFactory* s_pServiceFactory;

    // oldstyle services
    void invoke_oldstyle(CServiceImplBase* pService);
    CBinaryReader m_binaryReader;
    CBinaryWriter m_binaryWriter;
    char* m_pCommunicationBuffer;
    char m_pCommunicationBufferAlloc[COMM_BUFFER_SIZE + 8];

#ifdef PLATFORM_REMOTING_SECURITY_SUPPORT
private:
	CSecurityContext m_securityContext;
	bool AccessIsGranted(CServiceImplBase *, int);
#else
private:
	inline bool AccessIsGranted(CServiceImplBase *, int) { return true; };
#endif

public:
    inline char* GetCommunicationBuffer()   {   return((char*)m_pCommunicationBuffer);   }; 
    uint32 m_pCommunicationManagerID;
    unsigned m_currentCallStackId;
    unsigned m_currentForceListId;
};


#endif //__REMOTINGSERVER_H__
