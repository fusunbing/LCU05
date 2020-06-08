

#include "userApp.h"


DS_STU ds = {0};


void userApp_init(void)
{


}


//通过位带区地址和位带区的目标位，找到位带别名区的地址
uint32_t* bitBand(uint32_t* addr)
{
    return (uint32_t*)(((uint32_t)addr & 0xF0000000) + 0x02000000 + (((uint32_t)addr & 0x00ffffff) << 5));
}

