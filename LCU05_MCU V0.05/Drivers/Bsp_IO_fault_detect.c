

#include "Bsp_IO_fault_detect.h"
#include "Bsp_canApp.h"


#define IO_FLT_CNT         (5)


static void inFlt_detect_Two_Valid(uint8_t offset, uint8_t index1, uint8_t index2)
{
    uint8_t i;
    
    for(i = 0; i < BOARD_IN_COUNT; i++)
    {
        if(ds.boardIO[offset + index1].Bits_in[i] != ds.boardIO[offset + index2].Bits_in[i])
        {
            if(ds.boardIO[offset + index1].Bits_in[i] == 0)   ds.boardIO[offset + index1].flt_in_cnt[i]++; 
            if(ds.boardIO[offset + index2].Bits_in[i] == 0)   ds.boardIO[offset + index2].flt_in_cnt[i]++; 
        }
        else
        {
            ds.boardIO[offset + index1].flt_in_cnt[i] = 0;
            ds.boardIO[offset + index2].flt_in_cnt[i] = 0;
        }
    }
}


static void inFlt_detect_All_Valid(uint8_t offset)
{
    uint8_t i;
    BYTE_TO_BIT sts = {0};
    BYTE_TO_BIT flt = {0};
    
    for(i = 0; i < BOARD_IN_COUNT; i++)
    {
        flt.Bits.bit0 = ds.boardIO[offset].Bits_flt_in[i];          //DIO A IN Flt
        flt.Bits.bit1 = ds.boardIO[offset + 1].Bits_flt_in[i];      //DIO B IN Flt
        flt.Bits.bit2 = ds.boardIO[offset + 2].Bits_flt_in[i];      //DIO C IN Flt
        
        switch(flt.value)
        {
            case 0://000 A,B,C OK.
                sts.Bits.bit2 = ds.boardIO[offset].Bits_in[i];          //DIO A IN
                sts.Bits.bit1 = ds.boardIO[offset + 1].Bits_in[i];      //DIO B IN
                sts.Bits.bit0 = ds.boardIO[offset + 2].Bits_in[i];      //DIO C IN
                
                switch(sts.value)
                {
                    case 1: ds.boardIO[offset + 2].flt_in_cnt[i]++;  break;
                    case 2: ds.boardIO[offset + 1].flt_in_cnt[i]++;  break;
                    case 3: ds.boardIO[offset].flt_in_cnt[i]++;  break;
                    case 4: ds.boardIO[offset].flt_in_cnt[i]++;  break;
                    case 5: ds.boardIO[offset + 1].flt_in_cnt[i]++;  break;
                    case 6: ds.boardIO[offset + 2].flt_in_cnt[i]++;  break;
                    default:
                        ds.boardIO[offset].flt_in_cnt[i] = 0;
                        ds.boardIO[offset + 1].flt_in_cnt[i] = 0;
                        ds.boardIO[offset + 2].flt_in_cnt[i] = 0;
                    break;
                }
                break;
            case 1://001 A flt
                inFlt_detect_Two_Valid(offset, 1, 2);
                break;
            case 2://010 B flt
                inFlt_detect_Two_Valid(offset, 2, 0);
                break;
            case 4://100 C flt
                inFlt_detect_Two_Valid(offset, 0, 1);
                break;
            default:
                break; 
        }
    }
}


//input channels fault detect.
static void input_fault_update(uint8_t offset)
{
    uint8_t i;
    uint8_t j;    
    BYTE_TO_BIT sts = {0};
    
    sts.Bits.bit0 = ds.canNodes[offset + 7].sts.isLost;
    sts.Bits.bit1 = ds.canNodes[offset + 8].sts.isLost;
    sts.Bits.bit2 = ds.canNodes[offset + 9].sts.isLost;

    switch(sts.value)
    {
        case 0: //000 A、B、C均在位
            inFlt_detect_All_Valid(offset);
            break;
        case 1: //001 B、C在位、A不在位
            inFlt_detect_Two_Valid(offset, 1, 2);
            break;
        case 2: //010 A、C在位、B不在位
            inFlt_detect_Two_Valid(offset, 2, 0);
            break; 
        case 4: //100 A、B在位、C不在位
            inFlt_detect_Two_Valid(offset, 0, 1);
            break;
        default:
            break;
    }
    
    for(i = 0; i < BOARD_IN_COUNT; i++)
    {
        for(j = 0; j < 3; j++)
        {
            if(ds.boardIO[offset + j].Bits_flt_in[i] == 0)
            {
                ds.boardIO[offset + j].Bits_flt_in[i] = (ds.boardIO[offset + j].flt_in_cnt[i] >= IO_FLT_CNT) ? 1 : 0;
            }
        }
    }
}


static void ouFlt_detect(uint8_t offset, uint8_t index)
{
    uint8_t i;
    BYTE_TO_BIT sts = {0};
    uint8_t index1 = offset + index;
    uint8_t index2 = offset + (index + 1)%3;
    
    for(i = 0; i < 6; i++)
    {
        if(ds.boardIO[index1].Bits_ou[i] == ds.boardIO[index2].Bits_ou[i])
        {
            sts.Bits.bit2 = ds.boardIO[index1].Bits_fb[i*2];
            sts.Bits.bit1 = ds.boardIO[index1].Bits_ou[i];
            sts.Bits.bit0 = ds.boardIO[index2].Bits_fb[i*2 + 1];

            switch(sts.value)
            {
                case 1: ds.boardIO[index2].flt_ou2_cnt[i]++; break;
                case 2: ds.boardIO[index1].flt_ou1_cnt[i]++; break;
                case 3: ds.boardIO[index1].flt_ou1_cnt[i]++; break;
                case 4: ds.boardIO[index1].flt_ou1_cnt[i]++; break;
                case 5: ds.boardIO[index1].flt_ou1_cnt[i]++;
                        ds.boardIO[index2].flt_ou2_cnt[i]++; break;
                case 6: ds.boardIO[index2].flt_ou2_cnt[i]++; break;
                default:
                    ds.boardIO[index1].flt_ou1_cnt[i] = 0;
                    ds.boardIO[index2].flt_ou2_cnt[i] = 0;
                    break;
            }
        }
    }
}


static void swFlt_detect(uint8_t offset, uint8_t index)
{
    BYTE_TO_BIT sts = {0};
    uint8_t index1 = offset + index;
    uint8_t index2 = offset + (index + 1)%3;
    

    if(ds.boardIO[index1].Bits_ou[6] == ds.boardIO[index2].Bits_ou[6])
    {
        sts.Bits.bit3 = ds.boardIO[index1].Bits_ou[6];
        sts.Bits.bit2 = ds.boardIO[index1].Bits_fb[12];
        sts.Bits.bit1 = ds.boardIO[index1].Bits_fb[13];
        sts.Bits.bit0 = ds.boardIO[index2].Bits_fb[14];

        switch(sts.value)
        {
            case 1:     ds.boardIO[index2].flt_ou2_cnt[6]++;     break;
            case 2:     ds.boardIO[index1].flt_ou1_cnt[6]++;     break;
            case 3:     ds.boardIO[index1].flt_ou1_cnt[6]++;
                        ds.boardIO[index2].flt_ou2_cnt[6]++;     break;
            
            case 5:     ds.boardIO[index2].flt_ou2_cnt[6]++;     break;
            case 6:     ds.boardIO[index1].flt_ou1_cnt[6]++;     break;
            case 7:     ds.boardIO[index1].flt_ou1_cnt[6]++;
                        ds.boardIO[index2].flt_ou2_cnt[6]++;     break;
            
            case 9:     ds.boardIO[index2].flt_ou2_cnt[6]++;     break;
            case 10:    ds.boardIO[index1].flt_ou1_cnt[6]++;     break;
            case 11:    ds.boardIO[index1].flt_ou1_cnt[6]++;     break;
            case 12:    ds.boardIO[index1].flt_ou1_cnt[6]++;     break;
            case 13:    ds.boardIO[index1].flt_ou1_cnt[6]++;     break;
            case 14:    ds.boardIO[index2].flt_ou2_cnt[6]++;     break;
            default:
                ds.boardIO[index1].flt_ou1_cnt[6] = 0;
                ds.boardIO[index2].flt_ou2_cnt[6] = 0;
                break;
        }
    }

}


//output channels fault detect.
static void output_fault_update(uint8_t offset)
{
    uint8_t i;
    uint8_t j;

    for(j = 0; j < 3; j++)
    {
        ouFlt_detect(offset, j);
        swFlt_detect(offset, j);

        for(i = 0; i < BOARD_OU_COUNT; i++)
        {
            if(ds.boardIO[offset + j].Bits_flt_ou[i] == 0)
            {
                ds.boardIO[offset + j].Bits_flt_ou[i] = (ds.boardIO[offset + j].flt_ou1_cnt[i] >= IO_FLT_CNT || ds.boardIO[offset + j].flt_ou2_cnt[i] >= IO_FLT_CNT) ? 1 : 0;
            }
        }
    }
}


void io_fault_detect(void)
{
    uint8_t i;

    for(i = 0; i < ds.ioGroupCnt; i++)
    {
        input_fault_update(i * 3);
        output_fault_update(i * 3);
    }
    
    for(i = 0; i < (ds.ioGroupCnt * 3); i++)
    {
        if(ds.boardIO[i].flt_in[0] != 0 || ds.boardIO[i].flt_in[1] != 0 || ds.boardIO[i].flt_ou != 0)
        {
            if(ds.boardIO[i].fltFlag < 3)
            {
                can_send_ioFlt(i);
                ds.boardIO[i].fltFlag++;
            }
        }
    }
}


