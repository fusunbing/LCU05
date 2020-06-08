#include "eclr.h"
#include "SHM_LIB.h"
#include "IO_DRIVER_LIB.h"
#include "RT_TICK.h"

void LibLoad(void)
{
    if (IO_DRIVER_LIB::init() == false)
    {
        printf("IO_DRV_LIB Setup failed\n");
    }
    IO_DRIVER_LIB::loadLibrary();

    if (SHM_LIB::init() == false)
    {
    }
    SHM_LIB::loadLibrary();

    if (RT_TICK::init() == false)
    {
    }
    RT_TICK::loadLibrary();
    
}
