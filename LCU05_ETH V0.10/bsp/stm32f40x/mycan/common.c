
#include "common.h"
#include "Bsp_gpio.h"

DS_STU ds = {0};


void Board_Information_Init(void);
uint8_t GetSlotID(void);
uint8_t GetBordTypeID(void);
uint8_t GetBoxID(void);
static void Delay_tick(uint8_t cnt);


void Board_Information_Init(void)
{
    //系统slot ID、110V检测、5V检测gpio初始化
    stmPinsInit();
	
	  //降低热插拔读错板卡信息的概率
		do
		{
        ds.slotID = GetSlotID();
//			rt_kprintf("%d",ds.slotID);
		}while(ds.slotID != ETH_SLOT_ID);
		
//		ds.boardType = GetBordTypeID();

		ds.boxID = GetBoxID();

//		if(10 != ds.boardType)
//		    while(1);
}


uint8_t GetSlotID(void)
{
	  uint32_t i;
	  uint8_t temp = 0;
	  BYTE_TO_BIT id = {0};
    
	  do
		{
	      for(i=0; i<100; i++)
		    {
            id.Bits.bit0 = GetPin(IOSLOT_ID1);
            id.Bits.bit1 = GetPin(IOSLOT_ID2);
            id.Bits.bit2 = GetPin(IOSLOT_ID3);
            id.Bits.bit3 = GetPin(IOSLOT_ID4);
//            id.Bits.bit4 = GetPin(IOSLOT_ID5);
//	          id.Bits.bit5 = GetPin(IOSLOT_ID6);
			      temp = temp ^ id.value;
            Delay_tick(100);
		    }
	  }while(temp != 0);

    return id.value;  
}

uint8_t GetBordTypeID(void)
{
	  uint32_t i;
    uint8_t id[4];
	  uint8_t bordtypeid = 0;
	  uint8_t temp = 0;
    
	  do
		{
	      for(i=0; i<100; i++)
		    {
            id[0] = GetPin(IDD0);
            id[1] = GetPin(IDD0);
            id[2] = GetPin(IDD0);
            id[3] = GetPin(IDD0);
			      bordtypeid = ((id[3]<<3) + (id[2]<<2) + (id[1]<<1) + id[0]);
			      temp = temp ^  bordtypeid;
					  Delay_tick(100);
		    }
	  }while(temp != 0);
		
    return bordtypeid; 
}


uint8_t GetBoxID(void)
{
	  uint32_t i;
    uint8_t id[4];
	  uint8_t boxid = 0;
	  uint8_t temp = 0;
    
	  do
		{
	      for(i=0; i<100; i++)
		    {
            id[0] = GetPin(BOX_ID0);
            id[1] = GetPin(BOX_ID1);
            id[2] = GetPin(BOX_ID2);
            id[3] = GetPin(BOX_ID3);
			      boxid = ((id[3]<<3) + (id[2]<<2) + (id[1]<<1) + id[0]);
			      temp = temp ^  boxid;
					  Delay_tick(100);
		    }
	  }while(temp != 0);
		
    return boxid; 
}


static void Delay_tick(uint8_t cnt)
{
	  uint8_t i; 
	  
    for(i=0; i<cnt; i++)
		{
		    ;
		}
}


