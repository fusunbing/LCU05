
/***************************************************************************

Copyright (C), 1999-2012, Tongye Tech. Co., Ltd.

* @file           ds.h
* @author          
* @version        V1.0.0
* @date           2012-04-22
* @brief          ϵͳ���ݽӿڲ�

History:          // Revision Records

       <Author>             <time>       <version >            <desc>
                               
        grape             2012-04-22       V1.0.0             ���ν���
												 
***************************************************************************/
#ifndef _DS_H_
#define _DS_H_

//���س���BSP��汾�� V1.01
#define BSP_VERSION                (1L)
#define BSP_SUBVERSION             (0L)
#define BSP_REVISION               (1L)

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
#define FUNC_RET_OK                (0)
#define FUNC_RET_ERR               (-1)

//<o> DS.H  CANģ�����ڷ��������趨
//    ��10msΪ��λ�������趨1������Ϊ10ms
//                      �趨5������Ϊ50ms
#define    CAN_CYC_TIME            (1)



//<o> DS.H  KTģ��ѡ��
//    <0x00=> ϵͳ������KTģ��
//    <0x01=> ϵͳ  ����KTģ��
#define    SYSTEM_USE_KT           (1)

//<o> DS.H  CNT(software counter)ģ��ѡ��
//    <0x00=> ϵͳ������CNTģ��
//    <0x01=> ϵͳ  ����CNTģ��
#define    SYSTEM_USE_CNT          (1)

//------------------------------------------------------------------------------
//  ʱ��̵�������ӳ��ӿ�
//------------------------------------------------------------------------------
#define  KT_ED_MODE        ((u8)0x00)  //�õ���ʱģʽ
#define  KT_DD_MODE        ((u8)0x01)  //ʧ����ʱģʽ

#define  IS_KT_SUPPORT_MODE(MODE)    (((MODE) == KT_ED_MODE) || \
	((MODE) == KT_DD_MODE))

#define  KT_DISABLE        ((u8)0x0)
#define  KT_ENABLE         ((u8)(!KT_DISABLE))

#define  IS_KT_STATE(STATE)          (((STATE) == KT_DISABLE) || ((STATE) == KT_ENABLE))

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
#define RUN_T         (50)
/* - COUTER_TIMES(sec, milsec)����Ҫ����ϵͳԼ����ʱ������������.             */
#define COUTER_TIMES(sec, milsec)     (1000U * (sec) / RUN_T  + ((milsec) / RUN_T))
#define T(sec, milsec)                (COUTER_TIMES((sec), (milsec)))

#if(SYSTEM_USE_KT == 1)

typedef struct _KT_InitTypeDef_
{
	u16  FTimeInit;  //�״��ϵ�����ϵ��ӳ�
	u16  ktIndex;    //ʱ��ͨ��
	u16	 mode;		 //ʱ��ģʽ
	u16  enable;	 //ʹ�ܸ�ʱ��
	u32  timeOut;	 //ʱ�̶�ʱʱ��

}KT_InitTypeDef, *PKT_InitTypeDef;

typedef struct _DS_KT_INPUT_
{
	/* Bit01 */	u16	KT01_Input	: 1 ;  //ʱ��	1	  ������
	/* Bit02 */	u16	KT02_Input	: 1 ;  //ʱ��	2	  ������
	/* Bit03 */	u16	KT03_Input	: 1 ;  //ʱ��	3	  ������
	/* Bit04 */	u16	KT04_Input	: 1 ;  //ʱ��	4	  ������
	/* Bit05 */	u16	KT05_Input	: 1 ;  //ʱ��	5	  ������
	/* Bit06 */	u16	KT06_Input	: 1 ;  //ʱ��	6	  ������
	/* Bit07 */	u16	KT07_Input	: 1 ;  //ʱ��	7	  ������
	/* Bit08 */	u16	KT08_Input	: 1 ;  //ʱ��	8	  ������
	/* Bit09 */	u16	KT09_Input	: 1 ;  //ʱ��	9	  ������
	/* Bit10 */	u16	KT10_Input	: 1 ;  //ʱ��	10	  ������
	/* Bit11 */	u16	KT11_Input	: 1 ;  //ʱ��	11	  ������
	/* Bit12 */	u16	KT12_Input	: 1 ;  //ʱ��	12	  ������
	/* Bit13 */	u16	KT13_Input	: 1 ;  //ʱ��	13	  ������
	/* Bit14 */	u16	KT14_Input	: 1 ;  //ʱ��	14	  ������
	/* Bit15 */	u16	KT15_Input	: 1 ;  //ʱ��	15	  ������
	/* Bit16 */	u16	KT16_Input	: 1 ;  //ʱ��	16	  ������

	/* Bit01 */	u16	KT17_Input	: 1 ;  //ʱ��	17	  ������
	/* Bit02 */	u16	KT18_Input	: 1 ;  //ʱ��	18	  ������
	/* Bit03 */	u16	KT19_Input	: 1 ;  //ʱ��	19	  ������
	/* Bit04 */	u16	KT20_Input	: 1 ;  //ʱ��	20	  ������
	/* Bit05 */	u16	KT21_Input	: 1 ;  //ʱ��	21	  ������
	/* Bit06 */	u16	KT22_Input	: 1 ;  //ʱ��	22	  ������
	/* Bit07 */	u16	KT23_Input	: 1 ;  //ʱ��	23	  ������
	/* Bit08 */	u16	KT24_Input	: 1 ;  //ʱ��	24	  ������
	/* Bit09 */	u16	KT25_Input	: 1 ;  //ʱ��	25	  ������
	/* Bit10 */	u16	KT26_Input	: 1 ;  //ʱ��	26	  ������
	/* Bit11 */	u16	KT27_Input	: 1 ;  //ʱ��	27	  ������
	/* Bit12 */	u16	KT28_Input	: 1 ;  //ʱ��	28	  ������
	/* Bit13 */	u16	KT29_Input	: 1 ;  //ʱ��	29	  ������
	/* Bit14 */	u16	KT30_Input	: 1 ;  //ʱ��	30	  ������
	/* Bit15 */	u16	KT31_Input	: 1 ;  //ʱ��	31	  ������
	/* Bit16 */	u16	KT32_Input	: 1 ;  //ʱ��	32	  ������

	/* Bit01 */	u16	KT33_Input	: 1 ;  //ʱ��	33	  ������
	/* Bit02 */	u16	KT34_Input	: 1 ;  //ʱ��	34	  ������
	/* Bit03 */	u16	KT35_Input	: 1 ;  //ʱ��	35	  ������
	/* Bit04 */	u16	KT36_Input	: 1 ;  //ʱ��	36	  ������
	/* Bit05 */	u16	KT37_Input	: 1 ;  //ʱ��	37	  ������
	/* Bit06 */	u16	KT38_Input	: 1 ;  //ʱ��	38	  ������
	/* Bit07 */	u16	KT39_Input	: 1 ;  //ʱ��	39	  ������
	/* Bit08 */	u16	KT40_Input	: 1 ;  //ʱ��	40	  ������
	/* Bit09 */	u16	KT41_Input	: 1 ;  //ʱ��	41	  ������
	/* Bit10 */	u16	KT42_Input	: 1 ;  //ʱ��	42	  ������
	/* Bit11 */	u16	KT43_Input	: 1 ;  //ʱ��	43	  ������
	/* Bit12 */	u16	KT44_Input	: 1 ;  //ʱ��	44	  ������
	/* Bit13 */	u16	KT45_Input	: 1 ;  //ʱ��	45	  ������
	/* Bit14 */	u16	KT46_Input	: 1 ;  //ʱ��	46	  ������
	/* Bit15 */	u16	KT47_Input	: 1 ;  //ʱ��	47	  ������
	/* Bit16 */	u16	KT48_Input	: 1 ;  //ʱ��	48	  ������

	/* Bit01 */	u16	KT49_Input	: 1 ;  //ʱ��	49	  ������
	/* Bit02 */	u16	KT50_Input	: 1 ;  //ʱ��	50	  ������
	/* Bit03 */	u16	KT51_Input	: 1 ;  //ʱ��	51	  ������
	/* Bit04 */	u16	KT52_Input	: 1 ;  //ʱ��	52	  ������
	/* Bit05 */	u16	KT53_Input	: 1 ;  //ʱ��	53	  ������
	/* Bit06 */	u16	KT54_Input	: 1 ;  //ʱ��	54	  ������
	/* Bit07 */	u16	KT55_Input	: 1 ;  //ʱ��	55	  ������
	/* Bit08 */	u16	KT56_Input	: 1 ;  //ʱ��	56	  ������
	/* Bit09 */	u16	KT57_Input	: 1 ;  //ʱ��	57	  ������
	/* Bit10 */	u16	KT58_Input	: 1 ;  //ʱ��	58	  ������
	/* Bit11 */	u16	KT59_Input	: 1 ;  //ʱ��	59	  ������
	/* Bit12 */	u16	KT60_Input	: 1 ;  //ʱ��	60	  ������
	/* Bit13 */	u16	KT61_Input	: 1 ;  //ʱ��	61	  ������
	/* Bit14 */	u16	KT62_Input	: 1 ;  //ʱ��	62	  ������
	/* Bit15 */	u16	KT63_Input	: 1 ;  //ʱ��	63	  ������
	/* Bit16 */	u16	KT64_Input	: 1 ;  //ʱ��	64	  ������

	/* Bit01 */	u16	KT65_Input	: 1 ;  //ʱ��	65	  ������
	/* Bit02 */	u16	KT66_Input	: 1 ;  //ʱ��	66	  ������
	/* Bit03 */	u16	KT67_Input	: 1 ;  //ʱ��	67	  ������
	/* Bit04 */	u16	KT68_Input	: 1 ;  //ʱ��	68	  ������
	/* Bit05 */	u16	KT69_Input	: 1 ;  //ʱ��	69	  ������
	/* Bit06 */	u16	KT70_Input	: 1 ;  //ʱ��	70	  ������
	/* Bit07 */	u16	KT71_Input	: 1 ;  //ʱ��	71	  ������
	/* Bit08 */	u16	KT72_Input	: 1 ;  //ʱ��	72	  ������
	/* Bit09 */	u16	KT73_Input	: 1 ;  //ʱ��	73	  ������
	/* Bit10 */	u16	KT74_Input	: 1 ;  //ʱ��	74	  ������
	/* Bit11 */	u16	KT75_Input	: 1 ;  //ʱ��	75	  ������
	/* Bit12 */	u16	KT76_Input	: 1 ;  //ʱ��	76	  ������
	/* Bit13 */	u16	KT77_Input	: 1 ;  //ʱ��	77	  ������
	/* Bit14 */	u16	KT78_Input	: 1 ;  //ʱ��	78	  ������
	/* Bit15 */	u16	KT79_Input	: 1 ;  //ʱ��	79	  ������
	/* Bit16 */	u16	KT80_Input	: 1 ;  //ʱ��	80	  ������

	/* Bit01 */	u16	KT81_Input	: 1 ;  //ʱ��	81	  ������
	/* Bit02 */	u16	KT82_Input	: 1 ;  //ʱ��	82	  ������
	/* Bit03 */	u16	KT83_Input	: 1 ;  //ʱ��	83	  ������
	/* Bit04 */	u16	KT84_Input	: 1 ;  //ʱ��	84	  ������
	/* Bit05 */	u16	KT85_Input	: 1 ;  //ʱ��	85	  ������
	/* Bit06 */	u16	KT86_Input	: 1 ;  //ʱ��	86	  ������
	/* Bit07 */	u16	KT87_Input	: 1 ;  //ʱ��	87	  ������
	/* Bit08 */	u16	KT88_Input	: 1 ;  //ʱ��	88	  ������
	/* Bit09 */	u16	KT89_Input	: 1 ;  //ʱ��	89	  ������
	/* Bit10 */	u16	KT90_Input	: 1 ;  //ʱ��	90	  ������
	/* Bit11 */	u16	KT91_Input	: 1 ;  //ʱ��	91	  ������
	/* Bit12 */	u16	KT92_Input	: 1 ;  //ʱ��	92	  ������
	/* Bit13 */	u16	KT93_Input	: 1 ;  //ʱ��	93	  ������
	/* Bit14 */	u16	KT94_Input	: 1 ;  //ʱ��	94	  ������
	/* Bit15 */	u16	KT95_Input	: 1 ;  //ʱ��	95	  ������
	/* Bit16 */	u16	KT96_Input	: 1 ;  //ʱ��	96	  ������

}DS_KT_INPUT, *PDS_KT_INPUT;


typedef struct _DS_KT_OUTPUT_
{
	/* Bit01 */	u16	KT01_Output	: 1 ;  //ʱ��	1	  �����
	/* Bit02 */	u16	KT02_Output	: 1 ;  //ʱ��	2	  �����
	/* Bit03 */	u16	KT03_Output	: 1 ;  //ʱ��	3	  �����
	/* Bit04 */	u16	KT04_Output	: 1 ;  //ʱ��	4	  �����
	/* Bit05 */	u16	KT05_Output	: 1 ;  //ʱ��	5	  �����
	/* Bit06 */	u16	KT06_Output	: 1 ;  //ʱ��	6	  �����
	/* Bit07 */	u16	KT07_Output	: 1 ;  //ʱ��	7	  �����
	/* Bit08 */	u16	KT08_Output	: 1 ;  //ʱ��	8	  �����
	/* Bit09 */	u16	KT09_Output	: 1 ;  //ʱ��	9	  �����
	/* Bit10 */	u16	KT10_Output	: 1 ;  //ʱ��	10	  �����
	/* Bit11 */	u16	KT11_Output	: 1 ;  //ʱ��	11	  �����
	/* Bit12 */	u16	KT12_Output	: 1 ;  //ʱ��	12	  �����
	/* Bit13 */	u16	KT13_Output	: 1 ;  //ʱ��	13	  �����
	/* Bit14 */	u16	KT14_Output	: 1 ;  //ʱ��	14	  �����
	/* Bit15 */	u16	KT15_Output	: 1 ;  //ʱ��	15	  �����
	/* Bit16 */	u16	KT16_Output	: 1 ;  //ʱ��	16	  �����
	/* Bit01 */	u16	KT17_Output	: 1 ;  //ʱ��	17	  �����
	/* Bit02 */	u16	KT18_Output	: 1 ;  //ʱ��	18	  �����
	/* Bit03 */	u16	KT19_Output	: 1 ;  //ʱ��	19	  �����
	/* Bit04 */	u16	KT20_Output	: 1 ;  //ʱ��	20	  �����
	/* Bit05 */	u16	KT21_Output	: 1 ;  //ʱ��	21	  �����
	/* Bit06 */	u16	KT22_Output	: 1 ;  //ʱ��	22	  �����
	/* Bit07 */	u16	KT23_Output	: 1 ;  //ʱ��	23	  �����
	/* Bit08 */	u16	KT24_Output	: 1 ;  //ʱ��	24	  �����
	/* Bit09 */	u16	KT25_Output	: 1 ;  //ʱ��	25	  �����
	/* Bit10 */	u16	KT26_Output	: 1 ;  //ʱ��	26	  �����
	/* Bit11 */	u16	KT27_Output	: 1 ;  //ʱ��	27	  �����
	/* Bit12 */	u16	KT28_Output	: 1 ;  //ʱ��	28	  �����
	/* Bit13 */	u16	KT29_Output	: 1 ;  //ʱ��	29	  �����
	/* Bit14 */	u16	KT30_Output	: 1 ;  //ʱ��	30	  �����
	/* Bit15 */	u16	KT31_Output	: 1 ;  //ʱ��	31	  �����
	/* Bit16 */	u16	KT32_Output	: 1 ;  //ʱ��	32	  �����
	/* Bit01 */	u16	KT33_Output	: 1 ;  //ʱ��	33	  �����
	/* Bit02 */	u16	KT34_Output	: 1 ;  //ʱ��	34	  �����
	/* Bit03 */	u16	KT35_Output	: 1 ;  //ʱ��	35	  �����
	/* Bit04 */	u16	KT36_Output	: 1 ;  //ʱ��	36	  �����
	/* Bit05 */	u16	KT37_Output	: 1 ;  //ʱ��	37	  �����
	/* Bit06 */	u16	KT38_Output	: 1 ;  //ʱ��	38	  �����
	/* Bit07 */	u16	KT39_Output	: 1 ;  //ʱ��	39	  �����
	/* Bit08 */	u16	KT40_Output	: 1 ;  //ʱ��	40	  �����
	/* Bit09 */	u16	KT41_Output	: 1 ;  //ʱ��	41	  �����
	/* Bit10 */	u16	KT42_Output	: 1 ;  //ʱ��	42	  �����
	/* Bit11 */	u16	KT43_Output	: 1 ;  //ʱ��	43	  �����
	/* Bit12 */	u16	KT44_Output	: 1 ;  //ʱ��	44	  �����
	/* Bit13 */	u16	KT45_Output	: 1 ;  //ʱ��	45	  �����
	/* Bit14 */	u16	KT46_Output	: 1 ;  //ʱ��	46	  �����
	/* Bit15 */	u16	KT47_Output	: 1 ;  //ʱ��	47	  �����
	/* Bit16 */	u16	KT48_Output	: 1 ;  //ʱ��	48	  �����
	/* Bit01 */	u16	KT49_Output	: 1 ;  //ʱ��	49	  �����
	/* Bit02 */	u16	KT50_Output	: 1 ;  //ʱ��	50	  �����
	/* Bit03 */	u16	KT51_Output	: 1 ;  //ʱ��	51	  �����
	/* Bit04 */	u16	KT52_Output	: 1 ;  //ʱ��	52	  �����
	/* Bit05 */	u16	KT53_Output	: 1 ;  //ʱ��	53	  �����
	/* Bit06 */	u16	KT54_Output	: 1 ;  //ʱ��	54	  �����
	/* Bit07 */	u16	KT55_Output	: 1 ;  //ʱ��	55	  �����
	/* Bit08 */	u16	KT56_Output	: 1 ;  //ʱ��	56	  �����
	/* Bit09 */	u16	KT57_Output	: 1 ;  //ʱ��	57	  �����
	/* Bit10 */	u16	KT58_Output	: 1 ;  //ʱ��	58	  �����
	/* Bit11 */	u16	KT59_Output	: 1 ;  //ʱ��	59	  �����
	/* Bit12 */	u16	KT60_Output	: 1 ;  //ʱ��	60	  �����
	/* Bit13 */	u16	KT61_Output	: 1 ;  //ʱ��	61	  �����
	/* Bit14 */	u16	KT62_Output	: 1 ;  //ʱ��	62	  �����
	/* Bit15 */	u16	KT63_Output	: 1 ;  //ʱ��	63	  �����
	/* Bit16 */	u16	KT64_Output	: 1 ;  //ʱ��	64	  �����
	/* Bit01 */	u16	KT65_Output	: 1 ;  //ʱ��	65	  �����
	/* Bit02 */	u16	KT66_Output	: 1 ;  //ʱ��	66	  �����
	/* Bit03 */	u16	KT67_Output	: 1 ;  //ʱ��	67	  �����
	/* Bit04 */	u16	KT68_Output	: 1 ;  //ʱ��	68	  �����
	/* Bit05 */	u16	KT69_Output	: 1 ;  //ʱ��	69	  �����
	/* Bit06 */	u16	KT70_Output	: 1 ;  //ʱ��	70	  �����
	/* Bit07 */	u16	KT71_Output	: 1 ;  //ʱ��	71	  �����
	/* Bit08 */	u16	KT72_Output	: 1 ;  //ʱ��	72	  �����
	/* Bit09 */	u16	KT73_Output	: 1 ;  //ʱ��	73	  �����
	/* Bit10 */	u16	KT74_Output	: 1 ;  //ʱ��	74	  �����
	/* Bit11 */	u16	KT75_Output	: 1 ;  //ʱ��	75	  �����
	/* Bit12 */	u16	KT76_Output	: 1 ;  //ʱ��	76	  �����
	/* Bit13 */	u16	KT77_Output	: 1 ;  //ʱ��	77	  �����
	/* Bit14 */	u16	KT78_Output	: 1 ;  //ʱ��	78	  �����
	/* Bit15 */	u16	KT79_Output	: 1 ;  //ʱ��	79	  �����
	/* Bit16 */	u16	KT80_Output	: 1 ;  //ʱ��	80	  �����
	/* Bit01 */	u16	KT81_Output	: 1 ;  //ʱ��	81	  �����
	/* Bit02 */	u16	KT82_Output	: 1 ;  //ʱ��	82	  �����
	/* Bit03 */	u16	KT83_Output	: 1 ;  //ʱ��	83	  �����
	/* Bit04 */	u16	KT84_Output	: 1 ;  //ʱ��	84	  �����
	/* Bit05 */	u16	KT85_Output	: 1 ;  //ʱ��	85	  �����
	/* Bit06 */	u16	KT86_Output	: 1 ;  //ʱ��	86	  �����
	/* Bit07 */	u16	KT87_Output	: 1 ;  //ʱ��	87	  �����
	/* Bit08 */	u16	KT88_Output	: 1 ;  //ʱ��	88	  �����
	/* Bit09 */	u16	KT89_Output	: 1 ;  //ʱ��	89	  �����
	/* Bit10 */	u16	KT90_Output	: 1 ;  //ʱ��	90	  �����
	/* Bit11 */	u16	KT91_Output	: 1 ;  //ʱ��	91	  �����
	/* Bit12 */	u16	KT92_Output	: 1 ;  //ʱ��	92	  �����
	/* Bit13 */	u16	KT93_Output	: 1 ;  //ʱ��	93	  �����
	/* Bit14 */	u16	KT94_Output	: 1 ;  //ʱ��	94	  �����
	/* Bit15 */	u16	KT95_Output	: 1 ;  //ʱ��	95	  �����
	/* Bit16 */	u16	KT96_Output	: 1 ;  //ʱ��	96	  �����

}DS_KT_OUTPUT, *PKT_OUTPUT;


/* ʱ��̵������� -----------------------------------------*/
#define KT01_Index	((u16)0x0)
#define KT02_Index	((u16)0x1)
#define KT03_Index	((u16)0x2)
#define KT04_Index	((u16)0x3)
#define KT05_Index	((u16)0x4)
#define KT06_Index	((u16)0x5)
#define KT07_Index	((u16)0x6)
#define KT08_Index	((u16)0x7)

#define KT09_Index	((u16)0x8)
#define KT10_Index	((u16)0x9)
#define KT11_Index	((u16)0xa)
#define KT12_Index	((u16)0xb)
#define KT13_Index	((u16)0xc)
#define KT14_Index	((u16)0xd)
#define KT15_Index	((u16)0xe)
#define KT16_Index	((u16)0xf)

#define KT17_Index	((u16)0x10)
#define KT18_Index	((u16)0x11)
#define KT19_Index	((u16)0x12)
#define KT20_Index	((u16)0x13)
#define KT21_Index	((u16)0x14)
#define KT22_Index	((u16)0x15)
#define KT23_Index	((u16)0x16)
#define KT24_Index	((u16)0x17)

#define KT25_Index	((u16)0x18)
#define KT26_Index	((u16)0x19)
#define KT27_Index	((u16)0x1a)
#define KT28_Index	((u16)0x1b)
#define KT29_Index	((u16)0x1c)
#define KT30_Index	((u16)0x1d)
#define KT31_Index	((u16)0x1e)
#define KT32_Index	((u16)0x1f)

#define	KT33_Index	((u16)0x20)
#define	KT34_Index	((u16)0x21)
#define	KT35_Index	((u16)0x22)
#define	KT36_Index	((u16)0x23)
#define	KT37_Index	((u16)0x24)
#define	KT38_Index	((u16)0x25)
#define	KT39_Index	((u16)0x26)
#define	KT40_Index	((u16)0x27)

#define	KT41_Index	((u16)0x28)
#define	KT42_Index	((u16)0x29)
#define	KT43_Index	((u16)0x2a)
#define	KT44_Index	((u16)0x2b)
#define	KT45_Index	((u16)0x2c)
#define	KT46_Index	((u16)0x2d)
#define	KT47_Index	((u16)0x2e)
#define	KT48_Index	((u16)0x2f)

#define	KT49_Index	((u16)0x30)
#define	KT50_Index	((u16)0x31)
#define	KT51_Index	((u16)0x32)
#define	KT52_Index	((u16)0x33)
#define	KT53_Index	((u16)0x34)
#define	KT54_Index	((u16)0x35)
#define	KT55_Index	((u16)0x36)
#define	KT56_Index	((u16)0x37)

#define	KT57_Index	((u16)0x38)
#define	KT58_Index	((u16)0x39)
#define	KT59_Index	((u16)0x3a)
#define	KT60_Index	((u16)0x3b)
#define	KT61_Index	((u16)0x3c)
#define	KT62_Index	((u16)0x3d)
#define	KT63_Index	((u16)0x3e)
#define	KT64_Index	((u16)0x3f)

#define	KT65_Index	((u16)0x40)
#define	KT66_Index	((u16)0x41)
#define	KT67_Index	((u16)0x42)
#define	KT68_Index	((u16)0x43)
#define	KT69_Index	((u16)0x44)
#define	KT70_Index	((u16)0x45)
#define	KT71_Index	((u16)0x46)
#define	KT72_Index	((u16)0x47)

#define	KT73_Index	((u16)0x48)
#define	KT74_Index	((u16)0x49)
#define	KT75_Index	((u16)0x4a)
#define	KT76_Index	((u16)0x4b)
#define	KT77_Index	((u16)0x4c)
#define	KT78_Index	((u16)0x4d)
#define	KT79_Index	((u16)0x4e)
#define	KT80_Index	((u16)0x4f)

#define	KT81_Index	((u16)0x50)
#define	KT82_Index	((u16)0x51)
#define	KT83_Index	((u16)0x52)
#define	KT84_Index	((u16)0x53)
#define	KT85_Index	((u16)0x54)
#define	KT86_Index	((u16)0x55)
#define	KT87_Index	((u16)0x56)
#define	KT88_Index	((u16)0x57)

#define	KT89_Index	((u16)0x58)
#define	KT90_Index	((u16)0x59)
#define	KT91_Index	((u16)0x5a)
#define	KT92_Index	((u16)0x5b)
#define	KT93_Index	((u16)0x5c)
#define	KT94_Index	((u16)0x5d)
#define	KT95_Index	((u16)0x5e)
#define	KT96_Index	((u16)0x5f)

#define IS_ACTIVE_KT(INDEX)  (((INDEX) == KT01_Index) || \
	                          ((INDEX) == KT02_Index) || \
							  ((INDEX) == KT03_Index) || \
							  ((INDEX) == KT04_Index) || \
							  ((INDEX) == KT05_Index) || \
							  ((INDEX) == KT06_Index) || \
							  ((INDEX) == KT07_Index) || \
							  ((INDEX) == KT08_Index) || \
							  ((INDEX) == KT09_Index) || \
							  ((INDEX) == KT10_Index) || \
							  ((INDEX) == KT11_Index) || \
							  ((INDEX) == KT12_Index) || \
							  ((INDEX) == KT13_Index) || \
							  ((INDEX) == KT14_Index) || \
							  ((INDEX) == KT15_Index) || \
							  ((INDEX) == KT16_Index) || \
							  ((INDEX) == KT17_Index) || \
							  ((INDEX) == KT18_Index) || \
							  ((INDEX) == KT19_Index) || \
							  ((INDEX) == KT20_Index) || \
							  ((INDEX) == KT21_Index) || \
							  ((INDEX) == KT22_Index) || \
							  ((INDEX) == KT23_Index) || \
							  ((INDEX) == KT24_Index) || \
							  ((INDEX) == KT25_Index) || \
							  ((INDEX) == KT26_Index) || \
							  ((INDEX) == KT27_Index) || \
							  ((INDEX) == KT28_Index) || \
							  ((INDEX) == KT29_Index) || \
							  ((INDEX) == KT30_Index) || \
							  ((INDEX) == KT32_Index) || \
							  ((INDEX) == KT33_Index) || \
							  ((INDEX) == KT34_Index) || \
							  ((INDEX) == KT35_Index) || \
							  ((INDEX) == KT36_Index) || \
							  ((INDEX) == KT37_Index) || \
							  ((INDEX) == KT38_Index) || \
							  ((INDEX) == KT39_Index) || \
							  ((INDEX) == KT40_Index) || \
							  ((INDEX) == KT41_Index) || \
							  ((INDEX) == KT42_Index) || \
							  ((INDEX) == KT43_Index) || \
							  ((INDEX) == KT44_Index) || \
							  ((INDEX) == KT45_Index) || \
							  ((INDEX) == KT46_Index) || \
							  ((INDEX) == KT47_Index) || \
							  ((INDEX) == KT48_Index) || \
							  ((INDEX) == KT49_Index) || \
							  ((INDEX) == KT50_Index) || \
							  ((INDEX) == KT51_Index) || \
							  ((INDEX) == KT52_Index) || \
							  ((INDEX) == KT53_Index) || \
							  ((INDEX) == KT54_Index) || \
							  ((INDEX) == KT55_Index) || \
							  ((INDEX) == KT56_Index) || \
							  ((INDEX) == KT57_Index) || \
							  ((INDEX) == KT58_Index) || \
							  ((INDEX) == KT59_Index) || \
							  ((INDEX) == KT60_Index) || \
							  ((INDEX) == KT61_Index) || \
							  ((INDEX) == KT62_Index) || \
							  ((INDEX) == KT63_Index) || \
							  ((INDEX) == KT64_Index) || \
							  ((INDEX) == KT65_Index) || \
							  ((INDEX) == KT66_Index) || \
							  ((INDEX) == KT67_Index) || \
							  ((INDEX) == KT68_Index) || \
							  ((INDEX) == KT69_Index) || \
							  ((INDEX) == KT70_Index) || \
							  ((INDEX) == KT71_Index) || \
							  ((INDEX) == KT72_Index) || \
							  ((INDEX) == KT73_Index) || \
							  ((INDEX) == KT74_Index) || \
							  ((INDEX) == KT75_Index) || \
							  ((INDEX) == KT76_Index) || \
							  ((INDEX) == KT77_Index) || \
							  ((INDEX) == KT78_Index) || \
							  ((INDEX) == KT79_Index) || \
							  ((INDEX) == KT80_Index) || \
							  ((INDEX) == KT81_Index) || \
							  ((INDEX) == KT82_Index) || \
							  ((INDEX) == KT83_Index) || \
							  ((INDEX) == KT84_Index) || \
							  ((INDEX) == KT85_Index) || \
							  ((INDEX) == KT86_Index) || \
							  ((INDEX) == KT87_Index) || \
							  ((INDEX) == KT88_Index) || \
							  ((INDEX) == KT89_Index) || \
							  ((INDEX) == KT90_Index) || \
							  ((INDEX) == KT91_Index) || \
							  ((INDEX) == KT92_Index) || \
							  ((INDEX) == KT93_Index) || \
							  ((INDEX) == KT94_Index) || \
							  ((INDEX) == KT95_Index) || \
							  ((INDEX) == KT96_Index))

#endif

#if(SYSTEM_USE_CNT == 1)
//------------------------------------------------------------------------------
// SoftTimer
//------------------------------------------------------------------------------
typedef struct _STIM_InitTypeDef_
{
	u16  stimIndex;    //��ʱ��ͨ��
	u16  enable;	   //ʹ�ܸ���ʱ��
}STIM_InitTypeDef, *PSTIM_InitTypeDef;

#define    CNT01_Index    ((u16)0)
#define    CNT02_Index    ((u16)1)
#define    CNT03_Index    ((u16)2)
#define    CNT04_Index    ((u16)3)
#define    CNT05_Index    ((u16)4)
#define    CNT06_Index    ((u16)5)
#define    CNT07_Index    ((u16)6)
#define    CNT08_Index    ((u16)7)
#define    CNT09_Index    ((u16)8)
#define    CNT10_Index    ((u16)9)
#define    CNT11_Index    ((u16)10)
#define    CNT12_Index    ((u16)11)
#define    CNT13_Index    ((u16)12)
#define    CNT14_Index    ((u16)13)
#define    CNT15_Index    ((u16)14)
#define    CNT16_Index    ((u16)15)
#define    CNT17_Index    ((u16)16)
#define    CNT18_Index    ((u16)17)
#define    CNT19_Index    ((u16)18)
#define    CNT20_Index    ((u16)19)
#define    CNT21_Index    ((u16)20)
#define    CNT22_Index    ((u16)21)
#define    CNT23_Index    ((u16)22)
#define    CNT24_Index    ((u16)23)
#define    CNT25_Index    ((u16)24)
#define    CNT26_Index    ((u16)25)
#define    CNT27_Index    ((u16)26)
#define    CNT28_Index    ((u16)27)
#define    CNT29_Index    ((u16)28)
#define    CNT30_Index    ((u16)29)
#define    CNT31_Index    ((u16)30)
#define    CNT32_Index    ((u16)31)
#define    CNT33_Index    ((u16)32)
#define    CNT34_Index    ((u16)33)
#define    CNT35_Index    ((u16)34)
#define    CNT36_Index    ((u16)35)
#define    CNT37_Index    ((u16)36)
#define    CNT38_Index    ((u16)37)
#define    CNT39_Index    ((u16)38)
#define    CNT40_Index    ((u16)39)
#define    CNT41_Index    ((u16)40)
#define    CNT42_Index    ((u16)41)
#define    CNT43_Index    ((u16)42)
#define    CNT44_Index    ((u16)43)
#define    CNT45_Index    ((u16)44)
#define    CNT46_Index    ((u16)45)
#define    CNT47_Index    ((u16)46)
#define    CNT48_Index    ((u16)47)
#define    CNT49_Index    ((u16)48)
#define    CNT50_Index    ((u16)49)
#define    CNT51_Index    ((u16)50)
#define    CNT52_Index    ((u16)51)
#define    CNT53_Index    ((u16)52)
#define    CNT54_Index    ((u16)53)
#define    CNT55_Index    ((u16)54)
#define    CNT56_Index    ((u16)55)
#define    CNT57_Index    ((u16)56)
#define    CNT58_Index    ((u16)57)
#define    CNT59_Index    ((u16)58)
#define    CNT60_Index    ((u16)59)
#define    CNT61_Index    ((u16)60)
#define    CNT62_Index    ((u16)61)
#define    CNT63_Index    ((u16)62)
#define    CNT64_Index    ((u16)63)
#define    CNT65_Index    ((u16)64)
#define    CNT66_Index    ((u16)65)
#define    CNT67_Index    ((u16)66)
#define    CNT68_Index    ((u16)67)
#define    CNT69_Index    ((u16)68)
#define    CNT70_Index    ((u16)69)
#define    CNT71_Index    ((u16)70)
#define    CNT72_Index    ((u16)71)
#define    CNT73_Index    ((u16)72)
#define    CNT74_Index    ((u16)73)
#define    CNT75_Index    ((u16)74)
#define    CNT76_Index    ((u16)75)
#define    CNT77_Index    ((u16)76)
#define    CNT78_Index    ((u16)77)
#define    CNT79_Index    ((u16)78)
#define    CNT80_Index    ((u16)79)
#define    CNT81_Index    ((u16)80)
#define    CNT82_Index    ((u16)81)
#define    CNT83_Index    ((u16)82)
#define    CNT84_Index    ((u16)83)
#define    CNT85_Index    ((u16)84)
#define    CNT86_Index    ((u16)85)
#define    CNT87_Index    ((u16)86)
#define    CNT88_Index    ((u16)87)
#define    CNT89_Index    ((u16)88)
#define    CNT90_Index    ((u16)89)
#define    CNT91_Index    ((u16)90)
#define    CNT92_Index    ((u16)91)
#define    CNT93_Index    ((u16)92)
#define    CNT94_Index    ((u16)93)
#define    CNT95_Index    ((u16)94)
#define    CNT96_Index    ((u16)95)

#define    CNT97_Index    ((u16)96)
#define    CNT98_Index    ((u16)97)
#define    CNT99_Index    ((u16)98)
#define    CNT100_Index   ((u16)99)
#define    CNT101_Index   ((u16)100)
#define    CNT102_Index   ((u16)101)
#define    CNT103_Index   ((u16)102)
#define    CNT104_Index   ((u16)103)

#define    CNT105_Index    ((u16)104)
#define    CNT106_Index    ((u16)105)
#define    CNT107_Index    ((u16)106)
#define    CNT108_Index    ((u16)107)
#define    CNT109_Index    ((u16)108)
#define    CNT110_Index    ((u16)109)
#define    CNT111_Index    ((u16)110)
#define    CNT112_Index    ((u16)111)

#define    CNT113_Index    ((u16)112)
#define    CNT114_Index    ((u16)113)
#define    CNT115_Index    ((u16)114)
#define    CNT116_Index    ((u16)115)
#define    CNT117_Index    ((u16)116)
#define    CNT118_Index    ((u16)117)
#define    CNT119_Index    ((u16)118)
#define    CNT120_Index    ((u16)119)

#define    CNT121_Index    ((u16)120)
#define    CNT122_Index    ((u16)121)
#define    CNT123_Index    ((u16)122)
#define    CNT124_Index    ((u16)123)
#define    CNT125_Index    ((u16)124)
#define    CNT126_Index    ((u16)125)
#define    CNT127_Index    ((u16)126)
#define    CNT128_Index    ((u16)127)

#define IS_ACTIVE_CNT(INDEX) (((INDEX) == CNT01_Index)  || \
	                          ((INDEX) == CNT02_Index)  || \
							  ((INDEX) == CNT03_Index)  || \
							  ((INDEX) == CNT04_Index)  || \
							  ((INDEX) == CNT05_Index)  || \
							  ((INDEX) == CNT06_Index)  || \
							  ((INDEX) == CNT07_Index)  || \
							  ((INDEX) == CNT08_Index)  || \
							  ((INDEX) == CNT09_Index)  || \
							  ((INDEX) == CNT10_Index)  || \
							  ((INDEX) == CNT11_Index)  || \
							  ((INDEX) == CNT12_Index)  || \
							  ((INDEX) == CNT13_Index)  || \
							  ((INDEX) == CNT14_Index)  || \
							  ((INDEX) == CNT15_Index)  || \
							  ((INDEX) == CNT16_Index)  || \
							  ((INDEX) == CNT17_Index)  || \
							  ((INDEX) == CNT18_Index)  || \
							  ((INDEX) == CNT19_Index)  || \
							  ((INDEX) == CNT20_Index)  || \
							  ((INDEX) == CNT21_Index)  || \
							  ((INDEX) == CNT22_Index)  || \
							  ((INDEX) == CNT23_Index)  || \
							  ((INDEX) == CNT24_Index)  || \
							  ((INDEX) == CNT25_Index)  || \
							  ((INDEX) == CNT26_Index)  || \
							  ((INDEX) == CNT27_Index)  || \
							  ((INDEX) == CNT28_Index)  || \
							  ((INDEX) == CNT29_Index)  || \
							  ((INDEX) == CNT30_Index)  || \
							  ((INDEX) == CNT32_Index)  || \
							  ((INDEX) == CNT33_Index)  || \
							  ((INDEX) == CNT34_Index)  || \
							  ((INDEX) == CNT35_Index)  || \
							  ((INDEX) == CNT36_Index)  || \
							  ((INDEX) == CNT37_Index)  || \
							  ((INDEX) == CNT38_Index)  || \
							  ((INDEX) == CNT39_Index)  || \
							  ((INDEX) == CNT40_Index)  || \
							  ((INDEX) == CNT41_Index)  || \
							  ((INDEX) == CNT42_Index)  || \
							  ((INDEX) == CNT43_Index)  || \
							  ((INDEX) == CNT44_Index)  || \
							  ((INDEX) == CNT45_Index)  || \
							  ((INDEX) == CNT46_Index)  || \
							  ((INDEX) == CNT47_Index)  || \
							  ((INDEX) == CNT48_Index)  || \
							  ((INDEX) == CNT49_Index)  || \
							  ((INDEX) == CNT50_Index)  || \
							  ((INDEX) == CNT51_Index)  || \
							  ((INDEX) == CNT52_Index)  || \
							  ((INDEX) == CNT53_Index)  || \
							  ((INDEX) == CNT54_Index)  || \
							  ((INDEX) == CNT55_Index)  || \
							  ((INDEX) == CNT56_Index)  || \
							  ((INDEX) == CNT57_Index)  || \
							  ((INDEX) == CNT58_Index)  || \
							  ((INDEX) == CNT59_Index)  || \
							  ((INDEX) == CNT60_Index)  || \
							  ((INDEX) == CNT61_Index)  || \
							  ((INDEX) == CNT62_Index)  || \
							  ((INDEX) == CNT63_Index)  || \
							  ((INDEX) == CNT64_Index)  || \
							  ((INDEX) == CNT65_Index)  || \
							  ((INDEX) == CNT66_Index)  || \
							  ((INDEX) == CNT67_Index)  || \
							  ((INDEX) == CNT68_Index)  || \
							  ((INDEX) == CNT69_Index)  || \
							  ((INDEX) == CNT70_Index)  || \
							  ((INDEX) == CNT71_Index)  || \
							  ((INDEX) == CNT72_Index)  || \
							  ((INDEX) == CNT73_Index)  || \
							  ((INDEX) == CNT74_Index)  || \
							  ((INDEX) == CNT75_Index)  || \
							  ((INDEX) == CNT76_Index)  || \
							  ((INDEX) == CNT77_Index)  || \
							  ((INDEX) == CNT78_Index)  || \
							  ((INDEX) == CNT79_Index)  || \
							  ((INDEX) == CNT80_Index)  || \
							  ((INDEX) == CNT81_Index)  || \
							  ((INDEX) == CNT82_Index)  || \
							  ((INDEX) == CNT83_Index)  || \
							  ((INDEX) == CNT84_Index)  || \
							  ((INDEX) == CNT85_Index)  || \
							  ((INDEX) == CNT86_Index)  || \
							  ((INDEX) == CNT87_Index)  || \
							  ((INDEX) == CNT88_Index)  || \
							  ((INDEX) == CNT89_Index)  || \
							  ((INDEX) == CNT90_Index)  || \
							  ((INDEX) == CNT91_Index)  || \
							  ((INDEX) == CNT92_Index)  || \
							  ((INDEX) == CNT93_Index)  || \
							  ((INDEX) == CNT94_Index)  || \
							  ((INDEX) == CNT95_Index)  || \
							  ((INDEX) == CNT96_Index)  || \
							  ((INDEX) == CNT97_Index)  || \
							  ((INDEX) == CNT98_Index)  || \
							  ((INDEX) == CNT99_Index)  || \
							  ((INDEX) == CNT100_Index) || \
							  ((INDEX) == CNT101_Index) || \
							  ((INDEX) == CNT102_Index) || \
							  ((INDEX) == CNT103_Index) || \
							  ((INDEX) == CNT104_Index) || \
							  ((INDEX) == CNT105_Index) || \
							  ((INDEX) == CNT106_Index) || \
							  ((INDEX) == CNT107_Index) || \
							  ((INDEX) == CNT108_Index) || \
							  ((INDEX) == CNT109_Index) || \
							  ((INDEX) == CNT110_Index) || \
							  ((INDEX) == CNT111_Index) || \
							  ((INDEX) == CNT112_Index) || \
							  ((INDEX) == CNT113_Index) || \
							  ((INDEX) == CNT114_Index) || \
							  ((INDEX) == CNT115_Index) || \
							  ((INDEX) == CNT116_Index) || \
							  ((INDEX) == CNT117_Index) || \
							  ((INDEX) == CNT118_Index) || \
							  ((INDEX) == CNT119_Index) || \
							  ((INDEX) == CNT120_Index) || \
							  ((INDEX) == CNT121_Index) || \
							  ((INDEX) == CNT122_Index) || \
							  ((INDEX) == CNT123_Index) || \
							  ((INDEX) == CNT124_Index) || \
							  ((INDEX) == CNT125_Index) || \
							  ((INDEX) == CNT126_Index) || \
							  ((INDEX) == CNT127_Index) || \
							  ((INDEX) == CNT128_Index))

typedef struct _DS_CNT_
{
	/* CNT 01 */ 	u32  cnt01_val;		//�����ʱ��ͨ�� 01
	/* CNT 02 */	u32  cnt02_val;		//�����ʱ��ͨ�� 02
	/* CNT 03 */	u32  cnt03_val;		//�����ʱ��ͨ�� 03
	/* CNT 04 */	u32  cnt04_val;		//�����ʱ��ͨ�� 04
	/* CNT 05 */	u32  cnt05_val;		//�����ʱ��ͨ�� 05
	/* CNT 06 */	u32  cnt06_val;     //�����ʱ��ͨ�� 06
	/* CNT 07 */	u32  cnt07_val;		//�����ʱ��ͨ�� 07
	/* CNT 08 */	u32  cnt08_val;		//�����ʱ��ͨ�� 08
	/* CNT 09 */	u32  cnt09_val;		//�����ʱ��ͨ�� 09
	/* CNT 10 */	u32  cnt10_val;		//�����ʱ��ͨ�� 10
	/* CNT 11 */	u32  cnt11_val;		//�����ʱ��ͨ�� 11
	/* CNT 12 */	u32  cnt12_val;		//�����ʱ��ͨ�� 12
	/* CNT 13 */	u32  cnt13_val;		//�����ʱ��ͨ�� 13
	/* CNT 14 */	u32  cnt14_val;		//�����ʱ��ͨ�� 14
	/* CNT 15 */	u32  cnt15_val;     //�����ʱ��ͨ�� 15
	/* CNT 16 */	u32  cnt16_val;		//�����ʱ��ͨ�� 16
	/* CNT 17 */	u32  cnt17_val;		//�����ʱ��ͨ�� 17
	/* CNT 18 */	u32  cnt18_val;		//�����ʱ��ͨ�� 18
	/* CNT 19 */	u32  cnt19_val;		//�����ʱ��ͨ�� 19
	/* CNT 20 */	u32  cnt20_val;		//�����ʱ��ͨ�� 20
	/* CNT 21 */	u32  cnt21_val;		//�����ʱ��ͨ�� 21
	/* CNT 22 */	u32  cnt22_val;		//�����ʱ��ͨ�� 22
	/* CNT 23 */	u32  cnt23_val;		//�����ʱ��ͨ�� 23
	/* CNT 24 */	u32  cnt24_val;     //�����ʱ��ͨ�� 24
	/* CNT 25 */	u32  cnt25_val;		//�����ʱ��ͨ�� 25
	/* CNT 26 */	u32  cnt26_val;		//�����ʱ��ͨ�� 26
	/* CNT 27 */	u32  cnt27_val;		//�����ʱ��ͨ�� 27
	/* CNT 28 */	u32  cnt28_val;		//�����ʱ��ͨ�� 28
	/* CNT 29 */	u32  cnt29_val;		//�����ʱ��ͨ�� 29
	/* CNT 30 */	u32  cnt30_val;		//�����ʱ��ͨ�� 30
	/* CNT 31 */	u32  cnt31_val;		//�����ʱ��ͨ�� 31
	/* CNT 32 */	u32  cnt32_val;		//�����ʱ��ͨ�� 32
	/* CNT 33 */	u32  cnt33_val;		//�����ʱ��ͨ�� 33
	/* CNT 34 */	u32  cnt34_val;		//�����ʱ��ͨ�� 34
	/* CNT 35 */	u32  cnt35_val;		//�����ʱ��ͨ�� 35
	/* CNT 36 */	u32  cnt36_val;		//�����ʱ��ͨ�� 36
	/* CNT 37 */	u32  cnt37_val;		//�����ʱ��ͨ�� 37
	/* CNT 38 */	u32  cnt38_val;     //�����ʱ��ͨ�� 38
	/* CNT 39 */	u32  cnt39_val;		//�����ʱ��ͨ�� 39
	/* CNT 40 */	u32  cnt40_val;		//�����ʱ��ͨ�� 40
	/* CNT 41 */	u32  cnt41_val;		//�����ʱ��ͨ�� 41
	/* CNT 42 */	u32  cnt42_val;		//�����ʱ��ͨ�� 42
	/* CNT 43 */	u32  cnt43_val;		//�����ʱ��ͨ�� 43
	/* CNT 44 */	u32  cnt44_val;		//�����ʱ��ͨ�� 44
	/* CNT 45 */	u32  cnt45_val;		//�����ʱ��ͨ�� 45
	/* CNT 46 */	u32  cnt46_val;		//�����ʱ��ͨ�� 46
	/* CNT 47 */	u32  cnt47_val;     //�����ʱ��ͨ�� 47
	/* CNT 48 */	u32  cnt48_val;		//�����ʱ��ͨ�� 48
	/* CNT 49 */	u32  cnt49_val;		//�����ʱ��ͨ�� 49
	/* CNT 50 */	u32  cnt50_val;		//�����ʱ��ͨ�� 50
	/* CNT 51 */	u32  cnt51_val;		//�����ʱ��ͨ�� 51
	/* CNT 52 */	u32  cnt52_val;		//�����ʱ��ͨ�� 52
	/* CNT 53 */	u32  cnt53_val;		//�����ʱ��ͨ�� 53
	/* CNT 54 */	u32  cnt54_val;		//�����ʱ��ͨ�� 54
	/* CNT 55 */	u32  cnt55_val;		//�����ʱ��ͨ�� 55
	/* CNT 56 */	u32  cnt56_val;     //�����ʱ��ͨ�� 56
	/* CNT 57 */	u32  cnt57_val;		//�����ʱ��ͨ�� 57
	/* CNT 58 */	u32  cnt58_val;		//�����ʱ��ͨ�� 58
	/* CNT 59 */	u32  cnt59_val;		//�����ʱ��ͨ�� 59
	/* CNT 60 */	u32  cnt60_val;		//�����ʱ��ͨ�� 60
	/* CNT 61 */	u32  cnt61_val;		//�����ʱ��ͨ�� 61
	/* CNT 62 */	u32  cnt62_val;		//�����ʱ��ͨ�� 62
	/* CNT 63 */	u32  cnt63_val;		//�����ʱ��ͨ�� 63
	/* CNT 64 */	u32  cnt64_val;		//�����ʱ��ͨ�� 64
	/* CNT 65 */	u32  cnt65_val;		//�����ʱ��ͨ�� 65
	/* CNT 66 */	u32  cnt66_val;		//�����ʱ��ͨ�� 66
	/* CNT 67 */	u32  cnt67_val;		//�����ʱ��ͨ�� 67
	/* CNT 68 */	u32  cnt68_val;		//�����ʱ��ͨ�� 68
	/* CNT 69 */	u32  cnt69_val;		//�����ʱ��ͨ�� 69
	/* CNT 70 */	u32  cnt70_val;     //�����ʱ��ͨ�� 70
	/* CNT 71 */	u32  cnt71_val;		//�����ʱ��ͨ�� 71
	/* CNT 72 */	u32  cnt72_val;		//�����ʱ��ͨ�� 72
	/* CNT 73 */	u32  cnt73_val;		//�����ʱ��ͨ�� 73
	/* CNT 74 */	u32  cnt74_val;		//�����ʱ��ͨ�� 74
	/* CNT 75 */	u32  cnt75_val;		//�����ʱ��ͨ�� 75
	/* CNT 76 */	u32  cnt76_val;		//�����ʱ��ͨ�� 76
	/* CNT 77 */	u32  cnt77_val;		//�����ʱ��ͨ�� 77
	/* CNT 78 */	u32  cnt78_val;		//�����ʱ��ͨ�� 78
	/* CNT 79 */	u32  cnt79_val;     //�����ʱ��ͨ�� 79
	/* CNT 80 */	u32  cnt80_val;		//�����ʱ��ͨ�� 80
	/* CNT 81 */	u32  cnt81_val;		//�����ʱ��ͨ�� 81
	/* CNT 82 */	u32  cnt82_val;		//�����ʱ��ͨ�� 82
	/* CNT 83 */	u32  cnt83_val;		//�����ʱ��ͨ�� 83
	/* CNT 84 */	u32  cnt84_val;		//�����ʱ��ͨ�� 84
	/* CNT 85 */	u32  cnt85_val;		//�����ʱ��ͨ�� 85
	/* CNT 86 */	u32  cnt86_val;		//�����ʱ��ͨ�� 86
	/* CNT 87 */	u32  cnt87_val;		//�����ʱ��ͨ�� 87
	/* CNT 88 */	u32  cnt88_val;     //�����ʱ��ͨ�� 88
	/* CNT 89 */	u32  cnt89_val;		//�����ʱ��ͨ�� 89
	/* CNT 90 */	u32  cnt90_val;		//�����ʱ��ͨ�� 90
	/* CNT 91 */	u32  cnt91_val;		//�����ʱ��ͨ�� 91
	/* CNT 92 */	u32  cnt92_val;		//�����ʱ��ͨ�� 92
	/* CNT 93 */	u32  cnt93_val;		//�����ʱ��ͨ�� 93
	/* CNT 94 */	u32  cnt94_val;		//�����ʱ��ͨ�� 94
	/* CNT 95 */	u32  cnt95_val;		//�����ʱ��ͨ�� 95
	/* CNT 96 */	u32  cnt96_val;		//�����ʱ��ͨ�� 96

	/* CNT 97 */	u32  cnt97_val;	    //�����ʱ��ͨ�� 97
	/* CNT 98 */	u32  cnt98_val;	    //�����ʱ��ͨ�� 98
	/* CNT 99 */	u32  cnt99_val;	    //�����ʱ��ͨ�� 99
	/* CNT 100*/	u32  cnt100_val;	//�����ʱ��ͨ�� 100
	/* CNT 101*/	u32  cnt101_val;	//�����ʱ��ͨ�� 101
	/* CNT 102*/	u32  cnt102_val;	//�����ʱ��ͨ�� 102
	/* CNT 103*/	u32  cnt103_val;	//�����ʱ��ͨ�� 103
	/* CNT 104*/	u32  cnt104_val;	//�����ʱ��ͨ�� 104

	/* CNT 105*/	u32  cnt105_val;	//�����ʱ��ͨ�� 105
	/* CNT 106*/	u32  cnt106_val;	//�����ʱ��ͨ�� 106
	/* CNT 107*/	u32  cnt107_val;	//�����ʱ��ͨ�� 107
	/* CNT 108*/	u32  cnt108_val;	//�����ʱ��ͨ�� 108
	/* CNT 109*/	u32  cnt109_val;	//�����ʱ��ͨ�� 109
	/* CNT 110*/	u32  cnt110_val;	//�����ʱ��ͨ�� 110
	/* CNT 111*/	u32  cnt111_val;	//�����ʱ��ͨ�� 111
	/* CNT 112*/	u32  cnt112_val;	//�����ʱ��ͨ�� 112

	/* CNT 113*/	u32  cnt113_val;	//�����ʱ��ͨ�� 113
	/* CNT 114*/	u32  cnt114_val;	//�����ʱ��ͨ�� 114
	/* CNT 115*/	u32  cnt115_val;	//�����ʱ��ͨ�� 115
	/* CNT 116*/	u32  cnt116_val;	//�����ʱ��ͨ�� 116
	/* CNT 117*/	u32  cnt117_val;	//�����ʱ��ͨ�� 117
	/* CNT 118*/	u32  cnt118_val;	//�����ʱ��ͨ�� 118
	/* CNT 119*/	u32  cnt119_val;	//�����ʱ��ͨ�� 119
	/* CNT 120*/	u32  cnt120_val;	//�����ʱ��ͨ�� 120

	/* CNT 121*/	u32  cnt121_val;	//�����ʱ��ͨ�� 121
	/* CNT 122*/	u32  cnt122_val;	//�����ʱ��ͨ�� 122
	/* CNT 123*/	u32  cnt123_val;	//�����ʱ��ͨ�� 123
	/* CNT 124*/	u32  cnt124_val;	//�����ʱ��ͨ�� 124
	/* CNT 125*/	u32  cnt125_val;	//�����ʱ��ͨ�� 125
	/* CNT 126*/	u32  cnt126_val;	//�����ʱ��ͨ�� 126
	/* CNT 127*/	u32  cnt127_val;	//�����ʱ��ͨ�� 127
	/* CNT 128*/	u32  cnt128_val;	//�����ʱ��ͨ�� 128

}DS_CNT, *PDS_CNT;

//------------------------------------------------------------------------------
//  �������ӿ�ģʽ
//------------------------------------------------------------------------------
typedef struct _DS_IO_MODE_
{
	/* Bit01 */    u16    IO01_MODE      :  1  ;  //ͨ��01ģʽ
	/* Bit02 */    u16    IO02_MODE      :  1  ;  //ͨ��02ģʽ
	/* Bit03 */    u16    IO03_MODE      :  1  ;  //ͨ��03ģʽ
	/* Bit04 */    u16    IO04_MODE      :  1  ;  //ͨ��04ģʽ
	/* Bit05 */    u16    IO05_MODE      :  1  ;  //ͨ��05ģʽ
	/* Bit06 */    u16    IO06_MODE      :  1  ;  //ͨ��06ģʽ
	/* Bit07 */    u16    IO07_MODE      :  1  ;  //ͨ��07ģʽ
	/* Bit08 */    u16    IO08_MODE      :  1  ;  //ͨ��08ģʽ
	/* Bit09 */    u16    IO09_MODE      :  1  ;  //ͨ��09ģʽ
	/* Bit10 */    u16    IO10_MODE      :  1  ;  //ͨ��10ģʽ
	/* Bit11 */    u16    IO11_MODE      :  1  ;  //ͨ��11ģʽ
	/* Bit12 */    u16    IO12_MODE      :  1  ;  //ͨ��12ģʽ
	/* Bit13 */    u16    IO13_MODE      :  1  ;  //ͨ��13ģʽ
	/* Bit14 */    u16    IO14_MODE      :  1  ;  //ͨ��14ģʽ
	/* Bit15 */    u16    IO15_MODE      :  1  ;  //ͨ��15ģʽ
	/* Bit16 */    u16    IO16_MODE      :  1  ;  //ͨ��16ģʽ
}DS_IO_MODE, *PDS_IO_MODE;


//------------------------------------------------------------------------------
//  ����������ӳ��ӿ�
//------------------------------------------------------------------------------

 typedef struct _CAN_IO_IN_
 {
 	/* Bit01 */    u16    IN01_IO        :  1  ;  //����ͨ��01
 	/* Bit02 */    u16    IN02_IO        :  1  ;  //����ͨ��02
 	/* Bit03 */    u16    IN03_IO        :  1  ;  //����ͨ��03
 	/* Bit04 */    u16    IN04_IO        :  1  ;  //����ͨ��04
 	/* Bit05 */    u16    IN05_IO        :  1  ;  //����ͨ��05
 	/* Bit06 */    u16    IN06_IO        :  1  ;  //����ͨ��06
 	/* Bit07 */    u16    IN07_IO        :  1  ;  //����ͨ��07
 	/* Bit08 */    u16    IN08_IO        :  1  ;  //����ͨ��08
 	/* Bit09 */    u16    IN09_IO        :  1  ;  //����ͨ��09
 	/* Bit10 */    u16    IN10_IO        :  1  ;  //����ͨ��10
 	/* Bit11 */    u16    IN11_IO        :  1  ;  //����ͨ��11
 	/* Bit12 */    u16    IN12_IO        :  1  ;  //����ͨ��12
 	/* Bit13 */    u16    IN13_IO        :  1  ;  //����ͨ��13
 	/* Bit14 */    u16    IN14_IO        :  1  ;  //����ͨ��14
 	/* Bit15 */    u16    IN15_IO        :  1  ;  //����ͨ��15
 	/* Bit16 */    u16    IN16_IO        :  1  ;  //����ͨ��16

 }CAN_IO_IN, *PCAN_IO_IN;

 typedef struct _CAN_IO_INS_
 {
 	/* Bit01 */    uint32_t    IN01_IO        :  1  ;  //����ͨ��01
 	/* Bit02 */    uint32_t    IN02_IO        :  1  ;  //����ͨ��02
 	/* Bit03 */    uint32_t    IN03_IO        :  1  ;  //����ͨ��03
 	/* Bit04 */    uint32_t    IN04_IO        :  1  ;  //����ͨ��04
 	/* Bit05 */    uint32_t    IN05_IO        :  1  ;  //����ͨ��05
 	/* Bit06 */    uint32_t    IN06_IO        :  1  ;  //����ͨ��06
 	/* Bit07 */    uint32_t    IN07_IO        :  1  ;  //����ͨ��07
 	/* Bit08 */    uint32_t    IN08_IO        :  1  ;  //����ͨ��08
 	/* Bit09 */    uint32_t    IN09_IO        :  1  ;  //����ͨ��09
 	/* Bit10 */    uint32_t    IN10_IO        :  1  ;  //����ͨ��10
 	/* Bit11 */    uint32_t    IN11_IO        :  1  ;  //����ͨ��11
 	/* Bit12 */    uint32_t    IN12_IO        :  1  ;  //����ͨ��12
 	/* Bit13 */    uint32_t    IN13_IO        :  1  ;  //����ͨ��13
 	/* Bit14 */    uint32_t    IN14_IO        :  1  ;  //����ͨ��14
 	/* Bit15 */    uint32_t    IN15_IO        :  1  ;  //����ͨ��15
 	/* Bit16 */    uint32_t    IN16_IO        :  1  ;  //����ͨ��16

    /* Bit01 */    uint32_t    IN17_IO        :  1  ;  //����ͨ��01
 	/* Bit02 */    uint32_t    IN18_IO        :  1  ;  //����ͨ��02
 	/* Bit03 */    uint32_t    IN19_IO        :  1  ;  //����ͨ��03
 	/* Bit04 */    uint32_t    IN20_IO        :  1  ;  //����ͨ��04
 	/* Bit05 */    uint32_t    IN21_IO        :  1  ;  //����ͨ��05
 	/* Bit06 */    uint32_t    IN22_IO        :  1  ;  //����ͨ��06
 	/* Bit07 */    uint32_t    IN23_IO        :  1  ;  //����ͨ��07
 	/* Bit08 */    uint32_t    IN24_IO        :  1  ;  //����ͨ��08
 	/* Bit09 */    uint32_t    IN25_IO        :  1  ;  //����ͨ��09
 	/* Bit10 */    uint32_t    IN26_IO        :  1  ;  //����ͨ��10
 	/* Bit11 */    uint32_t    IN27_IO        :  1  ;  //����ͨ��11
 	/* Bit12 */    uint32_t    IN28_IO        :  1  ;  //����ͨ��12
 	/* Bit13 */    uint32_t    IN29_IO        :  1  ;  //����ͨ��13
 	/* Bit14 */    uint32_t    IN30_IO        :  1  ;  //����ͨ��14
 	/* Bit15 */    uint32_t    IN31_IO        :  1  ;  //����ͨ��15
 	/* Bit16 */    uint32_t    IN32_IO        :  1  ;  //����ͨ��16

 }CAN_IO_INS, *PCAN_IO_INS;

 
 
//typedef struct _DS_CAN_IO_IN_
//{
//	CAN_IO_IN  CAN_IN[6];
//
//}DS_CAN_IO_IN, *PDS_CAN_IO_IN;

 typedef struct _DS_CAN_IO_IN_
 {
     uint32_t  CAN_IN[16];

 }DS_CAN_IO_IN, *PDS_CAN_IO_IN;


//------------------------------------------------------------------------------
//  ����������ӳ��ӿ�
//------------------------------------------------------------------------------

 typedef struct _CAN_IO_OUT_
 {
 	/* Bit01 */    u16    OUT01_IO        :  1  ;  //���ͨ��01
 	/* Bit02 */    u16    OUT02_IO        :  1  ;  //���ͨ��02
 	/* Bit03 */    u16    OUT03_IO        :  1  ;  //���ͨ��03
 	/* Bit04 */    u16    OUT04_IO        :  1  ;  //���ͨ��04
 	/* Bit05 */    u16    OUT05_IO        :  1  ;  //���ͨ��05
 	/* Bit06 */    u16    OUT06_IO        :  1  ;  //���ͨ��06
 	/* Bit07 */    u16    OUT07_IO        :  1  ;  //���ͨ��07
 	/* Bit08 */    u16    OUT08_IO        :  1  ;  //���ͨ��08
 	/* Bit09 */    u16    OUT09_IO        :  1  ;  //���ͨ��09
 	/* Bit10 */    u16    OUT10_IO        :  1  ;  //���ͨ��10
 	/* Bit11 */    u16    OUT11_IO        :  1  ;  //���ͨ��11
 	/* Bit12 */    u16    OUT12_IO        :  1  ;  //���ͨ��12
    /* Bit13 */    u16    OUT13_IO        :  1  ;  //���ͨ��13
    /* Bit14 */    u16    OUT14_IO        :  1  ;  //���ͨ��14
    /*Bit1316*/    u16    RES             :  4  ;  //

 }CAN_IO_OUT, *PCAN_IO_OUT;


typedef struct _DS_CAN_IO_OUT_
{
	uint32_t  CAN_OUT[16];

}DS_CAN_IO_OUT, *PDS_CAN_IO_OUT;


//------------------------------------------------------------------------------
// �����ͨ�ţ����ղ�������  8bytes
//------------------------------------------------------------------------------
typedef struct _DS_RNET_IN_
{
    uint8_t  header;
    uint8_t  year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;
    uint8_t  chksum;
}DS_RNET_IN, *PDS_RNET_IN;

//------------------------------------------------------------------------------
// �����ͨ�ţ�Ӧ���������   16bytes
//------------------------------------------------------------------------------
typedef struct _DS_RNET_OUT_
{
    uint8_t  header;
    uint8_t  func;
    uint8_t  data1;
    uint8_t  data2;
    uint8_t  data3;
    uint8_t  data4;
    uint8_t  data5;
    uint8_t  data6;
    uint8_t  data7;
    uint8_t  data8;
    uint8_t  data9;
    uint8_t  data10;
    uint8_t  data11;
    uint8_t  data12;
    uint8_t  data13;
    uint8_t  chksum;

}DS_RNET_OUT, *PDS_RNET_OUT;

//------------------------------------------------------------------------------
//  ����������ӳ��ӿ�
//------------------------------------------------------------------------------

typedef struct _DS_IO_IN_
{
	/* Bit01 */    u16    IN01_IO        :  1  ;  //����ͨ��01
	/* Bit02 */    u16    IN02_IO        :  1  ;  //����ͨ��02
	/* Bit03 */    u16    IN03_IO        :  1  ;  //����ͨ��03
    /* Bit04 */    u16    IN04_IO        :  1  ;  //����ͨ��04
    /* Bit05 */    u16    IN05_IO        :  1  ;  //����ͨ��05
    /* Bit06 */    u16    IN06_IO        :  1  ;  //����ͨ��06
    /* Bit07 */    u16    IN07_IO        :  1  ;  //����ͨ��07
    /* Bit08 */    u16    IN08_IO        :  1  ;  //����ͨ��08
    /* Bit09 */    u16    IN09_IO        :  1  ;  //����ͨ��09
    /* Bit10 */    u16    IN10_IO        :  1  ;  //����ͨ��10
    /* Bit11 */    u16    IN11_IO        :  1  ;  //����ͨ��11
    /* Bit12 */    u16    IN12_IO        :  1  ;  //����ͨ��12
    /* Bit13 */    u16    IN13_IO        :  1  ;  //����ͨ��13
    /* Bit14 */    u16    IN14_IO        :  1  ;  //����ͨ��14
    /* Bit15 */    u16    IN15_IO        :  1  ;  //����ͨ��15
    /* Bit16 */    u16    IN16_IO        :  1  ;  //����ͨ��16

    /* Bit01 */    u16    IN17_IO        :  1  ;  //����ͨ��17
    /* Bit02 */    u16    IN18_IO        :  1  ;  //����ͨ��18
    /* Bit03 */    u16    IN19_IO        :  1  ;  //����ͨ��19
    /* Bit04 */    u16    IN20_IO        :  1  ;  //����ͨ��20
    /* Bit05 */    u16    IN21_IO        :  1  ;  //����ͨ��21
    /* Bit06 */    u16    IN22_IO        :  1  ;  //����ͨ��22
    /* Bit07 */    u16    IN23_IO        :  1  ;  //����ͨ��23
    /* Bit08 */    u16    IN24_IO        :  1  ;  //����ͨ��24
    /* Bit09 */    u16    IN25_IO        :  1  ;  //����ͨ��25
    /* Bit10 */    u16    IN26_IO        :  1  ;  //����ͨ��26
    /* Bit11 */    u16    IN27_IO        :  1  ;  //����ͨ��27
    /* Bit12 */    u16    IN28_IO        :  1  ;  //����ͨ��28
    /* Bit13 */    u16    IN29_IO        :  1  ;  //����ͨ��29
    /* Bit14 */    u16    IN30_IO        :  1  ;  //����ͨ��30
    /* Bit15 */    u16    IN31_IO        :  1  ;  //����ͨ��31
    /* Bit16 */    u16    IN32_IO        :  1  ;  //����ͨ��32

    /* Bit01 */    u16    IN33_IO        :  1  ;  //����ͨ��33
    /* Bit02 */    u16    IN34_IO        :  1  ;  //����ͨ��34
    /* Bit03 */    u16    IN35_IO        :  1  ;  //����ͨ��35
    /* Bit04 */    u16    IN36_IO        :  1  ;  //����ͨ��36
    /* Bit05 */    u16    IN37_IO        :  1  ;  //����ͨ��37
    /* Bit06 */    u16    IN38_IO        :  1  ;  //����ͨ��38
    /* Bit07 */    u16    IN39_IO        :  1  ;  //����ͨ��39
    /* Bit08 */    u16    IN40_IO        :  1  ;  //����ͨ��40
    /* Bit09 */    u16    IN41_IO        :  1  ;  //����ͨ��41
    /* Bit10 */    u16    IN42_IO        :  1  ;  //����ͨ��42
    /* Bit11 */    u16    IN43_IO        :  1  ;  //����ͨ��43
    /* Bit12 */    u16    IN44_IO        :  1  ;  //����ͨ��44
    /* Bit13 */    u16    IN45_IO        :  1  ;  //����ͨ��45
    /* Bit14 */    u16    IN46_IO        :  1  ;  //����ͨ��46
    /* Bit15 */    u16    IN47_IO        :  1  ;  //����ͨ��47
    /* Bit16 */    u16    IN48_IO        :  1  ;  //����ͨ��48

    /* Bit01 */    u16    IN49_IO        :  1  ;  //����ͨ��49
    /* Bit02 */    u16    IN50_IO        :  1  ;  //����ͨ��50
    /* Bit03 */    u16    IN51_IO        :  1  ;  //����ͨ��51
    /* Bit04 */    u16    IN52_IO        :  1  ;  //����ͨ��52
    /* Bit05 */    u16    IN53_IO        :  1  ;  //����ͨ��53
    /* Bit06 */    u16    IN54_IO        :  1  ;  //����ͨ��54
    /* Bit07 */    u16    IN55_IO        :  1  ;  //����ͨ��55
    /* Bit08 */    u16    IN56_IO        :  1  ;  //����ͨ��56
    /* Bit09 */    u16    IN57_IO        :  1  ;  //����ͨ��57
    /* Bit10 */    u16    IN58_IO        :  1  ;  //����ͨ��58
    /* Bit11 */    u16    IN59_IO        :  1  ;  //����ͨ��59
    /* Bit12 */    u16    IN60_IO        :  1  ;  //����ͨ��60
    /* Bit13 */    u16    IN61_IO        :  1  ;  //����ͨ��61
    /* Bit14 */    u16    IN62_IO        :  1  ;  //����ͨ��62
    /* Bit15 */    u16    IN63_IO        :  1  ;  //����ͨ��63
    /* Bit16 */    u16    IN64_IO        :  1  ;  //����ͨ��64

    /* Bit01 */    u16    IN65_IO        :  1  ;  //����ͨ��65
    /* Bit02 */    u16    IN66_IO        :  1  ;  //����ͨ��66
    /* Bit03 */    u16    IN67_IO        :  1  ;  //����ͨ��67
    /* Bit04 */    u16    IN68_IO        :  1  ;  //����ͨ��68
    /* Bit05 */    u16    IN69_IO        :  1  ;  //����ͨ��69
    /* Bit06 */    u16    IN70_IO        :  1  ;  //����ͨ��70
    /* Bit07 */    u16    IN71_IO        :  1  ;  //����ͨ��71
    /* Bit08 */    u16    IN72_IO        :  1  ;  //����ͨ��72
    /* Bit09 */    u16    IN73_IO        :  1  ;  //����ͨ��73
    /* Bit10 */    u16    IN74_IO        :  1  ;  //����ͨ��74
    /* Bit11 */    u16    IN75_IO        :  1  ;  //����ͨ��75
    /* Bit12 */    u16    IN76_IO        :  1  ;  //����ͨ��76
    /* Bit13 */    u16    IN77_IO        :  1  ;  //����ͨ��77
    /* Bit14 */    u16    IN78_IO        :  1  ;  //����ͨ��78
    /* Bit15 */    u16    IN79_IO        :  1  ;  //����ͨ��79
    /* Bit16 */    u16    IN80_IO        :  1  ;  //����ͨ��80

    /* Bit01 */    u16    IN81_IO        :  1  ;  //����ͨ��81
    /* Bit02 */    u16    IN82_IO        :  1  ;  //����ͨ��82
    /* Bit03 */    u16    IN83_IO        :  1  ;  //����ͨ��83
    /* Bit04 */    u16    IN84_IO        :  1  ;  //����ͨ��84
    /* Bit05 */    u16    IN85_IO        :  1  ;  //����ͨ��85
    /* Bit06 */    u16    IN86_IO        :  1  ;  //����ͨ��86
    /* Bit07 */    u16    IN87_IO        :  1  ;  //����ͨ��87
    /* Bit08 */    u16    IN88_IO        :  1  ;  //����ͨ��88
    /* Bit09 */    u16    IN89_IO        :  1  ;  //����ͨ��89
    /* Bit10 */    u16    IN90_IO        :  1  ;  //����ͨ��90
    /* Bit11 */    u16    IN91_IO        :  1  ;  //����ͨ��91
    /* Bit12 */    u16    IN92_IO        :  1  ;  //����ͨ��92
    /* Bit13 */    u16    IN93_IO        :  1  ;  //����ͨ��93
    /* Bit14 */    u16    IN94_IO        :  1  ;  //����ͨ��94
    /* Bit15 */    u16    IN95_IO        :  1  ;  //����ͨ��95
    /* Bit16 */    u16    IN96_IO        :  1  ;  //����ͨ��96

    /* Bit01 */    u16    IN97_IO        :  1  ;  //����ͨ��81
    /* Bit02 */    u16    IN98_IO        :  1  ;  //����ͨ��82
    /* Bit03 */    u16    IN99_IO        :  1  ;  //����ͨ��83
    /* Bit04 */    u16    IN100_IO       :  1  ;  //����ͨ��84
    /* Bit05 */    u16    IN101_IO       :  1  ;  //����ͨ��85
    /* Bit06 */    u16    IN102_IO       :  1  ;  //����ͨ��86
    /* Bit07 */    u16    IN103_IO       :  1  ;  //����ͨ��87
    /* Bit08 */    u16    IN104_IO       :  1  ;  //����ͨ��88
    /* Bit09 */    u16    IN105_IO       :  1  ;  //����ͨ��89
    /* Bit10 */    u16    IN106_IO       :  1  ;  //����ͨ��90
    /* Bit11 */    u16    IN107_IO       :  1  ;  //����ͨ��91
    /* Bit12 */    u16    IN108_IO       :  1  ;  //����ͨ��92
    /* Bit13 */    u16    IN109_IO       :  1  ;  //����ͨ��93
    /* Bit14 */    u16    IN110_IO       :  1  ;  //����ͨ��94
    /* Bit15 */    u16    IN111_IO       :  1  ;  //����ͨ��95
    /* Bit16 */    u16    IN112_IO       :  1  ;  //����ͨ��96

    /* Bit01 */    u16    IN113_IO       :  1  ;  //����ͨ��96
    /* Bit02 */    u16    IN114_IO       :  1  ;  //����ͨ��81
    /* Bit03 */    u16    IN115_IO       :  1  ;  //����ͨ��82
    /* Bit04 */    u16    IN116_IO       :  1  ;  //����ͨ��83
    /* Bit05 */    u16    IN117_IO       :  1  ;  //����ͨ��84
    /* Bit06 */    u16    IN118_IO       :  1  ;  //����ͨ��85
    /* Bit07 */    u16    IN119_IO       :  1  ;  //����ͨ��86
    /* Bit08 */    u16    IN120_IO       :  1  ;  //����ͨ��87
    /* Bit09 */    u16    IN121_IO       :  1  ;  //����ͨ��88
    /* Bit10 */    u16    IN122_IO       :  1  ;  //����ͨ��89
    /* Bit11 */    u16    IN123_IO       :  1  ;  //����ͨ��90
    /* Bit12 */    u16    IN124_IO       :  1  ;  //����ͨ��91
    /* Bit13 */    u16    IN125_IO       :  1  ;  //����ͨ��92
    /* Bit14 */    u16    IN126_IO       :  1  ;  //����ͨ��93
    /* Bit15 */    u16    IN127_IO       :  1  ;  //����ͨ��94
    /* Bit16 */    u16    IN128_IO       :  1  ;  //����ͨ��95

    /* Bit01 */    u16    IN129_IO       :  1  ;  //����ͨ��81
    /* Bit02 */    u16    IN130_IO       :  1  ;  //����ͨ��82
    /* Bit03 */    u16    IN131_IO       :  1  ;  //����ͨ��83
    /* Bit04 */    u16    IN132_IO       :  1  ;  //����ͨ��84
    /* Bit05 */    u16    IN133_IO       :  1  ;  //����ͨ��85
    /* Bit06 */    u16    IN134_IO       :  1  ;  //����ͨ��86
    /* Bit07 */    u16    IN135_IO       :  1  ;  //����ͨ��87
    /* Bit08 */    u16    IN136_IO       :  1  ;  //����ͨ��88
    /* Bit09 */    u16    IN137_IO       :  1  ;  //����ͨ��89
    /* Bit10 */    u16    IN138_IO       :  1  ;  //����ͨ��90
    /* Bit11 */    u16    IN139_IO       :  1  ;  //����ͨ��91
    /* Bit12 */    u16    IN140_IO       :  1  ;  //����ͨ��92
    /* Bit13 */    u16    IN141_IO       :  1  ;  //����ͨ��93
    /* Bit14 */    u16    IN142_IO       :  1  ;  //����ͨ��94
    /* Bit15 */    u16    IN143_IO       :  1  ;  //����ͨ��95
    /* Bit16 */    u16    IN144_IO       :  1  ;  //����ͨ��96

	//Total : 18 Bytes
}DS_IO_IN, *PDS_IO_IN;

//------------------------------------------------------------------------------
//  ���������ӳ��ӿ�
//------------------------------------------------------------------------------
typedef struct _DS_IO_OUT_
{
    /* Bit01 */    u16    OUT01_IO        :  1  ;  //���ͨ��01
    /* Bit02 */    u16    OUT02_IO        :  1  ;  //���ͨ��02
    /* Bit03 */    u16    OUT03_IO        :  1  ;  //���ͨ��03
    /* Bit04 */    u16    OUT04_IO        :  1  ;  //���ͨ��04
    /* Bit05 */    u16    OUT05_IO        :  1  ;  //���ͨ��05
    /* Bit06 */    u16    OUT06_IO        :  1  ;  //���ͨ��06
    /* Bit07 */    u16    OUT07_IO        :  1  ;  //���ͨ��07
    /* Bit08 */    u16    OUT08_IO        :  1  ;  //���ͨ��08
    /* Bit09 */    u16    OUT09_IO        :  1  ;  //���ͨ��09
    /* Bit10 */    u16    OUT10_IO        :  1  ;  //���ͨ��10
    /* Bit11 */    u16    OUT11_IO        :  1  ;  //���ͨ��11
    /* Bit12 */    u16    OUT12_IO        :  1  ;  //���ͨ��12
    /* Bit13 */    u16    OUT13_IO        :  1  ;  //���ͨ��13
    /* Bit14 */    u16    OUT14_IO        :  1  ;  //���ͨ��14
    /* Bit15 */    u16    OUT15_IO        :  1  ;  //���ͨ��15
    /* Bit16 */    u16    OUT16_IO        :  1  ;  //���ͨ��16

    /* Bit01 */    u16    OUT17_IO        :  1  ;  //���ͨ��17
    /* Bit02 */    u16    OUT18_IO        :  1  ;  //���ͨ��18
    /* Bit03 */    u16    OUT19_IO        :  1  ;  //���ͨ��19
    /* Bit04 */    u16    OUT20_IO        :  1  ;  //���ͨ��20
    /* Bit05 */    u16    OUT21_IO        :  1  ;  //���ͨ��21
    /* Bit06 */    u16    OUT22_IO        :  1  ;  //���ͨ��22
    /* Bit07 */    u16    OUT23_IO        :  1  ;  //���ͨ��23
    /* Bit08 */    u16    OUT24_IO        :  1  ;  //���ͨ��24
    /* Bit09 */    u16    OUT25_IO        :  1  ;  //���ͨ��25
    /* Bit10 */    u16    OUT26_IO        :  1  ;  //���ͨ��26
    /* Bit11 */    u16    OUT27_IO        :  1  ;  //���ͨ��27
    /* Bit12 */    u16    OUT28_IO        :  1  ;  //���ͨ��28
    /* Bit13 */    u16    OUT29_IO        :  1  ;  //���ͨ��29
    /* Bit14 */    u16    OUT30_IO        :  1  ;  //���ͨ��30
    /* Bit15 */    u16    OUT31_IO        :  1  ;  //���ͨ��31
    /* Bit16 */    u16    OUT32_IO        :  1  ;  //���ͨ��32

    /* Bit01 */    u16    OUT33_IO        :  1  ;  //���ͨ��33
    /* Bit02 */    u16    OUT34_IO        :  1  ;  //���ͨ��34
    /* Bit03 */    u16    OUT35_IO        :  1  ;  //���ͨ��35
    /* Bit04 */    u16    OUT36_IO        :  1  ;  //���ͨ��36
    /* Bit05 */    u16    OUT37_IO        :  1  ;  //���ͨ��37
    /* Bit06 */    u16    OUT38_IO        :  1  ;  //���ͨ��38
    /* Bit07 */    u16    OUT39_IO        :  1  ;  //���ͨ��39
    /* Bit08 */    u16    OUT40_IO        :  1  ;  //���ͨ��40
    /* Bit09 */    u16    OUT41_IO        :  1  ;  //���ͨ��41
    /* Bit10 */    u16    OUT42_IO        :  1  ;  //���ͨ��42
    /* Bit11 */    u16    OUT43_IO        :  1  ;  //���ͨ��43
    /* Bit12 */    u16    OUT44_IO        :  1  ;  //���ͨ��44
    /* Bit13 */    u16    OUT45_IO        :  1  ;  //���ͨ��45
    /* Bit14 */    u16    OUT46_IO        :  1  ;  //���ͨ��46
    /* Bit15 */    u16    OUT47_IO        :  1  ;  //���ͨ��47
    /* Bit16 */    u16    OUT48_IO        :  1  ;  //���ͨ��48

	/* Bit01 */    u16    OUT49_IO        :  1  ;  //���ͨ��33
	/* Bit02 */    u16    OUT50_IO        :  1  ;  //���ͨ��34
	/* Bit03 */    u16    OUT51_IO        :  1  ;  //���ͨ��35
	/* Bit04 */    u16    OUT52_IO        :  1  ;  //���ͨ��36
	/* Bit05 */    u16    OUT53_IO        :  1  ;  //���ͨ��37
	/* Bit06 */    u16    OUT54_IO        :  1  ;  //���ͨ��38
	/* Bit07 */    u16    OUT55_IO        :  1  ;  //���ͨ��39
	/* Bit08 */    u16    OUT56_IO        :  1  ;  //���ͨ��40
	/* Bit09 */    u16    OUT57_IO        :  1  ;  //���ͨ��41
	/* Bit10 */    u16    OUT58_IO        :  1  ;  //���ͨ��42
	/* Bit11 */    u16    OUT59_IO        :  1  ;  //���ͨ��43
	/* Bit12 */    u16    OUT60_IO        :  1  ;  //���ͨ��44
	/* Bit13 */    u16    OUT61_IO        :  1  ;  //���ͨ��45
	/* Bit14 */    u16    OUT62_IO        :  1  ;  //���ͨ��46
	/* Bit15 */    u16    OUT63_IO        :  1  ;  //���ͨ��47
	/* Bit16 */    u16    OUT64_IO        :  1  ;  //���ͨ��48

	/* Bit01 */    u16    OUT65_IO        :  1  ;  //���ͨ��33
	/* Bit02 */    u16    OUT66_IO        :  1  ;  //���ͨ��34
	/* Bit03 */    u16    OUT67_IO        :  1  ;  //���ͨ��35
	/* Bit04 */    u16    OUT68_IO        :  1  ;  //���ͨ��36
	/* Bit05 */    u16    OUT69_IO        :  1  ;  //���ͨ��37
	/* Bit06 */    u16    OUT70_IO        :  1  ;  //���ͨ��38
	/* Bit07 */    u16    OUT71_IO        :  1  ;  //���ͨ��39
	/* Bit08 */    u16    OUT72_IO        :  1  ;  //���ͨ��40
	/* Bit09 */    u16    OUT73_IO        :  1  ;  //���ͨ��41
	/* Bit10 */    u16    OUT74_IO        :  1  ;  //���ͨ��42
	/* Bit11 */    u16    OUT75_IO        :  1  ;  //���ͨ��43
	/* Bit12 */    u16    OUT76_IO        :  1  ;  //���ͨ��44
	/* Bit13 */    u16    OUT77_IO        :  1  ;  //���ͨ��45
	/* Bit14 */    u16    OUT78_IO        :  1  ;  //���ͨ��46
	/* Bit15 */    u16    OUT79_IO        :  1  ;  //���ͨ��47
	/* Bit16 */    u16    OUT80_IO        :  1  ;  //���ͨ��48

	/* Bit01 */    u16    OUT81_IO        :  1  ;  //���ͨ��33
	/* Bit02 */    u16    OUT82_IO        :  1  ;  //���ͨ��34
	/* Bit03 */    u16    OUT83_IO        :  1  ;  //���ͨ��35
	/* Bit04 */    u16    OUT84_IO        :  1  ;  //���ͨ��36
	/* Bit05 */    u16    OUT85_IO        :  1  ;  //���ͨ��37
	/* Bit06 */    u16    OUT86_IO        :  1  ;  //���ͨ��38
	/* Bit07 */    u16    OUT87_IO        :  1  ;  //���ͨ��39
	/* Bit08 */    u16    OUT88_IO        :  1  ;  //���ͨ��40
	/* Bit09 */    u16    OUT89_IO        :  1  ;  //���ͨ��41
	/* Bit10 */    u16    OUT90_IO        :  1  ;  //���ͨ��42
	/* Bit11 */    u16    OUT91_IO        :  1  ;  //���ͨ��43
	/* Bit12 */    u16    OUT92_IO        :  1  ;  //���ͨ��44
	/* Bit13 */    u16    OUT93_IO        :  1  ;  //���ͨ��45
	/* Bit14 */    u16    OUT94_IO        :  1  ;  //���ͨ��46
	/* Bit15 */    u16    OUT95_IO        :  1  ;  //���ͨ��47
	/* Bit16 */    u16    OUT96_IO        :  1  ;  //���ͨ��48

	/* Bit01 */    u16    OUT97_IO        :  1  ;  //���ͨ��33
	/* Bit02 */    u16    OUT98_IO        :  1  ;  //���ͨ��34
	/* Bit03 */    u16    OUT99_IO        :  1  ;  //���ͨ��35
	/* Bit04 */    u16    OUT100_IO       :  1  ;  //���ͨ��36
	/* Bit05 */    u16    OUT101_IO       :  1  ;  //���ͨ��37
	/* Bit06 */    u16    OUT102_IO       :  1  ;  //���ͨ��38
	/* Bit07 */    u16    OUT103_IO       :  1  ;  //���ͨ��39
	/* Bit08 */    u16    OUT104_IO       :  1  ;  //���ͨ��40
	/* Bit09 */    u16    OUT105_IO       :  1  ;  //���ͨ��41
	/* Bit10 */    u16    OUT106_IO       :  1  ;  //���ͨ��42
	/* Bit11 */    u16    OUT107_IO       :  1  ;  //���ͨ��43
	/* Bit12 */    u16    OUT108_IO       :  1  ;  //���ͨ��44
	/* Bit13 */    u16    OUT109_IO       :  1  ;  //���ͨ��45
	/* Bit14 */    u16    OUT110_IO       :  1  ;  //���ͨ��46
	/* Bit15 */    u16    OUT111_IO       :  1  ;  //���ͨ��47
	/* Bit16 */    u16    OUT112_IO       :  1  ;  //���ͨ��48  B7

	/* Bit01 */    u16    OUT113_IO       :  1  ;  //���ͨ��33
	/* Bit02 */    u16    OUT114_IO       :  1  ;  //���ͨ��34
	/* Bit03 */    u16    OUT115_IO       :  1  ;  //���ͨ��35
	/* Bit04 */    u16    OUT116_IO       :  1  ;  //���ͨ��36
	/* Bit05 */    u16    OUT117_IO       :  1  ;  //���ͨ��37
	/* Bit06 */    u16    OUT118_IO       :  1  ;  //���ͨ��38
	/* Bit07 */    u16    OUT119_IO       :  1  ;  //���ͨ��39
	/* Bit08 */    u16    OUT120_IO       :  1  ;  //���ͨ��40
	/* Bit09 */    u16    OUT121_IO       :  1  ;  //���ͨ��41
	/* Bit10 */    u16    OUT122_IO       :  1  ;  //���ͨ��42
	/* Bit11 */    u16    OUT123_IO       :  1  ;  //���ͨ��43
	/* Bit12 */    u16    OUT124_IO       :  1  ;  //���ͨ��44
	/* Bit13 */    u16    OUT125_IO       :  1  ;  //���ͨ��45
	/* Bit14 */    u16    OUT126_IO       :  1  ;  //���ͨ��46
	/* Bit15 */    u16    OUT127_IO       :  1  ;  //���ͨ��47
	/* Bit16 */    u16    OUT128_IO       :  1  ;  //���ͨ��48  B8

	/* Bit01 */    u16    OUT129_IO       :  1  ;  //���ͨ��33
	/* Bit02 */    u16    OUT130_IO       :  1  ;  //���ͨ��34
	/* Bit03 */    u16    OUT131_IO       :  1  ;  //���ͨ��35
	/* Bit04 */    u16    OUT132_IO       :  1  ;  //���ͨ��36
	/* Bit05 */    u16    OUT133_IO       :  1  ;  //���ͨ��37
	/* Bit06 */    u16    OUT134_IO       :  1  ;  //���ͨ��38
	/* Bit07 */    u16    OUT135_IO       :  1  ;  //���ͨ��39
	/* Bit08 */    u16    OUT136_IO       :  1  ;  //���ͨ��40
	/* Bit09 */    u16    OUT137_IO       :  1  ;  //���ͨ��41
	/* Bit10 */    u16    OUT138_IO       :  1  ;  //���ͨ��42
	/* Bit11 */    u16    OUT139_IO       :  1  ;  //���ͨ��43
	/* Bit12 */    u16    OUT140_IO       :  1  ;  //���ͨ��44
	/* Bit13 */    u16    OUT141_IO       :  1  ;  //���ͨ��45
	/* Bit14 */    u16    OUT142_IO       :  1  ;  //���ͨ��46
	/* Bit15 */    u16    OUT143_IO       :  1  ;  //���ͨ��47
	/* Bit16 */    u16    OUT144_IO       :  1  ;  //���ͨ��48  B9

	/* Bit01 */    u16    OUT145_IO       :  1  ;  //���ͨ��33
	/* Bit02 */    u16    OUT146_IO       :  1  ;  //���ͨ��34
	/* Bit03 */    u16    OUT147_IO       :  1  ;  //���ͨ��35
	/* Bit04 */    u16    OUT148_IO       :  1  ;  //���ͨ��36
	/* Bit05 */    u16    OUT149_IO       :  1  ;  //���ͨ��37
	/* Bit06 */    u16    OUT150_IO       :  1  ;  //���ͨ��38
	/* Bit07 */    u16    OUT151_IO       :  1  ;  //���ͨ��39
	/* Bit08 */    u16    OUT152_IO       :  1  ;  //���ͨ��40
	/* Bit09 */    u16    OUT153_IO       :  1  ;  //���ͨ��41
	/* Bit10 */    u16    OUT154_IO       :  1  ;  //���ͨ��42
	/* Bit11 */    u16    OUT155_IO       :  1  ;  //���ͨ��43
	/* Bit12 */    u16    OUT156_IO       :  1  ;  //���ͨ��44
	/* Bit13 */    u16    OUT157_IO       :  1  ;  //���ͨ��45
	/* Bit14 */    u16    OUT158_IO       :  1  ;  //���ͨ��46
	/* Bit15 */    u16    OUT159_IO       :  1  ;  //���ͨ��47
	/* Bit16 */    u16    OUT160_IO       :  1  ;  //���ͨ��48  B10

	/* Bit01 */    u16    OUT161_IO       :  1  ;  //���ͨ��33
	/* Bit02 */    u16    OUT162_IO       :  1  ;  //���ͨ��34
	/* Bit03 */    u16    OUT163_IO       :  1  ;  //���ͨ��35
	/* Bit04 */    u16    OUT164_IO       :  1  ;  //���ͨ��36
	/* Bit05 */    u16    OUT165_IO       :  1  ;  //���ͨ��37
	/* Bit06 */    u16    OUT166_IO       :  1  ;  //���ͨ��38
	/* Bit07 */    u16    OUT167_IO       :  1  ;  //���ͨ��39
	/* Bit08 */    u16    OUT168_IO       :  1  ;  //���ͨ��40
	/* Bit09 */    u16    OUT169_IO       :  1  ;  //���ͨ��41
	/* Bit10 */    u16    OUT170_IO       :  1  ;  //���ͨ��42
	/* Bit11 */    u16    OUT171_IO       :  1  ;  //���ͨ��43
	/* Bit12 */    u16    OUT172_IO       :  1  ;  //���ͨ��44
	/* Bit13 */    u16    OUT173_IO       :  1  ;  //���ͨ��45
	/* Bit14 */    u16    OUT174_IO       :  1  ;  //���ͨ��46
	/* Bit15 */    u16    OUT175_IO       :  1  ;  //���ͨ��47
	/* Bit16 */    u16    OUT176_IO       :  1  ;  //���ͨ��48  B11

	/* Bit01 */    u16    OUT177_IO       :  1  ;  //���ͨ��33
	/* Bit02 */    u16    OUT178_IO       :  1  ;  //���ͨ��34
	/* Bit03 */    u16    OUT179_IO       :  1  ;  //���ͨ��35
	/* Bit04 */    u16    OUT180_IO       :  1  ;  //���ͨ��36
	/* Bit05 */    u16    OUT181_IO       :  1  ;  //���ͨ��37
	/* Bit06 */    u16    OUT182_IO       :  1  ;  //���ͨ��38
	/* Bit07 */    u16    OUT183_IO       :  1  ;  //���ͨ��39
	/* Bit08 */    u16    OUT184_IO       :  1  ;  //���ͨ��40
	/* Bit09 */    u16    OUT185_IO       :  1  ;  //���ͨ��41
	/* Bit10 */    u16    OUT186_IO       :  1  ;  //���ͨ��42
	/* Bit11 */    u16    OUT187_IO       :  1  ;  //���ͨ��43
	/* Bit12 */    u16    OUT188_IO       :  1  ;  //���ͨ��44
	/* Bit13 */    u16    OUT189_IO       :  1  ;  //���ͨ��45
	/* Bit14 */    u16    OUT190_IO       :  1  ;  //���ͨ��46
	/* Bit15 */    u16    OUT191_IO       :  1  ;  //���ͨ��47
	/* Bit16 */    u16    OUT192_IO       :  1  ;  //���ͨ��48  B12

	/* Bit01 */    u16    OUT193_IO       :  1  ;  //���ͨ��33
	/* Bit02 */    u16    OUT194_IO       :  1  ;  //���ͨ��34
	/* Bit03 */    u16    OUT195_IO       :  1  ;  //���ͨ��35
	/* Bit04 */    u16    OUT196_IO       :  1  ;  //���ͨ��36
	/* Bit05 */    u16    OUT197_IO       :  1  ;  //���ͨ��37
	/* Bit06 */    u16    OUT198_IO       :  1  ;  //���ͨ��38
	/* Bit07 */    u16    OUT199_IO       :  1  ;  //���ͨ��39
	/* Bit08 */    u16    OUT200_IO       :  1  ;  //���ͨ��40
	/* Bit09 */    u16    OUT201_IO       :  1  ;  //���ͨ��41
	/* Bit10 */    u16    OUT202_IO       :  1  ;  //���ͨ��42
	/* Bit11 */    u16    OUT203_IO       :  1  ;  //���ͨ��43
	/* Bit12 */    u16    OUT204_IO       :  1  ;  //���ͨ��44
	/* Bit13 */    u16    OUT205_IO       :  1  ;  //���ͨ��45
	/* Bit14 */    u16    OUT206_IO       :  1  ;  //���ͨ��46
	/* Bit15 */    u16    OUT207_IO       :  1  ;  //���ͨ��47
	/* Bit16 */    u16    OUT208_IO       :  1  ;  //���ͨ��48  B13

	/* Bit01 */    u16    OUT209_IO       :  1  ;  //���ͨ��33
	/* Bit02 */    u16    OUT210_IO       :  1  ;  //���ͨ��34
	/* Bit03 */    u16    OUT211_IO       :  1  ;  //���ͨ��35
	/* Bit04 */    u16    OUT212_IO       :  1  ;  //���ͨ��36
	/* Bit05 */    u16    OUT213_IO       :  1  ;  //���ͨ��37
	/* Bit06 */    u16    OUT214_IO       :  1  ;  //���ͨ��38
	/* Bit07 */    u16    OUT215_IO       :  1  ;  //���ͨ��39
	/* Bit08 */    u16    OUT216_IO       :  1  ;  //���ͨ��40
	/* Bit09 */    u16    OUT217_IO       :  1  ;  //���ͨ��41
	/* Bit10 */    u16    OUT218_IO       :  1  ;  //���ͨ��42
	/* Bit11 */    u16    OUT219_IO       :  1  ;  //���ͨ��43
	/* Bit12 */    u16    OUT220_IO       :  1  ;  //���ͨ��44
	/* Bit13 */    u16    OUT221_IO       :  1  ;  //���ͨ��45
	/* Bit14 */    u16    OUT222_IO       :  1  ;  //���ͨ��46
	/* Bit15 */    u16    OUT223_IO       :  1  ;  //���ͨ��47
	/* Bit16 */    u16    OUT224_IO       :  1  ;  //���ͨ��48  B14


	// Total : 28 Bytes

}DS_IO_OUT, *PDS_IO_OUT;

//------------------------------------------------------------------------------
//  AD_IN  ����ӳ��ӿ�
//------------------------------------------------------------------------------
typedef struct _DS_AD_IN_
{
	/* Input 01 */ 	u16  inVal01_AD;	//ģ��������ͨ�� 01
	/* Input 02 */	u16  inVal02_AD;	//ģ��������ͨ�� 02
	/* Input 03 */	u16  inVal03_AD;	//ģ��������ͨ�� 03
	/* Input 04 */	u16  inVal04_AD;	//ģ��������ͨ�� 04
	/* Input 05 */	u16  inVal05_AD;	//ģ��������ͨ�� 05
	/* Input 06 */	u16  inVal06_AD;	//ģ��������ͨ�� 06
	/* Input 07 */	u16  inVal07_AD;	//ģ��������ͨ�� 07
	/* Input 08 */	u16  inVal08_AD;	//ģ��������ͨ�� 08
	/* Input 09 */	u16  inVal09_AD;	//ģ��������ͨ�� 09
	/* Input 10 */	u16  inVal10_AD;	//ģ��������ͨ�� 10
	/* Input 11 */	u16  inVal11_AD;	//ģ��������ͨ�� 11
	/* Input 12 */	u16  inVal12_AD;	//ģ��������ͨ�� 12
	/* Input 13 */	u16  inVal13_AD;	//ģ��������ͨ�� 13
	/* Input 14 */	u16  inVal14_AD;	//ģ��������ͨ�� 14
	/* Input 15 */	u16  inVal15_AD;	//ģ��������ͨ�� 15
	/* Input 16 */	u16  inVal16_AD;	//ģ��������ͨ�� 16

}DS_AD_IN, *PDS_AD_IN;

/*------------------------------- ��������� ---------------------------------------*/
typedef struct _DS_SOFT_CNT_
{
	void (*pInitHook)();
	void (*pRunHook)();
	void (*pSetCntHook)(PSTIM_InitTypeDef pstim_init);
	void (*pClearCntHook)(u16 cntIndex);
	u32  (*pGetCntValHook)(u16 cntIndex);
	void (*pSetCntValHook)(u16 cntIndex,u32 cntValue);
	u8   Initialized;
	u32  IndexMask[8];   //32*8 = 256bit,  ��Ӧ256��CNT��ʹ��״̬
}DS_SOFT_CNT, *PDS_SOFT_CNT;

#endif

//----------------------------------------------------
// �汾��ϵͳ��Ϣ
//-----------------------------------------------------
typedef struct _DS_BSP_Info_
{
	u32     CPLD_Ver;
	u32     BSP_Ver;
	u32     APP_Ver;
	u32     PCB_Ver;
	u32     RCC_CSR_REG;
	u32     SystemCoreClock;
	u32     ARM_TEMP;
	u32     BoardId;                 //���λ�� 
	u32     DeviceNew;               //�¾ɻ���ʶ��

	u32     Cryst_Ex    :  1  ;      //�ⲿ��ʱ������ʱ��Դ 
	u32     IWDG_RST    :  1  ;      //�������Ź���λ 
	u32     PROWER_RST  :  1  ;      //�ϵ縴λ��־
	u32     ExFlash     :  1  ;      //�ⲿFlash
	u32     res1        :  28 ;
	u32     RS485_1     :  16 ;      //RS485 1ͨѶ״̬ 
	u32     RS485_2     :  16 ;      //RS485 2ͨѶ״̬ 
	u32     COMM_CAN1   :  16 ;      //CAN1ͨѶ״̬ 
	u32     COMM_CAN2   :  16 ;      //CAN2ͨѶ״̬ 
    u32     CanBoardCom;             //��������CANͨ�ŵƿ�
    u32     CanBoardComFault;        //��������CANͨ��Fault Flag
}DS_BSP_INFO, *PDS_BSP_INFO;

#define INDEX_BOARD_A  (1)
#define INDEX_BOARD_B  (2)
#define INDEX_BOARD_C  (3)
#define INDEX_BOARD_D  (4)
#define INDEX_BOARD_E  (5)
#define INDEX_BOARD_F  (6)
#define INDEX_BOARD_G  (7)
#define INDEX_BOARD_H  (8)



//----------------------------------------------------
// CPUռ����
//-----------------------------------------------------
typedef struct _DS_CPU_USE_
{
	u8      cpu_usage_major;
	u8      cpu_usage_minor;

}DS_CPU_USE, *PDS_CPU_USE;

//----------------------------------------------------
// PWM�ɼ����
//-----------------------------------------------------
typedef struct _DS_PWM_IN_
{
	u32 Frequency;     //Ƶ��
	u16 DutyCycle;     //ռ�ձ�
	u16 res;

}DS_PWM_IN, *PDS_PWM_IN;

//----------------------------------------------------
// ��������ִ��ʱ��
//-----------------------------------------------------
typedef struct _DS_TASK_TIME_
{
	u32 sys_task;    
	u32 mvb_task;    
	u16 res;

}DS_TASK_TIME, *PDS_TASK_TIME;


/*------------------------------- Flash ���� ---------------------------------------*/
typedef struct _flash_buffer_
{
	u16 Info_type;
	u16 size;
	u16 Number;
	u8* payload;
}FBuff, *pFBuff;

typedef struct _DS_Flash_Operation_
{
	u16  initalized;
	int  (*pRead)(pFBuff fBuf);
	void (*pWrite)(pFBuff fBuf);
}DS_Flash_Operation, *PDS_Flash_Operation;

//------------------------------------------------------------------------
// Flash��������
//------------------------------------------------------------------------
#define FLASH_READ_AD          (0x01)
#define FLASH_READ_SYS	       (0x02)
#define FLASH_READ_LOG		   (0x03)
#define FLASH_READ_ERR		   (0x04)
#define FLASH_WRITE_AD         (0x11)
#define FLASH_WRITE_SYS  	   (0x12)
#define FLASH_WRITE_LOG		   (0x13)
#define FLASH_WRITE_ERR	       (0x14)
#define FLASH_NoRead           (0x00)

//------------------------------------------------------------------------
// LED״̬
//------------------------------------------------------------------------
#define LED_OK                 (0)  //����
#define LED_FAULT              (1)  //��ͨ����

typedef struct _DS_LED_MANAGE_
{
	/* Bit01 */	uint16_t	RS232CommStatus    : 1 ;  // RS232   ͨ��״̬
	/* Bit02 */	uint16_t	RS485Comm1Status   : 1 ;  // RS485_1 ͨ��״̬
	/* Bit03 */	uint16_t	RS485Comm2Status   : 1 ;  // RS485_2 ͨ��״̬
	/* Bit04 */	uint16_t	CAN1CommStatus     : 1 ;  // CAN1    ͨѶ״̬
	/* Bit05 */	uint16_t	CAN2CommStatus     : 1 ;  // CAN2    ͨ��״̬
	/*       */	uint16_t	Res01              : 11;  // 

}DS_LED_MANAGE, *PDS_LED_MANAGE;

#define LED_DRV_CNT_MAX (10)
//Led������Ԫ
typedef struct _DS_Drv_Led_
{
    const char *name;
    void (*Led_On)(void);
    void (*Led_Off)(void); 
    
}DS_DRV_LED, *DS_DRV_LED_T;

//Led��������
typedef struct _DS_DrvMan_Led_
{
    DS_DRV_LED  Leds[LED_DRV_CNT_MAX];
    
}DS_DRVMAN_LED,*DS_DRVMAN_LED_T;

//-----------------------------------------------------------------
// PTUͨ�Žӿ� 
//-----------------------------------------------------------------
typedef struct _ptuLinkLayer_
{
	void     (*pRx)(__IO uint8_t* pbuf, uint16_t len);
	uint16_t (*pTx)(__IO uint8_t* pbuf);
}PTU_Link, *PPTU_Link;


//------------------------------------------------------------------------
// �Ƿ������Ź���1ʹ�ܣ�0����
//------------------------------------------------------------------------
#define   IWDG_EN              (1)

/*=============================================================================*/
/*******************************************************************************
*                                                                              *
*                       �������ݼ�  (Drive Layer)                              *
*                                                                              *
*                                                                              *
********************************************************************************/
/*=============================================================================*/

typedef struct _DS_A_
{
	DS_KT_INPUT        theDS_KT_INPUT;         //ʱ��̵���������ͨ��
	DS_KT_OUTPUT       theDS_KT_OUTPUT;        //ʱ��̵��������ͨ��
	DS_SOFT_CNT        theDS_SOFT_CNT;
	DS_CNT             theDS_CNT;
	DS_BSP_INFO        theDS_BI;
	DS_Flash_Operation theDS_FOP;
	DS_CPU_USE         theDS_CPU;
	DS_IO_MODE         theDS_IO_MODE;
	DS_IO_IN           theDS_IO_IN;
	DS_IO_OUT          theDS_IO_OUT;
	DS_CAN_IO_IN       theDS_CAN_IN;
	DS_CAN_IO_OUT      theDS_CAN_OUT;
	DS_PWM_IN          theDS_PWM_IN;
	DS_LED_MANAGE      theDS_LED;
	DS_TASK_TIME       theDS_TASK;
	DS_AD_IN           theDS_AD_IN;
	PTU_Link           theDS_PTU_Link;
    DS_DRVMAN_LED      theDS_DML;              //LED����������
    DS_RNET_IN         theDS_RNIN;
    DS_RNET_OUT        theDS_RNOU;

}DS_A, *PDS_A;

/*=============================================================================*/

/*******************************************************************************
*                                                                              *
*                       �������ݼ�: '����'�궨��                               *
*                                                                              *
*                                                                              *
********************************************************************************/
/*=============================================================================*/

#define TDS(name)             (PDS_##name)
#define PDS(name)             (&(theDSA.theDS_##name))
#define SDS(name)             (sizeof(DS_##name))
#define CopyDS(name, source)  memcpy(&(theDSA.theDS_##name),source,sizeof(DS_##name))



/*=============================================================================*/
/*******************************************************************************
*                                                                              *
*                       �������ݼ�: �ṹ���������                             *
*                                                                              *
*                                                                              *
********************************************************************************/
/*=============================================================================*/

#ifdef DSA_MACRO
    DS_A	theDSA = { 0 };	
#else
    extern	DS_A	theDSA;
#endif

#endif




/******************* (C) COPYRIGHT 2012 Shenzhen Tongye *****END OF FILE****/

