/*
+------------------------------------------------------------------------------+
| ************************** DEVICEINFOSERVICE.H ***************************** |
+------------------------------------------------------------------------------+
|                       Copyright 2005 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| Handling of device info services (e.g. device type, firmware,..)             |
+------------------------------------------------------------------------------+
| Author: Jörg Kraemer, started at: 29.09.2005                                 |
| Changes have to be marked with an user time stamp (e.g. '// NN, 08.11.2003') |
+------------------------------------------------------------------------------+
*/

#ifndef __DEVICE_INFO_SERVICE_H__
#define __DEVICE_INFO_SERVICE_H__

#include "Services.h"

  typedef enum _CombinedPlcState
  {
      CombinedPlcState_PlcOn=0x0,
      CombinedPlcState_PlcLoading=0x1,
      CombinedPlcState_PlcStarting=0x2,
      CombinedPlcState_PlcRunning=0x3,
      CombinedPlcState_PlcHaltRequested=0x4,
      CombinedPlcState_PlcHalt=0x5,
      CombinedPlcState_PlcStopping=0x6,
      CombinedPlcState_PlcStop=0x7,
      CombinedPlcState_PlcResetting=0x8,
      CombinedPlcState_MasterSet=0x100,
      CombinedPlcState_IAmMaster=0x200,
      CombinedPlcState_HotStartPossible=0x400,
      CombinedPlcState_ErrorsOnClr=0x800,
      CombinedPlcState_SourceOnClr=0x1000,
      CombinedPlcState_CallstackAvailable=0x2000,
      CombinedPlcState_BreakpointSet=0x04000,
      CombinedPlcState_CallstackChanged=0x8000,
      CombinedPlcState_ForceListChanged=0x01000000,
      CombinedPlcState_ForceListAvailable=0x02000000
  } CombinedPlcState;

  typedef enum _StandardDeviceAttributes
  {
      StandardDeviceAttributes_Manufacturer=0x0,
      StandardDeviceAttributes_ProductName=0x1,
      StandardDeviceAttributes_HardwareVersion=0x3,
      StandardDeviceAttributes_FirmwareVersion=0x4,
      StandardDeviceAttributes_EClrCpuType=0x2,
      StandardDeviceAttributes_EClrMaxApplicationDataSize=0x5,
      StandardDeviceAttributes_EClrMaxApplicationRetainDataSize=0x6,
      StandardDeviceAttributes_EClrMaxApplicationProgramSize=0x7,
      StandardDeviceAttributes_EClrMaxApplicationImageSize=0x8,
      StandardDeviceAttributes_EClrMaxRuntimeHeapSize=0x9,
      StandardDeviceAttributes_EClrTimerResolution=0xA,
      StandardDeviceAttributes_EClrRuntimeVersion=0xB,
      StandardDeviceAttributes_RuntimeType=0xC,
      StandardDeviceAttributes_EClrThreadingType=0xD,
      StandardDeviceAttributes_EClrScheduleInterval=0xE,
      StandardDeviceAttributes_EClrThreadingPriorityLevel=0xF,
      StandardDeviceAttributes_EClrMaxAdditionalApplImageFilesSize=0x10,
      StandardDeviceAttributes_EClrDownloadChangeSupported=0x11,
      StandardDeviceAttributes_EClrBuiltinLinkerSupported=0x12,
      StandardDeviceAttributes_EClrFreeApplicationDataSize=0x4000,
      StandardDeviceAttributes_EClrFreeApplicationProgramSize=0x4001,
      StandardDeviceAttributes_CombinedClrStates=0x4002,
      StandardDeviceAttributes_SourceOnPlc=0x4003,
      StandardDeviceAttributes_Running=0x4004,
      StandardDeviceAttributes_EClrFreeRetainDataSize=0x4005,
      StandardDeviceAttributes_Identity=0x4006,
      StandardDeviceAttributes_EClrAppDomainName=0x4007,
      StandardDeviceAttributes_ImageOnPlc=0x4008,
      StandardDeviceAttributes_RetainCRC=0x4009,
      StandardDeviceAttributes_VariablesHashValue=0x400A,
      StandardDeviceAttributes_EClrMetaCRC=0x400B,
      StandardDeviceAttributes_EClrCodeCRC=0x400C,
      StandardDeviceAttributes_EClrImageTime=0x400D,
      StandardDeviceAttributes_EClrBootProjName=0x400E,
      StandardDeviceAttributes_EClrStopOnStringException=0x400F,
      StandardDeviceAttributes_EClrAllowRealtimeViolationDuringOnlineChange=0x4010,
      StandardDeviceAttributes_EClrInFlashExecutionSupported=0x4011,
      StandardDeviceAttributes_EClrPhysicalCodeAddress=0x4012,
      StandardDeviceAttributes_EClrPhysicalDataAddress=0x4013,
      StandardDeviceAttributes_EClrTickCount=0x4014,
      StandardDeviceAttributes_EClrMicroTickCount=0x4015,
      StandardDeviceAttributes_LogicAnalyzerState=0x4016,
      StandardDeviceAttributes_EClrUtcTicks=0x4E20,
      StandardDeviceAttributes_Any
  } StandardDeviceAttributes;

class CDeviceInfoService : public CServiceImplBase
{
public:
    CDeviceInfoService();
    ~CDeviceInfoService();
    void Invoke(sint16 methodHandle, char* const pDataBuffer);

    static CServiceImplBase* CreateInstance();

	unsigned lastForceListVersion;

protected:

   //Service method [1]: void GetAttribute(Int16 attributeId, out Object& value);
    void GetAttribute();

   //Service method [2]: void GetAttributeNames(out String[]& attributes);
    void GetAttributeNames();

   //Service method [3]: void GetAttributes(Int16[] attributeIds, out Object[]& values);
    void GetAttributes();

   //Service method [4]: void SetAttribute(Int16 attributeId, Object value);
    void SetAttribute();

   //Service method [5]: void SetAttributes(Int16[] attributeIds, Object[] values);
    void SetAttributes();

private:
    uint32 CombinedClrStates();
    void getSingleAttribute(int attributeId);
    bool setSingleAttribute(int attributeId, uint32 value);
    bool isSourceOnPlc();
    bool isImageOnPlc();
	bool hasForceListChanged();
    bool isCallstackChanged();
    bool isCallstackAvailable();
    static bool isBreakpointSet();
};


#endif //__DEVICE_INFO_SERVICE_H__
