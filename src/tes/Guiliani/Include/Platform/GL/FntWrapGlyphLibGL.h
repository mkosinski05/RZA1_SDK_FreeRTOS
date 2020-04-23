/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __FNT_WRAP_GLYPHLIB_GL_H__
#define __FNT_WRAP_GLYPHLIB_GL_H__

#include "FntWrapGlyphLib.h"
#include "FntWrapGlyphLibGLCache.h"

/// Base class for GlyphLib-based font wrapper using GL textures.
/** This class uses the class FntWrapGlyphLibGLCache for displaying GlyphLib
    fonts using the OpenGL-Wrapper.
  */
class CFntWrapGlyphLibGL : public CFntWrapGlyphLib
{
public:
    /** Creates the instance of this wrapper.
        @param uiCacheSize Maximal number of simultaneously cached characters. If parameter
                           equals zero the cache will automatically be resized to be
                           large enough to contain all required characters.
        @return True if successful, False otherwise
    */
    static eC_Bool CreateInstance(const eC_UInt &uiCacheSize = 0);

protected:
    /** Constructor.
        @param uiCacheSize Maximal number of simultaneously cached characters. If parameter
                           equals zero the cache will automatically be resized to be
                           large enough to contain all required characters.
    */
    CFntWrapGlyphLibGL(const eC_UInt &uiCacheSize);

    /// Destructor.
    virtual ~CFntWrapGlyphLibGL(void);

    virtual void SetFont(const FontResource_t &eID);

    virtual void LoadFont(const FontResource_t &eFontID);

    /** Unloading is not supported by GlyphLib
    @param eFontID FontID to unload
    */
    virtual void UnloadFont(const FontResource_t &eFontID);

    virtual void Text(const eC_Value &vX1, const eC_Value &vY1, const eC_String * const lpString);
    virtual void RequiredSpace (const eC_String * const pkText, eC_Value &vWidth, eC_Value &vHeight);
    virtual void FittingNumChars(const eC_String * const pkText, eC_Value vMaxWidth, eC_UInt & uiNumChars);

    virtual eC_Int GetInternalLeading() const;

    virtual eC_Bool SetNOFFonts(const eC_UInt uiNOFFonts);

    /// The font cache array is an eC_TArray of CFntWrapGlyphLibGLCache pointers
    eC_TArray<CFntWrapGlyphLibGLCache*> m_apGlyphLibGLCacheFonts;

private:
    virtual void RenderTextInternal(
        const eC_String *lpString,
        const eC_Value &vX1,
        const eC_Value &vY1,
        eC_Value &vWidth,
        eC_Value &vHeight,
        eC_Value vWidthMax,
        eC_UInt &uiNumFittingChars);

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapGlyphLibGL(const CFntWrapGlyphLibGL& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapGlyphLibGL& operator=(const CFntWrapGlyphLibGL& kSource);

    /** Calculates the width, the height if called from RequiredSpace and the number of characters
        that fit into a string with the width vMaxWidth
        @param pkText The text.
        @param vWidth Reference to width of the font in pixels.
        @param vHeight Reference to height of the font in pixels.
        @param vWidthMax Maximum width for the text in pixels.
        @param uiNumChars Reference value returning the number of characters that fit into that width
    */
    void RequiredSpaceInternal(const eC_String * const pkText, eC_Value &vWidth, eC_Value &vHeight,
                                eC_Value vMaxWidth, eC_UInt &uiNumChars);

    /// Stores the cache size information
    eC_UInt m_uiCacheSize;
};
#endif //#ifndef __FNT_WRAP_GLYPHLIB_GL_H__
