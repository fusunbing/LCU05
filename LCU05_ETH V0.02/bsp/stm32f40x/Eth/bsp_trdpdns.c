/***************************************************************************

Copyright (C), 1999-2014, Tongye Tech. Co., Ltd.

* @file           bsp_trdpdns.c
* @author        
* @version        V1.0.0
* @date           2016-06-29
* @brief          (1)  
                  (2) 
                  (3) 
History:          // Revision Records

<Author>             <time>       <version >            <desc>

care               2016-09-20       V1.0.0               

***************************************************************************/
/* Includes --------------------------------------------------------------*/
#include "bsp_trdpdns.h"

#include <string.h>


//---------------------------------------------------------------------------
//                        Trdp Object
//---------------------------------------------------------------------------


/*******************************************************************************
* Description    : 
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void md_dnsdata_init (tMD_REQUEST_DNS_PACKET data,const char  *url, const char  *name)
{
		strcpy(data->ARRAY[0].tcnUriStr,url);
	  strcpy(data->deviceName,name);
	  data->etbId = 0;
	  data->tcnUriCnt = 1;  
}

void md_data_printf(char *  data)
{ 
	  tUDPMD_REPLY_DNS_PACKET mddata = (tUDPMD_REPLY_DNS_PACKET)data;  
		TRDP_MSG_T msgtp =(TRDP_MSG_T)ntohs(mddata->mdhead.msgType);
		char ip[4] = {0};
		char ip1[4] = {0};
		ip[0] =  mddata->payload.ARRAY[0].tcnUriIpAddr & 0x0ff;
		ip[1] = (mddata->payload.ARRAY[0].tcnUriIpAddr >>8) & 0x0ff;
		ip[2] = (mddata->payload.ARRAY[0].tcnUriIpAddr >>16) & 0x0ff;
		ip[3] = (mddata->payload.ARRAY[0].tcnUriIpAddr >>24) & 0x0ff;
		ip1[0] =  mddata->payload.ARRAY[0].tcnUriIpAddr2 & 0x0ff;
		ip1[1] = (mddata->payload.ARRAY[0].tcnUriIpAddr2 >>8) & 0x0ff;
		ip1[2] = (mddata->payload.ARRAY[0].tcnUriIpAddr2 >>16) & 0x0ff;
		ip1[3] = (mddata->payload.ARRAY[0].tcnUriIpAddr2 >>24) & 0x0ff;
		rt_kprintf("ComID : %d   MD Type : ", ntohl(mddata->mdhead.comId));							
		switch (msgtp)
		{
			case TRDP_MSG_MN:
				rt_kprintf("MN\r\n");
				break;
			case TRDP_MSG_MR:
				rt_kprintf("MR\r\n");
				break;
			case TRDP_MSG_MP:
				rt_kprintf("MP\r\n");
				break;
			case TRDP_MSG_MQ:
				rt_kprintf("MQ\r\n");
				break;
			case TRDP_MSG_MC:
				rt_kprintf("MC\r\n");
				break;
			case TRDP_MSG_ME:
				rt_kprintf("ME\r\n");																															
				break;
			default:
					rt_kprintf("erro\r\n");
				break;
		}
		rt_kprintf("Payload length %d\r\n", ntohl(mddata->mdhead.datasetLength));
		rt_kprintf("tcnUriIpAddr: %x.%x.%x.%x\r\n",data[230], data[231], data[232], data[233]);
		rt_kprintf("tcnUriIpAddr2: %x.%x.%x.%x\r\n", ip1[0], ip1[1], ip1[2], ip1[3]);
}