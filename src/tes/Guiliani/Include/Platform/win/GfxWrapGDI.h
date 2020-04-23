/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GFXWRAPGDI__H_
#define GFXWRAPGDI__H_

#include "GfxWrap.h"
#include "GfxEnvWin.h"

/// GDI implementation of the graphics wrapper.

/**
    For details see description of its base class CGfxWrap.
    <h3>Platform Specific Limitations </h3>
*/

// @guiliani_doxygen limitations CGfxWrapGDI
/**
 GfxWrapGDI does only support .BMP as image format. (Class CGfxWrapGDI)
 Rotation is not supported for blits by GfxWrapGDI.
 BitmapPlanes are not supported by GfxWrapGDI.
*/
// @endguiliani_doxygen
/**
    See @ref subsec_limitations_CGfxWrapOGL "CGfxWrapGDI Platform Specific Limitations"
*/
class CGfxWrapGDI : public CGfxWrap, protected CGfxEnvWin
{
public:
    /**
    Create the singleton instance and set the pointer in the
    base class to this graphics wrapper.
    @param hWnd The window instance for this wrapper.
    @param uiScreenWidth The screen width.
    @param uiScreenHeight The screen height.
    @return Indicate whether by this call, an instance is created.
            The instance is created, if true, not created, otherwise.
    */
    static eC_Bool CreateInstance(const HWND &hWnd, const eC_UInt &uiScreenWidth, const eC_UInt &uiScreenHeight);

    virtual eC_Bool Refresh(const RefreshCall_t eRefreshCallType);

    virtual void CreateScreen(const eC_UInt &uiWidth, const eC_UInt &uiHeight);
    virtual void DestroyScreen(){}

    inline eC_UInt GetVirtualScreenWidth() {return m_uiWidth;}
    inline eC_UInt GetVirtualScreenHeight() {return m_uiHeight;}

    virtual eC_UInt GetPhysicalScreenWidth() {return m_uiWidth;}
    virtual eC_UInt GetPhysicalScreenHeight(){return m_uiHeight;}

    virtual void SetForegroundColorImpl(const eC_UByte &ubRed, const eC_UByte &ubGreen, const eC_UByte &ubBlue, const eC_UByte &ubAlpha = 255);

    /** Sets background color
        @param ubRed Red value
        @param ubGreen Green value
        @param ubBlue Blue value
    */
    virtual void SetBackgroundColor(const eC_UByte &ubRed, const eC_UByte &ubGreen, const eC_UByte &ubBlue);

    virtual eC_Value SetLineWidth(const eC_Value &vWidth);

    virtual void Line(const eC_Value &vX1, const eC_Value &vY1, const eC_Value &vX2, const eC_Value &vY2);
    virtual void Rect(const eC_Value &vX1, const eC_Value &vY1, const eC_Value &vX2, const eC_Value &vY2);
    virtual void FilledRect(const eC_Value &vX1, const eC_Value &vY1, const eC_Value &vX2, const eC_Value &vY2);
    virtual void Ellipse(const eC_Value &vX1, const eC_Value &vY1, const eC_Value &vX2, const eC_Value &vY2, const eC_Bool &bFilled);
    virtual void SetCliprect(const CGUIRect &NewClipRect);

    /// @return The Windows device context
    inline HDC GetScreen(){return m_hMemoryDC;}

    virtual eC_Bool ImageExists(const ImageResource_t eID) const
    {
        eC_UInt uiCurImage = MAP_IMGID_TO_ARRAY(eID);
        return m_pImage[uiCurImage] != NULL;
    }

protected:
    /** Load an image
        @param psPath Path to image file
        @param eID Image ID
    */
    virtual void LoadImgImpl  (const eC_String& psPath, const ImageResource_t &eID);
    virtual void UnloadImg    (const ImageResource_t &eID);

    /// Calculates the size of an image in pixels. Called by the image cache.
    virtual eC_UInt GetImageSize(const ImageResource_t& eID) const;

    /** Implementation of image blitting.
        For detailed explanations, please refer to CGfxWrap::BlitImgExtImpl
        Please note that rotated blits are not supported for GDI
        @param eID          Identifier of the image to be blitted.
        @param uiSrcX       X coordinate of upper left corner of the relevant area within the source image
        @param uiSrcY       Y coordinate of upper left corner of the relevant area within the source image
        @param uiSrcWidth   Width of the of the relevant area within the source image
        @param uiSrcHeight  Height of the of the relevant area within the source image
        @param vDestX       Destination X position in absolute coordinates
        @param vDestY       Destination Y position in absolute coordinates
        @param vDestWidth   Destination width
        @param vDestHeight  Destination height
        @param ubAlpha      Degree of transparency. 0=transparent 255=opaque  Default is opaque.
        @param vAngle       Angle in degrees by which the image will be rotated (Not supported for GDI!)
        @param vRotCenterX  Rotation center X. In range of 0 to 1  (like U/V coordinates)
        @param vRotCenterY  Rotation center Y. In range of 0 to 1  (like U/V coordinates)
    */
    virtual void BlitImgExtImpl(const ImageResource_t &eID,
                                const eC_UInt &uiSrcX, const eC_UInt &uiSrcY, const eC_UInt &uiSrcWidth, const eC_UInt &uiSrcHeight,
                                const eC_Value &vDestX, const eC_Value &vDestY, const eC_Value &vDestWidth, const eC_Value &vDestHeight,
                                const eC_UByte &ubAlpha = 255,
                                const eC_Value &vAngle = eC_FromFloat(0),
                                const eC_Value &vRotCenterX = eC_FromFloat(0.5), const eC_Value &vRotCenterY = eC_FromFloat(0.5) );

    virtual eC_UInt GetImgWidthImpl (const ImageResource_t &eID) const;
    virtual eC_UInt GetImgHeightImpl(const ImageResource_t &eID) const;

    eC_UInt m_uiWidth; ///< Screen width
    eC_UInt m_uiHeight; ///< Screen height

    HBITMAP *m_pImage; ///< Pointer to image


    /** Constructor
        @param hWnd The window used to create the screen. It is given because GetActiveWindow function
                    may get the windows studio back and it is not good for debugging.
    */
    CGfxWrapGDI(const HWND &hWnd = NULL);
    virtual ~CGfxWrapGDI(void);

 private:
    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGfxWrapGDI(const CGfxWrapGDI& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGfxWrapGDI& operator=(const CGfxWrapGDI& kSource);
};
#endif
