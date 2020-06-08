#ifndef __RT_TICK_H__
#define __RT_TICK_H__
// KW-Software eCLR Native Library Builder version 2.1.2.0
// RT_TICK.h native assembly header template.
// use this file to implement and/or extend your native c++ classes.
// insert additional private methods and member only outside of the @Begin/@End comment pair

#include "mscorlib.h"

// public scope class for library implementation
// use RT_TICK::init() and RT_TICK::loadLibrary() during startup 
// to make the library accessible by the managed application.
class RT_TICK
{
// @Begin automatically generated code, do not modify !
public:
enum LibVersion { Major=1, Minor=0, Build=0, Revision=0 };
static bool init();
static bool loadLibrary();
static bool isCompatible(const EclrVersion& requestedVersion);
static bool load(ClrAppDomain* pAppDomain);
static bool unload(ClrAppDomain* pAppDomain);
static unsigned s_modulHndl;
// @End automatically generated code

class SharedMemory1 : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    static Void* GetMemPtr();
    SharedMemory1();
    void ctor();
    // @End automatically generated code
    // insert additional private methods and member here ! 
};


}; // class RT_TICK

#endif //#ifndef __RT_TICK_H__
