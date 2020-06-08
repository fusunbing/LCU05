/*
 * File      : usart.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

#ifndef __USART_H__
#define __USART_H__


#include <rthw.h>
#include <rtthread.h>
#include "stm32f4xx.h"


void stm32_hw_usart_init( void );
void reset_dma_address( void );
void mem_printf( const char *buffer, rt_size_t size );
void console_services_init( void );

#endif
