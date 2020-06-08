/*
+------------------------------------------------------------------------------+
| ****************************** FORCESERVICE.H **************************** |
+------------------------------------------------------------------------------+
|                       Copyright 2005 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| Handling of force list services (e.g. add, remove,..)                        |
+------------------------------------------------------------------------------+
| Author: Jörg Kraemer, started at: 09.02.2006                                 |
| Changes have to be marked with an user time stamp (e.g. '// NN, 08.11.2003') |
+------------------------------------------------------------------------------+
*/

#ifndef __FORCESERVICE_H__
#define __FORCESERVICE_H__

#include "Services.h"
#include "VariableListManager.h"

class CForceService : public CServiceImplBase
{
public:
    CForceService();
    ~CForceService();
    void Invoke(sint16 methodHandle, char* const pDataBuffer);

    static CServiceImplBase* CreateInstance();

private:
   //Service method [1]: void AddItem(String variableName, Byte[] value);
    void AddItem();

   //Service method [2]: void ClearList();
    void ClearList();

   //Service method [3]: void GetList(out Int16& variableCount, out Byte[]& data);
    void GetList();

   //Service method [4]: Boolean IsEmptyList();
    void IsEmptyList();

   //Service method [5]: void RemoveItem(String variableName);
    void RemoveItem();

    // private function
    bool isMaster();
    void setForceListId(unsigned id);

    CPcosDomain* m_pPcosDomain;
};
#endif //__FORCESERVICE_H__
