
#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__


#include "stm32f4xx.h"
#include <rtthread.h>


enum PIN_MODE
{ 
    PIN_MODE_OUTPUT = 1,
    PIN_MODE_INPUT,
    PIN_MODE_INPUT_PULLUP,
};


enum PIN_NAME
{ 
    IOSLOT_ID1 = 0,
    IOSLOT_ID2,
    IOSLOT_ID3,
    IOSLOT_ID4,
    IOSLOT_ID5,
    IOSLOT_ID6,
    IDD0,
    IDD1,
    IDD2,
	  IDD3,
	  BOX_ID0,
	  BOX_ID1,
	  BOX_ID2,
	  BOX_ID3,
};

        
#define DC110_HCHK_PT   (GPIOF)
#define DC110_HCHK_PIN  (GPIO_Pin_9)

#define DC110_LCHK_PT   (GPIOF)
#define DC110_LCHK_PIN  (GPIO_Pin_8)

#define DC5_CHK_PT      (GPIOC)
#define DC5_CHK_PIN     (GPIO_Pin_2)

#define MODE_PT         (GPIOC)
#define MODE_PIN        (GPIO_Pin_3)

#define SLOT_ID1_PT     (GPIOB)
#define SLOT_ID1_PIN    (GPIO_Pin_15)

#define SLOT_ID2_PT     (GPIOB)
#define SLOT_ID2_PIN    (GPIO_Pin_14)

#define SLOT_ID3_PT     (GPIOG)
#define SLOT_ID3_PIN    (GPIO_Pin_8)

#define SLOT_ID4_PT     (GPIOG)
#define SLOT_ID4_PIN    (GPIO_Pin_7)

#define SLOT_ID5_PT     (GPIOG)
#define SLOT_ID5_PIN    (GPIO_Pin_6)        
        
        
void SetPin(uint32_t index, uint8_t val);
uint8_t GetPin(uint32_t index);
void stmPinsInit(void);

#endif


