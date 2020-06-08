//=============================================================================
//
//                      Copyright 2009 by KW Software GmbH                     
//                                                                             
//                      KW Software GmbH                                       
//                      Langenbruch 6                                       
//                      D-32657 Lemgo                                          
//                      Germany                                                
//                                                                             
//-----------------------------------------------------------------------------
// Module Name: eCLR image file interface implementation for FReeRTOS
//-----------------------------------------------------------------------------
// Author: Hicham Chaoui                           
//-----------------------------------------------------------------------------
// Change History:
// ----------------------------------------------------------------------------
// 	July. 23th, 2012 by X. Ren - created
//
// ============================================================================

#include "eclr.h"
//#include "file_include.h"

//the f_open function creates a file object to be used to access the file.
//FRESULT f_open ( FIL* FileObject,/* Pointer to the blank file object structure to be created*/
//                 const TCHAR* /*Pointer to the file neme */
//                 BYTE ModeFlags/* Mode flags: Specifies the type of access and open method for the file. It is specified by a combination of following flags: */);
//FA_READ: Specifies read access to the object. Data can be read from the file. Combine with FA_WRITE for read-write access.
//FA_WRITE: Specifies write access to the object. Data can be written to the file.
//FA_OPEN_EXISTING: Opens the file. The function fails if the file is not existing. (Default).
//FA_OPEN_ALWAYS: Opens the file if it is existing. If not, a new file is created.
//FA_CREATE_NEW: Creates a new file. The function fails if the file is already existing.
//FA_CREATE_ALWAYS: Creates a new file. If the file is existing, it is truncated and overwritten.

//FRESULT: FR_OK: The function succeeded and the file object is valid.
//FR_NO_FILE: Could not find the file.
//...

EclrFile* EclrFile::Open(const char* fname , enum OpenFlags mode)
{
	return NULL;
	/*
    FIL *result= (FIL*)malloc(sizeof(FIL));
    if(result==NULL) return NULL;

    FRESULT res = FR_OK;

    switch(mode)
    {
    case Open_r:
        res = f_open(result, fname ,FA_OPEN_EXISTING|FA_READ);
        break;
    case Open_w:
        res = f_open(result, fname ,FA_OPEN_EXISTING|FA_WRITE);
        break;
    case Open_rw:
        res = f_open(result, fname ,FA_OPEN_EXISTING|FA_READ|FA_WRITE);
        break;
    case Open_c:
        res = f_open(result, fname ,FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
        break;
    default:
        result = (FIL*)NULL;
        break;
    }
    
    if (res != FR_OK)
    {
      free((void*)result);
      return NULL;
    }
    else
    {
      return (EclrFile*)result;
    }
    */
}

// The f_unlink removes an object.
//Parameter: FileName: Pointer to the null-terminated string that specifies an object to be removed. 
//Return Values: FR_OK The function succeeded.</dd> 
//               FR_NO_FILE Could not find the file or directory.
//               FR_NO_PATH Could not find the path
//               FR_INVALID_NAME The path name is invalid
//For more details see the usermanual.

bool EclrFile::Remove(const char* fname)
{
    return false;
/*	bool result = false;
    if (f_unlink(fname) == 0)
    {
        result = true;
    }
    return result;
    */
}


bool EclrFile::Close(EclrFile* pFile)
{
    return false;
	/*
	bool result = false;
    if (pFile != NULL)
    {
        f_close((FIL*)pFile);
        free((void*)pFile);
        result = true;
    }
    return result;
    */
}

//FRESULT f_read ( 
//   FIL*  FileObject,   /* Pointer to the file object structure */
//   void* Buffer,       /* Pointer to the buffer to store read data */
//   UINT  ByteToRead,   /* Number of bytes to read */
//   UINT* ByteRead      /* Pointer to the variable to return number of bytes read */); 
int EclrFile::Read(EclrFile* pFile, void* buffer, int size)
{
    return 0;
	/*UINT nRVal = 0;
    FRESULT res;
    int ret;

    res = f_read((FIL*)pFile, buffer, size, &nRVal);
    if(res == FR_OK)
    {
      ret = (int)nRVal;
    }
    else
    {
      ret = 0;
    }
    return ret;*/
}

//FRESULT f_write ( 
//   FIL* FileObject,         /* Pointer to the file object structure */
//   const void* Buffer,      /* Pointer to the data to be written */
//   UINT ByteToWrite,    /* Number of bytes to write */
//   UINT* ByteWritten    /* Pointer to the variable to return number of bytes written */); 
// extern xTaskHandle ThEclrTask;
int EclrFile::Write(EclrFile* pFile, void* buffer, int size)
{   
    return 0;
    /*
	FRESULT res;
    int ret;
    UINT nRVal = 0;
  
    res = f_write((FIL*)pFile, buffer, size, &nRVal); //}

    if(res == FR_OK)
    {
      ret = (int)nRVal;
    }
    else
    {
      ret = 0;
    }
    return ret;
    */
}

//f_lseek: The function moves the file read/write pointer of an open file object. It can also be used to increase the file size (cluster pre-allocation).
//FRESULT f_lseek ( 
//                 FIL* FileObject,   /* Pointer to the file object structure */ 
//                 DWORD Offset       /* File offset in unit of byte */)
long EclrFile::Seek(EclrFile* pFile, long offset, enum SeekModes seekMode)
{
    return NULL;
	/*
	int newSeekMode = 0;
    long retVal = (long)0;

    FIL* fil = (FIL*)pFile;   // File object

    FRESULT res;
    res = f_sync((FIL*)pFile);

    if(retVal != (long)-1 && res == FR_OK)
    {
       if(f_lseek(fil, offset) == FR_OK)
        {
            //retVal = f_tell((FILE*)pFile);
            retVal = offset;
        }
        else
        {
            retVal = (long)-1;
        }
    }
    return retVal;
    */
}
