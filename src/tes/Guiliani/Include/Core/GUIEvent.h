/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIEVENT__H_
#define GUIEVENT__H_

#include "eC_Types.h"
#include "GUIEventTypeResource.h"

/// Base class for Guiliani Events.

/**
    Guiliani abstracts user input through the use of an internal platform-independent event system.
    The CGUIEvent class is the base class from which all specific events are derived. This ensures
    that every event which is delivered to the event handler has got an event type (EventType_t) and
    a get method (CGUIEvent::GetType) to access it, so that the event handler is able to safely
    identify and handle it.
*/
class CGUIEvent
{
public:
    /** Specifies the class of an event (keyboard, mouse).
    */
    enum EventSource_t
    {
        ES_NONE = 0x00000000, ///< Class of event unknown.
        ES_KEYBOARD, ///< Class of event is keyboard event.
        ES_MOUSE, ///< Class of event is mous event.
    };

    /** The modifier keys that are supported by Guiliani, but not necessarily
        by each CGUIInputMedia implementation.
      */
    enum Modifier_t
    {
        GKM_NONE    = 0x00000000, ///< No Modifier.
        GKM_SHIFT   = 0x00000001, ///< Shift key.
        GKM_CONTROL = 0x00000002, ///< Control key.
        GKM_ALT     = 0x00000004, ///< Alt key.
        GKM_META    = 0x00000008  ///< Additional modifier, depends on CGUIInputMedia implementation.
    };

    /** Constructs a CGUIEvent object.
        @param eType The type of event.
        @param uiModifiers Logical OR combination of Modifier_t values that
               represent the modifier keys that were pressed when the event
               occurred.
      */
    CGUIEvent(EventType_t eType, eC_UInt uiModifiers)
        : m_eType(eType), m_uiModifiers(uiModifiers)
    {}

    /// Destructor.
    virtual ~CGUIEvent() {}

    /** Get the type of the event.
        @return The type of the derived event.
      */
    inline EventType_t GetType() const { return m_eType; }

    /** Returns the modifier keys that were pressed when the event occurred.
        @return Logical OR combination of Modifier_t values.
      */
    inline eC_UInt GetModifiers() const { return m_uiModifiers; }


    /** Returns the source of the event (Mouse, Keyboard or neither of them).
    @return Enum representing the source.
    */
    EventSource_t GetEventSource() const
    {
        EventType_t eEventTyp = GetType();
        if (eEventTyp == ET_KEYDOWN || eEventTyp == ET_KEYUP ||
            eEventTyp == ET_CHAR || eEventTyp == ET_HOTKEYDOWN ||
            eEventTyp == ET_HOTKEYUP)
        {
            return ES_KEYBOARD;
        }
        if (eEventTyp == ET_LBUTTONDOWN || eEventTyp == ET_LBUTTONUP ||
            eEventTyp == ET_RBUTTONDOWN || eEventTyp == ET_RBUTTONUP ||
            eEventTyp == ET_MOUSEMOVE || eEventTyp == ET_MOUSEWHEEL)
        {
            return ES_MOUSE;
        }

        return ES_NONE;
    }

private:
    /// Do not allow default construction.
    CGUIEvent();

    /// Type of the event.
    EventType_t   m_eType;

    /// Modifiers of this event.
    eC_UInt m_uiModifiers;
};

/// Represents platform-independent keyboard events.

/**
    Keyboard events expand the functionality of the event base class by adding methods, which
    return the identifier (declared in GUIKeyIdentifier_t) and the character of the key, which was pressed.
*/
class CGUIKeyboardEvent
    : public CGUIEvent
{
public:
    /** Constructs a keyboard event.
        @param eType The event type.
        @param uiKey Key 'value', usually the pressed key's character code.
        @param eKeyIdentifier The Guiliani identifier for special keys.
        @param uiModifiers Logical OR combination of CGUIEvent::Modifier_t values.
      */
    CGUIKeyboardEvent( EventType_t eType, eC_UInt uiKey, GUIKeyIdentifier_t eKeyIdentifier, eC_UInt uiModifiers = 0)
        : CGUIEvent(eType, uiModifiers), m_uiKey(uiKey), m_eKeyIdentifier(eKeyIdentifier)
    {
    }

    ~CGUIKeyboardEvent() {}

    /** Get the character code of the key, which was pressed.
        @return the pressed key's character code.
      */
    inline eC_UInt GetKeyContent() const { return m_uiKey; }

    /** Get the key, which was pressed.
        @return the Guiliani special key identifier.
      */
    inline GUIKeyIdentifier_t GetKeyIdentifier() const { return m_eKeyIdentifier; }

private:
    /// Do not allow default construction.
    CGUIKeyboardEvent();

    /// The key that was pressed.
    eC_UInt   m_uiKey;

    /// Identifier for Guiliani special keys.
    GUIKeyIdentifier_t m_eKeyIdentifier;
};

/// Represents platform-independent mouse events.

/**
    Mouse events expand the functionality of the event base class by adding several methods for querying
    the mouse status (position, button status). Mouse events may be useful for handling several types
    of pointing devices, including touchpads.
*/
class CGUIMouseEvent
    : public CGUIEvent
{
    /// Do not allow default construction.
    CGUIMouseEvent();

    /// The coordinates of the mouse event.
    eC_Int     m_iX;
    eC_Int     m_iY;

    /// Indicates whether the left mouse button was pressed.
    eC_Bool    m_bLeftButtonPressed;
    /// Indicates whether the left mouse button was pressed.
    eC_Bool    m_bRightButtonPressed;

    // The mousewheel delta.
    eC_Value   m_vWheelDelta;

public:
    /** Constructs a mouse event.
        @param eType The type of mouse event (movement, click ...).
        @param iX The x position of the mouse pointer when the event occurred.
        @param iY The y position of the mouse pointer when the event occurred.
        @param bLeftButton Whether the left button is pressed down.
        @param bRightButton Whether the right button is pressed down.
        @param uiModifiers Logical OR combination of CGUIEvent::Modifier_t values.
        @param vWheelDelta The mousewheel delta.
      */
    CGUIMouseEvent(EventType_t eType, eC_Int iX=0, eC_Int iY=0,
        eC_Bool bLeftButton=false, eC_Bool bRightButton=false, eC_UInt uiModifiers = 0,
        eC_Value vWheelDelta = 0)
        : CGUIEvent(eType, uiModifiers), m_iX(iX), m_iY(iY),
          m_bLeftButtonPressed(bLeftButton), m_bRightButtonPressed(bRightButton),
          m_vWheelDelta(vWheelDelta)
    {
    }

    ~CGUIMouseEvent() {}
    /** Returns the x position of the mouse pointer when the event occurred.
        @return x position relative to Guiliani's root object.
      */
    inline eC_Int  GetX() const { return m_iX; }
    /** Returns the y position of the mouse pointer when the event occurred.
        @return y position relative to Guiliani's root object.
      */
    inline eC_Int  GetY() const { return m_iY; }

    /** Returns the wheel delta value when the event occurred.
        A positive value indicates that the wheel was rotated forward,
        away from the user; a negative value indicates that the wheel
        was rotated backward, toward the user.
        @return wheel delta.
      */
    inline eC_Value GetWheelDelta() const { return m_vWheelDelta; }

    /** Returns whether the left mouse button was pressed when the event occurred.
        @return The left mouse button state.
      */
    inline eC_Bool IsLeftButtonPressed () const { return m_bLeftButtonPressed; }

    /** Returns whether the right mouse button was pressed when the event occurred.
        @return The right mouse button state.
      */
    inline eC_Bool IsRightButtonPressed () const { return m_bRightButtonPressed; }
};

#endif
