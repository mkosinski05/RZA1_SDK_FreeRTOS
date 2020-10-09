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
*    Archive:       $URL: https://10.25.129.51:3690/svn/GSE/branches/Releases/1.0_Guiliani_2.1/StreamRuntime/src/platform/win/pocket_pc/StreamRuntime_Win.cpp $
*
*    Date created:  2005
*
*
*
*    Author:
*
*******************************************************************************
*   MODIFICATIONS
*******************************************************************************
*    ID
*    --------------------------------------------------------------------------
*    $Id: StreamRuntime_Win.cpp 2137 2014-11-04 08:13:54Z christian.euler $
*
******************************************************************************/
// StreamRuntime_Win.cpp : Main entry point for a Guiliani application
// In this file all platform specific code should reside.

#include "MyGUI_SR.h"
// either include "stdafx.h" for precompiled headers or "Windows.h" for build time compilation of headers
#include "Windows.h"
#include "gx.h"
//#include "stdafx.h"
#include "Configurator.h"
#include "GUITrace.h"
#include "GUIConfigDebug.h"
#include "GUIMemLeakFunctions.h"
#include "GUIMemLeakWatcher.h"
#include "GUIResourceFileHandler.h"
#include "GUIException.h"
#include "FntWrapWin.h"
#include "UserConfig.h"
#if defined GFX_USE_GDI
#include "GfxWrapGDIPocketPC.h" // Note: user code normally only needs to include GfxWrap.h, except for startup code
#elif defined GFX_USE_EGML
#include "GfxWrapeGMLwin.h"
#include "FntWrapFreeTypeeGML.h"
#elif defined GFX_USE_GL
#include "GfxWrapOGLwin.h"
#endif
#include "GUIInputWindows.h"
#include "SndWrapWin.h"
#include "WindowsLeakWatcher.h"

// Global variables
#define MAX_LOADSTRING 100
static TCHAR szTitle[MAX_LOADSTRING] = TEXT("StreamRuntime");
static TCHAR *szWindowClass = szTitle;

static const eC_String DEFAULT_CONFIG_NAME("StreamRuntimeConfig.xml");

#define TASKBAR_HEIGHT 0 //26

namespace NStartup
{
    ATOM MyRegisterClass(HINSTANCE hInstance);
    void Init(HINSTANCE hInstance, HWND &hWnd, int nCmdShow, eC_UInt uiWidth, eC_UInt uiHeight);
    static LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
    void ConstructResourceClasses(HWND hWnd, eC_UInt uiWidth, eC_UInt uiHeight);
    void DestructResourceClasses(HWND hWnd);
    void Taskbar(HWND hWnd, eC_Bool bShow);
    void SetDefaultResourcePathPrefix();
    eC_Bool LoadConfiguration();
}

int WINAPI WinMain(
    IN HINSTANCE hInstance,
    IN HINSTANCE hPrevInstance,
    IN LPTSTR lpCmdLine,
    IN int nCmdShow)
{
    GUI_REG_FILE_TRACE("StreamRuntime.log");
    GUI_REG_DEBUGGER_TRACE();

    // Use the executable path for loading the configuration file.
    NStartup::SetDefaultResourcePathPrefix();

    if (NStartup::LoadConfiguration() == false)
    {
        return 0;
    }

    eC_UInt uiWidth = ::GetSystemMetrics(SM_CXSCREEN);
    eC_UInt uiHeight = ::GetSystemMetrics(SM_CYSCREEN);
    HWND hWnd = 0;

    NStartup::Init(hInstance, hWnd, nCmdShow, uiWidth, uiHeight);

    CMyGUI* pMyGui = new CMyGUI(
        eC_FromInt(0),
        eC_FromInt(0),
        eC_FromInt(uiWidth),
        eC_FromInt(uiHeight),
        NO_HANDLE);

    pMyGui->Run();

    delete pMyGui;
    pMyGui = NULL;

    NStartup::DestructResourceClasses(hWnd);

#ifdef GUILIANI_WINDOWS_LEAK_WATCHER
    _CrtDumpMemoryLeaks();
#endif
#ifdef GUILIANI_LEAK_DETECTION
    //LEAK_DETECTION.ShowResults();
    //LEAK_DETECTION.WriteResultsToFile("StreamRuntime_MemoryLeaks.log");
#endif
    return 0;
}

namespace NStartup
{
void ConstructResourceClasses(HWND hWnd, eC_UInt uiWidth, eC_UInt uiHeight)
{
    /* Sets the graphics wrapper */
    try
    {
#if defined GFX_USE_GDI
        /* pc & pocketpc version: does not use eGML at all */
        CGfxWrapGDIPocketPC::CreateInstance(hWnd);
        CFntWrapWin::CreateInstance(((CGfxWrapGDIPocketPC&) GETGFX).GetScreen());
#elif defined GFX_USE_EGML
        /* pc version: uses eGML to draw to memory buffer and GDI blit
           to copy to framebuffer */
        CGfxWrapeGMLwin::CreateInstance(hWnd, uiWidth, uiHeight);
        //CFntWrapWin::CreateInstance(((CGfxWrapeGMLwin&) GETGFX).GetScreen());
        CFntWrapFreeTypeeGML::CreateInstance(512*1024, static_cast<CGfxWrapeGML&>(GETGFX).GetScreen());
#elif defined GFX_USE_GL
        /* pc version: uses OpenGL to draw to memory buffer */
        CGfxWrapOGLwin::CreateInstance(hWnd);
#else
#error No graphics wrapper chosen!!!
#endif
    }
    catch(...)
    {
        GUILOG_THROW_EXCEPTION(CGUIException() ,"NStartup::ConstructResourceClasses: Could not allocate graphics wrapper.\n");
    }

    try
    {
        CGUIInputWindows::CreateInstance(hWnd);
    }
    catch(...)
    {
        GUILOG_THROW_EXCEPTION(CGUIException() ,"NStartup::ConstructResourceClasses: Could not create input media.\n");
    }

    try
    {
        CSndWrapWin::CreateInstance();
    }
    catch(...)
    {
        GUILOG_THROW_EXCEPTION(CGUIException() ,"NStartup::ConstructResourceClasses: Could not create sound wrapper.\n");
    }
}

void DestructResourceClasses(HWND hWnd)
{
    Taskbar(hWnd, true);

    ::GXCloseInput();
    ::GXCloseDisplay();

    CSndWrapWin::DeleteInstance();

    CGUIInputMedia::DeleteInstance();
    //CGUILocalisationHandler::DeleteInstance();

    CFntWrap::DeleteInstance();

#ifdef GFX_USE_EGML
    CGfxWrapeGMLwin::DeleteInstance();
#elif defined GFX_USE_GDI
    CGfxWrapGDIPocketPC::DeleteInstance();
#endif
}

void Init(HINSTANCE hInstance, HWND &hWnd, int nCmdShow, eC_UInt uiWidth, eC_UInt uiHeight)
{
    //If it is already running, then focus on the window
    hWnd = ::FindWindow(szWindowClass, szTitle);
    if (hWnd)
    {
        // set focus to foremost child window
        // The "| 1" is used to bring any owned windows to the foreground and
        // activate them.
        ::SetForegroundWindow((HWND)((ULONG) hWnd | 1));
        GUILOG_THROW_EXCEPTION(CGUIException() , "NStartup::Init: Window already exists.\n");
    }

    NStartup::MyRegisterClass(hInstance);

    hWnd = ::CreateWindow(
        szWindowClass,
        szTitle,
        WS_VISIBLE, /* WS_POPUP may be better */
        0,
        0,
        uiWidth,
        uiHeight,
        NULL,                      // Parent
        NULL,                      // Windows' Menu
        hInstance,                 // Application instance
        NULL);                     // Pointer to parameters

    if (!hWnd)
    {
        GUILOG_THROW_EXCEPTION(CGUIException() , "NStartup::Init: Could not create window.\n");
    }

    Taskbar(hWnd, false);

    if (::GXOpenDisplay (hWnd, GX_FULLSCREEN) == 0)
    {
        GUILOG_THROW_EXCEPTION(CGUIException() , "NStartup::Init: Could not open display.\n");
    }

    ::GXOpenInput(); // disable keyboard event filtering of windows msgs

    ConstructResourceClasses(hWnd, uiWidth, uiHeight);

    ::ShowWindow(hWnd, nCmdShow);
    ::UpdateWindow(hWnd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_ACTIVATE:
            if (WA_INACTIVE == LOWORD(wParam))
            {
                ::GXSuspend();
            }
            else
            {
                ::GXResume();
            }
            break;

        case WM_PAINT:
            // We only draw directly to the screen so we need to make sure
            // it is frontmost before drawing
            if (::GetForegroundWindow() == hWnd)
            {
                PAINTSTRUCT ps;
                // This tells Windows that it doesn't need to issue WM_PAINT
                // during current update sequence anymore
                HDC hdc = BeginPaint(hWnd, &ps);

                // reduce rect to screen size
                GETGFX.InvalidateRect(
                    eC_FromInt(ps.rcPaint.left),
                    eC_FromInt(ps.rcPaint.top),
                    eC_FromInt(ps.rcPaint.right),
                    eC_FromInt(ps.rcPaint.bottom));

                EndPaint(hWnd, &ps);
            }
            break;

        case WM_WINDOWPOSCHANGED:
            {
                WINDOWPOS *pWp = (LPWINDOWPOS) lParam;
                if (pWp->hwndInsertAfter != NULL)
                {
                    TCHAR str[101];
                    ::GetWindowText(pWp->hwndInsertAfter, str, 100);
                    if (wcscmp(str, _T("Desktop")) == 0)
                    {
                        // Here we know that the Smart Minimize button is pressed
                        // but our window is already minimized
                        ::PostMessage(hWnd, WM_DESTROY, 0, 0);
                    }
                }
            }
            break;

        case WM_DESTROY:
            ::PostQuitMessage(0);
            break;
        default:
            return ::DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

void Taskbar(HWND hWnd, eC_Bool bShow)
{
    RECT rc;
    ::GetWindowRect( hWnd, &rc );

    HWND hWndTB = ::FindWindow(TEXT("HHTaskbar"), NULL);

    if (bShow)
    {
        ::SHFullScreen( hWnd, SHFS_SHOWTASKBAR | SHFS_SHOWSIPBUTTON | SHFS_SHOWSTARTICON );
        ::ShowWindow( hWndTB, SW_SHOW );
        ::MoveWindow(
            hWnd,
            rc.left,
            rc.top + TASKBAR_HEIGHT,
            rc.right,
            rc.bottom,
            TRUE);
    }
    else
    {
        ::SHSipPreference(hWnd, SIP_FORCEDOWN);
        ::SetForegroundWindow(hWnd);
        ::SHFullScreen( hWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON );
        ::ShowWindow( hWndTB, SW_HIDE );
        ::MoveWindow(
            hWnd,
            rc.left,
            rc.top - TASKBAR_HEIGHT,
            rc.right,
            rc.bottom,
            TRUE);
    }
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASS wc;

    wc.style            = 0;  //CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc        = (WNDPROC) WndProc;
    wc.cbClsExtra        = 0;
    wc.cbWndExtra        = 0;
    wc.hInstance        = hInstance;
    wc.hIcon            = NULL;
    wc.hCursor            = NULL;
    wc.hbrBackground    = NULL; // don't let Windows OS draw the background on
                                // its own, we do manage the complete client area
    wc.lpszMenuName        = NULL;
    wc.lpszClassName    = szWindowClass;

    return ::RegisterClass(&wc);
}

void SetDefaultResourcePathPrefix()
{
    // Get the path of the running program. Get the whole module name first,
    // then cut off the executable name.
    TCHAR acBuffer[512];
    ::GetModuleFileName(NULL, acBuffer, 512);
    eC_String kPath((eC_Char*) acBuffer, 0, eC_String::eUNICODE);
    // Cut off the executable file name after the last backslash.
    eC_UInt uiLastBackslash = 0, uiNextBackslash;
    while ( (uiNextBackslash = kPath.Find(eC_String("\\"), uiLastBackslash+1)) != eC_String::notfound )
    {
        uiLastBackslash = uiNextBackslash;
    }
    eC_TChar *pwcPath = (eC_TChar *)kPath.ToTChar_Alloc();
    eC_String kExecutablePath(pwcPath, uiLastBackslash+1);
    delete[] pwcPath;

    // kExecutablePath now contains the path name without the executable
    // file name, but including the last backslash.

    GETRESHANDLER.SetResourcePathPrefix(kExecutablePath /*+ "Resources\\"*/);
}

eC_Bool LoadConfiguration()
{
    return GETCONFIG.LoadConfiguration(DEFAULT_CONFIG_NAME);
}

} // namespace
