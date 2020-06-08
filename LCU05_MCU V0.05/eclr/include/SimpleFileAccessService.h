/*
+------------------------------------------------------------------------------+
| *************************** DOWNLOADSERVICE.H ****************************** |
+------------------------------------------------------------------------------+
|                       Copyright 2005 by KW Software GmbH                     |
|                                                                              |
|                       KW Software GmbH                                       |
|                       Lagesche Str. 32                                       |
|                       D-32657 Lemgo                                          |
|                       Germany                                                |
|                                                                              |
+------------------------------------------------------------------------------+
| Handling of download services                                                |
+------------------------------------------------------------------------------+
| Author: Jörg Kraemer, started at: 29.09.2005                                 |
| Changes have to be marked with an user time stamp (e.g. '// NN, 08.11.2003') |
+------------------------------------------------------------------------------+
*/

#ifndef __SIMPLEFILEACCESS_SERVICE_H__
#define __SIMPLEFILEACCESS_SERVICE_H__

#include "Services.h"

  typedef enum _FileError
  {
      FileError_NoError=0x0,
      FileError_DiskFull=0x1,
      FileError_WriteProtection=0x2,
      FileError_FileNotFound=0x3,
      FileError_DeviceNotFound=0x4,
      FileError_FileOpened=0x5,
  } FileError;

class CSimpleFileAccessService : public CServiceImplBase
{
public:
    CSimpleFileAccessService();
    ~CSimpleFileAccessService();
    
    void Invoke(sint16 methodHandle, char* const pDataBuffer);

    static CServiceImplBase* CreateInstance();

protected:

	//Service method: FileError BeginDownload(Boolean beginTransaction, String fileName, Int32 length, Byte[] data, out Int16& downloadHandle);
    void BeginDownload();

   //Service method: void Download(Int16 downloadHandle, Byte[] data);
    void Download();

   //Service method: FileError EndDownload(Boolean endTransaction, Int16 downloadHandle);
    void EndDownload();

   //Service method : FileError FullUpload(String fileName, out Byte[]& data);
    void FullUpload();

    void releaseUploadStream();
    void releaseDownloadStream();
    ClrUploadStream*    m_pUploadStream;
    ClrDownloadStream*  m_pDownloadStream;
    unsigned            m_uploadWriteCount;
    bool                m_isService2;
    bool                m_isPcosDownload;

};

class CSimpleFileAccessService2 : public CSimpleFileAccessService
{
public:
    CSimpleFileAccessService2();
    ~CSimpleFileAccessService2();

    void Invoke(sint16 methodHandle, char* const pDataBuffer);

    static CServiceImplBase* CreateInstance();

protected:

    //Service method: Int64 GetFileTime(String fileName);
    void GetFileTime();

   //Service method: FileError Delete(String fileName);
    void Delete();

    //Service method : FileError SegmentedUploadContinue(out Byte[]& data, out Boolean& finished);
    void SegmentedUploadContinue();

    //Service method : FileError SegmentedUploadStart(String fileName, out Byte[]& data, out Int32& fileLength, out Boolean& finished);
    void SegmentedUploadStart();

private:
    bool processSegmentedUploadStream(bool& finished);
 
};

class CFileUploadStream : public ClrUploadStream
{
public:
    CFileUploadStream(const char* pName);
    virtual ~CFileUploadStream();
    bool IsOpen()
    {
        return (m_pFile != NULL) ? true : false;
    }
    // virtual override
    int read(void* pData, unsigned length);
private:
    EclrFile* m_pFile;

};

class CFileDownloadStream : public ClrDownloadStream
{
public:
    CFileDownloadStream(const char* pName);
    virtual ~CFileDownloadStream();
    bool IsOpen()
    {
        return (m_pFile != NULL) ? true : false;
    }
   // virtual override
    int write(void* pData, unsigned length);
    bool terminate();

private:
    EclrFile* m_pFile;

};
#endif //__SIMPLEFILEACCESS_SERVICE_H__
