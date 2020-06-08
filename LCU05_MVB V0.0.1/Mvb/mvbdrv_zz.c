/*****************************************************************************
*
* Copyright (c) 2005,深圳通业科技有限公司 .All rights reserved.
*
* -----------------------------------------------------------------------------
*
* 文件摘要:MVB-RVS驱动函数
*
* -----------------------------------------------------------------------------
*
* 文件名称:	Mvbdrv_zz.c
* 软件设计:  hcx
* 编译环境:	mdk32
* 版本控制:  kpc
*
*****************************************************************************/


#include "mvbdrv_zz.h"
#include <rtthread.h>
#include <rthw.h>


u8   ConfigMvbPort(u16 MvbPortAddr,u8 PortNum,u16 direction,u16 length);
u8	 ConfigMvbDevice(u16 MvbDeviceAddr,u16 InputLine);
u8	 gf_put_pd(u8 PortNum,u8 *PointerSend,u8 Len);
u8	 gf_get_pd(u8 PortNum,u8 *PointerReceive,u8 Len);
u8	 ClearUpUart(void);
u8	 StopMvb(void);
u8	 StarMvb(void);
u8	 DisableMvbPort(void);
u8	 MVB_check_THRE_ON(void);
void MVB_clear_THRE(void);
u8	 MVB_check_DR_ON(void);
void MVB_clear_DR(void);
u8	 ReceiveMvb(void);
void TransmiteMvb(u8 value);
u8	 CheckUartStauts(void);
void delay_ms(u16 times);
u8	 ClearMvb(void);


u8 Uart_RD(u32 reg)
{
    u8 *ptr;
    u8 val;
    ptr = (u8*)reg;
    val = *ptr;
    return(val);	
}


void Uart_WR(u32 reg, u8 val)
{
    u8 *ptr;	
    ptr = (u8*)reg;
    *ptr = val;
}


u8	ClearMvb(void)
{
    vu8	temp;

    if( CheckUartStauts() == RET_ERROR )							//检查寄存器状态
        return(RET_ERROR);

    temp = RET_OK;
    temp &= ClearUpUart();												//清除UART
    temp &= StopMvb();												//停止与mvb的通讯
    temp &= DisableMvbPort();											//使数据处理端口失效

    return(temp);
}


u8	gf_put_pd(u8 PortNum,u8 *PointerSend,u8 Len)
{
    u8	i;
    u8	result;

    result = MVB_check_THRE_ON();

    if(result == RET_OK)	
    {
        TransmiteMvb('P');									//put data to mvb process data port
        TransmiteMvb(PortNum);								//source handle number

        for(i=0; i<Len; i++)					
        {
            TransmiteMvb(*(PointerSend+i));	
        }
        
        for(i=0; i<MAXMVBBYTE-Len; i++)					
        {
            TransmiteMvb(0);	
        }
        
        MVB_clear_THRE();
		
        return(RET_OK);
    }

    return(RET_ERROR);
}


u8	gf_get_pd(u8 PortNum,u8 *PointerReceive,u8 Len)
{
    u8	i;
    u8	result ,BusStatus;

    result = MVB_check_THRE_ON();

    if(result == RET_OK)	
    {
        TransmiteMvb('G');									//get data form mvb process data port			
        TransmiteMvb(PortNum);								//sink handle number
        MVB_clear_THRE();
        result = MVB_check_DR_ON();

        if(result == RET_OK)
        {
            MVB_clear_DR();
            BusStatus = ReceiveMvb()&0xC0;							//mvb bus status value(A and B line)

            if(BusStatus != 0)
            {

                for(i = 0; i < Len; i++)
                {
                    *PointerReceive++ = ReceiveMvb();
                }

                return(BusStatus);
            }
        }
    }

    return (RET_ERROR);											//数据接收不允许返回
}

u8 MVB_check_THRE_ON(void)
{
    u16	i;
    u8	LSR;
    LSR = 0;
    for(i=0;i<MVB_THRE_CHECK_TIMES;i++)
    {
        LSR = Uart_RD(UART_LSR);
        if((LSR&UART_THRE_OK) == UART_THRE_OK)
            return(RET_OK);
    }
    return(RET_ERROR);
}

void MVB_clear_THRE(void)
{
    u8 LSR;
    LSR = Uart_RD(UART_LSR);
    LSR &= 0xdf;
    Uart_WR(UART_LSR, LSR);
}


u8 MVB_check_DR_ON(void)
{
    u16	i;
    u8	LSR;
    LSR = 0;
    for(i=0;i<MVB_DR_CHECK_TIMES;i++)
    {
        LSR = Uart_RD(UART_LSR);
        if((LSR&UART_DR_OK) == UART_DR_OK)
            return(RET_OK);
    }
    return(RET_ERROR);
}

void MVB_clear_DR(void)
{
    u8	LSR;
    LSR = Uart_RD(UART_LSR);
    LSR &= 0xfe;
    Uart_WR (UART_LSR, LSR);
}


u8	CheckUartStauts()
{
    u8	LSR; 
    LSR = Uart_RD(UART_LSR);      
    if ((LSR&0x9e) == 0x90)
    {
        return(RET_OK);
    } 
    else 
    { 
        return(RET_ERROR);
    }  
}

u8	ClearUpUart()
{
    u8	i;		
    for(i=0;i<RESET_BYTE_COUNTE;i++)						//send 64p 0x00 to thr
        TransmiteMvb(0x55);	
    return(RET_OK);
}

u8	StopMvb()
{
    u16 MVBDAddr=0;
    u8	result = 0;
    result = MVB_check_THRE_ON();
    if(result == RET_OK)
    {
        TransmiteMvb('S');									//status read command
        MVB_clear_THRE();
    }
    else
    {
        return(RET_ERROR);
    }
    result = MVB_check_DR_ON();
    if(result == RET_OK)
    {
        MVBDAddr = ReceiveMvb();				//mvb device address low
        MVBDAddr += ReceiveMvb()<<8;				//mvb device address high
        MVB_clear_DR();
    }
    else
    {
        return(RET_ERROR);
    }
    MVBDAddr &= 0x3FFF;							//don't change device address,switch off a and b lines
    result = MVB_check_THRE_ON();
    if(result == RET_OK)
    {
        TransmiteMvb('C');									//control write command
        TransmiteMvb(MVBDAddr);					//mvb device address low
        TransmiteMvb(MVBDAddr>>8);					//mvb device address high
        MVB_clear_THRE();
    }
    else
    {
        return(RET_ERROR);
    }

    return(RET_OK);
}

u8	StarMvb()
{
    u16 MVBDAddr=0;
    u8	result = 0;
    result = MVB_check_THRE_ON();
    if(result == RET_OK)
    {
        TransmiteMvb('S');									//status read command
        MVB_clear_THRE();
    }
    else
    {
        return(RET_ERROR);
    }
    result = MVB_check_DR_ON();
    if(result == RET_OK)
    {
        MVBDAddr = ReceiveMvb();				//mvb device address low
        MVBDAddr += ReceiveMvb()<<8;				//mvb device address high
        MVB_clear_DR();
    }
    else
    {
        return(RET_ERROR);
    }
    MVBDAddr |= 0xC000;							//don't change device address,switch on a and b lines
    result = MVB_check_THRE_ON();
    if(result == RET_OK)
    {
        TransmiteMvb('C');									//control write command
        TransmiteMvb(MVBDAddr);					//mvb device address low
        TransmiteMvb(MVBDAddr>>8);					//mvb device address high
        MVB_clear_THRE();
    }
    else
    {
        return(RET_ERROR);
    }
    return(RET_OK);
}

u8	DisableMvbPort()
{
    u8	i;
    u8	result = 0;
    for(i=0;i<MAXPORTNUM;i++)
    {
        result = MVB_check_THRE_ON();
        if(result == RET_OK)	
        {
            TransmiteMvb('H');								//handle definition write
            TransmiteMvb(i);								//handle number
            TransmiteMvb(0x00);								//length = 0,src = 0(sink),port logical address = 0
            TransmiteMvb(0x00);
            delay_ms(1);			
            MVB_clear_THRE();
        }		
        else
        {
            return(RET_ERROR);
        }
    }
    return(RET_OK);
}

u8   ConfigMvbPort(u16 MvbPortAddr,u8 PortNum,u16 direction,u16 length)
{
    u16 MVBPortCFG = 0;
    u8	result = 0;
    MVBPortCFG = (direction << 15) | (length << 12) | MvbPortAddr;//length ,sfc,port logical address
    result = MVB_check_THRE_ON();
    if(result == RET_OK)	
    {
        TransmiteMvb('H');									//handle definition	write
        TransmiteMvb(PortNum);								//handle number
        TransmiteMvb(MVBPortCFG);					
        TransmiteMvb(MVBPortCFG>>8);		
        MVB_clear_THRE();
    }
    else
    {
        return(RET_ERROR);
    }
    return(RET_OK);	
}


////////////////////////////////////////////////////////
//--------to check the config imformation wj
///////////////////////////////////////////////////////
u8   ConfigMvbcheck(u16 MvbPortAddr,u8 PortNum,u16 direction,u16 length)
{
    u16 MVBPortCFG = 0;
    u16 MVBPortCFGcheck = 0;
    u8	result = 0;
    
    
    MVBPortCFG = (direction << 15) | (length << 12) | MvbPortAddr;//length ,sfc,port logical address
    result = MVB_check_THRE_ON();
    
    if(result == 1)	
    {
        TransmiteMvb('I');									//handle definition	write
        TransmiteMvb(PortNum);								//handle number
        MVB_clear_DR();
        MVB_clear_THRE(); 
        
        if(MVB_check_DR_ON() == 1)
        {
            MVBPortCFGcheck = ReceiveMvb();
            MVBPortCFGcheck += ReceiveMvb()<<8;
            // MVB_clear_DR();

            if (MVBPortCFG == MVBPortCFGcheck)
            {
                return(1);
            }
            else
            {
                return(0);
            }
        }
        else
        {
            return(0);
        }


    }
    else
    {
        return(0);
    }
}

u8	ConfigMvbDevice(u16 MvbDeviceAddr,u16 InputLine)
{
    u16 MVBDAddr=0,temp=0;
    vu8	result = 0;

    MVBDAddr = (InputLine << 14) | 0x0000 | MvbDeviceAddr;//mvb device address,led,inputlines
    result = MVB_check_THRE_ON();
    if(result == RET_OK)	
    {
        TransmiteMvb('C');									//control write
        TransmiteMvb(MVBDAddr);
        TransmiteMvb(MVBDAddr>>8);		
        MVB_clear_THRE();
    }
    else
    {
        return(RET_ERROR);
    }

    result = MVB_check_THRE_ON();
    if(result == RET_OK)	
    {
        TransmiteMvb('S');									//status read 
        MVB_clear_THRE();
    }
    else
    {
        return(RET_ERROR);
    }
    result = MVB_check_DR_ON();
    if(result == RET_OK)
    {
        temp = ReceiveMvb();
        temp += ReceiveMvb()<<8;	
        MVB_clear_DR();
    }
    else
    {
        return(RET_ERROR);
    }
    if(temp == MVBDAddr)
    {
        return(RET_OK);	
    }
    else
    {
        return(RET_ERROR);
    }
}

void TransmiteMvb(u8 value)
{
    Uart_WR(UART_THR, value);					//write to thr
}

u8	ReceiveMvb()
{
    u8	temp;
    temp = Uart_RD(UART_RBR);					//read from rbr
    return(temp);
}

void delay_ms(u16 times)
{
    u32 i=12000;

    while(i--);
}

