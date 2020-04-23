/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef _GUISCRIPTEDINPUT_H_
#define _GUISCRIPTEDINPUT_H_

#include "eC_Types.h"
#include "GUIInputMedia.h"
#include "GUIEvent.h"
#include "GUIEventTypeResource.h"
#include "eC_TList_doubleLinked.h"
#include "eC_String.h"
#include "GUIPoint.h"


class CGUIEvent;


/// CGUIScriptedInput is used to simulate user-input via scripted or randomized events.
// @guiliani_doxygen toplevel_module Scripted Input
/**
    <table border="0">
    <tr>
        <td width="200">@image html scripted_input.png</td>
            <td>
                The "scripted input" Module offers the possibility to create the GUI events by an xml file. One use case for this is to create automated user interface tests (Class: CGUIScriptedInput).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    The GUIScriptedInput class can be operated in three modes:
    - Read scripted events from an XML File
    - Simulate events via C++ code
    - Generate random events (e.g. for Testing)

    Use this class as follows:
    1. Create an instance of your "normal" Input media as always.
    2. Create an instance of CGUIScriptedInput

    Note that the CGUIScriptedInput will begin processing of the internal event queue
    after the uiStartTime milliseconds have passed, this delay defaults to 5 seconds to allow
    for GUI startup including e.g., image loading on slow target devices.

    Example:
@code
        CGUIInputWindows::CreateInstance(hWnd);
        CGUIScriptedInput::CreateInstance(0);
@endcode

    After events have been added - either by streaming them from an XML file, or by scripting them directly from code - they
    are held in an internal queue. This queue is continuously processed, so that an endless sequence of events is generated
    (unless the queue is empty, of course). The processing will restart with the first event after the delay time of the last
    event has passed.

    Example:
    Events can be added from code by using the functions AddMouseEvent(), AddMouseMovement() or AddKeyboardEvent().

@code
    // Simulate a Button down -> Move -> Button Up (=dragging) event
    CGUIScriptedInput::AddMouseEvent( ET_LBUTTONDOWN, 200,100);
    CGUIScriptedInput::AddMouseMovement( 200, 100, 80, 80, 1000, true);
    CGUIScriptedInput::AddMouseEvent( ET_LBUTTONUP, 80,80);

    // Simulate a key press on Escape
    CGUIScriptedInput::AddKeyboardEvent( ET_KEYDOWN, 0, GK_ESCAPE);
@endcode

    Alternatively, in case an event is to be processed immediately as the next event and deleted afterwards,
    setting bImmediate of AddMouseEvent(), AddMouseMovement() or AddKeyboardEvent() to true will force the
    event to be placed in front of the next event to be handled. It will then be deleted after handling.
    
    @note Placing immediate events into the queue will delay the queue by the time required to process
          the newly added event.
          Adding multiple immediate events will cause the last added event to be executed first (FILO).

    ONLY IF YOU USE GUILIANI_STREAM_GUI: Use the function ReadFromFile(Filename) to add events from file.
    The file should look like this:

@verbatim
    <EventScript>
        <NumEvents>2</NumEvents>            // This is the number of events. It must always fit the number of entries in the script.
        <Event>                             // Example for a mouse event:
            <Source>0</Source>              // This is the source for differ between a mouse (0) or keyboard (1) event.
            <Type>6</Type>                  // The number of the event enum from Event_t.
            <XPos>23</XPos>                 // The X position of the mouse cursor.
            <YPos>277</YPos>                // The Y position of the mouse cursor.
            <Time>200</Time>                // The time until the next event occurs.
        </Event>
        <Event>                             // Example for a keyboard event:
            <Source>1</Source>
            <Type>0</Type>
            <Key>0</Key>                    // The keycode number.
            <GUIKey>1</GUIKey>              // The Guiliani key enum number.
            <Time>5000</Time>
        </Event>
    </EventScript>
@endverbatim
@ingroup GUILIANI_DEBUGGING
*/

class CGUIScriptedInput
    : public CGUIInputMedia
{
public:
    /** @brief Struct which holds the information for each event in the list. */
    struct ScriptedInputEvent_t
    {
        EventType_t             m_eEventType; ///< event type
        GUIKeyIdentifier_t      m_eGUIKey; ///< key identifier
        eC_UInt                 m_uiKey; ///< key value
        eC_Int                  m_iMouseXPos; ///< x position of event
        eC_Int                  m_iMouseYPos; ///< y position of event
        eC_Bool                 m_bLeftButton; ///< Left button pressed
        eC_Bool                 m_bRightButton; ///< Right button pressed
        eC_UInt                 m_uiTime; ///< timestamp
        eC_Bool                 m_bMouseEvent; ///< True for mouse event
        eC_Bool                 m_bImmediateEvent; ///< True for immediate event
    };

    /** Creates the singleton instance.
        @param uiStartDelay Time in ms until the first event will be executed.
    */
    static void CreateInstance(const eC_UInt uiStartDelay = 5000);

    /** Gets an event from the actual input media OR from the scripted event list.
        Guiliani will automatically call this method once per frame from its RunOnce()-method.
        The supplied parameter specifies the time in milliseconds until the next processing of
        Animation-timers or Commands is required. An input media may use this to avoid unnecessary
        event polling. It MUST return from GetEvent after the specified time, though.
        @param uiIdleTime specifies the maximum idle time in ms
        @return The event
    */
    CGUIEvent* GetEvent(eC_UInt uiIdleTime);

    void StopIdle()
    {
        if( ms_pActualInputMedia)
            ms_pActualInputMedia->StopIdle();
    }

    /** Helper method that creates/simulate the left button click (button down, followed by button up) events
        @param pkObject object where the mouse click should executed.
        @return True if successful, False otherwise
    */
    static eC_Bool SimulateLeftButtonClick(CGUIObject* pkObject);

    /** Helper method that creates/simulate keypress and release-events.
        @param eKeyIdentifier the used key.
        @return True if successful, False otherwise
    */
    static eC_Bool SimulateKeyPress (const GUIKeyIdentifier_t &eKeyIdentifier);

    /** Adds a mouse event to events list.
        @param eEventType Type of event.
        @param iMouseXPos X position of mouse where the event will be executed.
        @param iMouseYPos Y position of mouse where the event will be executed.
        @param uiTime Time in ms until the next event will be executed.
        @param bImmediate Whether the event is "immediate", i.e. to be executed independently of the queue and then deleted.
    */
    static void AddMouseEvent(
        const EventType_t eEventType,
        const eC_Int iMouseXPos,
        const eC_Int iMouseYPos,
        const eC_UInt uiTime = 0,
        const eC_Bool bImmediate = false);

    /** Convenience method for adding a series of mouse events, simulating a movement of the mouse from the
        given start position to the end position. This will generate mouse events every 10 ms for the requested time.
        @param iMouseStartXPos X position of mouse where the mouse movement starts.
        @param iMouseStartYPos Y position of mouse where the mouse movement starts.
        @param iMouseEndXPos X position of mouse where the mouse movement ends.
        @param iMouseEndYPos Y position of mouse where the mouse movement ends.
        @param uiDuration Time in ms for the mouse move event sequence.
        @param bDrag If true the left mouse button will be pressed for the movement.
        @param uiTime Time in ms from the last event in the mouse move sequence until the next event in the queue will be executed.
        @param bImmediate Whether the events generated are"immediate", i.e. to be executed independently of the queue and then deleted.
    */
    static void AddMouseMovement(
        const eC_Int iMouseStartXPos,
        const eC_Int iMouseStartYPos,
        const eC_Int iMouseEndXPos,
        const eC_Int iMouseEndYPos,
        const eC_UInt uiDuration,
        const eC_Bool bDrag,
        const eC_UInt uiTime = 0,
        const eC_Bool bImmediate=false);

    /** Adds a keyboard event to events list.
        @param eEventType Type of event.
        @param uiKey Key code to be executed.
        @param eGUIKey Name of Guiliani key identifier to be executed.
        @param uiTime Time in ms until the next event will be executed.
        @param bImmediate Whether the event is "immediate", i.e. to be executed independently of the queue and then deleted.
    */
    static void AddKeyboardEvent(
        const EventType_t eEventType,
        const eC_UInt uiKey,
        const GUIKeyIdentifier_t eGUIKey,
        const eC_UInt uiTime = 0,
        const eC_Bool bImmediate = false);

    /** Enables / Disables the virtual mouse cursor.
        A simulated mouse cursor will be moved to the scripted / generated mouse positions to indicate the current mouse position on the screen
        @param bShowMouseCursor TRUE if the mouse cursor shall be shown, False otherwise
    */
    static void EnableVirtualMouseCursor( const eC_Bool bShowMouseCursor);

    /** Enables / Disables generation of random events.
        Enabling stress test will generate random user input events and forward them to the GUI just as if a user was randomly operating it.
        @param bStressTestActive TRUE to enable stress test, False otherwise
    */
    static void EnableStressTest( const eC_Bool bStressTestActive);

    /** Enables / Disables the "Demo-loop" mode of the queue.
        When in RunOnce-Mode, all scripted events in the queue will be deleted after handling.
        @param bRunOnceMode TRUE to enable run once mode, False otherwise (Demo-loop mode)
    */
    static void EnableRunOnceMode( const eC_Bool bRunOnceMode);

    /// Clears the internal event queue, removing all scripted events
    static void ClearEventQueue();

#ifdef GUILIANI_STREAM_GUI
    /** Read the events script file.
        @param kPath Path of the script file (needs the resource file handler).
    */
    static void ReadFromFile(const eC_String &kPath);
#endif //#ifdef GUILIANI_STREAM_GUI

    /** Checks if the event queue is empty.
        @return True if empty
    */
    static eC_Bool IsEventQueueEmpty()
    {
        return ms_kEventsList.IsEmpty();
    }

protected:
    /** Constructor
        @param uiStartTime Start time
    */
    CGUIScriptedInput(const eC_UInt uiStartTime);
    ~CGUIScriptedInput();

    /** Creates a single random mouse or keyboard event
        Warning: It is the caller's responsibility to delete the returned pointer!
        @return A newly allocated random event
    */
    CGUIEvent* CreateRandomEvent();

private:
    /// Internal method to add a mouse event to the queue without producing debug output
    static void AddMouseEventToQueue(
        const EventType_t eEventType,
        const eC_Int iMouseXPos,
        const eC_Int iMouseYPos,
        const eC_Bool bLeftButton,
        const eC_Bool bRightButton,
        const eC_UInt uiTime=0,
        const eC_Bool bImmediate=false);

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUIScriptedInput(const CGUIScriptedInput& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUIScriptedInput& operator=(const CGUIScriptedInput& kSource);

    /// Hold the time when the next event will be executed.
    eC_UInt m_uiTimeToNextEvent;

    /// Stores the time of last event's execution, used to calculate the expired time.
    eC_UInt m_uiTimeAtLastEvent;

    /// The list, which holds the events.
    static eC_TListDoubleLinked<ScriptedInputEvent_t> ms_kEventsList;

    /// Events list iterator.
    static eC_TListDoubleLinked<ScriptedInputEvent_t>::SafeIterator ms_kEventsListIter;

    /// Is StressTest mode active?
    static eC_Bool ms_bStressTestActive;

    /// Is RunOnce mode active ?
    static eC_Bool ms_bRunOnceActive;

    /// Pointer to actual Input media which was created prior to the ScriptedInput
    static CGUIInputMedia* ms_pActualInputMedia;

    /// Current mouse position, direction and button status for random event generation
    static CGUIObject* ms_pMouseCursor;
    CGUIPoint   m_kPosition;
    CGUIPoint   m_kDirection;
    eC_Bool     m_bButtonPressed;
};
#endif
