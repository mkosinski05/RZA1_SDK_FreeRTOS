/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eDBG
* file name:       eDBG_User_Config.h
* %version:        134.0 %
* 
* author:          hh04019
* creation date:   Fri Sep 09 18:46:40 2005
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef __EDBG_USER_CONFIG_H__
#define __EDBG_USER_CONFIG_H__

// eDBG - Config ------------------------------------------



#define eDBG_STATISTIC							// support for debug statistics

#define eDBG_DEBUG_LEVEL  3						// 0 - debugmacros are removed
												// 1 - asserts only
												// 2 - egml intern level1 logs and debugprints
												// 3 - egml intern level2 logs

#define eDBG_DEBUG_APPLICATION_LEVEL  2			// 0 - no application logs
												// 1 - level1 application logs
												// 2 - level2 application logs

#define eDBG_DEBUG_OUTPUT	0					// 0 - default
												// 1 - stdio
												// 2 - file
												// 3 - platform specific

#define eDBG_AUTOREGISTERSTAT					// autoregister statistic counters 

#define eDBG_EGMLOUTPUT							// enable graphical stat output

#define eDBG_FPSCOUNT							// enable automatic fps counting

#endif
