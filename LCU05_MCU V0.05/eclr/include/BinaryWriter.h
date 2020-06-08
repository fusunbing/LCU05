/*
+------------------------------------------------------------------------------+
| ****************************** BINARYWRITER.H ****************************** |
+------------------------------------------------------------------------------+
|                       Copyright 2005 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| Description: Write to communication buffer                                   |
|                                                                              |
| Author: Jörg Kraemer, started at: 28.11.2005                                 |
+------------------------------------------------------------------------------+
*/

#ifndef __BINARY_WRITER_H__
#define __BINARY_WRITER_H__

#include "Tools.h"
#define SAFETY_BUFFER_LENGTH 20 // when filling buffer manual lest for return value , etc.

class CRemotingServer;

class CBinaryWriter2 : public ClrAllocator
{
public:
    CBinaryWriter2();
    ~CBinaryWriter2();

    void WriteUint8(uint8 value);
    void WriteUint16(uint16 value);
    void WriteUint32(uint32 value);
    void WriteUint64(ClrInt64& val);
    inline void WriteSint8(sint8 value)
    {
        WriteUint8((uint8)value);
    }
    inline void WriteSint16(sint16 value)
    {
        WriteUint16((uint16)value);
    }
    inline void WriteSint32(sint32 value)
    {
        WriteUint32((uint32)value);
    }
    inline void WriteFloat(float value)
    {
        WriteUint32(*((uint32*)&value));
    }
    inline void WriteDouble(double value)
    {
        WriteUint64(*((ClrInt64*)&value));
    }

	inline void WriteStringLength(unsigned length)
    {
        WriteUint16((uint16)length);
    }
    inline void WriteArrayLength(unsigned length)
    {
        WriteUint16((uint16)length);
    }
	void WriteString(const void* pString, unsigned length);
    void WriteObject(int dataType, uint32 val);
    void WriteObject(int dataType, ClrInt64& val);
    void WriteObjectString(int stringEncoding, const void* pString, unsigned length);

    void WriteConfirmation();
    void WriteConfirmation(uint32 errorCode, const char* pErrorMsg = NULL);

    void Write(const void* pBuffer, unsigned length);
    void Flush();

    inline bool IsEstablished()
    {
        return !m_isTerminated;
    }
    inline bool IsTerminated()
    {
        return m_isTerminated;
    }
    void Switch();
    void Abort(int abortCode);
    inline void SetConnection(CRemotingServer* pConnection, EclrRemotingSocket* pSocket)
    {
        m_pConnection   = pConnection;
        m_pSocket       = pSocket;
    }
	inline static void SetSendTimeout (unsigned timeout)
	{
        s_sendTimeout = timeout;
	}
private:
	int Send(const void* pBuffer, unsigned length);
    static unsigned s_sendTimeout;
    CRemotingServer*    m_pConnection;
    EclrRemotingSocket* m_pSocket;
    uint8*              m_pBuffer;
    uint8*              m_pWrite;
    uint8*              m_pRead;
    unsigned            m_bufferSize;
    unsigned            m_avail;
    uint8               m_buffer[1200];
    bool                m_isTerminated;
    bool                m_isError;

};

class CBinaryWriter : public ClrAllocator
{
public:
    CBinaryWriter();
    ~CBinaryWriter();
    inline void SetConnection(CRemotingServer* pConnection, EclrRemotingSocket* pSocket)
    {
        m_pRemotingServer = pConnection;
        m_pSocket = pSocket;
    }
	inline static void SetSendTimeout (unsigned timeout)
	{
        s_sendTimeout = timeout;
	}
private:


    bool m_bFixedDataUpload; // Flag wether a upload of a eClr MemoryBlock is active
    bool m_bConstructedDataUpload; // Flag whether a upload of a Data collected from clr (e.g. variable groups
    bool m_bBufferingSend; // Flag indicates whether the data will be sent to socket or buffered 


    char * m_pBuffer;
    sint32 m_nBufferLength;
    sint32 m_nBufferPosition;
    sint32 m_nStartPosition;
    uint32 m_nSendData; // in diesem Command gesendete Daten
    sint32 m_nLastConstructedArrayLengthPosition; // Position im Straem für erzeugte ByteArrays
    sint16* m_pnActWrittenLength;
    uint16* m_pnSendLength;
    sint32* m_pnByteArrayLength;
    bool m_bFirstBuffer; // Flag whether first buffer
    bool m_bErrorSent; // Flag whether error is sent

    sint16 m_nActServiceHandle;           // Handle of service actual invoked
    sint16 m_nActMethodHandle;            // Handle of method actual invoked
    sint16 m_nActInvokeID;                // not used in Step 1



    // Sets the actual Socket in binary Writer
    EclrRemotingSocket* m_pSocket;
	unsigned static s_sendTimeout;
private:
    bool SendData(char *pBuffer, sint32 count);
	
public:
    // Tools for direct byte array writing
    char * GetByteWriteBufferPointer(uint32& maxBytes); // Pointer where to write and maximal possible number of bytes to write
    uint32* GetByteArrayLengthPointer(); // reserved field for byte array length
    void SetWrittenBytes(uint32 writtenBytes);
    bool  AddErrorObject( uint16 Typeint32, void* vpErrorValue);

    uint32 m_nErrorNumber;
    CRemotingServer* m_pRemotingServer;

    void inititaliseBuffer(char* buf);
    void inititaliseHeaderData(CommandHeader* pCommandHeader);
    void setHeaderData(CommandHeader* pCommandHeader);

    void postInititaliseBufferForUpload();

    void resetBuffer();

    void setSendLength(bool last);

    void setByteArrayLength(bool last = false);

    void SendError(uint32 errorNumber, const char* const message = NULL);
    void SendError(uint32 errorNumber, uint16 serviceHandle, uint16 methodHandle, const char* const message = NULL);

    
    inline void setByteBuffer(char* buf)
    {   m_pBuffer = buf;
    };

    inline void setBufferLength(sint32 length)
    {   m_nBufferLength = length;
    };

    inline void setActPosition(sint32 pos)
    {   m_nBufferPosition = pos; 
    };

    inline sint32 getActPosition()
    {   return((m_pBuffer != NULL) ? m_nBufferPosition : -1 );
    };

    inline sint32 getCapacity()
    {   return((m_pBuffer != NULL) ? m_nBufferLength - m_nBufferPosition : -1 );
    };

    inline  bool isErrorSent() {    return(m_bErrorSent); }; // Flag whether error is sent

    sint32 writeBytes(const char* const source, sint32 count, bool start=false);

    bool writeSint8Compact(sint8 value);
    bool writeUint8Compact(uint8 value);
    
    inline bool writeSint8(sint8 value, bool arrayValue = false)
    {
        if(arrayValue)
        {   
           return  (writeUint8Compact((uint8)value));
        }
        return writeSint16((sint16) value);
    }

    inline bool writeUint8(uint8 value, bool arrayValue = false)
    {
        if(arrayValue)
        {   
           return  (writeUint8Compact(value));
        }
        return writeUint16((uint16) value);
    }
    bool writeSint16(sint16 value);
    inline bool writeUint16(uint16 value)
    {
        return writeSint16((sint16)value);
    }
    bool writeSint32(sint32 value);
    inline bool writeUint32(uint32 value)
    {
        return writeSint32((sint32)value);
    }

    bool writeSint64(const ClrInt64& value);// UT, 2005.11.28 
    inline bool writeUint64(const ClrInt64& value)
    {
        return writeSint64(value);
    }
    inline bool writeFloat(float value)
    {
        float val=value;
        return writeSint32(*((sint32*)&val)); //prevent double to sint64 conversion
    }

    inline bool writeDouble(double value)
    {
        double val = value;
        return writeSint64(*((ClrInt64*)&val)); //prevent double to sint64 conversion
    }

    bool writeString(const char* const str);
    bool writeObject(uint16 objectTypeId, uint32 data); //UT, 2006.03.03
    bool Flush(bool last = false); // Sends the Buffer over socket and resets all Pointer

    void writeErrorProlog(uint16 serviceHandle, uint16 methodHandle);
    bool writeErrorEpilog();
	
    void changeTheEndianOfTheHeader(char* m_pBuffer);
	
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

#endif //__BINARY_WRITER_H__
