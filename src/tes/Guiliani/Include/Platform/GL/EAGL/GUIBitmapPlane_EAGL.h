/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIBITMAPLANE_EAGL__H_
#define GUIBITMAPLANE_EAGL__H_

#include "GUIBitmapPlane_GL.h"


class CGfxWrapEAGL;

/** @brief The BitmapPlane Class for EAGL with framebuffer object (FBO) support.
    @ingroup GUILIANI_BITMAPPLANES
*/
class CGUIBitmapPlane_EAGL : public CGUIBitmapPlane_GL
{
    friend class CGfxWrapEAGL;

protected:
    /** Constructor
        @param uiNewBitmapID Bitmap ID
        @param rkParentObject Parent object
        @param vWidth Width
        @param vHeight Height
    */
    CGUIBitmapPlane_EAGL(GUIBitmapPlaneID_t uiNewBitmapID, const CGUIObject& rkParentObject, eC_Value vWidth, eC_Value vHeight);
    virtual ~CGUIBitmapPlane_EAGL();

    virtual void CreateRenderSurface();
    eC_UInt GetRenderSurface();///<@return The render surface
    virtual void DeleteRenderSurface();

private:
    eC_UInt m_uiFBO; ///< Surface for Offscreen rendering and used as texture
};

#endif  //#if !defined GUIBITMAPLANE_EAGL__H_
