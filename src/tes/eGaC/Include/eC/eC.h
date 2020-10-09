/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC.h
* %version:        134.0 %
* 
* author:          hh04031
* creation date:   Mon Aug 08 07:15:09 2005
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EC__H_
#define EC__H_

//---------------------------------------------------------------
#include "eC_Config.h"
#include "eC_Platform.h"
#include "eC_Types.h"
#include "eC_Math.h"
#include "eC_Memory.h"
#include "eC_TArray.h"
#include "eC_TList.h"
#include "eC_String.h"
#include "eC_File.h"
#include "eC_Semaphore.h"
#include "eC_MemLeakDetection.h"
#include "eC_FileHandler.h"
#include "eC_BinFile.h"

//---------------------------------------------------------------
/**@name Component_eC
@memo
The <B>e</B>mbedded <B>C</B>omponents.
@doc
This component is a collection of modules that provide different functionalities necessary for whole <B>eGML</B> implementation,
including definition of all common types, mathematic operations, file I/O, string handling etc. <BR>
So <B>eC</B> helps to keep the core of <B>eGML</B> implementation and application code to be platform and storage type independent. <BR>
To get more information about {@link Component_eC eC} and its handling, look at the \Ref{Tutorial_eC}.
@see Tutorial_eC
*/

//@{

//@Include: ../../comps/eC/code/eC_Config.h
//@Include: ../../comps/eC/code/eC_Types.h
//@Include: ../../comps/eC/code/eC_Math.h
//@Include: ../../comps/eC/code/eC_Memory.h
//@Include: ../../comps/eC/code/eC_TArray.h
//@Include: ../../comps/eC/code/eC_TList.h
//@Include: ../../comps/eC/code/eC_File.h
//@Include: ../../comps/eC/code/eC_Semaphore.h
//@Include: ../../comps/eC/code/eC_MemLeakDetection.h
//@Include: ../../comps/eC/code/eC_String.h
//@Include: ../../comps/eC/code/eC_FileHandler.h
//@Include: ../../comps/eC/code/eC_BinFile.h

//@}
  
//---------------------------------------------------------------
#endif
