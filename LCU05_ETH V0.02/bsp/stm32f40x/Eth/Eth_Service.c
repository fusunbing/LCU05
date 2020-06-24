/***************************************************************************

Copyright (C), 1999-2014, Tongye Tech. Co., Ltd.

* @file           bsp_ethdrive.c
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
/* Includes --------------------------------------------------------------*/
#include "Eth_Service.h"
#include "stm32f4xx_eth.h"
#include <lwip/sockets.h>
#include "netif/ethernetif.h"
#include "lwip/netif.h"
#include "lwip/netifapi.h"
#include "Trdp_Service.h"
#include "static_mem.h"
#include "fat_service.h"
#include "ModBusTcp.h"
#include "common.h"

//---------------------------------------------------------------------------
//                        Task Macro 
//---------------------------------------------------------------------------
#define ETH_SERVICE_STACKSIZE       (8 * 1024 )  
#define ETH_SERVICE_PTU_STACKSIZE   (2 * 1024 )  
#define ETH_SERVICE_PRIORITY        (0x0f)
#define PTU_ETH_PORT                (59202U)
#define PTU_ETH_TX_PORT             (55555U)
#define PTU_ETH_RX_SIZE             (1U * 1024U)
#define PTU_ETH_TX_SIZE             (8U * 1024U)

#define  ETHSERV_ST_INIT        (0x00)
#define  ETHSERV_ST_CFGWAIT     (0x01)
#define  ETHSERV_ST_CFG         (0x02)
#define  ETHSERV_ST_SERVECREAT  (0x03)
#define  ETHSERV_ST_NORMAL      (0x04)
#define  ETHSERV_ST_FAULT       (0xFF)

typedef struct _eth_service_controlData_
{
	uint8_t  serviceSt;
	uint8_t  cfgvalidFlag;
}ETHSRVICE_CONTROL;
//---------------------------------------------------------------------------
//                        Task Object
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//   Task stack memory static allocate
//   Task object variable define
//---------------------------------------------------------------------------
static struct rt_thread  EthService_thread;
static struct rt_thread  EthServicePTU_thread;
static struct rt_thread  EthServiceModBus_thread;
static  ETHSRVICE_CONTROL  ethServ_control = {0};
static  uint8_t* recv_data;
static  uint8_t tx_data[PTU_ETH_TX_SIZE] = {0};
static void (*ptu_process)(uint8_t* input, uint8_t* outbuff, uint32_t* size);

static uint8_t ethService_staticMem[ETH_SERVICE_STACKSIZE] = {0};

//---------------------------------------------------------------------------
//                      Private functions 
//---------------------------------------------------------------------------
static void  EthService_Task(void* parameter);

//---------------------------------------------------------------------------
//                      External functions 
//---------------------------------------------------------------------------
extern void lwip_sys_init(void);
static void EthService_CreateThread(void);
static void EthService_Init(void);
static void EthService_PDInit(void);
static void EthService_PtuTask(void* parameter);
static void EthService_CreatePTUSrv(void);
static void EthService_ModBusTask(void* parameter);
static void EthService_CreateModBusSrv(void);


/*******************************************************************************
* Description    : 
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EthService_RegPtuHook(void(*pPro)(uint8_t* input, uint8_t* outbuff, uint32_t* size))
{
    ptu_process = pPro;
}

/*******************************************************************************
* Description    : 
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  EthService_CreateApp(void)
{
	EthService_CreateThread();
}

/*******************************************************************************
* Description    : 
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void  EthService_CreateThread(void)
{
    void* EthService_Stack;
    rt_err_t ret;
    
    //EthService_Stack = static_malloc(ETH_SERVICE_STACKSIZE);
    EthService_Stack = (void*)ethService_staticMem;

	ret = rt_thread_init(&EthService_thread,
        "EthService",
		EthService_Task,
		RT_NULL,
		EthService_Stack,
        ETH_SERVICE_STACKSIZE,
		ETH_SERVICE_PRIORITY,
		20
		);

	if (ret == RT_EOK)
	{
		rt_thread_startup(&EthService_thread);
	}
}


extern void netsetaddr(const char *nameip, const char *namemk, const char *namegw);
/* 针对以太网IP进行配置的部分 */
static void SetEthIP(uint8_t box_id)
{
    const char* ip = "10.10.1.77"; //非LCU IP
     
    
//    switch(box_id)
//    {
//        case 0:
//            ip = "10.10.1.77";
//            break;
//        case 1:
//            ip = "10.10.1.78";
//            break;
//        case 2:
//            ip = "10.4.2.80";
//            break;
//        case 3:
//            ip = "10.4.3.80";
//            break;
//        case 4:
//            ip = "10.4.4.80";
//            break;
//        case 5:
//            ip = "10.4.5.80";
//            break;
//        case 6:
//            ip = "10.4.6.81";
//            break;
//        case 7:
//            ip = "10.4.6.80";
//            break;
//        case 8:
//            ip = "10.4.2.81"; //不使用的IP
//            break;
//        case 9:
//            ip = "10.4.2.81"; //不使用的IP
//            break;
//        
//        default:
//            break;
//    }
    
    netsetaddr(ip, "255.255.255.0", "10.10.1.1");
}


/*******************************************************************************
* Description    : 
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void  EthService_Task(void* parameter)
{
	ethServ_control.serviceSt = ETHSERV_ST_INIT;
	
	for (;;)
	{
		switch(ethServ_control.serviceSt)
		{
			case ETHSERV_ST_INIT:
				EthService_Init();
                ethServ_control.serviceSt = ETHSERV_ST_CFG;
				break;
			case ETHSERV_ST_CFGWAIT:
				break;

			case ETHSERV_ST_CFG:
                ethServ_control.serviceSt = ETHSERV_ST_SERVECREAT;
				break;

			case ETHSERV_ST_SERVECREAT:
                
                SetEthIP(ds.carID);
                EthService_CreatePTUSrv();
                rt_thread_delay(1000);
                //EthService_CreateModBusSrv();
                //EthService_PDInit();
                ethServ_control.serviceSt = ETHSERV_ST_NORMAL;
				break;
			
			case ETHSERV_ST_NORMAL:
				break;
			case ETHSERV_ST_FAULT:
				break;
			default:
				break;
		}
		rt_thread_delay(100);
	}
}

/*******************************************************************************
* Description    : 
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void  EthService_CreatePTUSrv(void)
{
    void* thread_Stack;
    rt_err_t ret;
    
    thread_Stack = static_malloc(ETH_SERVICE_PTU_STACKSIZE);
    /* ptu_process = FatPtu_DownloadEvt; */
    EthService_RegPtuHook(FatPtu_DownloadEvt);

	ret = rt_thread_init(&EthServicePTU_thread,
        "EthPtu",
		EthService_PtuTask, /**/
		RT_NULL,
		thread_Stack,
        ETH_SERVICE_PTU_STACKSIZE,
		ETH_SERVICE_PRIORITY + 5,
		10
		);

	if (ret == RT_EOK)
	{
		rt_thread_startup(&EthServicePTU_thread);
	}
}


#define _debug_    /* 调试使用 正式代码要删除此处的宏定义 */
#define _tx_echo_

static void  EthService_PtuTask(void* parameter)
{
	int sock_rx = -1;
	int sock_tx = -1;

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	struct sockaddr_in Tx_server_addr;

	int bytes_read;
	uint32_t addr_len;
    uint32_t txCnt = 0;

    /* ETH Ptu 收发缓冲区最大空间申请 */
    recv_data = static_malloc(PTU_ETH_RX_SIZE);
    /*tx_data = static_malloc(PTU_ETH_TX_SIZE);  
      tx_data由于要被SD卡驱动操作，SD卡需用到DMA，所以tx_data不能分配到CCM内存中 */

	/* 创建一个socket，类型是SOCK_DGRAM，UDP类型  */
	if ((sock_rx = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		rt_kprintf("Socket Server error\n");

		return;
	}

	/* 创建一个socket，类型是SOCK_DGRAM，UDP类型  */
	if ((sock_tx = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		rt_kprintf("Socket Tx error\n");
		return;
	}


	/*  初始化服务端地址 */
	server_addr.sin_family      = AF_INET;
	server_addr.sin_port        = htons(PTU_ETH_PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	rt_memset(&(server_addr.sin_zero),0, sizeof(server_addr.sin_zero));

	/*  绑定socket到服务端地址 */
	if (bind(sock_rx,(struct sockaddr *)&server_addr,
		sizeof(struct sockaddr)) == -1)
	{
		/* 绑定地址失败  */
		rt_kprintf("Bind error\n");

		return;
	}


    //-----------------------------------------------------------------------
	// 初始化预连接的服务端地址 
	//         端口和IP地址 的内容在初始化时, 填写什么内容都可以
	//-----------------------------------------------------------------------
	Tx_server_addr.sin_family      = AF_INET;
	Tx_server_addr.sin_port        = htons(60355);
	Tx_server_addr.sin_addr.s_addr = inet_addr("192.168.2.208");
	rt_memset(&(Tx_server_addr.sin_zero), 0, sizeof(Tx_server_addr.sin_zero));


	addr_len = sizeof(struct sockaddr);
	rt_kprintf("PTU_Server Waiting for client on port : %d...\n", PTU_ETH_PORT);

	//mUapp_PtuApi_Init();

	for(;;)
	{

		/* 从sock_rx中收取最大 buffer_size 字节数据 */
		bytes_read = recvfrom(sock_rx, recv_data, PTU_ETH_RX_SIZE, 0,
			(struct sockaddr *)&client_addr, (unsigned long*)&addr_len);
		/* UDP不同于TCP，它基本不会出现收取的数据失败的情况，除非设置了超时等待  */

#ifdef _debug_    
		recv_data[bytes_read] = '\0'; /* 把末端清零 */

		/* 输出接收的数据 
		rt_kprintf("\n(%s , %d) said : ",inet_ntoa(client_addr.sin_addr),
			ntohs(client_addr.sin_port));*/
		//rt_kprintf("%s", recv_data);
		rt_kprintf("%d Bytes  \r", bytes_read);

#endif

        if(ptu_process != RT_NULL)
        {
            ptu_process(recv_data, tx_data, &txCnt);
#ifdef _tx_echo_
            Tx_server_addr.sin_addr = client_addr.sin_addr;
            Tx_server_addr.sin_port = htons(PTU_ETH_TX_PORT);//htons(55555);//client_addr.sin_port;

            if((txCnt > 0) && (txCnt <= PTU_ETH_TX_SIZE))
            {
                /* 发送数据到服务远端 */
                sendto(sock_tx, tx_data, txCnt, 0, (struct sockaddr *)&Tx_server_addr, sizeof(struct sockaddr));
            }
#endif    
        }


	}
        
}

/*******************************************************************************
* Description    : 
*                   ModBus Client
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void  EthService_CreateModBusSrv(void)
{
    void* thread_Stack;
    rt_err_t ret;
    
    thread_Stack = static_malloc(ETH_SERVICE_PTU_STACKSIZE);
    /* ptu_process = FatPtu_DownloadEvt; */
    //EthService_RegPtuHook(FatPtu_DownloadEvt);

	ret = rt_thread_init(&EthServiceModBus_thread,
        "EthModBus",
		EthService_ModBusTask, /**/
		RT_NULL,
		thread_Stack,
        ETH_SERVICE_PTU_STACKSIZE,
		ETH_SERVICE_PRIORITY + 4,
		10
		);

	if (ret == RT_EOK)
	{
		rt_thread_startup(&EthServiceModBus_thread);
	}
}

static uint8_t msg[12] = {0x30,0x90,0x00,0x00,0x00,0x06,0x01,0x03,0x00,0x00,0x00,0x01};
static uint8_t ModBusSendBuffer[1024] = {0};
static int modbusCommSt = 0;
static void  EthService_ModBusTask(void* parameter)
{
    struct sockaddr_in server_addr;
	int sock_fd;
    int bytes_read, sendBytes;
	struct sockaddr_in client_addr;
	uint32_t addr_len;
    uint8_t* rBuf;
    int connected = -1;

	
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1) {
		rt_kprintf("failed to create sock_fd!\n");
	}
	
    rBuf = static_malloc(PTU_ETH_RX_SIZE);

	rt_memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("192.168.1.4");
	server_addr.sin_port = htons(502);
	
    ModBusTcp_Int();

	while (1) 
    {
        switch(modbusCommSt)
        {
            case 0:
                if(rt_tick_get() > (1000 * 60))
                {
                    //Tcp 连接 组态屏的上电时间长，上电过程中
                    connected = connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
                }
                
                if(connected == 0)
                {
                    rt_kprintf(" ModBus Tcp connected!\r\n");
                    modbusCommSt = 1;
                }
                else
                {
                    rt_thread_delay(50);
                }
                break;
            case 1:
                //Tcp ModBus Communication
                sendBytes = ModBusTcp_Generate(ModBusSendBuffer, 1024);
                if(sendBytes > -1 )
                {
                    send(sock_fd, (char *)ModBusSendBuffer, sendBytes, 0);
                    //send(sock_fd, (char *)msg, 12, 0);
                    rt_thread_delay(6);
                    /* 从sock_rx中收取最大 buffer_size 字节数据 */
                    bytes_read = recvfrom(sock_fd, rBuf, PTU_ETH_RX_SIZE, 0,
                        (struct sockaddr *)&client_addr, (unsigned long*)&addr_len);
                    /* UDP不同于TCP，它基本不会出现收取的数据失败的情况，除非设置了超时等待  */

            #ifdef _debug_    
                    rBuf[bytes_read] = '\0'; /* 把末端清零 */

                    rt_kprintf("\r\n%02x  %02x  %02x  %02x  %02x  %02x  %02x  %02x  %02x  %02x  %02x",
                    rBuf[0],rBuf[1],rBuf[2],rBuf[3],rBuf[4],rBuf[5],rBuf[6],rBuf[7],rBuf[8],rBuf[9],
                    rBuf[10],rBuf[11]
                    );
                    /* 输出接收的数据 */
                    rt_kprintf("\n(%s , %d) said : ",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    //rt_kprintf("%s", recv_data);
                    rt_kprintf("%d Bytes  \r", bytes_read);

            #endif
                }
                rt_thread_delay(50);
                break;
            case 2:
                break;
            default:
                break;
        } 
	}
}





    
/*******************************************************************************
* Function Name  :
* Description    : [3/10/2017 Lily]
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void EthService_Init(void)
{
	/* register ethernetif device */
	eth_system_device_init();

	rt_hw_stm32_eth_init();

	/* init lwip system */
	lwip_sys_init();
	
	rt_kprintf("TCP/IP initialized!\n");

	//EthService_PDInit();
}
/*******************************************************************************
* Description    : 
*                   
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void  EthService_PDInit(void)
{
    int16_t Reply = 0;
	  
	 /*trdp-start*/
    //char DestAddress[] = "10.4.6.80";
    char DestAddress[] = "10.128.193.10";
    //char DestAddress[] = "10.3.1.10";
    uint32_t comID = 0;
    uint32_t interval = (uint32_t)(200 * 1000);
    pd_init (DestAddress, comID, interval);		
}



/*******************************************************************************
* Function Name  :
* Description    : [3/10/2017 Lily]
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static int EthService_SetNetAddr(uint32_t IpAddr, uint32_t GwAddr, uint32_t NmAddr)
{
    struct netif * netif;
    
    /*    
    stm32f4xx_eth.c里面初始化时，使用“e0”作为设备名称，
    这里就使用find API来找netif    
    eth_device_init(&(stm32_eth_device.parent), "e0");
    */
    netif = netif_find("e00");
	if (netif != NULL)
	{
		struct ip_addr ipaddr, netmask, gw;

		//IP4_ADDR(&ipaddr, RT_LWIP_IPADDR0, RT_LWIP_IPADDR1, RT_LWIP_IPADDR2, RT_LWIP_IPADDR3);
		//IP4_ADDR(&gw, RT_LWIP_GWADDR0, RT_LWIP_GWADDR1, RT_LWIP_GWADDR2, RT_LWIP_GWADDR3);
		//IP4_ADDR(&netmask, RT_LWIP_MSKADDR0, RT_LWIP_MSKADDR1, RT_LWIP_MSKADDR2, RT_LWIP_MSKADDR3);
		ipaddr.addr  = IpAddr;
		gw.addr      = GwAddr;
		netmask.addr = NmAddr;

		netifapi_netif_set_addr(netif, &ipaddr, &netmask, &gw);

		return 0;
	}

	return -1;
}


void netsetaddr(const char *nameip, const char *namemk, const char *namegw)
{
    uint32_t  ipaddr, netmask, gw;

    ipaddr  = inet_addr(nameip);
    gw      = inet_addr(namegw);
    netmask = inet_addr(namemk);

    EthService_SetNetAddr(ipaddr, gw, netmask);

}

#include "finsh.h"
FINSH_FUNCTION_EXPORT(netsetaddr, SET eth addr ip- mask- gw)




