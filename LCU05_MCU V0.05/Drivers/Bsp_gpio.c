

#include "Bsp_gpio.h"


static const PIN_INDEX pins[] =
{
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_0},      //SLOT ID 1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_1},      //SLOT ID 2
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_2},      //SLOT ID 3
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_3},      //SLOT ID 4 
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_6},      //BOX ID 1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_7},      //BOX ID 2
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_8},      //BOX ID 3
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_9},      //BOX ID 4
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_4},      //BOARD TYPE ID 1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_5},      //BOARD TYPE ID 2
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_13},     //BOARD TYPE ID 3
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_14},     //BOARD TYPE ID 4
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_10},     //DC5V DETECT
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_8},      //DC110V DETECT
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

