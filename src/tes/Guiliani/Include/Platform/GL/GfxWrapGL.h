/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GFXWRAPGL__H_
#define GFXWRAPGL__H_

#include "GfxWrap.h"
#include "GUIImageDataGL.h"
#include "GL_API_Mappings.h"

#ifdef GUILIANI_GFXOGLDEBUG
/// Helper macro for convenient checks for OpenGL errors in debug builds.
#define CHECK_GL_ERROR  CGfxWrapGL::CheckGLError(__FUNCTION__);
#else
#define CHECK_GL_ERROR
#endif

/// OpenGL graphic wrapper base class. This is the base class for all OpenGL related graphic wrappers (OpenGL and OpenGL-ES).
/** To understand how Guiliani is handling coordinatesystems under OpenGL please @see cvPixelCenterOffset.*/
class CGfxWrapGL:
    public CGfxWrap
{
    friend class CGUIBitmapPlane_GL;
public:
    /// Offset to get from OpenGL's coordinates to the pixel center.
    /// Note: OpenGL Pixel Coordinates do not refer to the "center" of the pixel, but to the edges between them.
    /// This means 0/0 would not point to the center of the top/left pixel, but instead to the top/left corner of this pixel
    /// Therefore, we move the coordinates to the center of the pixel by adding 0.375 pixels because of better compatibility 
    /// to more graphic cards.
    static const  eC_Value cvPixelCenterOffset;
    // ---------------------------------------------------------------
    // General functionality
    // ---------------------------------------------------------------

    void CreateScreen(const eC_UInt &uiWidth, const eC_UInt &uiHeight);

    virtual eC_UInt GetVirtualScreenWidth();
    virtual eC_UInt GetPhysicalScreenWidth();
    virtual eC_UInt GetVirtualScreenHeight();
    virtual eC_UInt GetPhysicalScreenHeight();
    virtual void SetScreenSize(const eC_UInt &uiWidth, const eC_UInt &uiHeight);

    // ---------------------------------------------------------------
    // Drawing functions
    // ---------------------------------------------------------------

    /** Sets the foreground color via r, g and b values and an alpha value
    @param ubRed Red intensity within a range of 0 - 255
    @param ubGreen Green intensity within a range of 0 - 255
    @param ubBlue Blue intensity within a range of 0 - 255
    @param ubAlpha alpha value
    */
    virtual void SetForegroundColorImpl(const eC_UByte &ubRed, const eC_UByte &ubGreen, const eC_UByte &ubBlue, const eC_UByte& ubAlpha=255);

    virtual eC_Value SetLineWidth(const eC_Value& vWidth);

    /** Draws a line from (vX1, vY1) to (vX2, vY2)
    @param vX1 X-Coordinate of starting point
    @param vY1 Y-Coordinate of starting point
    @param vX2 X-Coordinate of ending point
    @param vY2 Y-Coordinate of ending point
    */
    virtual void Line(const eC_Value &vX1, const eC_Value &vY1, const eC_Value &vX2, const eC_Value &vY2);

    /** Draws a rectangle with the top left edge at (vX1, vY1) and bottom right edge at (vX2, vY2)
    @param vX1 X-Coordinate of top left corner
    @param vY1 Y-Coordinate of top left corner
    @param vX2 X-Coordinate of bottom right corner
    @param vY2 Y-Coordinate of bottom right corner
    */
    virtual void Rect(const eC_Value &vX1, const eC_Value &vY1, const eC_Value &vX2, const eC_Value &vY2);

    /** Draws a filled rectangle with the top left edge at (vX1, vY1) and bottom right edge at (vX2, vY2)
    @param vX1 X-Coordinate of top left corner
    @param vY1 Y-Coordinate of top left corner
    @param vX2 X-Coordinate of bottom right corner
    @param vY2 Y-Coordinate of bottom right corner
    */
    virtual void FilledRect(const eC_Value &vX1, const eC_Value &vY1, const eC_Value &vX2, const eC_Value &vY2);

    /** Draws an ellipse which is complete inside the rectangle described through
    the rectangle the top left edge at (vX1, vY1) and bottom right edge at (vX2, vY2).
    The ellipse will be filled out if the last parameter is true
    @param vX1 X-Coordinate of bounding rectangle's top left corner
    @param vY1 Y-Coordinate of bounding rectangle's top left corner
    @param vX2 X-Coordinate of bounding rectangle's bottom right corner
    @param vY2 Y-Coordinate of bounding rectangle's bottom right corner
    @param bFilled Specifies whether the ellipse will be drawn outlined (false) or filled (true)
    */
    virtual void Ellipse(const eC_Value &vX1, const eC_Value &vY1, const eC_Value &vX2, const eC_Value &vY2, const eC_Bool &bFilled);

    // ---------------------------------------------------------------
    // Clipping and invalidated rectangles
    // ---------------------------------------------------------------

    /** Sets a clipping rectangle.
        All drawing operations will be limited to the region within the clipping rectangle. This is very
        useful for advanded features such as soft-scrolling. Note that the clipping rectangle is not
        allowed to exceed the invalidated rectangle, since this would lead to visual artifacts.
        @param rkNewClipRect A normalised rectangle that includes all pixels that could be drawn
               by the functions of the graphics wrapper. The rectangle includes the
               border. All pixels outside will be left untouched.
    */
    virtual void SetCliprect(const CGUIRect &rkNewClipRect);

    virtual eC_Bool ImageExists(const ImageResource_t eID) const
    {
        eC_UInt uiCurImage = MAP_IMGID_TO_ARRAY(eID);
        return m_apTextures[uiCurImage] != NULL && (m_apTextures[uiCurImage]->GetData() != NULL || m_apTextures[uiCurImage]->GetTextureID() != 0);
    }

    /** Gets pointer to an image data object.
    @param eImageID Image data object to be returned.
    @return Pointer to image data object.
    */
    CGUIImageDataGL* GetImageData(const ImageResource_t &eImageID);

    /** Sets the default 2D view used by the gfx wrapper for 2D drawing operations.
        This method uses the dimensions returned by GetVirtualScreenWidth() and GetVirtualScreenHeight().
    */
    void Set2DView();

    /** Sets the default 2D view used by the gfx wrapper for 2D drawing operations.
        @param uiWidth Width of Viewport
        @param uiHeight Height of Viewport
    */
    void Set2DView( const eC_UInt uiWidth, const eC_UInt uiHeight);

    /** Checks for occurency of OpenGL Erros.
        All OpenGL errors (error code and text) since the last call of this function (or glGetError) are log in the Guiliani Log.
        @param kCallingMethod String which will be added to the Log entry.
        @return True if successful, False otherwise
    */
    static eC_Bool CheckGLError(const eC_String &kCallingMethod);

    virtual eC_Bool DrawToBitmap(GUIBitmapPlaneID_t uiBitmapID);

    virtual eC_Bool DrawToScreen();

    /** Returns the TextureID and generates the texture if needed
        @param eImageID The image ID
        @return The texture ID
    */
    const eC_UInt GetTextureID(const ImageResource_t &eImageID);

    /**
     * Applies a perspective projection matrix to current GL_PROJECTION. Old
     * projection matrix will be overwritten. Projection matrices are used
     * to project 3D space coordinates to a 2D screen.
     *
     * OpenGL and OpenGL-ES provides projection matrix creation for parallel
     * projection mode (glOrtho), but for perspective projection invokation
     * of the GLU library is needed. To avoid this dependency, this
     * implementation provides this.
     *
     * The matrix is calculated the following way:
     *
     *     (1/w) ( 0 ) (       0       ) (       0       )
     * M = ( 0 ) (1/h) (       0       ) (       0       )
     *     ( 0 ) ( 0 ) ( -(near+far)/d ) ( -2*near*far/d )
     *     ( 0 ) ( 0 ) (      -1       ) (       0       )
     *
     * where:
     *
     * h = tan(fovy/2)
     * w = aspectRatio * h
     * d = far - near
     *
     * @param vFovy Field of view in the vertical direction in DEG [0; 90].
     * @param vAspect Aspect ratio of the viewport, which is width divided by
     *  height.
     * @param vNear Distance to front clipping plane.
     * @param vFar Distance to back clipping plane.
     */
    void SetPerspectiveProjection(const eC_Value& vFovy,
        const eC_Value& vAspect,
        const eC_Value& vNear,
        const eC_Value& vFar);

    ///Helper for initialization
    virtual eC_Bool InitView();

    /** Returns the currently used vertex buffer object.
        @return Vertex buffer object.
    */
    inline eC_UInt GetVBO() const {return m_uiVBO;}

    /** Returns the currently used UV vertex buffer object.
        @return UV vertex buffer object.
    */
    inline eC_UInt GetUVVBO() const {return m_uiUVVBO;}

#if defined GUILIANI_USE_OGL
    #if !defined SYSTEM_NAME_DARWIN
        /** Function pointers for Vertex Buffer Objects. */
        static PFNGLBINDBUFFERPROC              glBindBuffer_Guiliani;
        static PFNGLBUFFERDATAPROC              glBufferData_Guiliani;
        static PFNGLGENBUFFERSPROC              glGenBuffers_Guiliani;
        static PFNGLDELETEBUFFERSPROC           glDeleteBuffers_Guiliani;

        /** Function pointers for Frame Buffer Objects. */
        static PFNGLGENFRAMEBUFFERSPROC         glGenFramebuffers_Guiliani;
        static PFNGLBINDFRAMEBUFFERPROC         glBindFramebuffer_Guiliani;
        static PFNGLFRAMEBUFFERTEXTURE2DPROC    glFramebufferTexture2D_Guiliani;
        static PFNGLCHECKFRAMEBUFFERSTATUSPROC  glCheckFramebufferStatus_Guiliani;
        static PFNGLDELETEFRAMEBUFFERSPROC      glDeleteFramebuffers_Guiliani;
    #endif
#endif

#if defined GUILIANI_USE_OGLES2
    // Initializes default shader objects for OpenGL(ES)
    void InitShaders();

    /** Matrix multiplication.
        The matrix given in avMatrix will be multiplied with the matrix set by SetMatrix(), which will also
        be used to store the result of the multiplication.  ActiveMatrix = avMatrix * ActiveMatrix
        @param avMatrix Matrix to multiply the current matrix with.
    */
    void MultMatrix(const Matrix3D_t& avMatrix);

    ///Replacement for glOrtho in OpenGL ES 2.x
    void SetOrthoProjection(const eC_Value& vLeft, const eC_Value& vRight,
        const eC_Value& vBottom, const eC_Value& vTop,
        const eC_Value& vNear, const eC_Value& vFar);

    ///Replacement for glFrustum in OpenGL ES 2.x
    void SetFrustumProjection(const eC_Value& vLeft, const eC_Value& vRight,
        const eC_Value& vBottom, const eC_Value& vTop,
        const eC_Value& vNear, const eC_Value& vFar);

    ///Replacement for glRotate in OpenGL ES 2.x
    void SetRotation(const eC_Value& vDegrees, eC_Value vX, eC_Value vY, eC_Value vZ);

    ///Replacement for glTranslate in OpenGL ES 2.x
    void SetTranslation(const eC_Value& vXTranslate, const eC_Value& vYTranslate, const eC_Value& vZTranslate);

    ///Replacement for glScale in OpenGL ES 2.x
    void SetScaling(const eC_Value& vXScale, const eC_Value vYScale, const eC_Value vZScale);

    ///Calls SetScaling with the same parameter for the whole matrix
    void SetUniformScaling(const eC_Value& vScale);

    /** Returns the program object to get access to the shader program.
        @return Program object.
    */
    inline eC_UInt GetProgramObject() const {return m_uiProgramObject;}

    /** Replaces the currently used matrix with ne new one.
        @param avNewMatrix New matrix to set.
    */
    void SetMatrix(const Matrix3D_t& avNewMatrix);

    /** Returns the currently active matrix (or identity matrix in default switch case).
        @return Active matrix.
    */
    const Matrix3D_t& GetMatrix() const;

    /** Pushes the currently active Matrix onto the stack. */
    void PushMatrix();

    /** Pops the last used active Matrix from stack. */
    void PopMatrix();

    /** Available matrix types */
    enum MatrixType_t
    {
        MT_PROJECTION,
        MT_MODELVIEW
    };

    /** Sets the matrix mode
        @param eNewActiveMatrix Matrix to set active.
    */
    inline void SetMatrixMode(const MatrixType_t& eNewActiveMatrix) {m_eActiveMatrix = eNewActiveMatrix;}

    /** Sets identity matrix to currently active one. */
    void LoadIdentity();
#endif //#if defined GUILIANI_USE_OGLES2

protected:
    // ---------------------------------------------------------------
    // Construction / destruction
    // ---------------------------------------------------------------

    /// Destructor
    virtual ~CGfxWrapGL(void);

    /// Constructor
    CGfxWrapGL(void);

    virtual CGUIBitmapPlane* CreateGfxDependentBitmap(
        GUIBitmapPlaneID_t uiNewBitmapID,
        const CGUIObject& rkParentObject,
        const eC_Value &vWidth, const eC_Value &vHeight);

    /** Restores the Backbuffer of the display after it has been used for texture rendering
        Always begins in LOWER LEFT of screen!
        @param vWidth of the area to restore.
        @param vHeight of the area to restore.
    */
    virtual void RestoreBackBuffer(const eC_Value vWidth, const eC_Value vHeight);

    /** Loads an image.
    This loads an image from the given path and associates it with an Image-Identifier.
    @param psPath Complete path and filename of the image to be loaded.
    @param eID ID by which the image will later be accessible.
    @return True if the image was successfully loaded, False otherwise
    */
    virtual void LoadImgImpl(const eC_String& psPath, const ImageResource_t &eID);

    /** Unloads an image.
    Unloads the image associated with the supplied Image-identifier and frees the memory formerly
    occupied by this image.
    @param eID Identifier of the image to be unloaded.
    */
    virtual void UnloadImg(const ImageResource_t &eID);

    eC_Bool SetNOFImagesImpl(const eC_UInt uiNOFImages);

    // ---------------------------------------------------------------
    // Image-access
    // ---------------------------------------------------------------

    virtual void BlitImgExtImpl(
        const ImageResource_t &eID,
        const eC_UInt &uiSrcX, const eC_UInt &uiSrcY, const eC_UInt &uiSrcWidth, const eC_UInt &uiSrcHeight,
        const eC_Value &vDestX, const eC_Value &vDestY, const eC_Value &vDestWidth, const eC_Value &vDestHeight,
        const eC_UByte &ubAlpha = 255,
        const eC_Value &vAngle = eC_FromFloat(0),
        const eC_Value &vRotCenterX = eC_FromFloat(0.5), const eC_Value &vRotCenterY = eC_FromFloat(0.5) );

    /** Gets the width of an image.
    @param eID Identifier of the image.
    @return The width of the image in pixels.
    */
    virtual eC_UInt GetImgWidthImpl(const ImageResource_t &eID) const;

    /** Gets the height of an image.
    @param eID Identifier of the image.
    @return The height of the image in pixels.
    */
    virtual eC_UInt GetImgHeightImpl (const ImageResource_t &eID) const;

    /** Returns the amount of memory taken up by an image.
        @param eID ID of the image to be measured.
        @return Size of the image in byte.
      */
    virtual eC_UInt GetImageSize(const ImageResource_t& eID) const;

    /** Draws a rectangle
        @param vX1 X position of corner 1
        @param vY1 Y position of corner 1
        @param vX2 X position of corner 2
        @param vY2 Y position of corner 2
        @param bFilled Whether to draw a filled rectangle
      */
    void DrawRect(const eC_Value &vX1, const eC_Value &vY1, const eC_Value &vX2, const eC_Value &vY2, const eC_Bool bFilled);

    /** Generates a texture of matching (power of 2) size for the given image ID and copies the image into it
    @param eImageID ImageID for which the texture should be created.
    */
    void GenerateTexture(const ImageResource_t &eImageID);

    /** Accessing the Z-Correction for current render target.
      * Render targets may be the screen or any BitmapPlane.
      *  @return returns the current z-correction for the active render target.
      */
    eC_Value GetZCorrection();

    /** Increases the Z-Correction for the current render target.
      * The render target may be the screen or any BitmapPlane.
      */
    void IncreaseZCorrection();

    /** This SetCliprect version is used for an anti-clockwise rotation of the GUI. E.g. in GfxWrapEAGL implementation, if the iOS device is rotated.
        @param rkNewClipRect A normalised rectangle that includes all pixels that could be drawn
               by the functions of the graphics wrapper. The rectangle includes the
               border. All pixels outside will be left untouched.
        @param bRotate Set this true to set glScissor rotated anti-clockwise.
    */
    void SetCliprect(const CGUIRect &rkNewClipRect, const eC_Bool &bRotate);

    eC_UInt m_uiScreenWidth;    ///< Screen width
    eC_UInt m_uiScreenHeight;   ///< Screen height

    eC_Value m_vRed;            ///< Color values for red channel as value in the float range between 0 and 1
    eC_Value m_vGreen;          ///< Color values for green channel as value in the float range between 0 and 1
    eC_Value m_vBlue;           ///< Color values for blue channel as value in the float range between 0 and 1
    eC_Value m_vAlpha;          ///< Color values for alpha channel as value in the float range between 0 and 1

    eC_TArray<CGUIImageDataGL*> m_apTextures; ///< Array holding the textures

    /// Z-Correction of the ScreenBuffer
    eC_Value m_vDepthCorrection;

    /// transition matrix
    Matrix3D_t m_avTransMat;

    /// fovy
    eC_Value m_vFovy;

    /// aspect
    eC_Value m_vAspect;

    /// near
    eC_Value m_vNear;

    /// far
    eC_Value m_vFar;

    /// vertex buffer object
    eC_UInt m_uiVBO;

    /// UV vertex buffer object
    eC_UInt m_uiUVVBO;

#if defined GUILIANI_USE_OGLES2
    /// projection matrix
    Matrix3D_t m_avProjectionMatrix;

    /// modelview matrix
    Matrix3D_t m_avModelviewMatrix;

    /// modelview matrix stack
    eC_TListDoubleLinked<Matrix3D_t*> m_kModelviewMatrixStack;

    /// projection matrix stack
    eC_TListDoubleLinked<Matrix3D_t*> m_kProjectionMatrixStack;

    /// The vertex shader OpenGL handle
    eC_UInt m_uiVertexShaderHandle;
    /// fragment shader OpenGL handle
    eC_UInt m_uiFragmentShaderHandle;

    /// program object.
    eC_UInt m_uiProgramObject;

    ///active matrix
    MatrixType_t m_eActiveMatrix;
#endif //#if defined GUILIANI_USE_OGLES2

private:
    /** Method to initialize the graphic library
    @return returns True if the initialization succeded, False otherwise*/
    eC_Bool Init(void);

    /** Method to give the graphic library the opportunity to clean up
    @return returns True if the deinitialization succeded, False otherwise*/
    eC_Bool DeInit(void);
};

#endif //GFXWRAPGL__H_
