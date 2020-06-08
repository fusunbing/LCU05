/*
+------------------------------------------------------------------------------+
| **************************** REMOTINGCLRCMD.H ****************************** |
+------------------------------------------------------------------------------+
|                       Copyright 2005 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| Interface functions to eCLR                                                  |
+------------------------------------------------------------------------------+
| Author: Jörg Kraemer, started at: 29.09.2005                                 |
| Changes have to be marked with an user time stamp (e.g. '// NN, 08.11.2003') |
+------------------------------------------------------------------------------+
*/

#ifndef __REMOTING_CLR_CMD_H__
#define __REMOTING_CLR_CMD_H__


#include "controller.h"
#include "appdomain.h"
#include "Tools.h" 
#include "BinaryWriter.h"
#include "BinaryReader.h"
#include "pcosdomain.h"
#include "pcos.h"

class CRemotingClrCmd :  public ClrAllocator
{
public:
    CRemotingClrCmd();
    ~CRemotingClrCmd();

    static ClrStatus readErrorList(CBinaryWriter* pWriter);
 };


#endif //__REMOTING_CLR_CMD_H__
