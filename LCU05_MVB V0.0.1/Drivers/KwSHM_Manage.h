

#ifndef __KW_SHM_MANAGE_H
#define __KW_SHM_MANAGE_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "stm32f4xx.h"
#include "rtthread.h"
    
#define KWSHM_KADATA_LEN  (32)


#define KWSHM_ACAR_INDEX  (0UL)
#define KWSHM_BCAR_INDEX  (1UL)
#define KWSHM_CCAR_INDEX  (2UL)
#define KWSHM_CAB_INDEX   (3UL)
#define IS_KWSHM_CAR_INDEX(INDEX) (((INDEX) == KWSHM_ACAR_INDEX) || \
                                    ((INDEX) == KWSHM_BCAR_INDEX) || \
                                    ((INDEX) == KWSHM_CCAR_INDEX) || \
                                    ((INDEX) == KWSHM_CAB_INDEX))


#define KWSHM_APPREG_TYPE_UINT8    (1)
#define KWSHM_APPREG_TYPE_UINT16   (2)
#define KWSHM_APPREG_TYPE_UINT32   (3)
#define IS_KWSHM_APPREG_TYPE(value) (((value) == KWSHM_APPREG_TYPE_UINT8) || \
                                     ((value) == KWSHM_APPREG_TYPE_UINT16) || \
                                     ((value) == KWSHM_APPREG_TYPE_UINT32))



#define KWSHM_APPREG_INDEX_CARNUM (0U)
#define KWSHM_APPREG_INDEX_AGROUP (1U)
#define KWSHM_APPREG_INDEX_BGROUP (2U)
#define KWSHM_APPREG_INDEX_AFAULT (3U)
#define KWSHM_APPREG_INDEX_BFAULT (4U)
#define KWSHM_APPREG_INDEX_MAX    (255U)

typedef struct
{
    uint8_t mvbDataBase[1024];
    uint8_t commonDataBase[2048];
    uint8_t reserved[640];
    uint8_t kaDataBase[128];
    uint8_t kwAppRegBase[256];
}KwSHM_Manage_TypeDef;


    typedef struct
    {
        uint8_t ACar_ka[32];
        uint8_t BCar_ka[32];
        uint8_t CCar_ka[32];
        uint8_t Cab_ka[32];
    }KwSHM_Ka_TypeDef;



    void KwSHM_AppRegWrite(uint32_t regIndex, uint32_t value, uint32_t dataType);
    uint32_t KwSHM_AppRegRead(uint32_t regIndex, uint32_t dataType);
    uint8_t *KwSHM_GetKaData_ByCarNum(uint8_t carNum);
    KwSHM_Manage_TypeDef *KwSHM_GetManageObject(void);
    void KwSHM_InitManage(void);

#ifdef __cplusplus
}
#endif

#endif

