#ifndef __SERSOCKET_H__
#define __SERSOCKET_H__

#include "esocket.h"

#ifdef REMOTING_SOCKET_NEWSTYLE
class SerSocket : public EclrRemotingSocket
{
    public :

    enum BaudRate
    {
        Baud_9600   = 9600,
        Baud_19200  = 19200,
        Baud_38400  = 38400,
        Baud_57600  = 57600,
        Baud_115200 = 115200,
        Baud_380400 = 380400
    };

    enum Parity
    {
        Parity_None   = 0,
        Parity_Even   = 1,
        Parity_Odd    = 2
    };
    
    enum StopBits
    {
        StopBits_1   = 0,
        StopBits_1_5 = 1,
        StopBits_2   = 2
    };

    SerSocket();
    virtual ~SerSocket();

    // virtual adapt EclrSocket
    EclrRemotingSocket* Open(BlockingMode mode, bool& wouldblock);
    int Send(const void* buf, int len);
    int Recv(void* buf, int len);
    void Close();

    // obsolete
    void setNonBlocking();
    bool isNonBlocking() { return true; }
    void setNonBlockingSemantic() {}

    // 
    bool Open(int port, enum BaudRate baud, int databits, enum StopBits stopbits, enum Parity parity);
#ifdef SERSOCKET_BREAK_SUPPORT
    bool SendBreak( unsigned int breakOnMs, unsigned int breakOffMs );
#endif

    private:
    bool isOpen();
#ifdef SERSOCKET_BREAK_SUPPORT
    bool Break( unsigned int ms );
#endif

    int     v24Hndl;
};
#else
class SerSocket : public ESocket
{
    public :

    enum BaudRate
    {
        Baud_9600   = 9600,
        Baud_19200  = 19200,
        Baud_38400  = 38400,
        Baud_57600  = 57600,
        Baud_115200 = 115200
    };

    enum Parity
    {
        Parity_None   = 0,
        Parity_Even   = 1,
        Parity_Odd    = 2
    };
    
    enum StopBits
    {
        StopBits_1   = 0,
        StopBits_1_5 = 1,
        StopBits_2   = 2
    };

    SerSocket();
    virtual ~SerSocket();

    // virtual adapt EclrSocket
    bool Close();
    int Send(const void* buf, int len);
    int Recv(void* buf, int len);
    ESocket* Accept();
    void setNonBlocking();

    // 
    bool Open(int port, enum BaudRate baud, int databits, enum StopBits stopbits, enum Parity parity);

#ifdef SERSOCKET_BREAK_SUPPORT
    bool SendBreak( unsigned int breakOnMs, unsigned int breakOffMs );
#endif

    private:
    bool isOpen();
#ifdef SERSOCKET_BREAK_SUPPORT
    bool Break( unsigned int ms );
#endif

    int     v24Hndl;
};
#endif

#endif //#define __SERSOCKET_H__

