/***************************************************************************

Copyright (C), 1999-2014, Tongye Tech. Co., Ltd.

* @file           bsp_trdpdns.h
* @author        
* @version        V1.0.0
* @date           2016-06-29
* @brief          (1)  
                  (2) 
                  (3) 
History:          // Revision Records

<Author>             <time>       <version >            <desc>

care               2016-06-29       V1.0.0               

***************************************************************************/
#ifndef __BSP__TRDPDNS_H
#define __BSP__TRDPDNS_H
/* Includes --------------------------------------------------------------*/
#include "rtthread.h"
#include "stm32f4xx.h"

#include "trdp_proto.h"
#include "lwip/def.h"

//---------------------------------------------------------------------------
//                        Trdp Typedef Object
//---------------------------------------------------------------------------
typedef __packed  struct tcnUri{
		char      tcnUriStr[80];
		uint16_t  resolvState;
	  uint32_t  tcnUriIpAddr;
		uint32_t  tcnUriIpAddr2;
}TCNURI;

typedef __packed  struct md_request_dns_packet {
    uint16_t  version;
		uint16_t  reserved0;
    char      deviceName[16];
    uint32_t	trnTopoCnt;
	  uint32_t	opTrnTopoCnt;
	  uint8_t   etbId;
	  uint8_t   reserved1;
	  uint8_t   reserved2;
	  uint8_t   tcnUriCnt;  
		TCNURI    ARRAY[1];
		uint32_t  SafetyTrail0;
		uint32_t  SafetyTrail1;
		uint32_t  SafetyTrail2;
		uint32_t  SafetyTrail3;	
} MD_REQUEST_DNS_PACKET,*tMD_REQUEST_DNS_PACKET;


typedef __packed struct md_head_test
{
    UINT32  sequenceCounter;                    /**< Unique counter (autom incremented)                     */
    UINT16  protocolVersion;                    /**< fix value for compatibility                            */
    UINT16  msgType;                            /**< of datagram: Mn, Mr, Mp, Mq, Mc or Me                  */
    UINT32  comId;                              /**< set by user: unique id                                 */
    UINT32  etbTopoCnt;                         /**< set by user: ETB to use, '0' for consist local traffic */
    UINT32  opTrnTopoCnt;                       /**< set by user: direction/side critical, '0' if ignored   */
    UINT32  datasetLength;                      /**< defined by user: length of data to transmit            */
    INT32   replyStatus;                        /**< 0 = OK                                                 */
    UINT8   sessionID[16];                      /**< UUID as a byte stream                                  */
    UINT32  replyTimeout;                       /**< in us                                                  */
    UINT8   sourceURI[32];                      /**< User part of URI                                       */
    UINT8   destinationURI[32];                 /**< User part of URI                                       */
    UINT32  frameCheckSuM;                      /**< CRC32 of header                                        */
} MD_HEADER_TEST;



typedef __packed struct udp_md_reply_dns_packet{
		MD_HEADER_TEST  mdhead;
	  MD_REQUEST_DNS_PACKET payload;
    char  reserved[128];	
}UDPMD_REPLY_DNS_PACKET,*tUDPMD_REPLY_DNS_PACKET;






//------------------------------------------------------------------
//                      functions
//------------------------------------------------------------------
void md_dnsdata_init (tMD_REQUEST_DNS_PACKET data,const char  *url, const char  *name);
void md_data_printf(char *  mddata);
#endif 
