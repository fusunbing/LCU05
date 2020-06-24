
#include "LedTask.h"
#include "rthw.h"
#include "HwWDog_Max706.h"

#define POWER_OFF       0x00
#define POWER_ON        0x01

#define LED_TIME_COUNT (40)

static rt_thread_t bsp_ledService_thread;
const uint8_t _LED_ON[LED_TIME_COUNT] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };  //灯模式，常亮（周期）
const uint8_t _LED_OFF[LED_TIME_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //灯模式，常灭（周期）
const uint8_t _LED_FLASH_SLOW[LED_TIME_COUNT] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
                                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //灯模式，闪烁（周期1Hz）
const uint8_t _LED_FLASH_FAST[LED_TIME_COUNT] = { 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
                                                  1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0 };


const uint8_t* p_led_act;
const uint8_t* p_led_flt;   
uint8_t led_flt_flag = 0;																					 
																					
static void LedTask_Process(void* parameter);																								 

                                                 
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

    GPIO_InitStructure.GPIO_Pin   = LED_FLT_PIN;
    GPIO_Init(LED_FLT_PT, &GPIO_InitStructure);	
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


void Set_Led_Flt(uint8_t sts)
{
    if(sts == 0)
    {
        GPIO_SetBits(LED_FLT_PT, LED_FLT_PIN);
    }
    else
    {
    	GPIO_ResetBits(LED_FLT_PT, LED_FLT_PIN);    
    }   
}


//板载LED灯初始化
void System_Led_Init(void)
{
	  Led_gpio_init();
	
    Set_Led_Act(POWER_ON);
    Set_Led_Flt(POWER_ON);
    
    System_Led_SetMode(LED_ACT, MODE_FLASH_FAST);    
    System_Led_SetMode(LED_FLT, POWER_OFF);
}


void System_Led_SetMode(Led_Type lt, Led_Mode lm)
{
    const uint8_t *p;

    switch(lm)
    {
	    case MODE_ON:
	        p = _LED_ON;
	        break;
	    case MODE_OFF:
	        p = _LED_OFF;
	        break;
	    case MODE_FLASH_SLOW:
	        p = _LED_FLASH_SLOW;
	        break;
	    case MODE_FLASH_FAST:
	        p = _LED_FLASH_FAST;
	        break;
	    default:
	        p = _LED_OFF;
	        break;
    }

	switch(lt)
	{
		case LED_ACT:
			p_led_act = p;
			break;
		case LED_FLT:
			p_led_flt = p;
			break;
		default:
			break;
	}
}


//上电显示一次
static void Led_Logic_First(void)
{
    static uint32_t cnt = 0;

    if (cnt < 16)
    {
        switch(cnt)
        {
            case 3:
                Set_Led_Act(POWER_ON);
                break;
            case 4:           
                Set_Led_Act(POWER_OFF);
                Set_Led_Flt(POWER_ON);
                break;
            case 5:
                Set_Led_Flt(POWER_OFF);
                break;
            default:                
                break;
        }        
        cnt++;
    }
    else
    {
        cnt = 17;
    }
}


void System_Led_Logic(void)
{
    static uint32_t cnt = 0;
    
    Set_Led_Act(p_led_act[cnt]);
    Set_Led_Flt(p_led_flt[cnt]);

    cnt++;
    
    cnt = cnt % LED_TIME_COUNT;

	  if(1 == led_flt_flag)
		{
		    Set_Led_Flt(POWER_ON);
		}
		else
    {
		    Set_Led_Flt(POWER_OFF);
		}
	  
//    Led_Logic_First();
}



/*******************************************************************************
* Function Name  : 
* Description    : 
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  LedTask_CreateApp(void)
{
//	System_Led_Init();
    //-------------------------------------------------------------------------
    // led service task
    //-------------------------------------------------------------------------
    bsp_ledService_thread = rt_thread_create(
        "BspLedSv",
		LedTask_Process,
        RT_NULL,
		512,
		BSP_LEDSERVICE_PRIORITY,
        4);
        
	rt_thread_startup(bsp_ledService_thread);
}

/*************************************************************************
* Function Name  :  
* Description    :
*                 led service task [11/2/2014 lily] 
* Input          : None
* Output         : None
* Return         : None
**************************************************************************/
static void LedTask_Process(void* parameter)
{
    /************************************************************************/
    // 初始化任务                                                        
    /************************************************************************/
	static uint32_t ledSt = 0;

    for (;;)
    {
        IWDG_ReloadCounter(); /* 恢复内部独立看门狗 2019-4-17*/
        HwWDog_Feed();

        System_Led_Logic();
        rt_thread_delay(40);
    }
}


