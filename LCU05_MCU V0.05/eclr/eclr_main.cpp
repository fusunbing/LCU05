
// ----------------------------------------------------------------------------
// Includes 
// ----------------------------------------------------------------------------
#include "eclr.h"
#include "controller.h"
#include "pcosdomain.h"
#include "remoting.h"
#include "log.h"
#include "eclrImageFile.h"
#include "eclrEnvironment.h"
#include "econfig.h"
#include "remoting.h"
#include "BinaryReader.h"
#include "BinaryWriter.h"

// Includes for socket based communication.
#include "Sersocket.h"
#include "Connlayer.h"
#include "bsdsocket.h"
#include "SHM_LIB.h"
#include "IO_DRIVER_LIB.h"
#include "RT_TICK.h"

//RT-Thread Kernel
#include <rtthread.h>

// Others
#include "config.h"

#include "stm32f4xx.h"
// ----------------------------------------------------------------------------
// Definitions
// ----------------------------------------------------------------------------
#define GLOBAL_OPERATOR_NEW_OVERRIDE

// ----------------------------------------------------------------------------
// User Type Declarations
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Data Object Declarations
// ----------------------------------------------------------------------------

//static char eclr_object_heap[OBJECT_HEAP_SIZE];
static char plc_data_ram[DATA_RAM_SIZE];

#ifndef EXECUTION_INPLACE_SUPPORT
static char plc_code[PLC_CODE_SIZE];
#endif

// Small space for simulated retain data.
//static char loc_retain_heap[RETAIN_AMOUNT]; 

// ----------------------------------------------------------------------------
// Private Method Declarations
// ----------------------------------------------------------------------------


// ============================================================================
// Start of Implementation
// ----------------------------------------------------------------------------
#ifdef GLOBAL_OPERATOR_NEW_OVERRIDE
void* operator new(size_t size)
{
  volatile int a =0;
    void* p = ClrController::s_objectHeap.alloc(ClrGcHeap::DataObject, size);
    if(p== NULL)
      a=1;
    return p;
}

void* operator new[](size_t size)
{
  volatile int a =0;
    void* p = ClrController::s_objectHeap.alloc(ClrGcHeap::DataObject, size);
    if(p== NULL)
      a=1;
    return p;
};

void operator delete(void* p)
{
    ClrController::s_objectHeap.free(p);
}

void operator delete[](void* p)
{
    ClrController::s_objectHeap.free(p);
}
#endif

#ifdef PLATFORM_THREADING_SUPPORT
static void setPriorityScheme()
{
     // only one prio level available: remoting task
     mscorlib::PriorityClass::SetLowest(LOWER_PRIO, LOWER_PRIO);
     // 19 level available
     mscorlib::PriorityClass::SetNormal(NORM_LOW_PRIO, NORM_HIGH_PRIO);
     // only one prio level available:
     mscorlib::PriorityClass::SetHighest(HIGH_PRIO, HIGH_PRIO);
} 
#endif  //PLATFORM_THREADING_SUPPORT

void LibLoad(void);

uint8_t g_retain[256];

extern "C" int eclr_main()
{
#ifdef PLATFORM_THREADING_SUPPORT
    setPriorityScheme();
#endif

    //if (ClrController::Create("Default", eclr_object_heap,OBJECT_HEAP_SIZE) == NULL)
    if (ClrController::Create("Default", ECLR_HEAP_START,ECLR_HEAP_SIZE) == NULL)
    {
        //printf("Controller setup failed\n");
        return -1;
    }

    CDeviceInfo* pDeviceInfo = ClrController::getDeviceInfo();
    pDeviceInfo->setAttribute(CDeviceInfo::Manufacturer, "KW");
    
    pDeviceInfo->setAttribute(CDeviceInfo::HardwareVersion, "1.0");
    pDeviceInfo->setAttribute(CDeviceInfo::FirmwareVersion, "1.0");
    pDeviceInfo->setAttribute(CDeviceInfo::MaxApplRetainDataSize, (uint32)0);
    pDeviceInfo->setAttribute(CDeviceInfo::TimerResolution, 1000);
    pDeviceInfo->setAttribute(CDeviceInfo::ScheduleInterval, 1000);

    LibLoad();
#ifdef EXECUTION_INPLACE_SUPPORT
    pDeviceInfo->setAttribute(CDeviceInfo::ProductName, "eCLR2.2 for FreeRTOS-Cortex M4 XIP");
    ClrAppDomain* pAppDomain = ClrController::CreateDomain("Default", ClrAppDomain::ExecutionInFlash);
#else
    pDeviceInfo->setAttribute(CDeviceInfo::ProductName, "eCLR2.2 for FreeRTOS-Cortex M4 XIR");
    //ClrAppDomain* pAppDomain = ClrController::CreateDomain("Default", ClrAppDomain::DownloadChangeSupport);
    ClrAppDomain* pAppDomain = ClrController::CreateDomain("Default", ClrAppDomain::CreationDefault);
#endif
    
    if (pAppDomain == NULL)
    {
        //printf("Setup of application domain failed.\n");
        return -1;
    }

    
    pAppDomain->SetDataMemory(plc_data_ram, DATA_RAM_SIZE);
    
#ifdef EXECUTION_INPLACE_SUPPORT
    PcosImageFile* appImgFile = new PcosImageFile();
    pAppDomain->SetCodeMemory(appImgFile->GetBase(), appImgFile->GetSize());
    pDeviceInfo->setAttribute(CDeviceInfo::MaxApplImageFilesSize, appImgFile->GetSize());
    pDeviceInfo->setAttribute(CDeviceInfo::MaxApplImageSize, appImgFile->GetSize());

    if (!appImgFile->IsValid())          // if image stream is not valid, i.e., no valid boot project
    {
       if(!appImgFile->InitImageHdr())   // if initialize the image header fails, return -1
       {
            return -1;
       }
    }

    pAppDomain->SetImageStream(appImgFile);
#else
    pAppDomain->SetCodeMemory((void*)plc_code, PLC_CODE_SIZE);
#endif

    CPcosDomain* pPcosDomain = pcoslib::CreateDomain(pAppDomain);
    
    if (pPcosDomain == NULL)
    {
        //printf("Setup of pcos domain failed\n");
        return -1;
    }

#ifdef PLATFORM_THREADING_SUPPORT
    pPcosDomain->SetTicksPerSecond(1000); // scheduler is working with time base of EclrEnvironment
#endif

    pPcosDomain->SetCPUCapacity(1000,   // int calcFreq_ms
                                30,     // int minCPULoadPerCent
                                80,     // int maxCPULoadPerCent
                                2 );    // int minFreeTicks

    //pPcosDomain->AnnounceRetentiveMemory(loc_retain_heap, sizeof(loc_retain_heap));
    pPcosDomain->AnnounceRetentiveMemory(&g_retain, sizeof(g_retain));
    
    //Start
    pPcosDomain->WarmStartBootReq();
    
    
#ifdef SERIAL_SOCKET_SUPPORT
    // Activate serial communication interface.
    SerSocket* pSerSocket = new SerSocket();
    if (pSerSocket != NULL)
    {
        // Open socket on COM port 1. COM port 2 is not supported by now.
        if (pSerSocket->Open(1, SerSocket::Baud_115200, 8, SerSocket::StopBits_1, SerSocket::Parity_None) == true)
        {
            ConnLayer* pConn = new ConnLayer();
            if (pConn != NULL)
            {
                // open the connection layer
                if (pConn->Open(pSerSocket, 1024) == true)
                {
                    pConn->SetRetransmitionTimeout(1000);
                    CRemotingDeamon::addSocket(pConn);
                }
                pConn->release();
            }
        }
        pSerSocket->release();
    }    
#endif //SERIAL_SOCKET_SUPPORT
    
#ifdef BSD_SOCKET_SUPPORT    
        BsdSocket* pBsdSocket = new BsdSocket();    
        if (pBsdSocket != NULL)
        {
          if (pBsdSocket->Open(BsdSocket::Stream, CRemotingDeamon::ClrServerPort) == true)
            {           
            CRemotingDeamon::addSocket(pBsdSocket);        
          }        
          pBsdSocket->release();      
        }    
 #endif  //BSD_SOCKET_SUPPORT
     
    //Start Remoting
    CRemotingDeamon::start();
        
    while (true)
    {
        ClrController::Process();
        CThreadImpl::Sleep(1);
    }

}

