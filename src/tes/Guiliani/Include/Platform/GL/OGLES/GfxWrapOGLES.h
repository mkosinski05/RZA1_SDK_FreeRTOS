/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined __GFXWRAPOGLES__H__
#define __GFXWRAPOGLES__H__

#include "GfxWrapGL.h"
#include "EGLEnvironment.h"

#ifdef GUILIANI_GFXOGLDEBUG
/// Helper macro for convenient checks for OpenGL and EGL errors in debug builds.
#define CHECK_GL_AND_EGL_ERROR  CGfxWrapGL::CheckGLError(__FUNCTION__); CEGLEnvironment::CheckEGLError(__FUNCTION__);
#else
#define CHECK_GL_AND_EGL_ERROR
#endif


/// OGLES graphics wrapper base implementation.

/** This graphic wrapper implements the OpenGL ES base functions.
    All other OGLES wrapper are derived from this class.
    This class should be used if the target supports OpenGL ES 1.x and doesn't support
    PBuffers. A workaround will be used instead.
*/
class CGfxWrapOGLES : public CGfxWrapGL
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

    /** Returns the EGL environment object.
        @return EGL environment object.
    */
    inline CEGLEnvironment& GetEGLEnvironment() {return m_kEGLEnvironment;}

    virtual eC_Bool Refresh(const RefreshCall_t eRefreshCallType);

    /// Helper used to create the screen itself
    virtual void CreateScreen();

protected:
    /** Constructor
        @param[in] hWindowType which will be used for drawing
        @param[in] hDisplayType which will be used for drawing
        @param[in] uiVersion OpenGL ES version to instantiate
    */
    CGfxWrapOGLES(const NativeWindowType& hWindowType,
                  const NativeDisplayType& hDisplayType,
                  const eC_UInt& uiVersion);

    /** Destructor */
    virtual ~CGfxWrapOGLES();

    /** Method to initialize the graphic library
        @return returns True if the initialisation succeded, False otherwise.
    */
    virtual eC_Bool Init(void);

    /** Method to give the graphic library the opportunity to clean up
        @return returns True if the deinitialisation succeded, False otherwise.
    */
    virtual eC_Bool DeInit(void);

    /// the display handle
    NativeDisplayType m_hDisplayType;

    /// the window handle
    NativeWindowType m_hWindowType;

    /// The EGL enviroment object.
    CEGLEnvironment m_kEGLEnvironment;
};

#endif //__GFXWRAPOGLES__H__
