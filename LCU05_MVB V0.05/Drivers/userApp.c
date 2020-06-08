

#include "userApp.h"
#include "Bsp_gpio.h"
#include "Bsp_canApp.h"
#include "version.h"

BOARD_MVB mvb_board;

void userApp_init(void)
{
	uint32_t i;	
	mvb_board.boardType = 0;
	mvb_board.boxID = 0;
	mvb_board.slotID = 0;
	mvb_board.version = 0;
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
        
        mvb_board.slotID = (uint8_t)id.value;
    }while(mvb_board.slotID != SLOT_ID_MVB);

    do
    {
        for(i = 0; i<10000; i++)
        {
            id.Bits.bit0 = GetPin(BOX_ID1);
            id.Bits.bit1 = GetPin(BOX_ID2);
            id.Bits.bit2 = GetPin(BOX_ID3);
            id.Bits.bit3 = GetPin(BOX_ID4);
        }
        
        mvb_board.boxID = (uint8_t)id.value; 
    }while(0);//(ds.boxID > 5);
	
    do
    {
        id.Bits.bit0 = GetPin(BOARD_TYPE_ID1);
        id.Bits.bit1 = GetPin(BOARD_TYPE_ID2);
        id.Bits.bit2 = GetPin(BOARD_TYPE_ID3);
        id.Bits.bit3 = GetPin(BOARD_TYPE_ID4);

        mvb_board.boardType = (uint8_t)id.value; 
    }while(mvb_board.boardType != BOARD_TYPE_ID_MVB);
    mvb_board.version = (MAJOR_VERSION << 5) | (MINOR_VERSION);
}



//通过位带区地址和位带区的目标位，找到位带别名区的地址
uint32_t* bitBand(uint32_t* addr, uint8_t bit_num)
{
    return (uint32_t*)(((uint32_t)addr & 0xF0000000) + 0x02000000 + (((uint32_t)addr & 0x00ffffff) << 5) + (bit_num << 2));
}

