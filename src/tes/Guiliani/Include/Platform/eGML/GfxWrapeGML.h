/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GFXWRAPEGML__H_
#define GFXWRAPEGML__H_

#include "GfxWrap.h"
#include "eGML_Config.h"
#include "eGML_Platform.h"

class eGML;
class eImg_Bitmap;
class eGML_Screen;
class eGML_Bitmap;


///  This is the eGML implementation of the graphics wrapper.

/** This graphics wrapper uses eGML_Bitmap methods for blitting. The blit flags
    for all operations can be set by calling SetBlitFlags(). They are by default
    initialized to eGML_Bitmap::BLIT_BILINEAR.

    For additional details see description of base class CGfxWrap.
*/
class CGfxWrapeGML :
    public CGfxWrap
{
public:
    /** Singleton for creating a GfxWrapeGML object and change the pointer in
        base class to this graphics wrapper.
        @param uiScreenWidth The eGML screen width.
        @param uiScreenHeight The eGML screen height.
        @param eBitmap bitmap type to create
        @return Indicate whether by this call, an instance is created.
            The instance is created, if true, not created, otherwise.
    */
    static eC_Bool CreateInstance(const eC_UInt &uiScreenWidth = eGML_DEFAULT_SCREEN_WIDTH,
                                  const eC_UInt &uiScreenHeight = eGML_DEFAULT_SCREEN_HEIGHT,
                                  eC_Int eBitmapType = eGML_DEFAULT_BITMAP_TYPE,
                                  eC_UInt uiBufferCount = eGML_DEFAULT_BUFFERS);

    virtual eC_Value SetLineWidth(const eC_Value &vWidth);

    virtual eC_Bool Refresh(const RefreshCall_t eRefreshCallType);

    virtual void CreateScreen(const eC_UInt &uiWidth    = eGML_DEFAULT_SCREEN_WIDTH,
                              const eC_UInt &uiHeight   = eGML_DEFAULT_SCREEN_HEIGHT)
    {
    	CreateScreen(uiWidth, uiHeight,eGML_DEFAULT_BITMAP_TYPE, eGML_DEFAULT_BUFFERS );
    }

    virtual void CreateScreen(const eC_UInt &uiWidth    = eGML_DEFAULT_SCREEN_WIDTH,
                              const eC_UInt &uiHeight   = eGML_DEFAULT_SCREEN_HEIGHT,
                              eC_Int eBitmapType = eGML_DEFAULT_BITMAP_TYPE,
                              eC_UInt uiBufferCount = eGML_DEFAULT_BUFFERS);

    virtual void DestroyScreen();

    ///@return the eGML screen pointer
    inline eGML_Screen *GetScreen(void) {return m_pScreen;}

    ///@return the eGML render bitmap pointer
    inline eGML_Bitmap* GetRenderBitmap() {return m_pRenderBitmap;}
    
    ///@param pRenderBitmap The target bitmap for subsequent render operations
    void SetRenderBitmap(eGML_Bitmap* pRenderBitmap);

    virtual eC_UInt GetVirtualScreenWidth();
    virtual eC_UInt GetVirtualScreenHeight();
    virtual eC_UInt GetPhysicalScreenWidth();
    virtual eC_UInt GetPhysicalScreenHeight();

    virtual void SetForegroundColorImpl(
        const eC_UByte &ubRed,
        const eC_UByte &ubGreen,
        const eC_UByte &ubBlue,
        const eC_UByte &ubAlpha = 255);

    virtual void Line(const eC_Value &vX1, const eC_Value &vY1, const eC_Value &vX2, const eC_Value &vY2);
    virtual void Rect(const eC_Value &vX1, const eC_Value &vY1, const eC_Value &vX2, const eC_Value &vY2);
    virtual void FilledRect(const eC_Value &vX1, const eC_Value &vY1, const eC_Value &vX2, const eC_Value &vY2);
    /** Implementation of the CGfxWrap::Ellipse() interface for eGML.
        Note that eGML does currently only support filled circles (e.g. width and height must be identical), and
        the active foreground color must be fully opaque.
        @param vAbsX1 X-coordinate of bounding rectangle's top left corner
        @param vAbsY1 Y-Coordinate of bounding rectangle's top left corner
        @param vAbsX2 X-coordinate of bounding rectangle's bottom right corner
        @param vAbsY2 Y-Coordinate of bounding rectangle's bottom right corner
        @param bFilled Specifies whether the ellipse will be drawn outlined (false) or filled (true).
    */
    virtual void Ellipse( const eC_Value &vAbsX1, const eC_Value &vAbsY1, const eC_Value &vAbsX2, const eC_Value &vAbsY2, const eC_Bool &bFilled);
    virtual void SetCliprect(const CGUIRect &NewAbsClipRect);

    /** Returns a pointer to the image referenced by the specified ID.
        @param eID of the image to retrieve
        @return eImb_Bitmap pointer to the related bitmap   */
    virtual eImg_Bitmap* GetImage( const ImageResource_t &eID)
    {
        eC_UInt uiCurImage = MAP_IMGID_TO_ARRAY(eID);
        return m_pImage[uiCurImage];
    }

    /** Returns whether an image is present for the given ID.
        @param eID ID for which to check
        @return True if an image is present, False otherwise. */
    virtual eC_Bool ImageExists( const ImageResource_t eID) const;

    /** Enables/disables subpixel rendering.  (Disabled by default!)
        While subpixel rendering is enabled, blitting will be done with subpixel-accuracy (Resulting in a time-consuming RotBlit)
        @param bEnabled true to enable subpixel rendering, false to disable. */
    void SetSubpixelEnabled( const eC_Bool bEnabled) { m_bSubpixelEnabled = bEnabled; }

    /** Sets the eGML blit flags for all blit operations.
        @param uiFlags Combination of eGML blit flags.
        @see GetBlitFlags()
      */
    void SetBlitFlags(eC_Flag uiFlags) { m_uiBlitFlags = uiFlags; }

    /** Returns the currently set eGML blit flags.
        @return The current combination of eGML blit flags.
        @see SetBlitFlags()
      */
    inline eC_Flag GetBlitFlags() const { return m_uiBlitFlags; }

    /** Enables/Disables NEON optimization
    if neon is available this setting is true by default
    @param bNeon true to enable NEON optimization if available, false to disable
    */
    virtual void SetNEONOptimization(const bool bNeon);

    /** Enables/Disables bilinear filtering during blit operation
    if bilinear is available this setting is true by default
    @param bBillinear true to enable bilinear blit, false to disable bilinear filtering and use nearest neighbor texturing
    */
    virtual void SetBilinearBlit(const bool bBillinear);

    virtual eC_Bool DrawToScreen();
protected:
    /** Sets the total number of images. This is called internally by the ResourceManager when dynamically
        allocated image resources require the total number of images to be increased.
        @param uiNOFImages The number of images
        @return True if internal storage has been successfully adapted to support the requested
                number of images. False otherwise.
    */
    virtual eC_Bool SetNOFImagesImpl(const eC_UInt uiNOFImages);

    /// Loads an image file from disk. Internally called by ResourceManager.
    virtual void LoadImgImpl(const eC_String &psPath, const ImageResource_t &eID);

    /// Unloads an image file. Internally called by ResourceManager.
    virtual void UnloadImg(const ImageResource_t &eID);

    virtual void BlitImgExtImpl(const ImageResource_t &eID,
                                const eC_UInt &uiSrcX, const eC_UInt &uiSrcY, const eC_UInt &uiSrcWidth, const eC_UInt &uiSrcHeight,
                                const eC_Value &vDestX, const eC_Value &vDestY, const eC_Value &vDestWidth, const eC_Value &vDestHeight,
                                const eC_UByte &ubAlpha = 255,
                                const eC_Value &vAngle = eC_FromFloat(0),
                                const eC_Value &vRotCenterX = eC_FromFloat(0.5), const eC_Value &vRotCenterY = eC_FromFloat(0.5) );

    virtual eC_UInt GetImgWidthImpl(const ImageResource_t &eID) const;
    virtual eC_UInt GetImgHeightImpl(const ImageResource_t &eID) const;

    /// Calculates the size of an image in pixels. Called by the image cache.
    virtual eC_UInt GetImageSize(const ImageResource_t& eID) const;

    /** Sets the default 2D view used by the gfx wrapper for 2D drawing operations, like FilledRect, LIne, Blit, etc.
    */
    void SetDefault2DView();

    eGML *m_pEGML;                  ///< eGML instance pointer
    eGML_Screen*  m_pScreen;        ///< Screen for eGML
    eGML_Bitmap*  m_pRenderBitmap;  ///< Target bitmap for render operations in eGML

    eC_UInt m_uiScreenWidth;  ///< Screen width
    eC_UInt m_uiScreenHeight; ///<Screen height

    /// while subpixel rendering is enabled, blitting will be done with subpixel-accuracy (Resulting in a time-consuming RotBlit)
    eC_Bool m_bSubpixelEnabled;

    /// Array of pointers to eImg bitmaps (includes alpha layer)
    eC_TArray <eImg_Bitmap*> m_pImage;

    eC_Flag m_uiBlitFlags;  ///<Flags for blit operation

    CGfxWrapeGML(void);
    virtual ~CGfxWrapeGML(void);

private:

    virtual CGUIBitmapPlane* CreateGfxDependentBitmap(
        GUIBitmapPlaneID_t uiNewBitmapID,
        const CGUIObject& rkParentObject,
        const eC_Value &vWidth,
        const eC_Value &vHeight);

    eC_Bool Init(void);
    eC_Bool DeInit(void);

    /** Copy-constructor intentionally not implemented.
    */
    CGfxWrapeGML(const CGfxWrapeGML& kSource);

    /** Operator= method intentionally not implemented.
    */
    CGfxWrapeGML& operator=(const CGfxWrapeGML& kSource);
};
#endif