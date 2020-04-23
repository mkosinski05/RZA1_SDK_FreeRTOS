/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __FNT_WRAP_FREETYPE_EGML__H__
#define __FNT_WRAP_FREETYPE_EGML__H__

#include "eGML.h"
#include "FntWrapFreeType.h"


/// FreeType font wrapper specialization using eGML bitmaps.

/** This implementation uses eGML bitmap operations for rendering.
*/
class CFntWrapFreeTypeeGML:
    public CFntWrapFreeType
{
private:

    /** Helper class for having an eGML Bitmap which is modifiable in size and content.
        Usually a bitmap shall be created via the eGML_Screen::CreateBitmap() factory, and is then only modifiable via the 
        dedicated interfaces of eGML_Bitmap.
        For FreeType this is not desirable, since the Glyph-Bitmaps are stored in the freetype cache and the eGML_Bitmap
        is merely needed as a container for working with eGML's blitting interfaces. So in order to avoid creating temporary
        bitmaps for every glyph, we can re-use one CModifyableBitmap and simply re-initialize it for each glyph. 
    */
    class CModifyableBitmap : public eGML_Bitmap
    {
    public:
        CModifyableBitmap(eC_UInt uiWidth, eC_UInt uiHeight, eC_UByte ubBitmapBufferType) :
            eGML_Bitmap(uiWidth, uiHeight, ubBitmapBufferType)
        {
        }

        ~CModifyableBitmap()
        {
        }

        /** Initialize this instance with new image information.
            @param eFormat Format of Bitmap (see eGML_Config.h)
            @param uiWidth Width of bitmap data in pixels
            @param uiHeight Height of bitmap data in pixels
            @param ubBitmapBufferType eGML bitmap buffertype (see eGML_Config.h)
            @param pBitmapBuffer Pointer to raw bitmap pixel data
        */
        void SetNewBitmapData(eC_UInt eFormat, eC_UInt uiWidth, eC_UInt uiHeight, eC_UByte ubBitmapBufferType, void *pBitmapBuffer)
        {
            m_uiType   = eFormat;
            m_uiWidth  = uiWidth;
            m_uiHeight = uiHeight;
            m_ubBBT    = ubBitmapBufferType;
            m_pkData   = pBitmapBuffer;

            // pitch depends on bitmap format
            switch (eFormat)
            {
            case eGML::BM_ALPHA1:
                m_uiPitch = (uiWidth + 7) & ~0x07;
                break;
            case eGML::BM_ALPHA8:
                m_uiPitch = uiWidth;
                break;
            case eGML::BM_RGB565:
            case eGML::BM_RGB555:
                m_uiPitch = uiWidth * 2;
                break;
            case eGML::BM_RGB32:
            case eGML::BM_ARGB32:
                m_uiPitch = uiWidth * 4;
                break;
            default:
                GUILOG( GUI_TRACE_ERROR, "Unknown bitmap format. Failed to determine pitch.\n");
                break;
            }
        }

        virtual void * Lock()
        {
            // Dummy implementation of pure-virtual function in base-class
            return NULL;
        }

        virtual void Unlock(eC_UInt uiX1 = 0, eC_UInt uiY1 = 0, eC_UInt uiX2 = 0, eC_UInt uiY2 = 0)
        {
            // Dummy implementation of pure-virtual function in base-class
        }

        virtual void DrawLine(const eGML_Vector2 &kStart, const eGML_Vector2 &kEnd, eGML_Color kColor, eC_Flag ulFlags, eC_Value vStartWidth, eC_Value vEndWidth = 0)
        {
            // Dummy implementation of pure-virtual function in base-class
        }

        virtual void DrawPolygon(const eGML_Coord2D *pkVertices, eC_UInt uiCount, eGML_Color kColor, eC_Flag ulFlags)
        {
            // Dummy implementation of pure-virtual function in base-class
        }

        virtual void Clear(eGML_Color clColor)
        {
            // Dummy implementation of pure-virtual function in base-class
        }

        virtual void FillRect(eC_UInt uiWidth, eC_UInt uiHeight, eC_Int iX1, eC_Int iY1, eGML_Color clColor, eC_Flag ulFlags = 0)
        {
            // Dummy implementation of pure-virtual function in base-class
        }
    };

public:
    /** Create a FntWrap-FreeType object. It should be created
    in implementation after creation of GfxWrap.
    @param uiCacheSize The max size of the freetype cache in bytes.
    @param pScreen The eGML screen in which the text will be shown.
    @param uiMaxNumFaces Maximum number of opened font face objects managed by the FreeType cache.
    @param uiMaxNumSizeObjects Maximum number of opened size objects managed by the FreeType cache.
    @return True, if it is successfully created, False otherwise.
    */
    static eC_Bool CreateInstance(const eC_UInt& uiCacheSize,
                                  eGML_Screen* pScreen,
                                  const eC_UInt& uiMaxNumFaces = 10,
                                  const eC_UInt& uiMaxNumSizeObjects = 10);

protected:
    /** Renders a glyph to the screen.
    This function takes the 8 bit alpha image provided by freetype adds color and blits it to the screen.
    @param vX1 The x coordinate where the glyph image is rendered.
    @param vY1 The y coordinate where the glyph image is rendered.
    @param kGlyphImage The glyph image containing a 8 bit alpha image of the glyph.
    */
    virtual void RenderGlyphToScreen(const eC_Value &vX1, const eC_Value &vY1, CGUIImageData& kGlyphImage);

    virtual void SetRenderTarget(const CGUIRenderTarget &rRenderBitmap);

private:
    /*
        Constructor
        @param uiCacheSize The maximum size of the FreeType cache in byte.
        @param pScreen The eGML screen which is the render target for glyph images.
        @param uiMaxNumFaces Maximum number of opened font face objects managed by the FreeType cache.
        @param uiMaxNumSizeObjects Maximum number of opened size objects managed by the FreeType cache.
    */
    CFntWrapFreeTypeeGML(const eC_UInt& uiCacheSize,
                         eGML_Screen* pScreen,
                         const eC_UInt& uiMaxNumFaces,
                         const eC_UInt& uiMaxNumSizeObjects
                         );

    virtual ~CFntWrapFreeTypeeGML(void);

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
        */
    CFntWrapFreeTypeeGML(const CFntWrapFreeTypeeGML& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapFreeTypeeGML& operator=(const CFntWrapFreeTypeeGML& kSource);

    /// Pointer of the eGML screen in which we will create the temporary bitmaps for glyphs
    eGML_Screen *m_pScreen;

    /// Pointer to the eGML Bitmap in which the text will be printed.
    eGML_Bitmap *m_pRenderTarget;

    // We need a Modifyable eGML Bitmap which we can repeatedly use to Blit the glyphs with eGML
    CModifyableBitmap *m_pModifyableGlyphBitmap;
};
#endif
