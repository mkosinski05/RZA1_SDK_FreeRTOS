/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUI_TOUCH_SCROLL_VIEW_H)
#define GUI_TOUCH_SCROLL_VIEW_H

// Touchscreen-optimized scrollview
#include "GUIScrollView.h"
#include "SafeGUIObjectPtr.h"
#include "GUIEasing.h"

// @guiliani_doxygen toplevel_control Touch Scroll View
/**
    <table border="0">
        <tr>
            <td width="200">@image html touch_scroll_view.png</td>
            <td>
                The "touch scroll view" is a specialized scroll view which is optimized for touch screens and supports scrolling by dragging as well as kinetic effects (Class: CGUITouchScrollView).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    When dragging, the content of the scroll view follows the finger/mouse.
    Besides that it is possible to activate <i>kinetic scrolling</i> which will trigger a scroll animation after the drag, that slowly fades out with time.
    The animation speed depends on the drag speed, which means that quick fling gestures will result in faster scrolling.
    Optionally it is possible to activate a bounce back factor, which will invert the animation direction when the edge of the scroll view is reached.

    It is also possible to define a destination position (relative to the scroll view).
    When this is done the scroll view will always choose one object which is the 'active object' and align its center to this position.
    The destination position is only defined for one direction. Depending on the destination direction (horizontal or vertical) the target position
    is used for the XPosCenter or the YPosCenter of the active object.
    When clicking on the scroll view, the nearest object to the click position (in destination direction) is chosen as the active object.
    When dragging, the nearest object to the target position is chosen as active object when the drag is finished.
    When kinetic scrolling is activated, the nearest object to the target position is chosen when the kinetic animation is about to fade out.

@section sec_cguitouchscrollview_faq Frequently Asked Questions:
    <b>Why does the animation speed shortly increase again when the animation is about to fade out?</b><br>
    If a destination position is given the framework tries to make sure that at the end of animation a valid position is reached. It therefore automatically
    animates towards the destination position if the animation speed drops below a minimum threshold.
    <p>
    <b>Why does my CGUITouchScrollView immediately start scrolling upon entering the dialog?</b><br>
    If an object within the CGUITouchScrollView receives the focus, the scrollview will try to scroll this object to the destination position. Could it be
    that the focus (either on purpose, or by default since no explicit focus was set) got moved into the CGUITouchScrollView?
    <p>
    <b>I set a destination position, but the first/last child within the CGUITouchScrollView is never scrolled to this position.</b><br>
    If the first child object of the CGUITouchScrollView's internal container starts at the very beginning (e.g. at position "0") then the scrollview may not
    be able to scroll it to your desired destination position, since its scrolling range is limited by the size/position of the container object.
    This can be resolved be leaving some extra space before/after the child objects, e.g. by placing an empty placeholder object before/after them.
    <p>
    <b>I have placed composite objects into the CGUITouchScrollView, which contain further child objects. Now I wish to center the composite-objects on the
    defined destination position, but NOT the child objects themselves. How can I do this?</b><br>
    This is done via the SetScrollToImmediateChildrenOnly() API. This ensures that only direct children of the CGUITouchScrollView's container will be
    used for scrolling. Descendants deeper in the hierarchy will not be taken into account.
    <p>
    <p>

Example:
@code
// Create a touch scrollview.
CGUITouchScrollView* pkTouchScrollView = new CGUITouchScrollView(this,
    eC_FromInt(0), eC_FromInt(0),
    eC_FromInt(200), eC_FromInt(300));

// activate kinetic scrolling in vertical direction.
pkTouchScrollView->SetKineticScrolling(true, false);

// activate "bounce back" when reaching the end of the scrollarea
pkTouchScrollView->SetBounceBackFactor(eC_FromFloat(0.5));

// center the active object vertically in the center of the view.
pkTouchScrollView->SetDestination(CGUITouchScrollView::DIRECTION_VERTICALLY, pkTouchScrollView->GetHalfHeight());

// Add some children
for (eC_UInt i= 0; i< 10; i++)
{
    new CGUIButton(pkTouchScrollView, eC_FromInt(40), eC_Mul(eC_FromInt(i), eC_FromInt(50)), eC_FromInt(60), eC_FromInt(20), "Button" + eC_String(i), NULL);
}

@endcode

    @ingroup GUILIANI_CONTROLS
    @brief This ScrollView implementation is optimized for touch screens and supports scrolling by dragging.
*/
class CGUITouchScrollView
    : public CGUIScrollView
{
public:
    /// Constructor.
    CGUITouchScrollView(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const ObjectHandle_t &eID = NO_HANDLE);

#ifdef GUILIANI_STREAM_GUI
    /// Default constructor for streaming.
    CGUITouchScrollView();
#endif

    /** Enable / disable kinetic scrolling.
    @param bVertical True to enable vertical kinetic scrolling, else False.
    @param bHorizontal True to enable horizontal kinetic scrolling, else False.
    */
    void SetKineticScrolling(const eC_Bool bVertical, const eC_Bool bHorizontal)
    {
        m_bKineticScrollingVertical = bVertical; m_bKineticScrollingHorizontal = bHorizontal;
    }

    /**
        Determines if kinetic scrolling is activated for vertical scrolling.
        @see SetKineticScrolling()
        @return True if flag set, False otherwise
    */
    eC_Bool HasKineticScrollingVertical() const { return m_bKineticScrollingVertical;}

    /**
        Determines if kinetic scrolling is activated for horizontal scrolling.
        @see SetKineticScrolling()
        @return True if flag set, False otherwise
    */
    eC_Bool HasKineticScrollingHorizontal() const { return m_bKineticScrollingHorizontal;}

    /// Returns the index of the active object in the childlist or -1 if the active object is NULL.
    eC_Int GetElementIndexOfActiveObject() const;

    /// Direction enumeration used for scrolling, dragging and destination direction.
    enum Direction_t
    {
        DIRECTION_NONE,
        DIRECTION_HORIZONTALLY,
        DIRECTION_VERTICALLY,
    };

    /**
    Set a destination for the active object.
    @param eDestinationDirection Direction to decide if the position is used as X-coordinate for horizontal scrolling or as Y-coordinate for vertical scrolling.
    Use DIRECTION_NONE to deactivate the target position.
    @param vRelDestinationPos The relative destination position to scroll the active object to.
    */
    void SetDestination(const Direction_t eDestinationDirection, const eC_Value vRelDestinationPos = eC_FromInt(-1))
    {
        m_eDestinationDirection = eDestinationDirection;
        m_vDestination = vRelDestinationPos;
    }

    /// Returns the destination direction. The destination direction determines if the target position is used for horizontal or vertical scrolling.
    Direction_t GetDestinationDirection() const { return m_eDestinationDirection;}

    /// Returns the relative target position or -1 if no destination position is set.
    eC_Value GetDestinationPosition() const {return m_vDestination;}

    /// Returns the BounceBackFactor which is used when an edge of the scroll view is reached during an animation.
    eC_Value GetBounceBackFactor() const {return m_vBounceBackFactor;}

    /**
    Set the Bounceback factor.
    Sensible values lie between 0 and 1. Set to 0 if no bounce back shall be done and to 1 if a full bounce back shall be done.
    Technically, the scroll speed is multiplied by the negative bounce back factor when an edge
    of the scroll view is reached during an animation.
    @param vBounceBackFactor The bounce back factor for the animation speed.
    */
    void SetBounceBackFactor (const eC_Value vBounceBackFactor) {m_vBounceBackFactor = vBounceBackFactor;}

    /**
        Determines if only direct children or all descendants are checked when searching the closest object.
        @see SetScrollToImmediateChildrenOnly()
        @return True if flag set, False otherwise
    */
    eC_Bool ScrollsToImmediateChildrenOnly() const {return m_bScrollToImmediateChildrenOnly;}

    /**
    If a target position is set then the scroll view will always center an object on this target position.
    It will always choose the closest object to the target position in case of dragging or kinetic scroling, or to the click position in case of a click.
    This parameter determines if only direct children of the scroll view (=children whose parent is the container, or one of its siblings) are checked when 
    searching the closest object or all descendants.
    @param bOnlyImmediateChildren If true only direct children are checked, if false all descendants are checked.
    */
    void SetScrollToImmediateChildrenOnly(const eC_Bool bOnlyImmediateChildren) {m_bScrollToImmediateChildrenOnly = bOnlyImmediateChildren;}

    /** Sets the object to the target position.
    This does not trigger a scrolling animation.
    The position is set directly.
    @param pObjectToScrollTo The object to scroll to.
    @see ScrollToObjectAnimated.
    */
    virtual void ScrollToObject( CGUIObject* pObjectToScrollTo);

    /** Scrolls the object to the target position by starting an animation.
        The animation speed depends on the inertia.
    @param pObjectToScrollTo The object to scroll to.
    */
    virtual void ScrollToObjectAnimated( CGUIObject* pObjectToScrollTo);

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

    /// Returns the currently active object. The active Object is NULL while a kinetic scrolling animation is ongoing.
    CGUIObject* GetActiveObject() const {return m_pActiveObject;}

    /// Returns the destination position in absolute coordinates or -1 if no target position or no target direction was set.
    eC_Value GetAbsDestinationPos() const;

    /**
    Search the closest focusable child to an absolute position. The distance is calculated based on the destination direction.
    If not only scrolling to direct children (see SetScrollToImmediateChildrenOnly()), this function is called recursively.
    @param pSearchInObject Which object to search in.
    @param vAbsPosInDestinationDirection X or Y coordinate of the point the distance shall be calculated to.
    @return The closest object (in destination direction) or NULL if no target direction was set.
    */
    CGUIObject* FindClosestObjectToPosition(const CGUICompositeObject* pSearchInObject, const eC_Value &vAbsPosInDestinationDirection) const;

    /**
    Sets the factor by which the active object's distance to the target position is multiplied during calls to FindClosestObjectToPosition().
    This is useful if the TouchScrollView shall move to the next child object even if mathematically the activeobject would still be closest to the target position.
    A typical use-case would be a TouchScrollView, where each entry covers a large area, so that users would have to perform long drag-distances in order to get to
    the next entry.
    @param vActiveObjectDistanceFactor Factor for distance calculations with the active object
    */
    void SetActiveObjectDistanceFactor( const eC_Value vActiveObjectDistanceFactor) { m_vActiveObjectDistanceFactor = vActiveObjectDistanceFactor; }

protected:

    /// Overloaded to scroll animated to the currently focused object
    void ScrollToFocussedObject();

    /** Checks if the given object is a valid target for scrolling and returns the best suitable target object for scrolling.
        Therefore, the given object must...
        -# not be NULL
        -# be a descendant of the container
        -# and if ScrollToImmediateChildrenOny is True it needs to be an immediate child of the container.
        @param pActiveObject Pointer to object to which the view shall scroll
        @return Object which is a valid scrolling target, or NULL if none could be found
    */
    CGUIObject* GetValidScrollTarget( CGUIObject* pActiveObject);

    /** Implements the scrolling animation for kinetic scrolling and scrolling the active object to the target position.
        @param vTimes the correction factor
    */
    virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1));

    /// Called by DoAnimate when the animation is over. Sets the focus to active object. Overriding methods should call the base class implementation. 
    virtual void StopScrollingAnimation();

    /// Scroll the nearest object (nearest to the click) to the target position if a target position is defined.
    eC_Bool DoClick( const eC_Value &vAbsX, const eC_Value &vAbsY );

    /// Trigger kinetic scrolling (after dragging) if activated, or scroll the nearest object (nearest to the target position) to the target position.
    virtual eC_Bool DoDragEnd(const eC_Value &vAbsX, const eC_Value &vAbsY);

    /** Set the attributes for the drag start point.
        @param  vAbsX X-position in screen coordinates where the click occurred
        @param  vAbsY Y-position in screen coordinates where the click occurred
        @return True The event has been handled.<BR>
                False The event has not been handled.
    */
    eC_Bool DoButtonDown(const eC_Value &vAbsX, const eC_Value &vAbsY);

    virtual eC_Bool DoDrag( const eC_Value &vDeltaX, const eC_Value &vDeltaY, const eC_Value &vAbsX, const eC_Value &vAbsY );

    /// Determines if a target position is set.
    eC_Bool HasTargetPosition() const;
    /// Determines if a target position is set in scroll direction.
    eC_Bool HasTargetPositionInScrollDirection() const;
    /// Determines if a target position is set in drag direction.
    eC_Bool HasTargetPositionInDragDirection() const;

    /**
    Helper function for FindClosestObjectToPosition(). Calculates the distance based on the destination direction.
    @param pkObject The object to calculate the distance to.
    @param vAbsPosInDestinationDirection AbsX, or AbsY (depending on destination direction) of the point to calculate the distance to.
    @return The distance or eC_MAXVALUE if no destination direction is set.
    */
    eC_Value CalcDistanceToObject(const CGUIObject* pkObject, const eC_Value &vAbsPosInDestinationDirection) const;

    CSafeGUIObjectPtr m_pActiveObject; ///< The active object which is scrolled to the destination point.

    eC_UInt     m_uiLastDrag; ///< Holds the time when the last drag was started. Used to calculate the drag speed.
    CGUIPoint   m_kDragStart; ///< Holds the start point of a drag.
    eC_Value    m_vScrollSpeed; ///< The scroll speed of the ongoing animation.
    eC_Value    m_vBounceBackFactor; ///< The bounce back factor used when the upper or lower edge of the scroll view is reached while an animation.

    /// The relative destination position for the object center.
    eC_Value m_vDestination;
    /// The destination direction is used to determine if m_vDestination is the XPos or the YPos.
    Direction_t m_eDestinationDirection;

    /// Determines which objects are scrolled to the target position. Only direct children of the scroll view, or any descendant.
    eC_Bool     m_bScrollToImmediateChildrenOnly;

    /// Determines if kinetic scrolling is activated in vertical scroll direction.
    eC_Bool     m_bKineticScrollingVertical;
    /// Determines if kinetic scrolling is activated in horizontal scroll direction.
    eC_Bool     m_bKineticScrollingHorizontal;

    /** Holds the direction of the scroll animation.
    This is set in DoDrag based on the drag direction, or in DoClick based on the target
    positions which are set when scrolling to the closest object.*/
    Direction_t m_eScrollAnimationDirection;

    /// Holds the drag direction. DRAG_NONE when no drag is ongoing.
    Direction_t m_eCurrentDragDirection;

    /// Factor by which the active object's distance to the target position is multiplied.
    eC_Value    m_vActiveObjectDistanceFactor;

    /// Timestamp of last received DoDrag()-Event
    eC_UInt     m_uiDragTimeStamp;

    /// Speed of animation at the beginning of a kinetic animation
    eC_Value    m_vStartScrollSpeed;

    /// EasingType used for kinetic animation
    CGUIEasing::EasingType_t  m_eEasingType;

    /// Duration of easing animation for kinetic scrolling
    eC_Value m_vEasingDuration;
};

#endif // GUI_TOUCH_SCROLL_VIEW_H
