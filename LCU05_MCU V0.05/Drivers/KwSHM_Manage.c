
//  
//  +-------------+-------------+-----------------------+----------------------+
//  |                  KW Shared Memory Mapping Table V1.0                     |
//  +-------------+-------------+-----------------------+----------------------+
//  |   OffSet    | %MB3_Addr   |     Description       |        Comments      |
//  +-------------+-------------+-----------------------+----------------------+
//  |      0      |    1000     |                       |                      |
//  |      |      |      |      |      MVB Data         |                      |
//  |    1023     |    2023     |                       |                      |
//  +-------------+-------------+-----------------------+----------------------+
//  |    1024     |    2024     |                       |                      |
//  |      |      |      |      |      2K Reserved      |                      |
//  |    3071     |    4071     |                       |                      |
//  +-------------+-------------+-----------------------+----------------------+
//  |    3072     |    4072     |                       |                      |
//  |      |      |      |      |      640 Reserved     |                      |
//  |    3711     |    4711     |                       |                      |
//  +-------------+-------------+-----------------------+----------------------+
//  |    3712     |    4712     |                       |                      |
//  |      |      |      |      |      32 Bytes KA      |      A Car           |
//  |    3743     |    4743     |                       |                      |
//  +-------------+-------------+-----------------------+----------------------+
//  |    3744     |    4744     |                       |                      |
//  |      |      |      |      |      32 Bytes KA      |      B Car           |
//  |    3775     |    4775     |                       |                      |
//  +-------------+-------------+-----------------------+----------------------+
//  |    3776     |    4776     |                       |                      |
//  |      |      |      |      |      32 Bytes KA      |      C Car           |
//  |    3807     |    4807     |                       |                      |
//  +-------------+-------------+-----------------------+----------------------+
//  |    3808     |    4808     |                       |                      |
//  |      |      |      |      |      32 Bytes KA      |      Cabin           |
//  |    3839     |    4839     |                       |                      |
//  +-------------+-------------+-----------------------+----------------------+
//  |    3840     |    4840     |                       |                      |
//  |      |      |      |      |  256 Bytes Register   |      Common Var      |
//  |    4095     |    4095     |                       |                      |
//  +-------------+-------------+-----------------------+----------------------+
//  


#include "KwSHM_Manage.h"

static uint8_t _isInitialized = 0;
static KwSHM_Manage_TypeDef *pManageObject = RT_NULL;

//KW_SharedMemory 在eCLR模块中
extern volatile uint8_t shared_memory[4096];


static void KwSHM_CreateManage(void *sharedMem);


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void KwSHM_InitManage(void)
{
    KwSHM_CreateManage((void *)shared_memory);
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void KwSHM_CreateManage(void *sharedMem)
{
    if (sharedMem != RT_NULL)
    {
        pManageObject = (KwSHM_Manage_TypeDef *)sharedMem;
        _isInitialized = 1;
    }
}

/*******************************************************************************
* Function Name  :
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
KwSHM_Manage_TypeDef *KwSHM_GetManageObject(void)
{
    return pManageObject;
}



/*******************************************************************************
* Function Name  : uint8_t *KwSHM_GetKaData_ByCarNum(uint8_t carNum)
* Description    : 按照车节号返回KW逻辑运算用的共享中间变量。
* Input          : carNum: specifies the car index value.
*                   This parameter can be one of the following values:
*                     @arg KWSHM_ACAR_INDEX : Car A1 or A2
*                     @arg KWSHM_BCAR_INDEX : Car B1 or B2
*                     @arg KWSHM_CCAR_INDEX : Car C1 or C2
*                     @arg KWSHM_CAB_INDEX  : CabinA1 or CabinA2 (SZ9Line)
* Output         : None
* Return         : Ka var address base.
*******************************************************************************/
uint8_t *KwSHM_GetKaData_ByCarNum(uint8_t carNum)
{
    KwSHM_Ka_TypeDef *retCarKaData;
    uint8_t * ret;

    RT_ASSERT(IS_KWSHM_CAR_INDEX(carNum));

    retCarKaData = (KwSHM_Ka_TypeDef *)&(pManageObject->kaDataBase[0]);
    
    switch (carNum)
    {
    case KWSHM_ACAR_INDEX:
        ret = &(retCarKaData->ACar_ka[0]);
        break;
    case KWSHM_BCAR_INDEX:
        ret = &(retCarKaData->BCar_ka[0]);
        break;
    case KWSHM_CCAR_INDEX:
        ret = &(retCarKaData->CCar_ka[0]);
        break;
    case KWSHM_CAB_INDEX:
        ret = &(retCarKaData->Cab_ka[0]);
        break;
    default:
        ret = RT_NULL;
        break;
    }

    return ret;
}

/*******************************************************************************
* Function Name  :void KwSHM_AppRegWrite(uint32_t regIndex, 
*                                        uint32_t value,
*                                        uint32_t dataType)
* Description    :此API是针对Kw_sharedmemory 中规划用于KW_App的公共寄存器区域的写入
* Input          : @Param regIndex : specifies the reg index value.
*                   This parameter can be one of the following values:
*                     @arg 0 - 255
*
*                  @Param value : Reg value to be set.
*                   This parameter can be one of the following values:
*                     @arg Any value of uint8_t, uint16_t, uint32_t 
*
*                  @Param dataType : specifies the Reg DataType.
*                   This parameter can be one of the following values:
*                     @arg KWSHM_APPREG_TYPE_UINT8  : uint8_t
*                     @arg KWSHM_APPREG_TYPE_UINT16 : uint16_t
*                     @arg KWSHM_APPREG_TYPE_UINT32 : uint32_t

* Output         : None
* Return         : None
*******************************************************************************/
void KwSHM_AppRegWrite(uint32_t regIndex, uint32_t value, uint32_t dataType)
{

    RT_ASSERT(regIndex <= KWSHM_APPREG_INDEX_MAX);
    RT_ASSERT(IS_KWSHM_APPREG_TYPE(dataType));

    switch (dataType)
    {
    case KWSHM_APPREG_TYPE_UINT8:
        pManageObject->kwAppRegBase[regIndex] = (uint8_t)(value & 0xFF);
        break;
    case KWSHM_APPREG_TYPE_UINT16:
        *(uint16_t *)(&pManageObject->kwAppRegBase[regIndex]) = (uint16_t)(value & 0xFFFF);
        break;
    case KWSHM_APPREG_TYPE_UINT32:
        *(uint32_t *)(&pManageObject->kwAppRegBase[regIndex]) = value;
        break;
    default:
        break;
    }
}


/*******************************************************************************
* Function Name  :uint32_t KwSHM_AppRegRead(uint32_t regIndex, uint32_t dataType)
* Description    :此API是针对Kw_sharedmemory 中规划用于KW_App的公共寄存器区域的读取
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint32_t KwSHM_AppRegRead(uint32_t regIndex, uint32_t dataType)
{
    uint32_t retVal = 0;

    RT_ASSERT(regIndex <= KWSHM_APPREG_INDEX_MAX);
    RT_ASSERT(IS_KWSHM_APPREG_TYPE(dataType));

    switch (dataType)
    {
    case KWSHM_APPREG_TYPE_UINT8:
        retVal = pManageObject->kwAppRegBase[regIndex];
        break;
    case KWSHM_APPREG_TYPE_UINT16:
        retVal = *(uint16_t *)(&pManageObject->kwAppRegBase[regIndex]);
        break;
    case KWSHM_APPREG_TYPE_UINT32:
        retVal = *(uint32_t *)(&pManageObject->kwAppRegBase[regIndex]);
        break;
    default:
        break;
    }

    return retVal;
}





void kwprt(uint32_t index)
{
    rt_kprintf("\r\n kw mem 0x%x",*(uint32_t*)&shared_memory[index]);
}

void kwWrite(uint32_t index, uint8_t value)
{
    shared_memory[index] = value;
    rt_kprintf("\r\n kw mem %d : 0x%x", index, shared_memory[index]);
}

#include "finsh.h"
FINSH_FUNCTION_EXPORT(kwprt, for kw memory printf)
FINSH_FUNCTION_EXPORT(kwWrite, for kw memory write)

