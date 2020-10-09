/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eC
* file name:       eC_TList.h
* %version:        134.0 %
* 
* author:          hh04031
* creation date:   Wed Jan 04 11:10:00 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EC_TLIST__H_
#define EC_TLIST__H_
//----------------------------------------------------------------------------
#include "eC_Memory.h"

//----------------------------------------------------------------------------

// The TList class represents a singly linked list with header node.  The
// class T is intended to be native data (int, float, char*, etc.).  All
// list insertions occur at the front of the list.  If the list values were
// dynamically allocated by the application, it is the application's
// responsibility to delete those values.  This can be done by iterating
// over the list using GetFirst/GetNext and explicitly deleting the values.
//
// To iterate over a list, use
//
//
//	eC_TList<T> kList;
//  T tValue;
//
//	ForAll(klist, tValue)	
//	{
//		// code to process tValue goes here...
//	}
//
// which is expanded by the 'ForAll' macro to :
//
//     TList<T> kList;  // already built list
//     T tValue;
//     bool bFound = kList.GetFirst(tValue);
//     while ( bFound )
//     {
//         // code to process tValue goes here...
//         bFound = kList.GetNext(tValue);
//     }

template <class T>
class eC_TList
{
public:
    // construction and destruction
    eC_TList ();
    ~eC_TList ();

    // element access
    unsigned int GetQuantity () const;
    void Add (T tValue);
    bool AddUnique (T tValue);  // adds only if not already in list
    bool Remove (T tValue);
    bool RemoveFront (T& rtValue);
    void RemoveAll ();

    // linear traversal of list
    bool GetFirst (T& rtValue);
    bool GetNext (T& rtValue);

    // list operations
    void ReverseOrder ();
    bool Contains (T tValue) const;

protected:
    class Node
    {
    public:
        Node (T tValue, Node* pkNext)
        {
            m_tValue = tValue;
            m_pkNext = pkNext;
        }

        ~Node ()
        {
            delete m_pkNext;
        }

        T m_tValue;
        Node* m_pkNext;
    };

    unsigned int m_uiQuantity;
    Node* m_pkFront;

    // iterator for traversal
    Node* m_pkIterator;
};

//----------------------------------------------------------------------------

#define ForAll(list,item) for (bool __bFound__ = (list).GetFirst(item); __bFound__ ; __bFound__ = (list).GetNext(item))

//----------------------------------------------------------------------------
template <class T>
eC_TList<T>::eC_TList ()
{
    m_uiQuantity = 0;
    m_pkFront = 0;
    m_pkIterator = 0;
}
//----------------------------------------------------------------------------
template <class T>
eC_TList<T>::~eC_TList ()
{    
    RemoveAll();
}
//----------------------------------------------------------------------------
template <class T>
unsigned int eC_TList<T>::GetQuantity () const
{
    return m_uiQuantity;
}
//----------------------------------------------------------------------------
template <class T>
void eC_TList<T>::Add (T tValue)
{
    Node* pkNode = new Node(tValue,m_pkFront);

    m_pkFront = pkNode;
    m_uiQuantity++;
}
//----------------------------------------------------------------------------
template <class T>
bool eC_TList<T>::AddUnique (T tValue)
{
    Node* pkSearch = m_pkFront;
    while ( pkSearch )
    {
        if ( pkSearch->m_tValue == tValue )
            return false;
        pkSearch = pkSearch->m_pkNext;
    }

    Add(tValue);
    return true;
}
//----------------------------------------------------------------------------
template <class T>
bool eC_TList<T>::Remove (T tValue)
{
    if ( m_pkFront )
    {
        if ( m_pkFront->m_tValue == tValue )
        {
            // item at beginning of list
            Node* pkSave = m_pkFront;
            m_pkFront = m_pkFront->m_pkNext;
            pkSave->m_pkNext = 0;
            delete pkSave;
            m_uiQuantity--;
            return true;
        }

        // search for item
        Node* pkPrev = m_pkFront;
        Node* pkCurr = m_pkFront->m_pkNext;
        while ( pkCurr && pkCurr->m_tValue != tValue )
        {
            pkPrev = pkCurr;
            pkCurr = pkCurr->m_pkNext;
        }
        if ( pkCurr )
        {
            // found the item
            Node* pkSave = pkCurr;
            pkPrev->m_pkNext = pkCurr->m_pkNext;
            pkSave->m_pkNext = 0;
            delete pkSave;
            m_uiQuantity--;
            return true;
        }
    }

    return false;
}
//----------------------------------------------------------------------------
template <class T>
bool eC_TList<T>::RemoveFront (T& rtValue)
{
    if ( m_pkFront )
    {
        rtValue = m_pkFront->m_tValue;

        Node* pkSave = m_pkFront;
        m_pkFront = m_pkFront->m_pkNext;
        pkSave->m_pkNext = 0;
        delete pkSave;
        m_uiQuantity--;
        return true;
    }
    else
    {
        return false;
    }
}
//----------------------------------------------------------------------------
template <class T>
void eC_TList<T>::RemoveAll ()
{
    Node *pkNode;
    m_pkIterator = m_pkFront;

    while (m_pkIterator)
    {
        pkNode = m_pkIterator->m_pkNext;
        m_pkIterator->m_pkNext = NULL;
        delete m_pkIterator;
        m_pkIterator = pkNode;
    }

    m_pkFront = 0;
    m_uiQuantity = 0;
}
//----------------------------------------------------------------------------
template <class T>
bool eC_TList<T>::GetFirst (T& rtValue)
{
    if ( m_pkFront )
    {
        rtValue = m_pkFront->m_tValue;
        m_pkIterator = m_pkFront->m_pkNext;
        return true;
    }
    else
    {
        m_pkIterator = 0;
        return false;
    }
}
//----------------------------------------------------------------------------
template <class T>
bool eC_TList<T>::GetNext (T& rtValue)
{
    if ( m_pkIterator )
    {
        rtValue = m_pkIterator->m_tValue;
        m_pkIterator = m_pkIterator->m_pkNext;
        return true;
    }
    else
    {
        return false;
    }
}
//----------------------------------------------------------------------------
template <class T>
void eC_TList<T>::ReverseOrder ()
{
    Node* pkReverse = 0;

    while ( m_pkFront )
    {
        Node* pkNode = m_pkFront;
        m_pkFront = m_pkFront->m_pkNext;
        pkNode->m_pkNext = pkReverse;
        pkReverse = pkNode;
    }

    m_pkFront = pkReverse;
}
//----------------------------------------------------------------------------
template <class T>
bool eC_TList<T>::Contains (T tValue) const
{
    Node* pkSearch = m_pkFront;

    while ( pkSearch )
    {
        if ( pkSearch->m_tValue == tValue )
            return true;

        pkSearch = pkSearch->m_pkNext;
    }

    return false;
}
//----------------------------------------------------------------------------

#endif

