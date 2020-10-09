// Standard Headers
#include <new>
#include <string.h>
extern "C"{
#include <stdbool.h>
#include "control.h"
#include "r_devlink_wrapper.h"
#include "r_led_drv_api.h"
#include <stdlib.h>
#include <stdio.h>
#include "r_os_abstraction_api.h"
}
#include <fcntl.h>
#include "freertos.h"
// Guiliani Headers
#include "GUIConfigDebug.h"
#include "MyGUI_SR.h"
#include "GUITrace.h"
#include "UserConfig.h"
#include "GUIResourceFileHandler.h"
#include "StreamRuntimeConfig.h"
#include "StreamRuntimeStartup_FreeRTOS.h"
#include "GfxWrap.h"

#include "GUIInputMedia.h"
#include "GUIEvent.h"

// Mem leak watcher need to be last
#include "GUIMemLeakFunctions.h"
#include "GUIMemLeakWatcher.h"

#if __ICCARM__ == 1
extern "C"
{
    int open(const char *filename, int amode);
    int close(int handle);
}
#else
void *__dso_handle = 0;
#endif

// Resources FLASH section, size can be adapted here if needed
#if __ICCARM__ == 1
#pragma data_alignment=8
#define RES_FLASH  @ ".RES_FLASH"
uint8_t __guilianiResources[0x3000000] @ "RES_FLASH";
#else
extern const uint8_t __attribute__ ((section (".RES_FLASH"))) __guilianiResources[0x3000000];
#endif

extern "C" void GuilianiStart()
{
    GUI_REG_STDOUT_TRACE();

    GUILOG(GUI_TRACE_DEBUG, "CMyGUI::Init: RESOURCE FILE ACTIVE!\n");

    // use a resource file containing all required images, fonts, texts etc.
    GETRESHANDLER.SetResourceData(__guilianiResources);

    // Use CGUI::SetGuilianiLicenseKey("<ENTER YOUR CODE HERE>")) to set your Guiliani license key via code.
    // Alternatively use CGUI::LoadGuilianiLicenseKey("GuilianiLicense.key") to read your Guiliani license key from a file.

    CMyGUI* pMyGui = NULL;
    try
    {
        // Load configuration (e.g. ScreenSize, initial Dialog etc.) from file
        if (NStreamRuntime::LoadConfiguration() == false)
        {
            return;
        }

        NStreamRuntime::ConstructResourceClasses();

        pMyGui = new CMyGUI(
            eC_FromInt(0),
            eC_FromInt(0),
            eC_FromInt(GETRUNTIMECONFIG.GetScreenWidth()),
            eC_FromInt(GETRUNTIMECONFIG.GetScreenHeight()),
            NO_HANDLE);

        // Run the GUI in endless loop
        pMyGui->Run();
    }
    catch (...)
    {
        GUILOG( GUI_TRACE_ERROR, "An unrecoverable error occurred. Executing clean shutdown. \n");
    }

    delete pMyGui;
    pMyGui = NULL;

    NStreamRuntime::DestructResourceClasses();

#ifdef GUILIANI_LEAK_DETECTION
    LEAK_DETECTION.ShowResults();
#endif

    GUILOG(GUI_TRACE_DEBUG, "Guiliani shutdown complete.\n");
}


