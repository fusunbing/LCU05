/*
+------------------------------------------------------------------------------+
| *************************** SERVICEPROVIDER.H ****************************** |
+------------------------------------------------------------------------------+
|                       Copyright 2005 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| Service managment                                                            |
+------------------------------------------------------------------------------+
| Author: Jörg Kraemer, started at: 29.09.2005                                 |
| Changes have to be marked with an user time stamp (e.g. '// NN, 08.11.2003') |
+------------------------------------------------------------------------------+
*/

#ifndef __SERVICEPROVIDER_H__
#define __SERVICEPROVIDER_H__

#include "Remotingserver.h"
#include "RemotingClrCmd.h"
#include "Tools.h"
#include "BinaryWriter.h"

class CServiceImplBase;

// functionpointer to the class factory method
//typedef CServiceImplBase* (*ServiceFactoryMethod)();
// collection of class factory methods
typedef eVector<ServiceFactoryMethod,20> ServiceFactoryVector;
typedef eVector<const char*,20> ServiceFactoryNameVector;

/// Class for encoding / Coding Communication abd serving CLR
class CServiceProvider : public ClrAllocator
{   
public:
    CServiceProvider(CBinaryWriter* pBinaryWriter);
    ~CServiceProvider();
    CServiceImplBase* GetService(int serviceHandle);

    // Gets incomming request and interpret it
public:
    static void AddService(ServiceFactoryMethod method, const char* const serviceName);

    // Get Service Handle for requested service, if not exist it will be added
    // ret = 0, if not successfull
    int GetServiceHandle(char* serviceName);

    // calls methods of service
    void Invoke(int serviceHandle, int methodHandle, char* const pDataBuffer);
    static void Ctors();

private:
    CServiceImplBase* CreateService(char* serviceName);

    // class instance field member
    CBinaryReader   m_BinaryReader;
    // class reference field member
    CBinaryWriter* m_pBinaryWriter;
    CServiceImplBase* m_pFirstService;

public:

    static ServiceFactoryVector s_services; //UT, 2006-03-07
    static ServiceFactoryNameVector s_serviceNames; //UT, 2006-03-07
};



#endif //__SERVICEPROVIDER_H__
