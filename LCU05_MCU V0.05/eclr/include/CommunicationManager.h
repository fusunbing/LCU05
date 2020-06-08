/*
+------------------------------------------------------------------------------+
| ************************** COMMUNICATIONMANAGER.H ************************** |
+------------------------------------------------------------------------------+
|                       Copyright 2006 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| Description: Static class with communication information                     |
|                                                                              |
| Author: Jörg Kraemer, started at: 18.01.2006                                 |
+------------------------------------------------------------------------------+
*/
#ifndef __COMMUNICATION_MANAGER_H__
#define __COMMUNICATION_MANAGER_H__

#include "eclr.h"

class CCommunicationManager  : public ClrAllocator
{
private:// the constructor should not be used - this class is allways static
    CCommunicationManager();
    ~CCommunicationManager();

    static unsigned m_breakpointListVersion;        // Counter for breakpointlist changes (inital value: 1)
    static unsigned m_metaDataVersion;              // Counter for metaData changes (inital value: 1)
    static uint32 m_ThreadID;                       // unique ID for each thread

    static uint32 m_MasterID;                       // unique ID for each thread

    static bool m_bGeneralMasterActive;             // Flag whether explizit master is set

    static bool m_bSourceOnPlc;
    static bool m_bSourceOnPlcUpToDate;

    static bool m_bImageOnPlc;
    static bool m_bImageOnPlcUpToDate;

public:
    static inline uint32 createNewThreadID() {   return(m_ThreadID++);   };


    static bool setGeneralMasterActive(uint32 threadID);            // Flag whether explizit master is set


    static void resetGeneralMasterActive(uint32 threadID);          // Flag whether explizit master is set

    // Flag whether explizit master is set
    static inline bool isAnyMasterStateActive()
    {
        return(m_bGeneralMasterActive);
    }

    static inline bool isMasterID(uint32 threadID)
    {
        return (m_MasterID == threadID);
    }
    static uint16 getVarMetaHash(uint32 signature);
  
    static inline unsigned getBreakpointListVersion()   {   return(m_breakpointListVersion);    };      // Counter for breakpointlist changes (inital value: 1)
    static inline void incBreakpointListVersion()       {   m_breakpointListVersion++;  };      // Counter for breakpointlist changes (inital value: 1)
    static inline void resetBreakpointListVersion()     {   m_breakpointListVersion = 1;    };      // Counter for breakpointlist changes (inital value: 1)
    static inline bool breakpointListVersionChanged(unsigned lastVersion)   {   return(m_breakpointListVersion != lastVersion); };      // Counter for breakpointlist changes (inital value: 1)

    static inline unsigned getMetaDataVersion()     {   return(m_metaDataVersion);  };              // Counter for metaData changes (inital value: 1)
    static inline void incMetaDataVersion()         {   m_metaDataVersion++;    };              // Counter for metaData changes (inital value: 1)
    static inline void resetMetaDataVersion()       {   m_metaDataVersion = 1;  };              // Counter for metaData changes (inital value: 1)
    static inline bool metaDataVersionChanged(unsigned lastVersion) {   return(m_metaDataVersion != lastVersion);   };              // Counter for metaData changes (inital value: 1)

    static inline bool getSourceOnPlc(bool& sourceOnPlc)    {sourceOnPlc=m_bSourceOnPlc; return m_bSourceOnPlcUpToDate;};//returns false if not up to date
    static inline void setSourceOnPlc(bool sourceOnPlc)     {m_bSourceOnPlc=sourceOnPlc; m_bSourceOnPlcUpToDate=true;};
    static inline void setSourceOnPlcInvalid()              {m_bSourceOnPlcUpToDate=false;};

    static inline bool getImageOnPlc(bool& ImageOnPlc)      {ImageOnPlc=m_bImageOnPlc; return m_bImageOnPlcUpToDate;};//returns false if not up to date
    static inline void setImageOnPlc(bool ImageOnPlc)       {m_bImageOnPlc=ImageOnPlc; m_bImageOnPlcUpToDate=true;};
    static inline void setImageOnPlcInvalid()               {m_bImageOnPlcUpToDate=false;};
};






#endif // __COMMUNICATION_MANAGER_H__
