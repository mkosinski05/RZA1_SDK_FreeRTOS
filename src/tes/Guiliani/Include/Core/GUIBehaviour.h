/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIBEHAVIOUR__H_
#define GUIBEHAVIOUR__H_

#include "GUIStreamableObject.h"
#include "GUIEventTypeResource.h"
#include "eC_Math.h"

class CGUIEvent;
class CGUIObject;

/**
    Enumeration for all existing behaviour slots.
*/
enum BehaviourType_t
{
    BT_CLICK = 0,
    BT_LONG_CLICK,
    BT_DOUBLE_CLICK,
    BT_BUTTON_DOWN,
    BT_BUTTON_UP,
    BT_FOCUS_PREVIOUS,
    BT_FOCUS_NEXT,
    BT_DECREASE,
    BT_INCREASE,
    BT_DRAG,
    BT_MOUSE_ENTER,
    BT_MOUSE_LEAVE,
    BT_MOUSE_MOVE,
    BT_WHEEL_UP,
    BT_WHEEL_DOWN,
    BT_USER_EVENT,
    BT_KEY_DOWN,
    BT_KEY_UP,
    BT_CHAR,
    BT_GETFOCUS,
    BT_LOSEFOCUS,
    BT_DRAGEND,
    BT_DUMMY
};

/// CGUIBehaviour base class.

/** The methods of CGUIBehaviour define Guiliani's standard event handling slots.
    These slots will be called by CGUIEventHandler automatically in response to specific user actions.
    (see @ref subsec_event_slot_mapping for the detailed information of events to handling methods).

    Also, please refer to @ref sec_input_media_event_sequence for further information.

    Thus, if a control needs special handling for the associated events it can reimplement the corresponding
    slots with the desired functionality. When inheriting customized controls from Guiliani's standard set of widgets,
    it is advisable to call the base-class implementation of the overwritten method. Failure to do so may break the base
    classes default behaviour.
*/
class CGUIBehaviour
    : public CGUIStreamableObject
{
public:
    /** @name Mouse Events */
    ///@{
    /** Called when the object receives a click event.
        A click can e.g., be the result of a mouse click, a finger press/release sequence
        on touchscreens, or be triggered by a GK_ACTION keyboard event.

        @note DoClick() should work on the absolute positions of this object,
        that were set at the time of the passed redraw, as the user expects to
        click on what he sees and not to click on an object in a newer and
        probably intermediate state that he currently does not see. Thus, to
        calculate the relative position (where the mouse button was pressed
        and released at the same position for in a certain time span, relative
        to the origin of this object) from the vAbsY argument you must do
        it like this: vMyRelYPos = vAbsY - GetAbsYPos().
        @param  vAbsX X-position in screen coordinates where the click occurred
        @param  vAbsY Y-position in screen coordinates where the click occurred
        @return True The event has been handled.<BR>
                False The event has not been handled. In this base
                implementation it always returns False.
    */
    virtual eC_Bool DoClick(
        const eC_Value &vAbsX = eC_FromInt(-1),
        const eC_Value &vAbsY = eC_FromInt(-1));

    /** Called when an object is pressed for a longer period of time.
        @see CGUIEventHandler::SetLongClickTime()

        @note DoLongClick() should work on the absolute positions of this object,
        that were set at the time of the passed redraw, as the user expects to
        click on what he sees and not to click on an object in a newer and
        probably intermediate state that he currently does not see. Thus, to
        calculate the relative position (where the mouse button was pressed
        and held down at the same position for a certain amount of time,
        relative to the origin of this object) from the vAbsY argument you
        must do it like this:
        vMyRelYPos = vAbsY - GetAbsYPos().
        @param  vAbsX X-position in screen coordinates where the click occurred
        @param  vAbsY Y-position in screen coordinates where the click occurred
        @return True The event has been handled.<BR>
                False The event has not been handled. In this base
                implementation it always returns False.
    */
    virtual eC_Bool DoLongClick(
        const eC_Value &vAbsX = eC_FromInt(-1),
        const eC_Value &vAbsY = eC_FromInt(-1));

    /** Called when an object is clicked twice within a given     time span.
        @see CGUIEventHandler::SetDoubleClickTime()

        @note DoDoubleClick() should work on the absolute positions of this object,
        that were set at the time of the passed redraw, as the user expects to
        click on what he sees and not to click on the object in a newer and
        probably intermediate state that he currently does not see in this
        state. Thus, to calculate the relative position (where the two clicks
        were done relative to the origin of this object) from the vAbsY
        argument you must do it like this:
        vMyRelYPos = vAbsY - GetAbsYPos().
        @param  vAbsX X-position in screen coordinates where the click occurred
        @param  vAbsY Y-position in screen coordinates where the click occurred
        @return True The event has been handled.<BR>
                False The event has not been handled. In this base
                implementation it always returns False.
    */
    virtual eC_Bool DoDoubleClick(
        const eC_Value &vAbsX = eC_FromInt(-1),
        const eC_Value &vAbsY = eC_FromInt(-1));

    /** Called when the mouse button or finger is pressed down over an object.

        @note DoButtonDown() should work on the absolute positions of this object,
        that were set at the time of the passed redraw, as the user expects to
        click on what he sees and not to click on the object in a newer and
        probably intermediate state that he currently does not see in this
        state. Thus, to calculate the relative position (where the object was
        pressed relative to the origin of this object) from the vAbsY
        argument you must do it like this:
        vMyRelYPos = vAbsY - GetAbsYPos().
        @param  vAbsX X-position in screen coordinates where the click occurred
        @param  vAbsY Y-position in screen coordinates where the click occurred
        @return True The event has been handled.<BR>
                False The event has not been handled. In this base
                implementation always returns False.
    */
    virtual eC_Bool DoButtonDown(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    /** Called when the mouse button or finger is released.
        The object under the mouse at the time of releasing will receive this event.

        @note DoButtonUp() should work on the absolute positions of this object,
        that were set at the time of the passed redraw, as the user expects to
        click on what he sees and not to click on an object in a newer and
        probably intermediate state that he currently does not see in this
        state. Thus, to calculate the relative position (where the button was
        released relative to the origin of this object) from the vAbsY
        argument you must do it like this:
        vMyRelYPos = vAbsY - GetAbsYPos().
    @param  vAbsX X-position in screen coordinates where the mouse was
        released.
    @param  vAbsY Y-position in screen coordinates where the mouse was
        released.
    @return True The event has been handled.<BR>
            False The event has not been handled. In this base
            implementation always returns False.
    */
    virtual eC_Bool DoButtonUp(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

   /** Called when the object receives a Drag event.
        Drags are typically created if the mouse is being used while a mouse-button is being
        pressed, or if the user moves his finger over a touchscreen.

        @param vDeltaX relative movement in x-direction since the last call of DoDrag/ButtonDown
        @param vDeltaY relative movement in y-direction since the last call of DoDrag/ButtonDown
        @param vAbsX current X-position in screen coordinates of the mouse
        @param vAbsY current Y-position in screen coordinates of the mouse
        @return True If the event has been handled.<BR>
                false Else.
    */
    virtual eC_Bool DoDrag(
        const eC_Value &vDeltaX,
        const eC_Value &vDeltaY,
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    /** Called on the dragged object (=the object on which the initial ButtonDown-event occurred)
        when the mouse button is released after a drag event.

        @param vAbsX current X-position in screen coordinates of the mouse
        @param vAbsY current Y-position in screen coordinates of the mouse
        @return True If the event has been handled.<BR>
                false Else.
    */
    virtual eC_Bool DoDragEnd(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);


    /** Called when the mouse enters the area which is covered by an object. 
        If several objects cover the same region, the topmost one receives the event.
        Objects will not receive DoMouseEnter events if they are not highlightable.
        @see CGUIObject::IsHighlightable()

        @param vAbsX X-position in screen coordinates where the mouse entered the object
        @param vAbsY Y-position in screen coordinates where the mouse entered the object
        @return True If the event has been handled.<BR>
                false Else.
    */
    virtual eC_Bool DoMouseEnter(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    /** Called when the mouse leaves the area which is covered by an object.
        Objects will not receive DoMouseLeave events if they are not highlightable.
        @see CGUIObject::IsHighlightable()
        @param vAbsX X-position of the mouse in screen coordinates after it left the object
        @param vAbsY Y-position of the mouse in screen coordinates after it left the object
        @return True If the event has been handled.<BR>
                false Else.
    */
    virtual eC_Bool DoMouseLeave(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    /** Called when the mouse is moved within the area covered by an object. If
        several objects cover the same region, the topmost one receives the event.
        Objects will not receive DoMouseEnter events if they are not highlightable.
        @see CGUIObject::IsHighlightable()
        @param vAbsX X-position of the mouse in screen coordinates
        @param vAbsY Y-position of the mouse in screen coordinates
        @return True If the event has been handled.<BR>
                false Else.
    */
    virtual eC_Bool DoMouseMove(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);
    ///@}
    /** @name Focus Events */
    ///@{
        /** Called when the object is requested to focus the "previous" object.
        Typically, this happens in reaction to a standard GK_UP event
        or by request from Source-Code.
        For a simple object there is nothing to do but to indicate that the
        event has not been handled.
        For a standardized event handling mechanism, this method must exist.
        @param pRefObj usually is a child of this object which indicates the
               current position in the child list.
        @param bSubTreeOfRefObjDone indicates if the subtree that is under
               pRefObj has already been asked to take focus, or if not.
        @return True If the event has been handled.<BR>
                false Else.
    */
    virtual eC_Bool FocusPrevious(
        CGUIObject* const pRefObj = NULL,
        const eC_Bool &bSubTreeOfRefObjDone = false);

    /** Called when the object is requested to focus the "next" object.
        Called when the object receives a standard GK_DOWN event
        or by request from Source-Code.
        For a simple object there is nothing to do but to indicate that the
        event has not been handled.
        For a standardized event handling mechanism, this method must exist.
        @param pRefObj usually is a child of this object which indicates the
        current position in the child list.
        @param bSubTreeOfRefObjDone indicates if the subtree that is under
        pRefObj has already been asked to take focus, or if not.
        @return True If the event has been handled.<BR>
                false Else.
    */
    virtual eC_Bool FocusNext(
        CGUIObject* const pRefObj = NULL,
        const eC_Bool &bSubTreeOfRefObjDone = false);
    
    /** Called when the object, or one of its children, received the focus.
        If this control is a composite object then the following applies:
        Whether this control or a child control got the focus can be
        determined by checking if this control is the currently focused
        object in the event handler.
        @see LoseFocus, CGUIEventHandler::GetFocussedObject() */
    virtual void GetFocus();

    /** Called when an object has lost the focus. In this method the focused
        object is already set to the object that newly received the focus.
        @see GetFocus */
    virtual void LoseFocus();
    ///@}
    /** @name Keyboard Events */
    ///@{
    /** Called when the object receives a standard GK_PAGEDOWN event.
        For a simple object there is nothing to do but to indicate that the
        event has not been handled.
        For a standardized event handling mechanism, this method must exist.
        @return True If the event has been handled.<BR>
                false Else.
    */
    virtual eC_Bool Decrease();

    /** Called when the object receives a standard GK_PAGEUP event.
        For a simple object there is nothing to do but to indicate that the
        event has not been handled.
        For a standardized event handling mechanism, this method must exist.
        @return True If the event has been handled.<BR>
                false Else.
    */
    virtual eC_Bool Increase();
    
    /** Called in reaction to a ET_KEYDOWN event, which means in general that a non-character 
        key was pressed. For that reason you will only get the Guiliani-Key identifier (possibly with modifiers)
        in this slot and not the character of the pressed key.
        @param eKeyIdentifier The Guiliani key identifier.
        @param uiModifiers The key modifiers bitfield.
        @return True If the event has been handled.<BR>
                false Else.
    */
    virtual eC_Bool DoKeyDown(
        const GUIKeyIdentifier_t &eKeyIdentifier,
        const eC_UInt &uiModifiers);

    /** Called in reaction to a ET_KEYUP event, which means in general that a non-character key 
        was released. For that reason you will only get the Guiliani key identifier (possibly with modifiers)
        in this slot and not the character of the released key.
        @param eKeyIdentifier The Guiliani key identifier.
        @param uiModifiers The key modifiers bitfield.
        @return True If the event has been handled.<BR>
                false Else.
    */
    virtual eC_Bool DoKeyUp(
        const GUIKeyIdentifier_t &eKeyIdentifier,
        const eC_UInt &uiModifiers);

    /** Called in reaction to a ET_CHAR event, which means in general that a character key was pressed. 
        In this slot you will  get the character with the Guiliani key identifier and modifiers. In
        most cases the Guiliani key identifier will be GK_NONE. But there are
        some special cases (e.g. Smartphones) for which it makes sense to send
        a Guiliani-Key identifier (GK_INTERMIT) even if a character key was
        pressed.
        @param uiKey Character of the pressed key.
        @param eKeyIdentifier The Guiliani key identifier.
        @param uiModifiers The key modifiers bitfield.
        @return True If the event has been handled.<BR>
                false Else.
    */
    virtual eC_Bool DoChar(
        const eC_UInt &uiKey,
        const GUIKeyIdentifier_t &eKeyIdentifier,
        const eC_UInt &uiModifiers);
    ///@}
    /** @name Scroll Events */
    ///@{
    /** Called when the mouse wheel was scrolled in upward direction.
        @param vAbsX X-position of the mouse in screen coordinates
        @param vAbsY Y-position of the mouse in screen coordinates
        @return True If the event has been handled.<BR>
                false Else.
    */
    virtual eC_Bool DoScrollUp(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    /** Called when the mouse wheel was scrolled in downward direction.
        @param vAbsX X-position of the mouse in screen coordinates
        @param vAbsY Y-position of the mouse in screen coordinates
        @return True If the event has been handled.<BR>
                false Else.
    */
    virtual eC_Bool DoScrollDown(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    /** Called when the mouse wheel was scrolled in upward direction while
        the scroll direction modifier was pressed.
        @param vAbsX X-position of the mouse in screen coordinates
        @param vAbsY Y-position of the mouse in screen coordinates
        @see CGUIEventHandler::SetScrollModifiers()
        @return True If the event has been handled.<BR>
                false Else.
    */
    virtual eC_Bool DoScrollLeft(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    /** Called when the mouse wheel was scrolled in downward direction while
        the scroll direction modifier was pressed.
        @param vAbsX X-position of the mouse in screen coordinates
        @param vAbsY Y-position of the mouse in screen coordinates
        @see CGUIEventHandler::SetScrollModifiers()
        @return True If the event has been handled.<BR>
                false Else.
    */
    virtual eC_Bool DoScrollRight(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);
    ///@}
    /** Slot for handling of arbitrary user events. Override this slot to
        catch <i>any</i> incoming event before it is dealt with by standard handlers.

        Be aware that DoUserEvent() will be called on the currently <i>focused</i> object
        for <b>all</b> incoming events! This means for instance that your focused object
        (and its parents) will also receive events of type ET_LBUTTONDOWN when the user is
        clicking somewhere else on the screen.

        @note If DoUserEvent returns true, this will prevent <b>any</b> further handling of this event.
        i.e. If your implementation returns true for ET_MOUSEMOVE events received by this object,
        this will prevent <b>any</b> calls to the framework's standard DoMouseMove(), DoMouseEnter(),
        DoMouseLeave() slots.

        @param pEvent The event to be processed.
        @return True If the event has been handled.<BR>
                false Else.
    */
    virtual eC_Bool DoUserEvent( CGUIEvent* const pEvent);

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    /// XML tag to be used when writing a behaviour class ID into a stream.
    static const eC_Char XMLTAG_BEHAVIOURCLASSID[];
#endif
};

#endif
