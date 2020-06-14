

#include "userApp.h"
#include "Bsp_gpio.h"
#include "Bsp_canApp.h"


DS_STU ds = { 0 };

void userApp_init(void)
{
	uint32_t i;	
    BYTE_TO_BIT id = {0};

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
        
    }while(ds.slotID != SLOT_ID_MVB);

    do
    {
        for(i = 0; i<10000; i++)
        {
            id.Bits.bit0 = GetPin(BOX_ID1);
            id.Bits.bit1 = GetPin(BOX_ID2);
            id.Bits.bit2 = GetPin(BOX_ID3);
            id.Bits.bit3 = GetPin(BOX_ID4);
        }
        
        ds.carID = (uint8_t)id.value; 
        
    }while(0);//(ds.boxID > 5);
	
    do
    {
        id.Bits.bit0 = GetPin(BOARD_TYPE_ID1);
        id.Bits.bit1 = GetPin(BOARD_TYPE_ID2);
        id.Bits.bit2 = GetPin(BOARD_TYPE_ID3);
        id.Bits.bit3 = GetPin(BOARD_TYPE_ID4);

        ds.boardType = (uint8_t)id.value; 
    }while(ds.boardType != BOARD_TYPE_ID_MVB);
    
    //Èí¼þ°æ±¾ºÅ
    ds.version = 5;
}

