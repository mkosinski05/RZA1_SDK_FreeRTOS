/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef SNDWRAPNULL__H_
#define SNDWRAPNULL__H_

#include "SndWrap.h"
#include "GUITrace.h"

/// This is the Null Sound-Wrapper class.

/**
    It implements a null sound wrapper for Guiliani testing.
*/
class CSndWrapNull:
    public CSndWrap
{
public:
    /**
    Singleton for creating a GfxWrap-Null object and change the pointer in
    base class to this graphics wrap.
    */
    inline static void CreateInstance()
    {
        if(NULL == ms_pSndWrap)
        {
            ms_pSndWrap = new CSndWrapNull();
        }
    }

    inline eC_UByte GetVolume () {return 0;}
    inline eC_Bool PauseSnd (const SoundResource_t &eID) {return false;}
    inline eC_Bool PlaySnd (const SoundResource_t &eID, eC_UInt uiFlags = PLAY_ASYNC) {return false;}
    inline eC_Bool ResumeSnd (const SoundResource_t &eID) {return false;}
    inline eC_Bool SetVolume (eC_UByte ubVolume) {return false;}
    inline eC_Bool StopSnd (const SoundResource_t &eID) {return false;}
    inline eC_Bool SetNOFSounds(eC_UInt uiNOFSounds) {m_uiNOFSounds=uiNOFSounds; return true;}
    inline void LoadSnd (const eC_String& kPath, const SoundResource_t &eID, eC_UInt uiDeviceID = 0) {}
    inline void UnloadSnd (const SoundResource_t &eID) {}

private:
    /// Constructor
    CSndWrapNull (void)
    {
        GUILOG(GUI_TRACE_WARNING, "------------------------\n");
        GUILOG(GUI_TRACE_WARNING, "|  SndWrapNull chosen  |\n");
        GUILOG(GUI_TRACE_WARNING, "------------------------\n");
    }

    /** Copy-constructor.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CSndWrapNull(const CSndWrapNull& kSource);

    /** Operator= method.
        Dummy implementation to avoid unintended use of compiler-generated default    */
    CSndWrapNull& operator=(const CSndWrapNull& kSource);
};
#endif
