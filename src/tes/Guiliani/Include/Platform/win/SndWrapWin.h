/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef SNDWRAPWIN__H_
#define SNDWRAPWIN__H_

#include <windows.h>
#include "SndWrap.h"

# ifndef MMSYSTEM__H_
    # define MMSYSTEM__H_
    #include <mmsystem.h>
#endif

/// This is the Windows implementation of the sound wrapper.

/**
    For details see desription of its base class CSndWrap.
    warning! this code has been only tested for specific application and is not in a final state!
    operating without mistakes in any case can not be guaranteed!
    only wave_format_pcm is supported until now
*/
class CSndWrapWin:public CSndWrap
{
public:
    /**
        Create singleton instance and change the pointer in the
        base class to this sound wrapper.
    */
    static void CreateInstance();

    /// Method to give the sound library the opportunity to clean up
    eC_Bool DeInit (void);

    /// Method to initialize the sound library
    eC_Bool Init(void);

    eC_UByte GetVolume ();
    void LoadSnd (const eC_String &psPath, const SoundResource_t &eID, eC_UInt uiDeviceID = 0);
    eC_Bool PauseSnd (const SoundResource_t &eID);
    eC_Bool PlaySnd (const SoundResource_t &eID, eC_UInt Flags = PLAY_ASYNC);
    eC_Bool ResumeSnd (const SoundResource_t &eID);
    eC_Bool SetVolume (eC_UByte ubVolume);
    eC_Bool StopSnd (const SoundResource_t &eID);
    void UnloadSnd (const SoundResource_t &eID);

protected:
    /// Constructor
    CSndWrapWin (void);

    /// Destructor
    ~CSndWrapWin (void);

private:
    // structure of internal sound-data
    struct SoundData_t
    {
        eC_Bool     bPaused;
        WAVEHDR     kSound;
        HWAVEOUT    kWaveOut;
    };

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CSndWrapWin(const CSndWrapWin& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CSndWrapWin& operator=(const CSndWrapWin& kSource);

    /** Sets the total number of sounds. This is called internally by the ResourceManager when dynamically
        allocated sound resources require the total number of sounds to be raised. */
    eC_Bool SetNOFSounds(eC_UInt uiNOFSounds);

    eC_TArray<SoundData_t*> m_kSoundData;   ///< internal data structure for management
};
#endif
