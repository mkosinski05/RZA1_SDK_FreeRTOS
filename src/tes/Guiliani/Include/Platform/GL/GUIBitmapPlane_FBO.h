/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUIBITMAPLANE_FBO__H_
#define GUIBITMAPLANE_FBO__H_

#include "GUIBitmapPlane_GL.h"


/** @brief The BitmapPlane Class for OpenGL(ES) with framebuffer object (FBO) support.
    @ingroup GUILIANI_BITMAPPLANES
*/
class CGUIBitmapPlane_FBO : public CGUIBitmapPlane_GL
{
    friend class CGfxWrapOGLES2;
    friend class CGfxWrapOGL;

public:
    /** Returns the internal framebuffer object
        @return Framebuffer Object ID 
    */
    eC_UInt GetRenderSurface();

protected:
    /** Constructor
        @param uiNewBitmapID Bitmap ID
        @param rkParentObject Parent object
        @param vWidth Width
        @param vHeight Height
    */
    CGUIBitmapPlane_FBO(GUIBitmapPlaneID_t uiNewBitmapID, const CGUIObject& rkParentObject, eC_Value vWidth, eC_Value vHeight);
    virtual ~CGUIBitmapPlane_FBO();

    virtual void CreateRenderSurface();
    virtual void DeleteRenderSurface();

    virtual eC_Bool DrawToBitmap();
    virtual eC_Bool DrawToScreen();

private:
    eC_UInt m_uiFBO; ///< Surface for Offscreen rendering and used as texture
};

#endif  //#if !defined GUIBITMAPLANE_FBO__H_
