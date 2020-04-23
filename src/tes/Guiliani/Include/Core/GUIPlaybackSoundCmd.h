/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIPLAYBACKSOUNDCMD__H_
#define GUIPLAYBACKSOUNDCMD__H_

#include "GUICommand.h"
#include "GUISoundResource.h"

/// Plays a sound when executed.
class CGUIPlaybackSoundCmd
    : public CGUICommand
{
public:
    /** Constructor.
        @param eSoundID ID of the sound to be played.
      */
    CGUIPlaybackSoundCmd(const SoundResource_t eSoundID);

    /** Constructor that registers a dynamic sound.
        @param rkFileName The file name for which the dynamic sound is
               registered.
      */
    CGUIPlaybackSoundCmd(const eC_String& rkFileName);

    /** Destructor.
        Releases the internal sound ID.
      */
    virtual ~CGUIPlaybackSoundCmd();

#if defined(GUILIANI_STREAM_GUI)
    CGUIPlaybackSoundCmd();
    virtual void ReadFromStream();
#endif

#if defined(GUILIANI_WRITE_GUI)
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    /// Plays the sound.
    virtual void Do();

    /// ID of the sound to be played.
    SoundResource_t m_eID;

private:
    /// Helper for constructors.
    void Init();
};

#endif
