/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef EC_TL__H_
#define EC_TL__H_

//Iterator implementation
#include "eC_TList_Iterators.h"
#include <cassert>

/**
    Node template for doubly linked list with pointer to previous, next node
    and template value.
    @brief ListNode template class
*/
template <class T>
class ListNode
{
public:
    ListNode (): m_tValue()
    {
        m_pkNext = NULL;
        m_pkPrevious = NULL;
    }

    //-------------------------------------
    ~ListNode () {}

    //-------------------------------------

    ///pointer to next node
    ListNode* m_pkNext;

    ///pointer to previous node
    ListNode* m_pkPrevious;

    ///value of this node
    T m_tValue;
};


//----------------------------------------------------------------------------

/// Represents a double linked list template with header and tail node.

/**
    The list is not cyclic, i.e. the next element of the last element is
    NULL and the previous of the front element is NULL.

    If the list values were dynamically allocated by the application, it is the application's
    responsibility to delete those values.  This can be done by iterating
    over the list using the eC_TIterator class and explicitly deleting the values.
    Use eC_TSafeIterator to delete elements while iterating through the list.
    @see eC_TSafeIterator
*/
template <class T>
class eC_TListDoubleLinked
{
    typedef ListNode<T> Node;

    friend class eC_TSafeIterator<T>;

public:

    /// Iterator is typedef'd as eC_TIterator ot type T
    typedef eC_TIterator<T> Iterator;
    
    /// SafeIterator is typedef'd as eC_TSafeIterator ot type T
    typedef eC_TSafeIterator<T> SafeIterator;

    /** Constructor */
    eC_TListDoubleLinked ();

    /**
        Copy constructor.
        @param copy Where to copy from.
    */
    eC_TListDoubleLinked(const eC_TListDoubleLinked<T>& copy);

    /** Destructor */
    ~eC_TListDoubleLinked ();

    /**
        Returns number of elements in list.
        @return eC_Int Number of elements stored in list
    */
    eC_UInt GetQuantity () const;

    /**
        Test if the list contains no elements.
        @return eC_Bool True if the list contains no elements
    */
    eC_Bool IsEmpty() const;

    /**
        Adds element at front of list
        @param tValue The value of the node which will be added.
    */
    void Add ( const T &tValue );

    /**
        Adds element at end of list
        @param tValue The value of the node which will be added.
    */
    void AddAtEnd ( const T &tValue );

    /**
        Adds element at front of the Node.
        @param tValue2Add The value of the node which will be added.
        @param kIter Point of the node in the list.
    */
    void AddBefore ( const T &tValue2Add, Iterator & kIter );

    /**
        Adds element after the Node.
        @param tValue2Add The value of the node which will be added.
        @param kIter Point of the node in the list.
    */
    void AddAfter ( const T &tValue2Add, Iterator & kIter );

    /**
        Adds element at front of list, if list does not already contain this value.
        This function uses operator==.
        @param tValue The value of the node which will be added.
        @return returns True if element was added, False if it was not.
    */
    eC_Bool AddUnique ( const T &tValue );  // adds only if not already in list

    /**
        Adds element at end of list, if list does not already contain this value.
        This function uses operator==.
        @param tValue The value of the node which will be added.
        @return True if element was added, False if it was not.
    */
    eC_Bool AddUniqueAtEnd ( const T &tValue );  // adds only if not already in list

    /**
        Adds element sorted into list.
        Only works useful if existing elements in list are sorted.
        This function uses operator== and operator>.
        @param tValue The value of the node which will be added.
    */
    void AddSorted ( const T &tValue );

    /**
        Adds element sorted into list, if value does not alredy exist in list.
        Only works useful if existing elements in list are sorted.
        This function uses operator== and operator>.
        @param tValue The value of the node which will be added.
        @return returns True if element was added, False if it was not.
    */
    eC_Bool AddUniqueSorted ( const T &tValue );

    /**
        Removes element which is pointed by SafeIterator.
        @see eC_TSafeIterator.
        afer removing element iterator is invalid. Anyhow its possible to iterate trough list afer deleting.
        (Iterator is valid again, after ++,--,operation, or of course if you set it at front or end of list.
        @param  kIter A SafeIterator pointing to the element to be removed.
        @return True if element was removed, False if iterator was invalid (0).
    */
    eC_Bool Remove ( SafeIterator & kIter );

    /**
        Removes the first element from list, which is identified by value of parameter 1.
        This function uses or if not set operator==.
        @param tValue list element which should be removed
        @return True if element was removed, False if element was not found.
        @see void RemoveAll (T tValue)
    */
    eC_Bool Remove ( const T &tValue );

    /**
        Removes first element of list.
        @param rtValue to pop removed element into
        @return True if element was removed, False if list was empty
    */
    eC_Bool RemoveFront( T& rtValue );

    /**
        Removes last element of list.
        @param rtValue to pop removed element into
        @return True if element was removed, False if list was empty
    */
    eC_Bool RemoveEnd( T& rtValue );

    /**
        Removes all elements of list.
    */
    void RemoveAll ();

    /**
        Removes all elements of list wich have the value of parameter1.
        @param tValue to be removed
    */
    void RemoveAll ( const T &tValue );

    /**
        Swappes two elements pointed by iterators or safe iterators.
        @param kIter1, kIter2 iterators to be swapped
        @return True if elements where swapped, False if one iterator was invalid
    */
    eC_Bool Swap ( Iterator & kIter1, Iterator & kIter2 );

    /**
        Reverses the order of the list. Example:  1-2-3  --> 3-2-1
    */
    void ReverseOrder ();

    /**
        Checks if list does contain value of parameter 1
        This function uses operator==.
        @see eC_TListDoubleLinked()
        @param tValue to be tested for
        @return True if list does contain value, False if not
    */
    eC_Bool Contains ( const T &tValue ) const;

    /**
        Checks if list is empty
        @return True if list is empty, False if not
    */
    eC_Bool Empty() const;

    /**
        Sorts list in ascending order. It uses the Quicksort algorithm
        if the boolean variable is true, otherwise the Mergesort algorithm
        is utilized.
        Note that Quicksort is an unstable algorithm. On the other hand,
        Mergesort is a stable algorithm, thus preserving entrance
        order if the values are equal.
        @param bIsQuicksort Whether to use Quicksort
        This function uses operator== and operator>.
    */
    void Sort(eC_Bool bIsQuicksort = true);

    /**
        With GetFirst and GetNext you can Iterate throug list without using an iterator.
        GetFirst sets internal pointer to the beginning of the list and returns reference this element.
        @param rtValue to pop found element into
        @return True if list is not empty, False if  list is empty
    */
    eC_Bool GetFirst( T& rtValue ) const;

    /**
        With GetFirst and GetNext you can iterate through a list without using an iterator.
        GetNext sets internal pointer to the next list element and returns reference to this element.
        @param rtValue to pop found element into
        @return True if iterator is not at the end of list, False if pointing behind last element
    */
    eC_Bool GetNext( T& rtValue ) const;

    /**
        Returns an iterator that points to the first element in the list.
        Example:
@code
myIterator = myList.GetBegin();
@endcode
        @return Interator pointing to the first element
    */
    eC_TIterator<T> GetBegin() const
    {
        eC_TIterator<T> kIter( m_pkFront );
        return kIter;
    }

    /**
        Returns a safe iterator that points to the first element in the list.
        @return SafeInterator pointing to the first element
    */
    eC_TSafeIterator<T> GetBeginSafe()
    {
        eC_TSafeIterator<T> kIter;
        kIter.m_pNode = m_pkFront;
        if (m_pkFront != NULL)
        {
            kIter.m_pSaveNextNode = m_pkFront->m_pkNext;
            kIter.m_pSavePrevNode = m_pkFront->m_pkPrevious;
        }
        kIter.SetList( this);
        return kIter;
    }

    /**
        Returns an iterator that points to the last element in the list.
        Example:
@code
myIterator = myList.GetEnd();
@endcode
        @return Interator pointing to the last element
    */
    eC_TIterator<T> GetEnd() const
    {
        eC_TIterator<T> kIter( m_pkLast );
        return kIter;
    }

    /**
        Returns a safe iterator that points to the last element in the list.
        @return SafeInterator pointing to the last element
    */
    eC_TSafeIterator<T> GetEndSafe()
    {
        eC_TSafeIterator<T> kIter;
        kIter.m_pNode = m_pkLast;
        if (m_pkLast != NULL)
        {
            kIter.m_pSaveNextNode = m_pkLast->m_pkNext;
            kIter.m_pSavePrevNode = m_pkLast->m_pkPrevious;
        }
        kIter.SetList( this);
        return kIter;
    }

    /**
        Returns an iterator that points to a specific value in the list.
        The list is searched for the first occurrence of the value. An iterator
        pointing to that list node is returned.
        @param tValue Value to be found in the list.
        Example:
@code
myIterator=myList.GetFind(Value);
@endcode
        @return An iterator pointing to the first occurrence of tValue in the
                list.
        @see eC_TSafeIterator, eC_TIterator, GetFindSafe()
    */
    eC_TIterator<T> GetFind( const T &tValue ) const
    {
        eC_TIterator<T> kIter;
        Node* pkSearch = m_pkFront;

        while ( pkSearch )
        {
            if ( pkSearch->m_tValue == tValue )
            {
                kIter.m_pNode = pkSearch;
                return kIter;
            }

            pkSearch = pkSearch->m_pkNext;
        }

        kIter.m_pNode = NULL;
        return kIter;
    }

    /**
        Returns a safe iterator that points to a specific value in the list.
        The list is searched for the first occurrence of the value. A safe
        iterator pointing to that list node is returned.
        @param tValue Value to be found in the list.
        example: myIterator=myList.GetFindSafe(Value);
        @return A safe iterator pointing to the first occurrence of tValue in
                the list.
        @see eC_TSafeIterator, eC_TIterator, GetFind()
    */
    eC_TSafeIterator<T> GetFindSafe( const T &tValue )
    {
        eC_TSafeIterator<T> kIter;
        Node* pkSearch = m_pkFront;

        while ( pkSearch )
        {
            if ( pkSearch->m_tValue == tValue )
            {
                kIter.m_pNode = pkSearch;
                kIter.m_pSaveNextNode = pkSearch->m_pkNext;
                kIter.m_pSavePrevNode = pkSearch->m_pkPrevious;
                kIter.SetList( this);
                return kIter;
            }

            pkSearch = pkSearch->m_pkNext;
        }

        kIter.m_pNode = NULL;
        kIter.SetList( this);
        return kIter;
    }

    /**
        Returns an iterator pointing to the element at the specified index.
        @param uiPos The position index in the list.
        @return Iterator pointing to the uiPos-th element. If uiPos is greater
                than the length of the list, the returned iterator is invalid.
      */
    eC_TIterator<T> GetAt(eC_UInt uiPos) const
    {
        Node* pkRet = m_pkFront;

        while (pkRet != NULL && uiPos > 0)
        {
            pkRet = pkRet->m_pkNext;
            --uiPos;
        }

        return eC_TIterator<T>(pkRet);
    }

    /**
        Returns a safe iterator pointing to the element at the specified index.
        @param uiPos The position index in the list.
        @return Safe iterator pointing to the uiPos-th element. If uiPos is
                greater than the length of the list, the returned iterator is
                invalid.
      */
    eC_TSafeIterator<T> GetAtSafe(eC_UInt uiPos)
    {
        eC_TSafeIterator<T> kIter;
        if ((m_pkFront != NULL) && (uiPos < GetQuantity()))
        {
            Node* pkRet = m_pkFront;

            while (pkRet != NULL && uiPos > 0)
            {
                pkRet = pkRet->m_pkNext;
                --uiPos;
            }

            kIter.m_pNode = pkRet;
            if (pkRet != NULL)
            {
                kIter.m_pSaveNextNode = pkRet->m_pkNext;
                kIter.m_pSavePrevNode = pkRet->m_pkPrevious;
            }
        }
        kIter.SetList( this);
        return kIter;
    }

    /**
        Copies the contents of another list to this one.
        After this operation, this->GetQuantity()==copy.GetQuantity().
        @param copy The list to copy from.
        @return Reference to this list.
      */
    eC_TListDoubleLinked<T>& operator=(const eC_TListDoubleLinked<T>& copy)
    {
        // Avoid copying ourselves
        if (&copy==this) return *this;

        //Delete old stuff;
        RemoveAll();

        Iterator iter = copy.GetBegin();

        while (iter.IsInsideList())
        {
            AddAtEnd(*iter);
            ++iter;
        }

        return *this;
    }

    //-------------------------------------

private:
    Node* m_pkFront;
    Node* m_pkLast;

    mutable Node* m_pkIterator;

    eC_UInt m_uiQuantity;

    eC_TSafeIterator<T>** m_ppIterRegister;  //pointer to array where safe iterators are registerd
    eC_UInt m_uiIterRegisterCnt;
    eC_UInt m_uiIterRegisterSize;

    eC_Bool Remove ( Node* tNode );

    void SortArraysUsingQuickSort( Node** ppkArray, eC_Int iFirst , eC_Int iLast );

    void SortArraysUsingMergeSort( Node** ppkMerged, eC_UInt uiQuantity);

    void RegisterIterator( eC_TSafeIterator<T>* );
    void UnregisterIterator( eC_TSafeIterator<T>* );
    void UpdateIterators( Node*, Update_t ) const;
};

//----------------------------------------------------------------------------

// "(List)" in backets is required when passing a dereferenced pointer to
// the list to the macro, e.g.:
//   CDevice::eC_TListDoubleLinked<CSomething*>* pSomethingList;
//   FOR_ALL_FORWARD(SomethingIter, *pSomethingList)
#define FOR_ALL_FORWARD(Iterator,List) for ((Iterator) = (List).GetBegin(); \
          (Iterator).IsInsideList() ; ++(Iterator))

#define FOR_ALL_BACKWARD(Iterator,List) for ((Iterator) = (List).GetEnd(); \
          (Iterator).IsInsideList() ; --(Iterator))

#define FOR_ALL_FORWARD_SAFE(Iterator,List) for ((Iterator) = (List).GetBeginSafe(); \
          (Iterator).IsValid() || (Iterator).IsNextValid() ; ++(Iterator))

#define FOR_ALL_BACKWARD_SAFE(Iterator,List) for ((Iterator) = (List).GetEndSafe(); \
          (Iterator).IsValid() || (Iterator).IsPreviousValid(); --(Iterator))

//----------------------------------------------------------------------------

template <class T>
eC_TListDoubleLinked<T>::eC_TListDoubleLinked ()
:
    m_pkFront(NULL),
    m_pkLast(NULL),
    m_pkIterator(NULL),
    m_uiQuantity(0),
    m_ppIterRegister(NULL),
    m_uiIterRegisterCnt(0),
    m_uiIterRegisterSize(0)
{
}

template<class T>
eC_TListDoubleLinked<T>::eC_TListDoubleLinked(const eC_TListDoubleLinked<T>& copy)
:
    m_pkFront(NULL),
    m_pkLast(NULL),
    m_pkIterator(NULL),
    m_uiQuantity(0),
    m_ppIterRegister(NULL),
    m_uiIterRegisterCnt(0),
    m_uiIterRegisterSize(0)
{
    operator=(copy);
}

//----------------------------------------------------------------------------
template <class T>
eC_TListDoubleLinked<T>::~eC_TListDoubleLinked ()
{
    RemoveAll();
    // Unregister all safe iterators.
    for ( eC_UInt i = 0; i < m_uiIterRegisterCnt; i++ )
    { 
        ( m_ppIterRegister[i] )->SetList(NULL);
    }

    delete[] m_ppIterRegister;
}

//----------------------------------------------------------------------------
template <class T>
eC_UInt eC_TListDoubleLinked<T>::GetQuantity () const
{
    return m_uiQuantity;
}

//----------------------------------------------------------------------------
template <class T>
eC_Bool eC_TListDoubleLinked<T>::IsEmpty() const
{
    return m_uiQuantity == 0;
}

//----------------------------------------------------------------------------
template <class T>
eC_Bool eC_TListDoubleLinked<T>::GetFirst(T& rtValue) const
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
eC_Bool eC_TListDoubleLinked<T>::GetNext(T& rtValue) const
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
//adds an element tValue at front of list
template <class T>
void eC_TListDoubleLinked<T>::Add ( const T &tValue )
{
    Node * pkNewNode = new Node();
    pkNewNode->m_tValue = tValue;

    pkNewNode->m_pkNext = m_pkFront; // next of new element is formerly front element
    // (front element could be NULL)

    if ( m_pkFront )  //list is not empty
    {
        m_pkFront->m_pkPrevious = pkNewNode; // previous of formerly front is new node

        // last element will remain the same
    } else // list was empty, so new element is first element and last element
    {
        m_pkLast = pkNewNode;
    }

    // new first element already has no previous when it was constructed

    m_pkFront = pkNewNode; // new front is new node

    m_uiQuantity++;

    // Update SafeIterators which are registered, which are pointing at formerly
    // first element (they need to know new element)
    UpdateIterators( pkNewNode, ADDED );
}

//----------------------------------------------------------------------------
template <class T>
void eC_TListDoubleLinked<T>::AddAtEnd ( const T &tValue )
{
    Node * pkNode = new Node();
    pkNode->m_tValue = tValue;
    pkNode->m_pkPrevious = m_pkLast;

    if ( m_pkLast ) //list is not empty
    {
        m_pkLast->m_pkNext = pkNode; //next of last node is new node
    } else
    { //list was empty, so new node is First and Last
        m_pkFront = pkNode;
    }

    pkNode->m_pkNext = 0; //last element has no next

    m_pkLast = pkNode;
    m_uiQuantity++;

    // Update SafeIterators, if pointing at last Element (needs to know new Element)
    UpdateIterators( pkNode, ADDED );
}

//----------------------------------------------------------------------------
template <class T>
void eC_TListDoubleLinked<T>::AddBefore ( const T &tValue2Add, Iterator & kIter )
{
    Node * pNodeInList = kIter.GetNode();
    if( NULL==pNodeInList) // If kIter is illegal call Add()
        return Add(tValue2Add);
    Node* pkNode = new Node();
    pkNode->m_tValue = tValue2Add;
    Node* pkNodePre = pNodeInList->m_pkPrevious;
    pkNode->m_pkNext = pNodeInList;

    if ( pkNodePre )
    {
        pkNode->m_pkPrevious = pkNodePre;
        pkNodePre->m_pkNext = pkNode;
    }
    else
    {
        m_pkFront = pkNode;
        pkNode->m_pkPrevious = 0;
    }

    pNodeInList->m_pkPrevious = pkNode;
    m_uiQuantity++;

    // Update SafeIterators, if pointing at last Element (needs to know new element)
    UpdateIterators( pkNode, ADDED );
}

//----------------------------------------------------------------------------
template <class T>
void eC_TListDoubleLinked<T>::AddAfter ( const T &tValue2Add, Iterator & kIter )
{
    Node * pNodeInList = kIter.GetNode();
    if( NULL==pNodeInList) // If kIter is illegal call call AddAtEnd()
        return AddAtEnd(tValue2Add);
    Node* pkNode = new Node();
    pkNode->m_tValue = tValue2Add;
    Node* pkNodeNext = pNodeInList->m_pkNext;
    pkNode->m_pkPrevious = pNodeInList;

    if ( pkNodeNext )
    {
        pkNodeNext->m_pkPrevious = pkNode;
        pkNode->m_pkNext = pkNodeNext;
    }
    else
    {
        m_pkLast = pkNode;
        pkNode->m_pkNext = 0;
    }

    pNodeInList->m_pkNext = pkNode;
    m_uiQuantity++;
    // Update SafeIterators, if pointing at last Element (needs to know new element)
    UpdateIterators( pkNode, ADDED );
}

//----------------------------------------------------------------------------
template <class T>
eC_Bool eC_TListDoubleLinked<T>::AddUnique ( const T &tValue )
{
    if ( Contains( tValue ) )
    {
        return false;
    }

    Add( tValue );
    return true;
}

//----------------------------------------------------------------------------
template <class T>
eC_Bool eC_TListDoubleLinked<T>::AddUniqueAtEnd ( const T &tValue )
{
    if ( Contains( tValue ) )
    {
        return false;
    }

    AddAtEnd( tValue );
    return true;
}

//----------------------------------------------------------------------------
template <class T>
eC_Bool eC_TListDoubleLinked<T>::AddUniqueSorted ( const T &tValue )
{
    if ( Contains( tValue ) )
    {
        return false;
    }

    AddSorted( tValue );
    return true;
}

//----------------------------------------------------------------------------
template <class T>
void eC_TListDoubleLinked<T>::AddSorted ( const T &tValue )
{
    Node * pkSortIter = 0;

    if ( ( pkSortIter = m_pkLast ) != 0 )    //not Empty, set Iterater at the end
    {

        if ( tValue > pkSortIter->m_tValue )  //if tValue > Value of last element
        {
            AddAtEnd( tValue );  //Add At End
            return ;
        }

        //if not added at end
        pkSortIter = m_pkFront;

        if ( !( tValue > pkSortIter->m_tValue ) )  //if tValue <= Value of first element
        {
            Add( tValue );  //Add at Begin
            return ;
        }

        //not added at front and not added at end
        //search for right position
        while ( tValue > pkSortIter->m_tValue )  //Iterate trough list
        {
            pkSortIter = pkSortIter->m_pkNext;
        }

        Node* pkNextNode = pkSortIter;     //node behind new Node
        Node* pkPrevNode = pkNextNode->m_pkPrevious;  //node in front of new Node

        Node* pkNewNode = new Node(); //new node
        pkNewNode->m_pkNext = pkNextNode;
        pkNewNode->m_tValue = tValue;

        if ( pkPrevNode )
        {
            pkPrevNode->m_pkNext = pkNewNode;
        }

        pkNextNode->m_pkPrevious = pkNewNode;
        pkNewNode->m_pkPrevious = pkPrevNode;
        m_uiQuantity++;

        // Update SafeIterators, if pointing at Previous or next Element  (needs to know new Element)
        UpdateIterators( pkNewNode, ADDED );
    } else // list is empty
    {
        Add( tValue ); // add at front
    }

}

//----------------------------------------------------------------------------
template <class T>
eC_Bool eC_TListDoubleLinked<T>::Remove( SafeIterator &kIter )
{
    eC_Bool bResult = Remove( kIter.GetNode() );
    kIter.m_pNode = NULL; //after removing iterator it is invalid
    return bResult;
}

//----------------------------------------------------------------------------
template <class T>
eC_Bool eC_TListDoubleLinked<T>::Remove( const T &tValue )
{
    return Remove( ( GetFind( tValue ) ).m_pNode );  //finds node with specified value and passes it to Remove()
}

//----------------------------------------------------------------------------
template <class T>
eC_Bool eC_TListDoubleLinked<T>::Remove( Node* ptNode )
{
    if ( ptNode && m_pkFront )       // list is not empty and node is not 0
    {
        Node * pkSave = NULL;

        if ( m_pkFront == ptNode )         // special case: remove first element
        {
            pkSave = m_pkFront;                     // save the node that we want to delete
            m_pkFront = m_pkFront->m_pkNext;        // update front

            if ( m_pkFront != NULL )                  // there is another element in list
            {
                m_pkFront->m_pkPrevious = NULL;     // new front has no previous element
            } else         // last remaining element of the list is getting deleted
            {
                m_pkLast = NULL;
            }

        }
        else if ( m_pkLast == ptNode )           // special case: removing last element in list
            // the list can still have several elements afterwards
        {
            pkSave = m_pkLast;                      // save formerly last element
            m_pkLast = m_pkLast->m_pkPrevious;      // new last element is the element before the formerly last

            if ( m_pkLast != NULL )                    // there is another element in list
            {
                m_pkLast->m_pkNext = NULL;      // next of new last element is NULL
            } else                                    // last remaining element of the list is getting deleted
            {
                m_pkFront = NULL;
            }

        }
        else
        {
            // case: not first and not last element
            pkSave = ptNode;

            ptNode->m_pkPrevious->m_pkNext = ptNode->m_pkNext;
            ptNode->m_pkNext->m_pkPrevious = ptNode->m_pkPrevious;
        }

        m_uiQuantity--;
        UpdateIterators( pkSave, REMOVED );

        delete pkSave;                          // deleting node after removing it from data structure of list
        pkSave = NULL;

        return true;
    }

    return false; // nothing removed
}

//----------------------------------------------------------------------------
template <class T>
eC_Bool eC_TListDoubleLinked<T>::RemoveFront( T& rtValue )
{
    if ( m_pkFront )
    {
        rtValue = m_pkFront->m_tValue; // output parameter, only valid if function returns true

        Node* pkSave = m_pkFront;
        m_pkFront = m_pkFront->m_pkNext;

        if ( m_pkFront )
        {
            m_pkFront->m_pkPrevious = NULL;
        }
        else
        {
            // front was last element
            m_pkLast = NULL;
        }

        m_uiQuantity--;
        UpdateIterators( pkSave, REMOVED );

        delete pkSave;
        pkSave = NULL;

        return true;
    }

    return false;
}

//----------------------------------------------------------------------------
template <class T>
eC_Bool eC_TListDoubleLinked<T>::RemoveEnd( T& rtValue )
{
    if ( m_pkLast )
    {
        rtValue = m_pkLast->m_tValue; // output parameter, only valid if function returns true

        Node* pkSave = m_pkLast;

        if ( m_pkLast == m_pkFront )    // First and only element
        {
            m_pkLast = NULL;
            m_pkFront = NULL;
        } else
        {   // Last element is not first element
            m_pkLast = m_pkLast->m_pkPrevious;
            m_pkLast->m_pkNext = NULL;
        }

        m_uiQuantity--;
        UpdateIterators( pkSave, REMOVED );

        delete pkSave;
        pkSave = NULL;

        return true;
    }

    return false;
}

//----------------------------------------------------------------------------
template <class T>
void eC_TListDoubleLinked<T>::RemoveAll()
{
    Node * pkIterator = m_pkFront;

    while ( pkIterator )  // while not at end of list
    {
        Node * pkNode = pkIterator->m_pkNext; // save pointer to next
        delete pkIterator; // delete current
        pkIterator = pkNode; // next iteration working on current next
    }

    m_pkFront = NULL;
    m_pkLast = NULL;
    m_uiQuantity = 0;
    UpdateIterators( pkIterator, KILL );
}

//---------------------------------------------------------------------------
template <class T>
void eC_TListDoubleLinked<T>::RemoveAll( const T &tValue )
{
    Node * pkIteratorNode = m_pkFront;

    while ( pkIteratorNode )
    {
        Node * pkNextNode = pkIteratorNode->m_pkNext;

        if ( tValue == pkIteratorNode->m_tValue )
        {
            Remove( pkIteratorNode );
        }

        pkIteratorNode = pkNextNode;
    }
}

//----------------------------------------------------------------------------
template <class T>
void eC_TListDoubleLinked<T>::ReverseOrder()
{
    Node * pkReverse = NULL;

    m_pkLast = m_pkFront;

    while ( m_pkFront )
    {
        Node * pkNode = m_pkFront;
        m_pkFront = m_pkFront->m_pkNext;
        pkNode->m_pkNext = pkReverse;

        if ( pkReverse )
        {
            pkReverse->m_pkPrevious = pkNode;
        }

        pkReverse = pkNode;
    }

    m_pkFront = pkReverse;

    if ( m_pkFront )
    {
        m_pkFront->m_pkPrevious = NULL;
    }

    UpdateIterators( 0, REVERSE_ORDER );
}

//----------------------------------------------------------------------------
template <class T>
eC_Bool eC_TListDoubleLinked<T>::Contains ( const T &tValue ) const
{
    Node * pkSearch = m_pkFront;

    while ( pkSearch )
    {
        if ( pkSearch->m_tValue == tValue )
        {
            return true;
        }

        pkSearch = pkSearch->m_pkNext;
    }

    return false;
}

//----------------------------------------------------------------------------
template <class T>
eC_Bool eC_TListDoubleLinked<T>::Empty () const
{
    return !m_pkFront;
}

//----------------------------------------------------------------------------
template <class T>
eC_Bool eC_TListDoubleLinked<T>::Swap ( Iterator &kIter1, Iterator &kIter2 )
{
    if ( !( kIter1.GetNode() && kIter2.GetNode() ) )
    {
        return false;
    }

    T tTempValue = kIter1.GetNode() ->m_tValue;
    kIter1.GetNode() ->m_tValue = kIter2.GetNode() ->m_tValue;
    kIter2.GetNode() ->m_tValue = tTempValue;

    return true;
}

//----------------------------------------------------------------------------
template <class T>
void eC_TListDoubleLinked<T>::SortArraysUsingQuickSort( Node** ppSortPointers, eC_Int iFirst, eC_Int iLast )
{
    //quicksort implementation

    eC_Int iIter1 = iFirst, iIter2 = iLast;
    T tPivo = ppSortPointers[ ( iFirst + iLast ) / 2 ] ->m_tValue;

    while ( iIter1 <= iIter2 )
    {
        while ( !( ( ppSortPointers[ iIter1 ] ->m_tValue ) > tPivo ) &&
                !( ( ppSortPointers[ iIter1 ] ->m_tValue ) == tPivo ) )  // m_tValue< tPivo
            iIter1++;

        while ( ( ppSortPointers[ iIter2 ] ->m_tValue ) > tPivo )
            iIter2--;

        if ( iIter1 <= iIter2 )  //swap elements
        {
            Node * pkTempNode = ppSortPointers[ iIter1 ];
            ppSortPointers[ iIter1 ] = ppSortPointers[ iIter2 ];
            ppSortPointers[ iIter2 ] = pkTempNode;

            iIter1++;
            iIter2--;
        }
    }

    if ( iFirst < iIter2 )  //more than 1 element in "upper" part of Array
    {
        SortArraysUsingQuickSort( ppSortPointers, iFirst, iIter2 );
    }
    if ( iIter1 < iLast )  //more than 1 element in "lower" part of Array
    {
        SortArraysUsingQuickSort( ppSortPointers, iIter1, iLast );
    }
}

//----------------------------------------------------------------------------

template <class T>
void eC_TListDoubleLinked<T>::SortArraysUsingMergeSort(Node** ppkMerged, eC_UInt uiQuantity)
{

    // If there's only one element, there's nothing to sort
    if (uiQuantity > 1)
    {

        // Mergesort implementation
        eC_UInt uiMiddle= uiQuantity/2;
        eC_UInt uiRightQuantity= uiQuantity - uiMiddle;
        eC_UInt uiLeftQuantity= uiMiddle;

        Node** ppkLeft=   new Node* [uiLeftQuantity];
        Node** ppkRight=  new Node* [uiRightQuantity];


        for (eC_UInt i= 0; i < uiLeftQuantity; ++i)
            ppkLeft[i]= NULL;
        for (eC_UInt i= 0; i < uiRightQuantity; ++i)
            ppkRight[i]= NULL;

        eC_UInt uiRight, uiLeft;
        for (uiLeft= 0; uiLeft < uiMiddle; ++uiLeft)
            ppkLeft[uiLeft]= ppkMerged[uiLeft];

        uiRight= 0;
        for (eC_UInt ui= uiMiddle; ui < uiQuantity; ++ui)
        {
            ppkRight[uiRight]= ppkMerged[ui];
            uiRight++;
        }

        // Recursive call with first half of the list and number of elements of it
        SortArraysUsingMergeSort(ppkLeft, uiLeft);

        // Recursive call with second half of the list and number of elements of it
        SortArraysUsingMergeSort(ppkRight, uiRight);

        // Merging part
        eC_UInt ui= 0, uj= 0, uk= 0;
        while (ui < uiQuantity)
        {
            if ((uiLeftQuantity != uj) && (uiRightQuantity != uk))
            {
                if ((ppkRight[uk]->m_tValue == ppkLeft[uj]->m_tValue) || (ppkRight[uk]->m_tValue > ppkLeft[uj]->m_tValue))
                {
                    ppkMerged[ui]= ppkLeft[uj];
                    uj++;
                }
                else
                {
                    ppkMerged[ui]= ppkRight[uk];
                    uk++;
                }
            }
            else if (uiLeftQuantity != uj)
            {
                ppkMerged[ui]= ppkLeft[uj];
                uj++;
            }
            else if (uiRightQuantity != uk)
            {
                ppkMerged[ui]= ppkRight[uk];
                uk++;
            }
            ui++;
        }
        delete[] ppkLeft;
        delete[] ppkRight;
    }
}

//----------------------------------------------------------------------------

template <class T>
void eC_TListDoubleLinked<T>::Sort(eC_Bool bIsQuicksort)
{
    if ( m_uiQuantity > 1 )
    {
        eC_UInt ui;
        Node** ppSortPointers;  //pointer to Array to sort list
        ppSortPointers = new Node * [ m_uiQuantity ];
        Iterator pkIter;
        pkIter = GetBegin();


        //Get pointers to List Nodes into Array;

        for (ui = 0; ui < m_uiQuantity; ui++ )
        {
            ppSortPointers[ ui ] = pkIter.GetNode();
            if(pkIter.IsValid())
            {
                ++pkIter;
            }
            else
            {
                delete[] ppSortPointers;
                return;
            }
        }

        //sort Node pointers in Array
        if (bIsQuicksort)
            SortArraysUsingQuickSort( ppSortPointers, 0, m_uiQuantity - 1 );
        else
            SortArraysUsingMergeSort( ppSortPointers, m_uiQuantity);

        //set List pointers

        //firstElement
        m_pkFront = ppSortPointers[ 0 ];

        ppSortPointers[ 0 ] ->m_pkPrevious = NULL;

        ppSortPointers[ 0 ] ->m_pkNext = ppSortPointers[ 1 ];

        for (ui = 1; ui < m_uiQuantity - 1;ui++ )
        {
            ppSortPointers[ ui ] ->m_pkPrevious = ppSortPointers[ ui - 1 ];
            ppSortPointers[ ui ] ->m_pkNext = ppSortPointers[ ui + 1 ];
        }

        //lastElement
        ppSortPointers[ m_uiQuantity - 1 ] ->m_pkPrevious = ppSortPointers[ m_uiQuantity - 2 ];

        ppSortPointers[ m_uiQuantity - 1 ] ->m_pkNext = NULL;

        m_pkLast = ppSortPointers[ m_uiQuantity - 1 ];

        delete[] ppSortPointers;

        UpdateIterators( pkIter.GetNode(), KILL );
    }
}

//----------------------------------------------------------------------------

template <class T>
void eC_TListDoubleLinked<T>::UpdateIterators( Node* pkNode, Update_t eUpdate ) const
{
    for ( eC_UInt i = 0; i < m_uiIterRegisterCnt; i++ )
    { //Update Safe Iterators, if element is removed or added
        assert(m_ppIterRegister[i] != NULL);
        ( m_ppIterRegister[i] )->Validate( pkNode, eUpdate );
    }
}

//----------------------------------------------------------------------------
template <class T>
void eC_TListDoubleLinked<T>::RegisterIterator( eC_TSafeIterator<T>* pkNewIter )
{
    if( pkNewIter )
    {
        if ( m_uiIterRegisterSize == m_uiIterRegisterCnt )
            //resize Array (+5)
        {
            eC_TSafeIterator<T>** ppTempRegister = new eC_TSafeIterator<T>* [m_uiIterRegisterSize + 5];

            for ( eC_UInt i = 0; i < m_uiIterRegisterCnt; i++ )
                //Copy elements from old array to new one
            {
                ppTempRegister[ i ] = m_ppIterRegister[ i ];
            }

            if ( m_uiIterRegisterSize != 0 )
            {
                delete[] m_ppIterRegister;  //delete old array
            }

            m_ppIterRegister = ppTempRegister;
            m_uiIterRegisterSize = m_uiIterRegisterSize + 5;
        }

        m_ppIterRegister[ m_uiIterRegisterCnt ] = pkNewIter; //add new Iterator pointer
        m_uiIterRegisterCnt++;
        pkNewIter->SetList(this);
    }
}

//----------------------------------------------------------------------------
template <class T>
void eC_TListDoubleLinked<T>::UnregisterIterator( eC_TSafeIterator<T>* pkRemoveIter )
{
    if( pkRemoveIter )
    {
        for ( eC_UInt i = 0; i < m_uiIterRegisterCnt; i++ )
        {
            if ( m_ppIterRegister[i] == pkRemoveIter )  //delete and shrink
            {
                m_ppIterRegister[i]=NULL;
                pkRemoveIter->SetList(NULL);
                for ( eC_UInt j = i; j < m_uiIterRegisterCnt - 1; j++ )
                {
                    m_ppIterRegister[j] = m_ppIterRegister[ j+1 ];
                }

                m_uiIterRegisterCnt--;
                break;
            }
        }
    }
}

//----------------------------------------------------------------------------

/** Compares two lists. Returns True if list1.GetQuantity() == list2.GetQuantity()
    and if each element of list1 is equal to the corresponding element of list2.
  */
template<typename T>
bool operator==(const eC_TListDoubleLinked<T>& list1,
                const eC_TListDoubleLinked<T>& list2)
{
    if (list1.GetQuantity() != list2.GetQuantity())
    {
        return false;
    }

    typename eC_TListDoubleLinked<T>::Iterator it1, it2;
    it2 = list2.GetBegin();

    for (it1 = list1.GetBegin(); it1.IsInsideList(); ++it1, ++it2)
    {
        if ( !((*it1) == (*it2)))
        {
            return false;
        }
    }

    return true;
}

/** Compares two lists. Returns True if list1.GetQuantity() != list2.GetQuantity()
    or if any element of list1 is not equal to the corresponding element of list2.
  */
template<typename T>
bool operator!=(const eC_TListDoubleLinked<T>& list1,
                const eC_TListDoubleLinked<T>& list2)
{
    return !(list1 == list2);
}

#endif
