/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GFXWRAPRGA_H_
#define GFXWRAPRGA_H_

#include "GfxWrap.h"
#include "RGA.h"
#include "RGAImageData.h"
#include "RGA_Platform.h"
#include "GUITrace.h"

extern "C"
{
    extern void GrpDrv_SetFrameBuffer(void*);
}

/// Constants defined as per RGA Document
#define MAX_FRAME_BUFFER_WIDTH  1280
#define MAX_FRAME_BUFFER_HEIGHT 1024
#define MAX_IMAGE_WIDTH         2048
#define MAX_IMAGE_HEIGHT        2048

///  This is the RGA implementation of the graphics wrapper.

/** This graphics wrapper uses methods of Renesas Graphics Architecture (RGA) library for blitting.
  * The library allows drawing using hardware acceleration.

    For additional details see description of base class CGfxWrap.
*/
/**
<h3>Platform Specific Limitations </h3>
<p>
RGA can only draw a horizontal or vertical line. Also, it has no API to draw an ellipse. (Class CGfxWrapRGA)
*/

// The following limitations will be included in the section "Platform Specific Limitations" of Guiliani documentation.
// @guiliani_doxygen limitations CGfxWrapRGA
/*
 RGA can only draw a horizontal or vertical line. Also, it has no API to draw an ellipse. (Class CGfxWrapRGA)
 <p>
 When drawing characters of raster font, specify the image of A8, A4, A1 (Alpha Only) format to image argument.
 But, the width of the image must be same as width of drawing target frame buffer for RZ/A1H.
 (Class CFntWrapFreeTypeRGA)
 <p>
 Rotations around x-axis and y-axis are not supported. (Class CGUIBitmapPlane_RGA)
 <p>
*/
// @endguiliani_doxygen

/**
   See @ref subsec_limitations_CGfxWrapRGA "CGfxWrapRGA Platform Specific Limitations"
*/

class CGfxWrapRGA:
    public CGfxWrap
{
public:
    /**
       Singleton for creating a GfxWrapRGA object and change the pointer in
       base class to this graphics wrapper.
       @param uiScreenWidth The RGA screen width.
       @param uiScreenHeight The RGA screen height.
       @param ePixelFormat pixel format of the frame buffer
       @param uiBufferCount number of frame buffers to use
       @return Indicate whether by this call, an instance is created.
               True if the instance is created, False otherwise.
    */
    static eC_Bool CreateInstance(const eC_UInt &uiScreenWidth = RGA_DEFAULT_SCREEN_WIDTH,
                                  const eC_UInt &uiScreenHeight = RGA_DEFAULT_SCREEN_HEIGHT,
                                  pixel_format_t ePixelFormat = RGA_DEFAULT_BITMAP_TYPE,
                                  eC_UInt uiBufferCount = RGA_DEFAULT_BUFFERS);

    /** Method to show the drawn contents in frame buffer on the screen.
        @param eRefreshCallType The call type of this refresh
        @return True if the refresh was performed, False otherwise
    */
     eC_Bool Refresh(const RefreshCall_t eRefreshCallType);

    /** Allocates storage for frame buffer and drawing context. It initializes the frame buffer
        to specified uiWidth and uiHeight.
        @param uiWidth Width of virtual screen.
        @param uiHeight Height of virtual screen.
    */
     virtual void CreateScreen(const eC_UInt &uiWidth = RGA_DEFAULT_SCREEN_WIDTH,
                               const eC_UInt &uiHeight = RGA_DEFAULT_SCREEN_HEIGHT)
     {
         CreateScreen(uiWidth, uiHeight, RGA_DEFAULT_BITMAP_TYPE, RGA_DEFAULT_BUFFERS);
     }

     /// internally called by method CreateScreen of CGfxWrapRGA, which is inherited from base class
     virtual void CreateScreen(const eC_UInt &uiWidth    = RGA_DEFAULT_SCREEN_WIDTH,
                               const eC_UInt &uiHeight   = RGA_DEFAULT_SCREEN_HEIGHT,
                               pixel_format_t pixel_format_t = RGA_DEFAULT_BITMAP_TYPE,
                               eC_UInt uiBufferCount = RGA_DEFAULT_BUFFERS);

     /// Method to delete the drawing context of RGA, which was created in CreateScreen method.
     virtual void DestroyScreen();

     /// Method to get the current drawing context of RGA.
     inline Canvas2D_ContextClass* GetContext(void) {return &m_kCanvas2d;}
     
     /// Method to get the drawing frame buffer.
     inline frame_buffer_t* GetFrameBuffer(void) {return &m_kFrameBuffer;}

    /** Draws a line from (vAbsX1, vAbsY1) to (vAbsX2, vAbsY2). 
        @note It can only draw horizontal or vertical line due to limitations of RGA library.
        @param vAbsX1 X-coordinate of starting point
        @param vAbsY1 Y-Coordinate of starting point
        @param vAbsX2 X-coordinate of ending point
        @param vAbsY2 Y-Coordinate of ending point
    */
     void Line(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2);

    /** Draws a rectangle with the top left edge at (vAbsX1, vAbsY1) and
        bottom right edge at (vAbsX2, vAbsY2).
        @param vAbsX1 X-coordinate of top left corner
        @param vAbsY1 Y-Coordinate of top left corner
        @param vAbsX2 X-coordinate of bottom right corner
        @param vAbsY2 Y-Coordinate of bottom right corner
    */
     void Rect(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2);

    /** Draws a filled rectangle with the top left edge at (vAbsX1, vAbsY1)
        and bottom right edge at (vAbsX2, vAbsY2).
        @param vAbsX1 X-coordinate of top left corner
        @param vAbsY1 Y-Coordinate of top left corner
        @param vAbsX2 X-coordinate of bottom right corner
        @param vAbsY2 Y-Coordinate of bottom right corner
    */
     void FilledRect(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2);

    /** Draws an ellipse which is complete inside the rectangle described
        through the rectangle the top left edge at (vAbsX1, vAbsY1)
        and bottom right edge at (vAbsX2, vAbsY2).
        The ellipse will be filled out if the last parameter is true
        @param vAbsX1 X-coordinate of bounding rectangle's top left corner
        @param vAbsY1 Y-Coordinate of bounding rectangle's top left corner
        @param vAbsX2 X-coordinate of bounding rectangle's bottom right corner
        @param vAbsY2 Y-Coordinate of bounding rectangle's bottom right corner
        @param bFilled Specifies whether the ellipse will be drawn outlined (false) or filled (true)
    */
     void Ellipse(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2,
        const eC_Bool &bFilled);

    /** Sets the foreground color. This MUST be implemented by all graphics wrappers.
        For detailed explanations, please refer to CGfxWrap::SetForegroundColor
        @param ubRed Red intensity within a range of 0 - 255
        @param ubGreen Green intensity within a range of 0 - 255
        @param ubBlue Blue intensity within a range of 0 - 255
        @param ubAlpha Alpha intensity within a range of 0 (transparent) - 255 (opaque)
    */
     void SetForegroundColorImpl(
        const eC_UByte &ubRed,
        const eC_UByte &ubGreen,
        const eC_UByte &ubBlue,
        const eC_UByte &ubAlpha = 255);

    /** Returns whether the image with the given ID is currently loaded within the Graphics Wrapper.
        @param eID The ID to check
        @return True if the image with the given ID is currently loaded, False otherwise.
    */
    virtual eC_Bool ImageExists(const ImageResource_t eID) const;

    /** Returns the screen width.
        Will always return the physical screen width with which the CGfxWrapRGA was originally initialized.
        Regardless of whether the virtual screen size has been modified e.g., while drawing into a CGUIBitmapPlane.
        @return Physical screen width
    */
    virtual eC_UInt GetPhysicalScreenWidth();
    
    /** Returns the screen height.
        Will always return the physical screen height with which the CGfxWrapRGA was originally initialized.
        Regardless of whether the virtual screen size has been modified e.g., while drawing into a CGUIBitmapPlane.
        @return Physical screen height
    */
    virtual eC_UInt GetPhysicalScreenHeight();
    
    /** Returns the screen width in pixels, may vary if DrawToBitmap is active.
        It will then return the active BitmapPlane's width instead.
        @see GetPhysicalScreenWidth()
        @return eC_UInt The screen width
    */
    virtual eC_UInt GetVirtualScreenWidth();
    
    /** Returns the screen height in pixels, may vary if DrawToBitmap is active.
        It will then return the active BitmapPlane's height instead.
        @see GetPhysicalScreenHeight()
        @return eC_UInt The screen height
    */
    virtual eC_UInt GetVirtualScreenHeight();
    
    /** Activates the Screen as an active RenderTarget again
        @return True if the changing was successful, otherwise False
    */
    virtual eC_Bool DrawToScreen();

    /** Sets a clipping rectangle. All drawing operations will be limited to the region within the clipping rectangle.
        Note that the clipping rectangle is not allowed to exceed the invalidated rectangle, since this would lead to
        visual artifacts.
        @param NewClipRect A normalised rectangle that includes all pixels that could be drawn
            by the functions of the graphics wrapper. The rectangle includes the
            border. All pixels outside will be left untouched.
    */
    virtual void SetCliprect(const CGUIRect &NewClipRect);

protected:

    /** Sets the total number of images. This is called internally by the ResourceManager when dynamically
        allocated image resources require the total number of images to be increased.
        @param uiNOFImages The number of images
        @return True if internal storage has been successfully adapted to support the requested
                number of images. False otherwise.
    */
    virtual eC_Bool SetNOFImagesImpl(const eC_UInt uiNOFImages);

    /** Loads an image file.
        This loads an image from the given path and associates it with an Image-Identifier.
        Internally called by ResourceManager.
        @param kPath Complete path and filename of the image to be loaded.
        @param eID ID by which the image will later be accessible.
        @return True if the image was successfully loaded, False otherwise
    */
    virtual void LoadImgImpl(const eC_String& kPath, const ImageResource_t &eID);

    /** Unloads an image.
        Unloads the image associated with the supplied Image-identifier and frees the memory formerly
        occupied by this image.
        Internally called by ResourceManager.
        @param eID Identifier of the image to be unloaded.
    */
    virtual void UnloadImg(const ImageResource_t &eID);

    /** Blits an image. This MUST be implemented by all graphics wrappers. It is ensured that the supplied image ID is
        in the valid range and is not DUMMY_IMAGE. For detailed explanations, please refer to CGfxWrap::BlitImgExtImpl
        @param eID          Identifier of the image to be blitted.
        @param uiAbsSrcX    X coordinate of upper left corner of the relevant area within the source image
        @param uiAbsSrcY    Y coordinate of upper left corner of the relevant area within the source image
        @param uiSrcWidth   Width of the of the relevant area within the source image
        @param uiSrcHeight  Height of the of the relevant area within the source image
        @param vAbsDestX    Destination X position in absolute coordinates
        @param vAbsDestY    Destination Y position in absolute coordinates
        @param vDestWidth   Destination width
        @param vDestHeight  Destination height
        @param ubAlpha      Degree of transparency. 0=transparent 255=opaque  Default is opaque.
        @param vAngle       Angle in degrees by which the image will be rotated
        @param vRotCenterX  Rotation center X. In range of 0 to 1 (like U/V coordinates)
        @param vRotCenterY  Rotation center Y. In range of 0 to 1 (like U/V coordinates)
    */
    virtual void BlitImgExtImpl(
        const ImageResource_t &eID,
        const eC_UInt &uiAbsSrcX, const eC_UInt &uiAbsSrcY,
        const eC_UInt &uiSrcWidth, const eC_UInt &uiSrcHeight,
        const eC_Value &vAbsDestX, const eC_Value &vAbsDestY,
        const eC_Value &vDestWidth, const eC_Value &vDestHeight,
        const eC_UByte &ubAlpha = 255,
        const eC_Value &vAngle = eC_FromFloat(0),
        const eC_Value &vRotCenterX = eC_FromFloat(0.5), const eC_Value &vRotCenterY = eC_FromFloat(0.5));

    /** Returns the width of an image. This MUST be implemented by all graphics wrappers.
        @param eID The ID of the image whose width should be returned.
                   It is ensured that eID is always in the valid range and not
                   DUMMY_IMAGE.
        @return The width of the image in pixels.
    */
    virtual eC_UInt GetImgWidthImpl(const ImageResource_t &eID) const;

    /** Returns the height of an image. This MUST be implemented by all graphics wrappers.
        @param eID The ID of the image whose width should be returned.
                   It is ensured that eID is always in the valid range and not
                   DUMMY_IMAGE.
        @return The width of the image in pixels.
    */
    virtual eC_UInt GetImgHeightImpl(const ImageResource_t &eID) const;

    /** Computes the size of a loaded image's raw pixel data (e.g. RGBA or RGB565, depending on platform)
        in memory. Needed for the Image Cache.
        @param eID The image's ID.
        @return the size of the image in bytes
    */
    virtual eC_UInt GetImageSize(const ImageResource_t &eID) const;

    Canvas2D_ContextClass m_kCanvas2d; ///< Graphics drawing context of RGA
    frame_buffer_t m_kFrameBuffer; ///< Drawing target frame buffer

    eC_UInt m_uiWidth;  ///< Screen width
    eC_UInt m_uiHeight; ///< Screen height

    /// Array of pointers to RGA bitmaps
    eC_TArray<CRGAImageData*> m_pkImage;

    eC_Bool m_bClipped; ///< Status to show if any rectangle is already clipped

    #ifdef RGA_MEASURE_PERFORMANCE
        r_ospl_ftimer_spec_t  kTimer; // For measuring performance
    #endif

    /** Constructor
    */
    CGfxWrapRGA(void);
    
    /** Destructor
    */
    ~CGfxWrapRGA(void);

private:
    /** Method which will be called to instantiate a new Bitmap-Object.
        @param uiNewBitmapID ID of the new BitmapPlane
        @param rkParentObject reference to the GUIObject which will be the parent of the bitmap
        @param vWidth width of the new bitmap
        @param vHeight height of the new bitmap
        @return pointer to the new created BitmapObject
    */
    virtual CGUIBitmapPlane* CreateGfxDependentBitmap(
        GUIBitmapPlaneID_t uiNewBitmapID,
        const CGUIObject& rkParentObject,
        const eC_Value &vWidth,
        const eC_Value &vHeight);

    /** Copy-constructor intentionally not implemented.
        @param kSource Source object to be copied.
    */
    CGfxWrapRGA(const CGfxWrapRGA& kSource);

    /** Operator= method intentionally not implemented.
        @param kSource Source object to be copied.
        @return This instance.
    */
    CGfxWrapRGA& operator=(const CGfxWrapRGA& kSource);

    void SwapBuffer(); // To show the drawn content in the draw frame buffer

    eC_Bool Init(void); // To initialize the array of pointers of images
    eC_Bool DeInit(void); // To deinitialize the array of pointers of images
};
#endif /* GFXWRAPRGA_H_ */
