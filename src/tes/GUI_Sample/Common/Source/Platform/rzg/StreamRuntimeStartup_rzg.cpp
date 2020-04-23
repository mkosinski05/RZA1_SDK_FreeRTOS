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
*    MODULE:        StreamRuntimeStartup_rzg.cpp
*
*    Archive:       $URL: https://10.25.129.51:3690/svn/Common/StreamRuntime/src/platform/rzg/StreamRuntimeStartup_rzg.cpp $
*
*    Date created:  2005
*
*    Revision:      $Rev: 1609 $
*
*    Author:        ceu
*
*******************************************************************************
*   MODIFICATIONS
*******************************************************************************
*    ID
*    --------------------------------------------------------------------------
*    $Id: StreamRuntimeStartup_rzg.cpp 335 2016-03-14 14:20:03Z christian.wick $
*
******************************************************************************/

#include "GUIConfigDebug.h"
#include "MyGUI_SR.h"
#include "GUITrace.h"
#include "UserConfig.h"
#include "GUIResourceFileHandler.h"
#include "GUIImageCache.h"
#include "StreamRuntimeConfig.h"

// Graphic and Font Wrapper
#include "FntWrap.h"

#if defined GFX_USE_GL
    #include "GfxWrapOGLSDL.h"
    #include "FntWrapFreeTypeGL.h"
#elif defined GFX_USE_EGL
    #include "GfxWrapOGLES2.h"
    #include "FntWrapFreeTypeGL.h"
#elif defined GFX_USE_EGML
    #include "GfxFBDevice.h"
    #include "FntWrapFreeTypeeGML.h"
#endif

// Image Decoder
#include "GUIImageLoader.h"
#include "GUIImageDecoderBMP.h"
#include "GUIImageDecoderPNG.h"
#include "GUIImageDecoderJPG.h"
#include "GUIImageDecoderSVG.h"
#include "GUIImageDecoderRLE.h"
#include "GUIImageDecoderRAW.h"

// Sound Wrapper
#if defined SND_USE_NULL
#include "SndWrapNull.h"
#else
#include "SndWrapALSA.h"
//#include "SndWrapSDL.h"
//#include "SndWrapOSS.h"
#endif

// Input Wrapper
#if defined INPUT_USE_DEVICE_UNIX
#include "GUIFakeMouseCursor.h"
#include "GUIInputDeviceUnix.h"
#include "GUIInputUnixMouse.h" // Experimental Mouse Unix Input for Imperial
#else
#include "GUIInputSDL.h"
#endif
#include "GUINullInput.h"

#include "GUIException.h"
#include "GUIMemLeakWatcher.h"

// StreamRuntimeStartup_rzg.cpp : rzg specific default startup code for a Guiliani StreamRuntime
namespace NStreamRuntime
{

    static const eC_String DEFAULT_CONFIG_NAME("StreamRuntimeConfig.xml");

    void ConstructResourceClasses()
    {
GUILOGMESSAGE( "StreamRuntimeStartup_rzg.cpp NStreamRuntime::ConstructResourceClasses \n");
        // Sets the GraphicsWrapper
        try
        {
#if defined GFX_USE_EGL
            CGfxWrapOGLES2::CreateInstance();
            CFntWrapFreeTypeGL::CreateInstance();
            // add image-decoders
            GETIMGLOADER.AddDecoder(new CGUIImageDecoderBMP());
            GETIMGLOADER.AddDecoder(new CGUIImageDecoderPNG());
            GETIMGLOADER.AddDecoder(new CGUIImageDecoderJPG());
            GETIMGLOADER.AddDecoder(new CGUIImageDecoderSVG());
            GETIMGLOADER.AddDecoder(new CGUIImageDecoderRLE());
            GETIMGLOADER.AddDecoder(new CGUIImageDecoderRAW());
#elif defined GFX_USE_EGML
            CGfxFBDevice::CreateInstance(false);
            eGML_Screen* pkScreen = ((CGfxFBDevice&) GETGFX).GetScreen();
	    static_cast<CGfxWrapeGML&>(GETGFX).SetSubpixelEnabled(false);
            //create a fontwrapper for freetype with 1024 kByte cache.
	    CFntWrapFreeTypeeGML::CreateInstance(1024*1024,pkScreen);
#endif
        }
        catch (...)
        {
            GUILOG_THROW_EXCEPTION(CGUIException(), "CMyGui::Init: Could not allocate graphics wrapper.\n");
        }

        // Sets the SoundWrapper
        try
        {
#if defined SND_USE_NULL
            CSndWrapNull::CreateInstance();
#else
            CSndWrapALSA::CreateInstance();
            //CSndWrapSDL::CreateInstance();
            //CSndWrapOSS::CreateInstance();
#endif
        }
        catch (...)
        {
            GUILOG_THROW_EXCEPTION(CGUIException(), "CMyGui::Init: Could not allocate sound wrapper. \n");
        }

        try
        {
#if defined INPUT_USE_DEVICE_UNIX
            CGUIInputDeviceUnix::CreateInstance("/dev/input/event0", "/dev/input/touchscreen0", NULL, 2, 2, 1791, 1023, 10);
            //CGUIInputUnixMouse::CreateInstance("/dev/input/mouse0");
            //CGUIFakeMouseCursor::CreateInstance();
#else        
            CGUIInputSDL::CreateInstance();
#endif
        }
        catch (...)
        {
            GUILOG_THROW_EXCEPTION(CGUIException(), "CMyGUI::Init: Could not create input media.\n");
        }
        GETRESHANDLER.SetResourcePathPrefix("./");
    }

    void DestructResourceClasses()
    {
        //CGUIFakeMouseCursor::DeleteInstance();
        CGUIInputMedia::DeleteInstance();
        CSndWrap::DeleteInstance();
        CFntWrap::DeleteInstance();
        CGfxWrap::DeleteInstance();
    }

    eC_Bool LoadConfiguration(int argc, char** argv)
    {
        eC_String kConfigFileName;
        if (argc < 2)
        {
            GUILOGMESSAGE("Usage: " + eC_String(argv[0]) + " [config-file-name]\n");
            GUILOGMESSAGE("Trying default: " + DEFAULT_CONFIG_NAME + "\n");
            kConfigFileName = DEFAULT_CONFIG_NAME;
        }
        else
        {
            kConfigFileName = argv[1];
        }

        return GETRUNTIMECONFIG.LoadConfiguration(kConfigFileName);
    }
}

