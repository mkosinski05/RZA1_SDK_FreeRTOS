/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIBASESLIDER__H_
#define GUIBASESLIDER__H_

#include "GUIAbstractBar.h"
#include "GUIRange.h"
#include "GUIPoint.h"

/// Slider base class.

/** Like all the "BaseXXXXX"-Classes, this serves primarily as base-class, from
    which you can derive your own customized objects.
    For a slider implementation which uses images to visualize itself, please refer
    to CGUISlider.
      
    This class implements vector graphics based sliders and serves as a base
    class for other types of sliders. Vertically orientated sliders represent
    their maximum when the knob is at the topmost valid position. Horizontally
    ones represent their maximum when the knob is to the rightmost valid
    position. The same applies to the level of the associated range, for
    example, when the knob is at the topmost valid position of a vertical
    slider then the level is the highest valid one.

    Base sliders have an area where the knob slides over when it is dragged,
    called slider track. The size of the slider track can be adjusted by
    calling SetSliderTrackDistance(). For base sliders the track distance
    influences the background graphics look, which is not true for image
    based GUISliders for example.

    The knob is drawn by filled circles and the background graphics looks
    like the character H (a combination of 3 lines) that is stretched in the
    middle part. The left and right line on both sides of the character H lies
    under the middle of the knob at the slider knob's minimum and maximum
    positions respectively.

    Example:
@code

    // Create horizontal slider.
    CGUIBaseSlider* pkBaseSliderHor = new CGUIBaseSlider(this,
            eC_FromInt(0), eC_FromInt(0),
            eC_FromInt(500), eC_FromInt(30),
            50, 0, 100, 1,
            CGUIRange::BASE_AT_MINIMUM,
            CGUIBaseSlider::SLD_HORIZONTAL,
            OBJ_HORIZONTALSLIDER);

     // Get the length of the slider track in pixel
    eC_Value vTrackLength = pkBaseSliderHor->GetSliderTrackLength();

    // Set the value of the slider.
    pkBaseSliderHor->SetValue((eC_Int)100);

@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIBaseSlider :
    public CGUIAbstractBar
{
public:
    friend class CGUIScrollBar;

    /// Enumeration for slider styles
    enum SliderStyles_t
    {
        SLD_HORIZONTAL, ///< x-axis movement
        SLD_VERTICAL ///< y-axis movement
    };

    /** Constructor
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
        @param eStyle Style descriptor. Valid values are defined in CGUIBaseSlider::SliderStyles_t
        @param eID Object identifier
     */
    CGUIBaseSlider(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const eC_Int &iValue,
        const eC_Int &iMin, const eC_Int &iMax,
        const eC_UInt &uiStepSize,
        const CGUIRange::Base_t &eBase = CGUIRange::BASE_AT_MINIMUM,
        const SliderStyles_t &eStyle = SLD_HORIZONTAL,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructor
        @param pParent Pointer to the designated parent object
        @param kRect A CGUIRect describing the slider 
        @param iValue Current value of the slider
        @param iMin Minimum setting of the slider
        @param iMax Maximum setting of the slider
        @param uiStepSize The step size in units of the range
        @param eBase Steps are based at minimum or maximum.
        @param eStyle Style descriptor. Valid values are defined in CGUIBaseSlider::SliderStyles_t
        @param eID Object identifier
     */
    CGUIBaseSlider(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const eC_Int &iValue,
        const eC_Int &iMin, const eC_Int &iMax,
        const eC_UInt &uiStepSize,
        const CGUIRange::Base_t &eBase = CGUIRange::BASE_AT_MINIMUM,
        const SliderStyles_t &eStyle = SLD_HORIZONTAL,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Copy-constructor of the CGUIBaseSlider class.
        @param kSource Source object to be copied to a newly created object.
    */
    CGUIBaseSlider(const CGUIBaseSlider& kSource);

    /** Assignment operator of the CGUIBaseSlider class.
        @param kSource Source object to be copied to an already existing object.
        @return Copied object.
    */
    CGUIBaseSlider& operator=(const CGUIBaseSlider& kSource);

#ifdef GUILIANI_STREAM_GUI
    /** Standard constructor. Only to be called by factory.
        No user code should call this constructor, not even in
        streaming mode (that is, when GUILIANI_STREAM_GUI is defined)!
        @see CGUIAbstractBar()
    */
    CGUIBaseSlider();

    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

    /** Destructor */
    virtual ~CGUIBaseSlider();

    eC_Bool DoDraw();

    /** Action to perform in case of a click. This will move the slider directly to the coordinates of
        the click.
        @copydoc CGUIBehaviour::DoClick
    */
    virtual eC_Bool DoClick(const eC_Value &vAbsX, const eC_Value &vAbsY);

    /// Drags the slider along the mouse movement.
    virtual eC_Bool DoDrag(const eC_Value &vDeltaX, const eC_Value &vDeltaY, const eC_Value &vAbsX, const eC_Value &vAbsY);

    /// Action performed when the drag is finished over any other control.
    virtual eC_Bool DoDragEnd(const eC_Value &vAbsX, const eC_Value &vAbsY);

    /** Keeps the difference between real click point and middle of knob.
        @param  vAbsX X-position in screen coordinates where the click occurred
        @param  vAbsY Y-position in screen coordinates where the click occurred
        @return True The event has been handled.<BR>
                False The event has not been handled.
    */
    virtual eC_Bool DoButtonDown(const eC_Value &vAbsX, const eC_Value &vAbsY);

    /// Overridden to automatically update the slider's look.
    virtual void SetWidth(const eC_Value& vX)
    {
        CGUIAbstractBar::SetWidth(vX);
        if (m_eStyle == SLD_HORIZONTAL)
            // update the pre-calculated sliding area bounds (called "slider track")
            SetSliderTrackDistance(m_vSliderTrackDistance);
    }

    /// Overridden to automatically update the slider's look.
    virtual void SetHeight(const eC_Value& vY)
    {
        CGUIAbstractBar::SetHeight(vY);
        if (m_eStyle == SLD_VERTICAL)
            // update the pre-calculated sliding area bounds (called "slider track")
            SetSliderTrackDistance(m_vSliderTrackDistance);
    }

    /** Sets the pixel distance between the slider's border and the slider track (=sliding area)
        This automatically updates the slider's knob mininmum and maximum positions.
        @param vPixel The distance in pixel between the slider track
            and the control's borders in the direction of the orientation
            (horizontal or vertical).
            The same distance applies to both borders. Positive values shrink
            down the sliding area of the slider, also called slider track, but
            do not alter the background graphics of image based GUISliders.
            Background graphics of base sliders are altered by the track
            distance. */
    void SetSliderTrackDistance(const eC_Value vPixel);

    /** Returns the length of the slider's track.
        This value is determined by the slider track distance and the size
        of the slider control.
        @return the length of the slider track in pixel
        @see SetSliderTrackDistance() */
    inline eC_Value GetSliderTrackLength() const
    {
        return eC_Abs(m_vSliderMaxPos - m_vSliderMinPos);
    }

    /// @return the vertical or horizontal orientation
    inline SliderStyles_t GetOrientation() const
    {
        return m_eStyle;
    }


#ifndef GUILIANI_STREAM_GUI
    /// Only called by initialization list of the standard constructor.
    /// No custom user code should call this, therefore it is protected.
    CGUIBaseSlider();
#endif

    /// @return The vertical size of the knob
    inline eC_Value GetKnobSizeVertical() const
    {
        return m_vKnobSizeVertical;
    }

    /// @return The horizontal size of the knob
    inline eC_Value GetKnobSizeHorizontal() const
    {
        return m_vKnobSizeHorizontal;
    }

    /** Sets the knob size in pixels. (Sets identical size for width and height)
        @note The knob size and the slider track distance determine the size
              of the slider track.
        @param vKnobSize Knob size in pixels
    */
    inline void SetKnobSize(const eC_Value &vKnobSize)
    {
        SetKnobSizeHorizontal(vKnobSize);
        SetKnobSizeVertical(vKnobSize);
    }

    /** Sets the horizontal knob size in pixels.
        @param vKnobSize Knob size in pixels
    */
    void SetKnobSizeHorizontal(const eC_Value &vKnobSize);

    /** Sets the vertical knob size in pixels.
        @param vKnobSize Knob size in pixels
    */
    void SetKnobSizeVertical(const eC_Value &vKnobSize);

    /** Sets the slider's knob position when it represents the maximum value.
        @param vSliderMaxPos Relative pixel offset within the slider
    */
    void SetSliderMaxPos( const eC_Value vSliderMaxPos);

    /** Sets the slider's knob position when it represents the minimum value.
        @param vSliderMinPos Relative pixel offset within the slider
    */
    void SetSliderMinPos( const eC_Value vSliderMinPos);

    /** Calculates the area covered by the slider knob in coordinates relative to the slider.
        @return Rectangle covered by slider knob in relative coordinates  */
    virtual CGUIRect CalculateKnobRect();

    /** Sets the minimum allowed knob size (in direction of slider orientation) in pixels.
        @param vMinimumKnobSize Minimum knob size in pixels */
    void SetMinimumKnobSize( const eC_Value vMinimumKnobSize)
    {
        m_vMinimumKnobSize = vMinimumKnobSize;
        SetKnobSizeHorizontal( m_vKnobSizeHorizontal);
        SetKnobSizeVertical( m_vKnobSizeVertical);
    }

protected:

    /** Clips the passed value to the pixel bounds to which the range is associated.
        The value is given in pixel units not as a value of the range.
        @param vPos value to be clipped at min/max pos
    */
    inline void ClipToPixelBounds(eC_Value &vPos) const
    {
        // clip to bounds
        if (vPos < m_vSliderMinPos)
            vPos = m_vSliderMinPos;
        else if (vPos > m_vSliderMaxPos)
            vPos = m_vSliderMaxPos;
    }

    /** DoDrag() checks if the slider graphics is dragged or the rest of the
        control is dragged and calls this function or not, which does the
        real work. Thus, CGUISlider need not implement this function again
        but may do and really does a different check in the current
        implementation.
        @param vAbsX X-position of the dragging point in screen coordinates
        @param vAbsY Y-position of the dragging point in screen coordinates
    */
    void SetSliderPos(const eC_Value &vAbsX, const eC_Value &vAbsY);

    /** Orientation of this slider object
        Attention: it can only be changed in the constructor and the
        ReadFromStream() method in this class. */
    SliderStyles_t m_eStyle;

    /// Still holding the mouse button while dragging?
    eC_Bool m_bDraggingMode;

    /// Relative point that is beeing dragged
    CGUIPoint m_kRelDraggingPoint;

    /** Absolute rectangle of knob. This is used to communicate the absolute
        rectangle of the knob from DoDraw() to DoClick() and DoDrag(). */
    CGUIRect m_kKnobAbsRect;

    ///@{
    /** Bounds for slider track, to gain some space to the min and max borders
        of the control. These values are relative to the control and fall in
        the range of [0 .. GetWidth() or GetHeight()].
        These variables shall only be altered by SetSliderTrackDistance(), that is
        they should be treated read-only in derived classes. */
    eC_Value m_vSliderMaxPos, m_vSliderMinPos;
    ///@}

    /** Size of knob in horizontal direction of the control in pixel.
        This value is set via SetKnobSizeHorizontal() in GUISlider and
        GUIScrollBar for example. */
    eC_Value m_vKnobSizeHorizontal;

    /** Size of knob in vertical direction of the control in pixel.
        This value is set via SetKnobSizeVertical() in GUISlider and
        GUIScrollBar for example. */
    eC_Value m_vKnobSizeVertical;

    /** Copy attributes method of the CGUIBaseSlider class.
        @param kSource Source object to be copied.
    */
    inline void CopyAttributes(const CGUIBaseSlider& kSource);

    /** Gets the value of the associated range of the control corresponding
        to the given position. Attention: This method may only be called at
        times when absolute positions are available (DoDraw, DoClick, DoDrag
        etc.)
        @param vAbsX X-position in screen coordinates
        @param vAbsY Y-position in screen coordinates
        @return The slider value
    */
    inline eC_Int GetSliderValue(const eC_Value &vAbsX, const eC_Value &vAbsY) const;

    /** Helper function for constructors.
        @param iValue Slider value
        @param iMin Min slider value
        @param iMax Max slider value
        @param uiStepSize Step size
        @param eBase Slider style
    */
    inline void Init(
        const eC_Int &iValue,
        const eC_Int &iMin,
        const eC_Int &iMax,
        const eC_UInt &uiStepSize,
        const CGUIRange::Base_t &eBase);

    /** @see SetSliderTrackDistance
        This variable shall only be altered by SetSliderTrackDistance(). */
    eC_Value m_vSliderTrackDistance;

    /** Keeps the difference between knob middle and mouse position.
        Used in DoDrag() and DoButtonDown() */
    eC_Value m_vDiffKnobMouse;

    /// Minimum allowed knob size (in direction of slider orientation) in pixels (Defaults to 10)
    eC_Value m_vMinimumKnobSize;
};
#endif
