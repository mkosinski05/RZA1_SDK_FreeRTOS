/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(STREAMRUNTIMECONFIG_H)
#define STREAMRUNTIMECONFIG_H

#include "eC_String.h"

/** The NStreamRuntime Namespace contains generic helper classes for initializing a default StreamRuntime application.
    The code herein typically remains widely unchanged across most StreamRuntimes.
    Feel free to remove these classes from your own application and replace them with custom code, if you wish.*/
namespace NStreamRuntime
{

#define GETRUNTIMECONFIG (NStreamRuntime::Configurator::GetInstance())

/** Helper class for reading the StreamRuntime Configuration from an XML file.
    The configuration does e.g. specifiy the first dialog to show after startup, the initial language file,
    image set, screen resolution etc. */
class Configurator
{
public:
    // Specifices in which format to read the streaming files
    enum StreamingMode_t
    {
        STREAMING_MODE_XML,
        STREAMING_MODE_BINARY
    };

    static Configurator& GetInstance();
    eC_Bool LoadConfiguration(const eC_String& rkFileName);

    inline eC_Int GetScreenWidth() { return m_iScreenWidth; }
    inline eC_Int GetScreenHeight() { return m_iScreenHeight; }
    inline eC_Bool IsWindowResizable() { return m_bResizable; }
    inline const eC_String& GetResourcePathPrefix() { return m_kResourcePathPrefix; }
    inline const eC_String& GetImageFileName() { return m_kImageResFile; }
    inline const eC_String& GetFontFileName() { return m_kFontResFile; }
    inline const eC_String& GetSoundFileName() { return m_kSoundResFile; }
    inline const eC_String& GetGenResFileName() { return m_kGenResFile; }
    inline const eC_String& GetDataPoolFileName() { return m_kDataPoolFile; }
    inline const eC_String& GetPropertyFileName() { return m_kPropResFile; }
    inline const eC_String& GetLanguageFileName() { return m_kLanguageFile; }
    inline eC_Bool IsBackgroundEnabled() { return m_bBkgEnabled; }
    inline eC_UInt GetBackgroundColor() { return m_uiBkgColor; }
    inline eC_String GetDialogFileName() { return m_kDialogFile; }
    inline StreamingMode_t GetStreamingMode() { return m_eStreamingMode; }

private:
    Configurator() {};
    ~Configurator() {};

    eC_String m_kConfigFileName;
    eC_Int m_iScreenWidth;
    eC_Int m_iScreenHeight;
    eC_Bool m_bResizable;
    eC_String m_kResourcePathPrefix;
    eC_String m_kImageResFile;
    eC_String m_kFontResFile;
    eC_String m_kSoundResFile;
    eC_String m_kGenResFile;
    eC_String m_kDataPoolFile;
    eC_String m_kPropResFile;
    eC_String m_kLanguageFile;
    eC_Bool m_bBkgEnabled;
    eC_UInt m_uiBkgColor;
    eC_String m_kDialogFile;
    StreamingMode_t m_eStreamingMode;
};
}
#endif // STREAMRUNTIMECONFIG_H
