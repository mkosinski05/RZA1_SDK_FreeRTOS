/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef SNDWRAP__H_
#define SNDWRAP__H_

#include "eC_String.h"
#include "eC_Types.h"
#include "GUISoundResource.h"

/// Retrieve pointer to Sound Wrapper instance
#define GETSND              CSndWrap::GetInstance()

// define flags
#define PLAY_SYNC           1
#define PLAY_ASYNC          2
#define PLAY_WAIT           4
#define PLAY_DISCARD        8
#define PLAY_IMMEDIATELY    16


/// This is the Sound-Wrapper base class.

/**
    Guiliani is designed independently of a certain underlying sound library, in order to allow its
    porting to any desired platform. Abstraction of sound functionality is guaranteed through the use
    of sound-wrappers. This is the base-class, from which all wrappers customized for a specific
    sound library, have to be derived.<p>

    The methods declared in this base-class will be used by Guiliani standard-sounds to output
    themselves. You may leave implementations of certain methods empty for your specific derivation, if
    you know that you will not need them. For example you may decide not to implement sound-loading
    functionality for your specific sound-wrapper. You are also free to add completely new methods
    to your own wrapper-classes and use these within your custom-made controls.

    warning! this code has been only tested for specific application and is not in a final state!
    operating without mistakes in any case can not be guaranteed!
*/
class CSndWrap
{
    /*  the resourcemanager is the only class which may access the loadsnd and
        unloadsnd-methods of the sndwrapped directly. therefore it is granted
        friend-access.
    */
    friend class CGUIResourceManager;

public:
    /** return pointer to Sound-Wrapper instance.
        @return Pointer to Sound-Wrapper instance.
    */
    static inline CSndWrap& GetInstance ()
    {
        return *ms_pSndWrap;
    }

    /// Deletes the Sound-Wrapper instance.
    static void DeleteInstance();

    /** Method to get the volume of an output device.
       The volume has to be implemented in percent.
       The range is from 0 till 100.
       @return The currently set volume in percent.
    */
    virtual eC_UByte GetVolume() = 0;

    /** Method to break playback on a specified sound.
        @param eID ID of the sound you want to pause.
        @return False on error.
    */
    virtual eC_Bool PauseSnd(const SoundResource_t &eID) = 0;

    /** Method to send data to the specified output device.
        @param eID ID of the sound you want to play.
        @param uiFlags Flags to set for playing.
               possible flags:
               PLAY_SYNC        - wait until playback has finished before returning
               PLAY_ASYNC       - return immediately to calling function
               PLAY_WAIT        - if a sound is currently playing, wait for the playing
                                  sound to end, then play the new sound
               PLAY_DISCARD     - if a sound is currently playing, return without playing
                                  the new sound
               PLAY_IMMEDIATELY - if a sound is currently playing, immediately stop playback
                                  and play the new sound

        @return False on error.
    */
    virtual eC_Bool PlaySnd(const SoundResource_t &eID, eC_UInt uiFlags = PLAY_ASYNC) = 0;

    /** Method for continuing the playback of a specified sound.
        @param eID ID of the sound you want to resume.
        @return False on error.
    */
    virtual eC_Bool ResumeSnd(const SoundResource_t &eID) = 0;

    /** Method to set the volume of an output device.
       The volume has to be implemented in percent.
       The range is from 0 till 100.
       @param ubVolume to set in percent.
       @return False on error.
    */
    virtual eC_Bool SetVolume(eC_UByte ubVolume) = 0;

    /** Method to stop playback of a specified sound.
        @param eID ID of the sound you want to stop.
        @return False on error.
    */
    virtual eC_Bool StopSnd(const SoundResource_t &eID) = 0;

    /** Gets the number of sounds.
        @return Number of sounds.
    */
    inline virtual eC_UInt GetNOFSounds() {return m_uiNOFSounds;}

protected:
    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default.
        @param kSource The source to be copied
    */
    CSndWrap(const CSndWrap& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default.
        @param kSource The source to be assigned from
        @return The assignment destination
    */
    CSndWrap& operator=(const CSndWrap& kSource);

    /// Constructor
    CSndWrap();

    /// Destructor
    virtual ~CSndWrap();

    /** Sets the total number of sounds. This is called internally by the ResourceManager when dynamically
        allocated sound resources require the total number of sounds to be raised.
        @param uiNOFSounds Number of sounds to set.
        @return True if successfully set, False otherwise
    */
    virtual eC_Bool SetNOFSounds(eC_UInt uiNOFSounds);

    /** Method to load a sound file.
        @param kPath Path to the sound file.
        @param eID ID of the sound.
        @param uiDeviceID TBD
    */
    virtual void LoadSnd(const eC_String& kPath, const SoundResource_t &eID, eC_UInt uiDeviceID = 0) = 0;

    /** Method to unload a sound file.
        @param eID ID of the sound to unload.
    */
    virtual void UnloadSnd(const SoundResource_t &eID) = 0;

    /// static protected member pointer to Sound-Wrapper instance.
    static CSndWrap* ms_pSndWrap;

    /// total number of sound-objects.
    eC_UInt m_uiNOFSounds;
};
#endif
