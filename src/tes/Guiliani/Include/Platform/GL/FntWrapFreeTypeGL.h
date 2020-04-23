/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __FNTWRAPFREETYPEGL__H__
#define __FNTWRAPFREETYPEGL__H__

//FreeType includes
#include "ft2build.h"
#include FT_GLYPH_H
#include "FntWrap.h"
#include "FntWrapFreeTypeGLCache.h"


/// Base class for FreeType-based font wrapper using GL textures.
/** This class uses the class FntWrapFreeTypeGLCache for displaying FreeType
    fonts using the OpenGL-Wrapper.
  */
class CFntWrapFreeTypeGL:
    public CFntWrap
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
    CFntWrapFreeTypeGL(const eC_UInt &uiCacheSize);

    /// Destructor.
    virtual ~CFntWrapFreeTypeGL(void);

    virtual void LoadFont (const FontResource_t &eFontID);
    virtual void UnloadFont (const FontResource_t &eFontID);

    virtual void Text(const eC_Value &vX1, const eC_Value &vY1, const eC_String * const lpString);
    virtual void RequiredSpace (const eC_String * const pkText, eC_Value &vWidth, eC_Value &vHeight);
    virtual void FittingNumChars(const eC_String * const pkText, eC_Value vMaxWidth, eC_UInt & uiNumChars);

    virtual eC_Int GetInternalLeading() const;

    virtual eC_Bool SetNOFFonts(const eC_UInt uiNOFFonts);

    /// The font cache array is an eC_TArray of CFntWrapFreeTypeGLCache pointers
    eC_TArray<CFntWrapFreeTypeGLCache*> m_apFreeTypeGLCacheFonts;

private:
    /// Internal flag to indicate whether the text is actually being rendered or only its size is being calculated.
    /// Used by function RenderTextInternal().
    enum eFreeTypeTextOperation
    {
        RENDER_TEXT,
        GET_REQUIRED_SPACE
    };

    /** Renders a text to screen or calculates the required space.
        Because of redundant code of Text() and RequiredSpace() this function implements both.
        Text() and RequiredSpace() are using this function.
        @param[in] lpString The string which will be rendered or which dimensions will be calculated.
        @param[in] eTextOperation Use RENDER_TEXT to render the text or GET_REQUIRED_SPACE to calculate the required space.
        @param[in] vX1 The position of the text (x) (only used for text rendering).
        @param[in] vY1 The position of the text (y) (only used for text rendering).
        @param[out] vWidth The width of the required space.
        @param[out] vHeight The height of the required space.
        @param[in]  vWidthMax The maximum width possible the text can have.
        @param[out] uiNumFittingChars The number of characters that fit into the given maximum width.
    */
    void RenderTextInternal(const eC_String *lpString, const eFreeTypeTextOperation &eOperation, const eC_Value &vX1, const eC_Value &vY1,
                        eC_Value &vWidth, eC_Value &vHeight, eC_Value vWidthMax, eC_UInt &uiNumFittingChars);

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapFreeTypeGL(const CFntWrapFreeTypeGL& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapFreeTypeGL& operator=(const CFntWrapFreeTypeGL& kSource);

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

    /// FreeType library
    FT_Library m_pFTLibrary;
};
#endif //#ifndef __FNTWRAPFREETYPEGL__H__
