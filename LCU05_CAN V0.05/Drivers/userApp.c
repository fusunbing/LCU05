

#include "userApp.h"


DS_STU ds = {0};


void userApp_init(void)
{


}


//ͨ��λ������ַ��λ������Ŀ��λ���ҵ�λ���������ĵ�ַ
uint32_t* bitBand(uint32_t* addr)
{
    return (uint32_t*)(((uint32_t)addr & 0xF0000000) + 0x02000000 + (((uint32_t)addr & 0x00ffffff) << 5));
}

