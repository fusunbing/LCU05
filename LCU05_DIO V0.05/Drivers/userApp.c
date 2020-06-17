

#include "userApp.h"
#include "Bsp_gpio.h"


DS_STU ds = {0};


//通过位带区地址和位带区的目标位，找到位带别名区的地址
static uint32_t* bitBand(void* addr)
{
    return (uint32_t*)(((uint32_t)addr & 0xF0000000) + 0x02000000 + (((uint32_t)addr & 0x00ffffff) << 5));
}


static void DS_BitBand_Init(void)
{
    uint32_t i = 0;

    for(i = 0; i < 3; i++)
    {
        ds.MCU[i].Bits_ou = bitBand(ds.MCU[i].ou);
        
        ds.DIO[i].Bits_in = bitBand(ds.DIO[i].in);
        ds.DIO[i].Bits_ou = bitBand(&ds.DIO[i].ou);
        ds.DIO[i].Bits_fb = bitBand(ds.DIO[i].fb);
        ds.DIO[i].Bits_inFlt = bitBand(ds.DIO[i].inFlt);
        ds.DIO[i].Bits_ouFlt = bitBand(&ds.DIO[i].ouFlt);
    }
}


void GetSlotID(void)
{
    uint32_t i = 0;
    uint8_t index = 0;
    uint8_t group = 0;
    BYTE_TO_BIT id = { 0 };

    do
    {
        for(i = 0; i < 1000; i++)
        {
            id.Bits.bit0 = GetPin(SLOT_ID1);
            id.Bits.bit1 = GetPin(SLOT_ID2);
            id.Bits.bit2 = GetPin(SLOT_ID3);
            id.Bits.bit3 = GetPin(SLOT_ID4);
            id.Bits.bit4 = GetPin(SLOT_ID5);
            id.Bits.bit5 = GetPin(SLOT_ID6);
        }        
    }while(id.value < SLOT_ID_IO_MIN || id.value >SLOT_ID_IO_MAX);

    ds.slotID = id.value;
    
    ds.DIO[0].slotID = id.value;
    
    group = (ds.slotID - SLOT_ID_IO_MIN) / 3;
    index = (ds.slotID - SLOT_ID_IO_MIN) % 3;
    ds.offset = group * OU_CNT;

    switch(index)
    {
        case 0: //A板
            ds.DIO[1].slotID =  ds.slotID + 1;
            ds.DIO[2].slotID =  ds.slotID + 2;
            break;
        case 1: //B板
            ds.DIO[1].slotID =  ds.slotID + 1;
            ds.DIO[2].slotID =  ds.slotID - 1;
            break;
        case 2: //C板
            ds.DIO[1].slotID =  ds.slotID - 2;
            ds.DIO[2].slotID =  ds.slotID - 1;
            break;
        default:
            break;
    }
}


void userApp_init(void)
{
    DS_BitBand_Init();
    
    ds.version = (MAJOR_VERSION <<5 ) + MINOR_VERSION;
    
    GetSlotID();
}


