#ifndef __CONNLAYER_H__
#define __CONNLAYER_H__

#include "esocket.h"

#if (PLATFORM_TARGET_ENDIANESS == TARGET_BIG_ENDIAN)
void swapEndianess(uint16 &usValue);
void swapEndianess(uint32 &ulValue);
void swapEndianess(unsigned long long& ullValue);
#endif // #if (PLATFORM_TARGET_ENDIANESS == TARGET_BIG_ENDIAN)
uint16 getBits(uint16 value, int position, int numberOfBits);

class ConnPacket
{
public:
    enum Commands               // attention: this enum is used as uint16
    {
        ConnectRequ         = 1,
        ConnectResp         = 2,
        Push                = 3,
        Ack                 = 4,
        Retr                = 5,
        CommandEnd          = 31
    };


    uint16  m_port;             // destination port (eCLR 41100)    
    uint16  m_hxsum;            // header IP checksum    
	uint16  m_connectionFlags;
    //uint16
    //        m_connid   : 8,     // different connection id  (up to 255 concurrent connections possible)
    //        m_flag1    : 1,     // 
    //        m_flag2    : 1,     // 
    //        m_flag3    : 1,     //  
    //        m_cmd      : 5;     // connection layer commands

    uint16  m_seqFlags;
	//uint16
            //m_seq      : 8,     // send sequence counter
            //m_ack      : 8;     // recv sequence counter
    uint16  m_dxsum;            // data IP checksum    
    uint16  m_length;           // data length
    // sizeof 12

	void PutConnId(uint16 value)
	{
		m_connectionFlags |= value;
	}
	void PutFlag1(uint16 value)
	{
		m_connectionFlags |= value << 8;
	}
	void PutFlag2(uint16 value)
	{
		m_connectionFlags |= value << 9;
	}
	void PutFlag3(uint16 value)
	{
		m_connectionFlags |= value << 10;
	}
	void PutCmd(uint16 value)
	{
		m_connectionFlags |= value << 11;
	}
	void PutSeq(uint16 value)
	{
		m_seqFlags |= value;
	}
	void PutAck(uint16 value)
	{
		m_seqFlags |= value << 8;
	}

	uint8 GetConnId()
	{
		return (uint8)getBits(m_connectionFlags, 0, 8);
	}
	uint16 GetFlag1()
	{
		return getBits(m_connectionFlags, 8, 1);
	}
	uint16 GetFlag2()
	{
		return getBits(m_connectionFlags, 9, 1);
	}
	uint16 GetFlag3()
	{
		return getBits(m_connectionFlags, 10, 1);
	}
	uint8 GetCmd()
	{
		return (uint8)getBits(m_connectionFlags, 11, 5);
	}
	uint8 GetSeq()
	{
		return (uint8)getBits(m_seqFlags, 0, 8);
	}
	uint8 GetAck()
	{
		return (uint8)getBits(m_seqFlags, 8, 8);
	}
#if (PLATFORM_TARGET_ENDIANESS == TARGET_BIG_ENDIAN)
	void SwapEndianess()
	{
		swapEndianess(m_port);
		swapEndianess(m_hxsum);
		swapEndianess(m_connectionFlags);
		swapEndianess(m_seqFlags);
		swapEndianess(m_dxsum);
		swapEndianess(m_length);
	}
#endif // #if (PLATFORM_TARGET_ENDIANESS == TARGET_BIG_ENDIAN)
};

struct RoutingHeader
{
    uint16  m_dport;
    uint16  m_dstHigh;
    uint32  m_dst;
    uint16  m_sport;
    uint16  m_srcHigh;
    uint32  m_src;

    // sizeof 16
};

struct ConnectPacket
{
    uint16  m_maxSegSize;
	uint16  m_unionData;
    //union
    //{
    //    uint16  
    //            m_dynamicConn   : 1,        // SYN/FIN support
    //            m_unused        : 11,       // unused                    
    //            m_compatid      : 4;        // compatability id 
    //    uint16  m_mode;
    //};         
    // sizeof 4
	void PutDynammicConn(uint16 value)
	{
		m_unionData |= value;
	}
	void PutCompatId(uint16 value)
	{
		m_unionData |= value << 12;
	}
	void PutMode(uint16 value)
	{
		m_unionData = value;
	}
	uint16 GetDynamicConn()
	{
		return getBits(m_unionData, 0, 1);
	}
	uint16 GetCompatId()
	{
		return getBits(m_unionData, 12, 4);
	}
	uint16 GetMode()
	{
		return m_unionData;
	}
#if (PLATFORM_TARGET_ENDIANESS == TARGET_BIG_ENDIAN)
	void SwapEndianess()
	{
		swapEndianess(m_maxSegSize);
		swapEndianess(m_unionData);
	}
#endif // #if (PLATFORM_TARGET_ENDIANESS == TARGET_BIG_ENDIAN)
};
class ConnTimer
{
public:
    ConnTimer();
    void Start(unsigned delay_ms);
    void Stop()
    {
        m_running = false;
    }
    bool isElapsed();
    bool isRunning()
    {
        return m_running;
    }
private:
    
    uint32      m_start;
    unsigned    m_delay;
    bool        m_running;
};

class ConnStream
{
public:
    ConnStream();
    void Set(void* pBase, unsigned capacity);
    void Start(unsigned tmout_ms);
    void StartTimer(unsigned tmout_ms);
    inline void StopTimer()
    {
        m_timeout.Stop();
    }
    void* GetBase()
    {
        return m_pBase;
    }
    void Clear();

    inline unsigned Capacity()
    {
        return m_capacity;
    }
    inline void SetCapacity(unsigned cap)
    {
        m_capacity = cap;
    }
    bool isTimeout()
    {
        return m_timeout.isElapsed();
    }
protected:
    ConnTimer   m_timeout;
    uint8*      m_pBase;
    unsigned    m_capacity;
    unsigned    m_rptr;
    unsigned    m_wptr;
public:
    uint8       m_seq;
};
class ConnInput : public ConnStream
{
public:
#ifdef REMOTING_SOCKET_NEWSTYLE
    bool Process(EclrRemotingSocket* pStream);
#else
    bool Process(ESocket* pStream);
#endif
    int  Read(void* pBuffer, int length);
    inline unsigned Size()
    {
        return m_wptr;
    }
    unsigned LeftToRead()
    {
        return m_capacity - m_rptr;
    }
private:
};
class ConnOutput : public ConnStream
{
public:
#ifdef REMOTING_SOCKET_NEWSTYLE
    bool Process(EclrRemotingSocket* pStream);
#else
    bool Process(ESocket* pStream);
#endif
    int  Write(void* pBuffer, int length);
    inline unsigned Size()
    {
        return m_wptr;
    }    
    void Restart()
    {
        m_rptr = 0;
    }
    unsigned LeftToProcess()
    {
        return m_capacity - m_rptr;
    }
private:
};


class ConnData;
#ifdef REMOTING_SOCKET_NEWSTYLE
class ConnLayer : public EclrRemotingSocket
{
    public :
    enum OpenMode
    {
        OpenPassive ,
        OpenActive
    };
    enum AcceptMode
    {
        SingleConnection,
        MultipleConnections
    };
    enum ConnLayerSpec
    {
        CompatId    = 1,
        SpecAny     
    };
    enum ConnLayerState
    {
        StateIdle           = 0,
        StateReceiveHeader  = 1,
        StateReceiveData    = 2,
        StateReadData       = 3,
        StateAny     
    };
    ConnLayer();
    virtual ~ConnLayer();

    // virtual adapt EclrSocket
    EclrRemotingSocket* Open(BlockingMode mode, bool& wouldblock);
    int Send(const void* buf, int len);
    int Recv(void* buf, int len);
    void Close();
    // obsolete
    void setNonBlocking();
    void setNonBlockingSemantic() {}

    // 
#ifdef REMOTING_SOCKET_NEWSTYLE
    bool Open(EclrRemotingSocket* pStream, unsigned maxSegSize);
#else
    bool Open(ESocket* pStream, unsigned maxSegSize);
#endif
    bool Connect();
    bool isConnected()
    {
        return m_connEstablished;
    }
    void Process();
    void SetRetransmitionTimeout(unsigned tmout_ms)
    {
        m_retransTimeout = tmout_ms;
    }
    unsigned GetTxSize()
    {
        return m_txSize;
    }
    private:


    bool writeData(void* pBuffer, int length);
    bool writeHeader(uint16 cmd, int length = 0, uint16 dxsum = 0);
    bool recvHeader(bool& error);
    bool calcHeader(ConnPacket* pHeader, uint16 cmd, int length, uint16 dxsum);
    bool recvData(bool& abort);
    bool readData(void* pBuffer, int length);
    bool checkRxHeader();
    bool checkRxData();
    bool writeConnRequ();
    bool writeConnResp();
    ConnData* getConnection(unsigned id);
    ConnData* createConnection(unsigned id);
    void send();
    void clearInput(unsigned timeout_ms = 0);
    void clearInput(int length, unsigned timeout_ms);
    void sendCntl(uint16 cmd);
    void init();
    void setStateReceiveHeader();

#ifdef REMOTING_SOCKET_NEWSTYLE
    EclrRemotingSocket*    m_pStream;  
#else
    ESocket*    m_pStream;  
#endif
    ConnData*   m_pConnDataVec[16];
    ConnData*   m_pCurrentConn;
    uint8       m_seq;
    uint8       m_ack;

    unsigned    m_txSize;
    unsigned    m_rxSize;
    bool        m_acceptSupport;
    bool        m_isActive;
    bool        m_connEstablished;
    bool        m_connRequested;
    bool        m_connResponsed;

    ConnPacket  m_rxConnPacket;
    ConnPacket  m_cntlPacket;

    ConnTimer   m_retransTimer;
    unsigned    m_retransCounter;
    unsigned    m_retransTimeout;

    uint16      m_rxBuffer[1024];
    uint16      m_txBuffer[1024];
    ConnInput   m_hInput;
    ConnInput   m_dInput;
    ConnOutput  m_dOutput;
    ConnOutput  m_cntlOutput;

    enum ConnLayerState   m_state;
};
#else
class ConnLayer : public ESocket
{
    public :
    enum OpenMode
    {
        OpenPassive ,
        OpenActive
    };
    enum AcceptMode
    {
        SingleConnection,
        MultipleConnections
    };
    enum ConnLayerSpec
    {
        CompatId    = 1,
        SpecAny     
    };
    enum ConnLayerState
    {
        StateIdle           = 0,
        StateReceiveHeader  = 1,
        StateReceiveData    = 2,
        StateReadData       = 3,
        StateAny     
    };
    ConnLayer();
    virtual ~ConnLayer();

    // virtual adapt EclrSocket
    bool Close();
    int Send(const void* buf, int len);
    int Recv(void* buf, int len);
    ESocket* Accept();
    void setNonBlocking();

    // 
    bool Open(ESocket* pStream, unsigned maxSegSize);
    bool Connect();
    bool isConnected()
    {
        return m_connEstablished;
    }
    void Process();
    void SetRetransmitionTimeout(unsigned tmout_ms)
    {
        m_retransTimeout = tmout_ms;
    }
    unsigned GetTxSize()
    {
        return m_txSize;
    }
    private:


    bool writeData(void* pBuffer, int length);
    bool writeHeader(int cmd, int length = 0, uint16 dxsum = 0);
    bool recvHeader(bool& error);
    bool calcHeader(ConnPacket* pHeader, int cmd, int length, uint16 dxsum);
    bool recvData(bool& abort);
    bool readData(void* pBuffer, int length);
    bool checkRxHeader();
    bool checkRxData();
    bool writeConnRequ();
    bool writeConnResp();
    ConnData* getConnection(unsigned id);
    ConnData* createConnection(unsigned id);
    void send();
    void clearInput(unsigned timeout_ms = 0);
    void clearInput(int length, unsigned timeout_ms);
    void sendCntl(int cmd);
    void init();
    void setStateReceiveHeader();

    ESocket*    m_pStream;  
    ConnData*   m_pConnDataVec[16];
    ConnData*   m_pCurrentConn;
    uint8       m_seq;
    uint8       m_ack;

    unsigned    m_txSize;
    unsigned    m_rxSize;
    bool        m_acceptSupport;
    bool        m_isActive;
    bool        m_connEstablished;
    bool        m_connRequested;
    bool        m_connResponsed;

    ConnPacket  m_rxConnPacket;
    ConnPacket  m_cntlPacket;

    ConnTimer   m_retransTimer;
    unsigned    m_retransCounter;
    unsigned    m_retransTimeout;

    uint16      m_rxBuffer[1024];
    uint16      m_txBuffer[1024];
    ConnInput   m_hInput;
    ConnInput   m_dInput;
    ConnOutput  m_dOutput;
    ConnOutput  m_cntlOutput;

    enum ConnLayerState   m_state;
};
#endif

class ConnData : public ESocket
{
    public :
    ConnData(ConnLayer* pConnLayer);
    virtual ~ConnData();

    // virtual adapt EclrSocket
    bool Close();
    int Send(const void* buf, int len);
    int Recv(void* buf, int len);
    ESocket* Accept();
    void setNonBlocking();    

    // 
    private:

    ConnLayer*  m_pConnLayer;
};


#endif // __CONNLAYER_H__

