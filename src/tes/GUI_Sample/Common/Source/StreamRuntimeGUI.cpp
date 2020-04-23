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
*    MODULE:        StreamRuntimeGUI.cpp
*
*    Archive:       $URL: https://10.25.129.51:3690/svn/Common/branches/Releases/2.2/StreamRuntime/Source/StreamRuntimeGUI.cpp $
*
*    Date created:  2005
*
*
*
*    Author:        JRE
*
*******************************************************************************
*   MODIFICATIONS
*******************************************************************************
*    ID
*    --------------------------------------------------------------------------
*    $Id: StreamRuntimeGUI.cpp 610 2018-03-16 10:51:50Z sascha.sippel $
*
******************************************************************************/
#include "StreamRuntimeGUI.h"
#include "StreamRuntimeConfig.h"
#include "GUIProperties.h"
#include "GUILocalisationHandler.h"
#include "GUIResourceFileHandler.h"
#include "GUIResourceManager.h"
#include "GUIPropertyResource.h"
#include "GfxWrap.h"
#include "GUIFactoryManager.h"
#include "CustomExtensionFactory.h"
#include "GUITrace.h"
#include "GUIStreamReaderXML.h"
#include "GUIStreamReaderBinary.h"
#include "CustomExtensionFuncs.h"
#include "CustomStandardFactory.h"
#include "GUIException.h"
#include "GUIDataPool.h"

#include "GUIMemLeakWatcher.h"


namespace NStreamRuntime
{

CStreamRuntimeGUI::CStreamRuntimeGUI(
    eC_Value x, eC_Value y,
    eC_Value width, eC_Value height,
    ObjectHandle_t eID)
:
    CGUI (x, y, width, height, eID),
    m_pkStreamReader(NULL,false)
{
    Init();
}

CStreamRuntimeGUI::~CStreamRuntimeGUI()
{
    CGUIFactoryManager::DeleteInstance();
}

void CStreamRuntimeGUI::Init()
{
    //we set ResourcePathPrefix from config file only if it's not empty.
    eC_String kResourcePathPrefix = GETRUNTIMECONFIG.GetResourcePathPrefix();
    if (!kResourcePathPrefix.IsEmpty())
    {
        GETRESHANDLER.SetResourcePathPrefix(kResourcePathPrefix);
    }
    
    // Dynamically registered resources shall not be automatically unloaded if their request count reaches zero.
    // In a Gui created within GSE basically ALL resources will be dynamic.
    GETRESMANAGER.KeepDynamicImages(true);
    GETRESMANAGER.KeepDynamicFonts(true);
    GETRESMANAGER.KeepDynamicSounds(true);
    GETRESMANAGER.KeepDynamicGeneralResources(true);
    
    try
    {
        try
        {
            CGUIFactoryManager::CreateInstance();
            // Add the factory for Guiliani's standard widgets first
#ifdef STREAMRUNTIME_CUSTOM_STANDARD_FACTORY_EXISTS
            GETFACTORY.AddUserFactory(new CustomStandardFactory());
#else
            GETFACTORY.AddUserFactory(new CGUIStandardFactory());
#endif
            // Add factory for custom extensions
            GETFACTORY.AddUserFactory(new CustomExtensionFactory());
            // Dependant on StreamRuntime configuration the necessary StreamReader is
            // created (XML or binary streaming mode).
            switch(GETRUNTIMECONFIG.GetStreamingMode())
            {
                case Configurator::STREAMING_MODE_XML:
                    // XML based dialog files.
                    m_pkStreamReader = new CGUIStreamReaderXML();
                    break;
                case Configurator::STREAMING_MODE_BINARY:
                    // Guiliani binary based dialog files.
                    m_pkStreamReader = new CGUIStreamReaderBinary();
                    break;
            }
        }
        catch(...)
        {
            GUILOG_THROW_EXCEPTION(CGUIException(), "Init: CGUIFactoryManager and StreamReaders could not be created.\n");
        }

        // Now let's start the GUI:
        // let the factory create the objects, getting the arguments for the construction from the streaming file
        StreamGUI();

        // Trigger an initial refresh of the screen
        GETGFX.InvalidateRect(0,0, eC_FromInt (GETGFX.GetPhysicalScreenWidth()), eC_FromInt(GETGFX.GetPhysicalScreenHeight()) );
    }
    catch(...)
    {
        // As the CGUI destructor functionality is not automatically called when leaving via an exception
        // we clean up here explicitly
        RemoveAllChildren();
        throw;
    }
}

void CStreamRuntimeGUI::StreamGUI()
{
    // Load the initial language
    eC_String kFileName = GETRUNTIMECONFIG.GetLanguageFileName();
    if (!kFileName.IsEmpty())
    {
        GETLOCALEHDL.LoadLocalisationFile(kFileName);
    }

    // Register images, fonts, sounds etc. via resource files (if configured)
    kFileName = GETRUNTIMECONFIG.GetPropertyFileName();
    if (!kFileName.IsEmpty())
    {
        GETPROPHDL.ReadGlobalPropertiesFromFile(kFileName);
    }

    kFileName = GETRUNTIMECONFIG.GetImageFileName();
    if (!kFileName.IsEmpty())
    {
        GETRESMANAGER.RegisterImageResourcesFromFile(kFileName);
    }

    kFileName = GETRUNTIMECONFIG.GetFontFileName();
    if (!kFileName.IsEmpty())
    {
        GETRESMANAGER.RegisterFontResourcesFromFile(kFileName);
    }

    kFileName = GETRUNTIMECONFIG.GetSoundFileName();
    if (!kFileName.IsEmpty())
    {
        GETRESMANAGER.RegisterSoundResourcesFromFile(kFileName);
    }

    kFileName = GETRUNTIMECONFIG.GetGenResFileName();
    if (!kFileName.IsEmpty())
    {
        GETRESMANAGER.RegisterGeneralResourcesFromFile(kFileName);
    }

    // Load the DataPool file
    kFileName = GETRUNTIMECONFIG.GetDataPoolFileName();
    if (!kFileName.IsEmpty())
    {
        CGUIDataPool::ReadDataPoolFromFile(kFileName);
    }

    // Load initial dialog
    kFileName = GETRUNTIMECONFIG.GetDialogFileName();
    if (kFileName.IsEmpty())
    {
        GUILOG(GUI_TRACE_ERROR, "StreamGUI: No dialog name configured.\n");
    }
    else
    {
        RemoveAllChildren();
        AddObject(GETFACTORY.LoadDialogFromFile(kFileName));
    }
}

eC_Bool CStreamRuntimeGUI::DoDraw()
{
    // If the configuration requests us to fill the background in a given color
    if (!GETRUNTIMECONFIG.IsBackgroundEnabled())
        return false;
    // Set color
    GETGFX.SetForegroundColor(GETRUNTIMECONFIG.GetBackgroundColor());
    // Fill the GUI area with this color
    GETGFX.FilledRect(GetAbsRect());
    return true;
}



}
