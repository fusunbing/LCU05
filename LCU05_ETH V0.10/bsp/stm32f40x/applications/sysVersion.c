
/***************************************************************************

Copyright (C), 1999-2012, Tongye Tech. Co., Ltd.

* @file           sysVersion.c
* @author          
* @version        V1.0.0
* @date            2017-05-25
* @brief          (1)  
                  (2) 
                  (3) 
History:          // Revision Records
   <Author>             <time>        <version >            <desc>
  life£¬Lily            2017-05-25       V1.0.0             ³õ½¨ÎÄ¼þ
***************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include "stm32f4xx.h"

/* define-----------------------------------------------------------*/
#define PROJECT_VERSION          1L              /**< major version number */
#define PROJECT_SUBVERSION       0L              /**< minor version number */
#define PROJECT_REVISION         9L              /**< revise version number */

typedef struct __ModuleVersion_TypeDef
{
	uint32_t version;
	const char*  buildDate;
	const char*  buildTime;
}ModuleVersion_TypeDef;

/* private variable-----------------------------------------------------------*/
static ModuleVersion_TypeDef projectVersion = { 0, "not init.", "not init." };

/* private function-----------------------------------------------------------*/
static void prtProjectVer(void);

void System_GetVersion(void);

/*******************************************************************************
* Function Name  :
* Description    : [5/25/2017 Lily]
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  System_GetVersion(void)
{
	projectVersion.version = (PROJECT_VERSION << 16) + (PROJECT_SUBVERSION << 8) + PROJECT_REVISION;
	projectVersion.buildDate = __DATE__;
	projectVersion.buildTime = __TIME__;
	prtProjectVer();
}

/*******************************************************************************
* Function Name  :
* Description    : [5/25/2017 Lily]
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void prtProjectVer(void)
{
	//_canservVersion.version
	rt_kprintf("\r\n");
	rt_kprintf(" Project version: %d.%d.%d \r\n", PROJECT_VERSION, PROJECT_SUBVERSION, PROJECT_REVISION);
	rt_kprintf(" Project build date: %s \r\n", projectVersion.buildDate);
	rt_kprintf(" Project build time: %s \r\n", projectVersion.buildTime);
}




/*@}*/
#include "finsh.h"
FINSH_FUNCTION_EXPORT(prtProjectVer, Print system Version)


/******************* (C) COPYRIGHT 2016 Shenzhen Tongye *****END OF FILE****/
