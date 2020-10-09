/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef _FREETYPEENV_H_
#define _FREETYPEENV_H_

#include "ft2build.h"
#include "eC_File.h"
#include "GfxWrap.h"
#include "GUIResourceManager.h"
#include "GUIResourceFileHandler.h"

#include FT_MODULE_H

#if defined eGML_USES_eVRU
#include "eCLIB.h"
#endif

// CAFE and eVRU specific includes on DaVinci
#if defined eGML_USES_eVRU || defined CAFE_OS_LINUX
#include <eVRU.h>
#endif

#if defined CAFE_OS_LINUX
#include <cafe.h>
#include <cafe_x_hal/cafe_x_hal.h>
#include <cafe_x_vfs/cafe_x_vfs.h>
#include <cafe_x_img/cafe_x_img.h>
#endif

#if defined (eC_TARGET_ENV_FREERTOS)
#include "portable.h"
#endif

#if defined __MBED__
extern "C"
{
	void *pvPortMalloc( long xWantedSize );
	void *pvPortRealloc( long xWantedSize );
	void vPortFree( void *pv );
}
#endif

//macros for Freetype fixpoint
#define FT_CEIL(X)    (((X + 63) & -64) >> 6)
#define FT_FLOOR(X)    ((X & -64) >>6)


/// Class for all common FreeType functions.

/**
    Used by CFntWrapFreeType and CFntWrapFreeTypeOGL and CFntWrapFreeTypeCAFE
    @see CFntWrapFreeType
    @see CFntWrapFreeTypeOGL
    @see CFntWrapFreeTypeCAFE
*/

class CFreeTypeEnv
{
protected:
    static eC_Bool ms_bFakeBoldAndItalic; ///< True will enable auto-generation of bold/italic effects. False will disable it. (if supported by the given FontWrapper)
    static eC_Bool ms_bDisableKerning;    ///< True will disable Kerning. False will enable it (if supported by the given FontWrapper)

public:
    /** Enables auto-generation of Bold and Italic glyphs.
        When enabled, all fonts which have the GUIFont_t::FNT_BOLD style set, 
        will be rendered twice with a slight shift, to emulate a "bold" appearance.
        When enabled, all fonts which have the GUIFont_t::FNT_ITALIC style set, 
        will be transformed by FreeType with a sheering matrix, to emulate an "italic" appearance.
        @param bFakeBoldAndItalic True will enable auto-generation of bold/italic effects. False will disable it.
    */
    static void SetFakeBoldAndItalic(const eC_Bool bFakeBoldAndItalic ) { ms_bFakeBoldAndItalic=bFakeBoldAndItalic; }

    /** Indicates if auto-generation of Bold and Italic glyphs is enabled.
        @return True if auto-generation of Bold and Italic glyphs is enabled, False otherwise*/
    static eC_Bool GetFakeBoldAndItalic() { return ms_bFakeBoldAndItalic; }

    /** Disables Kerning support for the FreeType Fontwrappers.
        Since Kerning is not supported by the FreeType caching subsystem it can come at a notable performance cost
        on low-end platforms. Disabling Kerning can therefore speed up overall text rendering performance.
        @param bDisableKerning True will disable Kerning. False will enable it (if supported by the given FontWrapper)
    */
    static void SetDisableKerning(const eC_Bool bDisableKerning ) { ms_bDisableKerning=bDisableKerning; }

    /** Indicates if Kerning is disabled.
        @return True if Kerning is disabled, False if it is enabled. */
    static eC_Bool GetDisableKerning() { return ms_bDisableKerning; }
    

#if defined CAFE_OS_LINUX
    //   Callback functions for memory manager of freetype used with evru AND cafe on DaVinci
    static void* FreeType_AllocFunc( FT_Memory  memory,long size )
    {
        void* pAllocatedMem = cafe_mem_alloc( size);
        if( !pAllocatedMem)
            throw(" FreeType_AllocFunc:: Failed to allocate shared memory. \n");
        return pAllocatedMem;
    }

    static void FreeType_FreeFunc( FT_Memory  memory, void*      block )
    {
        cafe_mem_free(block);
    }

    static void* FreeType_ReAllocFunc( FT_Memory  memory, long cur_size, long new_size, void* block )
    {
        void* newBlock = cafe_mem_alloc(new_size);
        if( !newBlock)
            throw(" FreeType_ReAllocFunc:: Failed to allocate shared memory. \n");
        eC_Memcpy(newBlock,block,cur_size < new_size ? cur_size : new_size);
        cafe_mem_free(block);
        return newBlock;
    }
#endif

#if defined eGML_USES_eVRU
    // Callback functions for memory manager of freetype used with eVru
    static void* FreeType_AllocFunc( FT_Memory  memory,long size )
    {
        return eCLIB_AllocShared(size); 
    }

    static void FreeType_FreeFunc( FT_Memory  memory, void* block )
    {
        eCLIB_FreeShared(block);
    }

    static void* FreeType_ReAllocFunc( FT_Memory  memory, long cur_size, long new_size, void* block )
    {
        eCLIB_Void* newBlock = eCLIB_AllocShared(new_size);
        if( !newBlock) {
          throw(" FreeType_ReAllocFunc:: Failed to allocate shared memory. \n");
        }
        eC_Memcpy(newBlock,block,cur_size < new_size ? cur_size : new_size);
        eCLIB_FreeShared(block);
        return newBlock;
    }
#endif

#if defined(eC_TARGET_ENV_FREERTOS) || defined(eC_TARGET_ENV_MBED)
    // Callback functions for memory manager of freetype used with FreeRTOS
    static void* FreeType_AllocFunc( FT_Memory  memory,long size )
    {
        return pvPortMalloc(size); 
    }

    static void FreeType_FreeFunc( FT_Memory  memory, void* block )
    {
        vPortFree(block);
    }

    static void* FreeType_ReAllocFunc( FT_Memory  memory, long cur_size, long new_size, void* block )
    {
        void* newBlock = pvPortMalloc(new_size);
        if( !newBlock) {
          throw(" FreeType_ReAllocFunc:: Failed to allocate shared memory. \n");
        }
       
        eC_Memcpy(newBlock,block,cur_size < new_size ? cur_size : new_size) ;
       
        vPortFree(block);
        return newBlock;
    }
#endif

    /** Callback function used by freetype to read the font files (for use with eC_File).
        @param stream input stream
        @param offset offset
        @param buffer buffer
        @param count count
        @return the read data
    */
    static unsigned long FreeType_Stream_IoFunc(
        FT_Stream stream,
        unsigned long offset,
        unsigned char* buffer,
        unsigned long count )
    {
        eC_File* pkFontFile = static_cast<eC_File*>(stream->descriptor.pointer);
        pkFontFile->Seek(offset);
        if( count>0 && buffer) // FreeType calls us also for plain Seek-operations
            return pkFontFile->Read8(buffer, count);
        else
            return 0;
    }

    /** callback function for the freetype cache manager to unload a font.
        @param stream input stream
    */
    static void FreeType_CloseStreamFunc( FT_Stream stream )
    {
        eC_File *pTempFontFile = static_cast<eC_File*>(stream->descriptor.pointer);
        GETRESHANDLER.Close(pTempFontFile);
        delete stream;
    }

    /**
        Load a font when it was requested by the cache manager.
        Create a stream object which is used by freetype for reading operations on the font file.
        @param eFontID font ID
        @param pTheLoadedFace loaded font face
        @param pFTLibrary the font library 
        @return True if successful, False otherwise
    */
    static eC_Bool LoadRequestedFont(
        const FontResource_t eFontID, FT_Face* pTheLoadedFace, const FT_Library* pFTLibrary)
    {
        eC_File *pTempFontFile = NULL;
        eC_String kFontPath = GETRESMANAGER.GetFontPath(eFontID);
        if (kFontPath.IsEmpty())
        {
            GUILOG( GUI_TRACE_ERROR, "CFreeTypeEnv::LoadRequestedFont: FontPath for FontID " + eC_String(eFontID) + " is empty.\n");
            return false;
        }

        // Open the font file but do not completely buffer it into ram.
        pTempFontFile = GETRESHANDLER.Open(kFontPath, CGUIResourceFileHandler::BUFFERING_OFF);
        if (pTempFontFile==NULL)
        {
            GUILOG( GUI_TRACE_ERROR, "CFreeTypeEnv::LoadRequestedFont: Could not load font with id " + eC_String(eFontID) + "\n");
            return false;
        }

        FT_Open_Args args;
        FT_Stream stream;
        stream = new FT_StreamRec_;
        memset(stream, 0, sizeof(*stream));
        //set the font-file and the callback functions
        stream->descriptor.pointer = pTempFontFile;
        stream->read = FreeType_Stream_IoFunc;
        stream->close= FreeType_CloseStreamFunc;
        stream->pos = 1;
        stream->size = pTempFontFile->GetFilesize();
        stream->base = NULL; // Shall be NULL for disk-based streams

        args.flags = FT_OPEN_STREAM;
        args.stream = stream;

        eC_Int iFTError = FT_Open_Face(*pFTLibrary, &args, 0, pTheLoadedFace);

        if (iFTError!=0)
        {
            // We must not delete the stream here, because it gets deleted within FreeType_CloseStreamFunc already.
            // Neither must we close the file here, since this is done within FreeType_CloseStreamFunc as well.
            GUILOG(
                GUI_TRACE_ERROR, "CFreeTypeEnv::LoadRequestedFont: FreeType returned error code " +
                eC_String(iFTError) + ". Could not load font with id "+ eC_String(eFontID) + "\n");

            return false;
        }

        if( CFreeTypeEnv::GetFakeBoldAndItalic())
        {
            // Fake italic by sheering
            if( (GETRESMANAGER.GetFontStyle(eFontID) & CFntWrap::FNT_ITALIC) )
            {
                FT_Matrix matrix;
                matrix.xx = 0x10000L;
                matrix.xy = static_cast<FT_Fixed>(0.12 * 0x10000L);
                matrix.yx = 0;
                matrix.yy = 0x10000L;
                FT_Vector delta;
                delta.x = 0; // coordinates are in 26.6 pixel format
                delta.y = 0;
                FT_Set_Transform( *pTheLoadedFace, &matrix, &delta );
            }
        }

        return true;
    }
};

#endif //#ifndef _FREETYPEENV_H_
