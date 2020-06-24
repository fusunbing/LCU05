/**
  *************************************************************************
  * This file is part of Platform
  * COPYRIGHT (C) 1999-2014, TongYe R&D Team
  * @file    static_mem.h
  * @brief   This file contains all the functions prototypes for the   
  *          static_mem firmware library.
  * Change Logs:
  * Date           Author       Notes
  * 2014/04/14     Coase        the first version
  *************************************************************************
  */

#ifndef __STATIC_MEM__H__
#define __STATIC_MEM__H__


#ifdef STM32F40_41xxx

#include <stdio.h>

#define STATIC_MEM_ALIGN_SIZE	8

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief malloc memory that can't free
 *
 * @param size of memory to malloc
 *
 * @return memory address, if failed , return NULL.
 */
void * static_malloc(size_t size);

#ifdef __cplusplus
}
#endif


#endif
#endif

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/


/*@}*/

