#include "eclr.h"
#include "KWEventTask.h"
#include "pcosdomain.h"
#include "eclrImageFile.h"

static unsigned int kwEventId = 0;
uint32 kweventcnt = 0;

void TriggerEventTask(void)
{
    // create the ProConOS domain
    CPcosDomain* ProConOSDomain;

    kweventcnt++;
	
	ProConOSDomain= pcoslib::getCurrentDomain();
    ProConOSDomain->RiseTaskEvent(kwEventId);
}

void KW_SetEventId(unsigned int id)
{
    kwEventId = id;
}





