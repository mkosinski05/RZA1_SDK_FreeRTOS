/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUICONFIGDEBUG__H_
#define GUICONFIGDEBUG__H_

/** Contains Guiliani specific debug defines. The programmer may alter the defines in this file
    in order to enable or disable specific debugging features for debug builds.
*/

#if defined _DEBUG || defined DEBUG

// sanity check of the build environment
#ifdef NDEBUG
    #error 'NDEBUG and _DEBUG' or 'NDEBUG and DEBUG' are set.
#endif

// For the documented defines, see below.
#if !defined(DOXYGEN)

#define GUILIANI_STREAM_CMP_TAGS
//#define GUILIANI_LEAK_DETECTION
//#define GUILIANI_GFXDEBUG
//#define GUILIANI_GFXDEBUG_BLIT_LINE
//#define GUILIANI_GFXDEBUG_REFRESH_AFTER_EACH_BLIT
//#define GUILIANI_GFXDEBUG_REFRESH_AFTER_EACH_FILLEDRECT
//#define GUILIANI_GFXDEBUG_DONT_LET_BORDER_CROSSING_TRIGGER_REFRESH
//#define GUILIANI_GFXOGLDEBUG
//#define GUILIANI_DEBUG_RECTANGLES
//#define GUILIANI_IGNORE_MOUSEMOVE_EVENTS
//#define GUILIANI_CMDHDL_DEBUGGING
//#define GUILIANI_DEBUG_FOCUSSING
//#define GUILIANI_MEMORY_ALLOCATION_LOGGING
//#define GUILIANI_WINDOWS_LEAK_WATCHER

//sanity checks
#if defined GUILIANI_LEAK_DETECTION && defined GUILIANI_WINDOWS_LEAK_WATCHER
    #error Please do only define one leak detection.
#endif

#else // DOXYGEN is defined - all defines enabled for documentation
/** Enables comparison of tags read during XML streaming, with those expected by ReadFromStream-Methods.
    This leads to more robust streaming behaviour, with more meaningful error-messages, but comes at a performance cost.
*/
#define GUILIANI_STREAM_CMP_TAGS

/// Enables the graphics debug output.
#define GUILIANI_GFXDEBUG

/// Blits a diagonal line with seperate color on every object.
#define GUILIANI_GFXDEBUG_BLIT_LINE

/// Show every blit in the backbuffer.
#define GUILIANI_GFXDEBUG_REFRESH_AFTER_EACH_BLIT

/// Every filled rect will be shown in the framebuffer.
#define GUILIANI_GFXDEBUG_REFRESH_AFTER_EACH_FILLEDRECT

/// Enables debugging for the rectangle class. Includes for instance normalization checks
#define GUILIANI_DEBUG_RECTANGLES

/** Allows Guiliani to make use of Runtime Type Information (RTTI). This will for instance display the types of
    objects in debug messages.

    This option must be defined in the platform-specific project settings because it depends
    on the target compiler and platform. */
#define GUILIANI_USE_RTTI

/// Mouse moves will be ignored. There will be no highlighted states anymore.
#define GUILIANI_IGNORE_MOUSEMOVE_EVENTS

/// Activates the command handler debug output messages.
#define GUILIANI_CMDHDL_DEBUGGING

/** Disable automatically calling InvalidateArea() when the mouse moves over
    the border of a control (in non-debug mode this is done so that the
    highlighted state will be shown automatically for example).
*/
#define GUILIANI_GFXDEBUG_DONT_LET_BORDER_CROSSING_TRIGGER_REFRESH

/// Enables additional debug messages concerning focusing.
#define GUILIANI_DEBUG_FOCUSSING

/** Guiliani memory leak detection will be used. The output can be redirected to a
    Logfile.
    @note "valgrind" is too smart for the implemented leakwatcher: it will detect the
    mismatch between the different operators with a "mismatched free() / delete /
    delete[]" error message. Therefore, valgrind must only be used if
    GUILIANI_LEAK_DETECTION is not defined.
*/
#define GUILIANI_LEAK_DETECTION

/** Enables memory allocation file logging, which prints informations of every memory
    allocation and deallocation to the filename defined in GUILIANI_MEMORY_ALLOCATION_LOG_FILENAME.
    This define has to be set BEFORE "GUIMemLeakFunctions.h" will be included to
    take effect.
*/
#define GUILIANI_MEMORY_ALLOCATION_LOGGING

/** Sets the filename for the memory allocation logging file.
    If it wasn't set by user it defaults to MemoryAllocation.log.
*/
#define GUILIANI_MEMORY_ALLOCATION_LOG_FILENAME

/** Memory leak detetcion from Microsoft compiler will be used.
    You can click on the line of the output window to jump directly to
    the position in file, where the not deleted object was newed.
*/
#define GUILIANI_WINDOWS_LEAK_WATCHER
#endif // DOXYGEN

#endif // defined _DEBUG || defined DEBUG

#endif //#ifndef GUICONFIGDEBUG__H_
