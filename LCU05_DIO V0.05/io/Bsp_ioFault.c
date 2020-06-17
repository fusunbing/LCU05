

#include "Bsp_led.h"
#include "Bsp_ioFault.h"
#include "Can_fault_detect.h"


#define IN_FLT_CNT         (50)
#define OU_FLT_CNT         (50)


static uint32_t in_cnt[IN_CNT] = { 0 };
static uint32_t preMos_cnt[OU_CNT] = { 0 };
static uint32_t posMos_cnt[OU_CNT] = { 0 };


//input channels fault detect.
static void input_fault_detect(void)
{
    uint8_t i;

    //所有IO板卡在位
    if(ds.DIO[0].flt.isLost == RT_EOK && ds.DIO[1].flt.isLost == RT_EOK && ds.DIO[2].flt.isLost == RT_EOK)
    {
        for(i = 0; i < IN_CNT; i++)
        {
            if(ds.DIO[0].Bits_inFlt[i] == RT_EOK)
            {
                if(ds.DIO[0].Bits_in[i] != ds.DIO[1].Bits_in[i] && ds.DIO[0].Bits_in[i] != ds.DIO[2].Bits_in[i])
                {
                    in_cnt[i]++;
                }
                else
                {
                    in_cnt[i] = 0;
                }
                
                ds.DIO[0].Bits_inFlt[i] = (in_cnt[i] >= IN_FLT_CNT) ? RT_ERROR : RT_EOK;
            }
        }
    }
}


//output channels fault detect.
static void output_fault_detect(void)
{
    uint32_t i;

    //电源板、主控板、DIO板全部在位，则进行输出通道故障判断
    if(ds.DIO[0].flt.isLost == RT_EOK && ds.DIO[1].flt.isLost == RT_EOK && ds.DIO[1].flt.isLost == RT_EOK
        && ds.MCU[0].flt.isLost == RT_EOK && ds.MCU[1].flt.isLost == RT_EOK && ds.MCU[2].flt.isLost == RT_EOK
        && ds.MCU[0].dc110v == 1 && ds.MCU[1].dc110v == 1 && ds.MCU[2].dc110v == 1)
    {
        for(i = 0; i < OU_CNT; i++)
        {

            if(ds.DIO[0].Bits_ouFlt[i] == RT_EOK)
            {
                if(i < 6) //有源输出通道故障诊断
                {
                    //输出反馈不一致
                    if(ds.DIO[0].Bits_fb[i * 2] != ds.DIO[0].Bits_ou[i])
                    {
                        preMos_cnt[i]++;
                    }
                    else
                    {
                        preMos_cnt[i] = 0;
                    }
                    
                    if((ds.DIO[2].Bits_fb[i * 2] == 1 && ds.DIO[0].Bits_fb[i * 2 + 1] != ds.DIO[0].Bits_ou[i])
                        || (ds.DIO[2].Bits_fb[i * 2] == 0 && ds.DIO[0].Bits_fb[i * 2 + 1] == 1 && ds.DIO[0].Bits_ou[i] == 0))
                    {
                        posMos_cnt[i]++;
                    }
                    else
                    {
                        posMos_cnt[i] = 0;
                    }
                }
                else    //干节点故障诊断
                {

                    if((ds.DIO[0].Bits_ou[i] == 1 && ds.DIO[0].Bits_fb[12] != ds.DIO[0].Bits_fb[13])
                        || (ds.DIO[0].Bits_ou[i] == 0 && ds.DIO[0].Bits_fb[13] == 1))
                    {
                        preMos_cnt[i]++;
                    }
                    else
                    {
                        preMos_cnt[i] = 0;
                    }
                    
                    if((ds.DIO[2].Bits_fb[12] == 1 && ds.DIO[0].Bits_ou[i] == 1 && ds.DIO[2].Bits_fb[13] == 1 && ds.DIO[0].Bits_fb[14] == 0)
                        || (ds.DIO[2].Bits_fb[12] == 1 && ds.DIO[0].Bits_ou[i] == 0 && ds.DIO[0].Bits_fb[14] == 1)
                        || (ds.DIO[2].Bits_fb[12] == 0 && ds.DIO[0].Bits_fb[14] == 1))
                    {
                        posMos_cnt[i]++;
                    }
                    else
                    {
                        posMos_cnt[i] = 0;
                    }
                }

                ds.DIO[0].Bits_ouFlt[i] = (preMos_cnt[i] >= OU_FLT_CNT || posMos_cnt[i] >= OU_FLT_CNT) ? RT_ERROR : RT_EOK;
            }
                   
        }
    }
}


static void canNodes_detect(void)
{
    uint32_t i = 0;
    
    for(i = 0; i < 3; i++)
    {
        ds.MCU[i].flt.can1 = (uint8_t)Get_CanSts(1, (SLOT_ID_MCU_A + i));
        ds.MCU[i].flt.can2 = (uint8_t)Get_CanSts(2, (SLOT_ID_MCU_A + i));
        ds.MCU[i].flt.isLost = (ds.MCU[i].flt.can1 == RT_ERROR && ds.MCU[i].flt.can2 == RT_ERROR) ? RT_ERROR : RT_EOK;
        
        ds.DIO[i].flt.can1 = (uint8_t)Get_CanSts(1, ds.DIO[i].slotID);
        ds.DIO[i].flt.can2 = (uint8_t)Get_CanSts(2, ds.DIO[i].slotID);
        ds.DIO[i].flt.isLost = (ds.DIO[i].flt.can1 == RT_ERROR && ds.DIO[i].flt.can2 == RT_ERROR) ? RT_ERROR : RT_EOK;
        
        if(ds.MCU[i].flt.isLost == RT_ERROR)
        {
            rt_memset(ds.MCU[i].ou, 0, 8);
        }
        
        if(ds.DIO[i].flt.isLost == RT_ERROR)
        {
            rt_memset(ds.DIO[i].in, 0, 8);
        }
    }
}    


void io_fault_detect(void)
{
    canNodes_detect();
    input_fault_detect();
    output_fault_detect();
    
    //故障指示灯
    if(ds.DIO[0].flt.isLost == RT_ERROR || ds.DIO[0].flt.can1 == RT_ERROR ||ds.DIO[0].flt.can1 == RT_ERROR
        || ds.DIO[0].ouFlt != 0 || ds.DIO[0].inFlt[0] != 0|| ds.DIO[0].inFlt[1] != 0
        || (ds.MCU[0].flt.isLost == RT_ERROR && ds.MCU[1].flt.isLost == RT_ERROR && ds.MCU[2].flt.isLost == RT_ERROR))
    {
        System_Led_SetMode(LED_FLT, MODE_FLASH_SLOW);
    }
    else
    {
        System_Led_SetMode(LED_FLT, MODE_OFF);
    }
}

