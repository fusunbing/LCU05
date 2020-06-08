#include "eclr.h"
#include "sersocket.h"
#include "stm32f4xx_usart.h"

#define ECLR_USART (USART2)

bool SerSocket::Open(int nPort, enum BaudRate baud, int nDatabits, enum StopBits stopbits, enum Parity parity)
{

    USART_InitTypeDef USART_InitStructure;
    bool result = true;

    //USART1_init();
    
    switch (baud)
    {
        case Baud_9600:
            USART_InitStructure.USART_BaudRate = 9600;
            break;
        case Baud_19200:
            USART_InitStructure.USART_BaudRate = 19200;
            break;
        case Baud_38400:
            USART_InitStructure.USART_BaudRate = 38400;
            break;
        case Baud_57600:
            USART_InitStructure.USART_BaudRate = 57600;
            break;
        case Baud_115200:
            USART_InitStructure.USART_BaudRate = 115200;
            break;
        default:
            result = false;
            break;
    }

    switch (parity)
    {
        case Parity_None:
            USART_InitStructure.USART_Parity = USART_Parity_No;
            break;
        case Parity_Even:
            USART_InitStructure.USART_Parity = USART_Parity_Even;
            break;
        case Parity_Odd:
            USART_InitStructure.USART_Parity = USART_Parity_Odd;
            break;
        default:
            result = false;
            break;
    }

    switch (stopbits)
    {
        case StopBits_1:
            USART_InitStructure.USART_StopBits = USART_StopBits_1;
            break;
        case StopBits_1_5:
            USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
            break;
        case StopBits_2:
            USART_InitStructure.USART_StopBits = USART_StopBits_2;
            break;
        default:
            result = false;
            break;
    }
    
    switch (nDatabits)
    {
        case 8:
            USART_InitStructure.USART_WordLength = USART_WordLength_8b;
            break;
        case 9:
            USART_InitStructure.USART_WordLength = USART_WordLength_9b;
            break;
        default:
            result = false;
            break;
    }

    if (true == result)
    {

        // always no flowcontrol !!
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

        //UART_Interrupt Configuration
        USART_ITConfig(ECLR_USART, USART_IT_RXNE, ENABLE);

        // USART configuration 
        USART_Init(ECLR_USART, &USART_InitStructure);
        
        // Enable USART 
        USART_Cmd(ECLR_USART, ENABLE);  
        // Set the serial device handle to the port table
        v24Hndl = nPort;
    }

    return result;
}


int SerSocket::Send(const void* pBuffer, int nLen)
{

    uint8* p = (uint8*)pBuffer;
    int nResult = nLen;

    while (nLen > 0)
    {
        USART_SendData(ECLR_USART, (uint8_t)*p);
        p++;
        nLen--;        
        // Loop until the end of transmission
        while (USART_GetFlagStatus(ECLR_USART, USART_FLAG_TC) == RESET) {}            
    }
    return nResult;
}


//RX
extern "C" void Uart_CharRxReady(unsigned char uRxChar);

