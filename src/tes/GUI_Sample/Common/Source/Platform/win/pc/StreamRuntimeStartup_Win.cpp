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
*    MODULE:        StreamRuntimeStartup_Win.cpp
*
*    Archive:       $URL: https://10.25.129.51:3690/svn/Common/branches/Releases/2.2/StreamRuntime/Source/Platform/win/pc/StreamRuntimeStartup_Win.cpp $
*
*    Date created:  2005
*
*
*
*    Author:        ceu
*
*******************************************************************************
*   MODIFICATIONS
*******************************************************************************
*    ID
*    --------------------------------------------------------------------------
*    $Id: StreamRuntimeStartup_Win.cpp 585 2017-12-11 05:54:35Z sascha.sippel $
*
******************************************************************************/

// StreamRuntimeStartup_Win.cpp : Windows specific default startup code for a Guiliani StreamRuntime

#include "StreamRuntimeStartup_Win.h"
#include "StreamRuntimeConfig.h"
#include "Resource.h"
#include "GUIConfigDebug.h"
#include "GUIMemLeakFunctions.h"
#include "GUIResourceFileHandler.h"
#include "GUILocalisationHandler.h"
#include "UserConfig.h"
#include "GUITrace.h"
#include "GUIException.h"
#include "GUI.h"

// Includes for wrapper classes
#include "SndWrapWin.h"
#include "FntWrapWin.h"
#include "GUIInputWindows.h"
#include "GUIPlaybackInput.h"
#if defined GFX_USE_GDI
#include "GfxWrapGDIPC.h"
#elif defined GFX_USE_EGML
#include "GfxWrapeGMLwin.h"
#include "FntWrapFreeTypeeGML.h"
#elif defined GFX_USE_DAVE
#include "GfxWrapDAVEWin.h"
#include "FntWrapFreeTypeDAVE.h"
#include "GUIImageLoader.h"
#include "GUIImageDecoderBMP.h"
#include "GUIImageDecoderPNG.h"
#elif defined GFX_USE_GL
#include "GfxWrapOGLwin.h"
#include "FntWrapFreeTypeGL.h"
#include "GUIImageLoader.h"
#include "GUIImageDecoderBLU.h"
#include "GUIImageDecoderBMP.h"
#include "GUIImageDecoderPNG.h"
#include "GUIImageDecoderJPG.h"
#include "GUIImageDecoderRAW.h"
#include "GUIImageDecoderRLE.h"
#include "GUIImageDecoderSVG.h"
#else
#include "GfxWrapNull.h"
#endif

#include "GUIMemLeakWatcher.h"


// User configurable values for the application window position:
static const eC_UInt cuiWINDOWPOS_X = 0;
static const eC_UInt cuiWINDOWPOS_Y = 0;

// Window related variables
static const eC_UInt cuiMAX_LOADSTRING = 100;
static TCHAR szTitle[cuiMAX_LOADSTRING];                // Caption (title bar) text
static TCHAR szWindowClass[cuiMAX_LOADSTRING];


/*
Startup sequence is:
    Init()
    MyRegisterClass()
    OpenWindow()
    ConstructWrapperClasses()
    <--- Application lifetime --->
    DestructWrapperClasses()
*/

// Namespace containing default implementations for initializing a StreamRuntime under windows
namespace NStreamRuntime
{

    HWND        m_hWnd = NULL;
    HINSTANCE   m_hInstance = NULL;

    void ConstructWrapperClasses()
    {
        // Sets the graphics wrapper and font wrapper
#if defined GFX_USE_GDI
        CGfxWrapGDIPC::CreateInstance(m_hWnd, GETRUNTIMECONFIG.GetScreenWidth(), GETRUNTIMECONFIG.GetScreenHeight());
        CFntWrapWin::CreateInstance(((CGfxWrapGDIPC&) GETGFX).GetScreen());
#elif defined GFX_USE_EGML
        CGfxWrapeGMLwin::CreateInstance(m_hWnd, GETRUNTIMECONFIG.GetScreenWidth(), GETRUNTIMECONFIG.GetScreenHeight());
        CFntWrapFreeTypeeGML::CreateInstance(1024*1024, static_cast<CGfxWrapeGML&>(GETGFX).GetScreen());
#elif defined GFX_USE_GL
        CGfxWrapOGLwin::CreateInstance(m_hWnd, GETRUNTIMECONFIG.GetScreenWidth(), GETRUNTIMECONFIG.GetScreenHeight());
        CFntWrapFreeTypeGL::CreateInstance();
        GETIMGLOADER.AddDecoder(new CGUIImageDecoderBMP());
        GETIMGLOADER.AddDecoder(new CGUIImageDecoderBLU());
        GETIMGLOADER.AddDecoder(new CGUIImageDecoderPNG());
        GETIMGLOADER.AddDecoder(new CGUIImageDecoderJPG());
        GETIMGLOADER.AddDecoder(new CGUIImageDecoderRAW());
        GETIMGLOADER.AddDecoder(new CGUIImageDecoderRLE());
        GETIMGLOADER.AddDecoder(new CGUIImageDecoderSVG());
#elif defined GFX_USE_DAVE
        CGfxWrapDAVEWin::CreateInstance(m_hWnd, GETRUNTIMECONFIG.GetScreenWidth(), GETRUNTIMECONFIG.GetScreenHeight());
        CFntWrapFreeTypeDAVE::CreateInstance(1024*1024, static_cast<CGfxWrapDAVEWin&>(GETGFX).GetDaveDevice());
        GETIMGLOADER.AddDecoder(new CGUIImageDecoderBMP());
        GETIMGLOADER.AddDecoder(new CGUIImageDecoderPNG());
#else
        MessageBox( NULL, TEXT("Please choose a Graphics-Wrapper. Defaulting to CGfxWrapNull."), TEXT("Warning"), 0);
        CGfxWrapNull::CreateInstance();
#endif
        // Sets the sound wrapper
        CSndWrapWin::CreateInstance();
        // Sets the input wrapper
        CGUIInputWindows::CreateInstance(m_hWnd);
    }

    void DestructWrapperClasses()
    {
        CGUIInputMedia::DeleteInstance();
        CSndWrap::DeleteInstance();
        CFntWrap::DeleteInstance();
        CGfxWrap::DeleteInstance();
    }


    /* ---------------------------------------------------------------------------------
        Everything below is plain Windows-Code for setting up the application window
       ---------------------------------------------------------------------------------  */
    void Init(HINSTANCE hInstance, int nCmdShow)
    {
        m_hInstance = hInstance;
        // Load some global strings from programs resource chunk
        LoadString(hInstance, IDS_APP_TITLE, szTitle, cuiMAX_LOADSTRING);
        LoadString(hInstance, IDC_APP_WCLASS, szWindowClass, cuiMAX_LOADSTRING);

        MyRegisterClass();

        if (!OpenWindow(nCmdShow))
            GUILOG_THROW_EXCEPTION(CGUIException(), "NStreamRuntime::Init: Could not initialize application.\n");
    }

    // Creates and shows the window
    BOOL OpenWindow(int nCmdShow)
    {
        RECT aRect;

        // Used for client area
        SetRect(&aRect, 0, 0, GETRUNTIMECONFIG.GetScreenWidth(), GETRUNTIMECONFIG.GetScreenHeight());

        // Adjust the window size based on desired client area
        DWORD dwStyle = WS_SYSMENU | WS_CAPTION | WS_VISIBLE;
        
        if(GETRUNTIMECONFIG.IsWindowResizable())
        {
            //Resizable window;
            dwStyle |= WS_SIZEBOX;
        }
        AdjustWindowRect(&aRect, dwStyle, false);

        m_hWnd = CreateWindow(szWindowClass, szTitle, dwStyle,
            cuiWINDOWPOS_X, cuiWINDOWPOS_Y, aRect.right - aRect.left, aRect.bottom - aRect.top,
            NULL, NULL, m_hInstance, NULL);

        if (!m_hWnd)
        {
            return FALSE;
        }

        eC_Int caps;
        caps = GetDeviceCaps(GetDC(m_hWnd), BITSPIXEL);

        if (caps != 32 && caps != 16) // 16 or 32 Bit are the recommended color depth for windows
        {
            GUILOG( GUI_TRACE_WARNING,"Please choose a color depth of 16 or 32-Bit. \n");
        }

        ShowWindow(m_hWnd, nCmdShow);
        UpdateWindow(m_hWnd);
        return TRUE;
    }

    // Registers a Win32 windows class
    ATOM MyRegisterClass()
    {
        WNDCLASSEX wcex;
        wcex.cbSize             = sizeof(WNDCLASSEX);
        wcex.style              = 0;
        wcex.lpfnWndProc        = (WNDPROC)WndProc;
        wcex.cbClsExtra         = 0;
        wcex.cbWndExtra         = 0;
        wcex.hInstance          = m_hInstance;
        wcex.hIcon              = LoadIcon(m_hInstance, (LPCTSTR)IDI_APP_ICON);
        wcex.hCursor            = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground      = NULL;   // don't let Windows OS draw the background on
                                          // its own, we do manage the complete client area
        wcex.lpszMenuName       = NULL;
        wcex.lpszClassName      = szWindowClass;
        wcex.hIconSm            = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_APP_ICON_SMALL);

        return RegisterClassEx(&wcex);
    }

    // Processes some of the windows OS messages that are sent to this application window.
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        PAINTSTRUCT ps;
        HDC hdc;

        switch (message)
        {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            if( CGUI::IsCreated() )
            {
                // Trigger redraw of the GUI
                GETGFX.InvalidateRect(
                    eC_FromInt(ps.rcPaint.left),
                    eC_FromInt(ps.rcPaint.top),
                    eC_FromInt(ps.rcPaint.right),
                    eC_FromInt(ps.rcPaint.bottom));
            }
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

} // namespace
