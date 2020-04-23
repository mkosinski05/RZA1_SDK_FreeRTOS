/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_x86_win32_Math_DEBUG.h
* %version:        134.0 %
* 
* author:          hh04030
* creation date:   Mon Mar 27 14:22:20 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EC_WIN32_MATH_DEBUG__H_
#define EC_WIN32_MATH_DEBUG__H_

//-----------------
#include "eDBG.h"
//-----------------

#undef eC_FixFromInt
#undef eC_FixFromFloat

#undef eC_FixMul
#undef eC_FixMulQ

#undef eC_FixDiv
#undef eC_FixDivQ

#undef eC_FixReciQ

//------------------------------------------------------------------

#define eC_FixFromInt(a)   (eDBG_MARK, SecureFixFromFloat((float)(a)))
#define eC_FixFromFloat(a) (eDBG_MARK, SecureFixFromFloat((float)(a)))

inline eC_Fixed SecureFixFromFloat(eC_Float fA)
{
  if (fA > (float)32767) 
	{
	printf("%s",eDBG_AddPos("eC_FromFloat/Int: OVERFLOW (+)\n"));
	return (0x7fffffff);
	}
  if (fA < (float)-32768) 
	{
	printf("%s",eDBG_AddPos("eC_FromFloat/Int: OVERFLOW (-)\n"));
	return (0x80000000);
	}

  return ( (eC_Fixed) ((fA) * (float)eC_FP_ONE) );
}

//------------------------------------------------------------------

#define eC_FixMul(a, b)   (eDBG_MARK, SecureFixMul(a,b))
#define eC_FixMulQ(a, b)   (eDBG_MARK, SecureFixMul(a,b))

inline eC_Fixed SecureFixMul(eC_Fixed fA, eC_Fixed fB)
{
  if ((float)(fA)/(float)eC_FP_ONE * ((fB)/(float)eC_FP_ONE) > (float)32767) 
	{
	printf("%s",eDBG_AddPos("eC_Mul: OVERFLOW (+)\n"));
	return (0x7fffffff);
	}
  if ((float)(fA)/(float)eC_FP_ONE * ((fB)/(float)eC_FP_ONE) < (float)-32768) 
	{
	printf("%s",eDBG_AddPos("eC_Mul: OVERFLOW (-)\n"));
	return (0x80000000);
	}

  return (eC_Fixed)( (float)(fA) * ((fB)/(float)eC_FP_ONE) );
}

//------------------------------------------------------------------

#define eC_FixDiv(a, b)   (eDBG_MARK, SecureFixDiv(a,b))
#define eC_FixDivQ(a, b, c)  ( eDBG_MARK, SecureFixDiv (((a) << (c)) , ((b) >> (eC_FP_LBIT - (c)))))

inline eC_Fixed SecureFixDiv(eC_Fixed fA, eC_Fixed fB)
{
  if (fB==0)
	{
	printf("%s",eDBG_AddPos("FATAL: DIV by ZERO ERROR!!!, returning ONE\n"));
	return eC_FP_ONE;
	}
  if ((( (float)(fA) / (float)(fB))) > (float)32767)
	{
	printf("%s",eDBG_AddPos("eC_Div: OVERFLOW (+)\n"));
	return 0x7fffffff;
	}
  if ((( (float)(fA) / (float)(fB))) < (float)-32768)
	{
	printf("%s",eDBG_AddPos("eC_Div: OVERFLOW (-)\n"));
	return 0x80000000;
	}
  return ( (eC_Fixed)( (float)(fA) / ((fB)/(float)eC_FP_ONE) ) );
}

//------------------------------------------------------------------

#define eC_FixReciQ(a) (eDBG_MARK, SecureFixDiv(eC_FixFromInt(1),a))

#endif


