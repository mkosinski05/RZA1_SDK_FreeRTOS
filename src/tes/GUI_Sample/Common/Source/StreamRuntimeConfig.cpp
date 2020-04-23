/*
 * Copyright (C) 2004 TES Electronic Solutions GmbH,
 * All Rights Reserved.
 * This source code and any compilation or derivative thereof is the
 * proprietary information of TES Electronic Solutions GmbH
 * and is confidential in nature.
 * Under no circumstances is this software to be exposed to or placed
 * under an Open Source License of any type without the expressed
 * written permission of TES Electronic Solutions GmbH
 *
 *############################################################
 */

/******************************************************************************
*   PROJECT:        Guiliani
*******************************************************************************
*
*    MODULE:        Configurator.cpp
*
*    Archive:       $URL: https://10.25.129.51:3690/svn/Common/branches/Releases/2.2/StreamRuntime/Source/StreamRuntimeConfig.cpp $
*
*    Date created:  2005
*
*
*
*    Author:        ifa
*
*******************************************************************************
*   MODIFICATIONS
*******************************************************************************
*    ID
*    --------------------------------------------------------------------------
*    $Id: StreamRuntimeConfig.cpp 421 2016-04-29 09:19:38Z christian.wick $
*
******************************************************************************/
#include "StreamRuntimeConfig.h"
#include "GUIStreamReader.h"
#include "GUIResourceFileHandler.h"
#include "GUIAutoPtr.h"
#include "GUIStreamReaderXML.h"
#include "GUITrace.h"
#include "GUIStreamingException.h"

#include "GUIMemLeakWatcher.h"


#define CONFIGURATOR_FILE_VERSION   2

namespace NStreamRuntime
{

Configurator& Configurator::GetInstance()
{
    static Configurator kInstance;
    return kInstance;
}

eC_Bool Configurator::LoadConfiguration(const eC_String &rkFileName)
{
    // StreamRuntimeConfig is always XML format!
    CGUIStreamReaderXML kStreamReader;
    eC_File* pkFile = GETRESHANDLER.Open(rkFileName);
    if (pkFile == NULL || !pkFile->IsOpen())
    {
        GUILOG(GUI_TRACE_ERROR, "Configurator::LoadConfiguration: Could not open file \""+rkFileName+"\".\n");
        return false;
    }
    else
    {
        GUILOG(GUI_TRACE_DEBUG, "Configurator::LoadConfiguration: Opened file \""+rkFileName+"\".\n");
    }
    GETINPUTSTREAM.SetStreamingFile(pkFile);
    try
    {
        GETINPUTSTREAM.DeleteCommentTag("Configuration");
        eC_Int uiFileVersion  = GETINPUTSTREAM.ReadStreamingFileHeader(); 

        if (uiFileVersion > CONFIGURATOR_FILE_VERSION)
        {
            CGUIStreamingException kEx(rkFileName, GETINPUTSTREAM.GetCurLine());
            GUILOG_THROW_EXCEPTION(kEx,
                "Configurator::LoadConfiguration: Invalid file version " + eC_String(uiFileVersion) + ".\n");
        }

        m_iScreenWidth        = GETINPUTSTREAM.READ_INT("ScreenWidth");
        m_iScreenHeight       = GETINPUTSTREAM.READ_INT("ScreenHeight");
        if (uiFileVersion >= 2)
        {
            m_bResizable      = GETINPUTSTREAM.READ_BOOL("Resizable");
        } 
        m_kResourcePathPrefix = GETINPUTSTREAM.READ_STRING("ResourcePathPrefix");
        m_kImageResFile       = GETINPUTSTREAM.READ_STRING("Images");
        m_kFontResFile        = GETINPUTSTREAM.READ_STRING("Fonts");
        m_kSoundResFile       = GETINPUTSTREAM.READ_STRING("Sounds");
        m_kGenResFile         = GETINPUTSTREAM.READ_STRING("GeneralResources");
        m_kDataPoolFile       = GETINPUTSTREAM.READ_STRING("DataPool");
        m_kPropResFile        = GETINPUTSTREAM.READ_STRING("Properties");
        m_kLanguageFile       = GETINPUTSTREAM.READ_STRING("Language");
        m_bBkgEnabled         = GETINPUTSTREAM.READ_BOOL("BackgroundEnabled");
        m_uiBkgColor          = GETINPUTSTREAM.READ_HEX("BackgroundColor");
        m_kDialogFile         = GETINPUTSTREAM.READ_STRING("Dialog");
        m_eStreamingMode      = static_cast<StreamingMode_t>(GETINPUTSTREAM.READ_INT("StreamingMode"));
        GETINPUTSTREAM.DeleteCommentTag("/Configuration");
    }
    catch (...)
    {
        GETINPUTSTREAM.ResetStreamingFile(); // because else it clashes with the AutoPtr
        GETRESHANDLER.Close(pkFile);

        CGUIStreamingException kEx(rkFileName, GETINPUTSTREAM.GetCurLine());
        GUILOG( GUI_TRACE_ERROR, kEx.Report());
        GUILOG_THROW_EXCEPTION(kEx,"Configurator::LoadConfiguration: Caught an exception while reading the configuration file.\n");
        return false;
    }

    GETINPUTSTREAM.ResetStreamingFile();
    GETRESHANDLER.Close(pkFile);

    return true;
}

}
