/*
+------------------------------------------------------------------------------+
| ************************* PROCONOSCONTROLSERVICE.H ************************* |
+------------------------------------------------------------------------------+
|                       Copyright 2005 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| Handling of control services (e.g. start, stop,..)                           |
+------------------------------------------------------------------------------+
| Author: Jörg Kraemer, started at: 17.03.2006                                 |
| Changes have to be marked with an user time stamp (e.g. '// NN, 08.11.2003') |
+------------------------------------------------------------------------------+
*/

#ifndef __PROCONOS_CONTROL_SERVICE_H__
#define __PROCONOS_CONTROL_SERVICE_H__

#include "Services.h"

  typedef enum _StartMode
  {
      StartMode_ColdStart=0x1,
      StartMode_WarmStart=0x2,
      StartMode_HotStart=0x3,
  } StartMode;

class CProConOSControlService : public CServiceImplBase
{
public:
    CProConOSControlService();
    ~CProConOSControlService();

    void Invoke(sint16 methodHandle, char* const pDataBuffer);

    static CServiceImplBase* CreateInstance();

  //Service method [1]: void ActivateBootProject();
    void ActivateBootProject();

   //Service method [2]: void ClearErrors();
    void ClearErrors();

   //Service method [3]: void DeleteBootProject();
    void DeleteBootProject();

   //Service method [4]: void InitRetainVariables();
    void InitRetainVariables();

   //Service method [5]: void Reset();
    void Reset();

   //Service method [6]: void Start(StartMode mode);
    void Start(StartMode mode);

   //Service method [7]: void Stop();
    void Stop();
private:
    CPcosDomain* m_pPcosDomain;

};

#endif //__PROCONOS_CONTROL_SERVICE_H__
