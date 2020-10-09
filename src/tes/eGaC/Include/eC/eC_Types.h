/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_Types.h
* %version:        134.0 %
*
* author:          hh04030
* creation date:   Mon Mar 27 09:50:20 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EGML_TYPES__H_
#define EGML_TYPES__H_

//---------------------------------------------------------------------------------------------
#include "eC_Config.h"

#ifndef NULL
#define NULL (0L)
#endif

//---------------------------------------------------------------------------------------------
/**@name eC_Types
@memo
Basic datatypes.
@doc
Using system and platform independent types inside <B>eGML</B> is essential for portability.<BR>
To guarantee highest possible performance the internal representation of \Ref{eC_Value} can be changed from float to fixedpoint (integer)
when working with a system that does not provide a fast FPU or an Application that requires a fixedpoint interface. <BR>
To get more information about \Ref{eC_Types} and \Ref{eC_Constants} or their handling, look at the \Ref{Tutorial_eC}.
@see Tutorial_eC */

//@{

//---------------------------------------------------------------------------------------------
/**@name eC_Value
@memo
Noninteger value.
@doc
eC_Value is mapped to floating- or fixed-point at compiletime. Internal representation is transparent to the Application. Access eC_Values using the
macros and functions defined in \Ref{eC_Math} and avoid manual casting/modification if \Ref{eC_Math} has support for the desired operation.
The following operations are guaranteed to work without the use of any macro: addition, subtraction, negation and comparison.
eC_Value is stored as a 32bit (4byte) value, regardless of its internal organisation.
@see eC_Math */
#ifdef DOCXX
typedef eC_Value;
#endif

/// Signed 32bit integer. Maps to signed long int.
typedef long int eC_Int;

/// Unsigned 32bit integer. Maps to unsigned long int.
typedef unsigned long int eC_UInt;

/// Signed 16bit integer. Maps to signed short int.
typedef signed short int eC_Short;

/// Unsigned 16bit integer. Maps to unsigned short int.
typedef unsigned short int eC_UShort;

/// Signed 8bit integer. Maps to signed char.
typedef signed char eC_Byte;

/// Unsigned 8bit integer. Maps to unsigned char.
typedef unsigned char eC_UByte;

/// Single platform independent UTF-8 encoded character. Stored as unsigned short.
typedef unsigned char eC_UTF8;

/// Single platform independent UTF-16 encoded character. Stored as unsigned short.
typedef unsigned short int eC_UTF16;
/// untransformed 16 bit Unicode character (UCS2)
typedef unsigned short int eC_WChar;

/// Single platform independent UTF-32 encoded character. UTF-32 uses exactly 32 bits per Unicode code point, 
/// in contrast to all other Unicode transformation formats which are variable-length encodings.
/// So it's identical to the untransformed Unicode Format UCS4
typedef unsigned long int eC_UTF32;

/// Single unicode encoded character. Stored as wchar_t, which has platform dependent size and encoding (UTF-8, UTF-16, UTF-32). Used for interfacing to platform's unicode API.
typedef wchar_t eC_Unicode; // shall be named eC_WChar in future releases

/// Single character (8 bit char). Stored as signed 8 bit.
typedef char eC_Char;

/// Boolean value. Maps to C++ internal type bool.
typedef bool eC_Bool;

/// Abstract flag. Stored as unsigned 32bit integer.
typedef unsigned int eC_Flag;

/// Enumeration data type. Used for not typed enums.
typedef eC_UInt eC_Enum;

/** Floatingpoint value. Use float in case the limited range of fixedpoint numbers is not acceptable in this particular part of an algorithm.
Revert to \Ref{eC_Value} where possible.
@see eC_Math, eC_Value */
typedef float eC_Float;

/** Fixedpoint value. While \Ref{eC_Value} should be used where noninteger numbers can be represented in both (float/fixed) formats,
there are still places where fixedpoint is required.
@see eC_Math, eC_Value */
typedef long int eC_Fixed;

//---------------------------------------------------------------------------------------------
// !!! INTERNAL USE ONLY !!!
// define fixedpoint value
#define eC_FP_HBIT  eC_FP_SIZE                      // number of higher (interger) bits
#define eC_FP_LBIT  (32UL - eC_FP_SIZE)               // number of lower (fractional) bits
#define eC_FP_HIGH  ((~0L) << eC_FP_LBIT)           // integer part bitmask
#define eC_FP_LOW   (~eC_FP_HIGH)                   // fractional part bitmask
#define eC_FP_ONE   (1UL << eC_FP_SIZE)               // fixedpoint representation of 1.0
#define eC_FP_SIGN  0x80000000                      // signbit mask

// Definition of eC_Value
#ifdef eC_FIXPOINT

typedef eC_Fixed eC_Value;                          // value is signed 32bit

#define eC_MAXVALUE ( static_cast<eC_Fixed>(~eC_FP_SIGN) )     // largest possible positive number
#define eC_MINVALUE ( 1 )                           // smallest possible number larger than 0

#else

typedef eC_Float eC_Value;                          // value is 32bit float

#define eC_MAXVALUE ( FLT_MAX )                     // largest possible positive number
#define eC_MINVALUE ( FLT_MIN )                     // smallest possible number larger than 0

#endif

//---------------------------------------------------------------------------------------------
// !!! INTERNAL USE ONLY !!!
// Used this Casting operations
#ifdef __cplusplus
    #define EC_STATIC_CAST(type,var)        (static_cast<type>(var))
    #define EC_DYNAMIC_CAST(type,var)       (dynamic_cast<type>(var))
    #define EC_CONST_CAST(type,var)         (const_cast<type>(var))
    #define EC_REINTERPRET_CAST(type,var)   (reinterpret_cast<type>(var))
#else
    #define EC_STATIC_CAST(type,var)        ((type)(var))
    #define EC_DYNAMIC_CAST(type,var)       ((type)(var))
    #define EC_CONST_CAST(type,var)         ((type)(var))
    #define EC_REINTERPRET_CAST(type,var)   ((type)(var))
#endif


//---------------------------------------------------------------------------------------------
// !!! INTERNAL USE ONLY !!!
// Renderstate values are stored as untyped 4byte quantities. MixedType union is used to avoid tricky casts.
typedef union
{
  eC_UInt uiValue;
  eC_Int iValue;
  eC_Value vValue;
  eC_Fixed fxValue;
  eC_UInt kValue;
  eC_Bool bValue;
  void * pvValue;
}
eC_MixedType;

//---------------------------------------------------------------------------------------------
/**@name eC_Constants
@memo
Basic constants.
@doc
These constants are \Ref{eC_Value}s, so depending on the availability of fixedpoint or floatingpoint arithmetics (refer to \Ref{eC_FIXPOINT}),
their direct types are \Ref{eC_Fixed} or \Ref{eC_Float}. If you need explicitly <I>float</I> format (e. g. for <I>printf</I>)
- use \Ref{eC_ToFloat} macro. <BR>
They are all #defines (to provide best optimiser results), so you can not dereference them.
@see Tutorial_eC */

//@{

/**@name eC_MAXVALUE
@memo
Largest possible positive noninteger value.
@doc
Depends on the internal representation of \Ref{eC_Value}.
*/

/**@name eC_MINVALUE
@memo
Smallest possible noninteger still larger than zero.
@doc
Depends on the internal representation of \Ref{eC_Value}.
*/

//@}

//@}

//---------------------------------------------------------------
// complex types
#include "eC_Time64.h"                  // 64bit integer used for profiling

//---------------------------------------------------------------
#endif
