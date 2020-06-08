
/*
+------------------------------------------------------------------------------+
| ******************************* UAINTERFACE.H ****************************** |
+------------------------------------------------------------------------------+
|                       Copyright 2009 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| Handling of logic analyzer and OPC UA services (e.g. add, remove,..)         |
+------------------------------------------------------------------------------+
| Author: Sascha Hollmann, started at: 16.09.2009                              |
| Changes have to be marked with an user time stamp (e.g. '// NN, 08.11.2003') |
+------------------------------------------------------------------------------+
*/

#ifndef __UAINTERFACE_H__
#define __UAINTERFACE_H__

#include "Services.h"
#include "StandardServices.h"
#include "VariableListManager.h"
#include "eclr.h"
#include "appdomain.h"
#include "controller.h"
#include "pcosdomain.h"
#include "log.h"

#define HISTORICAL 0x01
#define TRIGGERED  0x02
#define CYCLIC     0x03
#define CONTINUOUS 0x04

/*****************************************************************/
/* struct only for MP and trigger conditions                     */
/*****************************************************************/
typedef struct _InternalTriggerCondition
{
    bool bBothAreVariables;
    char TriggerVariable[2][128];
    MetaElementType DatatypeOfConstValue;
    ClrInt64 constantValue;
    _TriggerConditionOperation TriggerCondition;
}InternalTriggerCondition;


/*****************************************************************/
/* Class to extract Token names and generate list of debug values*/
/*****************************************************************/
class DebugValueNode : public eListNode<DebugValueNode> , public PcosDebugValue
{
public:
    // just for debug purposes
    //eString<128> m_path;
};

class SubscriptionList;
class CTokenNames 
{

public:

    CBinaryReader2* m_pBinaryReader;

    CTokenNames();
    ~CTokenNames();
    void Release();


    bool GenerateDebugValueObjectList(CBinaryReader2* pBinaryReader, SubscriptionList* pSubscriptionList,uint32 lenghtOfToken);
   
    const char* parseInstanceFields(const char* p, PcosDebugValue& debugValue, eString<128>& path);
    const char* parseStaticFields(const char* p, ClrTypeDef* pTaskType);
    void getNextToken()
    {
        m_pBinaryReader->ReadString(m_tokenName, sizeof(m_tokenName));
    }
    void getNameSpace()
    {
        m_pBinaryReader->ReadString(m_nameSpace, sizeof(m_nameSpace));
    }
    void getTaskType()
    {
        m_pBinaryReader->ReadString(m_taskType, sizeof(m_taskType));

    }
    bool checkEndOfTokenList(const char* TokenName);

    SubscriptionList* m_pSubscriptionList;
    char m_tokenName[128];
    char m_nameSpace[64];
    char m_taskType[64];
    eString<128> path;
    uint32 m_nSizeOfBuffer;
    unsigned     m_lastError;
    uint32 m_lenghtOfToken;

    ClrAppDomain* m_pAppDomain;

    #define SCOPE_BEGIN  "{"
    #define SCOPE_END    "}"
    #define TOKENNAMES_END "0"
    
};


class TaskCallbackForCopyOfData : public PcosTaskCallback
{
public:
    TaskCallbackForCopyOfData(SubscriptionList* SubscriptionObject, short sKindOfRecord);
 
   void ProcessBegin();
 
   void ProcessEnd();

   SubscriptionList* m_CallBacksubscriptionObject;
   short m_sKindOfRecord;
   sint32 m_TempCycleCount;
   char* m_pTempBuffer;
   char* m_pBufferWorker;
   short m_ValueForCondition;
   bool result;
   bool OnlyOnce;
   uint32 m_MicroTick;
   bool m_bAfterEvent;
   uint32 m_CycleCounter;
   sint32 m_PostCycles;
 

};


/******************************************************************/
/* Subscription class to hold the information of each Subscription*/
/******************************************************************/
class SubscriptionList : public eListNode<SubscriptionList>, public ClrAllocator
{
public:
    SubscriptionList(/*ClrAppDomain* pAppDomain*/);
    ~SubscriptionList();
    bool CheckTriggerCondition(ClrInt64 TriggerValue1, ClrInt64 TriggerValue2, TriggerConditionOperation TriggerCondition);
    void RecordPostCycle();
    void SetSendBuffer(char* pBuffer);
    void SetActiveBuffer(char* pBuffer);
    void SwitchSendBuffer();
    bool StoreDebugValue(DebugValueNode* pDebugValueElement);
    bool CreateBufferForSubscription(uint32 SizeOfBuffer);
    bool RegisterTaskCallback();

    inline char* GetSendBuffer()
    {
       return m_pSendBuffer;
    } 
    inline char* GetActiveBuffer()
    {
       return m_pActiveBuffer;
    }

    inline sint32 GetHandle()
    {
        return m_SubscriptionHandle;
    }
    typedef eList<DebugValueNode>       DebugValueList;
    typedef DebugValueList::iterator    DebugValueListIter;

    PcosDebugValue*          m_pPcosDebugValue;
    DebugValueList           m_debugValueList;

   bool Init(sint32 SubscriptionHandle);
   _Subscription* m_subscriptionInfo;
   
   TaskCallbackForCopyOfData* m_pTaskCallback;

   CPcosDomain* m_pPcosDomain;

   _InternalTriggerCondition* m_pTriggercondition;
   
   char* m_pBuffer1;
   char* m_pBuffer2;
   char* m_pLastPreCycle;
      
   uint32 m_uBufferSizeOfDataTypes;
   sint32 m_SizeOfCompleteBuffer;
   sint32 m_uCompleteBufferWithoutPostCycles;

   sint32 m_preCycleCounter;
   sint32 m_CompleteCycles;
   
   short m_sKindOfRecord;

   PcosDebugValue m_debugValue1;
   PcosDebugValue m_debugValue2;
   ClrInt64 m_TriggerValue1;
   ClrInt64 m_TriggerValue2;
   MetaElementType m_DataTypOfTriggerValue1;
   MetaElementType m_DataTypOfTriggerValue2;

   bool m_ValuesNotSend;
   bool m_FirstCheck;
   bool m_TaskLinkedToSubscription;
   ClrInt64 m_StoredTriggerValue1;
   CTokenNames* m_pTokenNames;
   uint16 m_nameBuffer[16];

   bool m_TriggerConditionReached;
   _NotificationPeriod StateOfTrigger;
   bool m_bOnlyOnce;

private:
   void init();
   void release();
   sint32          m_SubscriptionHandle;
   char* m_pSendBuffer;
   char* m_pActiveBuffer;
   

};

/******************************************************************/
/* Client class to hold the information of each Client            */
/******************************************************************/
class ClientList : public eListNode<ClientList>, public ClrAllocator
{
public:
    ClientList();
    ~ClientList();
    bool Init(sint32 ClientHandle);
    SubscriptionList* generateSubscription(_Subscription *pSubscriptionStruct, _InternalTriggerCondition* pTriggercondition);

    inline sint32 GetHandle()
    {
        return m_ClientHandle;
    }
    typedef eList<SubscriptionList>             SubscriptionGroupList;
    typedef SubscriptionGroupList::iterator     SubscriptionGroupListIter;

    SubscriptionList*               m_pSubscriptionListEntry;
    SubscriptionGroupList           m_SubscriptionGroupList;

    SubscriptionList* FindSubscriptionHandle(sint32 SubscriptionHandle);
    bool createSubscriptionListEntry(_Subscription *pSubscriptionStruct, _InternalTriggerCondition* pTriggercondition);
    bool DeleteSubscriptionListEntry(sint32 SubscribtionHandle);
    /*TODO !!!!!!!! */
    /* Function to write values direct to the processimage of the PLC project*/

private:
    void init();
    void release();
    int             m_handle;

    sint32 m_ClientHandle;
    unsigned long m_TimeOutValue;

};


class CUAInterface : public ClrAllocator
{
public:
   CUAInterface();
   ~CUAInterface();
   typedef eList<ClientList>             ClientGroupList;
   typedef ClientGroupList::iterator     ClientGroupListIter;

   SubscriptionList* CreateSubscription(_Subscription *pSubscriptionStruct, _InternalTriggerCondition* pTriggercondition, sint32 ClientHandle);

   //void GetData(short SubscriptionHandle, short ClientHandle); //needed for OPC UA (direct access to buffer)
    
   //void CopyValueToBuffer(SubscriptionList * pSubscription, char* pSendbuffer, CBinaryWriter * m_pBinaryWriter);
   sint32 CreateClientHandle(sint32 clinetHandle);

   ClientList* FindClientHandle(sint32 ClientHandle);

   bool ReleaseClientHandle(sint32 ClientHandle);

   //void SetTimeOutOption(long TimeOutValue);

   sint32 GenerateClientHandle();

   bool createClientList(sint32 ClientHandle);

   ClientList* m_pClientList;    
   ClientGroupList           m_ClientGroupList;
   CPcosDomain* m_pPcosDomain;
   sint32 m_ClientHanlde;
   sint32 m_sLastValue;
   bool m_oldSubscription;
   bool m_bClientExists;

   static void Ctors();

   typedef eList<ClientList>       ClientGList;
   typedef ClientGList::iterator   ClientGListtIter;
   static  ClientGList             s_ClientGList;

};


#endif //__UAINTERFACE_H__


