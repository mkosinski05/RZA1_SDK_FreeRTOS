/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eDBG
* file name:       eDBG_Config.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:09:15 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EDBG_CONFIG__H_
#define EDBG_CONFIG__H_

//--------------------------------------------------------------------------------------------------------------------
#ifdef eDBG_USE_OWN_CONFIG

#ifdef eDBG_CONFIG_FILE
#include "eDBG_User_Config.h"
#endif

#else
//--------------------------------------------------------------------------------------------------------------------
#ifdef DOCXX
/**@name eDBG_Global_Defines
@memo
Macros for <B>eDBG</B> specific settings.
@doc
These macros adjust general settings and the level of effort for debugging and profiling functionality.
@see
Global_Platform_Defines
*/
//@{

/// Debug statistic. If defined debug statistic functionalities are enabled.
#define eDBG_STATISTIC

/** Debug level. This macro defines the level of logging, printing and assertions. The following levels can be defined: <BR>
<LI> 0 - all debugmacros are removed </LI>
<LI> 1 - asserts only </LI>
<LI> 2 - <B>eGML</B> intern level 1 logs, asserts and debug prints </LI>
<LI> 3 - all <B>eGML</B> intern level 2 logs, asserts and debug prints </LI> */
#define eDBG_DEBUG_LEVEL

/** Debug level for application. This macro defines the debug level of logging, printing and assertions inside of application code,
so you are flexible to define independently from normal debug level your own while testing your application.
The following levels can be defined: <BR>
<LI> 0 - no application logs </LI>
<LI> 1 - level 1 application logs </LI>
<LI> 2 - level 2 application logs </LI> */
#define eDBG_DEBUG_APPLICATION_LEVEL

// !!! NOT FINISHED for Release 1.3: <LI> 2 - file </LI> (belongs to eDBG_DEBUG_OUTPUT)

/** Debug output stream target. Depending on the defined value of this macro, the debug output stream will be sent to one of the
following targets: <BR>
<LI> 0 - default </LI>
<LI> 1 - stdio </LI>
<LI> 3 - platform specific </LI> */
#define eDBG_DEBUG_OUTPUT

/// Autoregister statistic counters. If defined the autoregister statistic counters are activated.
#define eDBG_AUTOREGISTERSTAT

/// Graphical statistic output. If defined graphical statistic output is enabled and will be displayed in realtime.
#define eDBG_EGMLOUTPUT

/// Automatic FPS counting. If defines automatic FPS counting is enabled.
#define eDBG_FPSCOUNT

//@}
#endif

#define eDBG_STATISTIC
#if _DEBUG
    #define eDBG_DEBUG_LEVEL 1
#else
    #define eDBG_DEBUG_LEVEL 0
#endif
#define eDBG_DEBUG_APPLICATION_LEVEL 0
#define eDBG_DEBUG_OUTPUT 0
// Autoregisterstat only tested on Win32
#ifdef eC_TARGET_ENV_WIN32
//#define eDBG_AUTOREGISTERSTAT
#endif
#define eDBG_EGMLOUTPUT
#define eDBG_FPSCOUNT

//--------------------------------------------------------------------------------------------------------------------
#endif

//--------------------------------------------------------------------------------------------------------------------
#endif
