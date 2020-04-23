#ifndef GFXWRAPVG_H
#define GFXWRAPVG_H

#include "openvg.h"
#include "vgu.h"//utility-lib

#include <EGL/egl.h>
#include <vector>
#include "SVGShape.h"

#include "GfxWrap.h"
#include "GUIImageData.h"
#include "GUIImageLoader.h"

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
/** This class encapsulates VGHandles for drawing images with OpenVG.
    SVG-Files and normal bitmaps can be drawn.
*/
class CGUIImageDataVG : public CGUIImageData
{
public:
    /** Constructor */
    CGUIImageDataVG();
    /** Destructor */
    ~CGUIImageDataVG();
    /** sets VGImage-handle of this bitmap graphic.
    @param uiImgHandle handle to use.
    */
    inline void SetImgHandle(const VGImage uiImgHandle)   {m_uiImageHandle = uiImgHandle;}
    /** Get the currrent VGImage-handle for bitmap graphics.
        @return current VGImage-handle.
    */
    inline VGImage GetImgHandle()                       const {return m_uiImageHandle;}

    ///VGHandles for drawing SVG-images
    std::vector<VGPath>  m_auiSvgPathHandles; ///< Path-handles
    std::vector<VGPaint> m_auiSvgFillPaintHandles; ///< handles for filling paint-operations
    std::vector<VGPaint> m_auiSvgStrokePaintHandles; ///< handles for stroking paint-operations

private:
    /** Copy-constructor
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGUIImageDataVG(const CGUIImageDataVG& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGUIImageData& operator=(const CGUIImageDataVG& kSource);

    VGImage         m_uiImageHandle;
};

/**
specialized version of the CGfXWrapGL to handle OpenVG-acceleration
*/
class CGfxWrapVG : public CGfxWrap
{
    public:

   /** Singleton for creating a GfxWrapVG object and change the pointer in
        base class to this graphic wrap.
        @return True if an instance was created by this call, otherwise False.
        @param[in] hWindowType which will be used for drawing
        @param[in] hDisplayType which will be used for drawing
    */
    static eC_Bool CreateInstance(const NativeWindowType& hWindowType = 0,
                                  const NativeDisplayType& hDisplayType = EGL_DEFAULT_DISPLAY);

    virtual void CreateScreen(const eC_UInt &uiWidth, const eC_UInt &uiHeight);
    /// Helper used to create the screen itself
    virtual void CreateScreen();

    virtual void SetCliprect(const CGUIRect& rkNewClipRect);

protected:
    CGfxWrapVG();
    /** Constructor
        @param[in] hWindowType which will be used for drawing
        @param[in] hDisplayType which will be used for drawing
    */
    CGfxWrapVG(const NativeWindowType& hWindowType,
                const NativeDisplayType& hDisplayType);

    /** Destructor */
    virtual ~CGfxWrapVG();

    virtual eC_Bool Refresh(const RefreshCall_t eRefreshCallType);

    virtual eC_UInt GetVirtualScreenWidth();
    virtual eC_UInt GetVirtualScreenHeight();

    virtual eC_UInt GetPhysicalScreenWidth()    {return m_uiScreenWidth;}
    virtual eC_UInt GetPhysicalScreenHeight()   {return m_uiScreenHeight;}

    virtual void Line(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2);

    virtual void Rect(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2);

    virtual void FilledRect(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2);

    virtual void Ellipse(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2,
        const eC_Bool &bFilled);

    virtual eC_Value SetLineWidth(const eC_Value& vWidth);

    virtual eC_Bool ImageExists(ImageResource_t eID) const;
    virtual eC_Bool SetNOFImagesImpl(const eC_UInt uiNOFImages);
    virtual void LoadImgImpl(const eC_String& kPath, const ImageResource_t& eImageID);
    virtual void UnloadImg(const ImageResource_t &eImageID);
    virtual eC_UInt GetImageSize(const ImageResource_t& eImageID) const;
    virtual eC_UInt GetImgWidthImpl(const ImageResource_t& eImageID) const;
    virtual eC_UInt GetImgHeightImpl(const ImageResource_t& eImageID) const;

    virtual void BlitImgExtImpl(const ImageResource_t&,
        const eC_UInt &uiAbsSrcX, const eC_UInt &uiAbsSrcY,
        const eC_UInt &uiSrcWidth, const eC_UInt &uiSrcHeight,
        const eC_Value &vAbsDestX, const eC_Value &vAbsDestY,
        const eC_Value &vDestWidth, const eC_Value &vDestHeight,
        const eC_UByte &ubAlpha = 255,
        const eC_Value &vAngle = eC_FromFloat(0),
        const eC_Value &vRotCenterX = eC_FromFloat(0.5), const eC_Value &vRotCenterY = eC_FromFloat(0.5) );

    virtual void SetForegroundColorImpl(
        const eC_UByte &ubRed,
        const eC_UByte &ubGreen,
        const eC_UByte &ubBlue,
        const eC_UByte &ubAlpha = 255);

    /** check for OpenVG error and output message with specified method-name
    @param kCallingMethod name of the calling method which is used for error-message
    @return true if error has occurred, else false
    */
    eC_Bool CheckVGError(const eC_String &kCallingMethod);

    /** check for EGL error and output message with specified method-name
    @param kCallingMethod name of the calling method which is used for error-message
    @return true if error has occurred, else false
    */
    eC_Bool CheckEGLError(const eC_String &kCallingMethod);

    /** Translate SVG-Data from GUIImageData to a OpenVG renderable form.
        @param uiCurImage ID by which the image will later be accessible.
    */
    void CreateSVGImage(const eC_UInt& uiCurImage);

    /** Create an bitmap image.
        @param uiCurImage ID by which the image will later be accessible.
    */
    void CreateBitmapImage(const eC_UInt& uiCurImage);

    /** Set OpenVG params and draw calls
        @param uiCurImage ID by which the image will later be accessible.
    */
    void RenderSVG(const eC_UInt& uiCurImage);

    /** Constructs a geometry object.
        @param kShape with information about SVG-form
        @param uiPathHandle VGPath handele to append points
    */
    void GetPathFromShape(const CSVGShape& kShape, VGPath& uiPathHandle);

    /** Create a VGPaint-object.
        @param kShape with information about SVG-form color
        @param uiPaintHandle VGHandle for color and gradients
        @param bIsStroke determines if paint is to stoke or fill
    */
    void GetPaintFromShape(CSVGShape& kShape, VGPaint& uiPaintHandle, eC_Bool bIsStroke);

    /** Construct a linear or radial gradient from CGradient.
        gets called in void GetPaintFromShape(CSVGShape& shape, VGPaint& paint, eC_Bool bIsStroke)
        @param kGradient contains infos from SVG, like color-ramp
        @param uiPaintHandle VGPaint-handle
    */
    void ConstructGradient(CGradient& kGradient, VGPaint& uiPaintHandle);

    /** Method to initialize the graphic library.
        Connects rendering API(OpenVG) with underlying native window system with EGL.
        create context -> attached to surface -> bound to a thread(outside OpenVG)
        handles: paths, paints always active for one current context(except shared context)
        set up render quality, scissoring etc.
        @return returns True if the initialisation succeeded, False otherwise.
    */
    virtual eC_Bool Init(void);

    /** Method to give the graphic library the opportunity to clean up.
        Connects rendering API(OpenVG) with underlying native window system with EGL.
        @return returns True if the de-initialisation succeeded, False otherwise.
    */
    virtual eC_Bool DeInit(void);

    /// the display handle
    NativeDisplayType m_hDisplayType;

    /// the window handle
    NativeWindowType m_hWindowType;

    /// EGL-Window-handling-magic
    EGLDisplay m_display; ///< display for EGL
    EGLSurface m_windowSurface; ///< window-surface for EGL

    EGLConfig m_screenConfig; ///< config for buffer
    EGLContext m_screenContext; ///< context for buffer

    ///for double-buffering
    EGLConfig m_offScreenConfig; ///< config of offscreen-buffer
    EGLContext m_offScreenContext; ///< context of offscreen-buffer

    eC_TArray<CGUIImageDataVG*> m_apImages; ///< array of VGImages or SVGImages
    
private:
    eC_UInt m_uiScreenWidth;    ///< Screen width
    eC_UInt m_uiScreenHeight;   ///< Screen height

    eC_Value m_vRed;        ///< Color values for red channel as value in the float range between 0 and 1
    eC_Value m_vGreen;      ///< Color values for green channel as value in the float range between 0 and 1
    eC_Value m_vBlue;       ///< Color values for blue channel as value in the float range between 0 and 1
    eC_Value m_vAlpha;      ///< Color values for alpha channel as value in the float range between 0 and 1

    VGPaint m_strokePaint;  ///VGPaint for outlined primitives.
    VGPaint m_fillPaint;    ///VGPaint for filled primitives.

    VGPath m_uiPrimStrokeHandle; ///VGHandle for outlined primitives.
    VGPath m_uiPrimFillHandle;  ///VGHandle for filled primitives.
};

#endif // GFXWRAPVG_H
