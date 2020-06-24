

#include "Bsp_gpio.h"
#include "common.h"


#define PIN_LOW                 0x00
#define PIN_HIGH                0x01



typedef struct
{
    uint32_t mode;
    uint32_t rcc;
    GPIO_TypeDef *gpio;
    uint32_t pin;
}PIN_INDEX, *pPIN_INDEX;


static const PIN_INDEX pins[] =
{
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_0},      //IOSLOT_ID1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_1},      //IOSLOT_ID2
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_2},      //IOSLOT_ID3
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_3},      //IOSLOT_ID4
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_4},      //IOSLOT_ID5
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_5},      //IOSLOT_ID6
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_6},      //IDD0
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_7},      //IDD1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_8},      //IDD2
		{PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_9},      //IDD3
};


void SetPin(uint32_t index, uint8_t val)
{    
    if(val == PIN_LOW)
    {
        GPIO_ResetBits(pins[index].gpio, pins[index].pin);  
    }
    else
    {
        GPIO_SetBits(pins[index].gpio, pins[index].pin);    
    }
}


uint8_t GetPin(uint32_t index)
{
    return GPIO_ReadInputDataBit(pins[index].gpio, pins[index].pin);
}




void stmPinsInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    uint32_t i = 0;
    
    for(i = 0; i < sizeof(pins)/sizeof(pins[0]); i++)
    {
        /* GPIO Periph clock enable */
        RCC_AHB1PeriphClockCmd(pins[i].rcc, ENABLE);
        
        /* Configure GPIO_InitStructure */
        GPIO_InitStructure.GPIO_Pin     = pins[i].pin;
        GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_2MHz;
        
        switch(pins[i].mode)
        {
            case PIN_MODE_OUTPUT:
                /* output setting */
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
                GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
            break;
            case PIN_MODE_INPUT:
                /* input setting: not pull. */
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
                GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
            break;
            case PIN_MODE_INPUT_PULLUP:
                /* input setting: pull up. */
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
                GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
            break;
            default:
                /* input setting:default. */
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
                GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
            break;        
        }        
        
        GPIO_Init(pins[i].gpio, &GPIO_InitStructure);
    }
}

