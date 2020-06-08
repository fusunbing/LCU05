// IO_DRIVER_LIB.cpp native assembly code template.
// KW-Software embedded CLR native assembly builder tool generated source file.
// use this file to implement your own native code

#include "rtthread.h"
#include "eclr.h"
#include "IO_DRIVER_LIB.h"
#include "stm32f4xx.h"
#include "userApp.h"


static uint8 in_buffer[128] = {0};
static uint8 out_buffer[128] = {0};
int x[10] = {0};

bool IO_DRIVER_LIB::init()
{
// if necessary, insert some initialization code for the IO_DRIVER_LIB library here !
// call IO_DRIVER_LIB::init() somewhere before IO_DRIVER_LIB::loadLibrary() is called.
// loadLibrary() have to be called after the ClrContoller object is instanciated
// for detailed information see the native library builder documentation
    return true;
}

bool IO_DRIVER_LIB::isCompatible(const EclrVersion& requestedVersion)
{
// this function is called by the runtime if an application will be bound
// to this library with the requested version.
// return true means that this library fits to the requested interface.
// return false forces the runtime to search for other libraries with the same name by calling isCompatible()
// if no libraries could be found, the runtime breaks the loading process with linker-error
// for multiple library version support read the documentation.
    return true;
}

bool IO_DRIVER_LIB::load(ClrAppDomain* pAppDomain)
{
// This function is called by the runtime if an application will be started.
// pAppDomain is the domain object witch hosts the current application process.
// At this point, the firmware library is able to initialize some internal objects.
// If the initialization is done ok, return true
// othewise return false.
    return true;
}

bool IO_DRIVER_LIB::unload(ClrAppDomain* pAppDomain)
{
// This function is called by the runtime if an application will be terminated.
// pAppDomain is the domain object witch hosts the current application process.
// At this point, the firmware library is able to finalize some internal objects.
// If the finalization is done ok, return true
// othewise return false.
    return true;
}

// class IO_DRIVER_LIB.KWIODriver implementation
IO_DRIVER_LIB::KWIODriver::KWIODriver()
{
// implement your constructor code here !
    m_pCurrentBuffer = in_buffer;

    for(int i = 0; i < 8; i++)
    {
        m_pInBufferTable[i]  = in_buffer;
        m_pOutBufferTable[i] = out_buffer;
    }
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::ctor()
{
// automatically generated auxiliary contructor : calls the contructor by displacement new operator !
// @Begin automatically generated code, do not modify !
    new (this) IO_DRIVER_LIB::KWIODriver();
// @End automatically generated code
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::Init(SZArray* p0, SZArray* p1)
{
// implement your code here !
    // check the single-dimensional zero based arrray (SZArray) of IOGroup arguments
    if (p0 != NULL)
    {
        int nGroups                 = p0->getLength();
        int startAddr = 0;
	    pcoslib::IOGroup* pIoGroups = (pcoslib::IOGroup*)p0->getData();
        for (int i = 0; i < nGroups; i++)
        {
            int addr = pIoGroups[i].addr;
            x[0] = addr;
		    int size = pIoGroups[i].size;
		    SZArray* pSZArray  = pIoGroups[i].param;
            if (pSZArray != NULL)
            {
	            uint16* pParam  = (uint16*)pSZArray->getData();
                unsigned nParam = pSZArray->getLength();
            }
            if (i == 0)
            {
                startAddr = addr;
                m_pInBufferTable[i] = &in_buffer[0];
            }
            else
            {
                addr = addr - startAddr;
                m_pInBufferTable[i] = &in_buffer[addr];
            }
        }
    }
    
    
    if (p1 != NULL)
    {
        int nGroups                 = p1->getLength();
        int startAddr = 0;
	    pcoslib::IOGroup* pIoGroups = (pcoslib::IOGroup*)p1->getData();
        for (int i = 0; i < nGroups; i++)
        {
            int addr = pIoGroups[i].addr;
		    int size = pIoGroups[i].size;
		    SZArray* pSZArray  = pIoGroups[i].param;
            if (pSZArray != NULL)
            {
	            uint16* pParam  = (uint16*)pSZArray->getData();
                unsigned nParam = pSZArray->getLength();
            }
            if (i == 0)
            {
                startAddr = addr;
                m_pOutBufferTable[i] = &out_buffer[0];
            }
            else
            {
                addr = addr - startAddr;
                m_pOutBufferTable[i] = &out_buffer[addr];
            }
        }
    }
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::Open()
{
// implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::Close()
{
// implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::BeginReadAll()
{
    // implement your code here !
    uint8_t *p;

    p = m_pInBufferTable[0];
    
    Input_KW(p);
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::BeginReadGroup(Int32 p0)
{
    // implement your code here !
    m_pCurrentBuffer = m_pInBufferTable[p0];
}

SByte __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadSByte(Int32 p0)
{
    // implement your code here !
    SByte val = (SByte)m_pCurrentBuffer[p0];
    return val;
}

Int16 __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadInt16(Int32 p0)
{
    // implement your code here !
    Int16 val = (Int16)(((uint32)m_pCurrentBuffer[p0+1]<<8) + m_pCurrentBuffer[p0]);
    return val;
}

Int32 __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadInt32(Int32 p0)
{
    // implement your code here !
    Int32 val = (Int32) ( ((uint32)m_pCurrentBuffer[p0+3]<<24) + 
                          ((uint32)m_pCurrentBuffer[p0+2]<<16) + 
                          ((uint32)m_pCurrentBuffer[p0+1]<<8) + 
                           (uint32)m_pCurrentBuffer[p0]);
    return val;
}

Byte __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadByte(Int32 p0)
{
    // implement your code here !
    Byte val = (Byte)m_pCurrentBuffer[p0];
    return val;
}

UInt16 __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadUInt16(Int32 p0)
{
    // implement your code here !
    return 0;
}

UInt32 __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadUInt32(Int32 p0)
{
    // implement your code here !
    return 0;
}

UInt32 __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadSingle(Int32 p0)
{
    // implement your code here !
    return 0;
}

UInt64 __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadDouble(Int32 p0)
{
    // implement your code here !
    return 0;
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadByteBuffer(Int32 p0, Byte* p1, Int32 p2)
{
    rt_memcpy(p1, &m_pCurrentBuffer[p0], p2);
    // implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::EndReadGroup()
{
    // implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::EndReadAll()
{
    // implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::BeginWriteAll()
{
    // implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::BeginWriteGroup(Int32 p0)
{
    // implement your code here !
    m_pCurrentBuffer = m_pOutBufferTable[p0];
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteSByte(Int32 p0, SByte p1)
{
    // implement your code here !
    
    m_pCurrentBuffer[p0] = (uint8)p1;
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteInt16(Int32 p0, Int16 p1)
{
    // implement your code here !
    m_pCurrentBuffer[p0]     = (uint8)p1;
    m_pCurrentBuffer[p0+1]   = (uint8)(p1>>8);
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteInt32(Int32 p0, Int32 p1)
{
    // implement your code here !
    m_pCurrentBuffer[p0]     = (uint8)p1;
    m_pCurrentBuffer[p0+1]   = (uint8)(p1>>8);
    m_pCurrentBuffer[p0+2]   = (uint8)(p1>>16);
    m_pCurrentBuffer[p0+3]   = (uint8)(p1>>24);
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteByte(Int32 p0, Byte p1)
{
    // implement your code here !
    m_pCurrentBuffer[p0] = p1;   
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteUInt16(Int32 p0, UInt16 p1)
{
    // implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteUInt32(Int32 p0, UInt32 p1)
{
    // implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteSingle(Int32 p0, UInt32 p1)
{
    // implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteDouble(Int32 p0, UInt64 p1)
{
    // implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteByteBuffer(Int32 p0, Byte* p1, Int32 p2)
{
    rt_memcpy(&m_pCurrentBuffer[p0], p1, p2);
    // implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::EndWriteGroup()
{
    // implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::EndWriteAll()
{
    uint8_t *p;

    // implement your code here !
    p = m_pOutBufferTable[0];
    
    KW_Output(p);
}

