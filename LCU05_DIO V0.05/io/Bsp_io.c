

#include "Bsp_io.h"
#include "userApp.h"
#include "Bsp_gpio.h"
#include "Bsp_canTask.h"


#define FILTER_COLLECT_LENTH       (8)
#define FILTER_VALID_LENTH         (6)


typedef struct
{
    uint8_t val;            //current input level
    uint8_t filterVal;
    uint8_t index;
    uint8_t res;
    uint8_t buf[FILTER_COLLECT_LENTH];
}INPUT_FILTER_UNIT, *PINPUT_FILTER_UNIT;


static INPUT_FILTER_UNIT filter[IN_CNT + OU_FB_CNT] = {0};


static void inputFilter(PINPUT_FILTER_UNIT pFilter)
{
    uint32_t i = 0;
    uint32_t high_Level_cnt = 0;
    uint32_t low_Level_cnt = 0;
    
    pFilter->index = (pFilter->index + 1) % FILTER_COLLECT_LENTH;
    pFilter->buf[pFilter->index] = pFilter->val;
    
    for(i = 0; i < FILTER_COLLECT_LENTH; i++)
    {
        if(pFilter->buf[i])
        {
            high_Level_cnt++;
        }
        else
        {
            low_Level_cnt++;
        }
    }
    
    if(high_Level_cnt >= FILTER_VALID_LENTH)
    {
        pFilter->filterVal = 1;
    }
    else if(low_Level_cnt >= FILTER_VALID_LENTH)
    {
        pFilter->filterVal = 0;
    }
}


void inputProcess(PBOARD_DIO_STU pDio)
{
    uint8_t i;    
    uint8_t tempIn[2];
    static rt_tick_t tick = 0;

    for(i = 0; i < (IN_CNT + OU_FB_CNT); i++)
    {
        filter[i].val = GetPin(PIN_IN1 + i);
        inputFilter(&filter[i]);
    }
    
    tempIn[0] = pDio->in[0];
    tempIn[1] = pDio->in[1];
    
    for(i = 0; i < IN_CNT; i++)
    {
        //输入通道故障，则清除此通道，导向安全
        if(pDio->Bits_inFlt[i] == RT_EOK)
        {
            pDio->Bits_in[i] = (uint32_t)filter[i].filterVal;
        }
        else
        {
            pDio->Bits_in[i] = 0;
        }
    }
    
    for(i = 0; i < OU_FB_CNT; i++)
    {
        pDio->Bits_fb[i] = (uint32_t)filter[IN_CNT + i].filterVal;
    }
    
    if(tempIn[0] != pDio->in[0] || tempIn[1] != pDio->in[1])
    {
        if((rt_tick_get() - tick) > 100)
        {
            Can_Send_Event();
        }
        
        tick = rt_tick_get();
    }
}


static void HW_output(uint32_t* ouBuf)
{
    uint32_t i;

    for(i = 0; i < OU_CNT; i++)
    {
        SetPin((PIN_OU1 + i), ouBuf[i]);
    }
}


void outputProcess(PBOARD_DIO_STU pDio, PBOARD_MCU_STU pMcu_A, PBOARD_MCU_STU pMcu_B, PBOARD_MCU_STU pMcu_C)
{
    uint32_t i;
    
    /* 输出指令三取二 */
    for(i = 0; i < OU_CNT; i++)
    {
        pDio->Bits_ou[i] = ((pMcu_A->Bits_ou[ds.offset + i] + pMcu_B->Bits_ou[ds.offset + i] + pMcu_C->Bits_ou[ds.offset + i]) >= 2) ? 1 : 0;
    }
    
    HW_output(pDio->Bits_ou);
}

