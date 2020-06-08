// RT_TICK.cpp native assembly code template.
// KW-Software embedded CLR native assembly builder tool generated source file.
// use this file to implement your own native code

#include "eclr.h"
#include "RT_TICK.h"

bool RT_TICK::init()
{
// if necessary, insert some initialization code for the RT_TICK library here !
// call RT_TICK::init() somewhere before RT_TICK::loadLibrary() is called.
// loadLibrary() have to be called after the ClrContoller object is instanciated
// for detailed information see the native library builder documentation
    return true;
}

bool RT_TICK::isCompatible(const EclrVersion& requestedVersion)
{
// this function is called by the runtime if an application will be bound
// to this library with the requested version.
// return true means that this library fits to the requested interface.
// return false forces the runtime to search for other libraries with the same name by calling isCompatible()
// if no libraries could be found, the runtime breaks the loading process with linker-error
// for multiple library version support read the documentation.
    return true;
}

bool RT_TICK::load(ClrAppDomain* pAppDomain)
{
// This function is called by the runtime if an application will be started.
// pAppDomain is the domain object witch hosts the current application process.
// At this point, the firmware library is able to initialize some internal objects.
// If the initialization is done ok, return true
// othewise return false.
    return true;
}

bool RT_TICK::unload(ClrAppDomain* pAppDomain)
{
// This function is called by the runtime if an application will be terminated.
// pAppDomain is the domain object witch hosts the current application process.
// At this point, the firmware library is able to finalize some internal objects.
// If the finalization is done ok, return true
// othewise return false.
    return true;
}
extern uint32 rt_tick;
// class RT_TICK.SharedMemory1 implementation
Void* __PInvoke__ RT_TICK::SharedMemory1::GetMemPtr()
{
// implement your code here !
    return (Void*)0x2000067c;
}

RT_TICK::SharedMemory1::SharedMemory1()
{
// implement your constructor code here !
}

void __PInvoke__ RT_TICK::SharedMemory1::ctor()
{
// automatically generated auxiliary contructor : calls the contructor by displacement new operator !
// @Begin automatically generated code, do not modify !
    new (this) RT_TICK::SharedMemory1();
// @End automatically generated code
}

