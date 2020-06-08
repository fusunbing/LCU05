

#include "userApp.h"
#include "Bsp_gpio.h"
#include "Bsp_canApp.h"


DS_STU ds = {0};


//KW_SharedMemory ��eCLRģ����
extern volatile uint8_t shared_memory[4096];
PKW_SHM_STU pKW_SHM = RT_NULL;


//ͨ��λ������ַ��λ������Ŀ��λ���ҵ�λ���������ĵ�ַ
static uint32_t* bitBand(void* addr)
{
    return (uint32_t*)(((uint32_t)addr & 0xF0000000) + 0x02000000 + (((uint32_t)addr & 0x00ffffff) << 5));
}


static void DS_BitBand_Init(void)
{
    uint32_t i = 0;

    //�������λ�ε�ַ��ʼ��
    ds.Bits_inBuf = bitBand(ds.inBuf);
    ds.Bits_ouBuf = bitBand(ds.ouBuf);
    
    for(i = 0; i < IO_BOARD_MAX; i++)
    {
        ds.DIO[i].Bits_in = bitBand(ds.DIO[i].in);
        ds.DIO[i].Bits_ou = bitBand(&ds.DIO[i].ou);
        ds.DIO[i].Bits_fb = bitBand(ds.DIO[i].fb);
        ds.DIO[i].Bits_inFlt = bitBand(ds.DIO[i].inFlt);
        ds.DIO[i].Bits_ouFlt = bitBand(&ds.DIO[i].ouFlt);
    }
}


void userApp_init(void)
{
	uint32_t i;	
    BYTE_TO_BIT id = {0};
    
    pKW_SHM = (PKW_SHM_STU)shared_memory;
    
    DS_BitBand_Init();

    do
    {
        id.Bits.bit0 = GetPin(BOARD_TYPE_ID1);
        id.Bits.bit1 = GetPin(BOARD_TYPE_ID2);
        id.Bits.bit2 = GetPin(BOARD_TYPE_ID3);
        id.Bits.bit3 = GetPin(BOARD_TYPE_ID4);
        ds.boardTypeID = id.value;
    }while(ds.boardTypeID != BOARD_TYPE_ID_MCU);
    
    
    do
    {
        for(i = 0; i < 10000; i++)
        {
            id.Bits.bit0 = GetPin(SLOT_ID1);
            id.Bits.bit1 = GetPin(SLOT_ID2);
            id.Bits.bit2 = GetPin(SLOT_ID3);
            id.Bits.bit3 = GetPin(SLOT_ID4);
        }
        
        ds.slotID = (uint8_t)id.value;
    }while(ds.slotID < SLOT_ID_MCU_A && ds.slotID > SLOT_ID_MCU_C);
    
    
    ds.MCU[ds.slotID - 4].armVersion = MINOR_VERSION;
    ds.MCU[ds.slotID - 4].kwVersion = 3;

    
    do
    {
        for(i = 0; i < 10000; i++)
        {
            id.Bits.bit0 = GetPin(BOX_ID1);
            id.Bits.bit1 = GetPin(BOX_ID2);
            id.Bits.bit2 = GetPin(BOX_ID3);
            id.Bits.bit3 = GetPin(BOX_ID4);
        }
        
        ds.carID = (uint8_t)id.value; 
    }while(ds.carID > 3);
    
    
    pKW_SHM->info[0] = ds.carID;
}


//����ͨ����ȡ���㷨
static void two_out_of_three(void)
{
    uint8_t i;
    uint8_t group;
    uint8_t offset;
    uint8_t index;

    for(group = 0; group < IO_GROUP_MAX; group++)
    {
        offset = group * BOARD_IN_COUNT; //����ͨ��ƫ��
        index = group * 3; //�忨
        
        for(i = 0; i < BOARD_IN_COUNT; i++)
        {
            ds.Bits_inBuf[offset + i] = ((ds.DIO[index].Bits_in[i] + ds.DIO[index + 1].Bits_in[i] + ds.DIO[index + 2].Bits_in[i]) >= 2) ? 1 : 0;
        }
    }
}

void Input_KW(uint8_t *buf)
{
    two_out_of_three();
    rt_memcpy(buf, ds.inBuf, 128);
}


void KW_Output(uint8_t *buf)
{
    if(rt_memcmp(ds.ouBuf, buf, 8) != 0)
    {
        rt_memcpy(ds.ouBuf, buf, 8);
        can_send_output();
    }
}
