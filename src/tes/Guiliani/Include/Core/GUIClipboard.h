/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUICLIPBOARD__H_
#define GUICLIPBOARD__H_

#include "eC_String.h"

// For clipboard access under linux X11 desktop
#if defined eC_TARGET_CPU_X86 && eC_TARGET_ENV_LINUX
#include "GUIThread.h"
#include <X11/Xmu/Atoms.h>  // XrmOptionDescRec, Atoms, ...

// GUIThread for handle copy to clipboard
class CClipboardThread : public CGUIThread
{
public:
    CClipboardThread();

    void Execute(void * pArg);

    /// Fill string with content for X11 clipboard
    void SetClipboardString(const eC_String & kStr);
private:
    /// Helper fucntion to copy string to X11 clipboard
    int SetX11Clipboard(Display * pkDisplay,
                        Window * kWindow,
                        XEvent xeEvent,
                        Atom * pxaProperty, Atom xaTarget,
                        unsigned char * pucText, unsigned long ulLength, unsigned long * pulPosition,
                        unsigned int * puiContext);

    eC_String m_kStrClipboard;     // String with content for clipboard
};
#endif

/// Retrieve pointer to the clipboard instance.
#define GET_CLIPBOARD       CGUIClipboard::GetInstance()
/// A simple Clipboard wrapper.

/** This offers basic access to the clipboard under windows, and serves as a plain global storage for a string
    on all other platforms.  This class is intended to be used internally by Guiliani controls.

    The following code demonstrates a basic Copy&Paste implementation which uses the CGUIClipboard class.
@code
eC_Bool CDemoObject::DoKeyDown(const GUIKeyIdentifier_t &eKeyIdentifier, const eC_UInt &uiModifiers)
{
    switch( eKeyIdentifier)
    {
        case GK_PASTE:
        {
            m_pTextObject->SetText( GET_CLIPBOARD.GetString() );
        }
        break;
        case GK_COPY:
        {
            GET_CLIPBOARD.PutString( m_pTextObject->GetText() );
        }
    }
    return false;
}
@endcode
*/
class CGUIClipboard
{
public:
    /// Destructor
    virtual ~CGUIClipboard(void);

    /// return clipboard global instance
    static inline CGUIClipboard& GetInstance() { return ms_kClipboard;}

    /** Push the given string onto the clipboard. This will replace any former clipboard contents.
        @param kStr String to copy onto the clipboard
    */
    void PutString(const eC_String& kStr);

    /** Retrieves the latest text string from the clipboard.
        @return String with text from clipboard
    */
    eC_String GetString() const;

    /** Clear the clipboard.
    */
    void ClearClipboard();

private:
    /// static protected member of the global clipboard instance
    static CGUIClipboard ms_kClipboard;

    /// Constructor
    CGUIClipboard();

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGUIClipboard(const CGUIClipboard& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUIClipboard& operator=(const CGUIClipboard& kSource);

    /** Stores the clipboard string (as a fallback for non-windows platforms.
    */
    eC_String m_kClipboardString;

#if defined eC_TARGET_CPU_X86 && eC_TARGET_ENV_LINUX
    /// Helper fucntion to copy string from X11 clipboard
    int GetX11Clipboard(Display * pkDisplay,
                        Window kWindow,
                        XEvent xeEvent,
                        Atom sel, Atom xaTarget,
                        unsigned char ** ppucText, unsigned long * pulLength,
                        unsigned int * uiContext) const;

    Display * m_pkDisplay;  // X11 display pointer

    Window m_kWindow;       // X11 window

    CClipboardThread * m_pkSetX11ClipboardThread;   // Handle copy to X11 clipboard
#endif
};
#endif
