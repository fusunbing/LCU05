

#include "Bsp_ITempMonit.h"


#define ADC_DR_OFFSET           ((uint32_t)0x4C)
#define ADC1_DR_ADRESS          ((uint32_t)(ADC1_BASE + ADC_DR_OFFSET))

#define ADC_TIM                 (TIM8)
#define ADC_TIM_RCC             (RCC_APB2Periph_TIM8)

#define ADC1_CHANNELS_CNT       (1)
#define ADC1_BUF_LENGTH         ((uint32_t)(ADC1_CHANNELS_CNT))


static uint16_t ADC1_ValTemp[ADC1_BUF_LENGTH] = {0};
static uint16_t iTemp = 0;


void Bsp_ITemp_Config(void)
{
    ADC_InitTypeDef           ADC_InitStructure       = { 0 };
    DMA_InitTypeDef           DMA_InitStructure       = { 0 };
    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure   = { 0 };
    TIM_OCInitTypeDef         TIM_OCInitStructure     = { 0 };
    ADC_CommonInitTypeDef     ADC_CommonInitStructure = { 0 };

    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

    // 停止ADC工作
    ADC_Cmd(ADC1, DISABLE);


    //--------------------------------------------------------------
    // TIM1 configuration 用于触发ADC1的规则转换组
    // 【注】APB2主频84MHz, TIM8 Clk = 84*2 = 168MHz
    //       TIM8的计数频率10kHz 
    //		 TIM_Period = 200;  触发周期为：200 * 100us = 20ms
    //--------------------------------------------------------------
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period        = 2000;
    TIM_TimeBaseStructure.TIM_Prescaler     = 0x0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_CounterMode  = TIM_CounterMode_Up;
    TIM_TimeBaseInit(ADC_TIM, &TIM_TimeBaseStructure);

    TIM_PrescalerConfig(ADC_TIM, (16800 - 1), TIM_PSCReloadMode_Immediate);

    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = 100;  
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;
    TIM_OC1Init(ADC_TIM, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(ADC_TIM, TIM_OCPreload_Enable);

    DMA_DeInit(DMA2_Stream0);
    DMA_InitStructure.DMA_Channel            = DMA_Channel_0;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (rt_uint32_t)ADC1_DR_ADRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (rt_uint32_t)ADC1_ValTemp;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize         = ADC1_BUF_LENGTH;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);


    /* ADC Common Init **********************************************************/
    ADC_CommonInitStructure.ADC_Mode             = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler        = ADC_Prescaler_Div2;
    ADC_CommonInitStructure.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);


    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Resolution           = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode         = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode   = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Falling;
    ADC_InitStructure.ADC_ExternalTrigConv     = ADC_ExternalTrigConv_T8_CC1;
    ADC_InitStructure.ADC_DataAlign            = ADC_DataAlign_Right;   //采样的数据以高位还是低位开始存放
    ADC_InitStructure.ADC_NbrOfConversion      = ADC1_CHANNELS_CNT;     // 转换通道的个数
    ADC_Init(ADC1, &ADC_InitStructure);
    
    
    //.......................................................................................
    /* ADC1 regular channel configuration                                                  */
    /* ADC1 采样时间：SampleTime+12  个采样周期，ADC的时钟60/2=30MHz                  */
    /* 例如         ：480 + 12 = 492个采样周期                  */
    /*  转换一个通道的时间 = 492 * 1 / 30 = 16.4 us                                          */
    //.......................................................................................
    ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_480Cycles);
	ADC_TempSensorVrefintCmd(ENABLE);
    
    /* Enable DMA request after last transfer (Single-ADC mode) */
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

    /* Enable DMA Stream0 */
    DMA_Cmd(DMA2_Stream0, ENABLE);
    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);
    /* Enable VBAT channel */
    ADC_VBATCmd(ENABLE);
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);


    //启动TIM2，触发ADC1\ADC3的规则组转换
    TIM_ARRPreloadConfig(ADC_TIM, ENABLE);
    /* TIM8 counter enable */
    TIM_Cmd(ADC_TIM, ENABLE);
    /* TIM8 main Output Enable */
    TIM_CtrlPWMOutputs(ADC_TIM, ENABLE);
}


//中位值平均滤波法，缓存采集length组，去掉最大最小值，剩下length - 2组取平均.
static void FilterAnalog(uint16_t srcdata, uint16_t *pdstdata)
{
    static uint16_t startTimer = 1;
    uint16_t index;
    uint16_t lastData = 0;

    if (startTimer != 0)
    {
        startTimer  = 0;
        lastData    = srcdata;
        *pdstdata   = lastData;
    }
    else
    {
        lastData = pdstdata[0];
        *pdstdata = (lastData * 9 + srcdata) / 10;
    }
}


//Temperature sensor range: -40 to 125C. +/-1.5C
void Bsp_ITemp_Process(void)
{

    iTemp = (ADC1_ValTemp[0] * 1200)/4096 - 304 + 25;//((Vsensor * 3.3)/4096 - 0.76) *1000/2.5 + 25

    //FilterAnalog(AD_Origine[0], &AD_FilterVal[0]);
    
    rt_kprintf("Temp = %d\r\n", iTemp);
}

