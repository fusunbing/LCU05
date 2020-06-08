#include "eclr.h"
#include "sersocket.h"
#include "stm32f4xx_usart.h"



// Buffer for first-in first-out buffer used during read from UART operation.
#define UART_FIFO_BUFFER 2000

// FIFO class for interrupt driver code
class UartFiFo
{
public:
    UartFiFo();

    bool PutChar(uint8 rxChar);
    unsigned Read(void* p, unsigned len);
    inline unsigned Capacity()
    {
        return m_capacity-1;
    }
    void Clear();
private:
    uint8*  m_pWrite;
    uint8*  m_pRead;
    uint8*  m_pEnd;
    unsigned m_capacity;
    uint8 m_buffer[UART_FIFO_BUFFER];
};

UartFiFo::UartFiFo()
{
    m_capacity  = sizeof(m_buffer);
    m_pWrite    = m_buffer;
    m_pRead     = m_buffer;
    m_pEnd      = m_buffer + sizeof(m_buffer);
}

void UartFiFo::Clear()
{
    m_pRead = m_pWrite;
}

bool UartFiFo::PutChar(uint8 rxChar)
{
    bool result = true;
    // 
    *m_pWrite++ = rxChar;
    if (m_pWrite == m_pEnd)
    {
        m_pWrite = m_buffer;
    }

    if (m_pWrite == m_pRead)
    {
        // overflow !!
        result = false;
    }

    return result;
}

unsigned UartFiFo::Read(void* p, unsigned len)
{
    unsigned result = 0;
    while (len > 0)
    {
        unsigned avail = 0;
        uint8* pWrite = m_pWrite;
        if (m_pRead < pWrite)
        {
            avail = pWrite - m_pRead;
        }
        else if (m_pRead > pWrite)
        {
            avail = m_pEnd - m_pRead;
        }

        unsigned n = min(avail, len);
        if (n > 0)
        {
            uint8* pRead = m_pRead;
            memcpy(p, pRead, n);
            pRead += n;
            result += n;
            len -= n;
            p = (uint8*)p + n;
            if (pRead == m_pEnd)
            {
                pRead = m_buffer;
            }
            m_pRead = pRead;
        }
        else
        {
            // no more data
            break;
        }
    }

    return result;

}

static UartFiFo s_uartFiFo;

static int s_rxOverflow = 0;

extern "C" void Uart_CharRxReady(unsigned char uRxChar)
{
    if (s_uartFiFo.PutChar(uRxChar) == false)
    {
        s_rxOverflow++;
    }
}

#ifdef PLATFORM_CTORS_NOTSUPPORTED
inline void* operator new(size_t, void* o)
{
    return o;
}
static void Ctors()
{
    static bool isCtorsExecuted = false;
    if (isCtorsExecuted == false)
    {
        isCtorsExecuted = true;
        ::new (&s_uartFiFo) UartFiFo();
    }
}
#endif //#ifdef PLATFORM_CTORS_NOTSUPPORTED


// ========================================================
// Public stuff.
// ========================================================

SerSocket::SerSocket()
{
#ifdef PLATFORM_CTORS_NOTSUPPORTED
    Ctors();
#endif
    v24Hndl = -1;
}

SerSocket::~SerSocket()
{
}

EclrRemotingSocket* 
SerSocket::Open(BlockingMode mode, bool& wouldblock)
{
    // open/close semantic unused
    return NULL;
}



int
SerSocket::Recv(void* pBuf, int nLen)
{
    int nResult = s_uartFiFo.Read(pBuf, nLen);
    return nResult;
}

void
SerSocket::Close()
{
}

// ========================================================
// Private stuff.
// ========================================================
