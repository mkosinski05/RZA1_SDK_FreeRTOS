/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __FNT_WRAP_FREETYPE__H__
#define __FNT_WRAP_FREETYPE__H__

#include "FntWrap.h"

#include "GUIImageData.h"
#include "GUIFontResource.h"

//freetype
#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_CACHE_H

/** This base class does everything that can be done platform-independently
    when using FreeType 2 but leaves the actual rendering for derived classes
    that may use any graphics library.

    When using this wrapper, fonts must be registered using their file name,
    for instance "fonts/Vera.ttf" (CGUIResourceFileHandler's path prefix
    applies).

    In addition to scalable font formats, bitmap fonts are supported, for
    instance the BDF format. Note that such fonts should be generated with a
    resolution of 72 dpi and must be registered with the exact same pixel size
    in Guiliani.

    Example: The font Vera14.bdf has been generated from Vera.ttf with a point
    size of 14 and a resolution of 72 dpi. Registering this font then would
    look like this:

@code
GETRESMANAGER.RegisterFontResource("Vera14.bdf", FNT_MYFONT1, 14, CFntWrap::FNT_NORMAL, true);
@endcode

    If the font size passed in this call doesn't match the BDF font's size,
    FreeType will fail silently when trying to load the font and this font
    wrapper will print a generic error message.

    @note due to differences in font formats and the information they contain,
    text positions might differ when using a TTF font and a BDF font generated
    from the same TTF. Especially top-aligned text's y positions might differ.

<h3>Platform Specific Limitations</h3>
*/
// @guiliani_doxygen limitations CFntWrapFreeType
/**
    FntWrapFreeType does not support auto-generation of italic/bold glyphs. Use dedicated TTF files instead (Class CFntWrapFreeType).
*/
// @endguiliani_doxygen
/**
    See @ref subsec_limitations_CFntWrapFreeType "CFntWrapFreeType Platform Specific Limitations"
    @ingroup GUILIANI_GRAPHICS
    @brief Base class for FreeType 2 font wrappers.
*/
class CFntWrapFreeType :
    public CFntWrap
{
protected:

    /** Constructor.
    @param uiCacheSize The maximum size of the FreeType cache in byte.
    @param uiMaxNumFaces Maximum number of opened font face objects managed by the FreeType cache.
    @param uiMaxNumSizeObjects Maximum number of opened size objects managed by the FreeType cache.
    */
    CFntWrapFreeType(const eC_UInt& uiCacheSize,
                     const eC_UInt& uiMaxNumFaces,
                     const eC_UInt& uiMaxNumSizeObjects);

    /// Default constructor.
    CFntWrapFreeType() {}

    /// Destructor.
    virtual ~CFntWrapFreeType(void);

    virtual void LoadFont  (const FontResource_t &eFontID);
    virtual void SetFont   (const FontResource_t &eID);
    virtual void UnloadFont(const FontResource_t &eFontID);

    virtual void Text(const eC_Value &vX1, const eC_Value &vY1, const eC_String * const lpString);
    virtual void RequiredSpace (const eC_String * const pkText, eC_Value &vWidth, eC_Value &vHeight);
    virtual void FittingNumChars(const eC_String * const pkText, eC_Value vWidthMax, eC_UInt &uiNumChars);

    virtual eC_Int GetInternalLeading() const { return ms_iInternalLeading; }

    virtual eC_Bool SetNOFFonts(const eC_UInt uiNOFFonts);

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

    /** Render a glyph bitmap to screen.
        This function is called by Text(eC_Value,eC_Value,eC_String*) to render a glyph image to the screen.
        In the implementation the 8 bit alpha image can be used to render a colored text to the screen.
        @param vX1 The x coordinate where the glyph image has to be rendered.
        @param vY1 The y coordinate where the glyph image has to be rendered.
        @param kGlyphImage The glyph image containing a 8 bit alpha image of the glyph.
    */
    virtual void RenderGlyphToScreen(const eC_Value &vX1, const eC_Value &vY1, CGUIImageData& kGlyphImage)=0;

    /**
        Checks if another font id uses the same font object.
        If this is the case it sets this id as reference.
        @param eFontID The id of the font.
        @param eExcludedFontID This FontID will not be used as reference, because it will be unloaded.
        @return True if a reference was set, False else.

    */
    eC_Bool FindFontIDWithSameFont(FontResource_t &eFontID, const FontResource_t& eExcludedFontID = DUMMY_FONT);

    /**
        Returns the referenced font id for a specified font.
        @param eFontID The id of the font.
        @return The referenced font id, or DUMMY_FONT if no reference is set
                or eFontID was invalid.
    */
    FontResource_t GetReferencedFontID (const FontResource_t& eFontID) const;

    /** Callback function for the freetype cache manager to request a font.
        This function calls LoadRequestedFont if necessary.
        For details see freetype documentation.
    */
    static FT_Error FTCM_RequestCallback(
        FTC_FaceID   face_id,
        FT_Library   library,
        FT_Pointer   request_data,
        FT_Face     *aface );

    /** Look up font with current id and current size in cache and calculate the new font metrics to use the font.
        @param[in] kCurFTFontAttributes Scaler whose size is to be looked up.
        @param[out] kFTFont The size object returned by FreeType.
        @param[out] iDescender The size's descender in pixels.
        @param[out] iAscender The size's ascender in pixels.
        @param[out] iInternalLeading The size's internal leading in pixels.
        @param[out] uiIndexOfUnicodeCharmap The index of the Unicode charmap
                    within the face.
        @return True if look-up was successful, False if the look-up failed or
                the charmap could not be selected.
      */
    eC_Bool LookupFont(
                            FTC_ScalerRec_& kCurFTFontAttributes,
                            FT_Size& kFTFont,
                            eC_Int& iDescender,
                            eC_Int& iAscender,
                            eC_Int& iInternalLeading,
                            eC_UInt& uiIndexOfUnicodeCharmap
                            );

    /// A freetype instance
    static FT_Library ms_kFreeType;

    /// A freetype cache manager
    static FTC_Manager  ms_kFTCManager;

    /// A freetype charmap cache
    static FTC_CMapCache  ms_kFTMapChache;

    /// A freetype cache for the glyph images
    static FTC_SBitCache  ms_kFTGlyphBitmapCache;

    /** This object is used to lookup faces with the current font ID and
        the current scale in the FreeType cache manager.
    */
    static FTC_ScalerRec_   ms_kCurFTFontAttributes;

    ///this object contains the face of the current font.
    static FT_Size ms_kCurFTFont;

    ///indicates if the current font was loaded/looked up successfully
    static eC_Bool ms_bCurFontLoaded;

    ///store the index of the unicode charmap for the current font (used for cache lookups).
    static eC_UInt ms_uiIndexOfUnicodeCharmap;

    /// The descender of the currently selected font in pixels.
    static eC_Int ms_iDescender;

    /// The ascender of the currently selected font in pixels.
    static eC_Int ms_iAscender;

    /// The internal leading of the currently selected font in pixels.
    static eC_Int ms_iInternalLeading;

    /// Array to store the reference count for each font ID and the referenced font id for each font.
    eC_TArray<FontResource_t> m_aReferencedFontIDs;

    /// Array to store XAdvance-Value for whitespace-characters which are handled erroneously by FreeType's Cache-Subsystem
    eC_TArray<eC_Int> m_aiWhiteSpaceXAdvance;

    /// Indicates if Kerning shall be disabled
    static eC_Bool ms_bDisableKerning;

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapFreeType(const CFntWrapFreeType& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapFreeType& operator=(const CFntWrapFreeType& kSource);

#if defined(eGML_USES_eVRU) || defined(CAFE_OS_LINUX) || defined(eC_TARGET_ENV_FREERTOS) || defined(eC_TARGET_ENV_MBED)
    FT_Memory m_hFreetypeMemoryManager;
#endif
};
#endif
