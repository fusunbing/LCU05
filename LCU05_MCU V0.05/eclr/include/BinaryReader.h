/*
+------------------------------------------------------------------------------+
| ****************************** BINARYREADER.H ****************************** |
+------------------------------------------------------------------------------+
|                       Copyright 2005 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| Description: Reading ints, floats, strings, structs... from a byte stream.   |
|                                                                              |
| Author: Uwe Thomas, started at: 21.10.2005                                   |
+------------------------------------------------------------------------------+
*/

//TODO: implement missing data types like float, structs, array of structs?

#ifndef __BINARYREADER_H__
#define __BINARYREADER_H__

#define REMOTING_READER_WRITER_NEWSTYLE

class CRemotingServer;

class CBinaryReader2 : public ClrAllocator
{
public:
    enum StringEncoding
    {
        Ansi    = 1,
        Utf8    = 2,
        Utf16   = 3
    };

    CBinaryReader2();
    ~CBinaryReader2();

    uint8  ReadUint8();
    uint16 ReadUint16();
    uint32 ReadUint32();
    void   ReadUint64(ClrInt64& val);
    double ReadDouble()
    {
        ClrInt64 val;
        ReadUint64(val);
        return *((double*)&val);
    }
    float ReadFloat()
    {
        uint32 result = ReadUint32();
        return *((float*)&result);
    }

    unsigned ReadString(void* pString, unsigned length);
    unsigned ReadStringUtf16(uint16* pString, unsigned length);
    void ReadString();

    int Recv(void* pBuffer, unsigned len);
    int Recv();
    void Read(void* pBuffer, unsigned length);
    void Read(unsigned length);

    inline sint8 ReadSint8()
    {
        return (sint8)ReadUint8();
    }
    inline sint16 ReadSint16()
    {
        return (sint16)ReadUint16();
    }
    inline sint32 ReadSint32()
    {
        return (sint32)ReadUint32();
    }
    inline unsigned ReadStringLength()
    {
        return (unsigned)ReadUint16();
    }
    inline unsigned ReadArrayLength()
    {
        return (unsigned)ReadUint16();
    }
    void ReadObject(int dataType, ClrInt64& val);
    unsigned ReadObjectString(void* pString, unsigned length, int& encoding);

    inline bool IsEstablished()
    {
        return !m_isTerminated;
    }
    inline bool IsTerminated()
    {
        return m_isTerminated;
    }
    inline unsigned Avail()
    {
        return m_avail;
    }
    inline void SetConnection(CRemotingServer* pConnection, EclrRemotingSocket* pSocket)
    {
        m_pConnection   = pConnection;
        m_pSocket       = pSocket;
    }
	inline static void SetRecvTimeout (unsigned timeout)
	{
        s_recvTimeout = timeout;
	}
private:
    static unsigned s_recvTimeout;
    void recvInternal();

    CRemotingServer*    m_pConnection;
    EclrRemotingSocket* m_pSocket;
    uint8*              m_pBuffer;
    uint8*              m_pWrite;
    uint8*              m_pRead;
    unsigned            m_bufferSize;
    unsigned            m_avail;
    uint8               m_buffer[256];
    bool                m_isTerminated;
    uint32              m_received;	
};

class CBinaryReader : public ClrAllocator
{
public:
    CBinaryReader();
    ~CBinaryReader();

private:
    const char * m_pBuffer;
    const char * m_pBehindBuffer;
    const char * m_pActPosition;
	

	

public:
    bool m_bBufferOverrun;

    inline sint32 getBufferRest()
    {
        return(m_pBehindBuffer - m_pActPosition);
    }

    inline void setByteBuffer(const char* const buf, sint32 length, long actPos = 0)
    {   
        m_pBuffer = buf;
        m_pBehindBuffer = buf+length;
        m_pActPosition = &m_pBuffer[actPos];
        m_bBufferOverrun = false;
    };

    inline sint32 getActPosition()
    {   return (m_pActPosition - m_pBuffer);
    };

    inline const char*getActPositionPointer()
    {   return (m_pActPosition);
    };

    inline uint32 alignActPosition(uint16 alignement)
    {
        uint32 padding = 0;
        if(alignement > 1)
        {   while(((long)m_pActPosition  & (alignement - 1) ) != 0 && m_pActPosition < m_pBehindBuffer)
            {   padding++;
                m_pActPosition++;
            }
        }
        return(padding);
    }

    void setActPosition(sint32 pos);

    const char* const readBytes(sint32 count);


    sint8 readSint8Compact();
    uint8 readUint8Compact();


    // for online communication sint8 are stored as 16Bit
    inline sint8 readSint8(bool arrayValue = false)
    {
        if(arrayValue)
        {   return readSint8Compact();
        }
        return (sint8)readSint16();
    }
    // for online communication uint8 are stored as 16Bit
    inline uint8 readUint8(bool arrayValue = false)
    {
        if(arrayValue)
        {   return readUint8Compact();
        }
        return (uint8)readSint16();
    }
    
    uint16 readUint16();
    uint32 readUint32();
    bool readUint64(ClrInt64& val);
    sint16 readSint16();
    sint32 readSint32();
    float readFloat()
    {
        uint32 result = readUint32();
        return *((float*)&result);
    }
    double readDouble()
    {
        ClrInt64 val;
        readUint64(val);
        return *((double*)&val);
    }
    const char* const readString();
    //If the result fits into 32 Bit the result is returned directly
    //otherwise (string, Int64, double) a pointer to the data is returned.
    uint32 readObject(uint16 dataType, bool arrayValue = false);
	
    //void changeTheEndianOfTheHeader(char* m_pBuffer);
	
	short SHORT_change_endian(short i)
	{   
	    short retVal;      // compiler ensures the alignment of this variable 

        *(((char*)&retVal)+1) = *(((char*)&i)+0);
        *(((char*)&retVal)+0) = *(((char*)&i)+1);

        return(retVal);

        // return ((i>>8)&0xff)+((i << 8)&0xff00); 
	}

	int INT_change_endian(int i)
	{   
        int retVal;       // compiler ensures the alignment of this variable  

        *(((char*)&retVal)+3) = *(((char*)&i)+0);
        *(((char*)&retVal)+2) = *(((char*)&i)+1);
        *(((char*)&retVal)+1) = *(((char*)&i)+2);
        *(((char*)&retVal)+0) = *(((char*)&i)+3);  
        
        return(retVal);

	    // return((i&0xff)<<24)+((i&0xff00)<<8)+((i&0xff0000)>>8)+((i>>24)&0xff); 	
	}

	// swap 64Bit datatype
#ifdef PLATFORM_LONGLONG_SUPPORT
	void LONG_LONG_change_endian(unsigned long long& ull)
	{   
	    unsigned long long retVal;    // compiler ensures the alignment of this variable

        *(((char*)&retVal)+7) = *(((char*)&ull)+0);
        *(((char*)&retVal)+6) = *(((char*)&ull)+1);
        *(((char*)&retVal)+5) = *(((char*)&ull)+2);
        *(((char*)&retVal)+4) = *(((char*)&ull)+3);
        *(((char*)&retVal)+3) = *(((char*)&ull)+4);
        *(((char*)&retVal)+2) = *(((char*)&ull)+5);
        *(((char*)&retVal)+1) = *(((char*)&ull)+6);
        *(((char*)&retVal)+0) = *(((char*)&ull)+7);

        ull = retVal;

//		ull = (ull >> 56) | 
//		((ull<<40) && 0x00FF000000000000) | 
//		((ull<<24) && 0x0000FF0000000000) | 
//		((ull<<8) && 0x000000FF00000000) | 
//		((ull>>8) && 0x00000000FF000000) | 
//		((ull>>24) && 0x0000000000FF0000) | 
//		((ull>>40) && 0x000000000000FF00) | 
//		(ull << 56);  
	}
#endif // PLATFORM_LONGLONG_SUPPORT 
};
#endif // __BINARYREADER_H__
