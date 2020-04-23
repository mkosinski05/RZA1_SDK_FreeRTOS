/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

/**
Please include this file in your .cpp file, otherwise the memory leak happens
in your .cpp file will not be correctly shown in the output window.
*/

/**************************************************************************************
*THIS FILE MUST BE INCLUDED AFTER ALL THE INCLUDE FILES IF YOU USE STL.
*THUS BE SURE TO USE THIS HEADER, YOU CALL INCLUDE THIS HEADER AFTER ALL
*THE INCLUDE FILES.
**************************************************************************************/
#ifndef _WINDOWS_LEAK_WATCHER__H
#define _WINDOWS_LEAK_WATCHER__H

#include "GUIConfigDebug.h"

// The Windows leak watcher is not yet supported on EVC 4.
#ifndef eC_TARGET_ENV_POCKET
#ifdef _WIN32
#ifdef GUILIANI_WINDOWS_LEAK_WATCHER
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
/*
Because the new operator is declared as an inline function. The following
macro make the inline workaround so that the __FILE__ shows the correct the file name.
*/
#ifdef new
#undef new
#endif

#define NEW_INLINE_WORKAROUND new ( _NORMAL_BLOCK ,__FILE__ , __LINE__ )
#define new NEW_INLINE_WORKAROUND
#endif//#ifdef GUILIANI_WINDOWS_LEAK_WATCHER
#endif//#ifdef _WIN32
#endif //#ifndef eC_TARGET_ENV_POCKET
#endif // _WINDOWS_LEAK_WATCHER__H
