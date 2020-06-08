

#include "Bsp_gpio.h"


#define PIN_LOW         0
#define PIN_HIGH        1

typedef struct
{
    uint32_t mode;
    uint32_t rcc;
    GPIO_TypeDef *gpio;
    uint32_t pin;
}PIN_INDEX, *pPIN_INDEX;


enum PIN_MODE
{ 
    PIN_MODE_OUTPUT = 1,
    PIN_MODE_INPUT,
    PIN_MODE_INPUT_PULLUP,
};


static const PIN_INDEX pins[] =
{
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_0},      //SLOT_ID1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_1},      //SLOT_ID2
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_2},      //SLOT_ID3
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_3},      //SLOT_ID4
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_4},      //SLOT_ID5
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_5},      //SLOT_ID6
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_6},      //BOARD_TYPE_ID1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_7},      //BOARD_TYPE_ID2
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_8},      //BOARD_TYPE_ID3
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_9},      //BOARD_TYPE_ID4
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_11},     //IN1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_10},     //IN2
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_9},      //IN3
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_8},      //IN4
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_7},      //IN5
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_6},      //IN6
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_5},      //IN7
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_4},      //IN8
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_3},      //IN9
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_2},      //IN10
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_1},      //IN11
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOF, GPIOF, GPIO_Pin_0},      //IN12
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_3},      //OU1_FB1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_4},      //OU1_FB2
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_1},      //OU2_FB1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_2},      //OU2_FB2
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_14},     //OU3_FB1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_0},      //OU3_FB2
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_12},     //OU4_FB1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_13},     //OU4_FB2
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_10},     //OU5_FB1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_11},     //OU5_FB2
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_8},      //OU6_FB1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_9},      //OU6_FB2
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_7},      //SW1_FB1
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_5},      //SW1_FB2
    {PIN_MODE_INPUT, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_6},      //SW1_FB3
    {PIN_MODE_OUTPUT, RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_0},     //OU1
    {PIN_MODE_OUTPUT, RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_1},     //OU2
    {PIN_MODE_OUTPUT, RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_2},     //OU3
    {PIN_MODE_OUTPUT, RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_3},     //OU4
    {PIN_MODE_OUTPUT, RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_4},     //OU5
    {PIN_MODE_OUTPUT, RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_5},     //OU6
    {PIN_MODE_OUTPUT, RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_6},     //SW1
};


void SetPin(uint32_t index, uint32_t val)
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
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
        
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

