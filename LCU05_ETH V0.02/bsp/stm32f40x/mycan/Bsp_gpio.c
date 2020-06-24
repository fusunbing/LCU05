

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
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_11},     //IOSLOT_ID1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_10},     //IOSLOT_ID2
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_9},      //IOSLOT_ID3
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_8},      //IOSLOT_ID4
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_7},      //BOARD_TYPE_ID1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_6},      //BOARD_TYPE_ID2
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_5},      //BOARD_TYPE_ID3
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_4},      //BOARD_TYPE_ID4
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_6},      //BOX_IDD0
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_7},      //BOX_IDD1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_8},      //BOX_IDD2
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_9},      //BOX_IDD3
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

