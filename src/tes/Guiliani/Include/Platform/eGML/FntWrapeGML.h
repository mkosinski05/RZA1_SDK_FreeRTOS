/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __FNT_WRAP_EGML__H__
#define __FNT_WRAP_EGML__H__

#include "eC_Types.h"
#include "eGML_Screen.h"
#include "FntWrap.h"

class eFnt_Bitmap;
class eImg_Bitmap;

/// eGML implementation of the font wrapper.

/**
    This is the eGML implementation of the font wrapper.
    It uses eFnt for font functionalities.
    @ingroup GUILIANI_GRAPHICS
*/

class CFntWrapeGML :
    public CFntWrap
{
public:
    /** Create a FntWrap-eGML object. It should be created
        in implementation after creation of GfxWrap.
        @param pScreen The eGML screen in which the text will be shown.
        @param bUseSubpixel Flag to switch subpixel positioning on or off. The default value is true.
        @return True if it is successfully created, False otherwise.
     */
    static eC_Bool CreateInstance(eGML_Screen* pScreen, const eC_Bool bUseSubpixel = true);

protected:

    virtual void SetRenderTarget(const CGUIRenderTarget& rRenderBitmap);

private:
    virtual void Text(const eC_Value &vX1, const eC_Value &vY1, const eC_String * const lpString);

    /// ---------------------------------------------------------------
    /// Font access
    /// ---------------------------------------------------------------

    virtual eC_Bool SetNOFFonts(const eC_UInt uiNOFFonts);
    virtual void LoadFont(const FontResource_t &eFontID);
    virtual void SetFont(const FontResource_t &eID);
    virtual void UnloadFont(const FontResource_t &eFontID);

    /** Calculates the required space in x- and y-direction of a given font.
    @param pkText The text.
    @param vWidth Reference to width of the text in pixels.
    @param vHeight Reference to height of the text in pixels.
    */
    virtual void RequiredSpace(const eC_String * const pkText, eC_Value &vWidth, eC_Value &vHeight);

    /** Default-constructor. Should not be used.
    *   Dummy declaration with no implementation, just to hide the function.
    */
    CFntWrapeGML();

    /// Constructor.
    CFntWrapeGML(eGML_Screen *pScreen, const eC_Bool bUseSubpixel);

    virtual ~CFntWrapeGML(void);

    /** Copy-constructor. Should not be used.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapeGML(const CFntWrapeGML& kSource);

    /** Operator= method. Should not be used.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CFntWrapeGML& operator=(const CFntWrapeGML& kSource);

    /// Reference of the eGML screen in which the text will be printed.
    eGML_Screen& m_rScreen;

    /// Reference of the eGML Bitmap in which the text will be printed.
    eGML_Bitmap* m_pRenderBitmap;

    /// Array of eGML Bitmap Fonts
    eC_TArray<eFnt_Bitmap*> m_pFont;

    /// Use subpixel positioning
    eC_Bool m_bUseSubpixel;
};
#endif
