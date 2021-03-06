

#include "Bsp_sysTask.h"
#include "Bsp_systimer.h"
#include "Bsp_eclrTask.h"
#include "Bsp_ITempMonit.h"
#include "Bsp_led.h"
#include "usart.h"
#include "Bsp_canTask.h"
#include "HwWDog_Max706.h"
#include "Bsp_gpio.h"
#include "userApp.h"
#include "Can_fault_detect.h"


#define SYS_TASK_STACK_SIZE         (1024)
#define SYS_TASK_PRIORITY           (10)


ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t sys_thread_stack[SYS_TASK_STACK_SIZE];
ALIGN(RT_ALIGN_SIZE)
static struct rt_thread sys_thread;

static struct rt_semaphore sem_10ms;
static rt_err_t sem_10ms_init;


static void versionFlt_detect(void)
{
    uint32_t i = 0;
    
    ds.MCU[ds.slotID - SLOT_ID_MCU_A].armVersion = MCU_VERSION;
    ds.MCU[ds.slotID - SLOT_ID_MCU_A].kwVersion = pKW_SHM->kwVer;

    for(i = 0; i < 3; i++)
    {
        if(ds.MCU[i].armVersion == 0 || ds.MCU[i].kwVersion == 0)
        {
            return;
        }
    }

    if(ds.MCU[0].armVersion != MCU_VERSION || ds.MCU[1].armVersion != MCU_VERSION || ds.MCU[2].armVersion  != MCU_VERSION)
    {
        pKW_SHM->me.verFlt.mcuVer = RT_ERROR;
    }
    
    if(ds.MCU[0].kwVersion != KW_VERSION || ds.MCU[1].kwVersion != KW_VERSION || ds.MCU[2].kwVersion  != KW_VERSION)
    {
        pKW_SHM->me.verFlt.kwVer = RT_ERROR;
    }
    
    if(ds.carID == CAR_ID_MC1 || ds.carID == CAR_ID_MC2)
    {
        for(i = 0; i < DIO_CNT_6U; i++)
        {
            if(ds.DIO[i].version != DIO_VERSION && ds.DIO[i].version != 0)
            {
                pKW_SHM->me.verFlt.dioVer = RT_ERROR;
                break;
            }
        }
    }
    else if(ds.carID == CAR_ID_TP1 || ds.carID == CAR_ID_TP2)
    {
        for(i = 0; i < DIO_CNT_3U; i++)
        {
            if(ds.DIO[i].version != DIO_VERSION && ds.DIO[i].version != 0)
            {
                pKW_SHM->me.verFlt.dioVer = RT_ERROR;
                break;
            }
        }
    }
}


static rt_err_t mcuAllOnline(void)
{
    if(ds.MCU[0].flt.lost == RT_ERROR
    || ds.MCU[1].flt.lost == RT_ERROR
    || ds.MCU[2].flt.lost == RT_ERROR)
    {
       return RT_ERROR;
    }
    else
    {
       return RT_EOK;
    }
}


static void canNodeFlt_detect(void)
{
    uint32_t i;    
    static uint32_t mcu_logic_cnt[3] = { 0 };
    static uint32_t mcu_remoteIn_cnt[3] = { 0 };
    
    for(i = 0; i < IO_BOARD_MAX; i++)
    {
        ds.DIO[i].flt.can1 = (Get_CanSts(1, (SLOT_ID_IO_MIN + i)) == RT_ERROR) ? RT_ERROR : RT_EOK;
        ds.DIO[i].flt.can2 = (Get_CanSts(2, (SLOT_ID_IO_MIN + i)) == RT_ERROR) ? RT_ERROR : RT_EOK;        
        ds.DIO[i].flt.lost = (ds.DIO[i].flt.can1 == RT_ERROR && ds.DIO[i].flt.can2 == RT_ERROR) ? RT_ERROR : RT_EOK;
        
        if(ds.DIO[i].inFlt[0] != 0 || ds.DIO[i].inFlt[1] != 0)
        {
            ds.DIO[i].flt.in = RT_ERROR;
        }
        else
        {
            ds.DIO[i].flt.in = RT_EOK;
        }
        
        if(ds.DIO[i].ouFlt != 0)
        {
            ds.DIO[i].flt.ou = RT_ERROR;
        }
        else
        {
            ds.DIO[i].flt.ou = RT_EOK;
        }
        
        if(ds.DIO[i].flt.can1 != RT_EOK
            || ds.DIO[i].flt.can2 != RT_EOK
            || ds.DIO[i].flt.lost != RT_EOK
            || ds.DIO[i].flt.in != RT_EOK
            || ds.DIO[i].flt.ou != RT_EOK)
        {
            ds.DIO[i].flt.board = RT_ERROR;
        }
        else
        {
            ds.DIO[i].flt.board = RT_EOK;
        }
        
        pKW_SHM->me.dio[i].board = ds.DIO[i].flt.board;
        pKW_SHM->me.dio[i].can1 = ds.DIO[i].flt.can1;
        pKW_SHM->me.dio[i].can2 = ds.DIO[i].flt.can2;
        pKW_SHM->me.dio[i].in = ds.DIO[i].flt.in;
        pKW_SHM->me.dio[i].ou = ds.DIO[i].flt.ou;
        pKW_SHM->me.dio[i].lost = ds.DIO[i].flt.lost;
        
        if(ds.DIO[i].flt.lost == RT_ERROR)
        {
            rt_memset(ds.DIO[i].in, 0, 8);
        }
    }

    rt_memcpy(ds.MCU[ds.slotID - SLOT_ID_MCU_A].ou, ds.ouBuf, 8);

    for(i = 0; i < 3; i++)
    {
        ds.MCU[i].flt.can1 = (Get_CanSts(1, (SLOT_ID_MCU_A + i)) == RT_ERROR) ? RT_ERROR : RT_EOK;
        ds.MCU[i].flt.can2 = (Get_CanSts(2, (SLOT_ID_MCU_A + i)) == RT_ERROR) ? RT_ERROR : RT_EOK;
        ds.MCU[i].flt.lost = (ds.MCU[i].flt.can1 == RT_ERROR && ds.MCU[i].flt.can2 == RT_ERROR) ? RT_ERROR : RT_EOK;
        
        if(rt_memcmp(ds.MCU[i].ou, ds.MCU[(i + 1) % 3].ou, 8) != 0 
            && rt_memcmp(ds.MCU[i].ou, ds.MCU[(i + 2) % 3].ou, 8) != 0
            && mcuAllOnline() == RT_EOK)
        {
            mcu_logic_cnt[i]++;
        }
        else
        {
            mcu_logic_cnt[i] = 0;
        }
        
        if(rt_memcmp(ds.MCU[i].remote_in, ds.MCU[(i + 1) % 3].remote_in, 8) != 0 
            && rt_memcmp(ds.MCU[i].remote_in, ds.MCU[(i + 2) % 3].remote_in, 8) != 0
            && mcuAllOnline() == RT_EOK)
        {
            mcu_remoteIn_cnt[i]++;
        }
        else
        {
            mcu_remoteIn_cnt[i] = 0;
        }

//        if(mcu_logic_cnt[i] > 10)
//        {
//            ds.MCU[i].flt.logic = RT_ERROR;
//        }

//        if(mcu_remoteIn_cnt[i] > 10)
//        {
//            ds.MCU[i].flt.remoteIn = RT_ERROR;
//        }

        ds.MCU[i].flt.logic = (mcu_logic_cnt[i] > 10) ? RT_ERROR : RT_EOK;
        ds.MCU[i].flt.remoteIn = (mcu_remoteIn_cnt[i] > 10) ? RT_ERROR : RT_EOK;

        if(ds.MCU[i].flt.can1 != RT_EOK
        || ds.MCU[i].flt.can2 != RT_EOK
        || ds.MCU[i].flt.lost != RT_EOK
        || ds.MCU[i].flt.logic != RT_EOK
        || ds.MCU[i].flt.remoteIn != RT_EOK)
        {
            ds.MCU[i].flt.board = RT_ERROR;
        }
        else
        {
            ds.MCU[i].flt.board = RT_EOK;
        }
        
        pKW_SHM->me.mcu[i].board = ds.MCU[i].flt.board;
        pKW_SHM->me.mcu[i].can1 = ds.MCU[i].flt.can1;
        pKW_SHM->me.mcu[i].can2 = ds.MCU[i].flt.can2;
        pKW_SHM->me.mcu[i].lost = ds.MCU[i].flt.lost;
       
        if(ds.MCU[i].flt.lost == RT_ERROR || ds.MCU[i].flt.logic == RT_ERROR)
        {
            rt_memset(ds.MCU[i].ou, 0, 8);
        }
        
        if(ds.MCU[i].flt.lost == RT_ERROR || ds.MCU[i].flt.remoteIn == RT_ERROR)
        {
            rt_memset(&ds.ouBuf[64], 0, 8);
        }
    }
    
    pKW_SHM->me.can.can1 =  (Get_CanSts(1, SLOT_ID_CAN) == RT_ERROR) ? RT_ERROR : RT_EOK;
    pKW_SHM->me.can.can2 =  (Get_CanSts(2, SLOT_ID_CAN) == RT_ERROR) ? RT_ERROR : RT_EOK;
    pKW_SHM->me.can.lost = (pKW_SHM->me.can.can1 == RT_ERROR && pKW_SHM->me.can.can2 == RT_ERROR) ? RT_ERROR : RT_EOK;
    pKW_SHM->me.can.ext_can1 = ds.can1_ExtCan;
    pKW_SHM->me.can.ext_can2 = ds.can2_ExtCan;
    
    if(pKW_SHM->me.can.can1 != RT_EOK
        || pKW_SHM->me.can.can2 != RT_EOK
        || pKW_SHM->me.can.lost != RT_EOK)
    {
        pKW_SHM->me.can.board = RT_ERROR;
    }
    else
    {
        pKW_SHM->me.can.board = RT_EOK;
    }
    
    pKW_SHM->me.mvb.can1 =  (Get_CanSts(1, SLOT_ID_MVB) == RT_ERROR) ? RT_ERROR : RT_EOK;
    pKW_SHM->me.mvb.can2 =  (Get_CanSts(2, SLOT_ID_MVB) == RT_ERROR) ? RT_ERROR : RT_EOK;
    pKW_SHM->me.mvb.lost = (pKW_SHM->me.mvb.can1 == RT_ERROR && pKW_SHM->me.mvb.can2 == RT_ERROR) ? RT_ERROR : RT_EOK;
    
    if(pKW_SHM->me.mvb.can1 != RT_EOK
        || pKW_SHM->me.mvb.can2 != RT_EOK
        || pKW_SHM->me.mvb.lost != RT_EOK)
    {
        pKW_SHM->me.mvb.board = RT_ERROR;
    }
    else
    {
        pKW_SHM->me.mvb.board = RT_EOK;
    }
    
    pKW_SHM->me.etu.can1 =  (Get_CanSts(1, SLOT_ID_ETU) == RT_ERROR) ? RT_ERROR : RT_EOK;
    pKW_SHM->me.etu.can2 =  (Get_CanSts(2, SLOT_ID_ETU) == RT_ERROR) ? RT_ERROR : RT_EOK;
    pKW_SHM->me.etu.lost = (pKW_SHM->me.etu.can1 == RT_ERROR && pKW_SHM->me.etu.can2 == RT_ERROR) ? RT_ERROR : RT_EOK;
    
    if(pKW_SHM->me.etu.can1 != RT_EOK
        || pKW_SHM->me.etu.can2 != RT_EOK
        || pKW_SHM->me.etu.lost != RT_EOK)
    {
        pKW_SHM->me.etu.board = RT_ERROR;
    }
    else
    {
        pKW_SHM->me.etu.board = RT_EOK;
    }
    
    for(i = 0; i < 4; i++)
    {
        pKW_SHM->me.car[i].can1 = Get_ExtCanSts(1, i);
        pKW_SHM->me.car[i].can2 = Get_ExtCanSts(2, i);
        pKW_SHM->me.car[i].lost = (pKW_SHM->me.car[i].can1 == RT_ERROR && pKW_SHM->me.car[i].can2 == RT_ERROR) ? RT_ERROR : RT_EOK;

        if(pKW_SHM->me.car[i].lost == RT_ERROR)
        {
            rt_memset(&ds.inBuf[64 + 8 * i], 0, 8);
        }
    }
}


static void powerFlt_detect(void)
{
    static uint32_t dc110v_a_cnt;
    static uint32_t dc5v_a_cnt;
    static uint32_t dc110v_b_cnt;
    static uint32_t dc5v_b_cnt;
    static uint32_t dc110v_c_cnt;
    static uint32_t dc5v_c_cnt;

    ds.dc110v = GetPin(DC110V_DETECT);
    ds.dc5v = GetPin(DC5V_DETECT);
    
    ds.MCU[ds.slotID - SLOT_ID_MCU_A].dc110v = ds.dc110v;
    ds.MCU[ds.slotID - SLOT_ID_MCU_A].dc5v = ds.dc5v;
    
    if(ds.MCU[0].dc110v == 0)
    {
        dc110v_a_cnt++;
    }
    else
    {
        dc110v_a_cnt = 0;
    }    
    
    if(ds.MCU[0].dc5v == 0 && ds.MCU[0].dc110v == 1)
    {
        dc5v_a_cnt++;
    }
    else
    {
        dc5v_a_cnt = 0;
    }

    if(ds.MCU[1].dc110v == 0)
    {
        dc110v_b_cnt++;
    }
    else
    {
        dc110v_b_cnt = 0;
    }    
    
    if(ds.MCU[1].dc5v == 0 && ds.MCU[1].dc110v == 1)
    {
        dc5v_b_cnt++;
    }
    else
    {
        dc5v_b_cnt = 0;
    }

    if(ds.MCU[2].dc110v == 0)
    {
        dc110v_c_cnt++;
    }
    else
    {
        dc110v_c_cnt = 0;
    }    
    
    if(ds.MCU[2].dc5v == 0 && ds.MCU[2].dc110v == 1)
    {
        dc5v_c_cnt++;
    }
    else
    {
        dc5v_c_cnt = 0;
    }
    
    pKW_SHM->me.pwr.dc110v_a = (dc110v_a_cnt > 5) ? RT_ERROR : RT_EOK;
    pKW_SHM->me.pwr.dc5v_a = (dc5v_a_cnt > 5) ? RT_ERROR : RT_EOK;
    
    pKW_SHM->me.pwr.dc110v_b = (dc110v_b_cnt > 5) ? RT_ERROR : RT_EOK;
    pKW_SHM->me.pwr.dc5v_b = (dc5v_b_cnt > 5) ? RT_ERROR : RT_EOK;
    
    pKW_SHM->me.pwr.dc110v_c = (dc110v_c_cnt > 5) ? RT_ERROR : RT_EOK;
    pKW_SHM->me.pwr.dc5v_c = (dc5v_c_cnt > 5) ? RT_ERROR : RT_EOK;
}


static rt_err_t GetPwrFlt(uint8_t slotID)
{
    rt_err_t ret = RT_EOK;

    switch(slotID)
    {
        case SLOT_ID_MCU_A:
            if(pKW_SHM->me.pwr.dc110v_a == RT_ERROR || pKW_SHM->me.pwr.dc5v_a == RT_ERROR)
            {
                ret = RT_ERROR;
            }
            break;

        case SLOT_ID_MCU_B:
            if(pKW_SHM->me.pwr.dc110v_b == RT_ERROR || pKW_SHM->me.pwr.dc5v_b == RT_ERROR)
            {
                ret = RT_ERROR;
            }
            break;

        case SLOT_ID_MCU_C:
            if(pKW_SHM->me.pwr.dc110v_c == RT_ERROR || pKW_SHM->me.pwr.dc5v_c == RT_ERROR)
            {
                ret = RT_ERROR;
            }
            break;
    
        default:
            ret = RT_ERROR;
            break;
    }

    return ret;
}


static rt_err_t canNodeSts(void)
{
    rt_err_t ret = RT_EOK;

    if(ds.carID == CAR_ID_MC1 || ds.carID == CAR_ID_MC2)
    {
        if(ds.DIO[0].flt.board == RT_ERROR || ds.DIO[1].flt.board == RT_ERROR || ds.DIO[2].flt.board == RT_ERROR ||
            ds.DIO[3].flt.board == RT_ERROR || ds.DIO[4].flt.board == RT_ERROR || ds.DIO[5].flt.board == RT_ERROR ||
            ds.DIO[6].flt.board == RT_ERROR || ds.DIO[7].flt.board == RT_ERROR || ds.DIO[8].flt.board == RT_ERROR ||
            ds.DIO[9].flt.board == RT_ERROR || ds.DIO[10].flt.board == RT_ERROR || ds.DIO[11].flt.board == RT_ERROR ||
            ds.DIO[12].flt.board == RT_ERROR || ds.DIO[13].flt.board == RT_ERROR || ds.DIO[14].flt.board == RT_ERROR ||
            ds.DIO[15].flt.board == RT_ERROR || ds.DIO[16].flt.board == RT_ERROR || ds.DIO[17].flt.board == RT_ERROR ||
            ds.DIO[18].flt.board == RT_ERROR || ds.DIO[19].flt.board == RT_ERROR || ds.DIO[20].flt.board == RT_ERROR ||
            ds.DIO[21].flt.board == RT_ERROR || ds.DIO[22].flt.board == RT_ERROR || ds.DIO[23].flt.board == RT_ERROR ||
            ds.DIO[24].flt.board == RT_ERROR || ds.DIO[25].flt.board == RT_ERROR || ds.DIO[26].flt.board == RT_ERROR ||
            pKW_SHM->me.can.board == RT_ERROR || pKW_SHM->me.etu.board == RT_ERROR || pKW_SHM->me.mvb.board == RT_ERROR)
        {
            ret = RT_ERROR;
        }
        else
        {
            ret = RT_EOK;
        }
    }
    else if(ds.carID == CAR_ID_TP1 || ds.carID == CAR_ID_TP2)
    {
        if(ds.DIO[0].flt.board == RT_ERROR || ds.DIO[1].flt.board == RT_ERROR || ds.DIO[2].flt.board == RT_ERROR ||
            ds.DIO[3].flt.board == RT_ERROR || ds.DIO[4].flt.board == RT_ERROR || ds.DIO[5].flt.board == RT_ERROR ||
            ds.DIO[6].flt.board == RT_ERROR || ds.DIO[7].flt.board == RT_ERROR || ds.DIO[8].flt.board == RT_ERROR ||
            pKW_SHM->me.can.board == RT_ERROR || pKW_SHM->me.etu.board == RT_ERROR)
        {
            ret = RT_ERROR;
        }
        else
        {
            ret = RT_EOK;
        }
    }
    
    return ret;
}    


static void sys_thread_entry(void* parameter)
{
    static  rt_uint32_t  TimingDelay = 0;
    
    rt_kprintf("\r\n+ BSP TASK INFO : BSP Task Init Success!\r\n");

    for(;;)
    {
        //等待系统操作信号量
        rt_sem_take(&sem_10ms, RT_WAITING_FOREVER);

        TimingDelay++;

#if IWDG_EN
        IWDG_ReloadCounter();       // 复位软件狗
        HwWDog_Feed();              // 复位硬件狗
#endif
        if(TimingDelay % 20 == 0)
        {
            pKW_SHM->me.lifeSignal++;
            
            if(rt_tick_get() > 2000)
            {
                canNodeFlt_detect();
                powerFlt_detect();
            }
            
            if(rt_tick_get() > 2000 && rt_tick_get() < 3000)
            {
                versionFlt_detect();
            }
            
            rt_memcpy(pKW_SHM->me.inBuf, ds.inBuf, 14);
            rt_memcpy(pKW_SHM->me.ouBuf, ds.ouBuf, 8);
            
            if(GetPwrFlt(ds.slotID) == RT_ERROR
                || pKW_SHM->me.verFlt.mcuVer == RT_ERROR
                || pKW_SHM->me.verFlt.kwVer == RT_ERROR
                || pKW_SHM->me.verFlt.dioVer == RT_ERROR)
            {
                ds.fltLevel = 1;
            }
            else if(ds.MCU[0].flt.board == RT_ERROR
                 || ds.MCU[1].flt.board == RT_ERROR
                 || ds.MCU[2].flt.board == RT_ERROR)
            {
                ds.fltLevel = 2;
            }
            else if(canNodeSts() == RT_ERROR)
            {
                ds.fltLevel = 4;
            }
            else
            {
                ds.fltLevel = 0;
            }
            
            switch(ds.fltLevel)
            {
                case 0:
                    System_Led_SetMode(LED_FLT, MODE_OFF);
                    break;
                case 1:
                    System_Led_SetMode(LED_FLT, MODE_ON);
                    break;
                case 2:
                    System_Led_SetMode(LED_FLT, MODE_FLASH_FAST);
                    break;
                case 4:
                    System_Led_SetMode(LED_FLT, MODE_FLASH_SLOW);
                    break;
                default:
                    System_Led_SetMode(LED_FLT, MODE_ON);
                    break;
            }
        }
        

        if(TimingDelay % 4 == 0)
        {
            System_Led_Logic(); // LED指示灯
        }
    }
}


static void Bsp_sysTask_Init(void)
{
    rt_err_t ret;
    
    sem_10ms_init  = rt_sem_init(&sem_10ms, "Sem10ms", 1, RT_IPC_FLAG_FIFO);

    ret = rt_thread_init(&sys_thread,
            "BspSysTask",
            sys_thread_entry,
            RT_NULL,
            sys_thread_stack, 
            SYS_TASK_STACK_SIZE,
            SYS_TASK_PRIORITY, 
            8);
        
    if(RT_EOK == ret)
    {
        rt_thread_startup(&sys_thread);
    }
}


void sem_10ms_Release(void)
{
    if(sem_10ms_init == RT_EOK)
    {
        rt_sem_release(&sem_10ms);
    }
}

void System_bsptask_create(void)
{
    rt_kprintf("\r\n[*] System all task Init Start ...... \r\n");
    rt_kprintf(" + \r\n");
    rt_kprintf(" + \r\n");
    
    console_services_init();
    
    /* 系统定时器硬件初始化 */
    Bsp_systimer_Init();    
    rt_kprintf("\r\n+ TIM INFO: Init success!\r\n");
    
    /* 系统任务线程初始化 */
    Bsp_sysTask_Init();
    
    /* CAN硬件及线程初始化 */
    Bsp_CanInit();
    
    /* KW线程初始化 */
    Bsp_eclrTask_Init();
}


void time_tick(void)
{
    rt_uint32_t tick = 0;
    tick = rt_tick_get();

    rt_kprintf("the rt_tick is : %d\r\n", tick);
}

void running_time(void)
{
    rt_uint8_t  hour = 0;
    rt_uint8_t  min  = 0;
    rt_uint8_t  sec  = 0;
    rt_uint32_t tick = 0;

    tick = rt_tick_get();

    hour = (rt_uint8_t)(tick/3600000);
    min  = (rt_uint8_t)((tick/60000)%60);
    sec  = (rt_uint8_t)((tick/1000)%60);

    rt_kprintf("the system running total time is : %3d:",hour);
    rt_kprintf("%02d:",min);
    rt_kprintf("%02d\r\n",sec);
}

#include "finsh.h"
FINSH_FUNCTION_EXPORT(time_tick, rt_tick show)
FINSH_FUNCTION_EXPORT(running_time, running time show)

