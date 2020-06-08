

#include "stm32f4xx.h"
#include "rtthread.h"
#include "Bsp_finsh_queue.h"
#include "Bsp_can_queue.h"


#define MASK_PRO  (0x1E000000)
#define MASK_DST  (0x01FE0000)
#define MASK_SRC  (0x0001FE00)
#define MASK_FUN  (0x000001FF)
#define MASK_EN   (0x80000000)

#define CAN_FLT_ENABLE     (1)
#define CAN_FLT_DISABLE    (0)

#define CANFLT_TABLE_SIZE  (24)
#define CANFLT_CFG_INVALID (0)
#define CANFLT_CFG_FUN_EN  (1)
#define CANFLT_CFG_SRC_EN  (2)
#define CANFLT_CFG_DST_EN  (4)
#define CANFLT_CFG_PRO_EN  (8)

#define CFC_INPARAM_VAL_OFFSET  (1000)


typedef struct _cansoft_flt_
{
    uint32_t maskid;
    uint32_t filtval;
}CANSOFT_FLT, *PCANSOFT_FLT;

typedef struct _cansoft_flt_config_
{
    uint16_t flag;
    uint16_t priority;
    uint16_t destAdd;
    uint16_t sorceAdd;
    uint16_t funcId;
}CANSOFT_FLT_CFG, *PCANSOFT_FLT_CFG;




static CANSOFT_FLT csf[CANFLT_TABLE_SIZE]      = {0};
static CANSOFT_FLT_CFG csfc_cur[CANFLT_TABLE_SIZE] = {0};
static CANSOFT_FLT_CFG csfc_bak[CANFLT_TABLE_SIZE] = {0};
static CANSOFT_FLT_CFG csfc_buffer = {0};

static uint8_t can_filter_control = CAN_FLT_DISABLE;
static PCANSOFT_FLT_CFG operatorCurr = csfc_cur;
static PCANSOFT_FLT_CFG operatorback = csfc_bak;
static uint8_t opFltCfg_pointer = 0;

static uint8_t canPrt_filter_checker(uint32_t canExid);
void canPrt_filter(CAN_RX_DATA_RAM * pbuf);
static void canPrt_serv_adjustFiltCfg(void);
static void canPrt_serv_CfgToFlt(void);
static void canPrt_serv_CfgToFltUnit(uint8_t cnt);
static uint8_t cfg_table_add(uint8_t cnt,PCANSOFT_FLT_CFG pmsg);


/*******************************************************************************
* Function Name  : uint8_t canPrt_filter_checker(uint32_t canExid)
 
* Description    :����ͨ��������1����ͨ������0
*                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static uint8_t canPrt_filter_checker(uint32_t canExid)
{
    uint8_t i = 0;

	//���ݱ�����������˲��룬���ι��ˣ�
	//����ͨ��������1�����򷵻�0
    do 
    {
        if (   (csf[i].maskid & MASK_EN)
            && ((canExid & csf[i].maskid) == csf[i].filtval)
            )
        {
            return 1;
        }

    } while (i++ < (CANFLT_TABLE_SIZE - 1));

    return 0;
}

/*******************************************************************************
* Function Name  : void canPrt_filter(CAN_RX_DATA_RAM * pbuf)
 
* Description    :�˲�ģ��ʹ�����˲�ͨ�������ӡ��
*                 ���˲�ģ��δ��ʹ�ܣ���ֱ�Ӵ�ӡ��������
*                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void canPrt_filter(CAN_RX_DATA_RAM * pbuf)
{
    FINSH_FRAME_INFO canRxFFI;

    if (can_filter_control == CAN_FLT_ENABLE)
    {
		//�����ӡ�˲�ģ��ʹ�ܣ������˲�ͨ�������ӡ
        if (canPrt_filter_checker(pbuf->buf.ExtId))
        {
            canRxFFI.FrameTick = pbuf->parent.Tick;
            canRxFFI.BoardId   = pbuf->parent.BoardId;
            canRxFFI.Port      = pbuf->parent.CanPort;
            canRxFFI.DataSize  = pbuf->buf.DLC;

            FinshQ_Push_Data(CAN_RX,
                &canRxFFI, 
                (uint8_t *)&pbuf->buf);
        }
    }
    else
    {
		//�����ӡ�˲���ʹ�ܣ���ֱ�Ӵ�ӡ�����˲���
        canRxFFI.FrameTick = pbuf->parent.Tick;
        canRxFFI.BoardId   = pbuf->parent.BoardId;
        canRxFFI.Port      = pbuf->parent.CanPort;
        canRxFFI.DataSize  = pbuf->buf.DLC;

        FinshQ_Push_Data(CAN_RX,
            &canRxFFI, 
            (uint8_t *)&pbuf->buf);
    }
}

//-------------------------- Service API  -----------------------------

/*******************************************************************************
* Function Name  :  static void canPrt_serv_adjustFiltCfg(void)
* Description    : �ڱ�񸱱��У�����ԭ����е����ݣ����˳���Ч����
*                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void canPrt_serv_adjustFiltCfg(void)
{
    PCANSOFT_FLT_CFG pTemp;
    uint8_t i, j;

	i = 0;
	j = 0;

	//�ڸ������������ԭʼ�������ݣ����˳���Ч������
	do 
	{
		operatorback[i].flag      = CANFLT_CFG_INVALID;
		operatorback[i].priority  = 0;
		operatorback[i].sorceAdd  = 0;
		operatorback[i].destAdd   = 0;
		operatorback[i].funcId    = 0;


		if (operatorCurr[i].flag != CANFLT_CFG_INVALID)
		{
			operatorback[j].flag     = operatorCurr[i].flag;
			operatorback[j].priority = operatorCurr[i].priority;
			operatorback[j].sorceAdd = operatorCurr[i].sorceAdd;
			operatorback[j].destAdd  = operatorCurr[i].destAdd;
			operatorback[j].funcId   = operatorCurr[i].funcId;
			j++;
		}

		i++;
         
    } while (i < CANFLT_TABLE_SIZE);

    //������ǰ���븱����ָ��
    pTemp = operatorCurr;
    operatorCurr = operatorback;
    operatorback = pTemp;
    //���µ�ǰ��Ĳ���ָ��
    opFltCfg_pointer = j;
}

/*******************************************************************************
* Function Name  : static void canPrt_serv_CfgToFlt(void) 
* Description    :�����ñ��е�����ת��Ϊ�˲���
*                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void canPrt_serv_CfgToFlt(void)
{
    uint8_t i;

    i = 0;

    do 
    {
		canPrt_serv_CfgToFltUnit(i);		

        i++;

    } while (i < CANFLT_TABLE_SIZE);
}

/*******************************************************************************
* Function Name  : 
* Description    : �����ñ�����ת��Ϊ�˲��� ��Ԫ
*                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void canPrt_serv_CfgToFltUnit(uint8_t cnt)
{
	csf[cnt].maskid  = 0;
	csf[cnt].filtval = 0;

	if (operatorCurr[cnt].flag != CANFLT_CFG_INVALID)
	{
		if (operatorCurr[cnt].flag & CANFLT_CFG_FUN_EN)
		{
			csf[cnt].maskid |=  MASK_FUN;
			csf[cnt].filtval = operatorCurr[cnt].funcId;
		}

		if (operatorCurr[cnt].flag  & CANFLT_CFG_SRC_EN)
		{
			csf[cnt].maskid  |= MASK_SRC;
			csf[cnt].filtval |= (operatorCurr[cnt].sorceAdd) << 9;
		}

		if (operatorCurr[cnt].flag  & CANFLT_CFG_DST_EN)
		{
			csf[cnt].maskid  |= MASK_DST;
			csf[cnt].filtval |= (operatorCurr[cnt].destAdd) << (9 + 8);
		}

		if (operatorCurr[cnt].flag  & CANFLT_CFG_PRO_EN)
		{
			csf[cnt].maskid  |= MASK_PRO;
			csf[cnt].filtval |= (operatorCurr[cnt].priority) << (9 + 8 + 8);
		}

		csf[cnt].maskid |= MASK_EN;
	}
}

/*******************************************************************************
* Function Name  :  static uint8_t cfg_table_add(uint8_t cnt,PCANSOFT_FLT_CFG pmsg)
* Description    : �������������ݽ����жϴ�������Ч��ѹ�����ñ�����flag��־λ
*                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static uint8_t cfg_table_add(uint8_t cnt, PCANSOFT_FLT_CFG pmsg)
{
	if (pmsg->priority >= CFC_INPARAM_VAL_OFFSET)
	{
		operatorCurr[cnt].priority = (pmsg->priority - CFC_INPARAM_VAL_OFFSET);
		operatorCurr[cnt].flag    |= CANFLT_CFG_PRO_EN;
	}
	if (pmsg->destAdd >= CFC_INPARAM_VAL_OFFSET)
	{
		operatorCurr[cnt].destAdd  = (pmsg->destAdd - CFC_INPARAM_VAL_OFFSET);
		operatorCurr[cnt].flag    |= CANFLT_CFG_DST_EN;
	}
	if (pmsg->sorceAdd >= CFC_INPARAM_VAL_OFFSET)
	{
		operatorCurr[cnt].sorceAdd = (pmsg->sorceAdd - CFC_INPARAM_VAL_OFFSET);
		operatorCurr[cnt].flag    |= CANFLT_CFG_SRC_EN;
	}
	if (pmsg->funcId >= CFC_INPARAM_VAL_OFFSET)
	{
		operatorCurr[cnt].funcId   = (pmsg->funcId - CFC_INPARAM_VAL_OFFSET);
		operatorCurr[cnt].flag    |= CANFLT_CFG_FUN_EN;
	}

	return operatorCurr[cnt].flag;
}

//-------------------------- Add to finsh -----------------------------
/*******************************************************************************
* Function Name  :static void cfg_add(uint16_t Pri_Id,uint16_t Target_Id,
										uint16_t Sourc_Id,uint16_t Func_Id)
* Description    :����̨���� ����
*                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void cfg_add(uint16_t Pri_Id,uint16_t Target_Id,uint16_t Sourc_Id,uint16_t Func_Id)
{
	//������̨����Ĳ��������뻺����
	csfc_buffer.priority = Pri_Id;
	csfc_buffer.destAdd  = Target_Id;
	csfc_buffer.sorceAdd = Sourc_Id;
	csfc_buffer.funcId = Func_Id;

	//���������Ĳ�����Ч���򾭹�����������ñ���
	if (opFltCfg_pointer < 24)
	{
		if (cfg_table_add(opFltCfg_pointer, &csfc_buffer) != CANFLT_CFG_INVALID)
		{
			//�����ñ��е�������Ч����ѹ���˲�����
			canPrt_serv_CfgToFltUnit(opFltCfg_pointer);
			//����ά��
			opFltCfg_pointer++;
			can_filter_control = CAN_FLT_ENABLE;
			//��ӡ����ӳɹ���
			rt_kprintf("successfully added to the table !");
		}
		else
		{
			//��������������Ч�����ӡ�����ʧ�ܡ�
			rt_kprintf("Add failure !");
		}
	} 
	else
	{
		rt_kprintf("config table is full!");
	}
}


/*******************************************************************************
* Function Name  :static void cfg_delete(uint8_t cnt)
  
* Description    :
*                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void cfg_delete(uint8_t cnt)
{
	operatorCurr[cnt].flag = CANFLT_CFG_INVALID;
	operatorCurr[cnt].priority = 0;
	operatorCurr[cnt].destAdd  = 0;
	operatorCurr[cnt].sorceAdd = 0;
	operatorCurr[cnt].funcId   = 0;

	//�������ñ��е�����
	canPrt_serv_adjustFiltCfg();
	//�������ñ��е����ݸ����˲���
	canPrt_serv_CfgToFlt();

	//��ӡ��ɾ���ɹ���
	rt_kprintf("successfully delete a line of the table");
}

/*******************************************************************************
* Function Name  :static void cfg_modify(uint8_t cnt,uint16_t Pri_Id,
					uint16_t Target_Id,uint16_t Sourc_Id,uint16_t Func_Id)  
* Description    :
*                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void cfg_modify(uint8_t cnt,uint16_t Pri_Id,
					  uint16_t Target_Id,uint16_t Sourc_Id,uint16_t Func_Id)
{
	csfc_buffer.priority = Pri_Id;
	csfc_buffer.destAdd  = Target_Id;
	csfc_buffer.sorceAdd = Sourc_Id;
	csfc_buffer.funcId = Func_Id;

	if (cfg_table_add(cnt, &csfc_buffer) != CANFLT_CFG_INVALID)
	{
		//�����ñ��е����ݶ�Ӧת��ѹ����˱���
		canPrt_serv_CfgToFltUnit(cnt);
		//��ӡ ���޸ĳɹ���
		rt_kprintf("successfully modify a line of the table");
	} 
	else
	{
		//��ӡ���޸�ʧ�ܡ�
		rt_kprintf("modify failure");
	}
}
/*******************************************************************************
* Function Name  :void cfg_enable(void)  
* Description    :
*                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void cfg_enable(void)
{
	if (can_filter_control == CAN_FLT_ENABLE)
	{
		can_filter_control = CAN_FLT_DISABLE;
	} 
	else
	{
		can_filter_control = CAN_FLT_ENABLE;
	}
}
/*******************************************************************************
* Function Name  :  
* Description    :
*                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void cfg_prt(void)
{
	uint8_t i;

	rt_kprintf("\r\n\r\n");

	rt_kprintf("         CanNet PrintServer Filter Module ConfigTable \r\n\r\n");
	rt_kprintf("+-------+-------------+-------------+-------------+-------------+\r\n");
	rt_kprintf("| Index | Priorty     | TargetAdd   | SourceAdd   | FuncID      |\r\n");
	rt_kprintf("+-------+-------------+-------------+-------------+-------------+\r\n");

	i = 0;
	do 
	{
		rt_kprintf("| %03d   ", i);
		if(operatorCurr[i].flag & CANFLT_CFG_PRO_EN)
		{
			rt_kprintf("| %03d (+)     ", operatorCurr[i].priority);
		}
		else
		{
			rt_kprintf("| %03d ( )     ",operatorCurr[i].priority);
		}

		if(operatorCurr[i].flag & CANFLT_CFG_DST_EN)
		{
			rt_kprintf("| %03d (+)     ", operatorCurr[i].destAdd);
		}
		else
		{
			rt_kprintf("| %03d ( )     ",operatorCurr[i].destAdd);
		}

		if(operatorCurr[i].flag & CANFLT_CFG_SRC_EN)
		{
			rt_kprintf("| %03d (+)     ", operatorCurr[i].sorceAdd);
		}
		else
		{
			rt_kprintf("| %03d ( )     ",operatorCurr[i].sorceAdd);
		}

		if(operatorCurr[i].flag & CANFLT_CFG_FUN_EN)
		{
			rt_kprintf("| %03d (+)     |", operatorCurr[i].funcId);
		}
		else
		{
			rt_kprintf("| %03d ( )     |",operatorCurr[i].funcId);
		}

		rt_kprintf("\r\n");

		rt_kprintf("+-------+-------------+-------------+-------------+-------------+\r\n");

		i++;
	} while (i < CANFLT_TABLE_SIZE);

}


#include "finsh.h"
FINSH_FUNCTION_EXPORT(cfg_add, add can filter config message )
FINSH_FUNCTION_EXPORT(cfg_delete,delete a line of the can filter config table)
FINSH_FUNCTION_EXPORT(cfg_modify,modify a line of the table)
FINSH_FUNCTION_EXPORT(cfg_enable,enable or disable the table of can filter)
FINSH_FUNCTION_EXPORT(cfg_prt, print can filter config table)

