/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_Math.h
* %version:        134.1 %
*
* author:          hh04019
* creation date:   Mon Nov 28 10:53:39 2005
* description:

* %date_modified:  % (%derived_by: hh04031 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EC_MATH__H_
#define EC_MATH__H_

//--------------------------------------------------------------------------------------------------
#include "eC.h"
#include "eC_Platform.h"

#ifdef eC_USE_TRIG_TABLE
extern void     eC_MathInitTable();
extern eC_Fixed EC_FNKT(eC_FixSin_Table)( eC_Fixed );
extern eC_Fixed EC_FNKT(eC_FixCos_Table)( eC_Fixed );
#endif

// Basic math functions to hide internal eC_Value representation from Application (fixed/float).
// Defines are used where neither operator-overloading nor inlined functions were able to produce perfectly optimised code on target systems.
// All macros are free from sideeffects (don't evaluate arguments multiple times), use them like global free functions.

//--------------------------------------------------------------------------------------------------
// docu [to provide type information we use dummy stubs]

#ifdef DOCXX
/**@name eC_Math
@memo
Arithmetic macros and functions.
@doc
These functions are meant to be used when working with noninteger data inside <B>eGML</B>.
The interface uses \Ref{eC_Value} which can be mapped to float or fixedpoint at compiletime.
While you should use eC_Values where possible all macros and functions have special versions to be used on \Ref{eC_Float} or \Ref{eC_Fixed} as well.
This way certain parts of an algorithm that needs special treatment can use fixedpoint or floatingpoint explicitly. <BR>
To get more information about \Ref{eC_Math} and its handling, look at the \Ref{Tutorial_eC}.
@see eC_Value, eC_Float, eC_Fixed
@see Tutorial_eC
*/

//@{

//--------------------------------------------------------------------------------------------------
/**@name eC_Value_macros
@memo
Macros for eC_Value parameters.
@doc
Please note that the compiler will <I>not</I> give you a warning when casting eC_Value (while represented as fixedpoint) to int or long, make sure
that you use the provided set of macros (\Ref{eC_FromInt}, \Ref{eC_FromFloat}, ..) to convert constants to \Ref{eC_Value}. In case value is mapped
to \Ref{eC_Float}, this conversion might be done automatically by the compiler, so you will not notice any errors due to missing conversions
- until switching value back to \Ref{eC_Fixed}.
@see eC_Float_macros, eC_Fixed_macros
*/

//@{

/** Multiplies two eC_Values.
@param a First factor.
@param b Second factor.
@return Result of multiplication, will overflow if product is larger than \Ref{eC_MAXVALUE}.
@see eC_MulQ, eC_MulFrac */
eC_Value eC_Mul(eC_Value a, eC_Value b);

/** 'Quick' version of eC_Value multiply instruction. When using fixedpoint values precision is reduced by half to avoid 64 bit
arithmetics during multiply.
@param a First factor.
@param b Second factor.
@return Result of quick multiplication.
@see eC_Mul, eC_MulFrac */
eC_Value eC_MulQ(eC_Value a, eC_Value b);

/** Special case multiply instruction for small values. In case both operands are lower than one (fractional), eC_MulFrac can be used
for multiplication. When using fixedpoint arithmetics this will help optimising the operation.
@param a First factor.
@param b Second factor.
@return Result of fractional multiplication.
@see eC_Mul, eC_MulQ */
eC_Value eC_MulFrac(eC_Value a, eC_Value b);

/** Divides two eC_Values. The first parameter will be divided by the second.
@param a Dividend.
@param b Divisor. Must <I>NOT</I> be zero.
@return Result of division, will underflow if quotient is smaller than \Ref{eC_MINVALUE}.
@see eC_DivQ */
eC_Value eC_Div(eC_Value a, eC_Value b);

/** 'Quick' version of eC_Value divide instruction. The first parameter will be divided by the second. When using fixedpoint
values precision is reduced to 'guard' bits to avoid 64 bit arithmetics during division.
@param a Dividend, upper 'guard' bits should be zero.
@param b Divisor. Must <I>NOT</I> be zero.
@param c Number of quard bits. You are able to avoid 64 bit arithmetics during division with it.
@return Result of 'quick' division.
@see eC_Div */
eC_Value eC_DivQ(eC_Value a, eC_Value b, eC_UInt guard);

/** Calculates the floor of an eC_Value. Floor is defined to be the largest integer that is less than or equal to parameter.
@param a Value to get the floor value from.
@return Floor value.
@see eC_Ceil */
eC_Value eC_Floor(eC_Value a);

/** Calculates the ceiling of an eC_Value. Ceiling is defined to be the smallest integer that is greater than or equal to parameter.
@param a Value to get the ceiling value from.
@return Ceiling value.
@see eC_Floor */
eC_Value eC_Ceil(eC_Value a);

/** Provides the fractional part of an eC_Value. Result is equal to <TT><B>a</B> - eC_Floor(<B>a</B>)</TT>.
@param a Value to get the fractional part from.
@return Value with just a fractional part.
@see eC_FracToByte, eC_ToInt, eC_Round */
eC_Value eC_Frac(eC_Value a);

/** Provides the fractional part of an eC_Value as a 8 bit integer. The fraction is scaled by 256 before truncating to integer, <BR>
i.e. <TT>eC_FracToByte(42.5) = 128</TT>.
@param a Value to get the fractional part from.
@return Fractional part as 8 bit integer value.
@see eC_Frac */
eC_UByte eC_FracToByte(eC_Value a);

/** Calculates absolute value of an eC_Value.
@param a Signed value to get the absolute value from.
@return Absolute value. */
eC_Value eC_Abs(eC_Value a);

/** Finds the minimum of two eC_Values.
@param a First value.
@param b Second value.
@return Minimum (signed) value of both arguments.
@see eC_Max */
eC_Value eC_Min(eC_Value a, eC_Value b);

/** Finds the maximum of two eC_Values.
@param a First value.
@param b Second value.
@return Maximum (signed) value of both arguments.
@see eC_Min */
eC_Value eC_Max(eC_Value a, eC_Value b);

/** Casts an eC_Value to int and discards its fractional part. See \Ref{eC_Round} if rounding is desired instead.
@param a Value to cast.
@return Signed integer part of the argument.
@see eC_ToFloat, eC_FromInt, eC_Round, eC_Frac */
eC_Int eC_ToInt(eC_Value a);

/** Converts an eC_Value to eC_Float. If the internal representation of eC_Value is already <I>eC_Float</I> this function has got no effect.
@param a Value to convert.
@return Converted eC_Float value.
@see eC_ToInt */
eC_Float eC_ToFloat(eC_Value a);

/** Casts an eC_Value to int and rounds it to the nearest integer.
@param a Value to cast.
@return Rounded signed integer part of the argument.
@see eC_ToInt, eC_Frac */
eC_Int eC_Round(eC_Value a);

/** Converts an integer to an eC_Value. Always use this function (or \Ref{eC_FromFloat}) when assigning constants to eC_Values.
@param a Integer value to convert.
@return Conversion result.
@see eC_FromFloat, eC_ToInt */
eC_Value eC_FromInt(eC_Int a);

/** Converts an eC_Float to an eC_Value. Always use this function (or \Ref{eC_FromInt}) when assigning constants to eC_Values.
@param a Floatingpoint value to convert.
@return Conversion result.
@see eC_FromInt, eC_ToFloat */
eC_Value eC_FromFloat(eC_Float a);

/** Provides square root of a given argument.
@param a Value to get square root from.
@return Square root.
@see eC_ISqrt, eC_Len2 */
eC_Value eC_Sqrt(eC_Value a);

/** Provides inverse square root of a given argument. Results: <TT>1/eC_Sqrt(<B>a</B>)</TT>.
@param a Value to get inverse square root from.
@return Inverse square root.
@see eC_Sqrt */
eC_Value eC_ISqrt(eC_Value a);

/** Provides sine value of a given radiant angle.
@param a Radiant angle.
@return Sine of given angle.
@see eC_Cos, eC_ASin */
eC_Value eC_Sin(eC_Value a);

/** Provides cosine value of a given radiant angle.
@param a Radiant angle.
@return Cosine of given angle.
@see eC_Sin, eC_ACos */
eC_Value eC_Cos(eC_Value a);

/** Provides tangent value of a given radiant angle.
@param a Radiant angle.
@return Tangent of given angle.
@see eC_ATan, eC_ATan2 */
eC_Value eC_Tan(eC_Value a);

/** Provides arc tangent of a given value.
@param a Value to get arc tangent from.
@return Arc tangent as radiant angle.
@see eC_Tan, eC_ATan2 */
eC_Value eC_ATan(eC_Value a);

/** Provides arc sine of a given value.
@param a Value to get arc sine from.
@return Arc sine as radiant angle.
@see eC_Sin, eC_ACos */
eC_Value eC_ASin(eC_Value a);

/** Provides arc cosine of a given value.
@param a Value to get arc cosine from.
@return Arc cosine as radiant angle.
@see eC_Cos, eC_ASin */
eC_Value eC_ACos(eC_Value a);

/** Provides arc tangent of a division result.
@param a First value - the dividend.
@param b Second value - the divisor. Must <I>NOT</I> be zero.
@return Arc tangent of division result. The return value is a radiant angle.
@see eC_Tan, eC_ATan */
eC_Value eC_ATan2(eC_Value a, eC_Value b);

/** Applys theorem of Pythagoras to two given values. Results: <TT>sqrt(a * a + b * b)</TT>.
Use eC_Len2 instead of \Ref{eC_Mul} and \Ref{eC_Sqrt} seperately to avoid problems of value range.
@param a First value.
@param b Second value.
@return <TT>sqrt(a * a + b * b)</TT>.
@see eC_Mul, eC_Sqrt */
eC_Value eC_Len2(eC_Value a, eC_Value b);

//@}

//--------------------------------------------------------------------------------------------------
/**@name eC_Float_macros
@memo
macros for eC_Float parameters.
@doc
All macros which are defined for \Ref{eC_Value} can be used with an <B>Flt</B> prefix as well. But not all of them make sense, documented here
are only those functions that are usefull when working directly with \Ref{eC_Float}. You should revert to the standard operators (<TT>+ - * /</TT>)
if there are no plans to rewrite the code for operating on eC_Values or \Ref{eC_Fixed} numbers.
@see eC_Value_macros, eC_Fixed_macros
*/

//@{

/** Calculates the floor of an eC_Float. Floor is defined to be the largest integer that is less than or equal to parameter.
@param a Value to get the floor value from.
@return Floor value.
@see eC_FltCeil */
eC_Float eC_FltFloor(eC_Float a);

/** Calculates the ceiling of an eC_Float. Ceiling is defined to be the smallest integer that is greater than or equal to parameter.
@param a Value to get the ceiling value from.
@return Ceiling value.
@see eC_FltFloor */
eC_Float eC_FltCeil(eC_Float a);

/** Provides the fractional part of an eC_Float. Result is equal to <TT><B>a</B> - eC_FltFloor(<B>a</B>)</TT>.
@param a Value to get the fractional part from.
@return Value with just a fractional part.
@see eC_FltFracToByte, eC_FltToInt, eC_FltRound */
eC_Float eC_FltFrac(eC_Value a);

/** Provides the fractional part of an eC_Float as a 8 bit integer. The fraction is scaled by 256 before truncating to integer, <BR>
i.e. <TT>eC_FltFracToByte(42.5) = 128</TT>.
@param a Value to get the fractional part from.
@return Fractional part as 8 bit integer value.
@see eC_FltFrac */
eC_UByte eC_FltFracToByte(eC_Float a);

/** Calculates absolute value of an eC_Float.
@param a Floatingpoint value to get the absolute value from.
@return Absolute value. */
eC_Float eC_FltAbs(eC_Float a);

/** Finds the minimum of two eC_Floats.
@param a First value.
@param b Second value.
@return Minimum (signed) floatingpoint value of both arguments.
@see eC_FltMax */
eC_Float eC_FltMin(eC_Float a, eC_Float b);

/** Finds the maximum of two eC_Floats.
@param a First value.
@param b Second value.
@return Maximum (signed) floatingpoint value of both arguments.
@see eC_FltMin */
eC_Float eC_FltMax(eC_Float a, eC_Float b);

/** Casts an eC_Float to int and discards its fractional part. See \Ref{eC_FltRound} if rounding is desired instead.
@param a Value to cast.
@return Signed integer part of the argument.
@see eC_FltToValue, eC_FltFromValue, eC_FltRound, eC_FltFrac */
eC_Int eC_FltToInt(eC_Float a);

/** Casts an eC_Float to int and rounds it to the nearest integer.
@param a Value to cast.
@return Rounded signed integer part of the argument.
@see eC_FltToInt, eC_FltFrac */
eC_Int eC_FltRound(eC_Float a);

/** Converts an eC_Float to an eC_Value. If the internal representation of eC_Value is already <I>eC_Float</I> this function has got no effect.
@param a Floatingpoint value to convert.
@return Conversion result.
@see eC_FltToInt, eC_FltFromValue */
eC_Value eC_FltToValue(eC_Float a);

/** Converts an eC_Value to eC_Float. If the internal representation of eC_Value is already <I>eC_Float</I> this function has got no effect.
@param a Value to convert.
@return Converted eC_Float value.
@see eC_FltToValue */
eC_Float eC_FltFromValue(eC_Value a);

/** Provides square root of a given argument.
@param a Value to get square root from.
@return Square root.
@see eC_FltISqrt, eC_FltLen2 */
eC_Float eC_FltSqrt(eC_Float a);

/** Provides inverse square root of a given argument. Results: <TT>1/eC_FltSqrt(<B>a</B>)</TT>.
@param a Value to get inverse square root from.
@return Inverse square root.
@see eC_FltSqrt */
eC_Float eC_FltISqrt(eC_Float a);

/** Provides sine value of a given radiant angle.
@param a Radiant angle.
@return Sine of given angle.
@see eC_FltCos, eC_FltASin */
eC_Float eC_FltSin(eC_Float a);

/** Provides cosine value of a given radiant angle.
@param a Radiant angle.
@return Cosine of given angle.
@see eC_FltSin, eC_FltACos */
eC_Float eC_FltCos(eC_Float a);

/** Provides tangent value of a given radiant angle.
@param a Radiant angle.
@return Tangent of given angle.
@see eC_FltATan, eC_FltATan2 */
eC_Float eC_FltTan(eC_Float a);

/** Provides arc tangent of a given value.
@param a Value to get arc tangent from.
@return Arc tangent as radiant angle.
@see eC_FltTan, eC_FltATan2 */
eC_Float eC_FltATan(eC_Float a);

/** Provides arc sine of a given value.
@param a Value to get arc sine from.
@return Arc sine as radiant angle.
@see eC_FltSin, eC_FltACos */
eC_Float eC_FltASin(eC_Float a);

/** Provides arc cosine of a given value.
@param a Value to get arc cosine from.
@return Arc cosine as radiant angle.
@see eC_FltCos, eC_FltASin */
eC_Float eC_FltACos(eC_Float a);

/** Provides arc tangent of a division result.
@param a First value - the dividend.
@param b Second value - the divisor. Must <I>NOT</I> be zero.
@return Arc tangent of division result. The return value is a radiant angle.
@see eC_FltTan, eC_FltATan */
eC_Float eC_FltATan2(eC_Float a, eC_Float b);

/** Applys theorem of Pythagoras to two given values. Results: <TT>sqrt(a * a + b * b)</TT>.
Use eC_FltLen2 instead of multiplication and \Ref{eC_FltSqrt} seperately to avoid problems of value range.
@param a First value.
@param b Second value.
@return <TT>sqrt(a * a + b * b)</TT>.
@see eC_FltSqrt */
eC_Float eC_FltLen2(eC_Float a, eC_Float b);

//@}

//--------------------------------------------------------------------------------------------------
/**@name eC_Fixed_macros
@memo
macros for eC_Fixed parameters.
@doc
All macros which are defined for \Ref{eC_Value} can be used with an <B>Fix</B> prefix as well. If fixedpoint is the most used number representation
in your application, map eC_Value to fixed and use value instead of \Ref{eC_Fixed}. Explicit use of fixedpoint numbers is mostly designed for inner
loops and other timecritical parts that can function within the limited range of fixedpoint resolution.
@see eC_Value_macros, eC_Float_macros, eC_Float
*/

//@{

/** Multiplies two eC_Fixeds.
@param a First factor.
@param b Second factor.
@return Result of multiplication, will overflow if product is larger than integer bits allow (refer to \Ref{eC_MAXVALUE}).
@see eC_FixMulQ */
eC_Fixed eC_FixMul(eC_Fixed a, eC_Fixed b);

/** Divides two eC_Fixeds. The first parameter will be divided by the second.
@param a Dividend.
@param b Divisor. Must <I>NOT</I> be zero.
@return Result of division.
@see eC_FixDivQ */
eC_Fixed eC_FixDiv(eC_Fixed a, eC_Fixed b);

/** 'Quick' version of eC_Fixed multiply instruction. Indicates that precision is reduced by half to avoid 64 bit arithmetics during multiply.
@param a First factor.
@param b Second factor.
@return Result of quick multiplication.
@see eC_FixMul */
eC_Fixed eC_FixMulQ(eC_Fixed a, eC_Fixed b);

/** 'Quick' version of eC_Fixed divide instruction. The first parameter will be divided by the second. Indicates that
precision is reduced to 'guard' bits to avoid 64 bit arithmetics during division.
@param a Dividend, upper 'guard' bits should be zero.
@param b Divisor. Must <I>NOT</I> be zero.
@param c Number of quard bits. You are able to avoid 64 bit arithmetics during division with it.
@return Result of 'quick' division.
@see eC_FixDiv */
eC_Fixed eC_FixDivQ(eC_Fixed a, eC_Fixed b, eC_UInt guard);

/** Calculates the floor of an eC_Fixed. Floor is defined to be the largest integer that is less than or equal to parameter.
@param a Value to get the floor value from.
@return Floor value.
@see eC_FixCeil */
eC_Fixed eC_FixFloor(eC_Fixed a);

/** Calculates the ceiling of an eC_Fixed. Ceiling is defined to be the smallest integer that is greater than or equal to parameter.
@param a Value to get the ceiling value from.
@return Ceiling value.
@see eC_FixFloor */
eC_Fixed eC_FixCeil(eC_Fixed a);

/** Provides the fractional part of an eC_Fixed. Result is equal to <TT><B>a</B> - eC_FixFloor(<B>a</B>)</TT>.
@param a Value to get the fractional part from.
@return Value with just a fractional part.
@see eC_FixFracToByte, eC_FixToInt, eC_FixRound */
eC_Fixed eC_FixFrac(eC_Fixed a);

/** Provides the fractional part of an eC_Fixed as a 8 bit integer. The fraction is scaled by 256 before truncating to integer, <BR>
i.e. <TT>eC_FixFracToByte(42.5) = 128</TT>.
@param a Value to get the fractional part from.
@return Fractional part as 8 bit integer value.
@see eC_FixFrac */
eC_UByte eC_FixFracToByte(eC_Fixed a);

/** Calculates absolute value of an eC_Fixed.
@param a Signed value to get the absolute value from.
@return Absolute value. */
eC_Fixed eC_FixAbs(eC_Fixed a);

/** Finds the minimum of two eC_Fixeds.
@param a First value.
@param b Second value.
@return Minimum (signed) value of both arguments.
@see eC_FixMax */
eC_Fixed eC_FixMin(eC_Fixed a, eC_Fixed b);

/** Finds the maximum of two eC_Fixeds.
@param a First value.
@param b Second value.
@return Maximum (signed) value of both arguments.
@see eC_FixMin */
eC_Fixed eC_FixMax(eC_Fixed a, eC_Fixed b);

/** Casts an eC_Fixed to integer and discards its fractional part. See \Ref{eC_FixRound} if rounding is desired instead.
@param a Value to cast.
@return Signed integer part of the argument.
@see eC_FixToValue, eC_FixFromInt, eC_FixRound, eC_FixFrac */
eC_Int eC_FixToInt(eC_Fixed a);

/** Casts an eC_Fixed to int and rounds it to the nearest integer.
@param a Value to cast.
@return Rounded signed integer part of the argument.
@see eC_FixToInt, eC_FixFrac */
eC_Int eC_FixRound(eC_Fixed a);

/** Converts an eC_Fixed to an eC_Value. If the internal representation of eC_Value is already <I>eC_Fixed</I> this function has got no effect.
@param a Fixpoint value to convert.
@return Conversion result.
@see eC_FixToInt, eC_FixFromValue */
eC_Value eC_FixToValue(eC_Fixed a);

/** Converts an eC_Value to eC_Fixed. If the internal representation of eC_Value is already <I>eC_Fixed</I> this function has got no effect.
@param a Value to convert.
@return Converted eC_Fixed value.
@see eC_FixToValue */
eC_Fixed eC_FixFromValue(eC_Value a);

/** Converts an integer to an eC_Fixed. Always use this function (or \Ref{eC_FixFromFloat}) when assigning constants to eC_Fixed values.
@param a Integer value to convert.
@return Conversion result.
@see eC_FixFromFloat, eC_FixToInt */
eC_Fixed eC_FixFromInt(eC_Int a);

/** Converts an eC_Float to an eC_Fixed. Always use this function (or \Ref{eC_FixFromInt}) when assigning constants to eC_Fixed values.
@param a Floatingpoint value to convert.
@return Conversion result.
@see eC_FixFromInt, eC_FixFromValue */
eC_Fixed eC_FixFromFloat(eC_Float a);

/** Provides square root of a given argument.
@param a Value to get square root from.
@return Square root.
@see eC_FixISqrt, eC_FixLen2 */
eC_Fixed eC_FixSqrt(eC_Fixed a);

/** Provides inverse square root of a given argument. Results: <TT>1/eC_FixSqrt(<B>a</B>)</TT>.
@param a Value to get inverse square root from.
@return Inverse square root.
@see eC_FixSqrt */
eC_Fixed eC_FixISqrt(eC_Fixed a);

/** Provides sine value of a given radiant angle.
@param a Radiant angle.
@return Sine of given angle.
@see eC_FixCos, eC_FixASin */
eC_Fixed eC_FixSin(eC_Fixed a);

/** Provides cosine value of a given radiant angle.
@param a Radiant angle.
@return Cosine of given angle.
@see eC_FixSin, eC_FixACos */
eC_Fixed eC_FixCos(eC_Fixed a);

/** Provides tangent value of a given radiant angle.
@param a Radiant angle.
@return Tangent of given angle.
@see eC_FixATan, eC_FixATan2 */
eC_Fixed eC_FixTan(eC_Fixed a);

/** Provides arc tangent of a given value.
@param a Value to get arc tangent from.
@return Arc tangent as radiant angle.
@see eC_FixTan, eC_FixATan2 */
eC_Fixed eC_FixATan(eC_Fixed a);

/** Provides arc sine of a given value.
@param a Value to get arc sine from.
@return Arc sine as radiant angle.
@see eC_FixSin, eC_FixACos */
eC_Fixed eC_FixASin(eC_Fixed a);

/** Provides arc cosine of a given value.
@param a Value to get arc cosine from.
@return Arc cosine as radiant angle.
@see eC_FixCos, eC_FixASin */
eC_Fixed eC_FixACos(eC_Fixed a);

/** Provides arc tangent of a division result.
@param a First value - the dividend.
@param b Second value - the divisor. Must <I>NOT</I> be zero.
@return Arc tangent of division result. The return value is a radiant angle.
@see eC_FixTan, eC_FixATan */
eC_Fixed eC_FixATan2(eC_Fixed a, eC_Fixed b);

/** Applys theorem of Pythagoras to two given values. Results: <TT>sqrt(a * a + b * b)</TT>.
Use eC_FixLen2 instead of \Ref{eC_FixMul} and \Ref{eC_FixSqrt} seperately to avoid problems of value range.
@param a First value.
@param b Second value.
@return <TT>sqrt(a * a + b * b)</TT>.
@see eC_FixMul, eC_FixSqrt */
eC_Fixed eC_FixLen2(eC_Fixed a, eC_Fixed b);

//@}

#endif

//--------------------------------------------------------------------------------------------------
/**@name eC_Math_Constants
@memo
Constants used by and for eC_Math macros.
@doc
These constants are already typed as \Ref{eC_Value} so you do <I>not</I> have to use \Ref{eC_FromFloat} on them.<BR>
They are all '#defines' (to provide best optimiser results), so you can not dereference them.
*/

//@{

/** Pi.
@return Number Pi as an eC_Value type.
@see eC_Value */
#define eC_PI eC_FromFloat(3.14159265358979323846f)

/** Pi/2.
@return Number 'Pi/2' as an eC_Value type.
@see eC_Value */
#define eC_HALF_PI eC_FromFloat(1.57079632679489661923f)

/** Pi/180.
@return Number 'Pi/180' as an eC_Value type to convert between degree and radiant angles.
@see eC_Value */
#define eC_RAD eC_FromFloat(0.01745329251994329576f)

//@}

//@}

//===============================================================
//===============================================================
// Fixpoint default implementations
//===============================================================
//===============================================================

#define eC_FixFloor_Default(a)					( (a) & eC_FP_HIGH )
#define eC_FixCeil_Default(a)					( ((a) + eC_FP_LOW) & eC_FP_HIGH )
#define eC_FixToInt_Default(a)					( (EC_STATIC_CAST(eC_Int, a)) >> eC_FP_LBIT )
#define eC_FixToIntPos_Default(a)				( (((EC_STATIC_CAST(eC_Int, a) >> ((eC_FP_LBIT+eC_FP_HBIT)-1)) & eC_FP_LOW) + EC_STATIC_CAST(eC_Int, a)) >> eC_FP_LBIT )
#define eC_FixToFloat_Default(a)				( EC_STATIC_CAST(eC_Float, a) / EC_STATIC_CAST(eC_Float, eC_FP_ONE) )
#define eC_FixRound_Default(a)					( ((EC_STATIC_CAST(eC_Int, a) + (eC_FP_ONE/2)) >> eC_FP_LBIT) )
#define eC_FixFromInt_Default(a)				( EC_STATIC_CAST(eC_Fixed, ((EC_STATIC_CAST(eC_Int, a)) << eC_FP_LBIT)) )
#define eC_FixFromFloat_Default(a)				( EC_STATIC_CAST(eC_Fixed, ((a) * EC_STATIC_CAST(eC_Float, eC_FP_ONE))) )
#define eC_FixFrac_Default(a)					( (a) & eC_FP_LOW )
#define eC_FixFracToByte_Default(a)				( EC_STATIC_CAST(eC_UByte, ((a) >> (eC_FP_LBIT-8)) & 0xff ))

inline eC_Fixed eC_FixAbs_Default(eC_Fixed vX)				{ return (vX>=0)?vX:-vX; }
inline eC_Fixed eC_FixMin_Default(eC_Fixed vA, eC_Fixed vB) { return (vA<vB)?vA:vB; }
inline eC_Fixed eC_FixMax_Default(eC_Fixed vA, eC_Fixed vB) { return (vA>vB)?vA:vB; }

#define eC_FixSqrt_Default(a)		( eC_FromFloat( eC_FltSqrt(eC_ToFloat(a)) ) )
#define eC_FixISqrt_Default(a)		( eC_FromFloat( 1.0f / eC_FltSqrt(eC_ToFloat(a)) ) )
#define eC_FixSin_Default(a)		( eC_FromFloat( eC_FltSin(eC_ToFloat(a)) ) )
#define eC_FixCos_Default(a)		( eC_FromFloat( eC_FltCos(eC_ToFloat(a)) ) )
#define eC_FixTan_Default(a)		( eC_FromFloat( eC_FltTan(eC_ToFloat(a)) ) )
#define eC_FixATan_Default(a)		( eC_FromFloat( eC_FltATan(eC_ToFloat(a)) ) )
#define eC_FixASin_Default(a)		( eC_FromFloat( eC_FltASin(eC_ToFloat(a)) ) )
#define eC_FixACos_Default(a)		( eC_FromFloat( eC_FltACos(eC_ToFloat(a)) ) )
#define eC_FixATan2_Default(a,b)	( eC_FromFloat( eC_FltATan2(eC_ToFloat(a),eC_ToFloat(b)) ) )
#define eC_FixExp_Default(a)        ( eC_FromFloat( eC_FltExp( eC_ToFloat(a)) ) )
#define eC_FixPow_Default(a,b)      ( eC_FromFloat( eC_FltPow( eC_ToFloat(a),eC_ToFloat(b)) ) )

#define eC_FixMul_Default(a, b)		( EC_STATIC_CAST(eC_Fixed, ( EC_STATIC_CAST(eC_Float, a) * ((b)/EC_STATIC_CAST(eC_Float, eC_FP_ONE)) ) ) )
#define eC_FixMulQ_Default(a, b)	( EC_STATIC_CAST(eC_Fixed, ( (((a) >> (eC_FP_LBIT/2)) * ((b) >> (eC_FP_LBIT - (eC_FP_LBIT/2)))) ) ) )

#define eC_FixMulFrac_Default(a, b) ( ((a)*(b)) >> eC_FP_LBIT )

#define eC_FixLShift_Default(a,b)   ((a)<<(b))
#define eC_FixRShift_Default(a,b)   ((a)>>(b))

#define eC_FixDiv_Default(a, b)   ( EC_STATIC_CAST(eC_Fixed, ( EC_STATIC_CAST(eC_Float, a) / ((b)/EC_STATIC_CAST(eC_Float,eC_FP_ONE)) ) ) )
#define eC_FixDivQ_Default(a, b, c)	( EC_STATIC_CAST(eC_Fixed, ( ((a) << (c)) / ((b) >> (eC_FP_LBIT - (c)))) ) )

#if (eC_FP_SIZE) < 16
#define eC_FixReciQ_Default(a)		eC_FixDivQ( eC_FixFromInt(1), (a), eC_FP_HBIT - 2 )
#else
#define eC_FixReciQ_Default(a)		eC_FixDivQ( eC_FixFromInt(1), (a), eC_FP_LBIT - 2 )
#endif
#define eC_FixReci_Default(a)       eC_FixDiv( eC_FixFromInt(1), (a) )

inline eC_Fixed eC_FixLen2_Default(eC_Fixed fxA, eC_Fixed fxB);
inline eC_Fixed eC_FixLen3_Default(eC_Fixed fxA, eC_Fixed fxB, eC_Fixed fxC);

//---------------------------------------------------------------
// fixed point math 'safe' functions (cant overflow)
//

#define eC_FixFloor(a)      eC_FixFloor_Default(a)
#define eC_FixCeil(a)       eC_FixCeil_Default(a)
#define eC_FixToInt(a)      eC_FixToInt_Default(a)
#define eC_FixToIntPos(a)   eC_FixToIntPos_Default(a)
#define eC_FixToFloat(a)    eC_FixToFloat_Default(a)
#define eC_FixRound(a)      eC_FixRound_Default(a)
#define eC_FixFromInt(a)    eC_FixFromInt_Default(a)
#define eC_FixFromFloat(a)  eC_FixFromFloat_Default(a)
#define eC_FixFrac(a)       eC_FixFrac_Default(a)
#define eC_FixFracToByte(a) eC_FixFracToByte_Default(a)

#define eC_FixAbs(a)        eC_FixAbs_Default(a)
#define eC_FixMin(a, b)     eC_FixMin_Default(a, b)
#define eC_FixMax(a, b)     eC_FixMax_Default(a, b)
#define eC_FixSqrt(a)       eC_FixSqrt_Default(a)
#define eC_FixISqrt(a)      eC_FixISqrt_Default(a)

#ifdef eC_USE_TRIG_TABLE
#define eC_FixSin(a)        eC_FixSin_Table(a)
#define eC_FixCos(a)        eC_FixCos_Table(a)
#else
#define eC_FixSin(a)        eC_FixSin_Default(a)
#define eC_FixCos(a)        eC_FixCos_Default(a)
#endif //eC_USE_TRIG_TABLE

#define eC_FixTan(a)        eC_FixTan_Default(a)
#define eC_FixATan(a)       eC_FixATan_Default(a)
#define eC_FixExp(a)        eC_FixExp_Default(a)
#define eC_FixPow(a,b)      eC_FixPow_Default(a,b)

#define eC_FixASin(a)       eC_FixASin_Default(a)
#define eC_FixACos(a)       eC_FixACos_Default(a)
#define eC_FixATan2(a,b)    eC_FixATan2_Default(a,b)

#define eC_FixLen2(a,b)     eC_FixLen2_Default(a,b)
#define eC_FixLen3(a,b,c)   eC_FixLen3_Default(a,b,c)


#define eC_FixLShift(a,b)   eC_FixLShift_Default(a,b)
#define eC_FixRShift(a,b)   eC_FixRShift_Default(a,b)

//---------------------------------------------------------------
// general fixedpoint mul/div using fpu or reduced precision
// should be replaced by processorspecific 64bit arithmetics if possible
//

#define eC_FixMul(a, b)	        eC_FixMul_Default(a, b)
#define eC_FixMulQ(a, b)        eC_FixMulQ_Default(a, b)
#define eC_FixDiv(a, b)         eC_FixDiv_Default(a, b)
#define eC_FixDivQ(a, b, c)     eC_FixDivQ_Default(a, b, c)
#define eC_FixMulFrac(a, b)     eC_FixMulFrac_Default(a, b)
#define eC_FixReci(a)           eC_FixReci_Default(a)
#define eC_FixReciQ(a)          eC_FixReciQ_Default(a)

//===============================================================
//===============================================================
// floating point implementations
//===============================================================
//===============================================================

#define eC_FltMul_Default(a, b)			( (a) * (b) )
#define eC_FltMulQ_Default(a, b)		( (a) * (b) )
#define eC_FltMulFrac_Default(a, b)     ( (a) * (b) )

#define eC_FltLShift_Default(a,b)		((a)*(1<<b))
#define eC_FltRShift_Default(a,b)		((a)/(1<<b))


#define eC_FltDiv_Default(a, b)			( (a) / (b) )
#define eC_FltDivQ_Default(a, b, c)		( (a) / (b) )
#define eC_FltFloor_Default(a)			( EC_STATIC_CAST(eC_Float, floor(a) ) )
#define eC_FltCeil_Default(a)			( EC_STATIC_CAST(eC_Float, ceil(a) ) )
#define eC_FltToInt_Default(a)			( EC_STATIC_CAST(eC_Int, a) )
#define eC_FltToFloat_Default(a)		( (a) )
#define eC_FltRound_Default(a)			( EC_STATIC_CAST(eC_Int, ((a) + 0.5f) ) )
#define eC_FltFromInt_Default(a)		( EC_STATIC_CAST(eC_Float, (a) ) )
#define eC_FltFromFloat_Default(a)		( EC_STATIC_CAST(eC_Float, (a) ) )
#define eC_FltFracToByte_Default(a)		EC_STATIC_CAST(eC_UByte, ( EC_STATIC_CAST(eC_Int, (eC_FltFrac(a) * (eC_Float)0x100) ) ))
#define eC_FltISqrt_Default(a)			( 1.0f / eC_FltSqrt(a) )

#define eC_FltReci_Default(a)           ( 1.0f / (a) )
#define eC_FltReciQ_Default(a)          ( 1.0f / (a) )

inline eC_Float eC_FltAbs_Default(eC_Float vX)					{ return (vX>=0)?vX:-vX; }
inline eC_Float eC_FltFrac_Default(eC_Float vX)					{ return vX - (eC_Float)floor(vX); }
inline eC_Float eC_FltMin_Default(eC_Float vA, eC_Float vB)		{ return (vA<vB)?vA:vB; }
inline eC_Float eC_FltMax_Default(eC_Float vA, eC_Float vB)		{ return (vA>vB)?vA:vB; }


//Special version of Sqrt to avoid FPU traps

#if  defined(eC_TARGET_ENV_NAVIO2) || defined(eC_TARGET_ENV_RR) || defined(eC_TARGET_ENV_CRUSADER)

inline eC_Float eC_FltSqrt_Default( eC_Float a )
{
	if ( a > EC_STATIC_CAST(eC_Float, 0.0) )
		return EC_STATIC_CAST(eC_Float, sqrt(a));
	else
		return EC_STATIC_CAST(eC_Float, 0.0);
}

#else

inline eC_Float eC_FltSqrt_Default( eC_Float a )
{
	return EC_STATIC_CAST(eC_Float, sqrt(a));
}

#endif

inline eC_Float eC_FltSin_Default( eC_Float a )
{
	return EC_STATIC_CAST(eC_Float, sin(a));
}

inline eC_Float eC_FltASin_Default( eC_Float a )
{
	return EC_STATIC_CAST(eC_Float, asin(a));
}

inline eC_Float eC_FltCos_Default( eC_Float a )
{
	return EC_STATIC_CAST(eC_Float, cos(a));
}

inline eC_Float eC_FltACos_Default( eC_Float a )
{
	return EC_STATIC_CAST(eC_Float, acos(a));
}

inline eC_Float eC_FltTan_Default( eC_Float a )
{
	return EC_STATIC_CAST(eC_Float, tan(a));
}

inline eC_Float eC_FltATan_Default( eC_Float a )
{
	return EC_STATIC_CAST(eC_Float, atan(a));
}

inline eC_Float eC_FltATan2_Default( eC_Float a, eC_Float b )
{
	return EC_STATIC_CAST(eC_Float, atan2(a,b));
}

inline eC_Float eC_FltExp_Default( eC_Float a )
{
	return EC_STATIC_CAST(eC_Float, exp(a));
}

inline eC_Float eC_FltPow_Default( eC_Float a, eC_Float b )
{
	return EC_STATIC_CAST(eC_Float, pow(a, b));
}

inline eC_Float eC_FltLen2_Default( eC_Float a, eC_Float b );

inline eC_Float eC_FltLen3_Default( eC_Float a, eC_Float b, eC_Float c );

inline eC_Int eC_FltExponent_Default( eC_Float a )
{
    int e;
    frexp( EC_STATIC_CAST(double, a), &e );
    return EC_STATIC_CAST(eC_Int, e);
}


#define eC_FltMul(a, b)         eC_FltMul_Default(a, b)
#define eC_FltMulFrac(a, b)     eC_FltMulFrac_Default(a, b)
#define eC_FltMulQ(a, b)	    eC_FltMulQ_Default(a, b)
#define eC_FltDiv(a, b)		    eC_FltDiv_Default(a, b)
#define eC_FltDivQ(a, b, c)	    eC_FltDivQ_Default(a, b, c)
#define eC_FltFloor(a)		    eC_FltFloor_Default(a)
#define eC_FltCeil(a)		    eC_FltCeil_Default(a)
#define eC_FltToInt(a)		    eC_FltToInt_Default(a)
#define eC_FltToFloat(a)	    eC_FltToFloat_Default(a)
#define eC_FltRound(a)		    eC_FltRound_Default(a)
#define eC_FltFromInt(a)	    eC_FltFromInt_Default(a)
#define eC_FltFromFloat(a)	    eC_FltFromFloat_Default(a)
#define eC_FltFracToByte(a)	    eC_FltFracToByte_Default(a)
#define eC_FltISqrt(a)		    eC_FltISqrt_Default(a)
#define eC_FltAbs(vX)		    eC_FltAbs_Default( vX)
#define eC_FltFrac(vX)		    eC_FltFrac_Default( vX)
#define eC_FltMin(vA, vB)	    eC_FltMin_Default( vA,  vB)
#define eC_FltMax(vA, vB)	    eC_FltMax_Default( vA,  vB)
#define eC_FltSqrt(a)           eC_FltSqrt_Default(  a )
#define eC_FltSin(a)            eC_FltSin_Default(  a )
#define eC_FltASin(a)           eC_FltASin_Default(  a )
#define eC_FltCos(a)            eC_FltCos_Default(  a )
#define eC_FltACos(a)           eC_FltACos_Default(  a )
#define eC_FltTan(a)            eC_FltTan_Default(  a )
#define eC_FltATan(a)           eC_FltATan_Default(  a )
#define eC_FltATan2(a,b)        eC_FltATan2_Default(  a,  b )
#define eC_FltLen2(a,b)         eC_FltLen2_Default(a , b)
#define eC_FltLen3(a,b,c)       eC_FltLen3_Default(a , b , c)
#define eC_FltReci(a)           eC_FltReci_Default( a )
#define eC_FltReciQ(a)          eC_FltReciQ_Default( a )
#define eC_FltExp(a)            eC_FltExp_Default( a )
#define eC_FltPow(a,b)          eC_FltPow_Default( a, b )
#define eC_FltExponent(a)       eC_FltExponent_Default( a )
#define eC_FltLShift(a,b)	    eC_FltLShift_Default( a , b )
#define eC_FltRShift(a,b)	    eC_FltRShift_Default( a , b )

//---------------------------------------------------------------
// implementations using macros
//

inline eC_Fixed eC_FixLen2_Default(eC_Fixed fxA, eC_Fixed fxB)
{
    eC_Float fA = eC_FixToFloat(fxA);
    eC_Float fB = eC_FixToFloat(fxB);
    return eC_FixFromFloat ( eC_FltSqrt( fA * fA + fB * fB ) );
}

inline eC_Fixed eC_FixLen3_Default(eC_Fixed fxA, eC_Fixed fxB, eC_Fixed fxC)
{
    eC_Float fA = eC_FixToFloat(fxA);
    eC_Float fB = eC_FixToFloat(fxB);
    eC_Float fC = eC_FixToFloat(fxC);
    return eC_FixFromFloat ( eC_FltSqrt( fA * fA + fB * fB + fC * fC ) );
}

inline eC_Float eC_FltLen2_Default( eC_Float a, eC_Float b )
{
    return eC_FltSqrt( a*a + b*b );
}

inline eC_Float eC_FltLen3_Default( eC_Float a, eC_Float b, eC_Float c )
{
    return eC_FltSqrt( a*a + b*b + c*c );
}

//===============================================================
//===============================================================
//
// special DEBUG Math routines for Win32 Env. with OVERFLOW Waring Output
//

#ifdef eC_TARGET_ENV_WIN32

#ifdef eC_FIXPOINT
#include "x86_win32/eC_x86_win32_Math_FIXPOINT.h"
#endif

#ifdef eC_SAVEMATH
#include "x86_win32/eC_x86_win32_Math_DEBUG.h"
#endif

#endif

//===============================================================
//===============================================================
//
// trimedia specific overloadind of the math functions
//

#if (eC_TARGET_COMPILER_TMC && eC_FP_SIZE==16)

#include "tm_navi/eC_tm_navi_Math.h"


#endif


//===============================================================
//===============================================================
//
// mips specific specific overloadind of the math functions
//

#if (eC_TARGET_ENV_RR && eC_FP_SIZE==16)

#include "mips_rr/eC_mips_rr_Math.h"

#endif

//===============================================================
//===============================================================
//
// linux specific specific overloadind of the math functions
//

#ifdef eC_TARGET_ENV_LINUX

#ifndef WIN32
#include "linux/eC_linux_Math.h"
#endif

#endif

//===============================================================
//===============================================================
//
// embedded linux specific specific overloadind of the math functions
//

#ifdef eC_TARGET_ENV_ELINUX

#ifndef WIN32
#include "elinux/eC_elinux_Math.h"
#endif

#endif

//===============================================================
//===============================================================
//
// OSE specific specific overloadind of the math functions
//

#ifdef eC_TARGET_ENV_OSE

#ifndef WIN32
#include "ose/eC_ose_Math.h"
#endif

#endif

//===============================================================
//===============================================================
//
// pocketpc specific specific overloadind of the math functions
//

#ifdef eC_TARGET_ENV_POCKET

#include "pocket/eC_pocket_Math.h"

#endif


//===============================================================
//===============================================================
//
// Ultratronik mmi StarterKit specific overloading of the math functions
//

#ifdef eC_TARGET_ENV_UTST

#include "utst/eC_utst_Math.h"

#endif


//===============================================================
//===============================================================
//
// NIOS specific overloading of the math functions
//

#ifdef eC_TARGET_ENV_NIOS

#include "nios/eC_nios_Math.h"

#endif


//===============================================================
//===============================================================
//
// RZA1H specific overloading of the math functions
//



#ifdef eC_TARGET_ENV_RZA1H

#include "RZA1H/eC_RZA1H_Math.h"

#endif

//===============================================================
//===============================================================
//
// Toshiba/Capricorn specific overloading of the math functions
//

#ifdef eC_TARGET_ENV_CAPI

#include "capi/eC_capi_Math.h"

#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_DSP


#include "eC_dsp_Math.h"

#endif

//-------------------------------------------------------------------------------------------
#ifdef eC_TARGET_ENV_DCX

#include "dcx/eC_dcx_Math.h"

#endif

#ifdef eC_TARGET_ENV_MAGIKMI
#ifdef eC_TARGET_ENV_ELINUX
#include "imx27_elinux/eC_imx27_elinux_Math.h"
#endif
#endif

//===============================================================
//===============================================================
// Map VALUE to fixedpoint
//===============================================================
//===============================================================
#ifdef eC_FIXPOINT

#define eC_Floor(a)			eC_FixFloor(a)
#define eC_Ceil(a)			eC_FixCeil(a)
#define eC_ToInt(a)			eC_FixToInt(a)
#define eC_ToFloat(a)		eC_FixToFloat(a)
#define eC_Round(a)			eC_FixRound(a)
#define eC_FromInt(a)		eC_FixFromInt(a)
#define eC_FromFloat(a)		eC_FixFromFloat(a)
#define eC_Frac(a)			eC_FixFrac(a)
#define eC_FracToByte(a)	eC_FixFracToByte(a)
#define eC_Abs(a)			eC_FixAbs(a)
#define eC_Min(a,b)			eC_FixMin(a,b)
#define eC_Max(a,b)			eC_FixMax(a,b)
#define eC_Mul(a,b)			eC_FixMul(a,b)
#define eC_MulQ(a,b)		eC_FixMulQ(a,b)
#define eC_MulFrac(a,b)		eC_FixMulFrac(a,b)
#define eC_Div(a,b)			eC_FixDiv(a,b)
#define eC_DivQ(a,b)		eC_FixDivQ(a,b)
#define eC_ReciQ(a)			eC_FixReciQ(a)
#define eC_Reci(a)			eC_FixReci(a)

#define eC_FixFromValue(a)	(a)
#define eC_FixToValue(a)	(a)
#define eC_FltFromValue(a)	eC_FixToFloat(a)
#define eC_FltToValue(a)	eC_FixFromFloat(a)

#define eC_Sqrt(a)			eC_FixSqrt(a)
#define eC_ISqrt(a)			eC_FixISqrt(a)
#define eC_Sin(a)			eC_FixSin(a)
#define eC_ASin(a)			eC_FixASin(a)
#define eC_Cos(a)			eC_FixCos(a)
#define eC_ACos(a)			eC_FixACos(a)
#define eC_Tan(a)			eC_FixTan(a)
#define eC_ATan(a)			eC_FixATan(a)
#define eC_ATan2(a,b)		eC_FixATan2(a,b)
#define eC_Exp(a)			eC_FixExp(a)
#define eC_Pow(a,b)		    eC_FixPow(a,b)

#define eC_Len2(a ,b)       eC_FixLen2(a ,b)
#define eC_Len3(a ,b, c)    eC_FixLen3(a ,b, c)

#define eC_LShift(a ,b)	    eC_FixLShift(a, b)
#define eC_RShift(a ,b)	    eC_FixRShift(a, b)


#else
//===============================================================
//===============================================================
// Map VALUE to float
//===============================================================
//===============================================================

#define eC_Floor(a)			eC_FltFloor(a)
#define eC_Ceil(a)			eC_FltCeil(a)
#define eC_ToInt(a)			eC_FltToInt(a)
#define eC_ToFloat(a)		eC_FltToFloat(a)
#define eC_Round(a)			eC_FltRound(a)
#define eC_FromInt(a)		eC_FltFromInt(a)
#define eC_FromFloat(a)		eC_FltFromFloat(a)
#define eC_Frac(a)			eC_FltFrac(a)
#define eC_FracToByte(a)	eC_FltFracToByte(a)
#define eC_Abs(a)			eC_FltAbs(a)
#define eC_Min(a,b)			eC_FltMin(a,b)
#define eC_Max(a,b)			eC_FltMax(a,b)
#define eC_Mul(a,b)			eC_FltMul(a,b)
#define eC_MulFrac(a,b)		eC_FltMulFrac(a,b)
#define eC_MulQ(a,b)		eC_FltMulQ(a,b)
#define eC_Div(a,b)			eC_FltDiv(a,b)
#define eC_DivQ(a,b)		eC_FltDivQ(a,b)
#define eC_ReciQ(a)			eC_FltReciQ(a)
#define eC_Reci(a)			eC_FltReci(a)

#define eC_FixFromValue(a)	eC_FixFromFloat(a)
#define eC_FixToValue(a)	eC_FixToFloat(a)
#define eC_FltFromValue(a)	(a)
#define eC_FltToValue(a)	(a)

#define eC_Sqrt(a)			eC_FltSqrt(a)
#define eC_ISqrt(a)			eC_FltISqrt(a)
#define eC_Sin(a)			eC_FltSin(a)
#define eC_ASin(a)			eC_FltASin(a)
#define eC_Cos(a)			eC_FltCos(a)
#define eC_ACos(a)			eC_FltACos(a)
#define eC_Tan(a)			eC_FltTan(a)
#define eC_ATan(a)			eC_FltATan(a)
#define eC_ATan2(a,b)		eC_FltATan2(a,b)
#define eC_Exp(a)			eC_FltExp(a)
#define eC_Pow(a,b)		    eC_FltPow(a,b)

#define eC_Len2(a ,b)       eC_FltLen2(a ,b)
#define eC_Len3(a ,b, c)    eC_FltLen3(a ,b, c)

#define eC_LShift(a ,b)	    eC_FltLShift(a, b)
#define eC_RShift(a ,b)	    eC_FltRShift(a, b)

#endif

//---------------------------------------------------------------
#endif
