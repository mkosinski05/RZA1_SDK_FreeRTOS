/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __FNT_WRAP_GLYPHLIB__H__
#define __FNT_WRAP_GLYPHLIB__H__

#include "eC_TArray.h"
#include "FntWrap.h"

#include "gui_font.h"

/// GlyphLib (uGuiliani font) implementation of the Font Wrapper

/**
<h3>Platform Specific Limitations </h3>
*/
// @guiliani_doxygen limitations CFntWrapGlyphLib
/**
As the CFntWrapGlyphLib uses a pre-compiled ROMFS to store the glyph bitmaps, it is not possible to change the number of used fonts via SetNOFFonts()
*/
// @endguiliani_doxygen
/**
See @ref subsec_limitations_CFntWrapGlyphLib "CFntWrapGlyphLib Platform Specific Limitations"
*/

/** This class is used to wrap the funtionality of the GlyphLib
*/
class CFntWrapGlyphLib : public CFntWrap
{
public:
    void RequiredSpace(const eC_String * const pkText, eC_Value &vWidth, eC_Value &vHeight);

    void SetFont(const FontResource_t &eID);

    void LoadFont(const FontResource_t &eFontID);

    /**Unloading is not supported by GlyphLib
    @param eFontID FontID to unload
    */
    void UnloadFont(const FontResource_t &eFontID);
protected:
    CFntWrapGlyphLib();

    void Text(const eC_Value &vX1, const eC_Value &vY1, const eC_String * const lpString);

    eC_Bool SetNOFFonts(const eC_UInt uiNOFFonts);

    eC_UInt m_uiCurrentFontIndex; ///< the currently used font-index
    eC_TArray<gui_font_t> m_kFontsHandles; ///< handles of all loaded fonts

private:
    virtual void RenderTextInternal(
        const eC_String *lpString,
        const eC_Value &vX1,
        const eC_Value &vY1,
        eC_Value &vWidth,
        eC_Value &vHeight,
        eC_Value vWidthMax,
        eC_UInt &uiNumFittingChars) = 0;

    /** Copy-constructor. Should not be used.
    Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapGlyphLib(const CFntWrapGlyphLib& kSource);

    /** Operator= method. Should not be used.
    Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapGlyphLib& operator=(const CFntWrapGlyphLib& kSource);
};

#endif //__FNT_WRAP_GLYPHLIB__H__
