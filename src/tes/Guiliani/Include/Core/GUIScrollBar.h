/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUISCROLLBAR__H_
#define __GUISCROLLBAR__H_

#include "GUICompositeObject.h"
#include "GUISlider.h"
#include "GUIRange.h"
#include "GUIObserver.h"
#include "GUIException.h"

class CGUIButton;

/// The ScrollBar allows navigating controls which are larger than their actual visual representation.

/**
    A GUIScrollBar is a CompositeObject which consists of two CGUIButton objects and a CGUISlider. It can be
    attached to a CompositeObject. If this composite object has children that lie outside of its
    visual area, the ScrollBar will automatically initialize itself to a scrolling range which is
    sufficient to make <i>all</i> of the composite object's children visible. You can either use the slider or
    the two buttons (which will act as up and down arrow) to navigate the visible area of the
    CompositeObject. If you click a button and hold it, the scrollbar will start an animation and
    scrolls until the border is reached or the button is released.

    The only object which is scrolled by this class is the one passed to SetScrolledObject().

    Be aware that you have access to all child objects of this control. If you
    don't use this feature carefully, you can produce strange behaviours.

    Example:
@code
    // Creates the object which is going to be scrolled using the ScrollBar.
    CGUIImage* pObject = new CGUIImage( &GETGUI,
                                eC_FromInt(0), eC_FromInt(0),
                                eC_Mul( eC_FromInt(2), GETGUI.GetWidth()), GETGUI.GetHeight(),
                                IMG_STDCTRL_PLACEHOLDER);
    // Create a scroll bar.
    CGUIScrollBar* pkScrollBar = new CGUIScrollBar( &GETGUI, NULL, 1,
                                        CGUIRange::BASE_AT_MINIMUM,
                                        CGUIBaseSlider::SLD_HORIZONTAL,
                                        NO_HANDLE);
    // Link ScrollBar to scrolled object
    pkScrollBar->SetScrolledObject(pObject);
@endcode

*/
class CGUIScrollBar
    :public CGUICompositeObject
    ,public CGUIObserver
{
public:
    /** Constructor
        This attaches the ScrollBar as a child to the given CompositeObject. The ScrollBar will automatically
        calculate its size and position with regard to the specified style and its scrolled object.
        @param pParent The scroll bar's parent object.
        @param pScrolledObject Pointer to the CompositeObject which is to be scrolled.
        @param uiStepSize The step size in units of the range.
        @param eBase Steps are based at minimum or maximum.
        @param eStyle Specifies whether this scroll bar will be horizontal or vertical.
        @param eID Object handle of this scroll bar.
      */
    CGUIScrollBar(
        CGUICompositeObject *const pParent,
        CGUIObject *const pScrolledObject,
        const eC_UInt &uiStepSize = 1,
        const CGUIRange::Base_t &eBase = CGUIRange::BASE_AT_MINIMUM,
        const CGUISlider::SliderStyles_t &eStyle = CGUISlider::SLD_HORIZONTAL,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Destructor */
    virtual ~CGUIScrollBar();

    /** Handles the button clicks by increasing or decreasing the slider's value.
        @param  vAbsX X-position in screen coordinates where the click occurred
        @param  vAbsY Y-position in screen coordinates where the click occurred
        @return True The event has been handled.<BR>
                False The event has not been handled. In this base
                implementation it always returns False.
    */
    eC_Bool DoClick(const eC_Value &vAbsX, const eC_Value &vAbsY);

    /** Calling this method causes the scrollbar to recalculate its scrolling range with regard
        to the position/size of the scrolled object in relation to the object against which it is clipped,
        You should usually call this method after the clipper or scrolled object's size has changed.
        @param vCLipperSize Size of the clipper object in scrolling direction.
        */
    void RecalculateScrollingRange(const eC_Value vCLipperSize);

    /** Sets the object which is going to be scrolled using this ScrollBar.
        @param pObject  Pointer to object to scroll
        */
    inline void SetScrolledObject( CGUIObject* pObject) { m_pScrolledObject = pObject; };

    /** Returns the object which is being scrolled using this ScrollBar.
        @return Pointer to object, which is being scrolled using this ScrollBar
        */
    inline CGUIObject* GetScrolledObject() const { return m_pScrolledObject; }

    /** Sets the value on the scrollbar's slider, thus scrolling to the respective position.
        @param rkValue The new value
        @return Always True.
    */
    inline eC_Bool SetValue( const CGUIValue& rkValue)
    {
        if (m_pSlider)
            m_pSlider->SetValue(rkValue.ToInt());
        else
            CGUICompositeObject::SetValue(rkValue);
        return true;
    }

    /** Returns the pointer to slider object.
        @return Pointer to slider object.
        */
    inline CGUISlider* GetSlider() const { return m_pSlider; }

    /** Returns the pointer to the up or left button depending on the orientation.
        @return Pointer to the button object.
        */
    inline CGUIButton* GetUpLeftButton() const { return m_pUpLeftButton; }

    /** Returns the pointer to the down or right button depending on the orientation.
        @return Pointer to the button object.
        */
    inline CGUIButton* GetDownRightButton() const { return m_pDownRightButton; }

    /** Returns the range of internal slider object.
        @return Range of internal slider object.
        */
    inline const CGUIRange& GetRange() const
    {
        if (!m_pSlider)
            GUILOG_THROW_EXCEPTION(
                CGUIException(),
                "CGUIScrollBar::GetRange: Slider pointer is null!\n");

        return m_pSlider->GetRange();
    }

    /** Will be called if the value of the slider object changes.
        @param kObservedValue New slider value.
        @param pkUpdatedObject Pointer to the slider object which changed.
        @param uiX X-Index Additional X-Index in case the updated value is part of a multidimensional array
        @param uiY Y-Index Additional Y-Index in case the updated value is part of a multidimensional array
    */
    void OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX=0, const eC_UInt uiY=0);

    /** Handles the scroll animation if the user keeps pressing on one of the scrollbar's buttons
        @param vTimes The correction factor
    */
    virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1));

    /** Sets scroll interval for auto scroll.
        This is the interval in milliseconds after which the scrollbar changes its value,
        while the user keeps pressing one of the scrollbar's button.
        @param ruiScrollInterval Interval in ms.
        */
    inline void SetScrollInterval(const eC_UInt &ruiScrollInterval)
        { m_uiScrollInterval = ruiScrollInterval; }

    /** Returns the scroll interval for auto scroll.
        @return Scroll interval in ms.
        */
    inline eC_UInt GetScrollInterval() const { return m_uiScrollInterval; }

    /** Sets number of pixels to scroll per step.
        @param ruiScrollSpeed Number of pixels to scroll per step.
        */
    inline void SetScrollSpeed(const eC_UInt &ruiScrollSpeed) const
    { if (m_pSlider) m_pSlider->SetAnimationSpeed(ruiScrollSpeed); }

    /** Returns number of pixels to scroll per step, or 0 if the internal slider object does not exist
        @return Step size or 0.
        */
    inline eC_UInt GetScrollSpeed() const
    {
        if (m_pSlider)
            return m_pSlider->GetAnimationSpeed();
        else
            return 0;
    }

    /** Shows or hides the buttons depending on given parameter.
        @param bHide True to hide buttons.
        */
    void HideButtons(const eC_Bool &bHide);

    /** Calls base implementation and makes sure that if any of the specific
        children is removed, its pointer is set to NULL.
        @param pObject Child object that is removed.
      */
    virtual void RemoveObject(CGUIObject* pObject);

#ifdef GUILIANI_STREAM_GUI
    /** Standard constructor. Only to be called by factory.
        No user code should call this constructor, not even in
        streaming mode (that is, when GUILIANI_STREAM_GUI is defined)!
        @see CGUISlider() */
    CGUIScrollBar() { Init(); }

    virtual void ReadFromStream();

#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    /** Implemented to start scroll animation.
        @param  vAbsX X-position in screen coordinates where the click occurred
        @param  vAbsY Y-position in screen coordinates where the click occurred
        @return True The event has been handled.<BR>
                False The event has not been handled.
    */
    eC_Bool DoButtonDown(const eC_Value& vAbsX, const eC_Value& vAbsY);

    /** Implemented to stop scroll animation.
        @param  vAbsX X-position in screen coordinates where the click occurred
        @param  vAbsY Y-position in screen coordinates where the click occurred
        @return True The event has been handled.<BR>
                False The event has not been handled.
    */
    eC_Bool DoDragEnd(const eC_Value& vAbsX, const eC_Value& vAbsY);

private:
    /// Initalizes the members.
    void Init();
    /// Deinitalizes the members.
    void DeInit();
    /// Adapts the knob's size depending on the relative size of the visible are compared to the total area
    void AdaptKnobSize();
    /// Adapts the slider's size and position depending on the size of the buttons.
    void AdaptSliderSize();

    eC_UInt     m_uiScrollInterval;     ///< scroll interval in milliseconds when pressing the buttons
    eC_Bool     m_bScrollDownRight;     ///< Is the animation in scroll "down or right" mode.
    eC_Int      m_iOldValue;            ///< Keeps the last set value.
    CGUIObject* m_pScrolledObject;      ///< The pointer to the scrolled object.
    CGUISlider* m_pSlider;              ///< The slider.
    CGUIButton* m_pUpLeftButton;        ///< The up or left button.
    CGUIButton* m_pDownRightButton;     ///< The down or right button.
};
#endif
