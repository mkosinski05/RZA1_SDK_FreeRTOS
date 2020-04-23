/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_Config.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 16:59:34 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EC_CONFIG__H_
#define EC_CONFIG__H_

//-------------------------------------------------------------------------------------------------------
#ifdef eC_USE_OWN_CONFIG

#ifdef eC_CONFIG_FILE
#include "eC_User_Config.h"
#endif

#else


//-------------------------------------------------------------------------------------------------------
#ifdef DOCXX

/**@name eC_Global_Defines
@memo
Macros for <B>eC</B> specific settings.
@doc
These macros adjust general settings and the mode of operation of <B>eC</B>. <BR>
Note: there is an interconnection between these <B>eC</B> specific 'defines' and the general
{@link Global_Platform_Defines platform} settings.
@see
Global_Platform_Defines
*/

//@{

/** Number of integer bits used in fixedpoint values.
Fractional bits are <TT>(32 - eC_FP_SIZE)</TT>. Normally value 16 is used. */
#define eC_FP_SIZE

/** Activates fixedpoint arithmetic.
If this macro is defined fixedpoint arithmetic will be used. */
#define eC_FIXPOINT

/** Overload of new and delete.
If this macro is defined default new and delete operators are overloaded and mapped to the
internal <B>eC</B> memory management unit. <BR>
Please be sure that the macro \Ref{eC_LEAKDETECTION} is <I>not</I> defined. */
#define eC_OVERLOAD_NEW_DELETE_OPERATOR


/** Activates reporting of memory consumption and leak detection of an application.
If this macro is defined the macros \Ref{eC_LEAKDETECTION} will also be defined.
@see eC_MEMORY_CONSUMPTION_REPORTLENGTH */
#define eC_MEMORY_CONSUMPTION_OVERVIEW

/** Default value for reporting list length of the memory consumtion system.
@see eC_MEMORY_CONSUMPTION_OVERVIEW 
@see eC_LEAKDETECTION */
#define eC_MEMORY_CONSUMPTION_REPORTLENGTH


/** Defines the size of the internal used hashtable in the MemLeakDetection system.
This value is default set to 0x1FFFE possible hashtable entries. Please redefine
this only if there are problems with the hashtables size. 
@see eC_LEAKDETECTION */
#define eC_LEAKDETECTION_HASHTABLE_SIZE

/** Avtivates Debug mode.
PLease use only this Marco, to activate the memory leak detection system of the application.

Additional information - especially by dynamic memory operations - are visible to the user if this
macro is defined. 

Do <I>not</I> use it when \Ref{eC_OVERLOAD_NEW_DELETE_OPERATOR} is defined. */
#define eC_LEAKDETECTION

/** Enables the use of a sinus and cosinus-lut, instead of sin/cos standard functions
*/
#define eC_USE_TRIG_TABLE

/** Calculates the TrigTable at startup. 
If not defined, it will be used a precalculated TrigTable. 
startup at systems without FPU is than much faster.
*/
#define eC_CALCULATE_TRIG_TABLE_DATA

//@}

#endif
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------




//-------------------------------
//----- User definition area ----
//-------------------------------
#define eC_FP_SIZE 16
//#define eC_FIXPOINT
//#define eC_OVERLOAD_NEW_DELETE_OPERATOR
//#define eC_MEMORY_CONSUMPTION_OVERVIEW
//#define eC_MEMORY_CONSUMPTION_REPORTLENGTH 10


#endif
//-------------------------------------------------------------------------------------------------------



//------------------------------------------------------
//-------- Do not change the definitions below! --------
//------------------------------------------------------
//---- These definitions are for consistence reasons ---
//------------------------------------------------------

//Set default value for report length
#ifndef eC_MEMORY_CONSUMPTION_REPORTLENGTH
#define eC_MEMORY_CONSUMPTION_REPORTLENGTH 10 
#endif

#ifdef eC_MEMORY_CONSUMPTION_OVERVIEW
//The Consumtion overview needs the debugmodus
#ifndef eC_LEAKDETECTION
#define eC_LEAKDETECTION
#endif
#endif


#ifdef eC_LEAKDETECTION
//define the length of the Hashtable to register Memory allocations
#ifndef eC_LEAKDETECTION_HASHTABLE_SIZE
#define eC_LEAKDETECTION_HASHTABLE_SIZE 0x1FFFE
#endif

//the leakdetection belongs to the debugmodus
#ifndef eC_LEAKDETECTION
#define eC_LEAKDETECTION
//if the debug functionality is activated, the eC_OVERLOAD_NEW_DELETE_OPERATOR
//must be deactivated!!
#undef eC_OVERLOAD_NEW_DELETE_OPERATOR
#endif

#endif

//This Defines controls the handling for dll handling under windows based systems
#ifndef EC_CLASS
#define EC_CLASS(x) class x
#endif

#ifndef EC_FNKT
#define EC_FNKT(x) x
#endif

#ifndef EC_DATA
#define EC_DATA(x) x
#endif

//-------------------------------------------------------------------------------------------------------
#endif
