/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined __EGLENVIRONMENT_H__
#define __EGLENVIRONMENT_H__

#include <EGL/egl.h>
#include "GUITrace.h"

/**
The official EGL Specification released by Khronos states in Appendix D Version 1.3
"For naming consistency, some tokens from EGL 1.2 have been renamed [...] The old names are also retained for backwards compatibility."
Unfortunately this is not implemented for some SDKs. The following definitions resolve this incompatibility.
*/
#ifdef GUILIANI_EGL13_COMPATIBILITY_WORKAROUND
#undef EGL_DEFAULT_DISPLAY
#define EGL_DEFAULT_DISPLAY     ((EGLNativeDisplayType)0)
typedef EGLNativeWindowType     NativeWindowType;
typedef EGLNativeDisplayType    NativeDisplayType;
#endif

/// Wrapper class for the EGL interface.

/** This class wraps EGL functionality and instantiates the EGL surface. */
class CEGLEnvironment
{
public:
    /** Constructor
        @param[in] hWindowType Window handle
        @param[in] hDisplayType Display handle
        @param[in] uiVersion OpenGL ES version to instantiate
    */
    CEGLEnvironment(const NativeWindowType& hWindowType, const NativeDisplayType& hDisplayType, const eC_UInt& uiVersion);

    /** Destructor */
    ~CEGLEnvironment();

    /** Checks for EGL error and prints the error code.
        @param[in] kCallingMethod Method name which calls this function. Used to print the method name.
        @return True if there is an error, False otherwise
    */
    static eC_Bool CheckEGLError(const eC_String &kCallingMethod);

    /** Call eglQuerySurface to get the screen dimensions.
        @param[out] uiWidth Screen width
        @param[out] uiHeight Screen height
    */
    void CreateScreen(EGLint& uiWidth, EGLint& uiHeight);

    /** Initialises the EGL surface.
        @return True if OK, False otherwise
    */
    eC_Bool Init();

    /** Deinitialises the EGL surface.
        @return True if OK, False otherwise
    */
    eC_Bool DeInit();

    /** Calls eglSwapBuffers.
        @return True if OK, False otherwise
    */
    eC_Bool Refresh();

    /** Returns the screen context.
        @return Screen context
    */
    inline const EGLContext GetScreenContext() const {return m_EGLScreenContext;}

    /** Returns the offscreen context.
        @return Offscreen context
    */
    inline const EGLContext GetOffScreenContext() const {return m_EGLOffScreenContext;}

    /** Returns the EGL display handle.
        @return EGL display handle
    */
    inline const EGLDisplay GetDisplay() const {return m_EGLDisplay;}

    /** Returns the screen config.
        @return Screen config
    */
    inline const EGLConfig GetScreenConfig() const {return m_ScreenConfig;}

    /** Returns the offscreen config.
        @return Offscreen config
    */
    inline const EGLConfig GetOffScreenConfig() const {return m_OffScreenConfig;}

    /** Returns the EGL window surface.
        @return EGL window surface
    */
    inline const EGLSurface GetWindowSurface() const {return m_EGLWindowSurface;}

    /** The screen buffer size. Total depth of the color buffer in bits.
        Feel free to set this as required BEFORE calling Createinstance() on derived graphic wrappers. */
    static eC_UInt ms_uiEGLScreenBufferSize;

    /** The offscreen buffer size. Total depth of the offscreen color buffer in bits.
        Feel free to set this as required BEFORE calling Createinstance() on derived graphic wrappers. */
    static eC_UInt ms_uiEGLOffScreenBufferSize;

    /** This allows the definition of custom attributes for the screen configuration.
        The given array of attributes will be handed over to eglChooseConfig during initialization.
        @code
            // Example custom configuration
            EGLint myconfig[] = {
                    EGL_BUFFER_SIZE, 32,
                    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
                    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
                    EGL_NONE
                };
            // Hand over custom configuration to CEGLEnvironment
            CEGLEnvironment::m_aiScreenConfig = myconfig;
            // Finally create instance of derived graphics wrapper
            CGfxWrapOGLES1_1::CreateInstance((NativeWindowType)hWnd, (NativeDisplayType) GetDC(hWnd));
        @endcode
        Feel free to set this as required BEFORE calling Createinstance() on derived graphic wrappers. */
    static EGLint* m_aiScreenConfig;

    /** This allows the definition of custom attributes for the OffScreen configuration.
        The given array of attributes will be handed over to eglChooseConfig during initialization.
        Feel free to set this as required BEFORE calling Createinstance() on derived graphic wrappers. */
    static EGLint* m_aiOffScreenConfig;

private:
    /** Helper method. */
    eC_Bool FindScreenConfig();

    /** Helper method. */
    eC_Bool FindOffScreenConfig();

    /// our EGL display
    EGLDisplay m_EGLDisplay;

    /// our EGL surface
    EGLSurface m_EGLWindowSurface;

    /// our EGL screen context
    EGLContext m_EGLScreenContext;

    /// our EGL offscreen context used for offscreen rendering
    EGLContext m_EGLOffScreenContext;

    /// our EGL configuration for the screen buffer
    EGLConfig m_ScreenConfig;

    /// our EGL configuration for the offscreen buffer
    EGLConfig m_OffScreenConfig;

    /// the EGL major version
    EGLint m_iEGLMajorVersion;

    /// the EGL minor version
    EGLint m_iEGLMinorVersion;

    /// version set by constructor parameters. Used to receive the correct EGLconfig
    eC_UInt m_uiVersion;

    /// Sets the wanted renderable type by given version number
    EGLint m_iRenderableType;

    /// the display handle
    NativeDisplayType m_hDisplayType;

    /// the window handle
    NativeWindowType m_hWindowType;
}; // class CEGLEnvironment

#endif //#if !defined __EGLENVIRONMENT_H__
