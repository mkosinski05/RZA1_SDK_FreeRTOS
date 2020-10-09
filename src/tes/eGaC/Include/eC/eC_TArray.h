/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_TArray.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:00:56 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EC_TARRAY__H_
#define EC_TARRAY__H_

//----------------------------------------------------------------------------------------------------
#include "eC_MemLeakDetection.h"
#include "eC_Types.h"

//----------------------------------------------------------------------------------------------------
/* The class T is intended to be native data (int, float, char *, etc.).  The member function SetQuantity
does memory copies knowing that T does not have side effects from construction or assignment. */

//----------------------------------------------------------------------------------------------------
/**
@memo
Internal class of <B>eGaC</B>.
@doc
Just for your information: this class is internal and so it is not documented.
*/
template <class T>
class eC_TArray
{
  public:
    
    // construction and destruction.
    eC_TArray(eC_UInt uiQuantity = 0, T * atArray = 0);
    
    //
    eC_TArray(eC_UInt uiQuantity, T tInitElement);
    
    //
    ~eC_TArray();
    
    // element access (uiIndex must be in range)
    T * GetArray() const;
    
    //
    inline T& operator[](eC_UInt uiIndex) const;
    
    // reorganization (new array elements are uninitialized)
    void SetQuantity(eC_UInt uiNewQuantity, eC_Bool bCopy);
    
    //
    eC_UInt GetQuantity() const;
    
    //
    void AddElement(const T &rkElement, eC_Bool bCopy = true);
    
    // buffer replacement 
    void AssignBuffer(T * atBuffer, eC_UInt uiQuantity);
    
    //
    inline void AssignBuffer(eC_TArray<T> &rtSource);
    
  protected:
    
    //
    eC_UInt m_uiQuantity;
    
    //
    T * m_atArray;
};

//----------------------------------------------------------------------------------------------------
template <class T>
inline T &eC_TArray<T>::operator[](eC_UInt uiIndex) const
{
  // Assert( m_atArray && uiIndex < m_uiQuantity, "Arrayindex out of bounds" );
  return m_atArray[uiIndex];
}

//----------------------------------------------------------------------------------------------------
template <class T>
inline void eC_TArray<T>::AssignBuffer(eC_TArray<T> &rtSource)
{
  AssignBuffer(rtSource.m_atArray, rtSource.m_uiQuantity);   
}

//----------------------------------------------------------------------------
template <class T>
inline eC_TArray<T>::eC_TArray (eC_UInt uiQuantity, T* atArray)
{
    m_uiQuantity = uiQuantity;

    if ( m_uiQuantity > 0 )
    {
        if ( atArray )
            m_atArray = atArray;
        else
            m_atArray = new T[m_uiQuantity];
    }
    else
    {
        m_atArray = 0;
    }
}

//----------------------------------------------------------------------------
template <class T>
inline eC_TArray<T>::eC_TArray (eC_UInt uiQuantity, T tInitElement)
{
    m_uiQuantity = uiQuantity;

    if ( m_uiQuantity > 0 )
    {
        m_atArray = new T[m_uiQuantity];

				for (eC_UInt uiI = 0; uiI < m_uiQuantity; uiI++)
            m_atArray[uiI] = tInitElement;
    }
    else
    {
        m_atArray = 0;
    }
}

//----------------------------------------------------------------------------
template <class T>
inline eC_TArray<T>::~eC_TArray ()
{
  if(m_atArray)
    delete[] m_atArray;
}

//----------------------------------------------------------------------------
template <class T>
inline T* eC_TArray<T>::GetArray () const
{
    return m_atArray;
}

//----------------------------------------------------------------------------
template <class T>
 void eC_TArray<T>::SetQuantity (eC_UInt uiNewQuantity, eC_Bool bCopy)
{
    if ( uiNewQuantity != m_uiQuantity )
    {
        if ( uiNewQuantity > 0 )
        {
            T* atNewArray = new T[uiNewQuantity];
            if ( bCopy )
            {
                if ( uiNewQuantity > m_uiQuantity )
                {
                    eC_UInt uiCopySize = m_uiQuantity*sizeof(T);
                    eC_Memcpy(atNewArray,m_atArray,uiCopySize);
                }
                else
                {
                    eC_Memcpy(atNewArray,m_atArray,uiNewQuantity*sizeof(T));
                }
            }

						if(m_atArray)
						{
							delete[] m_atArray;
						}
            m_uiQuantity = uiNewQuantity;
            m_atArray = atNewArray;
        }
        else
        {
						if(m_atArray)
						{
	            delete[] m_atArray;
						}
            m_uiQuantity = 0;
            m_atArray = 0;
        }
    }
}

//----------------------------------------------------------------------------
template <class T>
inline eC_UInt eC_TArray<T>::GetQuantity () const
{
    return m_uiQuantity;
}

//----------------------------------------------------------------------------
template <class T>
inline void eC_TArray<T>::AssignBuffer(T* atBuffer, eC_UInt uiQuantity)
{
    if (atBuffer)
    {
        // assign new buffer [WARNING: tarray takes ownership of buffer and will free it]
        if (m_uiQuantity) SetQuantity( 0, false );
        m_atArray = atBuffer;
        m_uiQuantity = uiQuantity;
    }
    else
    {
        // remove buffer without reassign [NOTE: ownership is released, buffer is not freed]
        m_atArray = NULL;
        m_uiQuantity = 0;
        if (uiQuantity>0) SetQuantity( uiQuantity, false );
    }
}

//----------------------------------------------------------------------------
template <class T>
 void eC_TArray<T>::AddElement(const T &rkElement, eC_Bool bCopy)
{
    eC_UInt uiOldQuantity = m_uiQuantity;
    SetQuantity(uiOldQuantity+1, bCopy);
    m_atArray[uiOldQuantity] = rkElement;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------
#endif
