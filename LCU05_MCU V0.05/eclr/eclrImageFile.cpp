//=============================================================================
//
//                      Copyright 2009 by KW Software GmbH                     
//                                                                             
//                      KW Software GmbH                                       
//                      Lagesche Str. 32                                       
//                      D-32657 Lemgo                                          
//                      Germany                                                
//                                                                             
//-----------------------------------------------------------------------------
// Module Name: eCLR image file interface implementation
//-----------------------------------------------------------------------------
// Author:
//-----------------------------------------------------------------------------
// Change History:
// ----------------------------------------------------------------------------
// 	January. 9th, 2013 by  - created
//
// ============================================================================

#include "eclrImageFile.h"

#ifdef EXECUTION_INPLACE_SUPPORT

#include "stm32f4xx_flash.h"
#include "config.h"


// code area for the PLC program in flash, defined in config.h

const uint32 PcosImageFile::FlashBase = (uint32)(ECLR_PLC_FLASH_START);
const uint32 PcosImageFile::FlashSize = ECLR_PLC_FLASH_SIZE;

const uint32 PcosImageFile::FlashPageSize   = FLASH_PAGE_SIZE;
const uint32 PcosImageFile::FlashSignature  = ECLR_FLASH_SIGNATURE;


// Local adaptation to erase flash sectors used to store application image.
FLASH_Status eraseFlashSectors() 
{
  FLASH_Status nResult = FLASH_COMPLETE;

  FLASH_ClearFlag(FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
  
  nResult = FLASH_EraseSector(FLASH_Sector_10, VoltageRange_3); 
    
  if (FLASH_COMPLETE == nResult || FLASH_BUSY == nResult)
  {
    nResult = FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3);
  }  
    
  return nResult;
  
}


PcosImageFile::PcosImageFile()
{
    m_pFile = NULL;
}

bool PcosImageFile::Open(int mode, unsigned length)
{
    bool result = false;
    FlashHeader* pHeader = (FlashHeader*)FlashBase;
    FLASH_Status nState;
    
    if ((mode & Create) == Create)
    {
        // open for existing image  for writing
        // erase the first page and invalidate the signature
        if (length < GetSize())
        {            
            FLASH_Unlock();// the image fits into the flash area, unlock flash
            nState = eraseFlashSectors();

            if (FLASH_BUSY == nState || FLASH_COMPLETE == nState)
            {
                FLASH_ClearFlag(FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
                nState = FLASH_ProgramWord((uint32)&pHeader->signature, FlashSignature);
                if (FLASH_BUSY == nState || FLASH_COMPLETE == nState)
                {
                    nState = FLASH_ProgramWord((uint32)&pHeader->filelength, length);
                }
            }
            
            if (FLASH_COMPLETE == nState)
            {
                m_pFile = (uint8*)GetBase();
            }
        }
    }
    else
    {
        // open for existing image  for reading
        // check signature and filelength
        if (pHeader->signature == FlashSignature && pHeader->filelength > 128)
        {
            m_pFile = (uint8*)GetBase();
        }
    }

    // Check if file was successfully opened.
    if (m_pFile != NULL)
    {
        result = true;
    }

    return result;
}

void PcosImageFile::Close()
{
    FlashHeader* pHeader = (FlashHeader*)FlashBase;
    m_pFile = NULL;
}

bool PcosImageFile::Activate(const char* pAppName, bool isPerstent, bool isOK)
{

    if (isOK == true)
    {
        FLASH_Lock();
    }
    else
    {
	FLASH_Lock();
        Remove();
    }
    return true;
}

void PcosImageFile::Remove()
{
  
   FLASH_Unlock();    
	
    eraseFlashSectors();  

    // reset flash header to indicate stream invalid
    FlashHeader* pHeader = (FlashHeader*)FlashBase;
    
    FLASH_ClearFlag(FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
    
    FLASH_ProgramWord((uint32)&pHeader->signature, 0);
    FLASH_ProgramWord((uint32)&pHeader->filelength, 0);

    //initialize the header here just in case user open info dialog in MP
    uint32 pImageHeader = (uint32)GetBase();
    FLASH_ClearFlag(FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
    FLASH_ProgramWord(pImageHeader, CAppImageHdr::eClrSignature);   //sEclr = eClrSignature; //signature 'ECLR'
    pImageHeader += 4;
    while(pImageHeader < ((uint32)GetBase() + sizeof(CAppImageHdr)))
    {
        FLASH_ClearFlag(FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
       FLASH_ProgramWord(pImageHeader, 0);
       pImageHeader += 4;
    }

    FLASH_Lock();
}

unsigned PcosImageFile::Read(unsigned pos, void* pData, unsigned length)
{
    unsigned result = 0;
    if (m_pFile != NULL)
    {
        FlashHeader* pHeader = (FlashHeader*)FlashBase;
        if (pos < pHeader->filelength)
        {
            result = min(pHeader->filelength-pos, length);
            memcpy(pData, m_pFile+pos, result);
        }
    }
    return result; 
}

unsigned PcosImageFile::Write(unsigned pos, void* pData, unsigned length)
{
    unsigned result = 0;
    if (m_pFile != NULL)
    {
        FlashHeader* pHeader = (FlashHeader*)FlashBase;
        unsigned filelength = pHeader->filelength;
        if (pos < filelength)
        {
            if ((pos % sizeof(uint32)) == 0)
            {
                // 4 byte alignment necessary
                result = min(filelength - pos, length);

                for (unsigned offs = 0; offs < result; offs += sizeof(uint32))
                {
                    uint8* p = (uint8*)pData + offs;
                    uint32 val = 0;
                    memcpy(&val, p, sizeof(uint32));
                    FLASH_ClearFlag(FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
                    if (FLASH_ProgramWord((uint32)(m_pFile+(offs+pos)), val) != FLASH_COMPLETE)
                    {
                        result = 0;
                        break;
                    }
                }
            }
        }
    }
    return result; 
}

bool PcosImageFile::IsValid()
{
  
  
    FlashHeader* pHeader = (FlashHeader*)FlashBase;
    bool result = false;
    if (pHeader->signature == FlashSignature && pHeader->filelength > 128)
    {
        result = true;
    }
    return result;
}

bool PcosImageFile::InitImageHdr()
{
  
    FLASH_Status nState;
    bool result = false;

    uint32 pImageHeader = (uint32)GetBase();
    if(pImageHeader != NULL)
    {  			
        FLASH_Unlock();      
        FLASH_ClearFlag(FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
        // Sector 8-11 is for the eclr image 
        nState = FLASH_EraseSector(FLASH_Sector_10, VoltageRange_3);
        nState = FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3);
            
        // Set eCLR signature to image header.        
        if (FLASH_BUSY == nState || FLASH_COMPLETE == nState)
        {
            FLASH_ClearFlag(FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
            nState = FLASH_ProgramWord(pImageHeader, CAppImageHdr::eClrSignature);
            if (FLASH_BUSY == nState || FLASH_COMPLETE == nState)
            {
                pImageHeader += 4;
                result = true;
                while(pImageHeader < ((uint32)GetBase() + sizeof(CAppImageHdr)))
                {
                                    FLASH_ClearFlag(FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
                    nState = FLASH_ProgramWord(pImageHeader, 0);                
                    if (FLASH_BUSY != nState && FLASH_COMPLETE != nState)
                    {
                        result = false;
                        break;
                    }
                    pImageHeader += 4;
                }
            }
        }
        
        FLASH_Lock();
    }
    return result;
}

#else // ifdef EXECUTION_INFLASH_SUPPORT

// ----------------------------------------------------------------------------
// Name:        s_pPersistentName
//
// Description: Defines the constant string used to load and store the
//              persistent image files.
// ----------------------------------------------------------------------------
const char* PcosImageFile::s_pPersistentName        = "b.img";//"bootfile.img"; //FS dont' support filesname-Size=8??

// ----------------------------------------------------------------------------
// Name:        s_pTmpName
//
// Description: Defines the constant string used to load and store the
//              temporary image file.
// ----------------------------------------------------------------------------
const char* PcosImageFile::s_pTmpPersistentName     = "_b.img";//"_bootfile.img";

// ----------------------------------------------------------------------------
// Name:        s_pNonPersistentName
//
// Description: Defines the constant string used to load and store the
//              non persistent image file.
// ----------------------------------------------------------------------------
const char* PcosImageFile::s_pNonPersistentName     = "b.tmp";//"bootfile.tmp";

// ----------------------------------------------------------------------------
// Name:        s_pTmpNonPersistentName
//
// Description: Defines the constant string used to load and store the
//              nonpersistent tmp boot project files.
// ----------------------------------------------------------------------------
const char* PcosImageFile::s_pTmpNonPersistentName  = "_b.tmp";//"bbootfile.tmp";

// ---------------------------------------------------------------------------
// Name:        PcosImageFile::PcosImageFile
//
// Description: Constructor
//
// Return:      None
//
// Parameters:  None
// ---------------------------------------------------------------------------

PcosImageFile::PcosImageFile()
{
    openCnt = 0;
}

// ---------------------------------------------------------------------------
// Name:        PcosImageFile::Open
//
// Description: Opens a new or existing image file.
//
// Return:      bool - true == file opened (created), false otherwise
//
// Parameters:  int mode - support Create, WrOnly, RdOnly, and 
//                         NonPersistent. Do not support RdWr at this time.
//              unsigned length - not used
// ---------------------------------------------------------------------------

bool PcosImageFile::Open(int mode, unsigned length)
{
    bool result = false;
    length = length;

    if (m_pFile == NULL)
    {
        if (mode & Create)
        {
            // create a new image file, open for writing
            m_pTmpName = s_pTmpPersistentName;
            if (mode & NonPersistent)
            {
                m_pTmpName = s_pTmpNonPersistentName;
            }
            m_pImageName = m_pTmpName;
            // create a new bootfile
            m_pFile = EclrFile::Open(m_pTmpName, EclrFile::Open_c);
//            MSG(("imagefile: open for writing %s , mode=%d, opencnt=%d, file=%08X", m_pTmpName, mode, openCnt, m_pFile));
        }
        else
        {
            // open for existing image  for reading
            m_pImageName = m_pTmpName;
            if (m_pImageName == NULL)
            {
                // there is no temporary image (image is activated), take the real image
                m_pImageName = s_pPersistentName;
                if (mode & NonPersistent)
                {
                    m_pImageName = s_pNonPersistentName;
                }
            }
            else
            {
                // access to the temorary image file (image not yet activated)
            }
            
            m_pFile = EclrFile::Open(m_pImageName, EclrFile::Open_r);
 //           MSG(("imagefile: open for reading %s , mode=%d, opencnt=%d, file=%08X", m_pImageName, mode, openCnt, m_pFile));
        }

    }
    if (m_pFile != NULL)
    {
        openCnt++;
        result = true;
    }
    return result;
}

// ---------------------------------------------------------------------------
// Name:        PcosImageFile::Close
//
// Description: Closes the open file referenced by internal handle!
//
// Return:      void
//
// Parameters:  void
// ---------------------------------------------------------------------------

void PcosImageFile::Close()
{
    openCnt--;
//    MSG(("imagefile: %s closed, opencnt=%d", m_pImageName, openCnt));
    if (m_pFile != NULL)
    {
        EclrFile::Close(m_pFile);
        m_pFile = NULL;
    }
}


// ---------------------------------------------------------------------------
// Name:        PcosImageFile::Activate
//
// Description: Activate is called by eCLR after an image is downloaded and 
//              verified. Activate renames the temporary image (just 
//              downloaded) to the persistent name. If a previous image 
//              exists than it is backed up and deleted if the temporary file 
//              renamed successfully or restored if failed to rename the 
//              temporary file. If persistent flag is not set than the file is 
//              left with the temporary name so that on cold boot it will not 
//              get loaded (aka no-persistent).
//
// Return:      bool - true == success, false == failure
//
// Parameters:  const char* pAppName - Application name
//              bool isPersistent - if true than it is persistent
//              bool isOK - if true than downloaded file is OK 
//
// ---------------------------------------------------------------------------

bool PcosImageFile::Activate(const char* pAppName, bool isPerstent, bool isOK)
{
    pAppName = pAppName;    // skip warnings
   
    m_pImageName = s_pPersistentName;
    if (isPerstent == false)
    {
        m_pImageName = s_pNonPersistentName;
    }
    
    bool result = true;
    if (isOK == true)
    {
        if (m_pTmpName != NULL)
        {
            // first remove the current image file
            int r = EclrFile::Remove(m_pImageName);
//            MSG(("imagefile: remove %s, result=%d", m_pImageName, r));
            // second, rename the temporarily file
            r = Rename(m_pTmpName, m_pImageName);
            //r = xcopy(m_pTmpName, m_pImageName);            
            EclrFile::Remove(m_pTmpName);
//            MSG(("imagefile: rename %s, %s, result=%d", m_pTmpName, m_pImageName, r));
            if (r != 0)
            {
                result = false;
            }
            
        }
        else
        {
            result = false;
        }
    }
    // now the image is activated
    m_pTmpName = NULL;
//    MSG(("imagefile: activate %s, result=%d", m_pImageName, result));

    return result;
}

// ---------------------------------------------------------------------------
// Name:        PcosImageFile::Rename
//
// Description: change the name of a file!
//
// Return:      void
//
// Parameters:  void
// ---------------------------------------------------------------------------

int PcosImageFile::Rename(const char* pSrcName, const char* pDstName)
{ 
/*  if(f_rename(pSrcName, pDstName) != FR_OK)
  {
     return -1;
  }
*/
  return 0;
}

// ---------------------------------------------------------------------------
// Name:        PcosImageFile::Remove
//
// Description: Deletes the image file referenced by internal handle.
//
// Return:      void
//
// Parameters:  void
//
// ---------------------------------------------------------------------------

void PcosImageFile::Remove()
{
    // remove the persistent image file
    int result = EclrFile::Remove(s_pPersistentName);
    result = result; // prevent warning
//    MSG(("imagefile: remove %s, opencnt=%d, result=%d", s_pPersistentName, openCnt, result));
}



// ---------------------------------------------------------------------------
// Name:        PcosImageFile::Read
//
// Description: Read requested number of bytes beginning at specified position.
//
// Return:      unsigned - Number of bytes read.
//
// Parameters:  unsigned pos - Offset to actual position
//              void* pData - Pointer to data buffer
//              unsigned length - Number of bytes to read
//
// ---------------------------------------------------------------------------

unsigned PcosImageFile::Read(unsigned pos, void* pData, unsigned length)
{
    EclrFile::Seek(m_pFile, (long)pos, EclrFile::Seek_set);
    unsigned result = EclrFile::Read(m_pFile, pData, length);
    return result; 
}


// ---------------------------------------------------------------------------
// Name:        PcosImageFile::Write
//
// Description: Write method required by eCLR's EclrImageFile class 
//              implementation.
//
// Return:      unsigned - Amount of actual bytes written, zero implies 
//                         failure.
//
// Parameters:  pos - unsigned - Position in file to write data
//              pData - void* - Pointer to data to write
//              length - unsigned - length of data to write
//
// Assumptions: File to write too was already opened and has a valid handle.
// ---------------------------------------------------------------------------

unsigned PcosImageFile::Write(unsigned pos, void* pData, unsigned length)
{
    // for upload write not supported
    EclrFile::Seek(m_pFile, (long)pos, EclrFile::Seek_set);
    unsigned result = EclrFile::Write(m_pFile, pData, length);
    if (result != length)
    {
        result = 0;
    }
    return result;
}

#endif //#ifdef EXECUTION_INFLASH_SUPPORT
// ----------------------------------------------------------------------------
// End of Implementation
// ============================================================================
