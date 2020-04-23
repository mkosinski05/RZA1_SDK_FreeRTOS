/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUITRACELEVELS__H_
#define GUITRACELEVELS__H_

/**
    Logs are only printed if the trace level of the requested log call is less than or equal to
    the trace level of the application. To define the application-wide trace level, use a define
    like this:
    @code
    #define GUI_TRACE_LEVEL GUI_TRACE_DEBUG
    @endcode
    This is currently done in GUIConfig.h.

    Example for logging:

    @verbatim
    Trace level set to GUI_TRACE_WARNING.
        GUILOG(GUI_TRACE_ERROR,   "My Message") will be printed.
        GUILOG(GUI_TRACE_WARNING, "My Message") will be printed.
        GUILOG(GUI_TRACE_DEBUG,   "My Message") won't be printed.
    @endverbatim
*/

/// Use this as GUI_TRACE_LEVEL to switch all trace output off.
#define GUI_TRACE_OFF    -1

/// Highest severity. Usually still printed in release builds.
#define GUI_TRACE_ERROR   0

/// Medium severity.
#define GUI_TRACE_WARNING 1

/// Low severity or debug information. Usually only printed in debug builds.
#define GUI_TRACE_DEBUG   2

#endif
