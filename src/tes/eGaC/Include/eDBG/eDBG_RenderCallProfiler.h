/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eDBG
* file name:       eDBG_RenderCallProfiler.h
* %version:        134.0 %
* 
* author:          hh04030
* creation date:   Thu May 04 17:09:26 2017
* description:

* %date_modified:  % (%derived_by: hh04030 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------

******************************************************************************/

#ifndef EDBG_RENDERCALLPROFILER__H_
#define EDBG_RENDERCALLPROFILER__H_

//---------------------------------------------------------------
#include "eDBG_Config.h"
#include <stdio.h>
extern FILE * LRCFile_h;

#define eDBG_RCP_StartMethod()   eC_Time64 uiTimestamp = eC_GETTICKS();
//#define eDBG_RCP_StopMethod(arg) fprintf(LRCFile_h, "%3ums:",eC_GetMS(eC_GETTICKS() - uiTimestamp)); fprintf arg ; fflush(LRCFile_h); fprintf(LRCFile_h, "\n");
#define eDBG_RCP_StopMethod(arg) printf("\n%3ums:",(unsigned int)eC_GetMS(eC_GETTICKS() - uiTimestamp)); printf arg ; printf("\n");
//---------------------------------------------------------------
#endif
