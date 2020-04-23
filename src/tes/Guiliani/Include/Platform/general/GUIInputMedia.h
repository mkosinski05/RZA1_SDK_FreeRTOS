/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIINPUTMEDIA__H_
#define GUIINPUTMEDIA__H_

#include "eC_Types.h"
#include <cassert>

/// Retrieve pointer to inputmedia instance
#define GETINPUTMEDIA        CGUIInputMedia::GetInstance()

class CGUI;
class CGUIObject;
class CGUIEvent;

/** Base-class for all input devices that are attachable to the GUI.
    @see @ref sec_porting_input "Porting User Input for Guiliani"
*/
class CGUIInputMedia
{
public:
    /// return pointer to inputmedia instance
    static inline CGUIInputMedia& GetInstance() { assert(NULL != ms_pInputMedia); return *ms_pInputMedia; }

    /// Deletes this input media.
    static void DeleteInstance();

    /** Retrieves a platform-specific event and creates an appropriate Guiliani event.
        The supplied parameter specifies the time in milliseconds until the next processing of
        animation timers or commands is required. An input media may use this to avoid unnecessary
        event polling. It MUST return from GetEvent after the specified time, though.
        @param uiIdleTime specifies the maximum idle time in ms.
        @return Pointer to a newly generated Guiliani event, or NULL if nothing happened.
    */
    virtual CGUIEvent* GetEvent(eC_UInt uiIdleTime) = 0;

    /** Forces the input media to quit idling immediately.
        Called by the framework if the input media is requested to stop idling immediately.
        Classes derived from GUIInputMedia should implement StopIdle in such a way, that it interrupts any
        idling/sleeping code in GetEvent().
    */
    virtual void StopIdle() = 0;

protected:
    CGUIInputMedia()
    {
    };

    virtual ~CGUIInputMedia()
    {
    };
    /// Pointer to input media
    static CGUIInputMedia       *ms_pInputMedia;

private:

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUIInputMedia(const CGUIInputMedia& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUIInputMedia& operator=(const CGUIInputMedia& kSource);
};

#endif
