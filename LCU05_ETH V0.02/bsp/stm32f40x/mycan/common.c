
#include "common.h"
#include "Bsp_gpio.h"

DS_STU ds = {0};


void Board_Information_Init(void)
{
    BYTE_TO_BIT id = {0};
    
    stmPinsInit();
    
//    do
//    {
//        id.Bits.bit0 = GetPin(BOARD_TYPE_ID1);
//        id.Bits.bit1 = GetPin(BOARD_TYPE_ID2);
//        id.Bits.bit2 = GetPin(BOARD_TYPE_ID3);
//        id.Bits.bit3 = GetPin(BOARD_TYPE_ID4);
//        ds.boardType = id.value;
//    }while(ds.boardType != BOARD_TYPE_ID_ETU);
    
    
    do
    {
        id.Bits.bit0 = GetPin(SLOT_ID1);
        id.Bits.bit1 = GetPin(SLOT_ID2);
        id.Bits.bit2 = GetPin(SLOT_ID3);
        id.Bits.bit3 = GetPin(SLOT_ID4);
        ds.slotID = id.value;
    }while(ds.slotID != SLOT_ID_ETU);
    
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
    
    ds.version = 2;
}

