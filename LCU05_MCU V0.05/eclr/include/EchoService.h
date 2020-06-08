#ifndef __ECHOSERVICE_H__
#define __ECHOSERVICE_H__

#include "Services.h"
#include "CallbackBase.h"
#include "math.h"


// Base class for Ade.Internal.Remoting.Services.IEchoService Service implementations
////////////////////////////////////////////
class CEchoService : public CServiceImplBase
{
public:
    CEchoService();
    ~CEchoService();

	static CServiceImplBase* CreateInstance();

    void Invoke(sint16 methodHandle, char* const pDataBuffer);
    void Continue(int methodHandle);

private:
   //Service method [1]: Int32[] EchoInt32(Int32[] intArray);
    void EchoInt32();
   //Service method [2]: String[] EchoString(String[] stringArray);
    void EchoString();
   //Service method [3]: int FileOpen(String[UTF8] filename);
    void FileOpen();
   //Service method [4]: Byte[] FileRead(int handle, int length);
    void FileRead();
   //Service method [5]: int FileWrite(int handle, Byte[] data);
    void FileWrite();
   //Service method [6]: void FileClose(int handle);
    void FileClose();

    // additional methods for yield pattern
    void FileReadContinue();

    EclrFile*   m_pFile;
    int         m_readLengthLeft;
};


#endif // __ECHOSERVICE_H__

