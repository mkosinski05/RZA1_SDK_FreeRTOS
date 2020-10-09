/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined __GFXWRAPOGLES2__H__
#define __GFXWRAPOGLES2__H__

//#include "GfxWrapOGLES1_1.h"
#include "GfxWrapOGLES.h"//neu
#include "GUI_GL.h"


/// OpenGL ES 2.x graphics wrapper implementation with FBO support.

/** This graphics wrapper uses OpenGL ES 2.x
    BitmapPlanes are realized via Framebuffer-Objects.
*/
class CGfxWrapOGLES2 
    : public CGfxWrapOGLES
{
public:
    /** Singleton for creating a GfxWrapOGLES object and change the pointer in
        base class to this graphic wrap.
        @return True if an instance was created by this call, otherwise False.
        @param[in] hWindowType which will be used for drawing
        @param[in] hDisplayType which will be used for drawing
    */
    static eC_Bool CreateInstance(const NativeWindowType& hWindowType = 0,
                                  const NativeDisplayType& hDisplayType = EGL_DEFAULT_DISPLAY);

protected:

    /** Constructor
        @param[in] hWindowType which will be used for drawing
        @param[in] hDisplayType which will be used for drawing
        @param[in] uiVersion OpenGL ES version to instantiate
    */
    CGfxWrapOGLES2(const NativeWindowType& hWindowType,
                   const NativeDisplayType& hDisplayType,
                   const eC_UInt& uiVersion);

    /** Destructor */
    virtual ~CGfxWrapOGLES2();

    CGUIBitmapPlane* CreateGfxDependentBitmap(GUIBitmapPlaneID_t uiNewBitmapID, const CGUIObject& rkParentObject, const eC_Value &vWidth, const eC_Value &vHeight);
};

#endif //#if !defined __GFXWRAPOGLES2__H__
