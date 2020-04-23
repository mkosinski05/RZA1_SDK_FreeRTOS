#ifndef CFNTWRAPFREETYPEVG_H
#define CFNTWRAPFREETYPEVG_H

#include "FntWrap.h"

#include "GUIFontResource.h"
#include "FntWrapFreeTypeVGCache.h"

//freetype
#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

#include "openvg.h"
#include <vector>
#include <algorithm>

/**
    This class loads a FreeType font and stores the glyphs via FntWrapFreeTypeVGChache in a OpenVG font-handle
    either as a Outline Font with VGPath or as a VGBitmap
*/
class CFntWrapFreeTypeVG : public CFntWrap
{
    public:
        /** Creates the instance of this wrapper.
        @return True if successful, False otherwise
        */
        static eC_Bool CreateInstance();///bool path oder bitmap oder in cache?

        /**
        Return internal leading of currently used font
        @return internal leading
        */
        eC_Int GetInternalLeading() const;

    protected:
        CFntWrapFreeTypeVG();
        ~CFntWrapFreeTypeVG();

        virtual eC_Bool SetNOFFonts(const eC_UInt uiNOFFonts);

        virtual void LoadFont (const FontResource_t &eFontID);

        virtual void UnloadFont (const FontResource_t &eFontID);

        virtual void Text(const eC_Value &vX1, const eC_Value &vY1, const eC_String * const lpString);

        virtual void RequiredSpace (const eC_String * const pkText, eC_Value &vWidth, eC_Value &vHeight);

        /**
        array of fonts which are currently cached
        */
        eC_TArray<CFntWrapFreeTypeVGCache*> m_apFreeTypeVGCacheFonts;
        
    private:
    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapFreeTypeVG(const CFntWrapFreeTypeVG& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapFreeTypeVG& operator=(const CFntWrapFreeTypeVG& kSource);

    ///helper function for OpenVG error codes
    eC_Bool CheckVGError(const eC_String &kCallingMethod);

    /// FreeType library
    FT_Library m_pFTLibrary;

};

#endif // CFNTWRAPFREETYPEVG_H
