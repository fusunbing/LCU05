/***************************************************************************

Copyright (C), 1999-2014, Tongye Tech. Co., Ltd.

* @file           bsp_trdpservice.c
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
#include "trdp_if_light.h"
#include <string.h>
#include "lwip/def.h"
#include "lwip/inet.h"
#include "iec61375-2-3.h"
//#include "CanStack_Slave.h"
//#include "bsp_trdpdns.h"

//---------------------------------------------------------------------------
//                        Task Macro
//---------------------------------------------------------------------------
#define BSP_TRDP_SERVICE_STACKSIZE   (1 * 1024 )
#define BSP_TRDP_SERVICE_PRIORITY    (0x13)
//---------------------------------------------------------------------------
//                        Trdp Service  Macro
//---------------------------------------------------------------------------
#define TRDPSRV_SIZE                 (65537)
//---------------------------------------------------------------------------
//                        Task Object
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//   Task stack memory static allocate
//   Task object variable define
//---------------------------------------------------------------------------
static rt_uint32_t Bsp_TrdpService_Stack[BSP_TRDP_SERVICE_STACKSIZE];
static struct rt_thread Bsp_TrdpService_thread;
//---------------------------------------------------------------------------
//                        Trdp Service Macro
//---------------------------------------------------------------------------

#define MAX_PAYLOAD_SIZE  1024

#define PD_COMID0           2000
#define PD_COMID0_CYCLE     1000000
#define PD_COMID0_TIMEOUT   3200000

#define PD_COMID1           2900
#define PD_COMID1_CYCLE     100000
#define PD_COMID1_TIMEOUT   200000

#define PD_COMID2           2002
#define PD_COMID2_CYCLE     100000
#define PD_COMID2_TIMEOUT   1000000

#define PD_COMID3           2003
#define PD_COMID3_CYCLE     100000
#define PD_COMID3_TIMEOUT   1200000

#define MD_COMID_DNSREQ         140
#define MD_COMID_DNSREQ_TIMEOUT 3000000

//---------------------------------------------------------------------------
//                        Trdp Data Object
//---------------------------------------------------------------------------
static size_t      gDataSize   = 128;       /* Size of test data            */
static uint32_t    gComID      = PD_COMID0;
static uint32_t    gInterval   = PD_COMID0_CYCLE;
static char        gTargetIP[16];
static int         gDataChanged    = 1;
//static int         gIsActive       = 1;
static int32_t     gRecFD          = 0;

//---------------------------------------------------------------------------
//                        Trdp Typedef Object
//---------------------------------------------------------------------------
typedef struct md_receive_packet
{
    TRDP_LIS_T  lisHandle;
    TRDP_UUID_T sessionId;
    uint32_t    comID;
    uint32_t  timeout;
    char        srcIP[16];
    uint8_t     message[64];
    uint32_t    msgsize;
    uint32_t    replies;
    int         changed;
    int         invalid;
} MD_RECEIVE_PACKET_T;

typedef struct pd_receive_packet
{
    TRDP_SUB_T  subHandle;
    uint32_t    comID;
    uint32_t    timeout;
    char        srcIP[16];
    uint32_t    counter;
    uint8_t     message[64];
    int         changed;
    int         invalid;
    TRDP_PD_CALLBACK_T pfuction;
    TRDP_FLAGS_T trdpflag;
} PD_RECEIVE_PACKET_T;

typedef struct md_request_packet
{
    TRDP_LIS_T  lisHandle;
    TRDP_UUID_T sessionId;
    uint32_t    comID;
    uint32_t  timeout;
    char        srcIP[16];
    uint8_t     message[64];
    uint32_t    msgsize;
    uint32_t    replies;
    int         changed;
    int         invalid;
} MD_REQUEST_PACKET_T;


//typedef __packed struct pd_dataset
//{
//    uint8_t  LifeSignalH;
//    uint8_t  LifeSignalL;
//    uint8_t   Er[32];
//    uint8_t   EtuVer;
//    uint8_t   McuVer;
//    uint8_t   In[21];
//    uint8_t   Reserve[7];
//    uint8_t   Out[15];
//    uint8_t   Reserved[49];
//} PD_DATASET;

typedef __packed struct pd_dataset
{
    uint8_t  Er[32];
    uint8_t  In[21];
    uint8_t  Reserve[11];
    uint8_t  Out[17];
    uint8_t  Reserved[47];
} PD_DATASET;
//---------------------------------------------------------------------------
//                        Test Object
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//                      Private functions
//---------------------------------------------------------------------------
static void pdCallBack (void                    *pCallerRef,
                        TRDP_APP_SESSION_T      appHandle,
                        const TRDP_PD_INFO_T    *pMsg,
                        UINT8                   *pData,
                        UINT32                  dataSize);
static void mdCallback(void                    *pRefCon,
                       TRDP_APP_SESSION_T      appHandle,
                       const TRDP_MD_INFO_T    *pMsg,
                       UINT8                   *pData,
                       UINT32                  dataSize);
static void pd_getData (int index, uint8_t *data, int invalid);
static uint32_t  gray2hex (uint32_t in);
static void dbgOut (void        *pRefCon,
                    TRDP_LOG_T  category,
                    const CHAR8 *pTime,
                    const CHAR8 *pFile,
                    UINT16      LineNumber,
                    const CHAR8 *pMsgStr);
static void pd_sub (PD_RECEIVE_PACKET_T *recPacket);
static void md_listen (MD_RECEIVE_PACKET_T *recPacket);
static void Bsp_TrdpService_CreateApp(void);
static void    Bsp_TrdpService_Task(void* parameter);
static void FrameHeaderParsing(   void *pRefCon,
                                  TRDP_APP_SESSION_T appHandle,
                                  const TRDP_PD_INFO_T *pMsg,
                                  UINT8 *pData,
                                  UINT32 dataSize);
//---------------------------------------------------------------------------
//                        Trdp Object
//---------------------------------------------------------------------------
TRDP_APP_SESSION_T  gAppHandle;             /*  Our identifier to the library instance  */
TRDP_PUB_T          gPubHandle;             /*  Our identifier to the publication   */

MD_RECEIVE_PACKET_T gMessageData = {0};
MD_REQUEST_PACKET_T gRequestData = {0};

PD_RECEIVE_PACKET_T gRec[] =
{
    {0, TTDB_STATUS_COMID, PD_COMID2_TIMEOUT, "10.3.1.10", 0, "", 0, 1, NULL, TRDP_FLAGS_NONE},
    {0, PD_COMID1, (PD_COMID1_TIMEOUT*2), "10.128.193.10", 0, "", 0, 1,NULL,(TRDP_FLAGS_CALLBACK | TRDP_FLAGS_FORCE_CB)},
    //{0, 2900, (PD_COMID1_TIMEOUT * 2), "10.4.2.81", 0, "", 0, 1, NULL, (TRDP_FLAGS_CALLBACK | TRDP_FLAGS_FORCE_CB)},
    {0, 3100, 500000, "10.128.193.10", 0, "", 0, 1, NULL, (TRDP_FLAGS_CALLBACK | TRDP_FLAGS_FORCE_CB)},
    {0, PD_COMID2, PD_COMID2_TIMEOUT, "10.0.0.203", 0, "", 0, 1, NULL, TRDP_FLAGS_NONE},
    {0, PD_COMID3, PD_COMID3_TIMEOUT, "10.0.0.204", 0, "", 0, 1, NULL, TRDP_FLAGS_NONE},
    {0, 0, 0, "", 0, "", 0, 1, NULL}
};
UINT8       gDataBuffer[MAX_PAYLOAD_SIZE] =
{
    0xcf, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
};

PD_DATASET gData = {0};
/*  Buffer for our MD DNS   */
//static MD_REQUEST_DNS_PACKET DnsReqData = {0};
//---------------------------------------------------------------------------
//                      External functions
//---------------------------------------------------------------------------


/*******************************************************************************
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int pd_init (const char  *pDestAddress, uint32_t comID, uint32_t interval)
{
    TRDP_PD_CONFIG_T        pdConfiguration = {pdCallBack, NULL, {0, 0},
                                               TRDP_FLAGS_CALLBACK, 1000000, TRDP_TO_SET_TO_ZERO, 0
                                              };
    TRDP_MD_CONFIG_T        mdConfiguration = {mdCallback, NULL, {0, 0},
                                               TRDP_FLAGS_CALLBACK, 5000000, 5000000, 5000000, 0, 0, 2
                                              };
    TRDP_MEM_CONFIG_T       dynamicConfig   = {NULL, 204800, {0}};

    TRDP_PROCESS_CONFIG_T   processConfig   = {"Me", "", 0, 0, (TRDP_OPTION_BLOCK | TRDP_OPTION_NO_REUSE_ADDR | TRDP_OPTION_NO_MC_LOOP_BACK)};

    rt_kprintf("pd_init%d\n", sizeof(PD_DATASET));

    strcpy(gTargetIP, pDestAddress);
    gComID      = 2901;
    gInterval   = PD_COMID1_TIMEOUT;

    /*  Init the library for dynamic operation  (PD only) */
    if (tlc_init(dbgOut,                            /* actually printf  */
                 NULL,
                 &dynamicConfig                    /* Use application supplied memory   */
                ) != TRDP_NO_ERR)
    {
        rt_kprintf("Initialization error\n");
        return 1;
    }

    /*  Open a session for callback operation   (PD only) */
    if (tlc_openSession(&gAppHandle,
                        0, 0,                              /* use default IP addresses */
                        NULL,                              /* no Marshalling    */
                        &pdConfiguration, &mdConfiguration,            /* system defaults for PD and MD */
                        &processConfig) != TRDP_NO_ERR)
    {
        rt_kprintf("Initialization error\n");
        return 1;
    }

    /*  Subscribe to control PDs        */
    //pd_sub(&gRec[2]);
    //pd_sub(&gRec[0]);
    pd_sub(&gRec[1]);

    if (tlp_publish(gAppHandle,                     /*    our application identifier    */
                    &gPubHandle,            /*    our pulication identifier     */
                    RT_NULL,
                    RT_NULL,
                    2901,                   /*    ComID to send                 */
                    0,                         /*    local consist only            */
                    0,                         /*    no orient/direction info      */
                    0,                      /*    default source IP             */
                    //vos_dottedIP("10.129.1.81"),                      /*    default source IP             */
                    vos_dottedIP(gTargetIP),    /*    where to send to              */
                    gInterval,                /*    Cycle time in ms              */
                    0,                        /*    not redundant                 */
                    TRDP_FLAGS_NONE,          /*    No callbacks                  */
                    NULL,                     /*    default qos and ttl           */
                    (const UINT8 *)&gData,                     /*    initial data                  */
                    128) != TRDP_NO_ERR)
    {
        rt_kprintf("Publish error\n");
        return 1;
    }



    /* Set up listener */
//    gMessageData.lisHandle = NULL;
//    rt_memset(gMessageData.sessionId, 0, 16);
//    gMessageData.comID = 2000;
//  //gMessageData.timeout;
//    gMessageData.srcIP[0] = 0;
//    gMessageData.message[0] = 0;
//    gMessageData.msgsize = 64;
//    gMessageData.replies = 0;
//    gMessageData.changed = FALSE;
//    gMessageData.invalid = TRUE;

//    md_listen(&gMessageData);

//3
//  tlm_request (gAppHandle,
//               NULL,                        //const void              *pUserRef,
//               NULL,                        //TRDP_MD_CALLBACK_T      pfCbFunction,
//               &gRequestData.sessionId,     //TRDP_UUID_T             *pSessionId,
//               MD_COMID_DNSREQ,             //UINT32                  comId,
//               0,                           //UINT32                  etbTopoCnt,
//               0,                           //UINT32                  opTrnTopoCnt,
//               0,                           //TRDP_IP_ADDR_T          srcIpAddr,
//               vos_dottedIP("10.4.0.1"),    //TRDP_IP_ADDR_T          destIpAddr,
//               TRDP_FLAGS_NONE,             //TRDP_FLAGS_T            pktFlags        No callbacks,
//               0,                           //UINT32                  numReplies,
//               3000000,                     //UINT32                  replyTimeout,
//               3,                           //UINT32                  maxNumRetries,
//               NULL,                        //const TRDP_SEND_PARAM_T *pSendParam,
//              (UINT8 *) &DnsReqData,        //const UINT8             *pData,
//               138,                         //UINT32                  dataSize,
//               0,                           //const TRDP_URI_USER_T   sourceURI,
//               0);                          //const TRDP_URI_USER_T   destURI)

    Bsp_TrdpService_CreateApp();
    return 0;
}
/******************************************************************************/
/** callback routine for receiving TRDP traffic
 *
 *  @param[in]      pCallerRef      user supplied context pointer
 *  @param[in]      pMsg            pointer to message block
 *  @retval         none
 */
static void pdCallBack (void                    *pCallerRef,
                        TRDP_APP_SESSION_T      appHandle,
                        const TRDP_PD_INFO_T    *pMsg,
                        UINT8                   *pData,
                        UINT32                  dataSize)
{

    int i ;// for test
    /*  Check why we have been called   */
    switch (pMsg->resultCode)
    {
        case TRDP_NO_ERR:
            switch (pMsg->comId)
            {
                case 100:
                    rt_kprintf("PD 100 received\n");
                    break;
                case 1000:
                    rt_kprintf("PD 1000 received\n");
                    break;
                case PD_COMID1:
                    /*rt_kprintf("\r\nrt=%d,PD-2900  ip: 0x%x data:",rt_tick_get(), pMsg->srcIpAddr);
                    for(int i = 0; i < dataSize; i++)
                    {
                        rt_kprintf(" 0x%x", pData[i]);
                    }*/
                    
                    if (pMsg->srcIpAddr == ntohl(inet_addr(gRec[1].srcIP)))
                    {
                        rt_kprintf(" r(%d bytes)", dataSize);
                        pd_getData(1, pData, 0);
                    }
                    
                    break;
                case PD_COMID2:
                    break;
                case PD_COMID3:
                    break;
                default:
                    break;
            }
            //rt_kprintf(" +RT:%d ComID:%d r(%d Bytes)\n", rt_tick_get(), pMsg->comId, dataSize);
            break;

        case TRDP_TIMEOUT_ERR:
            /* The application can decide here if old data shall be invalidated or kept */
            ////rt_kprintf("\r\nPacket timed out (ComID %d, SrcIP: %s)", pMsg->comId, vos_ipDotted(pMsg->srcIpAddr));

        default:
            /*
              rt_kprintf("Error on packet received (ComID %d), err = %d\n",
                     pMsg->comId,
                     pMsg->resultCode);
             */
            break;
    }
}
/******************************************************************************/
/** Kind of specialized marshalling!
 *
 *  @param[in]      index           into our subscription array
 *  @param[in]      data            pointer to received data
 *  @param[in]      valid           flag for timeouts
 *  @retval         none
 */
static void pd_getData (int index, uint8_t *data, int invalid)
{
    uint16_t lifeTime = 0;
    
    gRec[index].invalid = invalid;
    
    if (!invalid && data != NULL)
    {
        switch(index)
        {
            case 1:
                /* comID 2900  tcms timedate*/
                lifeTime = *(uint16_t*)data;
                lifeTime = ntohs(lifeTime);
                gRec[index].counter = lifeTime;
                rt_memcpy(gRec[index].message, data, 16);
                *(uint16_t*)(gRec[index].message) = lifeTime;
                break;
            default:
                break;
        }
    }
    gRec[index].changed = 1;
}

int pd_vcuData(uint8_t* buf, uint16_t size)
{
    rt_memcpy(buf, gRec[1].message, size);
	
	  return 0;
}

/******************************************************************************/
static uint32_t  gray2hex (uint32_t in)
{
    static uint32_t last    = 0;
    uint32_t        ar[]    = {2, 0, 8, 0xc, 4, 6, 0xE};
    int i; //for test
    for ( i = 0; i < 7; i++)
    {
        if(ar[i] == in)
        {
            if (in == 2 && last > 3)
            {
                last = 7;
                return 7;
            }
            last = i;
            return i;
        }
    }
    return 0;
}
/******************************************************************************/
static void mdCallback(void                    *pRefCon,
                       TRDP_APP_SESSION_T      appHandle,
                       const TRDP_MD_INFO_T    *pMsg,
                       UINT8                   *pData,
                       UINT32                  dataSize)
{
    TRDP_ERR_T  err;
    /*  Check why we have been called   */
    switch (pMsg->resultCode)
    {
        case TRDP_NO_ERR:
            rt_kprintf("ComID %d received (%d Bytes)\n", pMsg->comId, dataSize);

            if (pMsg->msgType == TRDP_MSG_MR)
            {
                /* Send reply   */
                err = tlm_reply(appHandle, &pMsg->sessionId, gMessageData.comID, 0, NULL, (UINT8*)"Maleikum Salam", 16);
                //err = tlm_reply (appHandle, pRefCon, &pMsg->sessionId, 0, 0, gMessageData.comID, pMsg->srcIpAddr, pMsg->srcIpAddr,
                //          TRDP_FLAGS_CALLBACK, 0, NULL, (UINT8*)"Maleikum Salam", 16, NULL, NULL);
                if (err != TRDP_NO_ERR)
                {
                    rt_kprintf("Error repling data (ComID %d, SrcIP: %s)\n", pMsg->comId, vos_ipDotted(pMsg->srcIpAddr));
                }
                else
                {
                    gMessageData.invalid = 0;
                    gMessageData.changed = 1;
                }
            }
            else if (pMsg->msgType == TRDP_MSG_MP &&
                     pData && dataSize > 0 && dataSize <= 64)
            {
                gMessageData.comID = pMsg->comId;
                memcpy(gMessageData.message, pData, dataSize);
                gMessageData.msgsize = dataSize;
                gMessageData.replies++;
                gMessageData.changed = 1;

                if (memcmp(gMessageData.sessionId, pMsg->sessionId, sizeof(gMessageData.sessionId)) != 0)
                {
                    rt_kprintf("Unexpected data! (ComID %d, SrcIP: %s)\n", pMsg->comId, vos_ipDotted(pMsg->srcIpAddr));
                    gMessageData.invalid = 1;
                }
                else
                {
                    gMessageData.invalid = 0;
                }
            }
            break;
        case TRDP_REPLYTO_ERR:
        case TRDP_TIMEOUT_ERR:
            if (memcmp(gMessageData.sessionId, pMsg->sessionId, sizeof(gMessageData.sessionId)) == 0)
            {
                rt_kprintf("Session timed out (UUID: %02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx\n",
                           pMsg->sessionId[0],
                           pMsg->sessionId[1],
                           pMsg->sessionId[2],
                           pMsg->sessionId[3],
                           pMsg->sessionId[4],
                           pMsg->sessionId[5],
                           pMsg->sessionId[6],
                           pMsg->sessionId[7],
                           pMsg->sessionId[8],
                           pMsg->sessionId[9],
                           pMsg->sessionId[10],
                           pMsg->sessionId[11],
                           pMsg->sessionId[12],
                           pMsg->sessionId[13],
                           pMsg->sessionId[14],
                           pMsg->sessionId[15]
                          );
                gMessageData.message[0] = 0;
                gMessageData.msgsize = 0;
                gMessageData.replies = 0;
                gMessageData.changed = 1;
                gMessageData.invalid = 1;
            }
            /* The application can decide here if old data shall be invalidated or kept */
            rt_kprintf("Packet timed out (ComID %d, SrcIP: %s)\n", pMsg->comId, vos_ipDotted(pMsg->srcIpAddr));

        default:
            break;
    }
}
/**********************************************************************************************************************/
/** callback routine for TRDP logging/error output
 *
 *  @param[in]      pRefCon         user supplied context pointer
 *  @param[in]      category        Log category (Error, Warning, Info etc.)
 *  @param[in]      pTime           pointer to NULL-terminated string of time stamp
 *  @param[in]      pFile           pointer to NULL-terminated string of source module
 *  @param[in]      LineNumber      line
 *  @param[in]      pMsgStr         pointer to NULL-terminated string
 *  @retval         none
 */
static void dbgOut (void        *pRefCon,
                    TRDP_LOG_T  category,
                    const CHAR8 *pTime,
                    const CHAR8 *pFile,
                    UINT16      LineNumber,
                    const CHAR8 *pMsgStr)
{
    const char *catStr[] = {"**Error:", "Warning:", "   Info:", "  Debug:"};
    rt_kprintf("%s %s %s:%d %s",
               pTime,
               catStr[category],
               pFile,
               LineNumber,
               pMsgStr);
}
/******************************************************************************/
static void pd_sub (PD_RECEIVE_PACKET_T *recPacket)
{

    TRDP_ERR_T err;

    if (recPacket->subHandle != 0)
    {
        tlp_unsubscribe(gAppHandle, recPacket->subHandle);
        recPacket->subHandle = NULL;
    }

    err = tlp_subscribe(
              gAppHandle,                                 /*  our application identifier          */
              &recPacket->subHandle,                      /*  our subscription identifier         */
              NULL,
              NULL,
              recPacket->comID,                           /*  ComID                               */
              0,                                          /*  topocount: local consist only       */
              0,
              vos_dottedIP(recPacket->srcIP),             /*  Source IP filter 1                  */
              vos_dottedIP(recPacket->srcIP),
              vos_dottedIP(ETB0_ALL_END_DEVICES_IP),      /*  Default destination (or MC Group)   */
              recPacket->trdpflag,
              recPacket->timeout,                         /*  Time out in us                      */
              TRDP_TO_SET_TO_ZERO);                       /*  delete invalid data on timeout      */

    if (err != TRDP_NO_ERR)
    {
        rt_kprintf("trdp_subscribe error\n");
    }
}
/******************************************************************************/
//static void md_listen (MD_RECEIVE_PACKET_T *recPacket)
//{
//    TRDP_ERR_T err;
//      if (recPacket->lisHandle != NULL)
//    {
//        tlm_delListener(gAppHandle, recPacket->lisHandle);
//        recPacket->lisHandle = NULL;
//    }
//    err = tlm_addListener(
//            gAppHandle,                                 /*    our application identifier          */
//          &recPacket->lisHandle,                      /*  listener handle                     */
//          NULL,  NULL,                                /*  user ref                            */
//          recPacket->comID,                           /*  ComID                               */
//          0,                                          /*  topocount: local consist only       */
//            0,
//          0,                                          /*  any source address                  */
//          TRDP_FLAGS_CALLBACK,                        /*  use callbacks                       */
//          NULL);

//    if (err != TRDP_NO_ERR)
//    {
//       rt_kprintf("trdp_subscribe error\n");
//    }
//}
/*******************************************************************************
* Description    : 根据comID100的报文获取 EtbTopoCnt  和    opTrnTopoCnt
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void FrameHeaderParsing(   void                  *pRefCon,
                                  TRDP_APP_SESSION_T      appHandle,
                                  const TRDP_PD_INFO_T    *pMsg,
                                  UINT8                   *pData,
                                  UINT32                  dataSize)
{
    rt_kprintf("q/r/n");
    rt_kprintf("w/r/n");
    rt_kprintf("e/r/n");
    rt_kprintf("r/r/n");
}
/*******************************************************************************
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Bsp_TrdpService_CreateApp(void)
{
    rt_thread_init(&Bsp_TrdpService_thread,
                   "TrdpService",
                   Bsp_TrdpService_Task,
                   RT_NULL,
                   &Bsp_TrdpService_Stack[0],
                   sizeof(Bsp_TrdpService_Stack),
                   BSP_TRDP_SERVICE_PRIORITY,
                   20
                  );
    rt_thread_startup(&Bsp_TrdpService_thread);
}
/*******************************************************************************
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void    Bsp_TrdpService_Task(void* parameter)
{
    /* INT32           pd_fd = 0; */
    TRDP_ERR_T  err;
    int         rv = 2;
    int         trdptime = 0;
    fd_set  rfds;
    //INT32   noDesc = -1;
    INT32   noDesc = 0;
    TRDP_TIME_T  tv;
    TRDP_TIME_T  max_tv = {0, 800000};
    rt_kprintf("pd_init\n");

    /* Enter the main processing loop. */
    while (1)
    {
        noDesc = -1;

        if (gPubHandle != NULL)
        {
            /*  Copy the packet into the internal send queue, prepare for sending.
                If we change the data, just re-publish it   */
//						CanSlave_UpdateEth(((uint8_t*)&gData));
            err = tlp_put(gAppHandle, gPubHandle, ( const UINT8 *)&gData, gDataSize);

            if (err != TRDP_NO_ERR)
            {
                rt_kprintf("put pd error\n");
            }
            gDataChanged = 0;
        }

        /* Prepare the file descriptor set for the select call.
             Additional descriptors can be added here. */

        FD_ZERO(&rfds);
        FD_SET(gRecFD, &rfds);

        /*Compute the min. timeout value for select.
             This way we can guarantee that PDs are sent in time...
             (A requirement of the TRDP Protocol) */

        tlc_getInterval(gAppHandle, &tv, (TRDP_FDS_T *) &rfds, &noDesc);


        /*The wait time for select must consider cycle times and timeouts of
            the PD packets received or sent.
            If we need to poll something faster than the lowest PD cycle, */

        if (vos_cmpTime(&tv, &max_tv) > 0)
        {
            tv = max_tv;
        }

        /*Select() will wait for ready descriptors or time out,
            what ever comes first.*/

        //原来的做法是屏蔽了的，后面配套rv = 1来弥补 2019-5-9
        //期待观察TRDP计算的tv时间，是否和LWIP使用的tv时间是对等的
        /*
        struct timeval {
              long    tv_sec;         / seconds /
              long    tv_usec;        / and microseconds /
            };
        TRDP中 tv_usec 按照微秒放大了1000倍
        LWIP中 tv_usec 应该是按照毫秒计算的，需要实际印证
        */
        tv.tv_usec = tv.tv_usec / 1000;
        rv = vos_select((int)noDesc + 1, &rfds, NULL, NULL, &tv);


        /*Check for overdue PDs (sending and receiving)
         Send any PDs if it's time...
         Detect missing PDs...
         'rv' will be updated to show the handled events, if there are
         more than one...
         The callback function will be called from within the trdp_work
         function (in it's context and thread)! */
        //rv = 1;  // 2019-5-9
        tlc_process(gAppHandle, (TRDP_FDS_T *) &rfds, &rv);
        //rt_thread_delay(5);

        /*PD发送数据生命信号更新，待移走*/
//        gData.LifeSignalL += 1;
//        if(gData.LifeSignalL == 0xff)
//        {
//            gData.LifeSignalH += 1;
//        }

        gDataChanged = 1;
    }
}


