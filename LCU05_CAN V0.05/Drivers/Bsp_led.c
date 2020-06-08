

#include "Bsp_led.h"


#define LED_TIME_COUNT (40)

const uint8_t LED_ON[LED_TIME_COUNT] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };  //灯模式，常亮（周期）
const uint8_t LED_OFF[LED_TIME_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //灯模式，常灭（周期）
const uint8_t LED_FLASH_SLOW[LED_TIME_COUNT] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
                                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //灯模式，闪烁（周期1Hz）
const uint8_t LED_FLASH_FAST[LED_TIME_COUNT] = { 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
                                                  1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0 };

const uint8_t *pLed;
                
static void Led_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	rt_kprintf("\r\n+ LED GPIO configure......\r\n");

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;    
    
	GPIO_InitStructure.GPIO_Pin   = LED_ACT_PIN;
	GPIO_Init(LED_ACT_PT, &GPIO_InitStructure);
}


static void Set_Led_Act(uint8_t sts)
{
    if(sts == 0)
    {
        GPIO_SetBits(LED_ACT_PT, LED_ACT_PIN);
    }
    else
    {
    	GPIO_ResetBits(LED_ACT_PT, LED_ACT_PIN);    
    }   
}


//板载LED灯初始化
void System_Led_Init(void)
{
	Led_gpio_init();
	
    System_Led_SetMode(MODE_FLASH_FAST);
}


void System_Led_SetMode(Led_Mode mode)
{
    switch(mode)
    {
	    case MODE_ON:
	        pLed = LED_ON;
	        break;
	    case MODE_OFF:
	        pLed = LED_OFF;
	        break;
	    case MODE_FLASH_SLOW:
	        pLed = LED_FLASH_SLOW;
	        break;
	    case MODE_FLASH_FAST:
	        pLed = LED_FLASH_FAST;
	        break;
	    default:
	        pLed = LED_OFF;
	        break;
    }
}


void System_Led_Logic(void)
{
    static uint32_t cnt = 0;
	static uint32_t cnt_10ms = 0;
    
    cnt_10ms++;

    if(cnt_10ms % 4 == 0)
    {
        Set_Led_Act(pLed[cnt]);

        cnt++;    
        cnt = cnt % LED_TIME_COUNT;
    }
}

