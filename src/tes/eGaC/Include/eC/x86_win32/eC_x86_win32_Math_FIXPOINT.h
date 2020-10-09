/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_x86_win32_Math_FIXPOINT.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:08:10 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EC_NIOS_MATH__H_
#define EC_NIOS_MATH__H_

//-----------------

#undef eC_FixMul
#undef eC_FixDiv
#undef eC_FixSqrt
#undef eC_FixLen2



inline eC_Fixed eC_FixMul_MSVC( eC_Fixed a, eC_Fixed b )
{
  __int64 a64= a;
  return (eC_Fixed)((a64*b)>>16);
}

inline eC_Fixed eC_FixDiv_MSVC( eC_Fixed a, eC_Fixed b )
{
  __int64 a64= a;
  return (eC_Fixed)((a64<<16)/b);
}

inline eC_Fixed eC_FixSqrt_MSVC( eC_Fixed val)
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

inline eC_Fixed eC_FixLen2_MSVC( eC_Fixed aa, eC_Fixed bb )
{
  __int64 a64= aa;
  __int64 b64= bb;

  a64*=a64;
  b64*=b64;

  a64+=b64;
  a64>>=16;
 	
  __int64 temp, g=0, b = 0x800000, bshft = 23;
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


#define eC_FixMul(a,b)   eC_FixMul_MSVC( (a), (b) )
#define eC_FixDiv(a,b)   eC_FixDiv_MSVC( (a), (b) )
#define eC_FixSqrt(a)    eC_FixSqrt_MSVC( (a) )
#define eC_FixLen2(a ,b) eC_FixLen2_MSVC( (a), (b) )


#endif

