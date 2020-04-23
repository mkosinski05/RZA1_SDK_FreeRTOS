/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GFXWRAPOGLWIN__H_
#define GFXWRAPOGLWIN__H_

#include <windows.h>
#include "GfxWrapOGL.h"

/**
    <h3>Platform Specific Limitations </h3>
*/

// @guiliani_doxygen limitations CGfxWrapOGL
/**
There are some limitations:
        - FntWrapWinOGL does currently only support ASCII Characters.\n
        - It does not return valid values for "RequiredSpace". That means word-wrap and text truncation do not work as expected.
        (Class CGfxWrapOGLwin)
*/
// @endguiliani_doxygen
/**
    See @ref subsec_limitations_CGfxWrapOGL "CGfxWrapOGLwin Platform Specific Limitations"
*/

/// The Windows implementation of the OpenGL wrapper.
class CGfxWrapOGLwin :
    public CGfxWrapOGL
{
public:
    /** CreateInstance
        Singleton for creating a GfxWrapOGLwin object and change the pointer in
        base class to this graphics wrapper.
        @param hWnd Handle of the application's window.
        @param uiScreenWidth The screen width.
        @param uiScreenHeight The screen height.
        @return Indicate whether by this call, an instance is created.
        The instance is created, if true, not created, otherwise.
    */
    static eC_Bool CreateInstance(const HWND &hWnd,
                                  const eC_UInt &uiScreenWidth,
                                  const eC_UInt &uiScreenHeight);
    ///@return the Window handle
    inline HDC GetScreen() {return m_hDC;}

    virtual eC_Bool Refresh    (const RefreshCall_t eRefreshCallType);

protected:

    HGLRC       m_hRC;              ///< Context for rendering commands in OpenGL
    HDC         m_hDC;              ///< Device context to connect the window with GDI
    HWND        m_hWnd;             ///< Handle to the GDI window

    /** Constructor
        Protected because of the singleton pattern.
        @param hWnd Window handle
    */
    CGfxWrapOGLwin(const HWND &hWnd);
    /** Destructor
        Protected because of the singleton pattern.
    */
    virtual ~CGfxWrapOGLwin(void);

private:
    eC_Bool Init          (void);
    eC_Bool DeInit          (void);

    /** Copy-constructor has intentionally no implementation.
    */
    CGfxWrapOGLwin(const CGfxWrapOGLwin& kSource);

    /** Operator= method has intentionally no implementation.
    */
    CGfxWrapOGLwin& operator=(const CGfxWrapOGLwin& kSource);

    /** Pixelformatdescriptor
        Stores all needed pixel information for drawing OpenGL
        in a given GDI window.
        Important: Static variable is used, because the same pixelformatdescriptor
               may used for more OGL windows.
    */
    static PIXELFORMATDESCRIPTOR m_hPFD;
};
#endif
