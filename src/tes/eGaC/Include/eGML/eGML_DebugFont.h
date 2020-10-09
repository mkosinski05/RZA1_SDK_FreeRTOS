/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_DebugFont.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:17:03 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_DEBUGFONT__H_
#define EGML_DEBUGFONT__H_
//---------------------------------------------------------------	

extern void DebugFont_Init();
extern void DebugFont_Shutdown();
extern long DebugFont_GetHeight();
extern bool DebugFont_GetChar(unsigned char c, void **ppData, long &riWidth, long &riHeight);

//---------------------------------------------------------------	
#endif
