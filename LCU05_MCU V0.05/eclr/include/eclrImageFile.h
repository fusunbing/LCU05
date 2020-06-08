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
// Author: Xiangyang Ren                           
//-----------------------------------------------------------------------------
// Change History:
// ----------------------------------------------------------------------------
// 	Jun. 15th, 2009 by X. Ren - created
//
// ============================================================================

#ifndef __CECLRIMAGEFILE_H
#define __CECLRIMAGEFILE_H

// ============================================================================
// Includes 
// ----------------------------------------------------------------------------

#include "eclr.h"
#include "econfig.h"

// ----------------------------------------------------------------------------
// Definitions:
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// User Type Declarations
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Data Object Declarations:
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Method Declarations:
// ------------------------------------------------------------------------

#ifdef EXECUTION_INPLACE_SUPPORT

#define ECLR_FLASH_SIGNATURE 	0x12345678
#define FLASH_PAGE_SIZE 	0x400

// implementation application domain image file interface
class PcosImageFile : public EclrImageFile
{
    public :
    PcosImageFile();

    // virtual override
    bool Open(int mode, unsigned length);
    void Close();
    bool Activate(const char* pAppName, bool isPerstent, bool isOK);
    void Remove();
    unsigned Read(unsigned pos, void* pData, unsigned length);
    unsigned Write(unsigned pos, void* pData, unsigned length);
    bool IsValid();
    bool InitImageHdr();

    typedef struct _flashheader
    {
        uint32 signature;
        uint32 filelength;
    } FlashHeader;

    void* GetBase()
    {
        return (void*)(FlashBase+sizeof(FlashHeader));
    }
    uint32 GetSize()
    {
        return FlashSize;
    }

    private:
    uint8*   m_pFile;

    static const uint32 FlashBase;
    static const uint32 FlashSize;

    static const uint32 FlashPageSize;
    static const uint32 FlashSignature;
};

#else
// implementation application domain image file interface
class PcosImageFile : public EclrImageFile
{
public :
   PcosImageFile();

   // virtual override
   bool Open(int mode, unsigned length);
   void Close();
   bool Activate(const char* pAppName, bool isPerstent, bool isOK);
   void Remove();
   int Rename(const char* pScrName, const char* pDstName);
   unsigned Read(unsigned pos, void* pData, unsigned length);
   unsigned Write(unsigned pos, void* pData, unsigned length);

private:
   static const char* s_pPersistentName;
   static const char* s_pTmpPersistentName;
   static const char* s_pNonPersistentName;
   static const char* s_pTmpNonPersistentName;
   EclrFile*   m_pFile;
   const char* m_pTmpName;
   const char* m_pImageName;
   int openCnt;

};
#endif

// ============================================================================
// End of Header
// ========================================================================
#endif
