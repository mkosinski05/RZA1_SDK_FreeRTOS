/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GFXWRAP__H_
#define GFXWRAP__H_

#include "GUIRect.h"
#include "eC_Math.h"
#include "GUIFontResource.h"
#include "GUIImageResource.h"
#include "GUIText.h"
#include "FntWrap.h"
#include "GUITrace.h"
#include "GUIImageCache.h"
#include "GUIObject.h"
#include "GUINinePatch.h"
#include "GUIPerfMon.h"

/** @defgroup GUILIANI_GRAPHICS Graphics
    @brief Group of framework components related to drawing graphics (Images, Texts, Graphical primitives,...) onto the screen

    Guiliani is designed independently of a specific underlying graphics library, in order to allow its
    porting to any desired platform. Abstraction of graphics functionality is guaranteed through the use
    of graphics wrappers. The functions offered by graphics wrappers range from drawing operations to
    management of the actual 'screen' (canvas) on which drawing takes place. Each graphics wrapper has
    to be derived from CGfxWrap. Read its documentation for more information. 

    The same platform independence is true for printing text. The Guiliani core supports different font
    rendering engines through its font wrappers (see CFntWrap). Font and graphics wrappers can be freely
    combined to some degree.

    @attention To keep the platform independence it is a good practice only to call functions using CGfxWrap.

    @attention Please be aware that due to the cross-platform character of the framework, not all functions declared
    within the framework's base class need to be available in all platform-specific implementations.
*/

/// Retrieve pointer to Graphics Wrapper instance
#define GETGFX CGfxWrap::GetInstance()

class CGUIBitmapPlane;

inline static eC_UInt UINT_FROM_RGB(const eC_UByte& R, const eC_UByte& G, const eC_UByte& B);
inline static eC_UInt UINT_FROM_ARGB(const eC_UByte& A, const eC_UByte& R, const eC_UByte& G, const eC_UByte& B);
inline static eC_UByte GET_A_VAL32(const eC_UInt& argb);
inline static eC_UByte GET_R_VAL32(const eC_UInt& argb);
inline static eC_UByte GET_G_VAL32(const eC_UInt& argb);
inline static eC_UByte GET_B_VAL32(const eC_UInt& argb);

/// The graphics wrapper base class.
/**
    The methods declared in this base class are used by all Guiliani standard controls to display
    themselves. You may leave implementations of certain methods empty for your specific derivation if
    you know that you will not need them. For example you may decide not to implement the image loading
    functionality for your specific graphics wrapper. You are also free to add completely new methods
    to your own wrapper classes and use these within your custom-made controls.

    <b>Please be aware that due to the cross-platform character of the framework, not all functions declared
    within the CGfxWrap base class need to be available in all platform-specific implementations.</b>

    @ingroup GUILIANI_GRAPHICS
*/
class CGfxWrap
{
    /// The resource manager accesses image resources.
    friend class CGUIResourceManager;

    /// The Background Image Loader accesses the LoadImg method.
    friend class CGUIBackgroundImageLoaderThread;

    /** The ImageCache is the only class which may access the LoadImg and
        UnloadImg-Methods of the GfxWrapper directly. Therefore, it is granted
        friend-access.
    */
    friend class CGUIImageCacheImpl;

    /// CGUIText accesses the protected methods GetTextYPosAdjustment and GetTextXPosAdjustment.
    friend class CGUIText;

    /// CFntWrap accesses the protected method SetFntWrap.
    friend class CFntWrap;

    /// CGUI accesses the protected method RedrawGUI.
    friend class CGUI;

public:

    /// Destructor
    virtual ~CGfxWrap(void);

    /** return graphics wrapper instance
        @return Graphics wrapper instance
    */
    static inline CGfxWrap& GetInstance() { return *ms_pGfxWrap;}

    /// Deinitialization
    void DeInit();

    /// Delete the graphics wrapper instance
    static void DeleteInstance();

    // ---------------------------------------------------------------
    //  General functionality
    // ---------------------------------------------------------------

    /**
        The Refresh function is called for every invalidated rect of a frame.
        For some GFXWrappers it is necessary to
        know if it is the first or last call of a frame, so you can decide what to do.
        This might be a simple copy instruction of the invalidated
        area or the flip at the end of the frame.
        This enum contains the needed information.
    */
    enum RefreshCall_t
    {
        SINGLE_REFRESH,         ///< Only one refresh in this frame. So it is the first and the last call.
        FIRST_REFRESH_OF_FRAME, ///< The first refresh call in this frame.
        INTERMEDIATE_REFRESH,   ///< Any Refresh between the first and the last call.
        LAST_REFRESH_OF_FRAME,  ///< The last refresh call in this frame.
        FORCED_REFRESH          ///< Refresh was forced. A refresh of this type is used in addition to one of the others, see ForceRefresh().
    };

    /** Forces the graphics wrapper to call Refresh() in the next frame, no
        matter if anything was invalidated.
        An application can use this when implementing a derived graphics
        wrapper that for instance displays a changing background image that
        is not known to Guiliani. The framework will call Refresh() at least
        once, using the refresh type FORCED_REFRESH. Any other necessary
        refresh calls will take place after that.
        At the time of this call, the invalidated rectangle may not be
        comprehensive and the absolute object positions are not updated yet.
    */
    inline void ForceRefresh() { m_bForceRefresh = true; }
    /** @name Screen parameters
    */
    ///@{
    /** Allocates storage for a virtual screen in which the graphics operations will draw.
        @param uiWidth Width of virtual screen.
        @param uiHeight Height of virtual screen.
    */
    virtual void CreateScreen(
        const eC_UInt &uiWidth, const eC_UInt &uiHeight) {}

    /** Returns the screen width in pixels, may vary if DrawToBitmap is active.
        It will then return the active BitmapPlane's width instead.
        @see GetPhysicalScreenWidth()
        @return eC_UInt The screen width
    */
    virtual eC_UInt GetVirtualScreenWidth() = 0;

    /** Returns the screen height in pixels, may vary if DrawToBitmap is active.
        It will then return the active BitmapPlane's height instead.
        @see GetPhysicalScreenHeight()
        @return eC_UInt The screen height
    */
    virtual eC_UInt GetVirtualScreenHeight() = 0;

    /** Sets the new physical screen size.
        @param uiWidth New screen width.
        @param uiHeight New screen height.
    */
    virtual void SetScreenSize(const eC_UInt &uiWidth, const eC_UInt &uiHeight);

    /** Returns the screen width. 
        Will always return the physical screen width with which the GfxWrap was originally initialized.
        Regardless of whether the virtual screen size has been modified e.g. while drawing into a CGUIBitmapPlane.
        @return Physical screen width
    */
    virtual eC_UInt GetPhysicalScreenWidth() = 0;

    /** Returns the screen height.
        Will always return the physical screen height with which the GfxWrap was originally initialized.
        Regardless of whether the virtual screen size has been modified e.g. while drawing into a CGUIBitmapPlane.
        @return Physical screen height
    */
    virtual eC_UInt GetPhysicalScreenHeight() = 0;

    /** Returns the vertical pixel size in pixel per inch.
        @returns vertical PPI
    */
    virtual eC_UInt GetScreenVerticalPPI() const {return (72);}          // 72 as default

    /** Returns the horizontal pixel size in pixel per inch.
        @returns horizontal PPI
    */
    virtual eC_UInt GetScreenHorizontalPPI() const {return (72);}        // 72 as default
    ///@}
    // ---------------------------------------------------------------
    // Drawing functions
    // ---------------------------------------------------------------

    /** @name Drawing parameters
    */
    ///@{
    /** Sets the foreground color via red, green, blue and alpha values
        @param ubRed Red intensity within a range of 0 - 255
        @param ubGreen Green intensity within a range of 0 - 255
        @param ubBlue Blue intensity within a range of 0 - 255
        @param ubAlpha Alpha intensity within a range of 0 (transparent) - 255 (opaque)
    */
    void SetForegroundColor(
        const eC_UByte &ubRed,
        const eC_UByte &ubGreen,
        const eC_UByte &ubBlue,
        const eC_UByte &ubAlpha = 255)
    {
        SetForegroundColorImpl(ubRed, ubGreen, ubBlue, ApplyGlobalAlpha(ubAlpha));
    }

    /** Sets the foreground color via a 32 Bit value in the Format 0xAARRGGBB
        Example Values:
            0xFFFF0000  = Red (fully opaque)
            0x77FFFFFF  = White (semi transparent)
        @param uiCol 32Bit Color value in the Format 0xAARRGGBB
    */
    void SetForegroundColor(const eC_UInt &uiCol)
    {
        // The value of alpha is multiplied by the scaling factor (in the event of widgets with text, the text's alpha has to be scaled)
        SetForegroundColorImpl(
            GET_R_VAL32(uiCol), GET_G_VAL32(uiCol), GET_B_VAL32(uiCol), ApplyGlobalAlpha(GET_A_VAL32(uiCol)));
    }

    /** Updates the global alpha via an unsigned Byte
        The global alpha value will affect all drawing operations.
        Subsequent calls to UpdateGlobalAlpha will blend the new alpha value with
        the formerly set one.
        @see SetGlobalAlpha
        @param ubAlpha unsigned Byte
    */
    void UpdateGlobalAlpha(const eC_UByte &ubAlpha)
    {
        m_ubAlpha= ApplyGlobalAlpha(ubAlpha);
        m_vGlobalAlphaFactor= eC_Div(eC_FromInt(m_ubAlpha), eC_FromFloat(255.0f));
    }


    /** Sets the global alpha via an unsigned Byte.
        In contrast to UpdateGlobalAlpha, this will NOT blend subsequent alphas
        @see UpdateGlobalAlpha
        @param ubAlpha unsigned Byte
    */
    void SetGlobalAlpha(const eC_UByte &ubAlpha)
    {
        m_ubAlpha= ubAlpha;
        m_vGlobalAlphaFactor= eC_Div(eC_FromInt(m_ubAlpha), eC_FromFloat(255.0f));
    }

    /** Sets the line width in pixels
        @param vWidth width of lines
        @return The previous line width.
    */
    virtual eC_Value SetLineWidth(const eC_Value &vWidth)
    {
        eC_Value vOldWidth = m_vLineWidth;
        m_vLineWidth = vWidth;
        return vOldWidth;
    }

    /** Gets the current foreground color.
        @return The foreground color.
     */
    inline eC_UInt GetForegroundColor() const {return m_uiColor;}

    /** Gets the current alpha value.
        @return The alpha value.
     */
    inline eC_UByte GetGlobalAlpha() const {return m_ubAlpha;}

    /** Applies the current global alpha to the given alpha value and returns the result.
        If e.g. the global alpha is 127, and the supplied alpha is 127, the returned value will be 64.
        @param ubAlpha The alpha value to which global alpha shall be applied.
        @return The modified alpha value.
     */
    inline eC_UByte ApplyGlobalAlpha(const eC_UByte ubAlpha) const {return static_cast<eC_UByte>(eC_ToInt(eC_Mul(eC_FromInt(ubAlpha), m_vGlobalAlphaFactor)));}
    ///@}
    /** @name Drawing primitives
    */
    ///@{
    /** Draws a line from (vAbsX1, vAbsY1) to (vAbsX2, vAbsY2)
        @param vAbsX1 X-coordinate of starting point
        @param vAbsY1 Y-Coordinate of starting point
        @param vAbsX2 X-coordinate of ending point
        @param vAbsY2 Y-Coordinate of ending point
    */
    virtual void Line(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2) = 0;

    /** Draws a line defined by a CGUIRect
        @param kAbsRect CGUIRect containing the line endpoints
        @see CGfxWrap::Line(const eC_Value &vAbsX1, const eC_Value &vAbsY1, const eC_Value &vAbsX2, const eC_Value &vAbsY2)
    */
    inline void Line(const CGUIRect& kAbsRect)
    {
        Line(
            kAbsRect.GetX1(), kAbsRect.GetY1(),
            kAbsRect.GetX2(), kAbsRect.GetY2());
    }

    /** Draws a rectangle with the top left edge at (vAbsX1, vAbsY1) and
            bottom right edge at (vAbsX2, vAbsY2).
        The bottom edge and the right edge is not included in the drawn rect.
        e.g. Rect(0, 0, 10, 10 ); the rect is drawn from (0,0) to (9,9)
        (both included).
        @see CGUIObject::GetAbsRect()
        @param vAbsX1 X-coordinate of top left corner
        @param vAbsY1 Y-Coordinate of top left corner
        @param vAbsX2 X-coordinate of bottom right corner
        @param vAbsY2 Y-Coordinate of bottom right corner
    */
    virtual void Rect(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2) = 0;

    /** Draws a rectangle defined by a CGUIRect
        @param kAbsRect CGUIRect containing the rectangle corners' endpoints
        @see CGfxWrap::Rect(const eC_Value &vAbsX1, const eC_Value &vAbsY1, const eC_Value &vAbsX2, const eC_Value &vAbsY2)
    */
    inline void Rect(const CGUIRect& kAbsRect)
    {
        Rect(
            kAbsRect.GetX1(), kAbsRect.GetY1(),
            kAbsRect.GetX2(), kAbsRect.GetY2());
    }

    /** Draws a filled rectangle with the top left edge at (vAbsX1, vAbsY1)
            and bottom right edge at (vAbsX2, vAbsY2)
        The bottom edge and the right edge is not included in the drawn rect.
        e.g. FilledRect(0, 0, 10, 10 ); the rect is drawn from (0,0) to (9,9)
        (both included).
        @see CGUIObject::GetAbsRect()
        @param vAbsX1 X-coordinate of top left corner
        @param vAbsY1 Y-Coordinate of top left corner
        @param vAbsX2 X-coordinate of bottom right corner
        @param vAbsY2 Y-Coordinate of bottom right corner
    */
    virtual void FilledRect(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2) = 0;

    /** Draws a filled rectangle defined by a CGUIRect
        @param kAbsRect CGUIRect containing the rectangle corners' endpoints
        @see CGfxWrap::FilledRect(const eC_Value &vAbsX1, const eC_Value &vAbsY1, const eC_Value &vAbsX2, const eC_Value &vAbsY2)
    */
    inline void FilledRect(const CGUIRect& kAbsRect)
    {
        FilledRect(
            kAbsRect.GetX1(), kAbsRect.GetY1(),
            kAbsRect.GetX2(), kAbsRect.GetY2());
    }

    /** Draws an ellipse which is completly inside the rectangle described
            through the rectangle the top left edge at (vAbsX1, vAbsY1)
            and bottom right edge at (vAbsX2, vAbsY2).
            The ellipse will be filled out if the last parameter is true
        @param vAbsX1 X-coordinate of bounding rectangle's top left corner
        @param vAbsY1 Y-Coordinate of bounding rectangle's top left corner
        @param vAbsX2 X-coordinate of bounding rectangle's bottom right corner
        @param vAbsY2 Y-Coordinate of bounding rectangle's bottom right corner
        @param bFilled Specifies whether the ellipse will be drawn outlined (false) or filled (true)
    */
    virtual void Ellipse(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2,
        const eC_Bool &bFilled) = 0;

    /** Draws an ellipse defined by a CGUIRect
        @see CGfxWrap::Ellipse(const eC_Value &vAbsX1, const eC_Value &vAbsY1, const eC_Value &vAbsX2, const eC_Value &vAbsY2)
        @param kAbsRect CGUIRect containing the rectangle corners' endpoints
        @param bFilled Specifies whether the ellipse will be drawn outlined (false) or filled (true)
    */
    inline void Ellipse(
        const CGUIRect& kAbsRect,
        const eC_Bool &bFilled)
    {
        Ellipse(
            kAbsRect.GetX1(), kAbsRect.GetY1(),
            kAbsRect.GetX2(), kAbsRect.GetY2(),
            bFilled);
    }

    /** Draws the given Text at (vAbsX1, vAbsY1)
        @param vAbsX1 X-position where the Text will be drawn
        @param vAbsY1 Y-position where the Text will be drawn
        @param lpString Text to output
    */
    void Text(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_String * const lpString)
    {
        if (ms_pkFntWrap != NULL)
        {
            CONTROLPOINTSTART(CGUIPerfMon::eText)
            ms_pkFntWrap->Text(vAbsX1, vAbsY1, lpString);
            CONTROLPOINTEND(CGUIPerfMon::eText)
        }
    }
    ///@}

    // ---------------------------------------------------------------
    // Clipping and invalidated rectangles
    // ---------------------------------------------------------------
    
    /** @name Clipping and invalidated rectangles
    */
    ///@{
    /** Redraw the invalidated regions of the screen and refresh the visible display of the GUI.
    This method is declared virtual so that you may implement specialized behaviour in derived classes,
    for example if you are using a graphics engine which forces you to flip the front and back buffers and
    you are thus required to disable the Invalidated-rectangles approach. */
    virtual void RedrawGUI();

    /** Sets a clipping rectangle.
        All drawing operations will be limited to the region within the clipping rectangle. This is very
        useful for advanced features such as soft-scrolling. Note that the clipping rectangle is not
        allowed to exceed the invalidated rectangle, since this would lead to visual artifacts.
        @param NewAbsClipRect A normalised rectangle that includes all pixels that could be drawn
            by the functions of the graphics wrapper. The rectangle includes the
            border. All pixels outside will be left untouched.
    */
    virtual void SetCliprect(const CGUIRect &NewAbsClipRect) = 0;

    /// Resets the clipping rectangle
    virtual void ResetCliprect();

    /// Retrieves the currently set clipping rectangle
    virtual void GetCliprect(CGUIRect &AbsClipRect) const;

    /** Invalidates a region of the GUI.
        All objects overlapping with an invalidated region will be redrawn during the next redraw-cycle.
        The right edge and the bottom edge of the rectangle is not included in the invalidation.
        e.g. InvalidateRect(0, 0, 10, 10); will invalidate from (0,0) to (9,9)
            on the screen (both included).
        So InvalidateRect(CGUIObject::GetAbsRect()) will invalidate the complete drawing area of the object.
        @see CGUIObject::GetAbsRect().
        @param kInvalidRect Rectangular region of the GUI which has to be redrawn
    */
    void InvalidateRect( const CGUIRect &kInvalidRect);

    /** Invalidates a region of the GUI.
        @see CGfxWrap::InvalidateRect(const CGUIRect &kInvalidRect)
        @param vAbsX1 X-Coordinate of region's top left corner
        @param vAbsY1 Y-Coordinate of region's top left corner
        @param vAbsX2 X-Coordinate of region's bottom right corner
        @param vAbsY2 Y-Coordinate of region's bottom right corner
    */
    void InvalidateRect(
        const eC_Value &vAbsX1, const eC_Value &vAbsY1,
        const eC_Value &vAbsX2, const eC_Value &vAbsY2)
    {
        InvalidateRect( CGUIRect( vAbsX1, vAbsY1, vAbsX2, vAbsY2));
    }

    /// Returns the current invalidated rectangle
    inline const CGUIRect& GetInvalidatedRect() const
    {
        return m_InvalidatedRect;
    }

      /** Called for each invalidated rectangle right before CGUI::HandleDraw()
        is called.
        A derived graphics wrapper can implement this method to do necessary
        clean-up, if required, before the GUI tree is drawn. Since HandleDraw
        will only affect the current invalidated rectangle, this method should
        also restrict its operations to that region of the screen.
    */
    virtual void ClearInvalidRect() {}
    ///@}
    
    /** @name Image handling
    */
    ///@{
    // ---------------------------------------------------------------
    // Image-access
    // ---------------------------------------------------------------
    /** Blit an image.
        Blits (draws) the image associated with the supplied Image-identifier to the specified position
        on the screen.
        @param eID       Identifier of the image to be blitted.
        @param vAbsDestX Destination x-position.
        @param vAbsDestY Destination y-position.
    */
    void BlitImg(
        const ImageResource_t &eID,
        const eC_Value &vAbsDestX, const eC_Value &vAbsDestY)
    {
        BlitImgExt( eID, 0, 0, GetImgWidth( eID), GetImgHeight( eID), vAbsDestX, vAbsDestY, eC_FromInt(GetImgWidth( eID)), eC_FromInt(GetImgHeight( eID)));
    }

    /** Extended blit of an image.
        Performs a stretch blit of the image associated with the supplied Image Identifier.
        The image will be blitted to the position specified by the x- and y-parameter and automatically
        stretched to meet the desired width and height if the bStretch flag is set to true.
        @param eID       Identifier of the image to be blitted.
        @param vAbsDestX Destination x-position.
        @param vAbsDestY Destination y-position.
        @param vWidth    Target width. Image will be automatically stretched if necessary, if bStretch is true.
        @param vHeight   Target height. Image will be automatically stretched if necessary, if bStretch is true.
        @param bStretch  Image will be stretched to meet the desired size if set to true.
        @param ubAlpha   Degree of transparency. 0=transparent 255=opaque  Default is opaque.
    */
    void BlitImgExt(
        const ImageResource_t &eID,
        const eC_Value &vAbsDestX, const eC_Value &vAbsDestY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const eC_Bool &bStretch = true,
        const eC_UByte &ubAlpha = 255)
    {
        if (eID != DUMMY_IMAGE)
        {
            if( bStretch)
            {
                BlitImgExt(
                    eID,
                    0, 0, GetImgWidth( eID), GetImgHeight( eID),
                    vAbsDestX, vAbsDestY, vWidth, vHeight,ubAlpha);
            }
            else
            {
                BlitImgExt(
                    eID,
                    0, 0,
                    eC_ToInt(eC_Min( eC_FromInt(GetImgWidth( eID)), vWidth)), eC_ToInt(eC_Min( eC_FromInt(GetImgHeight( eID)), vHeight)),
                    vAbsDestX, vAbsDestY,
                    eC_Min( eC_FromInt(GetImgWidth( eID)), vWidth), eC_Min( eC_FromInt(GetImgHeight( eID)), vHeight),ubAlpha);
            }
        }
    }

    /** Nine-Patch blit of an image.
        Performs an intelligent 9-patch stretch blit of the image associated with the supplied Image Identifier.
        The image will be blitted to the position given by the upper left corner of the supplied rectangle and stretched
        to the desired width and height. To reduce visual artifacts during stretching, the image will automatically be split
        up into 9 parts. The four corners will be left unscaled, the four edges are each scaled in one axis, and the middle part
        is stretched in both axes. The two parameters uiXBorder and uiYBorder specify the width/height of the sections in pixels.
	@see CGUINinePatch
        @param eID              Identifier of the image to be blitted.
        @param kAbsRect         Rectangle describing the destination position and size of the image on the screen
        @param uiTopBorder      Height of the topmost three sections of the Nine-patch in pixels
        @param uiBottomBorder   Height of the lowermost three sections of the Nine-patch in pixels
        @param uiLeftBorder     Width of the leftmost three sections of the Nine-patch in pixels
        @param uiRightBorder    Width of the rightmost three sections of the Nine-patch in pixels
        @param ubAlpha          Degree of transparency. 0=transparent 255=opaque.  Default is opaque.
    */
    void BlitImgNinePatch(
        const ImageResource_t &eID,
        const CGUIRect& kAbsRect,
        const eC_UInt uiTopBorder,
        const eC_UInt uiBottomBorder,
        const eC_UInt uiLeftBorder,
        const eC_UInt uiRightBorder,
        const eC_UByte &ubAlpha = 255);

    /** Nine-Patch blit of an image.
        @see BlitImgNinePatch(const ImageResource_t &eID, const CGUIRect& kAbsRect, const eC_UInt uiTopBorder, const eC_UInt uiBottomBorder, const eC_UInt uiLeftBorder, const eC_UInt uiRightBorder, const eC_UByte &ubAlpha)
        @param eID              Identifier of the image to be blitted.
        @param kAbsRect         Rectangle describing the destination position and size of the image on the screen
        @param kNinePatch       Nine-Patch describing the borders
        @param ubAlpha          Degree of transparency. 0=transparent 255=opaque.  Default is opaque.
    */
    void BlitImgNinePatch(
        const ImageResource_t &eID,
        const CGUIRect& kAbsRect,
        const CGUINinePatch& kNinePatch,
        const eC_UByte &ubAlpha = 255)
    {
        BlitImgNinePatch(eID, kAbsRect, kNinePatch.GetTop(), kNinePatch.GetBottom(), kNinePatch.GetLeft(), kNinePatch.GetRight(), ubAlpha );
    }

    /** Extended blit of an image.
        Performs a stretch blit of the image associated with the supplied Image Identifier.
        The image will be blitted to the position given by the upper left corner of the supplied rectangle.
        It will be stretched to meet the desired width and height if the bStretch flag is set to true.
        @param eID       Identifier of the image to be blitted.
        @param kAbsRect  Rectangle describing the destination position and size of the image on the screen
        @param bStretch  Image will be stretched to meet the desired size if set to true.
        @param ubAlpha   Degree of transparency. 0=transparent 255=opaque  Default is opaque.
    */
    inline void BlitImgExt(
        const ImageResource_t &eID,
        const CGUIRect& kAbsRect,
        const eC_Bool &bStretch = true,
        const eC_UByte &ubAlpha = 255)
    {
        BlitImgExt(
            eID,
            kAbsRect.GetX1(), kAbsRect.GetY1(),
            kAbsRect.GetWidth(), kAbsRect.GetHeight(),
            bStretch,
            ubAlpha);
    }

    /** Extended blit of an image.
        Performs a stretch blit of the image associated with the supplied Image Identifier.
        An area of the image (which is defined by kSrcAbsRect) will be blitted to the position given by the upper left
        corner of kDstAbsRect. It will be stretched to meet the desired width and height.
        @param eID          Identifier of the image to be blitted.
        @param kSrcAbsRect  Rectangle describing the relevant area within the source image
        @param kDstAbsRect  Rectangle describing the destination position and size of the image on the screen
        @param ubAlpha      Degree of transparency. 0=transparent 255=opaque  Default is opaque.
    */
    inline void BlitImgExt(
        const ImageResource_t &eID,
        const CGUIRect& kSrcAbsRect,
        const CGUIRect& kDstAbsRect,
        const eC_UByte &ubAlpha = 255)
    {
        BlitImgExt(eID,
            eC_ToInt(kSrcAbsRect.GetX1()), eC_ToInt(kSrcAbsRect.GetY1()),
            eC_ToInt(kSrcAbsRect.GetWidth()), eC_ToInt(kSrcAbsRect.GetHeight()),
            kDstAbsRect.GetX1(), kDstAbsRect.GetY1(),
            kDstAbsRect.GetWidth(), kDstAbsRect.GetHeight(),
            ubAlpha);
    }

    /** Extended blit of an image.
        Performs a rotated stretch blit of the image associated with the supplied Image Identifier.
        An area of the image (which is defined by uiAbsSrcX, uiAbsSrcY, uiSrcWidth and uiSrcHeight) will be blitted to the
        position given by vAbsDestX and vAbsDestY. It will be stretched to meet vDestWidth and vDestHeight if the bStretch
        flag is set to true.
        Optionally, the image can be rotated by vAngle degrees around a point defined by vRotCenterX and vRotCenterY.
        The center of rotation is given in coordinates within a range of 0 to 1 (similar to U/V coordinates), where (0,0) is the
        image's top left corner, (1,1) equals the lower right corner and (0.5, 0.5) corresponds to the image's center.

        @param eID          Identifier of the image to be blitted.
        @param uiAbsSrcX    X coordinate of upper left corner of the relevant area within the source image
        @param uiAbsSrcY    Y coordinate of upper left corner of the relevant area within the source image
        @param uiSrcWidth   Width of the relevant area within the source image
        @param uiSrcHeight  Height of the relevant area within the source image
        @param vAbsDestX    Destination X position in absolute coordinates
        @param vAbsDestY    Destination Y position in absolute coordinates
        @param vDestWidth   Destination width
        @param vDestHeight  Destination height
        @param ubAlpha      Degree of transparency. 0=transparent 255=opaque  Default is opaque.
        @param vAngle       Angle in degrees by which the image will be rotated
        @param vRotCenterX  Rotation center X. In range of 0 to 1  (like U/V coordinates)
        @param vRotCenterY  Rotation center Y. In range of 0 to 1  (like U/V coordinates)
        */
    inline void BlitImgExt(
        const ImageResource_t &eID,
        const eC_UInt &uiAbsSrcX, const eC_UInt &uiAbsSrcY,
        const eC_UInt &uiSrcWidth, const eC_UInt &uiSrcHeight,
        const eC_Value &vAbsDestX, const eC_Value &vAbsDestY,
        const eC_Value &vDestWidth, const eC_Value &vDestHeight,
        const eC_UByte &ubAlpha = 255,
        const eC_Value &vAngle = eC_FromFloat(0),
        const eC_Value &vRotCenterX = eC_FromFloat(0.5), const eC_Value &vRotCenterY = eC_FromFloat(0.5) )
    {
        if (MAP_IMGID_TO_ARRAY(eID) >= m_uiNOFImages)
        {
            GUILOG(GUI_TRACE_ERROR, "CGfxWrap::BlitImgExt: Illegal image ID (" + eC_String(eID) + ") supplied.\n");
            return;
        }

        if (eID == DUMMY_IMAGE)
        {
         //   GUILOG(GUI_TRACE_ERROR, "CGfxWrap::BlitImgExt: Tried to access DUMMY_IMAGE. ImageResource is not set.\n");
            return;
        }

        CGUIImageCache::NotifyImageAccess(eID);

        if( !ImageExists( eID) )
        {
            return;
        }
        //The value of alpha is multiplied by the scaling factor (in the event of, for example, widgets in a composite object)
        BlitImgExtImpl(eID, uiAbsSrcX, uiAbsSrcY, uiSrcWidth, uiSrcHeight,
            vAbsDestX, vAbsDestY, vDestWidth, vDestHeight, ApplyGlobalAlpha(ubAlpha), vAngle, vRotCenterX, vRotCenterY);
    }

    /** Returns the width of an image.
        This method first checks for validity of the ID, then calls
        GetImgWidthImpl().
        @param eID Identifier of the image.
        @return The width of the image in pixels.
    */
    inline eC_UInt GetImgWidth(const ImageResource_t &eID) const
    {
        if (MAP_IMGID_TO_ARRAY(eID) >= m_uiNOFImages)
        {
            GUILOG(GUI_TRACE_ERROR, "CGfxWrap::GetImgWidth: Illegal image ID (" + eC_String(eID) + ") supplied.\n");
            return 0;
        }

        if (eID == DUMMY_IMAGE)
        {
          //  GUILOG(GUI_TRACE_WARNING, "CGfxWrap::GetImgWidth: Tried to access DUMMY_IMAGE. ImageResource is not set.\n");
            return 0;
        }

        CGUIImageCache::NotifyImageAccess(eID);

        if( !ImageExists( eID) )
        {
            return 0;
        }


        return GetImgWidthImpl(eID);
    }

    /** Gets the height of an image.
        This method first checks for validity of the ID, then calls
        GetImgHeightImpl().
        @param eID Identifier of the image.
        @return The height of the image in pixels.
    */
    inline eC_UInt GetImgHeight(const ImageResource_t &eID) const
    {
        if (MAP_IMGID_TO_ARRAY(eID) >= m_uiNOFImages)
        {
            GUILOG(GUI_TRACE_ERROR, "CGfxWrap::GetImgHeight: Illegal image ID (" + eC_String(eID) + ") supplied.\n");
            return 0;
        }

        if (eID == DUMMY_IMAGE)
        {
          //  GUILOG(GUI_TRACE_ERROR, "CGfxWrap::GetImgHeight: Tried to access DUMMY_IMAGE. ImageResource is not set.\n");
            return 0;
        }

        CGUIImageCache::NotifyImageAccess(eID);

        if( !ImageExists( eID) )
        {
            return 0;
        }

        return GetImgHeightImpl(eID);
    }

    /// Returns the bounding box of the image, placed at the origin (0, 0)
    inline CGUIRect GetImgOriginRect(const ImageResource_t &eID) const
    {
        return CGUIRect(eC_FromInt(0), eC_FromInt(0),
            eC_FromInt(GetImgWidth(eID)), eC_FromInt(GetImgHeight(eID)));
    }
    ///@}
    /** @name Bitmap Planes
        @see @ref GUILIANI_BITMAPPLANES
    */
    ///@{
    // ---------------------------------------------------------------
    // BitmapPlane-Access
    // ---------------------------------------------------------------

    /** Changes the RenderTarget to the BitmapPlane with the given ID.
        All successive GfxWrap draw commands will be done in the bitmap of this plane
        instead of within the framebuffer
        @param uiBitmapID ID of the BitmapPlane in which will be drawn.
        @return True if the BitmapPlane with given ID exists, otherwise False
    */
    virtual eC_Bool DrawToBitmap(GUIBitmapPlaneID_t uiBitmapID);

    /** Activates the Screen as active RenderTarget again
        @return True if the changing was successful, otherwise False
    */
    virtual eC_Bool DrawToScreen();

     /** Creates a BitmapPlane with given width and height.
        @param rkParentObject reference to the GUIObject
               which will be the parent of the BitmapPlane
        @param vWidth width of the new BitmapPlane
        @param vHeight height of the new BitmapPlane
        @return the ID of the new generated BitmapPlane
    */
    virtual GUIBitmapPlaneID_t CreateBitmapPlane(const CGUIObject& rkParentObject,
                                                 const eC_Value &vWidth, const eC_Value &vHeight);

    /** Deletes the BitmapPlane with the given ID.
        @param uiBitmapID ID of the BitmapPlane to delete
    */
    void DeleteBitmapPlane(const GUIBitmapPlaneID_t uiBitmapID);

    /** Deletes all BitmapPlanes belonging to the given Object.
       @param pObject pointer of the Object whose BitmapPlanes will be deleted
    */
    void DeleteBitmapPlanes(const CGUIObject* pObject);

    /** Accessing the bitmap with the given ID.
        If the bitmap with the given ID does not exist,
        a default bitmap with the size of 0/0 will be returned
        @param uiBitmapID id of the bitmap
        @return CGUIBitmapPlane& The bitmap plane
    */
    CGUIBitmapPlane& GetBitmapPlane(const GUIBitmapPlaneID_t uiBitmapID);

    /** Accessor for the ID of the currently used Bitmap.
        0 if the Wrapper renders directly to the screen.
        @return ID of the render target bitmap or 0 for the screen
    */
    inline GUIBitmapPlaneID_t GetUsedBitmapPlaneID() const {return m_uiUsedRenderBitmapID;}
    ///@}
    /** @name Font handling
    */
    ///@{
    // ---------------------------------------------------------------
    // Font-access
    // ---------------------------------------------------------------

    /** Sets the supplied font active. All subsequent text operations will
        use this font,
        @param eID id of the font.
    */
    void SetFont(const FontResource_t &eID)
    {
        if (ms_pkFntWrap != NULL)
        {
            ms_pkFntWrap->SetFont(eID);
        }
    }

    /** Sets a new font spacing, i.e the space between letters.
        @param fSpacing New font spacing.
    */
    void SetFontSpacing(const eC_Float &fSpacing)
    {
        if (ms_pkFntWrap != NULL)
        {
            ms_pkFntWrap->SetFontSpacing(fSpacing);
        }
    }

    /** Calculates the required space in x- and y-direction for a given text-string using the currently
        set font.
        @param pkText The text.
        @param vWidth Reference to width of the font in pixels.
        @param vHeight Reference to height of the font in pixels.
    */
    void RequiredSpace(const eC_String * const pkText, eC_Value &vWidth, eC_Value &vHeight)
    {
        if (ms_pkFntWrap != NULL)
        {
            CONTROLPOINTSTART(CGUIPerfMon::eRequiredSpace)
            ms_pkFntWrap->RequiredSpace(pkText, vWidth, vHeight);
            CONTROLPOINTEND(CGUIPerfMon::eRequiredSpace)
        }
    }

    /** Calculates the number of characters that fit in the given maximum width for a given text-string
        using the currently set font.
        @param pkText The text.
        @param vWidthMax Maximum width of the font in pixels.
        @param uiNumChars Reference to the numbers of characters that fit given the maximum width.
    */
    void FittingNumChars(const eC_String * const pkText, eC_Value vWidthMax, eC_UInt &uiNumChars)
    {
        if (ms_pkFntWrap != NULL)
        {
            ms_pkFntWrap->FittingNumChars(pkText, vWidthMax, uiNumChars);
        }
    }

    /** Returns the currently set font's internal leading.
        If the internal leading is not supported by a given Font-Wrapper, this returns 0.
        @return internal leading in pixels
    */
    eC_Int GetInternalLeading() const
    {
        if (ms_pkFntWrap != NULL)
        {
            return ms_pkFntWrap->GetInternalLeading();
        }
        return 0;
    }
    ///@}


    /** Gets the number of images (array size).
        @return Number of images.
    */
    inline virtual eC_UInt GetNOFImages() {return m_uiNOFImages;}

    /** Gets the number of fonts (array size).
        @return Number of fonts. If no font wrapper was instantiated, the
        GUIConfig constant number of font resources is returned which only
        reflects 'static' resources without taking dynamic fonts into account.
    */
    inline eC_UInt GetNOFFonts()
    {
        if (ms_pkFntWrap != NULL)
        {
            return ms_pkFntWrap->GetNOFFonts();
        }
        return cuiNOFFontResources;
    }

    /** Returns whether the image with the given ID currently is loaded within the Graphics Wrapper.
        This interface must be implemented by derived graphics wrappers.
        @param eID The ID to check
        @return True if the image with the given ID is currently loaded, False otherwise.
    */
    virtual eC_Bool ImageExists(const ImageResource_t eID) const = 0;


    /** helper method for calculating the next power of two _very_ fast.
        @param vValue eC_value for which the next power of two will be calculated
        @return Returns the closest power-of-two number greater or equal to vValue.
                If NON_POWER_OF_2_TEXTURES is defined, it will return the rounded up
                integer value of vValue.
                Will return 0 when n = 0 and 1 when n = 1.
    */
    inline static eC_UInt GetNextPowerOf2(const eC_Value &vValue)
    {
#if defined NON_POWER_OF_2_TEXTURES
        return eC_ToInt(eC_Ceil(vValue));
#else
        eC_UInt uiPOW = eC_ToInt(vValue);
        --uiPOW;
        uiPOW |= uiPOW >> 16;
        uiPOW |= uiPOW >> 8;
        uiPOW |= uiPOW >> 4;
        uiPOW |= uiPOW >> 2;
        uiPOW |= uiPOW >> 1;
        ++uiPOW;
        return uiPOW;
#endif
    }

    /** Tell Guiliani that the underlying graphics API enforces flipping the back and front buffers (Instead of copying from the back to the front buffer).
        @param bDoubleBufferingEnforcesFlipping Set to True if flipping of buffers leads to visual artifacts */
    void SetDoubleBufferingEnforcesFlipping( const eC_Bool bDoubleBufferingEnforcesFlipping) { m_bDoubleBufferingEnforcesFlipping = bDoubleBufferingEnforcesFlipping; }
    /** Returns whether the DoubleBufferingEnforcesFlipping-flag has been set.
        @return The current status of the DoubleBufferingEnforcesFlipping-flag (as set by the user, or marked as default by the chosen graphics wrapper) */
    eC_Bool GetDoubleBufferingEnforcesFlipping() { return m_bDoubleBufferingEnforcesFlipping; }

    /** @name Framerate Counter
    	@see CGUIFramerate
    */
    ///@{
    /** Enables the FPS counter. 
    	@see CGUIFramerate
    */
    inline void EnableFPSCounter() { m_bFPSCounter = true; }

    /** Disables the FPS counter.
        @see CGUIFramerate
    */
    inline void DisableFPSCounter() { m_bFPSCounter = false; }

    /** Enables the Abs FPS counter. 
        @see CGUIFramerate
    */
    inline void EnableAbsFPSCounter() { m_bAbsFPSCounter = true; }

    /** Disables the Abs FPS counter. 
        @see CGUIFramerate
    */
    inline void DisableAbsFPSCounter() { m_bAbsFPSCounter = false; }
    ///@}
#if defined _DEBUG
    /** @name Debug 
    */
    ///@{
    /** Enables the debug output on console. */
    inline void EnableDebugOutput() { m_bDebugOutput = true; }

    /** Disables the debug output on console. */
    inline void DisableDebugOutput() { m_bDebugOutput = false; }

    /** Enables the invalidate area lines. */
    inline void EnableInvalidateAreaLine() { m_bInvalidateAreaLine = true; }

    /** Disables the invalidate area lines. */
    inline void DisableInvalidateAreaLine() { m_bInvalidateAreaLine = false; }
    ///@}
#endif

protected:
    /** @name Smart redraw */
    ///@{
    /** Method to bring the screen content onto the physical screen so that it becomes
        visible, usually via a blit operation.
        @param eRefreshCallType The call type of this refresh
        @return True of the refresh was performed, False otherwise
    */
    virtual eC_Bool Refresh(const RefreshCall_t eRefreshCallType) = 0;

    /** Called for every single InvalidatedRect() during RedrawGUI().
        Also serves as proxy function to make GETGUI.HandleDraw() accessible from derived graphics wrappers.
        Make sure to call CGfxWrap::StartHandleDraw() when overriding this function. 
        @param crkClipRect Rectangle against which this cycle of handle draw calls will be clipped. */
    virtual void StartHandleDraw(const CGUIRect &crkClipRect);
    ///@}
    /** @name Callbacks */
    ///@{
    /** Callback which is called at the beginning of RedrawGUI()
        Note that this function as well as EndRedrawGUI() will also get called if the list of invalidated rects is empty.
        Check m_kInvalidatedRectList if you need more details on the number and dimensions of invalidated areas. */
    virtual void StartRedrawGUI() {}

    /// Callback which is called at the end of RedrawGUI() after all Invalidated areas were drawn and refreshed
    virtual void EndRedrawGUI() {}

    /// Method to clean and release the screen which was created via the CreateScreen Method
    virtual void DestroyScreen() {}
    ///@}

    /** @name Resource management */
    ///@{
    /** Sets the total number of images. This is called internally by the ResourceManager when dynamically
        allocated image resources require the total number of images to be raised. This calls the SetNOFImagesImpl()
        method internally, which should be implemented in derived GfxWrappers.
        @param uiNOFImages The number of images
        @return True if internal storage has been successfully adapted to support the requested
                number of images. False otherwise.*/
    eC_Bool SetNOFImages(const eC_UInt uiNOFImages)
    {
        m_kGfxWrapSemaphore.Enter();
        eC_Bool bRet = SetNOFImagesImpl( uiNOFImages);
        m_kGfxWrapSemaphore.Leave();
        return bRet;
    }

    /** Sets the total number of images. This is called internally by the ResourceManager when dynamically
        allocated image resources require the total number of images to be raised. Derived classes will
        have to override this method and resize their internal image-storage structures according to
        the new number of images which will have to be stored.
        @param uiNOFImages The number of images
        @return True if internal storage has been successfully adapted to support the requested
                number of images. False otherwise.*/
    virtual eC_Bool SetNOFImagesImpl(const eC_UInt uiNOFImages);


    /** Sets the total number of fonts. This is called internally by the ResourceManager when dynamically
        allocated font resources require the total number of fonts to be raised. the Font-Wrapper classes
        have to implement this method and resize their internal image-storage structures according to
        the new number of fonts which will have to be stored.
        @param uiNOFFonts The number of fonts
        @return True if internal storage has been successfully adapted to support the requested
                number of fonts. False otherwise.*/
    inline eC_Bool SetNOFFonts(const eC_UInt uiNOFFonts)
    {
        if (ms_pkFntWrap != NULL)
        {
            return ms_pkFntWrap->SetNOFFonts(uiNOFFonts);
        }
        return false;
    }
    
    /** Loads an image.
        This loads an image from the given path and associates it with an Image-Identifier.
        This calls the LoadImgImpl() method internally, which has to be implemented by every GfxWrapper.
        @param kPath Complete path and filename of the image to be loaded.
        @param eID ID by which the image will later be accessible.
    */
    void LoadImg(const eC_String& kPath, const ImageResource_t &eID)
    {
        CONTROLPOINTSTART(CGUIPerfMon::eLoadImg)
        m_kGfxWrapSemaphore.Enter();
        // Only load, if this ImageID is not already in use.
        if( !ImageExists( eID))
        {
            LoadImgImpl( kPath, eID);
        }
        m_kGfxWrapSemaphore.Leave();
        CONTROLPOINTEND(CGUIPerfMon::eLoadImg)
    }

    /** Loads an image.
        This loads an image from the given path and associates it with an Image-Identifier.
        @param kPath Complete path and filename of the image to be loaded.
        @param eID ID by which the image will later be accessible.
    */
    virtual void LoadImgImpl(const eC_String& kPath, const ImageResource_t &eID) = 0;

    /** Unloads an image.
        Unloads the image associated with the supplied Image-identifier and frees the memory formerly
        occupied by this image.
        @param eID Identifier of the image to be unloaded.
    */
    virtual void UnloadImg(const ImageResource_t &eID) {}

    /** Change a font identified by pcPath in an array of fonts at position eFontID.
    If the font already exists, it is unloaded first.
    @param eFontID id (and hence index) of the font.
    @param pcPath Path or name of the font.
    @param uiFontSize Size of the font.
    @param eFontStyle Style of the font (italic, underline, bold, ... see GUIFont.h).
    */
    void ChangeFont(const FontResource_t &eFontID,
        const eC_Char* const pcPath,
        const eC_UInt &uiFontSize,
        const CFntWrap::GUIFont_t &eFontStyle)
    {
        if (ms_pkFntWrap != NULL)
        {
            ms_pkFntWrap->ChangeFont(eFontID, pcPath, uiFontSize, eFontStyle);
        }
    }

    /** Loads a font identified by pcPath into an array of fonts at position eFontID.
    If the font already exists, it it not loaded again. Should only be called by a GfxWrapper.
    @param eFontID id (and hence index) of the font.
    @return True if Font was loaded successfully.<BR>
    False otherwise.
    */
    void LoadFont(const FontResource_t &eFontID)
    {
        if (ms_pkFntWrap != NULL)
        {
            CONTROLPOINTSTART(CGUIPerfMon::eLoadFont)
                ms_pkFntWrap->LoadFont(eFontID);
            CONTROLPOINTEND(CGUIPerfMon::eLoadFont)
        }
    }

    /** Removes the font from the font list. Should only be called by a GfxWrapper.
    @param eFontID Id of font to be removed.
    */
    void UnloadFont(const FontResource_t &eFontID)
    {
        if (ms_pkFntWrap != NULL)
        {
            ms_pkFntWrap->UnloadFont(eFontID);
        }
    }
    ///@}

    /** @name Abstract image methods */
    ///@{
    /** Computes the size of a loaded image's raw pixel data (e.g. RGBA or RGB565, depending on platform)
        in memory. Needed for the Image Cache.
        Since this will be called by the Guiliani Image cache, derived wrappers _must not_ call
        NotifyImageAccess() from within this method.
        @param eID The image's ID.
        @return the size of the image in bytes
    */
    virtual eC_UInt GetImageSize(const ImageResource_t& eID) const = 0;

    /** Returns the width of an image. Must be implemented by each graphics
        wrapper.
        @param eID The ID of the image whose width should be returned.
                   It is ensured that eID is always in the valid range and not
                   DUMMY_IMAGE.
        @return The width of the image in pixels.
      */
    virtual eC_UInt GetImgWidthImpl(const ImageResource_t &eID) const = 0;

    /** Returns the height of an image. Must be implemented by each graphics
        wrapper.
        @param eID The ID of the image whose width should be returned.
                   It is ensured that eID is always in the valid range and not
                   DUMMY_IMAGE.
        @return The width of the image in pixels.
      */
    virtual eC_UInt GetImgHeightImpl(const ImageResource_t &eID) const = 0;

    /** Blits an image. This MUST be implemented by all graphics wrappers.
        It is ensured that the supplied image ID is in the valid range and is not DUMMY_IMAGE.
        For detailed explanations, please refer to CGfxWrap::BlitImgExt

        @param eID          Identifier of the image to be blitted.
        @param uiAbsSrcX    X coordinate of upper left corner of the relevant area within the source image
        @param uiAbsSrcY    Y coordinate of upper left corner of the relevant area within the source image
        @param uiSrcWidth   Width of the relevant area within the source image
        @param uiSrcHeight  Height of the relevant area within the source image
        @param vAbsDestX    Destination X position in absolute coordinates
        @param vAbsDestY    Destination Y position in absolute coordinates
        @param vDestWidth   Destination width
        @param vDestHeight  Destination height
        @param ubAlpha      Degree of transparency. 0=transparent 255=opaque  Default is opaque.
        @param vAngle       Angle in degrees by which the image will be rotated
        @param vRotCenterX  Rotation center X. In range of 0 to 1  (like U/V coordinates)
        @param vRotCenterY  Rotation center Y. In range of 0 to 1  (like U/V coordinates)
      */
    virtual void BlitImgExtImpl(
        const ImageResource_t &eID,
        const eC_UInt &uiAbsSrcX, const eC_UInt &uiAbsSrcY,
        const eC_UInt &uiSrcWidth, const eC_UInt &uiSrcHeight,
        const eC_Value &vAbsDestX, const eC_Value &vAbsDestY,
        const eC_Value &vDestWidth, const eC_Value &vDestHeight,
        const eC_UByte &ubAlpha = 255,
        const eC_Value &vAngle = eC_FromFloat(0),
        const eC_Value &vRotCenterX = eC_FromFloat(0.5), const eC_Value &vRotCenterY = eC_FromFloat(0.5) ) = 0;
    ///@}

    /** Sets the foreground color. This MUST be implemented by all graphics wrappers.
    For detailed explanations, please refer to CGfxWrap::SetForegroundColor
        @param ubRed Red intensity within a range of 0 - 255
        @param ubGreen Green intensity within a range of 0 - 255
        @param ubBlue Blue intensity within a range of 0 - 255
        @param ubAlpha Alpha intensity within a range of 0 (transparent) - 255 (opaque)
    */
    virtual void SetForegroundColorImpl(
        const eC_UByte &ubRed,
        const eC_UByte &ubGreen,
        const eC_UByte &ubBlue,
        const eC_UByte &ubAlpha = 255) = 0;

    /** @name Invalidated rectangles */
    ///@{
    /** Sets the current invalidated (dirty) rectangle. This marks the region which has to be redrawn
        during the next call to Refresh()
        @param vAbsX1 X-Coordinate of top left corner
        @param vAbsY1 Y-Coordinate of top left corner
        @param vAbsX2 X-Coordinate of bottom right corner
        @param vAbsY2 Y-Coordinate of bottom right corner
    */
    void SetInvalidatedRect(const eC_Value &vAbsX1, const eC_Value &vAbsY1, const eC_Value &vAbsX2, const eC_Value &vAbsY2);

    /** Sets the current invalidated (dirty) rectangle. This marks the region which has to be redrawn
        during the next call to Refresh()
        @param NewAbsRect Structure of type CGUIRect, specifying the new invalidated rectangle's extends.
    */
    void SetInvalidatedRect(const CGUIRect &NewAbsRect)
    {
        SetInvalidatedRect( NewAbsRect.GetX1(), NewAbsRect.GetY1(), NewAbsRect.GetX2(), NewAbsRect.GetY2() );
    }

    /// Resets the invalidated rectangle.
    void ResetInvalidatedRect();
    ///@}

    /**
    Get the X position adjustment because of platform specific font structure.
    @param eHorAlignment The horizontal alignment of GUIText object.
    @return The x position adjustment. The GUItext object should add this adjustment to
            get the correct position.
    */
    eC_Value GetTextXPosAdjustment(const CGUIText::HorAligned_t eHorAlignment)
    {
        if (ms_pkFntWrap != NULL)
        {
            return ms_pkFntWrap->GetTextXPosAdjustment(eHorAlignment);
        }
        return 0;
    }

    /**
    Get the Y position adjustment because of platform specific font structure.
    @param eVerAlignment The vertical alignment of GUIText object.
    @return The y position adjustment. The GUItext object should add this adjustment to
            get the correct position.
    */
    eC_Value GetTextYPosAdjustment(const CGUIText::VerAligned_t eVerAlignment)
    {
        if (ms_pkFntWrap != NULL)
        {
            return ms_pkFntWrap->GetTextYPosAdjustment(eVerAlignment);
        }
        return 0;
    }

    /// This is the list of invalidated rectangular regions within the GUI.
    eC_TListDoubleLinked<CGUIRect> m_kInvalidatedRectList;

    /// This is the list of regions which have been invalidated during the LAST frame
    eC_TListDoubleLinked <CGUIRect> m_kLastFrameInvalidatedRectList;

    /** Guiliani's smart redraw mechanism requires that the back buffer gets COPIED to the front buffer and NOT FLIPPED
        If the underlying graphics API enforces flipping, though, you can set the m_bDoubleBufferingEnforcesFlipping flag
        and Guiliani will react accordingly. */
    eC_Bool m_bDoubleBufferingEnforcesFlipping;

    /** This rectangle marks the region of the screen, which is currently being redrawn.
    During a redraw of the GUI, this will be assigned every rectangle within m_kInvalidetedRectList    */
    CGUIRect m_InvalidatedRect;

    /** The current clipping rect. Note that this will usually have to be set within the
    used graphics library as well. */
    CGUIRect m_ClippingRect;

    /// Line width in pixels
    eC_Value m_vLineWidth;

    /// static member pointer to Graphics-Wrapper instance
    static CGfxWrap* ms_pGfxWrap;

    /// static member pointer to-Font wrapper instance.
    static CFntWrap* ms_pkFntWrap;

    /// total number of image-objects
    eC_UInt m_uiNOFImages;

    /// Currently set foreground color
    eC_UInt m_uiColor;

    /// Currently set global alpha value
    eC_UByte m_ubAlpha;

    /** Current scaling factor for global alpha
        Used as a helper variable when scaling concatenated alpha operations. */
    eC_Value m_vGlobalAlphaFactor;
    /**
        Indicates whether HandleDraw() was called in last frame.
        In this case we need a refresh if we use double buffering (to switch buffers)
        even if there were no changes in this frame.
    */
    eC_Bool m_bInvalidationInLastFrame;

    /// Constructor
    CGfxWrap();

    /// Semaphore to lock access to methods used in a multi thread context
    eC_Semaphore m_kGfxWrapSemaphore;

    // ---------------------------------------------------------------
    // Bitmap-Access
    // ---------------------------------------------------------------
    /** @name Bitmap Planes */
    ///@{
    /** Method which will be called by CreateBitmap to instantiate a new Bitmap-Object.
        This Method needs to be implemented by every derived GfxWrapper.
        If a GfxWrapper does not support Bitmap-Handling, he may create an CGUIBaseBitmap
	
	@see @ref GUILIANI_BITMAPPLANES
        @param uiNewBitmapID ID of the new BitmapPlane
        @param rkParentObject reference to the GUIObject which will be the parent of the bitmap
        @param vWidth width of the new bitmap
        @param vHeight height of the new bitmap
        @return pointer to the new created BitmapObject
    */
    virtual CGUIBitmapPlane* CreateGfxDependentBitmap(
        GUIBitmapPlaneID_t uiNewBitmapID,
        const CGUIObject& rkParentObject,
        const eC_Value &vWidth, const eC_Value &vHeight);

    /** Accessor for the dummy bitmap with the size 0/0
        @return reference to the dummy bitmap
    */
    inline CGUIBitmapPlane& GetDummyBitmap();

    /// This is called by the framework when rendering to a bitmap plane is finished.
    virtual eC_Bool FinishDrawToBitmap();

    /** Sets the new RenderTarget in the Fnt wrapper,
        should be called by inherited gfx wrappers
        @param rRenderTarget reference to an object with information about the new render target
    */
    inline void SetRenderTarget(const CGUIRenderTarget& rRenderTarget) const
    {
        if (ms_pkFntWrap != NULL)
        {
            ms_pkFntWrap->SetRenderTarget(rRenderTarget);
        }
    }
    ///@}
private:
    /** Helper to get a unused BitmapPlaneID.
        @return Free BitmapPlaneID that can be used to create a new BitmapID.
    */
    GUIBitmapPlaneID_t GetFreeBitmapID();

    ///Removes all invalidated areas.
    void ResetInvalidation();

    /// Used internally to tell the Graphics-Wrapper which Font-Wrapper to use for text-output
    inline void SetFntWrap(CFntWrap* pkFntWrap){ms_pkFntWrap = pkFntWrap;}

    /** Copy-constructor. Should not be used.
        Dummy declaration with no implementation, just to hide the function.
        @param kSource Source object to be copied.
    */
    CGfxWrap(const CGfxWrap& kSource);

    /** Operator= method. Should not be used.
        Dummy declaration with no implementation, just to hide the function.
        @param kSource Source object to be copied.
        @return This instance.
    */
    CGfxWrap& operator=(const CGfxWrap& kSource);

    /** Whether a refresh should be forced when the next frame is drawn.
        @see ForceRefresh()
    */
    eC_Bool m_bForceRefresh;

    /// dummy bitmapplane
    CGUIBitmapPlane* m_pDummyBitmap;
    /// dummy GUIObject for dummy bitmapplane;
    CGUIObject* m_pDummyGUIObject;
    /// array of bitmaps
    eC_TArray<CGUIBitmapPlane*> m_apBitmaps;

    /// rect stored during bitmap rendering
    CGUIRect m_kRealInvalidatedRect;
    CGUIRect m_kRealClipRect;

    /// ID of the currently used render target bitmap. 0 if the screen is the render target.
    GUIBitmapPlaneID_t m_uiUsedRenderBitmapID;

    /// Indicates if the GUI is currently being redrawn. While a redraw is in progress e.g. Invalidation will be disabled. 
    eC_Bool m_bCurrentlyRedrawing;

    /// Activate GfxWrapper debug FPS counter
    eC_Bool m_bFPSCounter;

    /// Activate GfxWrapper debug Abs FPS counter
    eC_Bool m_bAbsFPSCounter;

#if defined _DEBUG
    /// Activate GfxWrapper debug output
    eC_Bool m_bDebugOutput;

    /// Activate GfxWrapper debug invalidate area lines
    eC_Bool m_bInvalidateAreaLine;
#endif
    static eC_Bool ms_bWatermark;
};

// Return an unsigned int value from RGB (Red, Green, Blue) color value. R, G and B are expected to be bytes.
inline static eC_UInt UINT_FROM_RGB(const eC_UByte& R, const eC_UByte& G, const eC_UByte& B)
{
    return
        (static_cast<eC_UInt>(B)) |
        ((static_cast<eC_UInt>(G))<<8) |
        ((static_cast<eC_UInt>(R))<<16);
}

// Return an unsigned int value from ARGB (Alpha, Red, Green, Blue) color value. A, R, G and B are expected to be bytes.
inline static eC_UInt UINT_FROM_ARGB(const eC_UByte& A, const eC_UByte& R, const eC_UByte& G, const eC_UByte& B)
{
    return
        (static_cast<eC_UInt>(B)) |
        ((static_cast<eC_UInt>(G))<<8) |
        ((static_cast<eC_UInt>(R))<<16) |
        ((static_cast<eC_UInt>(A))<<24);
}

/// Return the alpha portion of the parameter.
inline static eC_UByte GET_A_VAL32(const eC_UInt& argb)
{
    return static_cast<eC_UByte>((argb >> 24) & 0xFF);
}
/// Return the red portion of the parameter.
inline static eC_UByte GET_R_VAL32(const eC_UInt& argb)
{
    return static_cast<eC_UByte>((argb >> 16) & 0xFF);
}

/// Return the green portion of the parameter.
inline static eC_UByte GET_G_VAL32(const eC_UInt& argb)
{
    return static_cast<eC_UByte>((argb >> 8) & 0xFF);
}

/// Returns the blue portion of the parameter.
inline static eC_UByte GET_B_VAL32(const eC_UInt& argb)
{
    return static_cast<eC_UByte>(argb & 0xFF);
}
#endif
