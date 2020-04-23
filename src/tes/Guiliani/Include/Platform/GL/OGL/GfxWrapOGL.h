/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GFXWRAPOGL__H_
#define GFXWRAPOGL__H_

#include "GfxWrapGL.h"

/// OpenGL (non-'OpenGL-ES') graphic wrapper.

class CGfxWrapOGL :
    public CGfxWrapGL
{
public:
    virtual eC_Bool Refresh    (const RefreshCall_t eRefreshCallType);

protected:
    /** Constructor
        Protected because of the singleton pattern.
    */
    CGfxWrapOGL(void);
    /** Destructor
        Protected because of the singleton pattern.
    */
    virtual ~CGfxWrapOGL(void);

    /** Init
        Intended to be executed in
        user specific OS implementations of the OGLwrapper.
        @return True if OK, False otherwise
    */
    eC_Bool Init    (void);

    /** DeInit
        Intended to be executed in
        user specific OS implementations of the OGLwrapper.
        @return True if OK, False otherwise
    */
    eC_Bool DeInit    (void);

    eC_Bool m_bFBOSupported;    ///< Indicates if FrameBufferObjects are supported on this machine

    virtual CGUIBitmapPlane* CreateGfxDependentBitmap(
        GUIBitmapPlaneID_t uiNewBitmapID,
        const CGUIObject& rkParentObject,
        const eC_Value &vWidth, const eC_Value &vHeight);

private:
    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGfxWrapOGL(const CGfxWrapOGL& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGfxWrapOGL& operator=(const CGfxWrapOGL& kSource);
};
#endif
