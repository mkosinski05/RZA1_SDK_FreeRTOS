/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef _GUIINPUTWINDOWS_H_
#define _GUIINPUTWINDOWS_H_

#include "eC_Types.h"
#include "GUIInputMedia.h"
#include "GUIEventTypeResource.h"
#include <windows.h>

class CGUIEvent;

// Definition similar to windowsx.h GET_X_LPARAM(lp) used for Mousewheel
// include "windowsx.h" not usable because _WIN32_WINNT is not defined and therefore the function
// will not be declared

/** @brief Standard input Wrapper for systems running a windows OS.*/
class CGUIInputWindows
    : public CGUIInputMedia
{
public:
    /// Creates the singleton instance.
    static void CreateInstance(const HWND &hWnd);
    virtual CGUIEvent* GetEvent(eC_UInt uiIdleTime);
    virtual void StopIdle();

protected:

    /** Reads an event from the windows message queue.
        @param kMsg Windows message which was read from the message queue
        @param uiIdleTime Maximum allowed idle time (in millisec) after which the method must return
        @return True if a message was read, False otherwise */
    virtual BOOL        ReadMessageFromWindowsQueue(MSG &kMsg, eC_UInt uiIdleTime);

    /** Creates a Guiliani Event from the given windows message
        @param kMsg Windows message for which a corresponding Guiliani event shall be created
        @return Newly created Guiliani event (or NULL) */
    virtual CGUIEvent*  CreateGuilianiEventFromWindowsMessage(MSG &kMsg);

    /** Constructor
        @param hWnd Window handle
    */
    CGUIInputWindows(const HWND &hWnd);
    ~CGUIInputWindows();

    /** Helper for translating Windows VK codes into Guiliani key IDs.
        @param virtualKeyCode The key code to be translated. This should be the
               wParam of the WM_KEYDOWN or WM_KEYUP message.
        @param uiModifiers The current modifiers state.
        @return The translated Guiliani key ID. GK_NONE if VK is not handled.
      */
    GUIKeyIdentifier_t GetKeyID(UINT virtualKeyCode, eC_UInt uiModifiers);

    /// Handle to associated window
    HWND m_hWnd;

    /// Indicate that the left mouse button is currently pressed.
    eC_Bool    m_bLeftButtonPressed;

    /// Indicate that the right mouse button is currently pressed.
    eC_Bool    m_bRightButtonPressed;

    /// Set to True if StopIdle() was called, reset to False by GetEvent()
    eC_Bool    m_bStopIdle;

private:
    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGUIInputWindows(const CGUIInputWindows& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGUIInputWindows& operator=(const CGUIInputWindows& kSource);
};
#endif
