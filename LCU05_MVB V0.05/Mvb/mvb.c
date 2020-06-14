

#include "mvb.h"
#include "mvbdrv_zz.h"
#include "userApp.h"


#define MVB_RST_PIN         (GPIO_Pin_7)
#define MVB_RST_PIN_PORT    (GPIOB)
#define MVB_RST_PIN_RCC     (RCC_AHB1Periph_GPIOB)

#define MC1_DEV_ADDR        (0x28)
#define MC2_DEV_ADDR        (0x08)

#define MVB_PORT_SINK       0
#define MVB_PORT_SOURCE     1

#define FCODE_2             0
#define FCODE_4             1
#define FCODE_8             2
#define FCODE_16            3
#define FCODE_32            4


typedef struct
{
	uint16_t addr;      //端口地址
	uint16_t index;     //端口索引
	uint16_t dir;       //端口方向：0为宿端口，1为源端口
	uint16_t fcode;     //端口数据长度
}MVB_PORT_INFO_STU, *PMVB_PORT_INFO_STU;

static const uint8_t MVB_BYTE_LEN[5] = {2, 4, 8, 16, 32};

static const MVB_PORT_INFO_STU mc1_ports[] =
{
    {0x001,     1,      MVB_PORT_SINK,      FCODE_2},       //生命信号
    {0x002,     2,      MVB_PORT_SINK,      FCODE_8},       //时间端口
    {0x281,     3,      MVB_PORT_SOURCE,    FCODE_32},      //Mc1车LCU诊断数据
    {0x481,     4,      MVB_PORT_SOURCE,    FCODE_32},      //Tp1车LCU诊断数据
    {0xE81,     5,      MVB_PORT_SOURCE,    FCODE_32},      //Tp2车LCU诊断数据
    {0x081,     6,      MVB_PORT_SOURCE,    FCODE_32},      //Mc2车LCU诊断数据
    {0x283,     7,      MVB_PORT_SOURCE,    FCODE_32},      //Mc1车LCU状态数据
    {0x483,     8,      MVB_PORT_SOURCE,    FCODE_32},      //Tp1车LCU状态数据
    {0xE83,     9,      MVB_PORT_SOURCE,    FCODE_32},      //Tp2车LCU状态数据
    {0x083,     10,     MVB_PORT_SOURCE,    FCODE_32},      //Mc2车LCU状态数据
};


static const MVB_PORT_INFO_STU mc2_ports[] =
{
    {0x001,     1,      MVB_PORT_SINK,      FCODE_2},       //生命信号
    {0x002,     2,      MVB_PORT_SINK,      FCODE_8},       //时间端口
    {0x282,     3,      MVB_PORT_SOURCE,    FCODE_32},      //Mc1车LCU诊断数据
    {0x482,     4,      MVB_PORT_SOURCE,    FCODE_32},      //Tp1车LCU诊断数据
    {0xE82,     5,      MVB_PORT_SOURCE,    FCODE_32},      //Tp2车LCU诊断数据
    {0x082,     6,      MVB_PORT_SOURCE,    FCODE_32},      //Mc2车LCU诊断数据
    {0x284,     7,      MVB_PORT_SOURCE,    FCODE_32},      //Mc1车LCU状态数据
    {0x484,     8,      MVB_PORT_SOURCE,    FCODE_32},      //Tp1车LCU状态数据
    {0xE84,     9,      MVB_PORT_SOURCE,    FCODE_32},      //Tp2车LCU状态数据
    {0x084,     10,     MVB_PORT_SOURCE,    FCODE_32},      //Mc2车LCU状态数据
};


uint8_t mvbPort_config(uint8_t carID)
{
    uint32_t  i;
    uint8_t  ret = 1;
    
	switch(carID)
	{
		case 0:
            ret = ConfigMvbDevice(MC1_DEV_ADDR, 3);
            for(i = 0; i < sizeof(mc1_ports)/sizeof(mc1_ports[0]); i++)
            {
                ret &= ConfigMvbPort(mc1_ports[i].addr, mc1_ports[i].index, mc1_ports[i].dir, mc1_ports[i].fcode);
            }
			break;
		case 3:
            ret = ConfigMvbDevice(MC2_DEV_ADDR, 3);
            for(i = 0; i < sizeof(mc2_ports)/sizeof(mc2_ports[0]); i++)
            {
                ret &= ConfigMvbPort(mc2_ports[i].addr, mc2_ports[i].index, mc2_ports[i].dir, mc2_ports[i].fcode);
            }
			break;
		default:
			break;
	}
    
    return ret;
}


void mvbPort_Read(uint8_t carID)
{
    uint32_t  i;

	switch(carID)
	{
		case 0:
            for(i = 0; i < sizeof(mc1_ports)/sizeof(mc1_ports[0]); i++)
            {
                if(mc1_ports[i].dir == MVB_PORT_SINK)
                {
                    if(gf_get_pd(mc1_ports[i].index, ds.mvb_port[i].data, MVB_BYTE_LEN[mc1_ports[i].fcode]) != 0)
                    {
                        
                    }
                }
            }
			break;
		case 3:
            for(i = 0; i < sizeof(mc2_ports)/sizeof(mc2_ports[0]); i++)
            {
                if(mc2_ports[i].dir == MVB_PORT_SINK)
                {
                    if(gf_get_pd(mc2_ports[i].index, ds.mvb_port[i].data, MVB_BYTE_LEN[mc2_ports[i].fcode]) != 0)
                    {
                        
                    }
                }
            }
			break;
		default:
			break;
	}
}


void mvbPort_write(uint8_t carID)
{
    uint32_t  i;

	switch(carID)
	{
		case 0:
            for(i = 0; i < sizeof(mc1_ports)/sizeof(mc1_ports[0]); i++)
            {
                if(mc1_ports[i].dir == MVB_PORT_SOURCE)
                {
                    gf_put_pd(mc1_ports[i].index, ds.mvb_port[i].data, MVB_BYTE_LEN[mc1_ports[i].fcode]);
                }
            }
			break;
		case 3:
            for(i = 0; i < sizeof(mc2_ports)/sizeof(mc2_ports[0]); i++)
            {
                if(mc2_ports[i].dir == MVB_PORT_SOURCE)
                {
                    gf_put_pd(mc2_ports[i].index, ds.mvb_port[i].data, MVB_BYTE_LEN[mc2_ports[i].fcode]);
                }
            }
			break;
		default:
			break;
	}
}


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


uint8_t MVB_Device_Init (void)
{
    uint8_t  ret = 0;
     
    MVB_Card_RST_GPIO_Init();
	rt_thread_delay(1);
    
    // MVB controler hardware reset.
    MVB_HW_Reset();
    
    // MVB controler clear internal data.
    ret = ClearMvb();

    return (ret);
}

