/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined __GUIBITMAPPLANE_PBUFFER__H__
#define __GUIBITMAPPLANE_PBUFFER__H__

#include <EGL/egl.h>
#include "GUIBitmapPlane_GL.h"

/** @brief The BitmapPlane Class for EGL with PBuffer support.
    @ingroup GUILIANI_BITMAPPLANES
*/
class CGUIBitmapPlane_PBuffer : public CGUIBitmapPlane_GL
{
    friend class CGfxWrapOGLES1_1;

public:
    /** Returns the PBuffer's internal EGLSurface. The surface may be destroyed at any time, so do not expect it to remain unchanged.
        @return EGL render surface */
    EGLSurface& GetRenderSurface();

protected:
    /** Constructor
        @param uiNewBitmapID Bitmap plane ID
        @param rkParentObject Parent object
        @param vWidth Width
        @param vHeight Height
    */
    CGUIBitmapPlane_PBuffer(GUIBitmapPlaneID_t uiNewBitmapID, const CGUIObject& rkParentObject, eC_Value vWidth, eC_Value vHeight);
    virtual ~CGUIBitmapPlane_PBuffer();

    virtual void CreateRenderSurface();
    virtual void DeleteRenderSurface();

    virtual eC_Bool DrawToBitmap();

    virtual eC_Bool DrawToScreen();
    
    virtual eC_Bool FinishDrawToBitmap();

private:
    EGLSurface m_RenderSurface; ///< Surface for Offscreen rendering and used as texture
    eC_Bool m_bOffscreenContextReady; ///< Flag indicating if the offscreen context has been fully initialized
};

#endif //#if !defined __GUIBITMAPPLANE_PBUFFER__H__
