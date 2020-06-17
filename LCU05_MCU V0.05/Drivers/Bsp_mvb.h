

#ifndef _BSP_MVB_H_
#define _BSP_MVB_H_

#ifdef __cplusplus
extern "C" {
#endif 

#include <rtthread.h>
#include "stm32f4xx.h"
    
/*
0x11223344
       µÕµÿ÷∑ --> ∏ﬂµÿ÷∑
¥Û∂Àƒ£ Ω£∫11 22 33 44
–°∂Àƒ£ Ω£∫44 33 22 11
  */  

typedef struct
{
    /* Word 0 */
    uint16_t res01          :1;     /* ‘§¡Ù */
    uint16_t res02          :1;     /* ‘§¡Ù */
    uint16_t pwr_c_5v       :1;     /* C◊ÈµÁ‘¥π ’œ */
    uint16_t pwr_c_110v     :1;     /* C◊Èπ©µÁ∂œø™ */
    uint16_t pwr_b_5v       :1;     /* B◊ÈµÁ‘¥π ’œ */
    uint16_t pwr_b_110v     :1;     /* B◊Èπ©µÁ∂œø™ */
    uint16_t pwr_a_5v       :1;     /* A◊ÈµÁ‘¥π ’œ */
    uint16_t pwr_a_110v     :1;     /* A◊Èπ©µÁ∂œø™ */
    
    uint16_t mcu_a_can2     :1;     /* ÷˜øÿ∞ÂA±≥∞ÂCAN2π ’œ */
    uint16_t mcu_a_lost     :1;     /* ÷˜øÿ∞ÂA∂™ ß */
    uint16_t lcu_flt_ser    :1;     /* —œ÷ÿπ ’œ */
    uint16_t lcu_flt_med    :1;     /* ÷–µ»π ’œ */
    uint16_t lcu_flt_sli    :1;     /* «·Œ¢π ’œ */
    uint16_t mvb_flt        :1;     /* MVB∞Âπ ’œ */
    uint16_t mvb_can1       :1;     /* MVB∞ÂCAN1π ’œ */
    uint16_t mvb_can2       :1;     /* MVB∞ÂCAN2π ’œ */
    
    /* Word 1 */
    uint16_t mcu_b_flt      :1;     /* ÷˜øÿ∞ÂBπ ’œ */
    uint16_t mcu_b_can1     :1;     /* ÷˜øÿ∞ÂB±≥∞ÂCAN1π ’œ */
    uint16_t mcu_b_can2     :1;     /* ÷˜øÿ∞ÂB±≥∞ÂCAN2π ’œ */
    uint16_t mcu_b_lost     :1;     /* ÷˜øÿ∞ÂB∂™ ß */
    uint16_t res11          :1;     /* ‘§¡Ù */
    uint16_t mcu_a_ou       :1;     /* ÷˜øÿ∞ÂA ‰≥ˆπ ’œ£®‘À¥Ô∑Ω∞∏£© */
    uint16_t mcu_a_flt      :1;     /* ÷˜øÿ∞ÂBπ ’œ */
    uint16_t mcu_a_can1     :1;     /* ÷˜øÿ∞ÂA±≥∞ÂCAN1π ’œ */
    
    uint16_t res12          :1;     /* ‘§¡Ù */
    uint16_t mcu_c_ou       :1;     /* ÷˜øÿ∞ÂC ‰≥ˆπ ’œ£®‘À¥Ô∑Ω∞∏£© */
    uint16_t mcu_c_flt      :1;     /* ÷˜øÿ∞ÂCπ ’œ */
    uint16_t mcu_c_can1     :1;     /* ÷˜øÿ∞ÂC±≥∞ÂCAN1π ’œ */
    uint16_t mcu_c_can2     :1;     /* ÷˜øÿ∞ÂC±≥∞ÂCAN2π ’œ */
    uint16_t mcu_c_lost     :1;     /* ÷˜øÿ∞ÂC∂™ ß */
    uint16_t res13          :1;     /* ‘§¡Ù */
    uint16_t mcu_b_ou       :1;     /* ÷˜øÿ∞ÂB ‰≥ˆπ ’œ£®‘À¥Ô∑Ω∞∏£© */
    
    /* Word 2 */
    uint16_t etu_can1       :1;     /* ETU∞Â±≥∞ÂCAN1π ’œ */
    uint16_t etu_can2       :1;     /* ETU∞Â±≥∞ÂCAN1π ’œ */
    uint16_t can_flt        :1;     /* CAN∞Âπ ’œ */
    uint16_t can_lost       :1;     /* CAN∞Â∂™ ß */
    uint16_t can_ext_can1   :1;     /* CANÕ¯πÿ∞Â«∞√Ê∞ÂCAN1π ’œ */
    uint16_t can_ext_can2   :1;     /* CANÕ¯πÿ∞Â«∞√Ê∞ÂCAN2π ’œ */
    uint16_t can_can1       :1;     /* CANÕ¯πÿ∞Â±≥∞ÂCAN1π ’œ */
    uint16_t can_can2       :1;     /* CANÕ¯πÿ∞Â±≥∞ÂCAN2π ’œ */
    
    uint16_t dio_ver        :1;     /* DIO»Ìº˛∞Ê±æ∫≈≤ª“ª÷¬ */
    uint16_t mcu_kw_ver     :1;     /* MCU”¶”√≤„»Ìº˛∞Ê±æ∫≈≤ª“ª÷¬ */
    uint16_t mcu_ver        :1;     /* MCUµ◊≤„»Ìº˛∞Ê±æ∫≈≤ª“ª÷¬ */
    uint16_t res21          :1;     /* ‘§¡Ù */
    uint16_t res22          :1;     /* ‘§¡Ù */
    uint16_t etu_lost       :1;     /* ETU∞Â∂™ ß */
    uint16_t etu_eth        :1;     /* ETU∞Â“‘Ã´Õ¯Õ®—∂π ’œ */
    uint16_t etu_flt        :1;     /* ETU∞Âπ ’œ */
    
    /* Word 3 */
    uint16_t io1_a_flt      :1;     /* IO1∞ÂAπ ’œ */
    uint16_t io1_a_can1     :1;     /* IO1∞ÂA±≥∞ÂCAN1π ’œ */
    uint16_t io1_a_can2     :1;     /* IO1∞ÂA±≥∞ÂCAN1π ’œ */
    uint16_t io1_a_in       :1;     /* IO1∞ÂA ‰»Îπ ’œ */
    uint16_t io1_a_ou       :1;     /* IO1∞ÂA ‰≥ˆπ ’œ */
    uint16_t io1_a_lost     :1;     /* IO1∞ÂA∂™ ß */
    uint16_t res31          :1;     /* ‘§¡Ù */
    uint16_t gio_ver        :1;     /* GIO»Ìº˛∞Ê±æ∫≈≤ª“ª÷¬£®‘À¥Ô∑Ω∞∏£© */
    
    uint16_t res32          :1;     /* ‘§¡Ù */
    uint16_t io1_b_flt      :1;     /* IO1∞ÂBπ ’œ */
    uint16_t io1_b_can1     :1;     /* IO1∞ÂB±≥∞ÂCAN1π ’œ */
    uint16_t io1_b_can2     :1;     /* IO1∞ÂB±≥∞ÂCAN1π ’œ */
    uint16_t io1_b_in       :1;     /* IO1∞ÂB ‰»Îπ ’œ */
    uint16_t io1_b_ou       :1;     /* IO1∞ÂB ‰≥ˆπ ’œ */
    uint16_t io1_b_lost     :1;     /* IO1∞ÂB∂™ ß */
    uint16_t res33          :1;     /* ‘§¡Ù */
    
    /* Word 4 */
    uint16_t io2_a_lost     :1;     /* IO2∞ÂA∂™ ß */
    uint16_t res41          :1;     /* ‘§¡Ù */
    uint16_t io1_c_flt      :1;     /* IO1∞ÂCπ ’œ */
    uint16_t io1_c_can1     :1;     /* IO1∞ÂC±≥∞ÂCAN1π ’œ */
    uint16_t io1_c_can2     :1;     /* IO1∞ÂC±≥∞ÂCAN1π ’œ */
    uint16_t io1_c_in       :1;     /* IO1∞ÂC ‰»Îπ ’œ */
    uint16_t io1_c_ou       :1;     /* IO1∞ÂC ‰≥ˆπ ’œ */
    uint16_t io1_c_lost     :1;     /* IO1∞ÂC∂™ ß */
    
    uint16_t io2_b_ou       :1;     /* IO2∞ÂB ‰≥ˆπ ’œ */
    uint16_t io2_b_lost     :1;     /* IO2∞ÂB∂™ ß */
    uint16_t res42          :1;     /* ‘§¡Ù */    
    uint16_t io2_a_flt      :1;     /* IO2∞ÂAπ ’œ */    
    uint16_t io2_a_can1     :1;     /* IO2∞ÂA±≥∞ÂCAN1π ’œ */
    uint16_t io2_a_can2     :1;     /* IO2∞ÂA±≥∞ÂCAN1π ’œ */
    uint16_t io2_a_in       :1;     /* IO2∞ÂA ‰»Îπ ’œ */
    uint16_t io2_a_ou       :1;     /* IO2∞ÂA ‰≥ˆπ ’œ */
    
    /* Word 5 */
    uint16_t io2_c_in       :1;     /* IO2∞ÂC ‰»Îπ ’œ */
    uint16_t io2_c_ou       :1;     /* IO2∞ÂC ‰≥ˆπ ’œ */
    uint16_t io2_c_lost     :1;     /* IO2∞ÂC∂™ ß */    
    uint16_t res51          :1;     /* ‘§¡Ù */    
    uint16_t io2_b_flt      :1;     /* IO2∞ÂBπ ’œ */
    uint16_t io2_b_can1     :1;     /* IO2∞ÂB±≥∞ÂCAN1π ’œ */
    uint16_t io2_b_can2     :1;     /* IO2∞ÂB±≥∞ÂCAN1π ’œ */
    uint16_t io2_b_in       :1;     /* IO2∞ÂB ‰»Îπ ’œ */
    
    uint16_t io3_a_can2     :1;     /* IO3∞ÂA±≥∞ÂCAN1π ’œ */
    uint16_t io3_a_in       :1;     /* IO3∞ÂA ‰»Îπ ’œ */
    uint16_t io3_a_ou       :1;     /* IO3∞ÂA ‰≥ˆπ ’œ */
    uint16_t io3_a_lost     :1;     /* IO3∞ÂA∂™ ß */
    uint16_t res52          :1;     /* ‘§¡Ù */    
    uint16_t io2_c_flt      :1;     /* IO2∞ÂCπ ’œ */
    uint16_t io2_c_can1     :1;     /* IO2∞ÂC±≥∞ÂCAN1π ’œ */
    uint16_t io2_c_can2     :1;     /* IO2∞ÂC±≥∞ÂCAN1π ’œ */
    
    /* Word 6 */
    uint16_t io3_b_can1     :1;     /* IO3∞ÂB±≥∞ÂCAN1π ’œ */
    uint16_t io3_b_can2     :1;     /* IO3∞ÂB±≥∞ÂCAN1π ’œ */
    uint16_t io3_b_in       :1;     /* IO3∞ÂB ‰»Îπ ’œ */
    uint16_t io3_b_ou       :1;     /* IO3∞ÂB ‰≥ˆπ ’œ */
    uint16_t io3_b_lost     :1;     /* IO3∞ÂB∂™ ß */
    uint16_t res61          :1;     /* ‘§¡Ù */    
    uint16_t io3_a_flt      :1;     /* IO3∞ÂAπ ’œ */
    uint16_t io3_a_can1     :1;     /* IO3∞ÂA±≥∞ÂCAN1π ’œ */

    uint16_t io3_c_flt      :1;     /* IO3∞ÂCπ ’œ */
    uint16_t io3_c_can1     :1;     /* IO3∞ÂC±≥∞ÂCAN1π ’œ */
    uint16_t io3_c_can2     :1;     /* IO3∞ÂC±≥∞ÂCAN1π ’œ */
    uint16_t io3_c_in       :1;     /* IO3∞ÂC ‰»Îπ ’œ */
    uint16_t io3_c_ou       :1;     /* IO3∞ÂC ‰≥ˆπ ’œ */
    uint16_t io3_c_lost     :1;     /* IO3∞ÂC∂™ ß */    
    uint16_t res62          :1;     /* ‘§¡Ù */    
    uint16_t io3_b_flt      :1;     /* IO3∞ÂBπ ’œ */
    
    /* Word 7 */
    uint16_t res71          :1;     /* ‘§¡Ù */    
    uint16_t res72          :1;     /* ‘§¡Ù */    
    uint16_t res73          :1;     /* ‘§¡Ù */    
    uint16_t res74          :1;     /* ‘§¡Ù */    
    uint16_t res75          :1;     /* ‘§¡Ù */    
    uint16_t res76          :1;     /* ‘§¡Ù */    
    uint16_t res77          :1;     /* ‘§¡Ù */    
    uint16_t res78          :1;     /* ‘§¡Ù */
    
    uint16_t res79          :1;     /* ‘§¡Ù */
    uint16_t io4_a_flt      :1;     /* IO4∞ÂAπ ’œ */
    uint16_t io4_a_can1     :1;     /* IO4∞ÂA±≥∞ÂCAN1π ’œ */
    uint16_t io4_a_can2     :1;     /* IO4∞ÂA±≥∞ÂCAN1π ’œ */
    uint16_t io4_a_in       :1;     /* IO4∞ÂA ‰»Îπ ’œ */
    uint16_t io4_a_ou       :1;     /* IO4∞ÂA ‰≥ˆπ ’œ */
    uint16_t io4_a_lost     :1;     /* IO4∞ÂA∂™ ß */
    uint16_t res710         :1;     /* ‘§¡Ù */
    
    /* Word 8 */
    uint16_t io4_c_lost     :1;     /* IO4∞ÂC∂™ ß */    
    uint16_t res81          :1;     /* ‘§¡Ù */
    uint16_t io4_b_flt      :1;     /* IO4∞ÂBπ ’œ */
    uint16_t io4_b_can1     :1;     /* IO4∞ÂB±≥∞ÂCAN1π ’œ */
    uint16_t io4_b_can2     :1;     /* IO4∞ÂB±≥∞ÂCAN1π ’œ */
    uint16_t io4_b_in       :1;     /* IO4∞ÂB ‰»Îπ ’œ */
    uint16_t io4_b_ou       :1;     /* IO4∞ÂB ‰≥ˆπ ’œ */
    uint16_t io4_b_lost     :1;     /* IO4∞ÂB∂™ ß */
    
    uint16_t io5_a_ou       :1;     /* IO5∞ÂA ‰≥ˆπ ’œ */
    uint16_t io5_a_lost     :1;     /* IO5∞ÂA∂™ ß */
    uint16_t res82          :1;     /* ‘§¡Ù */
    uint16_t io4_c_flt      :1;     /* IO4∞ÂCπ ’œ */
    uint16_t io4_c_can1     :1;     /* IO4∞ÂC±≥∞ÂCAN1π ’œ */
    uint16_t io4_c_can2     :1;     /* IO4∞ÂC±≥∞ÂCAN1π ’œ */
    uint16_t io4_c_in       :1;     /* IO4∞ÂC ‰»Îπ ’œ */
    uint16_t io4_c_ou       :1;     /* IO4∞ÂC ‰≥ˆπ ’œ */
    
    /* Word 9 */
    uint16_t io5_b_in       :1;     /* IO5∞ÂB ‰»Îπ ’œ */
    uint16_t io5_b_ou       :1;     /* IO5∞ÂB ‰≥ˆπ ’œ */
    uint16_t io5_b_lost     :1;     /* IO5∞ÂB∂™ ß */
    uint16_t res91          :1;     /* ‘§¡Ù */
    uint16_t io5_a_flt      :1;     /* IO5∞ÂAπ ’œ */
    uint16_t io5_a_can1     :1;     /* IO5∞ÂA±≥∞ÂCAN1π ’œ */
    uint16_t io5_a_can2     :1;     /* IO5∞ÂA±≥∞ÂCAN1π ’œ */
    uint16_t io5_a_in       :1;     /* IO5∞ÂA ‰»Îπ ’œ */
    
    uint16_t io5_c_can2     :1;     /* IO5∞ÂC±≥∞ÂCAN1π ’œ */
    uint16_t io5_c_in       :1;     /* IO5∞ÂC ‰»Îπ ’œ */
    uint16_t io5_c_ou       :1;     /* IO5∞ÂC ‰≥ˆπ ’œ */
    uint16_t io5_c_lost     :1;     /* IO5∞ÂC∂™ ß */    
    uint16_t res92          :1;     /* ‘§¡Ù */
    uint16_t io5_b_flt      :1;     /* IO5∞ÂBπ ’œ */
    uint16_t io5_b_can1     :1;     /* IO5∞ÂB±≥∞ÂCAN1π ’œ */
    uint16_t io5_b_can2     :1;     /* IO5∞ÂB±≥∞ÂCAN1π ’œ */
    
    /* Word 10 */
    uint16_t io6_a_can1     :1;     /* IO6∞ÂA±≥∞ÂCAN1π ’œ */
    uint16_t io6_a_can2     :1;     /* IO6∞ÂA±≥∞ÂCAN1π ’œ */
    uint16_t io6_a_in       :1;     /* IO6∞ÂA ‰»Îπ ’œ */
    uint16_t io6_a_ou       :1;     /* IO6∞ÂA ‰≥ˆπ ’œ */
    uint16_t io6_a_lost     :1;     /* IO6∞ÂA∂™ ß */
    uint16_t res101         :1;     /* ‘§¡Ù */
    uint16_t io5_c_flt      :1;     /* IO5∞ÂCπ ’œ */
    uint16_t io5_c_can1     :1;     /* IO5∞ÂC±≥∞ÂCAN1π ’œ */
    
    uint16_t io6_b_flt      :1;     /* IO6∞ÂBπ ’œ */
    uint16_t io6_b_can1     :1;     /* IO6∞ÂB±≥∞ÂCAN1π ’œ */
    uint16_t io6_b_can2     :1;     /* IO6∞ÂB±≥∞ÂCAN1π ’œ */
    uint16_t io6_b_in       :1;     /* IO6∞ÂB ‰»Îπ ’œ */
    uint16_t io6_b_ou       :1;     /* IO6∞ÂB ‰≥ˆπ ’œ */
    uint16_t io6_b_lost     :1;     /* IO6∞ÂB∂™ ß */    
    uint16_t res102         :1;     /* ‘§¡Ù */
    uint16_t io6_a_flt      :1;     /* IO6∞ÂAπ ’œ */
    
    /* Word 11 */
    uint16_t res111         :1;     /* ‘§¡Ù */
    uint16_t io6_c_flt      :1;     /* IO6∞ÂCπ ’œ */
    uint16_t io6_c_can1     :1;     /* IO6∞ÂC±≥∞ÂCAN1π ’œ */
    uint16_t io6_c_can2     :1;     /* IO6∞ÂC±≥∞ÂCAN1π ’œ */
    uint16_t io6_c_in       :1;     /* IO6∞ÂC ‰»Îπ ’œ */
    uint16_t io6_c_ou       :1;     /* IO6∞ÂC ‰≥ˆπ ’œ */
    uint16_t io6_c_lost     :1;     /* IO6∞ÂC∂™ ß */    
    uint16_t res112         :1;     /* ‘§¡Ù */
    
    uint16_t io7_b_lost     :1;     /* IO7∞ÂB∂™ ß */
    uint16_t res113         :1;     /* ‘§¡Ù */
    uint16_t io7_a_flt      :1;     /* IO7∞ÂAπ ’œ */
    uint16_t io7_a_can1     :1;     /* IO7∞ÂA±≥∞ÂCAN1π ’œ */
    uint16_t io7_a_can2     :1;     /* IO7∞ÂA±≥∞ÂCAN1π ’œ */
    uint16_t io7_a_in       :1;     /* IO7∞ÂA ‰»Îπ ’œ */
    uint16_t io7_a_ou       :1;     /* IO7∞ÂA ‰≥ˆπ ’œ */
    uint16_t io7_a_lost     :1;     /* IO7∞ÂA∂™ ß */
   
    /* Word 12 */
    uint16_t io7_c_ou       :1;     /* IO7∞ÂC ‰≥ˆπ ’œ */
    uint16_t io7_c_lost     :1;     /* IO7∞ÂC∂™ ß */    
    uint16_t res121         :1;     /* ‘§¡Ù */
    uint16_t io7_b_flt      :1;     /* IO7∞ÂBπ ’œ */
    uint16_t io7_b_can1     :1;     /* IO7∞ÂB±≥∞ÂCAN1π ’œ */
    uint16_t io7_b_can2     :1;     /* IO7∞ÂB±≥∞ÂCAN1π ’œ */
    uint16_t io7_b_in       :1;     /* IO7∞ÂB ‰»Îπ ’œ */
    uint16_t io7_b_ou       :1;     /* IO7∞ÂB ‰≥ˆπ ’œ */
    
    uint16_t io8_a_in       :1;     /* IO8∞ÂA ‰»Îπ ’œ */
    uint16_t io8_a_ou       :1;     /* IO8∞ÂA ‰≥ˆπ ’œ */
    uint16_t io8_a_lost     :1;     /* IO8∞ÂA∂™ ß */
    uint16_t res122         :1;     /* ‘§¡Ù */
    uint16_t io7_c_flt      :1;     /* IO7∞ÂCπ ’œ */
    uint16_t io7_c_can1     :1;     /* IO7∞ÂC±≥∞ÂCAN1π ’œ */
    uint16_t io7_c_can2     :1;     /* IO7∞ÂC±≥∞ÂCAN1π ’œ */
    uint16_t io7_c_in       :1;     /* IO7∞ÂC ‰»Îπ ’œ */
    
    /* Word 13 */    
    uint16_t io8_b_can2     :1;     /* IO8∞ÂB±≥∞ÂCAN1π ’œ */
    uint16_t io8_b_in       :1;     /* IO8∞ÂB ‰»Îπ ’œ */
    uint16_t io8_b_ou       :1;     /* IO8∞ÂB ‰≥ˆπ ’œ */
    uint16_t io8_b_lost     :1;     /* IO8∞ÂB∂™ ß */
    uint16_t res131         :1;     /* ‘§¡Ù */    
    uint16_t io8_a_flt      :1;     /* IO8∞ÂAπ ’œ */
    uint16_t io8_a_can1     :1;     /* IO8∞ÂA±≥∞ÂCAN1π ’œ */
    uint16_t io8_a_can2     :1;     /* IO8∞ÂA±≥∞ÂCAN1π ’œ */
    
    uint16_t io8_c_can1     :1;     /* IO8∞ÂC±≥∞ÂCAN1π ’œ */
    uint16_t io8_c_can2     :1;     /* IO8∞ÂC±≥∞ÂCAN1π ’œ */
    uint16_t io8_c_in       :1;     /* IO8∞ÂC ‰»Îπ ’œ */
    uint16_t io8_c_ou       :1;     /* IO8∞ÂC ‰≥ˆπ ’œ */
    uint16_t io8_c_lost     :1;     /* IO8∞ÂC∂™ ß */
    uint16_t res132         :1;     /* ‘§¡Ù */   
    uint16_t io8_b_flt      :1;     /* IO8∞ÂBπ ’œ */
    uint16_t io8_b_can1     :1;     /* IO8∞ÂB±≥∞ÂCAN1π ’œ */
    
    /* Word 14 */
    uint16_t io9_a_flt      :1;     /* IO9∞ÂAπ ’œ */
    uint16_t io9_a_can1     :1;     /* IO9∞ÂA±≥∞ÂCAN1π ’œ */
    uint16_t io9_a_can2     :1;     /* IO9∞ÂA±≥∞ÂCAN1π ’œ */
    uint16_t io9_a_in       :1;     /* IO9∞ÂA ‰»Îπ ’œ */
    uint16_t io9_a_ou       :1;     /* IO9∞ÂA ‰≥ˆπ ’œ */
    uint16_t io9_a_lost     :1;     /* IO9∞ÂA∂™ ß */    
    uint16_t res141         :1;     /* ‘§¡Ù */    
    uint16_t io8_c_flt      :1;     /* IO8∞ÂCπ ’œ */
    
    uint16_t res142         :1;     /* ‘§¡Ù */
    uint16_t io9_b_flt      :1;     /* IO9∞ÂBπ ’œ */
    uint16_t io9_b_can1     :1;     /* IO9∞ÂB±≥∞ÂCAN1π ’œ */
    uint16_t io9_b_can2     :1;     /* IO9∞ÂB±≥∞ÂCAN1π ’œ */
    uint16_t io9_b_in       :1;     /* IO9∞ÂB ‰»Îπ ’œ */
    uint16_t io9_b_ou       :1;     /* IO9∞ÂB ‰≥ˆπ ’œ */
    uint16_t io9_b_lost     :1;     /* IO9∞ÂB∂™ ß */    
    uint16_t res143         :1;     /* ‘§¡Ù */    
    
    /* Word 15 */
    uint16_t car_mand_ou    :1;     /* Àæª˙ “’º”– ‰≥ˆπ ’œ */    
    uint16_t res151         :1;     /* ‘§¡Ù */    
    uint16_t io9_c_flt      :1;     /* IO9∞ÂCπ ’œ */
    uint16_t io9_c_can1     :1;     /* IO9∞ÂC±≥∞ÂCAN1π ’œ */
    uint16_t io9_c_can2     :1;     /* IO9∞ÂC±≥∞ÂCAN1π ’œ */
    uint16_t io9_c_in       :1;     /* IO9∞ÂC ‰»Îπ ’œ */
    uint16_t io9_c_ou       :1;     /* IO9∞ÂC ‰≥ˆπ ’œ */
    uint16_t io9_c_lost     :1;     /* IO9∞ÂC∂™ ß */
    
    uint16_t res152         :1;     /* ‘§¡Ù */
    uint16_t res153         :1;     /* ‘§¡Ù */
    uint16_t res154         :1;     /* ‘§¡Ù */
    uint16_t res155         :1;     /* ‘§¡Ù */
    uint16_t res156         :1;     /* ‘§¡Ù */
    uint16_t res157         :1;     /* ‘§¡Ù */
    uint16_t res158         :1;     /* ‘§¡Ù */
    uint16_t res159         :1;     /* ‘§¡Ù */
}MVB_FAULT_STU, *PMVB_FAULT_STU;


typedef struct
{
    uint16_t inBuf[6];
    
    /* Word 6 */
    uint16_t ou1            :1;
    uint16_t ou2            :1;
    uint16_t ou3            :1;
    uint16_t ou4            :1;
    uint16_t ou5            :1;
    uint16_t ou6            :1;
    uint16_t ou7            :1;
    uint16_t ou8            :1;
    
    uint16_t ou9            :1;
    uint16_t ou10           :1;
    uint16_t ou11           :1;
    uint16_t ou12           :1;
    uint16_t ou13           :1;
    uint16_t ou14           :1;
    uint16_t ou15           :1;
    uint16_t ou16           :1;
    
    /* Word 7 */
    uint16_t ou17           :1;
    uint16_t ou18           :1;
    uint16_t ou19           :1;
    uint16_t ou20           :1;
    uint16_t ou21           :1;
    uint16_t ou22           :1;
    uint16_t ou23           :1;
    uint16_t ou24           :1;
    
    uint16_t ou25           :1;
    uint16_t ou26           :1;
    uint16_t ou27           :1;
    uint16_t ou28           :1;
    uint16_t ou29           :1;
    uint16_t ou30           :1;
    uint16_t ou31           :1;
    uint16_t ou32           :1;
    
    /* Word 8 */
    uint16_t ou33           :1;
    uint16_t ou34           :1;
    uint16_t ou35           :1;
    uint16_t ou36           :1;
    uint16_t ou37           :1;
    uint16_t ou38           :1;
    uint16_t ou39           :1;
    uint16_t ou40           :1;
    
    uint16_t ou41           :1;
    uint16_t ou42           :1;
    uint16_t ou43           :1;
    uint16_t ou44           :1;
    uint16_t ou45           :1;
    uint16_t ou46           :1;
    uint16_t ou47           :1;
    uint16_t ou48           :1;
    
    /* Word 9 */
    uint16_t sw1            :1;
    uint16_t sw2            :1;
    uint16_t sw3            :1;
    uint16_t sw4            :1;
    uint16_t sw5            :1;
    uint16_t sw6            :1;
    uint16_t sw7            :1;
    uint16_t sw8            :1;
    uint16_t sw9            :1;
    uint16_t sw_res         :7;
    
    uint16_t res[4];
    uint16_t mcuVer;
    uint16_t lifesign;
}MVB_STS_STU, *PMVB_STS_STU;


void mvb_flt_data(uint8_t* buf);
void mvb_sts_data(uint8_t* buf);


#ifdef __cplusplus
}
#endif

#endif

