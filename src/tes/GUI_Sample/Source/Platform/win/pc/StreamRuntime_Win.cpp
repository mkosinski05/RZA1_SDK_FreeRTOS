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
*    MODULE:        StreamRuntime_Win.cpp
*
*    Archive:       $URL: https://10.25.129.51:3690/svn/GSE/branches/Releases/1.0_Guiliani_2.1/StreamRuntime/src/platform/win/pc/StreamRuntime_Win.cpp $
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
*    $Id: StreamRuntime_Win.cpp 2186 2015-01-09 16:01:25Z christian.euler $
*
******************************************************************************/

// StreamRuntime_Win.cpp : Main entry point for a Guiliani StreamRuntime application
// In this file all platform specific code should reside.

#include "StreamRuntimeStartup_Win.h"
#include "StreamRuntimeConfig.h"
#include "MyGUI_SR.h"
#include "UserConfig.h"
#include "GUITrace.h"
#include "GUIMemLeakDetection.h"
#include "GUIResourceFileHandler.h"

#include "GUIMemLeakWatcher.h"



int WINAPI WinMain(
    IN HINSTANCE hInstance,
    IN HINSTANCE hPrevInstance,
    IN LPSTR lpCmdLine,
    IN int nShowCmd)
{
    // Enable logging to a file
    GUI_REG_FILE_TRACE("StreamRuntime.log");
    // Enable logging to the Visual Studio Debugger
    GUI_REG_DEBUGGER_TRACE();
    // Enable logging to STDOUT
    GUI_REG_STDOUT_TRACE();

    try
    {
        // Comment in to use a resource file containing all required images, fonts, texts etc.
        GETRESHANDLER.SetResourceFile("Resources.dat");
        GETRESHANDLER.SetFileOpenOrder(CGUIResourceFileHandler::RESOURCE_FILE_FIRST);

        // Use CGUI::SetGuilianiLicenseKey("<ENTER YOUR CODE HERE>")) to set your Guiliani license key via code.
        // Alternatively use CGUI::LoadGuilianiLicenseKey("GuilianiLicense.key") to read your Guiliani license key from a file.

        // Load configuration (e.g. ScreenSize, initial Dialog etc.) from file
        if( !GETRUNTIMECONFIG.LoadConfiguration("StreamRuntimeConfig.xml") )
        {
            return 0; // Failed to load configuration file.
        }
        // Initialize the application window
        NStreamRuntime::Init(hInstance, nShowCmd);
        // Initialize platform specific wrappers for Graphics, Fonts, Input etc. 
        NStreamRuntime::ConstructWrapperClasses(); // Use default initialization of wrappers for Windows

        // Create the root GUI object derived from CGUI
        CMyGUI kMyGui(
            eC_FromInt(0),
            eC_FromInt(0),
            eC_FromInt(GETRUNTIMECONFIG.GetScreenWidth()),
            eC_FromInt(GETRUNTIMECONFIG.GetScreenHeight()),
            NO_HANDLE);

        // Now start the GUI. This will return when the application quits.
        kMyGui.Run();
    }
    catch(...)
    {
        MessageBox( NULL, TEXT("StreamRuntime performed a clean shutdown after an exception was caught. \nPlease check StreamRuntime.log for further details on the problem."), TEXT("StreamRuntime encountered a problem"), 0);
    }

    // Shutdown and clean up
    NStreamRuntime::DestructWrapperClasses();

#ifdef GUILIANI_LEAK_DETECTION
    LEAK_DETECTION.ShowResults();
    // Comment in to write leak detection output to a file
    //LEAK_DETECTION.WriteResultsToFile("StreamRuntime_MemoryLeaks.log");
#endif
    return 0;
}
