/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUISLIDER__H_
#define GUISLIDER__H_

#include "GUIBaseSlider.h"
#include "GUIImageResource.h"
#include "GUINinePatch.h"

/// Slider specialization that uses images for graphical representation.
// @guiliani_doxygen toplevel_control Slider
/**
<table border="0">
<tr>
<td width="200">@image html slider.png</td>
<td>
The "slider" control is used for setting and representing of values as a knob on a bar. It supports horizontal and vertical orientation of the bar.
The minimum value is represented by the left most or bottom position of the knob. (Class: CGUISlider).
The "slider" supports nine patch images (CGUINinePatch).
</td>
</tr>
</table>
*/
// @endguiliani_doxygen
/**
The image based slider has a knob that may be dragged inside the slider
track (a specific area inside the control) into one direction that is
determined by the orientation of the control (horizontal or vertical).
The position of the knob determines the associated value of the control or,
to be more precise, the value of the associated CGUIRange.

The knob is an image that is drawn centered (relative to the opposite
direction of the control). Its size may be determined by the user calling
SetKnobSizeHorizontal() and SetKnobSizeVertical()), so that it is
automatically stretched if needed. Stretching behaviour can be customized
via the knob's CGUINinePatch.

The background is an image which will be stretched into the direction of the
slider, and centered in the opposite direction. E.g. a horizontal slider's
background will be stretched horizontally.and centered vertically.

Example:
@code

// Create horizontal slider.
CGUISlider* pkSliderHor = new CGUISlider(this,
eC_FromInt(0), eC_FromInt(0),
eC_FromInt(300), eC_FromInt(20),
0, 0, 100, 10,
CGUIRange::BASE_AT_MINIMUM,
CGUIBaseSlider::SLD_HORIZONTAL,
OBJ_HORIZONTALSLIDER);

// Set images of the slider.
// Before calling this method, please add image IDs to UserImageResource.h.
// After that call GETRESMANAGER.RegisterImageResource() to register images for image IDs.
pkSliderHor->SetImages(
IMG_SLD_BG,
IMG_KNOB_NORMAL,
IMG_KNOB_HIGHLIGHTED,
IMG_KNOB_PRESSED);

// Sets the distance of background to the border of the control.
pkSliderHor->SetBackgroundMargin( eC_FromInt(10));

@endcode

@ingroup GUILIANI_CONTROLS
*/
class CGUISlider :
    public CGUIBaseSlider
{
public:
    /** Constructor
    Please use the SetImages method to set the correct images.
    @param pParent Pointer to the designated parent object
    @param vX X-position relative to its parent object
    @param vY Y-position relative to its parent object
    @param vWidth Width of the slider
    @param vHeight Height of the slider
    @param iValue Current value of the slider
    @param iMin Minimum setting of the slider
    @param iMax Maximum setting of the slider
    @param uiStepSize The step size in units of the range
    @param eBase Steps are based at minimum or maximum.
    @param eStyle Orientation of control
    @param eID Object identifier of this slider (chose NO_HANDLE if none
    is required) */
    CGUISlider(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const eC_Int &iValue,
        const eC_Int &iMin,
        const eC_Int &iMax,
        const eC_UInt &uiStepSize,
        const CGUIRange::Base_t &eBase = CGUIRange::BASE_AT_MINIMUM,
        const SliderStyles_t &eStyle = SLD_HORIZONTAL,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructor
    Please use the SetImages method to set the correct images.
    @param pParent Pointer to the designated parent object
    @param kRect The boundary rectangle, relative to parent.
    @param iValue Current value of the slider
    @param iMin Minimum setting of the slider
    @param iMax Maximum setting of the slider
    @param uiStepSize The step size in units of the range
    @param eBase Steps are based at minimum or maximum.
    @param eStyle Orientation of control
    @param eID Object identifier of this slider.
    */
    CGUISlider(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const eC_Int &iValue,
        const eC_Int &iMin,
        const eC_Int &iMax,
        const eC_UInt &uiStepSize,
        const CGUIRange::Base_t &eBase = CGUIRange::BASE_AT_MINIMUM,
        const SliderStyles_t &eStyle = SLD_HORIZONTAL,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Copy-constructor of the CGUIBaseSlider class.
    @param kSource Source object to be copied. */
    CGUISlider(const CGUISlider& kSource);

    /** Operator= method of the CGUISlider class.
    @param kSource Source object to be copied.
    @return Copied object. */
    CGUISlider& operator=(const CGUISlider& kSource);

    /// Destructor
    virtual ~CGUISlider();

#ifdef GUILIANI_STREAM_GUI
    /** Standard constructor. Only to be called by factory.
    No user code should call this constructor, not even in
    streaming mode (that is, when GUILIANI_STREAM_GUI is defined)!
    @see CGUIBaseSlider() */
    CGUISlider();

    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

    /** SetImages
    Method to change the images used for the slider representation.
    The sizes of the knob images may differ.
    If the knob size varies from the supplied image's size, the image will automatically split into
    three portions. These are either a left/middle/right part for horizontal, or a top/middle/bottom part
    for vertical sliders. The left/top images will be constructed from the left/top half of the original
    image. Analogue, The right/bottom images will be constructor from the right/bottom half of the original image.
    The pixel row/column in the middle of the original image will be stretched to fill up the space, if the knob
    is to be displayed bigger than the original image.
    @param eSldBG ID of background image
    @param eSldKnobNormal ID of image used to represent the normal state of the slider
    @param eSldKnobHighlighted ID of image used to represent the highlighted slider
    @param eSldKnobPressed ID of image used to represent the pressed slider
    @param eSldKnobGrayedOut ID of image used to represent the grayed out slider (compat: set to DUMMY_IMAGE) */
    void SetImages(
        const ImageResource_t &eSldBG, const ImageResource_t &eSldKnobNormal,
        const ImageResource_t &eSldKnobHighlighted, const ImageResource_t &eSldKnobPressed,
        const ImageResource_t &eSldKnobGrayedOut = DUMMY_IMAGE);

    eC_Bool DoDraw();

    /// Sets the distance of background to the border of the control. This may be used
    /// to align the middle of the knob to the visible ends of the background (in the
    /// scrolling direction) without using transparent background graphics and
    /// still dragging the knob completely visible all the time (no clipping at
    /// the control's borders), for example.
    /// @param vBkgndMargin distance of background to the border of the control in pixel
    inline void SetBackgroundMargin(const eC_Value &vBkgndMargin)
    {
        m_vBkgndMargin = vBkgndMargin;
    }

    /// Shrinks the background image so that its borders lie under the
    /// middle of the knob at the knob's extreme positions if the track
    /// distance is zero.
    inline void AdjustBackgroundMarginToMiddleOfKnob()
    {
        if (m_eStyle == SLD_VERTICAL)
        {
            m_vBkgndMargin = eC_Div(GetKnobSizeVertical(), eC_FromInt(2));
        }
        else
        {
            m_vBkgndMargin = eC_Div(GetKnobSizeHorizontal(), eC_FromInt(2));
        }
    }

    /** Grants access to the object's nine patch.
    @return Nine patch used to display the object */
    CGUINinePatch& GetNinePatch()
    {
        return m_kNinePatch;
    }

protected:
#ifndef GUILIANI_STREAM_GUI
    /// Only called by initialization list of the standard constructor.
    /// No custom user code should call this, therefore it is protected.
    CGUISlider();
#endif

    ImageResource_t m_eSldBG;              ///< Slider background

    ImageResource_t m_eSldKnobNormal;      ///< Slider knob normal state
    ImageResource_t m_eSldKnobHighlighted; ///< Slider knob highlighted
    ImageResource_t m_eSldKnobPressed;     ///< Slider knob pressed
    ImageResource_t m_eSldKnobGrayedOut;   ///< SLider knob grayed out

    CGUINinePatch   m_kNinePatch; ///< Nine Patch for the slider knob

private:
    /** Set the stardard Guiliani images to this slider. */
    void SetStandardImages();

    /** Init method for CGUISlider. Sets the dummy images. */
    inline void Init();

    /** Copy attributes method of the CGUISlider class.
    @param kSource Source object to be copied.
    @return Copied object. */
    inline void CopyAttributes(const CGUISlider& kSource);

    /// Helper for destructor and ReadFromStream.
    void DeInit();

    /// @see SetBackgroundMargin()
    /// Distance of background to the border of the control. This may be used
    /// to align the middle of the knob to the ends of the background (in the
    /// scrolling direction) without using transparent background graphics and
    /// still dragging the knob completely visible all the time (no clipping at
    /// the control's borders), for example.
    eC_Value m_vBkgndMargin;
};
#endif
