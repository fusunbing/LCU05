#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifdef __cplusplus
 extern "C" {
#endif

// ==========================================================================
// Communication Switch Control section.
// ==========================================================================
#define SERIAL_SOCKET_SUPPORT       /*use USART*/
//#define BSD_SOCKET_SUPPORT            /*use Socket*/

// ==========================================================================
// Thread priority and stack section.
// ==========================================================================
// For thread with FIFO scheduling policy (low to high)
#define HIGH_PRIO       (6) /* unused*/
#define NORM_HIGH_PRIO  (7) /* UserTasks*/
#define NORM_LOW_PRIO   (25)
#define LOWER_PRIO      (26)  /* remoting*/

#define TASK_PRO_ECLRMAIN (configMAX_PRIORITIES - 2)       /* Priority of eCLR main task.*/
#define tskIDLE_PRIORITY  ( RT_THREAD_PRIORITY_MAX - 3 )
//eclr_main() task stack size    
#define TASK_STACK_SIZE_ECLRMAIN    1024                    /* Amount of bytes used for stack of eCLR main task.*/

//use softTimer as eclr schedule interval
//#define USE_SOFT_TIMER  
   
#ifdef USE_SOFT_TIMER
/* The period after which the check timer will expire, in ms*/
#define mainCHECK_TIMER_PERIOD_MS	( 1UL / portTICK_RATE_MS )
/* A block time of zero simply means "don't block". */
#define mainDONT_BLOCK			( 0UL )   
#endif
   
// ==========================================================================
// Memory section.
// ==========================================================================

#ifdef EXECUTION_INPLACE_SUPPORT
#define OBJECT_HEAP_SIZE 24000   
#define DATA_RAM_SIZE    20000
#else  
//As there is not enough internel RAM, this is just a demo, you should change them based on your platform.
#define PLC_CODE_SIZE    28000 /*84000*/
#define DATA_RAM_SIZE    10000 
//#define OBJECT_HEAP_SIZE 12000
#endif /** EXECUTION_INPLACE_SUPPORT */

//Retain Memory
#define RETAIN_AMOUNT 100

// in XIP mode, the flash area for the PLC executable code
//#define ECLR_PLC_FLASH_START    (0x080A0000) // 增大kw flash程序空间大小（由256k增加至384k）
#define ECLR_PLC_FLASH_START    (0x080C0000) 
#define ECLR_PLC_FLASH_END      (0x080FFFFF)
#define ECLR_PLC_FLASH_SIZE 	(ECLR_PLC_FLASH_END - ECLR_PLC_FLASH_START + 1)   /*256k*/

// in XIR mode, the extern RAM area for the PLC executable code
extern char __plc_ext_ram_start;
extern char __plc_ext_ram_end;
#define ECLR_PLC_EXT_RAM_SIZE ((&__plc_ext_ram_end) - (&__plc_ext_ram_start) + 1)

// for both XIP and XIR mode, the internal RAM area for PLC data
#define ECLR_PLC_RAM_START     ((char*)0x10008000)
#define ECLR_PLC_RAM_END       ((char*)0x1000FFFF)
#define ECLR_PLC_RAM_SIZE      (0x1000FFFF-0x10008000+1)  /*(ECLR_PLC_RAM_END - ECLR_PLC_RAM_START + 1)*/

// for both XIP and XIR mode, the internal RAM area for eclr internal heap
#define ECLR_HEAP_START        ((char*)0x10000000)
#define ECLR_HEAP_END          ((char*)0x1000FFFF)
#define ECLR_HEAP_SIZE         (0x1000FFFF - 0x10000000+1)  /*(ECLR_HEAP_END - ECLR_HEAP_START + 1)   */ 
      
#ifdef __cplusplus
}
#endif

#endif /** __CONFIG_H__ */

