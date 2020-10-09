/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined __GFXWRAPOGLES1_1__H__
#define __GFXWRAPOGLES1_1__H__

#include "GfxWrapOGLES.h"
#include "GUI_GL.h"

class CGUIBitmapPlane_PBuffer;


/// OpenGL ES 1.x graphics wrapper implementation with PBuffer support.

/** This graphic wrapper is derived from the CGfxWrapOGLES base class and uses PBuffer instead of the workaround. */
class CGfxWrapOGLES1_1 : public CGfxWrapOGLES
{
public:
    friend class CGUIBitmapPlane_PBuffer;

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
    CGfxWrapOGLES1_1(const NativeWindowType& hWindowType,
                     const NativeDisplayType& hDisplayType,
                     const eC_UInt& uiVersion);

    /** Destructor */
    virtual ~CGfxWrapOGLES1_1();

    /** Creates the graphic wrapper dependant bitmap (PBuffer in this case)
        @param[in] uiNewBitmapID Bitmap plane id
        @param[in] rkParentObject Parent object
        @param[in] vWidth Width of bitmap plane
        @param[in] vHeight Height of bitmap plane
        @return The created bitmap
    */
    virtual CGUIBitmapPlane* CreateGfxDependentBitmap(GUIBitmapPlaneID_t uiNewBitmapID,
                                                      const CGUIObject& rkParentObject,
                                                      const eC_Value &vWidth, const eC_Value &vHeight);

    /// Attribute keeps information if offscreen context is ready or not.
    eC_Bool m_bOffscreenContextReady;

private:

};

#endif //#if !defined __GFXWRAPOGLES1_1__H__
