/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef _GUIPLAYBACKINPUT_H_
#define _GUIPLAYBACKINPUT_H_

#include "eC_Types.h"
#include "GUIInputMedia.h"
#include "eC_TList_doubleLinked.h"
#include "eC_String.h"

class CGUIEvent;



/// This is the GUI playback input media, which records user-input for later playback.
// @guiliani_doxygen toplevel_module Playback
/**
    <table border="0">
    <tr>
        <td width="200">@image html playback.png</td>
            <td>
                The "playback" module is used as a recorder for all mouse and keyboard events. GUI events can be recorded to a file and can be played back again like an old tape deck (Class: CGUIPlaybackInput).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    It is designed to record mouse-input or keyboard-input, and playback the recorded events at a later time, for
    example for displaying self-running demos on a fair.

@section sec_cguiplaybackinput_automatic_mode Using CGUIPlaybackInput in automatic mode

    The easiest way to use this class as follows:
    1. Create an instance of your "normal" Input media as always.
    2. Create an instance of GUIPlaybackInput

    Example:
@code
        CGUIInputWindows::CreateInstance(hWnd);
        CGUIPlaybackInput::CreateInstance();
@endcode

    The playbackinput will then intercept all events returned by the actual input media and when the
    program is shut down, it will write the logged events into a file called "EventRecord.dat" (unless you specify another name).

    PLAYBACK MODE:
    At startup CGUIPlaybackInput search for this file. If it is found, the PlaybackInputMedia will read the
    events and automatically shoot them into the application in the correct time-order.

    RECORD MODE:
    If the PlaybackInputMedia does NOT find the "EventRecord.dat" file, it will automatically enter
    Record mode, and log all events to such a file during shutdown.


@section sec_cguiplaybackinput_manual_mode Using CGUIPlaybackInput in manual mode

    If you require more control over the behaviour of CGUIPlaybackInput, you may wish to use it in manual mode.
    In this case you need to start Playback / Recording of events explicitly via the interfaces StartPlayback(), StopPlayback(),
    StartRecording() and StopRecording()

    Example:
@code
        CGUIInputWindows::CreateInstance(hWnd);
        CGUIPlaybackInput::CreateInstance(false, "other_resources/EventRecord.dat");
        // Somewhere within the application's code...
        static_cast<CGUIPlaybackInput&>(GETINPUTMEDIA).StartPlayback();
        static_cast<CGUIPlaybackInput&>(GETINPUTMEDIA).StopPlayback();
@endcode

    ATTENTION: If you record the events in PC, write them in file and want to simulate in a target,
               you should always check whether the size of the type in the target is same as in PC.
@ingroup GUILIANI_DEBUGGING
*/
class CGUIPlaybackInput
    : public CGUIInputMedia
{
public:
    /**
    Singleton for creating a Playback Input Media.
    @param bAutomaticMode When set to TRUE CGUIPlaybackInput will automatically search for an existing "EventRecord.dat" (or your supplied name) at Startup and enter playback mode if it was found.
                          When set to FALSE you must manually start/end record and playback using the StartPlayback(), StopPlayback(), StartRecording() and StopRecording() APIs.
    @param kEventRecordFilename Filename from which previously recorded events will be loaded. (And to which they will be saved in case you are recording)
    */
    static void CreateInstance(const eC_Bool bAutomaticMode = true, const eC_String& kEventRecordFilename= "EventRecord.dat");

    /** Gets an event from the actual input media OR the internal list of recorded events,
        depending on whether the PlaybackInputMedia is in record or playback mode
        @param uiIdleTime The idle time
        @return The event from the queue
    */
    CGUIEvent* GetEvent(eC_UInt uiIdleTime);

    /// Forwards the StopIdle call to the actual input media
    virtual void StopIdle();

    /// Tries to load a file with previously recorded events and commences playback if successful. The filename can be given during construction or via SetEventRecordFilename().
    void StartPlayback();

    /// Stops any ongoing playback
    void StopPlayback();

    /// Starts recording events from the actual input media
    void StartRecording();

    /// Stops recording events and writes them to the file. @see SetEventRecordFilename
    void StopRecording();

    /** Specifies whether a running playback will immediately be stopped when there is an event received from the actual input media.
        Note that this will only take effect 1 second after starting a playback, in order to give the user a chance to e.g. start playback by clicking on a button, without accidentally stopping
        the playback immediately again.
        By default, StopPlaybackOnUserInput is set to true.
        @param bStopPlaybackOnUserInput If true, then any ongoing playback will be stopped immediately after receiving an event from the actual input media.
    */
    void StopPlaybackOnUserInput( const eC_Bool bStopPlaybackOnUserInput) { m_bStopPlaybackOnUserInput = bStopPlaybackOnUserInput; }

    /** Sets the name of the file containing previously recorded events. This file will be used for successive calls to StartPlayback().
        @param kEventRecordFilename Name of file containing recorded events.
    */
    void SetEventRecordFilename( const eC_String& kEventRecordFilename) { m_kEventRecordFilename = kEventRecordFilename; }

protected:
    /** Constructor
        @param pInputMedia The input media
        @param bAutomaticMode True for automatic mode
        @param kEventRecordFilename The file name of the record
    */
    CGUIPlaybackInput(CGUIInputMedia* pInputMedia, const eC_Bool bAutomaticMode, const eC_String& kEventRecordFilename);
    ~CGUIPlaybackInput();

private:

    // Structure to hold recorded events
    struct RecordedEvent
    {
        eC_UInt uiTime;         // Time offset at which this event occurred
        CGUIEvent*  pEvent;     // pointer to actual CGUI Event containing all further information
    };

    /** Reads recorded Events from file
        @return True if successful, False otherwise */
    eC_Bool ReadRecordedEvents();
    /** Writes recorded Events to file */
    void WriteRecordedEvents();
    /** Logs an event into the internal event list */
    void RecordEvent( CGUIEvent* pEvent);
    /** Retrieves an event from the internal event list */
    CGUIEvent* GetRecordedEvent();

    /** Returns the time until the next event will occur. */
    eC_Int GetTimeUntilNextEvent();

    /// Clears the list of recroded events and delets any contained CGUIEvent-objects
    void ClearEventRecordList();

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUIPlaybackInput(const CGUIPlaybackInput& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CGUIPlaybackInput& operator=(const CGUIPlaybackInput& kSource);

    /// Boolean flag indicating whether the input device is in record mode
    eC_Bool m_bIsRecording;

    /// Boolean flag indicating whether the input device is in playback mode
    eC_Bool m_bIsPlaying;

    ///  Boolean flag indicating whether playback will be stopped automatically when receiving an event from the actual input media
    eC_Bool m_bStopPlaybackOnUserInput;

    /// List of recorded events
    eC_TListDoubleLinked <RecordedEvent> m_EventRecordList;

    /// Start time index for recording/playback
    eC_UInt m_uiStartTime;

    /// The actual Input Media, whose input is being recorded.
    CGUIInputMedia*  m_pActualInputMedia;

    /// Name of the file containing the recorded Events
    eC_String m_kEventRecordFilename;
};
#endif
