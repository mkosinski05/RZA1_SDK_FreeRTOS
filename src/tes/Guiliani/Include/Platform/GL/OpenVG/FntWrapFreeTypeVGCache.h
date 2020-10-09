#ifndef CFNTWRAPFREETYPEVGCACHE_H
#define CFNTWRAPFREETYPEVGCACHE_H

//FreeType includes
#include "ft2build.h"
#include FT_GLYPH_H

#include "eC_Types.h"
#include "eC_String.h"
#include "GUIFontResource.h"
#include "GUITrace.h"

#include "openvg.h"
#include <vector>

/** This class manages a font and adds glyphs.
    The glyphs can eithere be outlines or bitmaps.
    To determine wich type of glyph is used, set the variable m_bUseBitmapsForGlyphs in the ctor.
*/
class CFntWrapFreeTypeVGCache
{
public:
    /** This type contains all infos to create a OpenVG-bitmap glyph.
    */
    struct FreeTypeVGChar_t
    {
        FreeTypeVGChar_t() :
            m_uiUnicode(0), m_iBitmapGlyphLeft(0),
            m_iBitmapGlyphTop(0), m_iBitmapRows(0),
            m_iBitmapWidth(0), m_uiWidth(0),
            m_vOffsetY(0.f)
           {};
        eC_UInt m_uiUnicode; ///< Unicode of the cached character
        eC_Int  m_iBitmapGlyphLeft; ///< Glyph bitmap position offset as retrieved from freetype
        eC_Int  m_iBitmapGlyphTop; ///< Glyph bitmap position offset as retrieved from freetype
        eC_Int  m_iBitmapRows; ///< Bitmap height
        eC_Int  m_iBitmapWidth; ///< Width of the bitmap itself
        eC_UInt m_uiWidth; ///< Total width of character with space on the left and right
        eC_Value m_vOffsetY; ///< Vertical offset

    };
    /** All infos encapsulated for the work with FntWrapFreeTypeVG.
    */
    struct CharInfo_t
    {
        /**
        Initialization
        @param uiUnicode Unicode-Character
        @param uiWidth width of character
        */
        CharInfo_t(eC_UInt uiUnicode, eC_UInt uiWidth): m_uiUnicode(uiUnicode), m_uiWidth(uiWidth){};
        eC_UInt m_uiUnicode; ///< Unicode of the cached character
        eC_UInt m_uiWidth; ///< Width of the glyph

        /**
        comparator for sorting etc.
        @param rhs righthand-side
        @return result of comparison
        */
        bool operator == (eC_UInt const& rhs)
        {
            return (this->m_uiUnicode == rhs);
        };
    };
    ///array of rendered chars
    std::vector<CharInfo_t> m_akChars;

    /**
    Constructor for FontID and pointer to Freetype-Library to use.
    @param eFontID FontID to use
    @param pFTLib Pointer to Freetype-Library
    */
    CFntWrapFreeTypeVGCache(const FontResource_t &eFontID, const FT_Library &pFTLib);

    /** Destructor */
    ~CFntWrapFreeTypeVGCache();

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

    /** Returns the Freetype face object of this font
        @return this font's Freetype face object.
        */
    inline FT_Face GetFace() {return m_pFace;}
    /** Returns the OpenVG handle of this font
        @return this font's OpenVG-handle.
        */
    inline VGFont GetVGFontHandle() {return m_uiFontHandle;}

    /** Adds glyp heither as Outline/VGPath or as Bitmap/VGImage
        @param uiUnicode Idetifier for glyph
    */
    void AddGlyph(const eC_UInt& uiUnicode);

    /** Determine if glyphs are outlines or bitmaps.
        @return true if glyhps are Bitmaps, false if glyphs are outlines
    */
    eC_Bool BitmapsUsedForGlyphs() const {return m_bUseBitmapsForGlyphs;}

private:

    ///helper functions
    eC_Bool CheckVGError(const eC_String &kCallingMethod);

    /**
    Initialization which is called from the constructor
    @param eFontID FontID to use
    @param pFTLib Pointer to Freetype-Library
    @return true on success, else false
    */
    eC_Bool Init(const FontResource_t &eFontID, FT_Library pFTLib);

    /** Load a glyph with Freetype and store it as VGImage in VGFont-handle.
        @param kChar Class contains infos for storing the glyph.
    */
    void LoadBitmapGlyph(FreeTypeVGChar_t& kChar);

    /** Load a glyph with Freetype and store it as VGPath in VGFont-handle.
        @param kChar Class contains infos for storing the glyph.
    */
    void LoadOutlineGlyph(FreeTypeVGChar_t& kChar);

    /** Render a glyph as a scalable path.
        @param kOutline contains form of glyph from FreeType.
        @param kChar contains infos for storing the glyph.
    */
    void ConvertGlyphToPath(const FT_Outline& kOutline, const FreeTypeVGChar_t& kChar);

    /** Render a glyph as a pixmap/bitmap.
        @param kBitmap contains the rendered glyph from FreeType.
        @param kChar Class contains infos for storing the glyph.
    */
    void ConvertGlyphToBitmap(const FT_Bitmap& kBitmap, const FreeTypeVGChar_t& kChar);

    ///This variable is set in ctor
    eC_Bool m_bUseBitmapsForGlyphs;///not final!

    FT_Face m_pFace;
    ///params for face
    eC_Int m_iDescender;
    eC_Int m_iAscender;
    eC_Int m_iInternalLeading;
    eC_Int m_iHeight;

    ///OpenVG fonts
    ///Collection of glyphs. Each glyph can be a path or bitmap(no layout infos)
    VGFont m_uiFontHandle;

    ///Code mostly from /rootfs/opt/vc/src/hello_pi/libs/vgfont/vgft.c"
    ///transforms a freeType outline to a VGPath
    static const int SEGMENTS_COUNT_MAX = 256;
    static const int COORDS_COUNT_MAX = 1024;

    VGuint m_uiSegmentsCount;
    VGubyte m_aubSegments[SEGMENTS_COUNT_MAX];
    VGuint m_uiCoordsCount;
    VGfloat m_avCoords[COORDS_COUNT_MAX];

    void ConvertOutline(const FT_Vector *points, const eC_Char *tags, const eC_Short *contours, eC_Short contours_count, eC_Short points_count);
    void ConvertContour(const FT_Vector *points, const eC_Char *tags, eC_Short points_count);
    VGfloat float_from_26_6(FT_Pos x);
};

#endif // CFNTWRAPFREETYPEVGCACHE_H
