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

#include FT_MODULE_H

#include "eC_Types.h"

#include "GUIFontResource.h"

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
#include "freertosconfig.h"
#include "projdefs.h"
#include "portable.h"
#endif

#if defined (eC_TARGET_ENV_MBED)
extern "C"
{
    void *pvPortMalloc(long xWantedSize);
    void *pvPortRealloc(long xWantedSize);
    void vPortFree(void *pv);
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
    static void SetFakeBoldAndItalic(const eC_Bool bFakeBoldAndItalic);

    /** Indicates if auto-generation of Bold and Italic glyphs is enabled.
    @return True if auto-generation of Bold and Italic glyphs is enabled, False otherwise*/
    static eC_Bool GetFakeBoldAndItalic();

    /** Disables Kerning support for the FreeType Fontwrappers.
    Since Kerning is not supported by the FreeType caching subsystem it can come at a notable performance cost
    on low-end platforms. Disabling Kerning can therefore speed up overall text rendering performance.
    @param bDisableKerning True will disable Kerning. False will enable it (if supported by the given FontWrapper)
    */
    static void SetDisableKerning(const eC_Bool bDisableKerning);

    /** Indicates if Kerning is disabled.
    @return True if Kerning is disabled, False if it is enabled. */
    static eC_Bool GetDisableKerning();

    // Callback functions for memory manager of freetype used with evru AND cafe on DaVinci
    static void* FreeType_AllocFunc(FT_Memory  memory, long size);

    static void FreeType_FreeFunc(FT_Memory  memory, void* block);

    static void* FreeType_ReAllocFunc(FT_Memory  memory, long cur_size, long new_size, void* block);

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
        unsigned long count);

    /** callback function for the freetype cache manager to unload a font.
    @param stream input stream
    */
    static void FreeType_CloseStreamFunc(FT_Stream stream);

    /**
    Load a font when it was requested by the cache manager.
    Create a stream object which is used by freetype for reading operations on the font file.
    @param eFontID font ID
    @param pTheLoadedFace loaded font face
    @param pFTLibrary the font library
    @return True if successful, False otherwise
    */
    static eC_Bool LoadRequestedFont(
        const FontResource_t eFontID,
        FT_Face* pTheLoadedFace,
        const FT_Library* pFTLibrary);
};

#endif //#ifndef _FREETYPEENV_H_
