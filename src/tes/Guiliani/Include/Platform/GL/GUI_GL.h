/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUI_GL__H__
#define __GUI_GL__H__

#if !defined GUILIANI_USE_OGL
    #if defined GUILIANI_USE_OGLES2
        #include <GLES2/gl2.h>
    #else
        //include path for old versions of EGL
        //#include <GLES/egl.h>
        //#include <GLES/gl.h>

        //include path for new versions of EGL (includes changed)
        #if defined SYSTEM_NAME_DARWIN //the header path is different on Darwin systems (iOS/MacOS)
            #include <OpenGLES/ES1/gl.h>
        #else
            #include <GLES/gl.h>
        #endif
    #endif
#else
    #ifdef _WIN32
        #include <windows.h> // needed for gl.h on windows
    #endif

#if defined SYSTEM_NAME_DARWIN
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>

    // Use extension for VBO and FBO 
    //defines for the VBO extensions
    #define glDeleteBuffers             CGfxWrapGL::glDeleteBuffers_Guiliani
    #define glBindBuffer                CGfxWrapGL::glBindBuffer_Guiliani
    #define glGenBuffers                CGfxWrapGL::glGenBuffers_Guiliani
    #define glBufferData                CGfxWrapGL::glBufferData_Guiliani

    //defines for the FBO extensions
    #define glGenFramebuffers           CGfxWrapGL::glGenFramebuffers_Guiliani
    #define glBindFramebuffer           CGfxWrapGL::glBindFramebuffer_Guiliani
    #define glFramebufferTexture2D      CGfxWrapGL::glFramebufferTexture2D_Guiliani
    #define glCheckFramebufferStatus    CGfxWrapGL::glCheckFramebufferStatus_Guiliani
    #define glDeleteFramebuffers        CGfxWrapGL::glDeleteFramebuffers_Guiliani

#endif


#endif

#endif //#ifndef _GUI_GL__H__
