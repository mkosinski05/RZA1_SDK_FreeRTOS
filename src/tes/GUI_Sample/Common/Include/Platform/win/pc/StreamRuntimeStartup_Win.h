/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(STREAMRUNTIMESTARTUP_WIN_H)
#define STREAMRUNTIMESTARTUP_WIN_H

#include <Windows.h>
#include <shellapi.h>
#include "eC_Types.h"

/** The NStreamRuntime Namespace contains generic helper classes for initializing a default StreamRuntime application.
    The code herein typically remains widely unchanged across most StreamRuntimes.
    Feel free to remove these classes from your own application and replace them with custom code, if you wish.*/
namespace NStreamRuntime
{
    // Windows specific methods for creating the application window
    void Init(HINSTANCE hInstance, int nCmdShow);
    BOOL OpenWindow(int nCmdShow);
    ATOM MyRegisterClass();
    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    
    // Methods for Initialization/Shutdown of Guiliani 
    void ConstructWrapperClasses();
    void DestructWrapperClasses();

    // Window Instance handle
    extern HINSTANCE m_hInstance;
    // Window handle
    extern HWND      m_hWnd;
}
#endif //#ifndef STREAMRUNTIMESTARTUP_WIN_H
