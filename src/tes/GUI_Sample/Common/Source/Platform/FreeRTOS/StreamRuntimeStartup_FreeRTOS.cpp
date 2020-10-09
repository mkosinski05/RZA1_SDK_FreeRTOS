/******************************************************************************
*   PROJECT:        Guiliani
*******************************************************************************
*
*    MODULE:        StreamRuntimeStartup_FreeRTOS.cpp
*
*    Archive:       $URL: https://10.25.129.51:3690/svn/Common/branches/Releases/2.2/StreamRuntime/Source/Platform/FreeRTOS/StreamRuntimeStartup_FreeRTOS.cpp $
*
*    Date created:  2014
*
*    Revision:      $Rev: 1650 $
*
*    Author:        THE
*
*******************************************************************************
*   MODIFICATIONS
*******************************************************************************
*    ID
*    --------------------------------------------------------------------------
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

#ifdef USE_LIB_RGA
    #include "GfxWrapRGA.h"
    #include "FntWrapFreeTypeRGA.h"
#else
    #include "GfxWrapeGML.h"
    #include "FntWrapFreeTypeeGML.h"
#endif

#include "FntWrapNull.h"

// Sound Wrapper
#include "SndWrapNull.h"

// Input Wrapper
#ifdef PLATFORM_RENESAS_DISPLAYIT_V1      
    #include "GUIInputDisplayIt.h"
#elif defined PLATFORM_RENESAS_STREAMIT_V2
    #include "GUIInputStreamIt.h"
#else
    #include "GUIInputExample.h"
#endif

#include "GUIConfigDebug.h"
#include "GUIException.h"
#include "GUIThread.h"
#include "GUIPlaybackInput.h"

// Memory Leak Detection
#include "GUIMemLeakWatcher.h"

namespace NStreamRuntime
{
  static const eC_String DEFAULT_CONFIG_NAME("StreamRuntimeConfig.xml");

  void ConstructResourceClasses()
  {
    /* Sets the graphics wrapper */
    try
    {
#ifdef USE_LIB_RGA 
      CGfxWrapRGA::CreateInstance(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
#else
      CGfxWrapeGML::CreateInstance(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
#endif
      GETGFX.SetDoubleBufferingEnforcesFlipping(true);
    }
    catch(...)
    {
      GUILOG_THROW_EXCEPTION(CGUIException(), "NStartup::ConstructResourceClasses: Could not allocate graphics wrapper.\n");
    }

    // Sets the font wrapper
    try
    {
#ifdef USE_LIB_RGA 
      CFntWrapFreeTypeRGA::CreateInstance(512*512, (static_cast<CGfxWrapRGA&> (CGfxWrapRGA::GetInstance())).GetContext(),
                                                   (static_cast<CGfxWrapRGA&> (CGfxWrapRGA::GetInstance())).GetFrameBuffer());
#else
      CFntWrapFreeTypeeGML::CreateInstance(512*512, (static_cast<CGfxWrapeGML&> (CGfxWrapeGML::GetInstance())).GetScreen());
#endif
      //CFntWrapNull::CreateInstance();
    }
    catch(...)
    {
      GUILOG_THROW_EXCEPTION(CGUIException(), "NStartup::ConstructResourceClasses: Could not allocate font wrapper.\n");
    }

    // Sets the sound wrapper
    try
    {
      CSndWrapNull::CreateInstance();
    }
    catch (...)
    {
      GUILOG_THROW_EXCEPTION(CGUIException(), "NStartup::ConstructResourceClasses: Could not allocate sound wrapper.\n");
    }

    // Set pointer to resource file handler instance
    try
    {
#ifdef PLATFORM_RENESAS_DISPLAYIT_V1      
      CGUIInputDisplayIt::CreateInstance();
#elif defined PLATFORM_RENESAS_STREAMIT_V2
      CGUIInputStreamIt::CreateInstance(0, 0, 480, 272, 10);
#else
      CGUIInputExample::CreateInstance();
#endif
    }
    catch(...)
    {
      GUILOG_THROW_EXCEPTION(CGUIException(), "NStartup::ConstructResourceClasses: Could not create input media.\n");
    }
    GETRESHANDLER.SetResourcePathPrefix("./");
  }


  void DestructResourceClasses()
  {
    CGUIInputMedia::DeleteInstance();
    CSndWrap::DeleteInstance();
    CFntWrap::DeleteInstance();
    CGfxWrap::DeleteInstance();
  }

  eC_Bool LoadConfiguration()
  {
    eC_String kConfigFileName(DEFAULT_CONFIG_NAME);

    GUILOGMESSAGE( "Trying default: "+DEFAULT_CONFIG_NAME+"\n");

    return GETRUNTIMECONFIG.LoadConfiguration(kConfigFileName);
  }
} // namespace
