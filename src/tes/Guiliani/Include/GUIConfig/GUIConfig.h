/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUICONFIG__H_
#define GUICONFIG__H_

//--------------------------------------------------------------------------------------
#include "GUITraceLevels.h"


/// Trace level
/// In release mode, the trace will not be used.
#if defined _DEBUG || defined DEBUG
    #define GUI_TRACE_LEVEL GUI_TRACE_DEBUG
#else
    #define GUI_TRACE_LEVEL GUI_TRACE_ERROR
#endif

/// Guiliani-internal cast which uses dynamic_casts if RTTI is enabled, and static_casts otherwise
#if defined GUILIANI_USE_RTTI
    #define auto_cast dynamic_cast
#else
    #define auto_cast static_cast
#endif

/*  The following describes some pre-compiler defines that have to be added to your
    IDE's Guiliani project build configuration(s) if they should be enabled. Probably
    in a Guiliani build configuration and the corresponding application build
    configuration.
    Thus, they should not be defined in any header file.

    DON'T TOUCH THEM HERE!!!
*/

#ifdef DOXYGEN
/** Disables four way navigation. Needs to be defined in Guiliani only.
*/
#define GUILIANI_NO_4WAY_NAVIGATION

/** Is used to write the XML files. Must be defined in Guiliani and the application project.
*/
#define GUILIANI_WRITE_GUI

/** Is used to read from the XML files. Must be defined in Guiliani and the application project.
*/
#define GUILIANI_STREAM_GUI

/** Set this if you are building Guiliani as a library. Must NOT be set in the application project
*/
#define GUILIANI_GUILIANILIB

/** Disables the DataPool 
*/
#define GUILIANI_NO_DATAPOOL

/** Disable image cache function of Guiliani
*/
#define GUILIANI_NO_IMAGE_CACHE

#endif

#endif
