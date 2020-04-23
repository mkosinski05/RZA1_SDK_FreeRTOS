/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_Time64.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:01:01 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EGML_TIME64__H_
#define EGML_TIME64__H_

//---------------------------------------------------------------

#include "eC_Types.h"

//---------------------------------------------------------------	
// eC_Time64 is a 64Bit unsigned integer base class
// Only used for time profiling

class eC_Time64
{

  public:

    // default constructor
    inline eC_Time64()                                { }
    // copy constructor
    inline eC_Time64(const eC_Time64& rkTime)         { Set( rkTime.m_uiHigh, rkTime.m_uiLow ); }
    // 32bit constructor
    inline eC_Time64(eC_UInt uiLow)                   { Set( 0, uiLow ); }
    // 64bit constructor
    inline eC_Time64(eC_UInt uiHigh, eC_UInt uiLow)   { Set( uiHigh, uiLow ); }

    // comparison
    inline bool operator== (const eC_Time64& rkTime) const;
    inline bool operator!= (const eC_Time64& rkTime) const;

    // arithmetics
    inline eC_Time64 operator- (const eC_Time64& rkTime) const;
    inline eC_Time64 operator+ (const eC_Time64& rkTime) const;

    // access
    inline void Set(eC_UInt uiHigh, eC_UInt uiLow);

    // conversion
    inline double toDouble() const;

    // data
    eC_UInt m_uiHigh;
    eC_UInt m_uiLow;
};

//----------------------------------------------------------------------------
inline void eC_Time64::Set(eC_UInt uiHigh, eC_UInt uiLow)
{
    m_uiHigh = uiHigh;
    m_uiLow  = uiLow;  
}

//----------------------------------------------------------------------------
inline double eC_Time64::toDouble() const
{
    double ret;

    ret= (double)m_uiHigh;
//    ret= ret * (double)(65536.0) * (double)(65536.0)+ m_uiLow;

    ret= ret * ((double)(0xffffffff) + 1) + m_uiLow;
//ret = (double)((long long)m_uiHigh << 32) + m_uiLow
    return ret;
}

//----------------------------------------------------------------------------
inline bool eC_Time64::operator== (const eC_Time64& rkTime) const
{
    return (m_uiHigh == rkTime.m_uiHigh) && (m_uiLow == rkTime.m_uiLow);
}

//----------------------------------------------------------------------------
inline bool eC_Time64::operator!= (const eC_Time64& rkTime) const
{
    return (m_uiHigh != rkTime.m_uiHigh) || (m_uiLow != rkTime.m_uiLow);
}

//----------------------------------------------------------------------------
inline eC_Time64 eC_Time64::operator- (const eC_Time64& rkTime) const
{
  eC_UInt uiCarry=0;
  eC_Time64 kRet;

  kRet.m_uiLow  = m_uiLow  - rkTime.m_uiLow;

  if (kRet.m_uiLow>m_uiLow)
    uiCarry = 1;

  kRet.m_uiHigh = m_uiHigh - rkTime.m_uiHigh - uiCarry;

  return kRet;
}

//----------------------------------------------------------------------------
inline eC_Time64 eC_Time64::operator+ (const eC_Time64& rkTime) const
{
  eC_UInt uiCarry=0;
  eC_Time64 kRet;

  kRet.m_uiLow  = m_uiLow  + rkTime.m_uiLow;

  if (kRet.m_uiLow<m_uiLow)
    uiCarry = 1;

  kRet.m_uiHigh = m_uiHigh + rkTime.m_uiHigh + uiCarry;

  return kRet;
}




//---------------------------------------------------------------
#endif

