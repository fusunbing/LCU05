

#include "userApp.h"
#include "Bsp_gpio.h"


DS_STU ds = {0};


void userApp_init(void)
{
    BYTE_TO_BIT id = {0};
    
//    do
//    {
//        id.Bits.bit0 = GetPin(BOARD_TYPE_ID1);
//        id.Bits.bit1 = GetPin(BOARD_TYPE_ID2);
//        id.Bits.bit2 = GetPin(BOARD_TYPE_ID3);
//        id.Bits.bit3 = GetPin(BOARD_TYPE_ID4);
//        ds.boardTypeID = id.value;
//    }while(ds.boardTypeID != BOARD_TYPE_ID_CAN);
    
    do
    {
        id.Bits.bit0 = GetPin(SLOT_ID1);
        id.Bits.bit1 = GetPin(SLOT_ID2);
        id.Bits.bit2 = GetPin(SLOT_ID3);
        id.Bits.bit3 = GetPin(SLOT_ID4);
        ds.slotID = id.value;
    }while(ds.slotID != SLOT_ID_CAN);
    
    do
    {
        id.Bits.bit0 = GetPin(BOX_ID1);
        id.Bits.bit1 = GetPin(BOX_ID2);
        id.Bits.bit2 = GetPin(BOX_ID3);
        id.Bits.bit3 = GetPin(BOX_ID4);
        
        switch(id.value)
        {
            case 3:
                ds.carID = 0; 
                break;
            case 5:
                ds.carID = 1; 
                break;
            case 6:
                ds.carID = 2; 
                break;
            case 9:
                ds.carID = 3; 
                break;
            default:
                ds.carID = 0xFF; 
                break;
        }
    }while(ds.carID > 3);
    
    ds.boardTypeID = BOARD_TYPE_ID_CAN;
    ds.version = CAN_VERSION;
}


//通过位带区地址和位带区的目标位，找到位带别名区的地址
uint32_t* bitBand(uint32_t* addr)
{
    return (uint32_t*)(((uint32_t)addr & 0xF0000000) + 0x02000000 + (((uint32_t)addr & 0x00ffffff) << 5));
}

