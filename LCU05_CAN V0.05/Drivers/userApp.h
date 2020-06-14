

//add by fusunbing

#ifndef _USER_APP_H_
#define _USER_APP_H_

#ifdef __cplusplus
extern "C" {
#endif 

#include <rtthread.h>
#include "stm32f4xx.h"


#define SOFTWARE_VERSION    (10) 	//»Ìº˛∞Ê±æ∫≈

#define	SLOT_ID_MVB			(1)
#define	SLOT_ID_CAN			(2)
#define	SLOT_ID_ETU			(3)
#define	SLOT_ID_MCU_A		(4)
#define	SLOT_ID_MCU_B		(5)
#define	SLOT_ID_MCU_C		(6)
#define	SLOT_ID_IO_MIN		(7)
#define	SLOT_ID_IO_MAX		(33)

#define	BOARD_TYPE_ID_MVB	(1)
#define	BOARD_TYPE_ID_MCU	(2)
#define	BOARD_TYPE_ID_CAN	(3)
#define	BOARD_TYPE_ID_ETU	(4)
#define	BOARD_TYPE_ID_IO	(5)


typedef union
{
    struct
    {
        uint8_t bit0    :1;
        uint8_t bit1    :1;
        uint8_t bit2    :1;
        uint8_t bit3    :1;
        uint8_t bit4    :1;
        uint8_t bit5    :1;
        uint8_t bit6    :1;
        uint8_t bit7    :1;
    } Bits;
    
    uint8_t value;
    
}BYTE_TO_BIT, *PBYTE_TO_BIT;


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
}LCU_FAULT_STU, *PLCU_FAULT_STU;


typedef struct
{
    uint16_t lifesign;
    uint8_t rtc[8];
    uint8_t remoteIn[8];
    
    uint8_t devSts[32];
    uint8_t ioSts[32];
}CAN_DATE_STU,*PCAN_DATE_STU;


typedef struct
{
    uint8_t carID;
    uint8_t slotID;
    uint8_t boardTypeID;
    uint8_t version;
    
    uint32_t can1_sts;  //ƒ⁄Õ¯◊¥Ã¨
    uint32_t can2_sts;  //Õ‚Õ¯◊¥Ã¨
    
    CAN_DATE_STU lcu[8];
}DS_STU,*PDS_STU;


extern  DS_STU ds;

void userApp_init(void);
uint32_t* bitBand(uint32_t* addr);


#ifdef __cplusplus
}
#endif

#endif

