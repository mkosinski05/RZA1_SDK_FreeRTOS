/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUI_IMAGE_STACK_H)
#define GUI_IMAGE_STACK_H

#include "GUIAnimatedImage.h"

// @guiliani_doxygen toplevel_control Image Stack
/**
    <table border="0">
        <tr>
            <td width="200">@image html image_stack.png</td>
            <td>
                The "image stack" control is a specialized animated image that shows zooming and fading effects for image transitions (Class: CGUIImageStack).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    @brief A specialization of CGUIAnimatedImage that shows zooming and fading
           effects for image transitions.

    The base class' list of image IDs is used here for showing pseudo 3D
    transitions: one image fades out into the background while getting smaller,
    another one comes in from the front, getting smaller and fading in. The
    opposite direction works similarly: the images appear to come from the
    back to the front while fading. These animations can be started by calling
    ShowNext() and ShowPrevious().

    If this control is focusable, it reacts to GK_PAGEUP and GK_PAGEDOWN
    presses by calling ShowNext() and ShowPrevious(), respectively.

    Alternatively you may operate it via mouse/touch, by dragging in vertical direction.

    The minimum and maximum scale factors for the animation can be freely
    modified, as well as the scale factor for the <i>static</i> image that is
    displayed when no animation is running. A scale factor is a multiplier that
    is applied to the displayed image with this control's size as the base. A
    factor of 1.0 means the exact size of this control, 0.5 means half the size
    and so on. The aspect ratio of the original images are ignored, the images
    are always stretched with the scale factors applied to width <i>and</i>
    height. During animations, the <i>front</i> image's size changes from the
    maximum scale to the static scale (or vice versa), and the <i>back</i>
    image's size changes from the static scale to the minimum scale (or vice
    versa).

    The frame delay (CGUIAnimatedImage::SetFrameDelay()) is re-used as the time
    between <i>static</i> images.

    Example:
@code
    // Create an ImageStack
    CGUIImageStack* pImageStack = new CGUIImageStack( this, eC_FromInt(100), eC_FromInt(100), eC_FromInt(200), eC_FromInt(200), 500);
    // Add some images to it
    eC_TArray<ImageResource_t> kImages;
    kImages.AddElement( IMG_STDCTRL_IMGBTN_STANDARD);
    kImages.AddElement( GUILIANI_LOGO);
    kImages.AddElement( IMG_STDCTRL_IMGBTN_FOCUSED);
    pImageStack->SetImages( kImages);
    // Show the image at index 1
    pImageStack->SetAnimationIndex(1);
    // Set target size of image when fading out
    pImageStack->SetMinimumZoomFactor( eC_FromFloat(0.1) );
    // Set size of visible image while there is no animation ongoing
    pImageStack->SetStaticZoomFactor( eC_FromFloat(0.5) );
    // Set target size of image when fading in
    pImageStack->SetMaximumZoomFactor( eC_FromFloat(1.0) );
    // Return the index of the image which is currently visible
    pImageStack->GetIndexOfVisibleImage();
@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIImageStack : public CGUIAnimatedImage
{
public:
    /** Constructs an empty image stack.
        @param pParent Pointer to the parent object of this image.
        @param vX The x coordinate of the upper left corner of this image stack.
        @param vY The y coordinate of the upper left corner of this image stack.
        @param vWidth The width of this image stack.
        @param vHeight The height of this image stack.
        @param uiFrameDelay Time in milliseconds between static images.
        @param eID Object Identifier of this object.
      */
    CGUIImageStack(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        eC_UInt uiFrameDelay,
        const ObjectHandle_t &eID = NO_HANDLE);

    /// Default constructor for factories.
    CGUIImageStack();

    /** Starts an animation to show the next image. If there are less than
        two images in the list, this method does nothing.
      */
    void ShowNext();

    /** Starts an animation to show the previous image. If there are less than
        two images in the list, this method does nothing.
      */
    void ShowPrevious();

    eC_Bool DoDraw();

    virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1));

    /** Reacts to GK_PAGEUP and GK_PAGEDOWN presses. Calls ShowNext() or
        ShowPrevious() respectively.
        @param eKeyID The Guiliani key identifier.
        @param uiModifiers The key modifiers bitfield.
        @return True if GK_PAGEUP or GK_PAGEDOWN have been handled, otherwise False.
    */
    eC_Bool DoKeyDown(
        const GUIKeyIdentifier_t& eKeyID,
        const eC_UInt& uiModifiers);

    /** Enables control via mouse/touch.
        Dragging in Y-direction by more than half of the object's height will show the next/previous image within the stack.
        @param vDeltaX Mouse movement in X since last frame
        @param vDeltaY Mouse movement in Y since last frame
        @param vAbsX Mouse position in X where event occurred
        @param vAbsY Mouse position in Y where event occurred
        @return Always False
    */
    eC_Bool DoDrag( const eC_Value &vDeltaX, const eC_Value &vDeltaY, const eC_Value &vAbsX, const eC_Value &vAbsY);

    eC_Bool DoButtonDown( const eC_Value &vAbsX, const eC_Value &vAbsY);

    /** Sets the scale multiplier for the maximum zoom level. This factor is
        used for the image that is displayed at the 'front' during animations.
        It is relative to this controls size, i.e,. a factor of 1.0 means that
        the fully-zoomed image has the same size as this control.
        @param vMaxFactor Maximum scale factor.
      */
    inline void SetMaximumZoomFactor(eC_Value vMaxFactor) { m_vMaxScale = vMaxFactor; }

    /** Sets the scale multiplier for the minimum zoom level. This factor is
        used for the image that is displayed in the 'back' during animations.
        It is relative to this controls size, i.e., a factor of 1.0 means that
        the fully-zoomed image has the same size as this control.
        @param vMinFactor Minimum scale factor.
      */
    inline void SetMinimumZoomFactor(eC_Value vMinFactor) { m_vMinScale = vMinFactor; }

    /** Sets the scale multiplier for the static zoom level. This factor is
        used for the image that is displayed when no animation is running.
        It is relative to this controls size, i.e., a factor of 1.0 means that
        the fully-zoomed image has the same size as this control.
        @param vStaticFactor Static scale factor.
      */
    inline void SetStaticZoomFactor(eC_Value vStaticFactor) { m_vStaticScale = vStaticFactor; InvalidateArea(); }

    /** @return The scale multiplier for the maximum zoom level.
        @see SetMaximumZoomFactor()
      */
    inline eC_Value GetMaximumZoomFactor() const { return m_vMaxScale; }

    /** @return The scale multiplier for the minimum zoom level.
        @see SetMinimumZoomFactor()
      */
    inline eC_Value GetMinimumZoomFactor() const { return m_vMinScale; }

    /** @return The scale multiplier for the static zoom level.
        @see SetStaticZoomFactor()
      */
    inline eC_Value GetStaticZoomFactor() const { return m_vStaticScale; }

#if defined(GUILIANI_STREAM_GUI)
    /** Reads all object attributes from streaming file.
        This method is called by CGUIFactoryManager after one of the registered
        factories has created an instance of this class.
    */
    virtual void ReadFromStream();
#endif

#if defined(GUILIANI_WRITE_GUI)
    /** Writes all object attributes to the streaming file. A CGUIStreamWriter
        has to be initialized first.
        @param bWriteClassID This flag is used to select if writing of ControlID,
               leading and trailing tags is performed.
    */
    virtual void WriteToStream(const eC_Bool bWriteClassID=false);
#endif

protected:
    /** Helper for DoDraw().
        @param eID ID of the image to be drawn.
        @param vScale Scale factor to be used (relative to control dimensions).
        @param vYOffset Y offset for the image.
        @param ubAlpha Alpha value to be used for blitting.
      */
    void BlitScaledImage(ImageResource_t eID, eC_Value vScale, eC_Value vYOffset, eC_UByte ubAlpha);

    /// Internal animation direction types.
    enum AnimationDirection
    {
        AD_NONE, ///< No current animation.
        AD_IN,   ///< Animating "into the screen" (front to back).
        AD_OUT   ///< Animating "out of the screen" (back to front).
    };

    /// Current animation direction.
    AnimationDirection m_eDirection;

    /// Animation progress in percent.
    eC_Value m_vAnimationPercentage;

    /** The scale multiplier for the maximum zoom level (relative to this
        control's size). */
    eC_Value m_vMaxScale;

    /** The scale multiplier for the minimum zoom level (relative to this
        control's size). */
    eC_Value m_vMinScale;

    /** The scale multiplier for the 'static' zoom level (relative to this
        control's size). */
    eC_Value m_vStaticScale;

    /// Starting position of last drag event in absolute coordinates
    eC_Value m_vDragStartY;
};

#endif // GUI_IMAGE_STACK_H
