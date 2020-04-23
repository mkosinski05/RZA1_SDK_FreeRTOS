/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GL_API_MAPPINGS_H__
#define __GL_API_MAPPINGS_H__

#if defined GUILIANI_USE_OGL
    // this must be included for the vbo extensions
    #ifdef WIN32
        #include <windows.h> // needed for gl.h on windows
    #endif
    #if defined SYSTEM_NAME_DARWIN
        #include <OpenGL/gl.h>
        #include <OpenGL/glext.h>
    #else
        #include <GL/gl.h>
        #include <GL/glext.h>
    #endif
#endif

// Fixpoint environments (embedded targets) usually only support OpenGL ES.
// Float is usually supported on (non-ES) OpenGL desktop computers.
// Therefore, we do a simple mapping:
// Fixpoint -> OpenGL ES
// Float -> OpenGL

#if defined eC_FIXPOINT && (defined GUILIANI_USE_OGL || defined GUILIANI_USE_OGLES2)
  #error Fixpoint is not supported in OGL and OGLES2 mode!
#endif

// maps the gl calls
#if defined eC_FIXPOINT
    #define GL_EC_VALUE GL_FIXED
    #define glTexEnvV glTexEnvx
    #define glScaleV glScalex
    #define glColor4V glColor4x
    #define glOrthoV glOrthox
    #define glFrustumV glFrustumx
    #define glTranslateV glTranslatex
    #define glClearColorV glClearColorx
    #define glTexParameterV glTexParameterx
    #define glTexEnvV glTexEnvx
    #define glLoadMatrixV(m) glLoadMatrixx((GLfixed*)(m))
    #define glMultMatrixV(m) glMultMatrixx((GLfixed*)(m))
    #define glLineWidthV glLineWidthx
    #define glClearDepthV glClearDepthx
    // Use self implemented routine here. No more dependencies to glu library,
    // when gluPerspectivex is not needed.
    #define gluPerspectiveV static_cast<CGfxWrapGL&>(GETGFX).SetPerspectiveProjection
    #define glRotateV glRotatex
    #define glNormal3V glNormal3x
    #define glMaterialV glMaterialx
    #define glMaterialVv(a, b, c) glMaterialxv(a, b, (GLfixed*)(c))
    #define glLightV glLightx
    #define glLightVv(a, b, c) glLightxv(a, b, (GLfixed*)(c))
    #define glLightModelVv(a, b) glLightModelxv(a, (GLfixed*)(b))
    #define glGetV(a, b) glGetFixedv(a, (GLfixed*)(b))
#else
    #if defined GUILIANI_USE_OGLES2
        #define glTranslateV static_cast<CGfxWrapGL&>(GETGFX).SetTranslation
        #define glColor4V(r, g, b, a) glUniform4f(glGetUniformLocation(static_cast<CGfxWrapGL&>(GETGFX).GetProgramObject(), "Color"), eC_ToFloat(r), eC_ToFloat(g), eC_ToFloat(b), eC_ToFloat(a))
        #define glOrthoV static_cast<CGfxWrapGL&>(GETGFX).SetOrthoProjection
        #define glFrustumV static_cast<CGfxWrapGL&>(GETGFX).SetFrustumProjection
        #define glRotateV(a, b, c, d) static_cast<CGfxWrapGL&>(GETGFX).SetRotation(a, b, c, d)
        #define glScaleV static_cast<CGfxWrapGL&>(GETGFX).SetScaling
        #define GL_PROJECTION CGfxWrapGL::MT_PROJECTION
        #define GL_MODELVIEW CGfxWrapGL::MT_MODELVIEW
        #define glMatrixMode static_cast<CGfxWrapGL&>(GETGFX).SetMatrixMode
        #define glLoadIdentity static_cast<CGfxWrapGL&>(GETGFX).LoadIdentity
        #define glPushMatrix static_cast<CGfxWrapGL&>(GETGFX).PushMatrix
        #define glPopMatrix static_cast<CGfxWrapGL&>(GETGFX).PopMatrix
        #define glLoadMatrixV static_cast<CGfxWrapGL&>(GETGFX).SetMatrix
        #define glEnableClientState glEnableVertexAttribArray
        #define glDisableClientState glDisableVertexAttribArray
        #define glVertexPointer(a, b, c, d) glVertexAttribPointer(GL_VERTEX_ARRAY, a, b, GL_FALSE, c, d)
        #define glTexCoordPointer(a, b, c, d) glVertexAttribPointer(GL_TEXTURE_COORD_ARRAY, a, b, GL_FALSE, c, d)
        #define glColorPointer(a, b, c, d) glVertexAttribPointer(GL_COLOR_ARRAY, a, b, GL_FALSE, c, d)
    #else
        #define glTranslateV(x, y, z) glTranslatef(eC_ToFloat(x), eC_ToFloat(y), eC_ToFloat(z))
        #define glColor4V(r, g, b, a) glColor4f(eC_ToFloat(r), eC_ToFloat(g), eC_ToFloat(b), eC_ToFloat(a))
        #define glRotateV(a, b, c, d) glRotatef(eC_ToFloat(a), eC_ToFloat(b), eC_ToFloat(c), eC_ToFloat(d))
        #define glTexEnvV glTexEnvf
        #define glScaleV glScalef
        #define glLoadMatrixV(a) glLoadMatrixf(reinterpret_cast<const GLfloat*>(a))
        #define glMultMatrixV glMultMatrixf
        #define glNormal3V(x, y, z) glNormal3f(eC_ToFloat(x), eC_ToFloat(y), eC_ToFloat(z))
        #define glMaterialV(a, b, c) glMaterialf(a, b, eC_ToFloat(c))
        #define glMaterialVv glMaterialfv
        #define glLightV(a, b, c) glLightf(a, b, eC_ToFloat(c))
        #define glLightVv glLightfv
        #define glLightModelVv glLightModelfv
        #if defined GUILIANI_USE_OGL
            #define glOrthoV(a, b, c, d, e, f) glOrtho(eC_ToFloat(a), eC_ToFloat(b), eC_ToFloat(c), eC_ToFloat(d), eC_ToFloat(e), eC_ToFloat(f))
            #define glFrustumV(a, b, c, d, e, f) glFrustum(eC_ToFloat(a), eC_ToFloat(b), eC_ToFloat(c), eC_ToFloat(d), eC_ToFloat(e), eC_ToFloat(f))
        #else
            #define glOrthoV(a, b, c, d, e, f) glOrthof(eC_ToFloat(a), eC_ToFloat(b), eC_ToFloat(c), eC_ToFloat(d), eC_ToFloat(e), eC_ToFloat(f))
            #define glFrustumV(a, b, c, d, e, f) glFrustumf(eC_ToFloat(a), eC_ToFloat(b), eC_ToFloat(c), eC_ToFloat(d), eC_ToFloat(e), eC_ToFloat(f))
        #endif
    #endif
    #define GL_EC_VALUE GL_FLOAT
    #define glClearColorV(r, g, b, a) glClearColor(eC_ToFloat(r), eC_ToFloat(g), eC_ToFloat(b), eC_ToFloat(a))
    #define glTexParameterV(target, pname, param) glTexParameterf(eC_ToFloat(target), eC_ToFloat(pname), eC_ToFloat(param))
    #define glLineWidthV glLineWidth
    #define glClearDepthV(a) glClearDepth(eC_ToFloat(a))
    #define gluPerspectiveV static_cast<CGfxWrapGL&>(GETGFX).SetPerspectiveProjection
    #define glGetV glGetFloatv
#endif

// Macro to simplify the call of the Switch3D function.
#define SWITCH_TO_3D ((CGfxWrapGL&)GETGFX).Switch3D(GETGFX.GetVirtualScreenWidth(), \
    GETGFX.GetVirtualScreenHeight(), GetAbsXPos(), GetAbsYPos(), GetRelZPos(), \
    GetZeroZ(), GetWidth(), GetHeight(), GetInitWidth(), GetInitHeight(), \
    GetRotateX(), GetRotateY(), GetRotateZ(), GetNearPlane(), GetFarPlane(), \
    GetPerspectiveAngle())

// Index to bind the attributes to vertex shaders
#if defined GUILIANI_USE_OGLES2
    #define GL_VERTEX_ARRAY         0
    #define GL_TEXTURE_COORD_ARRAY  1
    #define GL_COLOR_ARRAY          2
#endif

#if defined SYSTEM_NAME_DARWIN
    #define glBindBuffer_Guiliani glBindBuffer
    #define glBufferData_Guiliani glBufferData
    #define glGenBuffers_Guiliani glGenBuffers
    #define glDeleteBuffers_Guiliani glDeleteBuffers
    #define glGenFramebuffers_Guiliani glGenFramebuffers
    #define glBindFramebuffer_Guiliani glBindFramebuffer
    #define glFramebufferTexture2D_Guiliani glFramebufferTexture2D
    #define glCheckFramebufferStatus_Guiliani glCheckFramebufferStatus
    #define glDeleteFramebuffers_Guiliani glDeleteFramebuffers
#endif

// Type for the 3d matrix
const eC_UByte cubNOFMatrixElements = 4;
typedef eC_Value Matrix3D_t[cubNOFMatrixElements][cubNOFMatrixElements];

// the identity matrix
const Matrix3D_t cavIdentityMatrix = {
    {eC_FromInt(1), eC_FromInt(0), eC_FromInt(0), eC_FromInt(0)},
    {eC_FromInt(0), eC_FromInt(1), eC_FromInt(0), eC_FromInt(0)},
    {eC_FromInt(0), eC_FromInt(0), eC_FromInt(1), eC_FromInt(0)},
    {eC_FromInt(0), eC_FromInt(0), eC_FromInt(0), eC_FromInt(1)}
};

#endif //#ifndef __GL_API_MAPPINGS_H__
