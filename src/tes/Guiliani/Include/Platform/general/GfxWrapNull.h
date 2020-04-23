/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GFXWRAPNULL__H_
#define GFXWRAPNULL__H_

#include "GfxWrap.h"

/// The null Graphics-Wrapper class.
/**
    This is a dummy implementation of a graphic wrapper. You may wish to use this at an early stage of
    porting to a new platform, as long as there is no matching graphics implementation available. Another
    use of the null wrapper is for reducing the overall complexity for debugging.

    @ingroup GUILIANI_GRAPHICS
*/
class CGfxWrapNull:
    public CGfxWrap
{
public:
    /**
    Singleton for creating a GfxWrap-Null object and change the pointer in
    base class to this gfx wrapper.
    */
    inline static void CreateInstance()
    {
        if(NULL == ms_pGfxWrap)
        {
            ms_pGfxWrap = new CGfxWrapNull();
        }
    }

    inline eC_Bool Refresh(const RefreshCall_t eRefreshCallType) {return false;}
    inline void CreateScreen(
        const eC_UInt &uiWidth, const eC_UInt &uiHeight) {}
    inline eC_UInt GetVirtualScreenWidth() {return 0;}
    inline eC_UInt GetVirtualScreenHeight() {return 0;}

    inline void SetCliprect(const CGUIRect &NewAbsClipRect) {}

    inline void Line(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2) {}

    inline void Rect(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2) {}

    inline void FilledRect(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2) {}

    inline void Ellipse(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2,
        const eC_Bool &bFilled) {}

    inline void SetForegroundColorImpl(
        const eC_UByte &ubRed,
        const eC_UByte &ubGreen,
        const eC_UByte &ubBlue,
        const eC_UByte &ubAlpha = 255) 
    { 
        m_uiColor = UINT_FROM_ARGB(ubAlpha, ubRed, ubGreen, ubBlue); 
    }

    inline eC_UInt GetImgWidthImpl(const ImageResource_t &eID) const {return 0;}

    inline eC_UInt GetImgHeightImpl(const ImageResource_t &eID) const {return 0;}

    inline eC_UInt GetImageSize(const ImageResource_t &eID) const {return 0;}

    void BlitImgExtImpl(
        const ImageResource_t &eID,
        const eC_UInt &uiAbsSrcX, const eC_UInt &uiAbsSrcY,
        const eC_UInt &uiSrcWidth, const eC_UInt &uiSrcHeight,
        const eC_Value &vAbsDestX, const eC_Value &vAbsDestY,
        const eC_Value &vDestWidth, const eC_Value &vDestHeight,
        const eC_UByte &ubAlpha = 255,
        const eC_Value &vAngle = eC_FromFloat(0),
        const eC_Value &vRotCenterX = eC_FromFloat(0.5), const eC_Value &vRotCenterY = eC_FromFloat(0.5) ) {}

    void LoadImgImpl(const eC_String& kPath, const ImageResource_t &eID) {}

    eC_Bool ImageExists(const ImageResource_t eID) const { return false; }

    virtual eC_UInt GetPhysicalScreenWidth() {return 0;}

    virtual eC_UInt GetPhysicalScreenHeight() {return 0;}

protected:
    inline void DestroyScreen() {}

    virtual eC_Bool SetNOFImagesImpl(const eC_UInt uiNOFImages)
    {
        m_uiNOFImages = uiNOFImages;
        return true;
    };

private:
    CGfxWrapNull()
    {
        GUILOG(GUI_TRACE_WARNING, "------------------------\n");
        GUILOG(GUI_TRACE_WARNING, "|  GfxWrapNull chosen  |\n");
        GUILOG(GUI_TRACE_WARNING, "------------------------\n");
    }

    /** Copy-constructor.
        @param kSource Source object to be copied.
    */
    CGfxWrapNull(const CGfxWrapNull& kSource);

    /** Operator= method.
        @param kSource Source object to be copied.
        @return This instance.
    */
    CGfxWrapNull& operator=(const CGfxWrapNull& kSource);
};
#endif
