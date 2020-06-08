#include "stm32f4xx.h"
#include "rtthread.h"
#include "eclr.h"
#include "pcosdomain.h"
#include "KWAppInfo.h"
#include "eclrImageFile.h"


uint8 GetKWAppState(void)
{
    uint8 isKWAppValid;
    PcosImageFile AppImageFile;
    AppImageFile = PcosImageFile();
    isKWAppValid = AppImageFile.IsValid();
    return isKWAppValid;
}


uint64 GetPLCUnixTimestamp(void)
{
	uint64 timestamp = 0;
	CPcosDomain* pPcosDomain = pcoslib::getCurrentDomain();
		
	if(NULL == pPcosDomain)
		return timestamp;  

	ClrAppDomain* pAppDomain = pPcosDomain->getAppDomain();

	if(NULL == pAppDomain)
		 return timestamp;
	 
	CAppImageHdr* pHdr = pAppDomain->getImageHdr();
	 
	if(NULL == pHdr)
		return timestamp;

	//calculated by ctime64()    
    timestamp = pHdr->ctimeHigh;
    timestamp = (timestamp << 32) +  (uint64)(pHdr->ctimeLow);

	return timestamp;
}



