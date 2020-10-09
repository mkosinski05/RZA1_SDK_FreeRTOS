/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIBITMAPPLANE_RGA_H_
#define GUIBITMAPPLANE_RGA_H_

#include "GUIBitmapPlane.h"
#include "RGA_Platform.h"
#include "RGAImageData.h"

class CGfxWrapRGA;

/** @brief The BitmapPlane Class for RGA
<h3>Platform Specific Limitations </h3>
*/

/**
 Rotations around x-axis and y-axis are not supported. (Class CGUIBitmapPlane_RGA)
 See @ref subsec_limitations_CGfxWrapRGA "CGfxWrapRGA Platform Specific Limitations"
 @ingroup GUILIANI_BITMAPPLANES
*/
class CGUIBitmapPlane_RGA: public CGUIBitmapPlane
{
    friend class CGfxWrapRGA;

protected:
    /** Constructor
        @param uiNewBitmapID Bitmap ID
        @param rkParentObject Parent object
        @param vWidth Width
        @param vHeight Height
    */
    CGUIBitmapPlane_RGA(GUIBitmapPlaneID_t uiNewBitmapID, const CGUIObject& rkParentObject,
                        eC_Value vWidth, eC_Value vHeight);
    /** Destructor
    */
    virtual ~CGUIBitmapPlane_RGA();

    /** Draws this BitmapPlane. The Bitmap will be drawn to the current RenderTarget, with the set rotation and
        position. The set cliprect also applies to this drawing.
    */
    virtual void DoDraw();
    
    /** Sets the alpha values for all corners of the Bitmap plane. Please note: In RGA only ONE alpha value for the
        whole plane is supported. This implementation takes the value in "ubTopLeft".
        @param ubTopLeft alphaValue for the upper left corner
        @param ubBottomLeft alphaValue for the lower left corner
        @param ubBottomRight alphaValue for the lower right corner
        @param ubTopRight alphaValue for the upper right corner
    */
    virtual void SetAlphaValues(const eC_UByte ubTopLeft, const eC_UByte ubBottomLeft,
                                const eC_UByte ubBottomRight, const eC_UByte ubTopRight)
    {
        m_ubAlpha = ubTopLeft;
    }
    
    /** Clears this bitmap with the given color.
        @param ubRed is the red part of the clear color
        @param ubGreen is the green part of the clear color
        @param ubBlue is the blue part of the clear color
        @param ubAlpha is the alpha part of the clear color
    */
    virtual void Clear(const eC_UByte ubRed, const eC_UByte ubGreen, const eC_UByte ubBlue, const eC_UByte ubAlpha);
    
    /** Changes the RenderTarget to the BitmapPlane so that all successive drawing operations will be done in the
        bitmap plane.
        @return True, if RenderTarget is successfully changed to the BitmapPlane, False otherwise.
     */
    virtual eC_Bool DrawToBitmap();

private:
    Canvas2D_ContextClass *m_pkCanvas2d; ///< Graphics drawing context of RGA
    frame_buffer_t  m_kTextureBuffer; ///< Frame buffer for texture or bitmap plane
    eC_UByte m_ubAlpha; ///< Alpha value 0-255
};

#endif /* GUIBITMAPPLANE_RGA_H_ */
