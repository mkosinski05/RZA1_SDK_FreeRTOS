/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef _GUIINPUTSMARTPHONE__H_
#define _GUIINPUTSMARTPHONE__H_

#include "GUIInputMedia.h"
#include "eC_String.h"


//
// User Messages used to send characters from the MyGUI back to the input wrapper
//

#define USER_MSG_COMPOSITIONSTRING  (WM_USER + 1)
#define USER_MSG_RESULTSTRING       (WM_USER + 2)
#define USER_MSG_CHAR               (WM_USER + 3)

#define MAX_NUMBER_OF_BUFFERED_EVENTS 32


/// Used to parse the input events from a Smartphone.

class CGUIInputSmartphone
    : public CGUIInputMedia
{
public:
    /**
    Singleton for creating a SteamReaderXML object and change the pointer in
    base class to this graphic wrap.
    @return Indicate whether by this call, an instance is created.
            The instance is created, if true, not created, otherwise.
    */
    static void CreateInstance(const HWND &);
    /**
    provides the current event
    note: while intermittent multi-tap key codes are available, the keyboard events are indicated with GK_INTERMIT
    @param uiIdleTime Idle time
    @return The event
    */
    CGUIEvent* GetEvent(eC_UInt uiIdleTime);
    /**
    This method is called from CNavSmartphoneHotkeysBehaviour::DoUserEvent
    whenever a hotkey event is processed and notifies the input wrapper to
    ignore the IME next events.
    @param bIgnore Whether to ignore trailing events
    */
    void NotifyIgnoreTrailingEvents(bool bIgnore = true);
    /**
    This method sets the IME into number mode and is used in the NavCompexList
    to avoid entering of alpabetical characters where wanted.
    @param bNumeric whether to set IME to numeric mode
    */
    void SetIMEInputBehaviourNumeric(bool bNumeric);
private:
    /// window handle
    HWND        m_hWnd;               ///< window handle
    /// pointer to a buffered event (will be reported at next chance)
    CGUIEvent*      m_pBufferedEvent[MAX_NUMBER_OF_BUFFERED_EVENTS];
    eC_Int          m_iEventCounter;
    /**
    While this bool is true IME events will be ignpored until the next
    USER_MSG_RESULTSTRING event is received.
    */
    bool m_bTemporaryIgnoreIMEEvents;
    /// constructor
    CGUIInputSmartphone(const HWND &hWnd);
    /// destructor
    ~CGUIInputSmartphone();
    /// add an event to the buffer
    void AddEventToBuffer(CGUIEvent* pNewBufferedEvent);
    /// removed an event from the buffer
    CGUIEvent* RemoveEventFromBuffer(void);
};

#endif //#ifndef _GUIINPUTSMARTPHONE__H_
