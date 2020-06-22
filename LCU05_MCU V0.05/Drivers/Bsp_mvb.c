

#include "Bsp_mvb.h"
#include "userApp.h"


static MVB_FAULT_STU mvb_flt = { 0 };
static MVB_STS_STU mvb_sts = { 0 };


void mvb_flt_data(uint8_t* buf)
{
    /* 电源板故障 */
    mvb_flt.pwr_a_110v = pKW_SHM->me.pwr.dc110v_a;
    mvb_flt.pwr_b_110v = pKW_SHM->me.pwr.dc110v_b;
    mvb_flt.pwr_c_110v = pKW_SHM->me.pwr.dc110v_c;
    
    mvb_flt.pwr_a_5v = pKW_SHM->me.pwr.dc5v_a;
    mvb_flt.pwr_b_5v = pKW_SHM->me.pwr.dc5v_b;
    mvb_flt.pwr_c_5v = pKW_SHM->me.pwr.dc5v_c;
    
    /* 故障等级 */
    mvb_flt.lcu_flt_ser = pKW_SHM->me.fltLevel.ser;
    mvb_flt.lcu_flt_med = pKW_SHM->me.fltLevel.med;
    mvb_flt.lcu_flt_sli = pKW_SHM->me.fltLevel.sli;
    
    /* 软件版本号不一致 */
    mvb_flt.mcu_ver = pKW_SHM->me.verFlt.mcuVer;
    mvb_flt.mcu_kw_ver = pKW_SHM->me.verFlt.kwVer;
    mvb_flt.dio_ver = pKW_SHM->me.verFlt.dioVer;
    
    /* CAN网关板 */
    mvb_flt.can_flt = pKW_SHM->me.can.board;
    mvb_flt.can_can1 = pKW_SHM->me.can.can1;
    mvb_flt.can_can2 = pKW_SHM->me.can.can2;
    mvb_flt.can_lost = pKW_SHM->me.can.lost;
    mvb_flt.can_ext_can1 = pKW_SHM->me.can.ext_can1;
    mvb_flt.can_ext_can2 = pKW_SHM->me.can.ext_can2;
    
    /* ETU板 */
    mvb_flt.etu_flt = pKW_SHM->me.etu.board;
    mvb_flt.etu_can1 = pKW_SHM->me.etu.can1;
    mvb_flt.etu_can1 = pKW_SHM->me.etu.can1;
    mvb_flt.etu_lost = pKW_SHM->me.etu.lost;
    mvb_flt.etu_eth = pKW_SHM->me.etu.eth;
    
    /* MVB板 */
    mvb_flt.mvb_flt = pKW_SHM->me.mvb.board;
    mvb_flt.mvb_can1 = pKW_SHM->me.mvb.can1;
    mvb_flt.mvb_can1 = pKW_SHM->me.mvb.can1;

    /* 主控板A */
    mvb_flt.mcu_a_flt = pKW_SHM->me.mcu[0].board;
    mvb_flt.mcu_a_can1 = pKW_SHM->me.mcu[0].can1;
    mvb_flt.mcu_a_can2 = pKW_SHM->me.mcu[0].can2;
    mvb_flt.mcu_a_lost = pKW_SHM->me.mcu[0].lost;
    mvb_flt.mcu_a_ou = 0;
    
    /* 主控板B */
    mvb_flt.mcu_b_flt = pKW_SHM->me.mcu[1].board;
    mvb_flt.mcu_b_can1 = pKW_SHM->me.mcu[1].can1;
    mvb_flt.mcu_b_can2 = pKW_SHM->me.mcu[1].can2;
    mvb_flt.mcu_b_lost = pKW_SHM->me.mcu[1].lost;
    mvb_flt.mcu_b_ou = 0;
    
    /* 主控板C */
    mvb_flt.mcu_c_flt = pKW_SHM->me.mcu[2].board;
    mvb_flt.mcu_c_can1 = pKW_SHM->me.mcu[2].can1;
    mvb_flt.mcu_c_can2 = pKW_SHM->me.mcu[2].can2;
    mvb_flt.mcu_c_lost = pKW_SHM->me.mcu[2].lost;
    mvb_flt.mcu_c_ou = 0;

    /* 第一组DIO板 */
    mvb_flt.io1_a_flt = pKW_SHM->me.dio[0].board;
    mvb_flt.io1_a_can1 = pKW_SHM->me.dio[0].can1;
    mvb_flt.io1_a_can2 = pKW_SHM->me.dio[0].can2;
    mvb_flt.io1_a_in = pKW_SHM->me.dio[0].in;
    mvb_flt.io1_a_ou = pKW_SHM->me.dio[0].ou;
    mvb_flt.io1_a_lost = pKW_SHM->me.dio[0].lost;

    mvb_flt.io1_b_flt = pKW_SHM->me.dio[1].board;
    mvb_flt.io1_b_can1 = pKW_SHM->me.dio[1].can1;
    mvb_flt.io1_b_can2 = pKW_SHM->me.dio[1].can2;
    mvb_flt.io1_b_in = pKW_SHM->me.dio[1].in;
    mvb_flt.io1_b_ou = pKW_SHM->me.dio[1].ou;
    mvb_flt.io1_b_lost = pKW_SHM->me.dio[1].lost;
    
    mvb_flt.io1_c_flt = pKW_SHM->me.dio[2].board;
    mvb_flt.io1_c_can1 = pKW_SHM->me.dio[2].can1;
    mvb_flt.io1_c_can2 = pKW_SHM->me.dio[2].can2;
    mvb_flt.io1_c_in = pKW_SHM->me.dio[2].in;
    mvb_flt.io1_c_ou = pKW_SHM->me.dio[2].ou;
    mvb_flt.io1_c_lost = pKW_SHM->me.dio[2].lost;
    
    /* 第二组DIO板 */
    mvb_flt.io2_a_flt = pKW_SHM->me.dio[3].board;
    mvb_flt.io2_a_can1 = pKW_SHM->me.dio[3].can1;
    mvb_flt.io2_a_can2 = pKW_SHM->me.dio[3].can2;
    mvb_flt.io2_a_in = pKW_SHM->me.dio[3].in;
    mvb_flt.io2_a_ou = pKW_SHM->me.dio[3].ou;
    mvb_flt.io2_a_lost = pKW_SHM->me.dio[3].lost;

    mvb_flt.io2_b_flt = pKW_SHM->me.dio[4].board;
    mvb_flt.io2_b_can1 = pKW_SHM->me.dio[4].can1;
    mvb_flt.io2_b_can2 = pKW_SHM->me.dio[4].can2;
    mvb_flt.io2_b_in = pKW_SHM->me.dio[4].in;
    mvb_flt.io2_b_ou = pKW_SHM->me.dio[4].ou;
    mvb_flt.io2_b_lost = pKW_SHM->me.dio[4].lost;
    
    mvb_flt.io2_c_flt = pKW_SHM->me.dio[5].board;
    mvb_flt.io2_c_can1 = pKW_SHM->me.dio[5].can1;
    mvb_flt.io2_c_can2 = pKW_SHM->me.dio[5].can2;
    mvb_flt.io2_c_in = pKW_SHM->me.dio[5].in;
    mvb_flt.io2_c_ou = pKW_SHM->me.dio[5].ou;
    mvb_flt.io2_c_lost = pKW_SHM->me.dio[5].lost;
    
    /* 第三组DIO板 */
    mvb_flt.io3_a_flt = pKW_SHM->me.dio[6].board;
    mvb_flt.io3_a_can1 = pKW_SHM->me.dio[6].can1;
    mvb_flt.io3_a_can2 = pKW_SHM->me.dio[6].can2;
    mvb_flt.io3_a_in = pKW_SHM->me.dio[6].in;
    mvb_flt.io3_a_ou = pKW_SHM->me.dio[6].ou;
    mvb_flt.io3_a_lost = pKW_SHM->me.dio[6].lost;

    mvb_flt.io3_b_flt = pKW_SHM->me.dio[7].board;
    mvb_flt.io3_b_can1 = pKW_SHM->me.dio[7].can1;
    mvb_flt.io3_b_can2 = pKW_SHM->me.dio[7].can2;
    mvb_flt.io3_b_in = pKW_SHM->me.dio[7].in;
    mvb_flt.io3_b_ou = pKW_SHM->me.dio[7].ou;
    mvb_flt.io3_b_lost = pKW_SHM->me.dio[7].lost;
    
    mvb_flt.io3_c_flt = pKW_SHM->me.dio[8].board;
    mvb_flt.io3_c_can1 = pKW_SHM->me.dio[8].can1;
    mvb_flt.io3_c_can2 = pKW_SHM->me.dio[8].can2;
    mvb_flt.io3_c_in = pKW_SHM->me.dio[8].in;
    mvb_flt.io3_c_ou = pKW_SHM->me.dio[8].ou;
    mvb_flt.io3_c_lost = pKW_SHM->me.dio[8].lost;
    
    /* 第四组DIO板 */
    mvb_flt.io4_a_flt = pKW_SHM->me.dio[9].board;
    mvb_flt.io4_a_can1 = pKW_SHM->me.dio[9].can1;
    mvb_flt.io4_a_can2 = pKW_SHM->me.dio[9].can2;
    mvb_flt.io4_a_in = pKW_SHM->me.dio[9].in;
    mvb_flt.io4_a_ou = pKW_SHM->me.dio[9].ou;
    mvb_flt.io4_a_lost = pKW_SHM->me.dio[9].lost;

    mvb_flt.io4_b_flt = pKW_SHM->me.dio[10].board;
    mvb_flt.io4_b_can1 = pKW_SHM->me.dio[10].can1;
    mvb_flt.io4_b_can2 = pKW_SHM->me.dio[10].can2;
    mvb_flt.io4_b_in = pKW_SHM->me.dio[10].in;
    mvb_flt.io4_b_ou = pKW_SHM->me.dio[10].ou;
    mvb_flt.io4_b_lost = pKW_SHM->me.dio[10].lost;
    
    mvb_flt.io4_c_flt = pKW_SHM->me.dio[11].board;
    mvb_flt.io4_c_can1 = pKW_SHM->me.dio[11].can1;
    mvb_flt.io4_c_can2 = pKW_SHM->me.dio[11].can2;
    mvb_flt.io4_c_in = pKW_SHM->me.dio[11].in;
    mvb_flt.io4_c_ou = pKW_SHM->me.dio[11].ou;
    mvb_flt.io4_c_lost = pKW_SHM->me.dio[11].lost;
    
    /* 第五组DIO板 */
    mvb_flt.io5_a_flt = pKW_SHM->me.dio[12].board;
    mvb_flt.io5_a_can1 = pKW_SHM->me.dio[12].can1;
    mvb_flt.io5_a_can2 = pKW_SHM->me.dio[12].can2;
    mvb_flt.io5_a_in = pKW_SHM->me.dio[12].in;
    mvb_flt.io5_a_ou = pKW_SHM->me.dio[12].ou;
    mvb_flt.io5_a_lost = pKW_SHM->me.dio[12].lost;

    mvb_flt.io5_b_flt = pKW_SHM->me.dio[13].board;
    mvb_flt.io5_b_can1 = pKW_SHM->me.dio[13].can1;
    mvb_flt.io5_b_can2 = pKW_SHM->me.dio[13].can2;
    mvb_flt.io5_b_in = pKW_SHM->me.dio[13].in;
    mvb_flt.io5_b_ou = pKW_SHM->me.dio[13].ou;
    mvb_flt.io5_b_lost = pKW_SHM->me.dio[13].lost;
    
    mvb_flt.io5_c_flt = pKW_SHM->me.dio[14].board;
    mvb_flt.io5_c_can1 = pKW_SHM->me.dio[14].can1;
    mvb_flt.io5_c_can2 = pKW_SHM->me.dio[14].can2;
    mvb_flt.io5_c_in = pKW_SHM->me.dio[14].in;
    mvb_flt.io5_c_ou = pKW_SHM->me.dio[14].ou;
    mvb_flt.io5_c_lost = pKW_SHM->me.dio[14].lost;
    
    /* 第六组DIO板 */
    mvb_flt.io6_a_flt = pKW_SHM->me.dio[15].board;
    mvb_flt.io6_a_can1 = pKW_SHM->me.dio[15].can1;
    mvb_flt.io6_a_can2 = pKW_SHM->me.dio[15].can2;
    mvb_flt.io6_a_in = pKW_SHM->me.dio[15].in;
    mvb_flt.io6_a_ou = pKW_SHM->me.dio[15].ou;
    mvb_flt.io6_a_lost = pKW_SHM->me.dio[15].lost;
    
    mvb_flt.io6_b_flt = pKW_SHM->me.dio[16].board;
    mvb_flt.io6_b_can1 = pKW_SHM->me.dio[16].can1;
    mvb_flt.io6_b_can2 = pKW_SHM->me.dio[16].can2;
    mvb_flt.io6_b_in = pKW_SHM->me.dio[16].in;
    mvb_flt.io6_b_ou = pKW_SHM->me.dio[16].ou;
    mvb_flt.io6_b_lost = pKW_SHM->me.dio[16].lost;
    
    mvb_flt.io6_c_flt = pKW_SHM->me.dio[17].board;
    mvb_flt.io6_c_can1 = pKW_SHM->me.dio[17].can1;
    mvb_flt.io6_c_can2 = pKW_SHM->me.dio[17].can2;
    mvb_flt.io6_c_in = pKW_SHM->me.dio[17].in;
    mvb_flt.io6_c_ou = pKW_SHM->me.dio[17].ou;
    mvb_flt.io6_c_lost = pKW_SHM->me.dio[17].lost;
    
    /* 第七组DIO板 */
    mvb_flt.io7_a_flt = pKW_SHM->me.dio[18].board;
    mvb_flt.io7_a_can1 = pKW_SHM->me.dio[18].can1;
    mvb_flt.io7_a_can2 = pKW_SHM->me.dio[18].can2;
    mvb_flt.io7_a_in = pKW_SHM->me.dio[18].in;
    mvb_flt.io7_a_ou = pKW_SHM->me.dio[18].ou;
    mvb_flt.io7_a_lost = pKW_SHM->me.dio[18].lost;

    mvb_flt.io7_b_flt = pKW_SHM->me.dio[19].board;
    mvb_flt.io7_b_can1 = pKW_SHM->me.dio[19].can1;
    mvb_flt.io7_b_can2 = pKW_SHM->me.dio[19].can2;
    mvb_flt.io7_b_in = pKW_SHM->me.dio[19].in;
    mvb_flt.io7_b_ou = pKW_SHM->me.dio[19].ou;
    mvb_flt.io7_b_lost = pKW_SHM->me.dio[19].lost;
    
    mvb_flt.io7_c_flt = pKW_SHM->me.dio[20].board;
    mvb_flt.io7_c_can1 = pKW_SHM->me.dio[20].can1;
    mvb_flt.io7_c_can2 = pKW_SHM->me.dio[20].can2;
    mvb_flt.io7_c_in = pKW_SHM->me.dio[20].in;
    mvb_flt.io7_c_ou = pKW_SHM->me.dio[20].ou;
    mvb_flt.io7_c_lost = pKW_SHM->me.dio[20].lost;
    
    
    /* 第三组DIO板 */
    mvb_flt.io8_a_flt = pKW_SHM->me.dio[21].board;
    mvb_flt.io8_a_can1 = pKW_SHM->me.dio[21].can1;
    mvb_flt.io8_a_can2 = pKW_SHM->me.dio[21].can2;
    mvb_flt.io8_a_in = pKW_SHM->me.dio[21].in;
    mvb_flt.io8_a_ou = pKW_SHM->me.dio[21].ou;
    mvb_flt.io8_a_lost = pKW_SHM->me.dio[21].lost;

    mvb_flt.io8_b_flt = pKW_SHM->me.dio[22].board;
    mvb_flt.io8_b_can1 = pKW_SHM->me.dio[22].can1;
    mvb_flt.io8_b_can2 = pKW_SHM->me.dio[22].can2;
    mvb_flt.io8_b_in = pKW_SHM->me.dio[22].in;
    mvb_flt.io8_b_ou = pKW_SHM->me.dio[22].ou;
    mvb_flt.io8_b_lost = pKW_SHM->me.dio[22].lost;
    
    mvb_flt.io8_c_flt = pKW_SHM->me.dio[23].board;
    mvb_flt.io8_c_can1 = pKW_SHM->me.dio[23].can1;
    mvb_flt.io8_c_can2 = pKW_SHM->me.dio[23].can2;
    mvb_flt.io8_c_in = pKW_SHM->me.dio[23].in;
    mvb_flt.io8_c_ou = pKW_SHM->me.dio[23].ou;
    mvb_flt.io8_c_lost = pKW_SHM->me.dio[23].lost;
    
    /* 第九组DIO板 */
    mvb_flt.io9_a_flt = pKW_SHM->me.dio[24].board;
    mvb_flt.io9_a_can1 = pKW_SHM->me.dio[24].can1;
    mvb_flt.io9_a_can2 = pKW_SHM->me.dio[24].can2;
    mvb_flt.io9_a_in = pKW_SHM->me.dio[24].in;
    mvb_flt.io9_a_ou = pKW_SHM->me.dio[24].ou;
    mvb_flt.io9_a_lost = pKW_SHM->me.dio[24].lost;

    mvb_flt.io9_b_flt = pKW_SHM->me.dio[25].board;
    mvb_flt.io9_b_can1 = pKW_SHM->me.dio[25].can1;
    mvb_flt.io9_b_can2 = pKW_SHM->me.dio[25].can2;
    mvb_flt.io9_b_in = pKW_SHM->me.dio[25].in;
    mvb_flt.io9_b_ou = pKW_SHM->me.dio[25].ou;
    mvb_flt.io9_b_lost = pKW_SHM->me.dio[25].lost;
    
    mvb_flt.io9_c_flt = pKW_SHM->me.dio[26].board;
    mvb_flt.io9_c_can1 = pKW_SHM->me.dio[26].can1;
    mvb_flt.io9_c_can2 = pKW_SHM->me.dio[26].can2;
    mvb_flt.io9_c_in = pKW_SHM->me.dio[26].in;
    mvb_flt.io9_c_ou = pKW_SHM->me.dio[26].ou;
    mvb_flt.io9_c_lost = pKW_SHM->me.dio[26].lost;
    
    rt_memcpy(buf, &mvb_flt, 32);
}


void mvb_sts_data(uint8_t* buf)
{
    static uint16_t lifesign = 0;
    
    mvb_sts.inBuf[0] = (ds.inBuf[0] << 8) + ds.inBuf[1];
    mvb_sts.inBuf[1] = (ds.inBuf[2] << 8) + ds.inBuf[3];
    mvb_sts.inBuf[2] = (ds.inBuf[4] << 8) + ds.inBuf[5];
    mvb_sts.inBuf[3] = (ds.inBuf[6] << 8) + ds.inBuf[7];
    mvb_sts.inBuf[4] = (ds.inBuf[8] << 8) + ds.inBuf[9];
    mvb_sts.inBuf[5] = (ds.inBuf[10] << 8) + ds.inBuf[11];
    
    /* 第1组DIO板输出 */
    mvb_sts.ou1 = (uint16_t)ds.Bits_ouBuf[0];
    mvb_sts.ou2 = (uint16_t)ds.Bits_ouBuf[1];
    mvb_sts.ou3 = (uint16_t)ds.Bits_ouBuf[2];
    mvb_sts.ou4 = (uint16_t)ds.Bits_ouBuf[3];
    mvb_sts.ou5 = (uint16_t)ds.Bits_ouBuf[4];
    mvb_sts.ou6 = (uint16_t)ds.Bits_ouBuf[5];
    mvb_sts.sw1 = (uint16_t)ds.Bits_ouBuf[6];
    
    /* 第2组DIO板输出 */
    mvb_sts.ou7 = (uint16_t)ds.Bits_ouBuf[7];
    mvb_sts.ou8 = (uint16_t)ds.Bits_ouBuf[8];
    mvb_sts.ou9 = (uint16_t)ds.Bits_ouBuf[9];
    mvb_sts.ou10 = (uint16_t)ds.Bits_ouBuf[10];
    mvb_sts.ou11 = (uint16_t)ds.Bits_ouBuf[11];
    mvb_sts.ou12 = (uint16_t)ds.Bits_ouBuf[12];
    mvb_sts.sw2 = (uint16_t)ds.Bits_ouBuf[13];
    
    /* 第3组DIO板输出 */
    mvb_sts.ou13 = (uint16_t)ds.Bits_ouBuf[14];
    mvb_sts.ou14 = (uint16_t)ds.Bits_ouBuf[15];
    mvb_sts.ou15 = (uint16_t)ds.Bits_ouBuf[16];
    mvb_sts.ou16 = (uint16_t)ds.Bits_ouBuf[17];
    mvb_sts.ou17 = (uint16_t)ds.Bits_ouBuf[18];
    mvb_sts.ou18 = (uint16_t)ds.Bits_ouBuf[19];
    mvb_sts.sw3 = (uint16_t)ds.Bits_ouBuf[20];
    
    /* 第4组DIO板输出 */
    mvb_sts.ou19 = (uint16_t)ds.Bits_ouBuf[21];
    mvb_sts.ou20 = (uint16_t)ds.Bits_ouBuf[22];
    mvb_sts.ou21 = (uint16_t)ds.Bits_ouBuf[23];
    mvb_sts.ou22 = (uint16_t)ds.Bits_ouBuf[24];
    mvb_sts.ou23 = (uint16_t)ds.Bits_ouBuf[25];
    mvb_sts.ou24 = (uint16_t)ds.Bits_ouBuf[26];
    mvb_sts.sw4 = (uint16_t)ds.Bits_ouBuf[27];
    
    /* 第5组DIO板输出 */
    mvb_sts.ou25 = (uint16_t)ds.Bits_ouBuf[28];
    mvb_sts.ou26 = (uint16_t)ds.Bits_ouBuf[29];
    mvb_sts.ou27 = (uint16_t)ds.Bits_ouBuf[30];
    mvb_sts.ou28 = (uint16_t)ds.Bits_ouBuf[31];
    mvb_sts.ou29 = (uint16_t)ds.Bits_ouBuf[32];
    mvb_sts.ou30 = (uint16_t)ds.Bits_ouBuf[33];
    mvb_sts.sw5 = (uint16_t)ds.Bits_ouBuf[34];
    
    /* 第6组DIO板输出 */
    mvb_sts.ou31 = (uint16_t)ds.Bits_ouBuf[35];
    mvb_sts.ou32 = (uint16_t)ds.Bits_ouBuf[36];
    mvb_sts.ou33 = (uint16_t)ds.Bits_ouBuf[37];
    mvb_sts.ou34 = (uint16_t)ds.Bits_ouBuf[38];
    mvb_sts.ou35 = (uint16_t)ds.Bits_ouBuf[39];
    mvb_sts.ou36 = (uint16_t)ds.Bits_ouBuf[40];
    mvb_sts.sw6 = (uint16_t)ds.Bits_ouBuf[41];
    
    /* 第7组DIO板输出 */
    mvb_sts.ou37 = (uint16_t)ds.Bits_ouBuf[42];
    mvb_sts.ou38 = (uint16_t)ds.Bits_ouBuf[43];
    mvb_sts.ou39 = (uint16_t)ds.Bits_ouBuf[44];
    mvb_sts.ou40 = (uint16_t)ds.Bits_ouBuf[45];
    mvb_sts.ou41 = (uint16_t)ds.Bits_ouBuf[46];
    mvb_sts.ou42 = (uint16_t)ds.Bits_ouBuf[47];
    mvb_sts.sw7 = (uint16_t)ds.Bits_ouBuf[48];
    
    /* 第8组DIO板输出 */
    mvb_sts.ou43 = (uint16_t)ds.Bits_ouBuf[49];
    mvb_sts.ou44 = (uint16_t)ds.Bits_ouBuf[50];
    mvb_sts.ou45 = (uint16_t)ds.Bits_ouBuf[51];
    mvb_sts.ou46 = (uint16_t)ds.Bits_ouBuf[52];
    mvb_sts.ou47 = (uint16_t)ds.Bits_ouBuf[53];
    mvb_sts.ou48 = (uint16_t)ds.Bits_ouBuf[54];
    mvb_sts.sw8 = (uint16_t)ds.Bits_ouBuf[55];
    
    /* 第9组DIO板输出 */
//    mvb_sts.ou49 = (uint16_t)ds.Bits_ouBuf[56];
//    mvb_sts.ou50 = (uint16_t)ds.Bits_ouBuf[57];
//    mvb_sts.ou51 = (uint16_t)ds.Bits_ouBuf[58];
//    mvb_sts.ou52 = (uint16_t)ds.Bits_ouBuf[59];
//    mvb_sts.ou53 = (uint16_t)ds.Bits_ouBuf[60];
//    mvb_sts.ou54 = (uint16_t)ds.Bits_ouBuf[61];
    mvb_sts.sw9 = (uint16_t)ds.Bits_ouBuf[62];
    
    mvb_sts.lifesign = lifesign++;
    mvb_sts.mcuVer = MCU_VERSION;
    
    rt_memcpy(buf, &mvb_sts, 32);
}

