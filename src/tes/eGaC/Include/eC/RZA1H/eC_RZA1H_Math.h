/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_nios_Math.h
* %version:        134.0 %
* 
* author:          hh04019
* creation date:   Fri Sep 09 18:46:28 2005
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EC_RZA1H_MATH__H_
#define EC_RZA1H_MATH__H_

//-----------------

#undef eC_FixMul
#undef eC_FixDiv
#undef eC_FixSqrt
#undef eC_FixLen2


inline eC_Fixed eC_FixMul_RZA1H( eC_Fixed a, eC_Fixed b )
{
  long long a64= a;
  return (eC_Fixed)((a64*b)>>16);
}

inline eC_Fixed eC_FixDiv_RZA1H( eC_Fixed a, eC_Fixed b )
{
  long long a64= a;
  return (eC_Fixed)((a64<<16)/b);
}

inline eC_Fixed eC_FixSqrt_RZA1H( eC_Fixed val)
{
  unsigned long temp, g=0, b = 0x8000, bshft = 15;
  do 
  {
    if ((unsigned long)val >= (temp = (((g<<1)+b)<<bshft--))) 
    {
      g += b;
      val -= temp;
    }
  } while (b >>= 1);

  return (eC_Fixed)(g<<8);
}

inline eC_Fixed eC_FixLen2_RZA1H( eC_Fixed aa, eC_Fixed bb )
{
  long long a64= aa;
  long long b64= bb;

  a64*=a64;
  b64*=b64;

  a64+=b64;
  a64>>=16;
 	
  long long temp, g=0, b = 0x800000, bshft = 23;
  do 
  {
    if (a64 >= (temp = (((g<<1)+b)<<bshft--))) 
    {
      g += b;
      a64 -= temp;
    }
  } while (b >>= 1);

  return (eC_Fixed)(g<<8);
}


#define eC_FixMul(a,b)  eC_FixMul_RZA1H( (a), (b) )
#define eC_FixDiv(a,b)  eC_FixDiv_RZA1H( (a), (b) )
#define eC_FixSqrt(a) eC_FixSqrt_RZA1H( (a) )
#define eC_FixLen2(a ,b) eC_FixLen2_RZA1H( (a), (b) )

#endif

