#include "MVB_Cfg.h"
#include "userApp.h"
#include "mvbdrv_zz.h"
#include "Bsp_led.h"
#include "mvb_event.h"
#include "Bsp_canApp.h"
#include "crc16.h"

#define MVB_RST_PIN        (GPIO_Pin_7)
#define MVB_RST_PIN_PORT   (GPIOB)
#define MVB_RST_PIN_RCC    (RCC_AHB1Periph_GPIOB)


const MVB_PORT_INFO  TC1_MVB_SINK_PORT_CFG[MVB_PORT_NUM_MAX] = {
    {1176,  1,      MVB_PORT_TYPE_SINK,     16},
	{2,     2,      MVB_PORT_TYPE_SINK,     4},
	{3,     3,      MVB_PORT_TYPE_SINK,     8},
	{4,     4,      MVB_PORT_TYPE_SINK,     16},
	{5,     5,      MVB_PORT_TYPE_SINK,     32},
};


const MVB_PORT_INFO  TC1_MVB_SRCE_PORT_CFG[MVB_PORT_NUM_MAX] = {
	{1168,  6,      MVB_PORT_TYPE_SRCE,     32},
	{7,     7,      MVB_PORT_TYPE_SRCE,     4},
	{8,     8,      MVB_PORT_TYPE_SRCE,     8},
	{9,     9,      MVB_PORT_TYPE_SRCE,     16},
	{10,    10,     MVB_PORT_TYPE_SRCE,     32},
};


const MVB_PORT_INFO  TC2_MVB_SINK_PORT_CFG[MVB_PORT_NUM_MAX] = {
	{11,    11,     MVB_PORT_TYPE_SINK,     2},
	{12,    12,     MVB_PORT_TYPE_SINK,     4},
	{13,    13,     MVB_PORT_TYPE_SINK,     8},
	{14,    14,     MVB_PORT_TYPE_SINK,     16},
	{15,    15,     MVB_PORT_TYPE_SINK,     32},
};


const MVB_PORT_INFO  TC2_MVB_SRCE_PORT_CFG[MVB_PORT_NUM_MAX] = {
	{16,    16,     MVB_PORT_TYPE_SRCE,     2},
	{17,    17,     MVB_PORT_TYPE_SRCE,     4},
	{18,    18,     MVB_PORT_TYPE_SRCE,     8},
	{19,    19,     MVB_PORT_TYPE_SRCE,     16},
	{20,    20,     MVB_PORT_TYPE_SRCE,     32},
};


MVB_PORT_CFG Mvb_Port_Cfg = {0};

MVB_SEND_DATA_BUF  Mvb_Send_Data[MVB_PORT_ADDR_NUM] = {0};

MVB_SEND_DATA_BUF  Mvb_Recv_Data[MVB_PORT_ADDR_NUM] = {0};

uint8_t tmp_data[] = {0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x20,
					  0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x30,
					  0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x40,0x41,0x42};

static MVB_PORT_CFG TC1_MVB_Device = {0};
static MVB_PORT_CFG TC2_MVB_Device = {0};

static rt_uint32_t mvb_event_flag = 0;

static void Bsp_mvbService_Task(void* parameter);


/*************************************************************************
* Function Name  : MVB_Data_Init(void)
* Description    : 初始化MVB接收、发送缓冲区空间.
*                  [11/5/2019 zyh] 
* Input          : None
* Output         : None
* Return         : None
**************************************************************************/
static void MVB_Data_Init(void)
{
	uint8_t index = 0;
	for(index = 0;index < MVB_PORT_NUM_MAX;index++)
	{
		Mvb_Send_Data[index].data = (uint8_t*)rt_malloc(MVB_PORT_SIZE_MAX);
		Mvb_Recv_Data[index].data = (uint8_t*)rt_malloc(MVB_PORT_SIZE_MAX);
	}
	
	Mvb_Port_Cfg.mvb_device_addr = 0;
	Mvb_Port_Cfg.sinkPortCount = 0;
	Mvb_Port_Cfg.soucPortCount = 0;
	for(index = 0;index < MVB_PORT_NUM_MAX;index++)
	{
	
		Mvb_Port_Cfg.sink_port[index].MvbPortAddr = 0;
		Mvb_Port_Cfg.sink_port[index].portnum = 0;
		Mvb_Port_Cfg.sink_port[index].direction = MVB_PORT_TYPE_SINK;
		Mvb_Port_Cfg.sink_port[index].length = 0;
		
		Mvb_Port_Cfg.source_port[index].MvbPortAddr = 0;
		Mvb_Port_Cfg.source_port[index].portnum = 0;
		Mvb_Port_Cfg.source_port[index].direction = MVB_PORT_TYPE_SRCE;
		Mvb_Port_Cfg.source_port[index].length = 0;
		
	}
}

/*************************************************************************
* Function Name  : uint8_t MVB_PORT_INFO_CFG(MVB_PORT_CFG *PORT_INFO)
* Description    : 配置MVB通信地址和端口信息.
*                  [11/5/2019 zyh] 
* Input          : MVB配置信息端口结构体
* Output         : None
* Return         : 配置信息是否成功，0不成功，非0配置成功
**************************************************************************/

uint8_t MVB_PORT_INFO_CFG(MVB_PORT_CFG *PORT_INFO)
{
	uint8_t portindex = 0;
	uint8_t fcode = 0;
	uint8_t  retWert = 1;
	
	
	retWert = ConfigMvbDevice(PORT_INFO->mvb_device_addr,1);
	for(portindex = 0;portindex < PORT_INFO->soucPortCount;portindex++)
	{
	
		if(PORT_INFO->source_port[portindex].length ==       2) fcode = 0;
        else if(PORT_INFO->source_port[portindex].length ==  4) fcode = 1;
        else if(PORT_INFO->source_port[portindex].length ==  8) fcode = 2;
        else if(PORT_INFO->source_port[portindex].length == 16) fcode = 3;
        else if(PORT_INFO->source_port[portindex].length == 32) fcode = 4;
        else retWert = RET_ERROR;
			

		retWert &= ConfigMvbPort(PORT_INFO->source_port[portindex].MvbPortAddr,PORT_INFO->source_port[portindex].portnum,
		              PORT_INFO->source_port[portindex].direction - 1,fcode);

	}
	
	for(portindex = 0;portindex < PORT_INFO->soucPortCount;portindex++)
	{
	
		if(PORT_INFO->sink_port[portindex].length ==       2) fcode = 0;
        else if(PORT_INFO->sink_port[portindex].length ==  4) fcode = 1;
        else if(PORT_INFO->sink_port[portindex].length ==  8) fcode = 2;
        else if(PORT_INFO->sink_port[portindex].length == 16) fcode = 3;
        else if(PORT_INFO->sink_port[portindex].length == 32) fcode = 4;
        else retWert = RET_ERROR;
			

		retWert &= ConfigMvbPort(PORT_INFO->sink_port[portindex].MvbPortAddr,PORT_INFO->sink_port[portindex].portnum,
		              PORT_INFO->sink_port[portindex].direction - 1,fcode);

	}
	
//	retCode &= ConfigMvbPort(0x490,1,1,4);
//	retCode &= ConfigMvbPort(0x498,2,0,3);
	return retWert;
}

/*
目标端口是接收端口 0
源端口是发送端口   1
*/
/*************************************************************************
* Function Name  : uint8_t Mvb_Set_Port_Info(void)
* Description    : MVB通信信息结构体初始化，并进行配置.
*                  [11/5/2019 zyh] 
* Input          : None
* Output         : None
* Return         : None
**************************************************************************/

void Mvb_Set_Port_Info(void)
{
	uint8_t retWert = 1;
	switch(mvb_board.boxID)
	{
		case CAR_A1:
			Mvb_Port_Cfg.mvb_device_addr = 0x28;
			Mvb_Port_Cfg.sinkPortCount = 1;
			Mvb_Port_Cfg.soucPortCount = 1;
			rt_memcpy(Mvb_Port_Cfg.sink_port,TC1_MVB_SINK_PORT_CFG,sizeof(TC1_MVB_SINK_PORT_CFG));
			rt_memcpy(Mvb_Port_Cfg.source_port,TC1_MVB_SRCE_PORT_CFG,sizeof(TC1_MVB_SRCE_PORT_CFG));
			break;
		case CAR_B1:
			
			break;
		case CAR_C1:
			
			break;
		case CAR_C2:
			
			break;
		case CAR_B2:
			
			break;
		case CAR_A2:
			Mvb_Port_Cfg.mvb_device_addr = 0x08;
			Mvb_Port_Cfg.sinkPortCount = 5;
			Mvb_Port_Cfg.soucPortCount = 5;
			rt_memcpy(Mvb_Port_Cfg.sink_port,TC2_MVB_SINK_PORT_CFG,sizeof(TC2_MVB_SINK_PORT_CFG));
			rt_memcpy(Mvb_Port_Cfg.source_port,TC2_MVB_SRCE_PORT_CFG,sizeof(TC2_MVB_SRCE_PORT_CFG));
			break;
		default:
			
			break;
	}
	
//	retWert = MVB_PORT_INFO_CFG(&TC1_MVB_Device);
	
	return;
}

/*************************************************************************
* Function Name  : static void MVB_Device_Write(MVB_PORT_CFG *PORT_INFO)
* Description    : 将设备需要发送的数据写入FPGA对应区域.
*                  [11/5/2019 zyh] 
* Input          : MVB配置信息端口结构体
* Output         : None
* Return         : None
**************************************************************************/
static void MVB_Device_Write(MVB_PORT_CFG *PORT_INFO)
{
	uint8_t index   = 0;
    uint8_t portNum = 0;
	uint8_t portindex = 0;
	for(portindex = 0;portindex < PORT_INFO->soucPortCount;portindex++)
	{
		portNum = PORT_INFO->source_port[portindex].portnum;
		if(Mvb_Send_Data[portNum].fresh_data)
		{
			Mvb_Send_Data[portNum].fresh_data = 0;
            
            gf_put_pd(portNum, Mvb_Send_Data[portNum].data, PORT_INFO->source_port[portindex].length);
//			gf_put_pd(portindex + 1, Mvb_Send_Data[portNum].data, DEVICE_PORT_CFG[index].portinfo[portindex].length);
		}		
		
	}
}

static uint8_t mdbuf[32] = {0};
static uint8_t fresh = 0;
static uint8_t write_fresh = 1;

/*************************************************************************
* Function Name  : static void MVB_Device_Read_Data(MVB_PORT_CFG *PORT_INFO)
* Description    : 接收FPGA从MVB抓取到的数据.
*                  [11/5/2019 zyh] 
* Input          : MVB配置信息端口结构体
* Output         : None
* Return         : None
**************************************************************************/

static uint8_t mvb_rx_prt = 0;

static void MVB_Device_Read_Data(MVB_PORT_CFG *PORT_INFO)
{
	uint8_t portindex = 0;
    uint8_t portNum = 0;
    uint8_t errCnt  = 0;
    uint8_t isFresh = 0;
	
	for(portindex = 0;portindex < PORT_INFO->sinkPortCount;portindex++)
	{
		portNum = PORT_INFO->sink_port[portindex].portnum;
		
		if(gf_get_pd(portNum, mdbuf, PORT_INFO->sink_port[portindex].length) != 0)
		{
			rt_memcpy(Mvb_Recv_Data[portNum].data, mdbuf, PORT_INFO->sink_port[portindex].length);
			Mvb_Recv_Data[portNum].fresh_data = 1;
		}
        
        
		if(mvb_rx_prt == 1)
		{
			rt_kprintf("+-b0 -+-b1 -+-b2 -+-b3 -+-b4 -+-b5 -+-b6 -+-b7 -+\r\n");
			rt_kprintf("| %3d | %3d | %3d | %3d | %3d | %3d | %3d | %3d |\r\n", 
					   mdbuf[0], mdbuf[1], 
					   mdbuf[2], mdbuf[3], 
					   mdbuf[4], mdbuf[5], 
					   mdbuf[6], mdbuf[7]);
			rt_kprintf("+-b8 -+-b9 -+-b10-+-b11-+-b12-+-b13-+-b14-+-b15-+\r\n");
			rt_kprintf("| %3d | %3d | %3d | %3d | %3d | %3d | %3d | %3d |\r\n", 
					   mdbuf[8], mdbuf[9], 
					   mdbuf[10], mdbuf[11], 
					   mdbuf[12], mdbuf[13], 
					   mdbuf[14], mdbuf[15]);
			rt_kprintf("+-b16-+-b17-+-b18-+-b19-+-b20-+-b21-+-b22-+-b23-+\r\n");
			rt_kprintf("| %3d | %3d | %3d | %3d | %3d | %3d | %3d | %3d |\r\n", 
					   mdbuf[16], mdbuf[17], 
					   mdbuf[18], mdbuf[19], 
					   mdbuf[20], mdbuf[21], 
					   mdbuf[22], mdbuf[23]);
			rt_kprintf("+-b24-+-b25-+-b26-+-b27-+-b28-+-b29-+-b30-+-b31-+\r\n");
			rt_kprintf("| %3d | %3d | %3d | %3d | %3d | %3d | %3d | %3d |\r\n", 
					   mdbuf[24], mdbuf[25], 
					   mdbuf[26], mdbuf[27], 
					   mdbuf[28], mdbuf[29], 
					   mdbuf[30], mdbuf[31]);
		}
		
		
	}

}


/*************************************************************************
* Function Name  : void MVB_Card_RST_GPIO_Init(void)
* Description    : MVB复位引脚初始化.
*                  [11/10/2014 lily] 
* Input          : None
* Output         : None
* Return         : None
**************************************************************************/
static void MVB_Card_RST_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};

    //-----------------------------------------------------------------------
	// Enable GPIOx clocks. 
    //-----------------------------------------------------------------------
	RCC_AHB1PeriphClockCmd(MVB_RST_PIN_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
    //-----------------------------------------------------------------------
	//  Configure reset pin. 
    //-----------------------------------------------------------------------
	GPIO_InitStructure.GPIO_Pin   = MVB_RST_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_Init(MVB_RST_PIN_PORT, &GPIO_InitStructure);
	 
	
	
}

/*************************************************************************
* Function Name  : void MVB_HW_Reset(void)
* Description    : Mvb 硬件复位操作.
*                  [11/10/2014 lily] 
* Input          : None
* Output         : None
* Return         : None
**************************************************************************/
static void MVB_HW_Reset(void)
{
    //-----------------------------------------------------------------------
    // Set reset pin to logic high.
    // Delay 25ms , ensure that hardware into reset status.
    //-----------------------------------------------------------------------
	GPIO_SetBits(MVB_RST_PIN_PORT, MVB_RST_PIN);  
	rt_thread_delay(25);

    //-----------------------------------------------------------------------
    // Set reset pin to logic low.
    // Delay 80ms , ensure that hardware reset complete.
    //-----------------------------------------------------------------------
	GPIO_ResetBits(MVB_RST_PIN_PORT, MVB_RST_PIN);  
	rt_thread_delay(80);
}

/*************************************************************************
* Function Name  : int MVB_Device_Init (void) 
* Description    : Mvb controler default init.
*                  [11/10/2014 lily] 
* Input          : None
* Output         : None
* Return         : None
**************************************************************************/
static int MVB_Device_Init (void)
{
    uint8_t  retWert = 0;

    //----------------------------------------------------------------------
    // MVB controler hardware reset.
    //----------------------------------------------------------------------
    MVB_HW_Reset();
    //----------------------------------------------------------------------
    // MVB controler clear internal data.
    //----------------------------------------------------------------------
    retWert = ClearMvb();

    return (retWert);
}


static void MVB_CAN_Data(MVB_PORT_CFG *PORT_INFO)
{
	uint8_t portindex = 0;
	uint8_t portnum = 0;
	uint16_t crc_code = 0;
	uint8_t data[8] = {0};
	uint8_t port_len = 0;
	//待优化
	for(portindex = 0;portindex < PORT_INFO->sinkPortCount;portindex++)
	{
		
		if(PORT_INFO->sink_port[portindex].length ==       2) port_len = 0;
        else if(PORT_INFO->sink_port[portindex].length ==  4) port_len = 1;
        else if(PORT_INFO->sink_port[portindex].length ==  8) port_len = 2;
        else if(PORT_INFO->sink_port[portindex].length == 16) port_len = 3;
        else if(PORT_INFO->sink_port[portindex].length == 32) port_len = 4;
        else port_len = 255;
		
		portnum = PORT_INFO->sink_port[portindex].portnum;
		rt_memset(data,0,8);
		switch(port_len)
		{
			case MVB_RECV_2BYTE:
				
				if(Mvb_Recv_Data[portnum].fresh_data == 1)
				{
					crc_code = getCRC16(Mvb_Recv_Data[portnum].data,PORT_INFO->sink_port[portindex].length);
					
					data[0] = 0xff;
					rt_memcpy(&data[1],Mvb_Recv_Data[portnum].data,PORT_INFO->sink_port[portindex].length);
					data[PORT_INFO->sink_port[portindex].length + 1] = (crc_code >> 8);
					data[PORT_INFO->sink_port[portindex].length + 2] = crc_code;
					MVB_Date(data);
					
					Mvb_Recv_Data[portnum].fresh_data = 0;
				}
				
				break;
			case MVB_RECV_4BYTE:
				
				if(Mvb_Recv_Data[portnum].fresh_data == 1)
				{
					crc_code = getCRC16(Mvb_Recv_Data[portnum].data,PORT_INFO->sink_port[portindex].length);
					
					data[0] = 0xff;
					rt_memcpy(&data[1],Mvb_Recv_Data[portnum].data,PORT_INFO->sink_port[portindex].length);
					data[PORT_INFO->sink_port[portindex].length + 1] = (crc_code >> 8);
					data[PORT_INFO->sink_port[portindex].length + 2] = crc_code;
					MVB_Date(data);
					
					Mvb_Recv_Data[portnum].fresh_data = 0;
				}
				
				break;
			case MVB_RECV_8BYTE:
				if(Mvb_Recv_Data[portnum].fresh_data == 1)
				{
					crc_code = getCRC16(Mvb_Recv_Data[portnum].data,PORT_INFO->sink_port[portindex].length);
					
					data[0] = 0x01;                                     //数据包编号
					rt_memcpy(&data[1],Mvb_Recv_Data[portnum].data,7);
					MVB_Date(data);
					
					data[0] = 0xFF;                                     //数据包编号，FF为最后一包数据
					data[1] = Mvb_Recv_Data[portnum].data[7];
					data[((PORT_INFO->sink_port[portindex].length) % 7) + 1] = (crc_code >> 8);
					data[((PORT_INFO->sink_port[portindex].length) % 7) + 2] = crc_code;
					MVB_Date(data);
					
					Mvb_Recv_Data[portnum].fresh_data = 0;
				}
				break;
			case MVB_RECV_16BYTE:
				if(Mvb_Recv_Data[portnum].fresh_data == 1)
				{
					crc_code = getCRC16(Mvb_Recv_Data[portnum].data,PORT_INFO->sink_port[portindex].length);
					
					data[0] = 0x01;                                     //数据包编号
					rt_memcpy(&data[1],Mvb_Recv_Data[portnum].data,7);
					MVB_Date(data);
					
					data[0] = 0x02;
					rt_memcpy(&data[1],&Mvb_Recv_Data[portnum].data[7],7);
					MVB_Date(data);
					
					data[0] = 0xFF;                                     //数据包编号，FF为最后一包数据
					data[1] = Mvb_Recv_Data[portnum].data[14];
					data[2] = Mvb_Recv_Data[portnum].data[15];
					data[((PORT_INFO->sink_port[portindex].length) % 7) + 1] = (crc_code >> 8);
					data[((PORT_INFO->sink_port[portindex].length) % 7) + 2] = crc_code;
					MVB_Date(data);
					
					Mvb_Recv_Data[portnum].fresh_data = 0;
				}
				break;
			case MVB_RECV_32BYTE:
				if(Mvb_Recv_Data[portnum].fresh_data == 1)
				{
					crc_code = getCRC16(Mvb_Recv_Data[portnum].data,PORT_INFO->sink_port[portindex].length);
					
					data[0] = 0x01;                                     //数据包编号
					rt_memcpy(&data[1],Mvb_Recv_Data[portnum].data,7);
					MVB_Date(data);
					
					data[0] = 0x02;
					rt_memcpy(&data[1],&Mvb_Recv_Data[portnum].data[7],7);
					MVB_Date(data);
					
					data[0] = 0x03;
					rt_memcpy(&data[1],&Mvb_Recv_Data[portnum].data[14],7);
					MVB_Date(data);
					
					data[0] = 0x04;
					rt_memcpy(&data[1],&Mvb_Recv_Data[portnum].data[21],7);
					MVB_Date(data);
					
					data[0] = 0xFF;                                     //数据包编号，FF为最后一包数据
					data[1] = Mvb_Recv_Data[portnum].data[28];
					data[2] = Mvb_Recv_Data[portnum].data[29];
					data[3] = Mvb_Recv_Data[portnum].data[30];
					data[4] = Mvb_Recv_Data[portnum].data[31];
					data[((PORT_INFO->sink_port[portindex].length) % 7) + 1] = (crc_code >> 8);
					data[((PORT_INFO->sink_port[portindex].length) % 7) + 2] = crc_code;
					MVB_Date(data);
					
					Mvb_Recv_Data[portnum].fresh_data = 0;
				}
				break;
			default:
				
				break;
		}
		
		
		
	}
}

void  Bsp_MvbTask_CreateApp(void)
{
	rt_thread_t bsp_mvbService_thread;

	MVB_Card_RST_GPIO_Init();
	
	MVB_Data_Init();
	MVB_Event_Init();
    bsp_mvbService_thread = rt_thread_create("MvbSrv",
                                             Bsp_mvbService_Task,
                                             RT_NULL,
                                             //sizeof(bsp_mvbService_stack),
											 BSP_MVB_SERVICE_STACKSIZE,
                                             BSP_MVB_SERVICE_PRIORITY, 
                                             20);
    if (bsp_mvbService_thread != RT_NULL)
        rt_thread_startup(bsp_mvbService_thread);
    
}


static void Bsp_mvbService_Task(void* parameter)
{
    //-----------------------------------------------------------------------
    // Mvb service task event object init
    //-----------------------------------------------------------------------
	static uint8_t flag = 1;
	static uint8_t retWert = 1;
	static uint8_t mvb_status = 0;
	
    for (;;)
    {
		switch(mvb_status)
		{
			case MVB_POWER_ON:
				
                retWert &= MVB_Device_Init();
                Mvb_Set_Port_Info();
                retWert &= MVB_PORT_INFO_CFG(&Mvb_Port_Cfg);
                if(retWert == RET_ERROR)
                {
                    System_Led_SetMode(LED_FLT, MODE_FLASH_SLOW);
                    mvb_status = MVB_POWER_ON;
                }
                mvb_status = MVB_NOMAL;
				break;
			case MVB_NOMAL:
				mvb_event_flag = MVB_Check_Event();
				if(mvb_event_flag & MVB_EVENT_WRITE)
				{
					MVB_Device_Write(&Mvb_Port_Cfg);
				}
				
				MVB_Device_Read_Data(&Mvb_Port_Cfg);
				MVB_CAN_Data(&Mvb_Port_Cfg);
				
				break;
			
			case MVB_ERROR:
				
				mvb_status = MVB_POWER_ON;
				break;
			default:
				
				break;
		}

    }
}


void Mvb_rx_prt(void)
{
	mvb_rx_prt = 1;
}


void Stop_Mvb_rx_prt(void)
{
	mvb_rx_prt = 0;
}


#include "finsh.h"
FINSH_FUNCTION_EXPORT(Mvb_rx_prt,print mvb receive data)
FINSH_FUNCTION_EXPORT(Stop_Mvb_rx_prt,stop print mvb receive data)

