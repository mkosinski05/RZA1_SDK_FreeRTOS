/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __FNTWRAP_GLYPHLIB_GLCACHE_H__
#define __FNTWRAP_GLYPHLIB_GLCACHE_H__

#include "eC_Types.h"
#include "eC_TArray.h"
#include "GUIFontResource.h"

#include "gui_font.h"

/// FntWrapGlyphLibGLCache class used by FntWrapGlyphLibGL.
/** This class is used by FntWrapFreeTypeGL. It stores the cache information for
    one Font.
  */
class CFntWrapGlyphLibGLCache
{
public:
    /// Struct representing one character inside the cache array
    struct CachedGlyphLibGLChar_t
    {
        CachedGlyphLibGLChar_t() : m_uiUnicode(0), m_uiTexID(0), m_iBitmapGlyphLeft(0), m_iBitmapGlyphTop(0), m_iBitmapRows(0), m_iBitmapWidth(0), m_uiWidth(0), m_uiTimestamp(0) {};
        eC_UInt m_uiUnicode; ///< Unicode of the cached character
        eC_UInt m_uiTexID; ///< OpenGL texture ID.
        eC_Int  m_iBitmapGlyphLeft; ///< Glyph bitmap position offset as retrieved from freetype
        eC_Int  m_iBitmapGlyphTop; ///< Glyph bitmap position offset as retrieved from freetype
        eC_Int  m_iBitmapRows; ///< Bitmap height
        eC_Int  m_iBitmapWidth; ///< Width of the bitmap itself
        eC_UInt m_uiWidth; ///< Total width of character with space on the left and right
        eC_UInt m_uiTimestamp; ///< Stores timestamp of last access
    };

    /** Constructor
        @param eFontID ID of selected font.
        @param uiCacheSize Maximal number of simultaneously cached characters. If parameter
                           equals zero the cache will automatically be resized to be
                           large enough to contain all required characters.
        @param kFont the loaded font
        */
    CFntWrapGlyphLibGLCache(const FontResource_t &eFontID, const eC_UInt &uiCacheSize, gui_font_t kFont);

    /** Destructor */
    ~CFntWrapGlyphLibGLCache();

    /** This method returns a struct with the wanted character's information.
        It adds a character to the cache array if it does not exist.
        @param uiUnicode Unicode of wanted character.
        @return Struct of wanted character.
        */
    CachedGlyphLibGLChar_t GetChar(const eC_UInt &uiUnicode);

    /** Returns the height of current font.
        @return Font's height.
        */
    inline eC_Int GetHeight() const {return m_iHeight;}

    /** Returns the ascender of the current font.
        @return Font's ascender.
        */
    inline eC_Int GetAscender() const {return m_iAscender;}

    /** Returns the descender of the current font.
        @return Font's descender.
        */
    inline eC_Int GetDescender() const {return m_iDescender;}

    /** Returns the internal leading of the current font.
        @return Font's internal leading.
        */
    inline eC_Int GetInternalLeading() const {return m_iInternalLeading;}

private:
    /** Adds a character to the cache array and sorts array afterwards.
        @param uiUnicode Unicode to add to array.
        @param uiPos Position in array where the character should be added.
        @return Struct of just added character.
        */
    CachedGlyphLibGLChar_t AddChar(const eC_UInt &uiUnicode, const eC_UInt &uiPos);

    /** Removes a character from array.
        @param uiPos Position of the character to remove.
        */
    void RemoveChar(const eC_UInt &uiPos);

    /** Initializes this class by opening the font face and setting parameters.
        @param eFontID ID of the font to be created.
        */
    void Init(const FontResource_t &eFontID);

    /** Searches for a character inside the cache array by using divide and
        conquer.
        @param uiUnicode Character to search for.
        @return Position in array of -1 if not found.
    */
    eC_Int FindChar(const eC_UInt &uiUnicode);

    /** Swaps two given values.
        @param pPosA Pointer of the first value.
        @param pPosB Pointer of the second value.
        */
    void Swap(CachedGlyphLibGLChar_t *pPosA, CachedGlyphLibGLChar_t *pPosB);

    /** Sorts the cache list by unicode in ascending order using bubble sort
        algorithm.*/
    void Sort();

    /** Searches the list for the lowest timestamp and return that position.
        @return Lowest timestamp position in array.
        */
    eC_UInt FindLowestTimestampPos();

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CFntWrapGlyphLibGLCache(const CFntWrapGlyphLibGLCache& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CFntWrapGlyphLibGLCache& operator=(const CFntWrapGlyphLibGLCache& kSource);

    eC_Int m_iAscender;
    eC_Int m_iDescender;
    eC_Int m_iInternalLeading;
    eC_Int m_iHeight;
    eC_UInt m_uiCacheSize;              /// Maximal cache size.
    eC_UInt m_uiTimer;                  /// Timer for the timestamp

    eC_UInt m_uiQuantity;               /// Current array quantity.
    eC_Bool m_bAutoResize;

    eC_TArray<CachedGlyphLibGLChar_t> m_asCache;  /// The dynamic cache array.

    eC_Bool m_bArrayNeedsSorting;       /// Flag indicating that the internal array of stored characters needs to be sorted

    gui_font_t m_kFont;
};
#endif //#ifndef __FNTWRAP_GLYPHLIB_GLCACHE_H__
