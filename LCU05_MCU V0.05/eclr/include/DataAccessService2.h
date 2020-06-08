/*
+------------------------------------------------------------------------------+
| ************************** DATAACCESSSERVICE.H ***************************** |
+------------------------------------------------------------------------------+
|                       Copyright 2009 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| access to plc variable including historical values (e.g. read, write,..)     |
+------------------------------------------------------------------------------+
| Author: Uwe Thomas, started at: 22.10.2009                                   |
| Changes have to be marked with an user time stamp (e.g. '// NN, 08.11.2003') |
+------------------------------------------------------------------------------+
*/

#ifndef __DATA_ACCES_SERVICE2_H__
#define __DATA_ACCES_SERVICE2_H__

#include "Services.h"
#include "StandardServices.h"
#include "VariableListManager.h"
#include "UAInterface.h"

class CDataAccessService2 : public CServiceImplBase
{
public:
    CDataAccessService2();
    ~CDataAccessService2();

    // implBase interface used by the remoting server
    static CServiceImplBase* CreateInstance();

    void Invoke(sint16 methodHandle, char* const);

   //Service method [1]: Int32 CreateSubscription(Subscription subscriptions);
    void CreateSubscription();

   //Service method [2]: void DeleteSubscriptions(Int32 subscriptionHandle);
    void DeleteSubscription();

   //Service method [3]: Notification[] GetSpecificResponse(Int32 subscriptionHandle);
    void GetSpecificResponse();

    
private:

    CUAInterface m_oCUAInterface;

    void writeValue(int typeInfo, void* pSendBuffer);
   

};

#endif //__DATA_ACCES_SERVICE2_H__
