/**
  *************************************************************************
  * This file is part of Platform
  * COPYRIGHT (C) 1999-2014, TongYe R&D Team
  * @file    static_mem.c
  * @brief   This file provides all the static_mem firmware functions.
  * Change Logs:
  * Date           Author       Notes
  * 2014/04/14     Coase        the first version
  *************************************************************************
  */
#include "stm32f4xx.h"
#if defined(STM32F40_41xxx) || defined(STM32F40XX)

#include "static_mem.h"
#include "rthw.h"

static void * current_address = (void *)0x10000000;
static void * end_address =     (void *)0x10010000;

static size_t align(size_t size)
{
    //此算法仅用于STATIC_MEM_ALIGN_SIZE为2的幕的对齐运算。
    return ((size + STATIC_MEM_ALIGN_SIZE - 1) 
        & (~ (STATIC_MEM_ALIGN_SIZE - 1)));
}

static void * get_new_address(size_t size)
{
    return (void *)((size_t)current_address + size);
}

void * static_malloc( size_t size )
{
    void * rtn = current_address;
    void * new_address = NULL;
    int is_enter_critical = 0;
    if ((void*)(rt_thread_self()->name) != RT_NULL)
    {
        is_enter_critical = 1;
        rt_enter_critical();
    }

    if (size == 0)
    {
        return NULL;
    }

    size = align(size);
    
    new_address = get_new_address(size);
    
    if (new_address > end_address)
    {
        return NULL;
    }

    current_address = new_address;
    if (is_enter_critical)
    {
        rt_exit_critical();
    }
    return rtn;
}

#endif
/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/


/*@}*/

