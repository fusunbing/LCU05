/*
+------------------------------------------------------------------------------+
| ************************** DATAACCESSSERVICE.H ***************************** |
+------------------------------------------------------------------------------+
|                       Copyright 2005 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| Handling of data services (e.g. read, write,..)                              |
+------------------------------------------------------------------------------+
| Author: Jörg Kraemer, started at: 29.09.2005                                 |
| Changes have to be marked with an user time stamp (e.g. '// NN, 08.11.2003') |
+------------------------------------------------------------------------------+
*/

#ifndef __DATA_ACCES_SERVICE_H__
#define __DATA_ACCES_SERVICE_H__

#include "Services.h"
#include "VariableListManager.h"

class CDataAccessService : public eListNode<CDataAccessService>, public CServiceImplBase
{
public:
    CDataAccessService();
    ~CDataAccessService();
    void Invoke(sint16 methodHandle, char* const pDataBuffer);

    static CServiceImplBase* CreateInstance();

    // support DataAccessService collection
    typedef eList<CDataAccessService>   DASList;
    typedef DASList::iterator           DASListIter;

    static DASList  s_dasList;
    static void ClearVarlists();
    
    typedef eList<DataList>             DataGroupList;
    typedef DataGroupList::iterator     DataGroupListIter;

    static void Ctors();

private:


   //Service method: void ContinueCreatingGroup(Int16 groupHandle, Byte[] variables);
    void ContinueCreatingGroup();

   //Service method: UInt32 ContinueReadDataList(Byte[] variables, out Byte[]& values);
    void ContinueReadDataList();


   //Service method: UInt32 ContinueWriteDataList(Byte[] variables, Byte[]& values);
    void ContinueWriteDataList();


   //Service method: void DeleteGroup(Int16 groupHandle);
    void DeleteGroup();

   //Service method: UInt32 Read(String itemName, out Byte[]& value);
    void Read();

   //Service method: UInt32 ReadGroup(Int16 groupHandle, out Byte[]& value);
    void ReadGroup();

   //Service method: Int16 StartCreatingGroup(UInt32 stringFieldSize, UInt16 stringTokenCount, UInt16 variableCount, UInt32 listDataLength, Byte[] variables);
    void StartCreatingGroup();

   //Service method: UInt32 StartReadDataList(UInt32 stringFieldSize, UInt16 stringTokenCount, UInt16 variableCount, UInt32 listDataLength, Byte[] variables, out Byte[]& values);
    inline void StartReadDataList()
    {
        startReadDataList(false);
    }
   //Service method: UInt32 StartWriteDataList(UInt32 stringFieldSize, UInt16 stringTokenCount, UInt16 variableCount, UInt32 listDataLength, Byte[] variables, Byte[]& values);
    inline void StartWriteDataList()
    {
        startWriteDataList(false);
    }

   //Service method: UInt32 StartReadRawDataList(UInt32 stringFieldSize, UInt16 stringTokenCount, UInt16 variableCount, UInt32 listDataLength, Byte[] variables, out Byte[]& values);
    inline void StartReadRawDataList()
    {
        startReadDataList(true);
    }

    //Service method: UInt32 StartWriteRawDataList(UInt32 stringFieldSize, UInt16 stringTokenCount, UInt16 variableCount, UInt32 listDataLength, Byte[] variables, Byte[]& values);
    inline void StartWriteRawDataList()
    {
        startWriteDataList(true);
    }

    //Service method: UInt32 ContinueReadDataList(Byte[] variables, out Byte[]& values);
    void ContinueReadRawDataList();

    //Service method: UInt32 ContinueWriteDataList(Byte[] variables, Byte[]& values);
    void ContinueWriteRawDataList();

    //Service method: UInt32 Write(String itemName, String value);
    void Write();

private:
    UInt32 ClrState();
    DataList*               m_pDataList;
    DataGroupList           m_dataGroupList;
    void clearDataGroupList();
    void responseWriteRequest(bool result);
    void responseReadRequest(bool result);
    void sendError(uint32 clrState, uint32 commError);
    static DataList* new_DataList(unsigned stringTabSize, unsigned stringCount, unsigned variableCount, bool isRaw);
    bool createDataList(unsigned stringTabSize, unsigned stringCount, unsigned variableCount, bool isRaw);
    bool continueReadDataList(uint8* pData, unsigned dataSize);
    bool continueWriteDataList(uint8* pData, unsigned dataSize, uint8* pValues, unsigned valuesSize);
    DataList* getDataList(int handle);

    void startReadDataList(bool bRaw);
    void startWriteDataList(bool bRaw);
    void continueReadDataList();
    void continueWriteDataList();
};

#endif //__DATA_ACCES_SERVICE_H__
