/*
+------------------------------------------------------------------------------+
| ************************** VARIABLELISTMANAGER.H *************************** |
+------------------------------------------------------------------------------+
|                       Copyright 2005 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| Manages list of variables for debug                                          |
+------------------------------------------------------------------------------+
                                                                               |
| Changes have to be marked with an user time stamp (e.g. '// NN, 08.11.2003') |
+------------------------------------------------------------------------------+
*/

#ifndef __VARIABLE_LIST_MANAGER_H__
#define __VARIABLE_LIST_MANAGER_H__

#include "pcoslib.h"

class StringToken : public ClrAllocator
{
public:
    StringToken(ClrAppDomain* pAppDomain);
    ~StringToken();
    bool Init(unsigned stringTabSize, unsigned stringCount, unsigned variableCount);
    bool Parse(CBinaryReader* pReader, PcosDebugValue* m_pDebugValueTab, bool& isFinished);

private:
    void init();
    ClrAppDomain* m_pAppDomain;
    char*      m_pStringBuffer;
    unsigned    m_stringBufferSize;
    const char** m_ppStrings;
    const char* m_varNamesTab[32];
    unsigned    m_stringCount;
    unsigned    m_stringIndex;
    unsigned    m_varsCount;
    unsigned    m_varsIndex;
    unsigned    m_varsTokenCount;
    unsigned    m_varsTokenIndex;
    StreamIO    m_stream;

};

class DataList : public eListNode<DataList>, public ClrAllocator
{
public:
    DataList(ClrAppDomain* pAppDomain, bool isRaw);
    ~DataList();
    bool Init(unsigned stringTabSize, unsigned stringCount, unsigned variableCount);
    bool Init(CBinaryReader* pReader);
    bool IsReady()
    {
        return m_isInitialized;
    }

    bool Read(CBinaryWriter* pWriter);  
    bool Write(CBinaryReader* pReader, void* pValues, unsigned valueLength, bool& isFinished);

    inline int GetHandle()
    {
        return m_handle;
    }
    static bool readValue(PcosDebugValue& debugValue, CBinaryWriter* pWriter, bool isRaw);
    static bool readValue(CBinaryWriter* pWriter, int dataSize, ClrInt64& val);
    static bool writeValue(PcosDebugValue& debugValue, CBinaryReader* pReader, bool& isFinished);
private:
    void init();
    void release();
    StringToken*    m_pStringToken;
    PcosDebugValue* m_pDebugValueTab;
    ClrAppDomain*   m_pAppDomain;
    void*           m_pStreamBuffer;
    unsigned        m_streamBufferSize;
    unsigned        m_streamStart;
    unsigned        m_itemCount;
    unsigned        m_currItem;
    int             m_handle;
    bool            m_isRaw;
    bool            m_isInitialized;

};

extern bool skipIECTypeIndexer(char* buffer);

#endif //__VARIABLE_LIST_MANAGER_H__
