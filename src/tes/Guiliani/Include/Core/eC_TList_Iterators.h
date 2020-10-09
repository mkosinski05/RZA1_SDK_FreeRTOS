/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined( ECTLISTITERATORS_H )
#define ECTLISTITERATORS_H

#include "eC_Types.h"
#include <cassert>

template <class T> class eC_TListDoubleLinked;
template <class T> class ListNode;
template <class T> class eC_TSafeIterator;


/// Classic iterator implementation with operator++, operator--, operator*.

/** Example for creating an iterator:
@code
eC_TListDoubleLinked<listT>::Iterator my_Iterator;
@endcode

    Use the following methods for initializing an iterator:
    - eC_TListDoubleLinked<T>::GetBegin()
    - eC_TListDoubleLinked<T>::GetEnd()
    - eC_TListDoubleLinked<T>::GetFind()
*/
template <class T>

class eC_TIterator
{
    friend class eC_TListDoubleLinked<T>;
    friend class eC_TSafeIterator<T>;

protected:

    /// Node is typedef'd as a ListNode of type T
    typedef ListNode<T> Node;

    /// this node
    Node* m_pNode;

public:

    //-------------------------------------
    /**
        Default constructor, used to initialize an invalid iterator.
    */
    eC_TIterator() : m_pNode(NULL) {}

    /**
        Constructs an iterator with the given node.
        @param pNode The node that the new iterator points to.
    */
    eC_TIterator( Node* pNode ) : m_pNode( pNode ) {}

    /// Destructor
    virtual ~eC_TIterator() {}

    /**
        Copy constructor
        e.g. eC_TIterator myIterator;
                eC_TIterator myIterator2 = myIterator;
        @param x Where to copy from
    */
    eC_TIterator( const eC_TIterator<T>& x ) : m_pNode( x.m_pNode ) {}

    //-------------------------------------
    /**
        Assignment operator is used to initialize with GetBegin(), GetEnd() and GetFind()
        e.g. myIterator=myList.GetBegin()
        @param kIter Where to copy from
        @return eC_TIterator& to this node after assignment
    */
    eC_TIterator& operator=( const eC_TIterator& kIter )
    {
        m_pNode = kIter.m_pNode;
        return *this;
    }

    //-------------------------------------
    /**
        Equivalence operator is used to compare with GetBegin(), GetEnd(), and GetFind(Value)
        e.g. "if (myIterator==myList.GetBegin())"
        @param kIter What to compare with
        @return eC_Bool True if identical, False otherwise
    */
    eC_Bool operator==( const eC_TIterator& kIter ) const
    {
        return m_pNode == kIter.m_pNode;
    }

    /**
        Used to compare iterator and node pointer,
        e.g. if (iter == NULL).
        @return True, if iterator points to node.
        @return False else.
        @param ptrNode What to compare with
    */
    eC_Bool operator==( const Node* const ptrNode ) const
    {
        return m_pNode == ptrNode;
    }

    //-------------------------------------
    /**
        Used to compare two iterators.
        @return True, if iterators are not pointing at the same list element,
                False if iterators are pointing to same list element.
        @param kIter What to compare with
    */
    eC_Bool operator!=( const eC_TIterator& kIter ) const
    {
        return m_pNode != kIter.m_pNode;
    }

    //------------------------------
    /**
        Used to test if the element the Iterator is pointing to, was removed or not.
        Note, you cannot continue to iterate through the list using an invalid iterator,
        e.g. "iterator++;" on an invalid iterator will do nothing.
        You also cannot dereference an iterator that is invalid.
        @return True if iterator is valid (element was not removed), False if iterator is invalid.
    */
    eC_Bool IsValid() const
    {
        return m_pNode != NULL;
    }

    //-------------------------------------
    /**
        Used to test if iterator is not pointing beyond list.
        @return True, if iterator is pointing an element of the list, false
            if iterator is pointing in front of first or behind last list element.
    */
    virtual eC_Bool IsInsideList() const
    {
        return m_pNode != NULL;
    }

    //-------------------------------------
    /**
        Used to increment iterator. Postfix version, e.g. "iter++;".
        Iterator is pointing to next list element.
        This operation is inefficient, please use prefix version.
        @return Old iterator before incrementation.
                The result (old iterator) is used when the incremented iterator
                ('this') is assigned or copied.
    */
    eC_TIterator operator++(int)
    {
        eC_TIterator tmpIter = *this; // save old iterator
        if ( m_pNode )
        {
            m_pNode = m_pNode->m_pkNext; // next may be NULL if at end
        }
        return tmpIter; // return old iterator
    }

    //-------------------------------------
    /**
        Used to increment iterator. Prefix version, e.g. "++iter;".
        Iterator is pointing to next list element.
        @return New iterator after incrementation. Invalid iterator if beyond the end.
                The result (new iterator) is used when the incremented iterator
                ('this') is assigned or copied.
    */
    eC_TIterator& operator++()
    {
        if ( m_pNode != NULL )
        {
            m_pNode = m_pNode->m_pkNext; // next may be NULL if at end
        }

        return *this; // return new iterator (uses copy contructor)
    }

    //-------------------------------------
    /**
        Used to decrement Iterator. Postfix version, e.g. "iter--;".
        Iterator is pointing to previous list element.
        This operation is inefficient, please use prefix version.
        @return Old iterator before incrementation.
                The result (old iterator) is used when the incremented iterator
                ('this') is assigned or copied.
    */
    eC_TIterator operator--(int)
    {
        eC_TIterator tmpIter = *this;
        if ( m_pNode )
        {
            m_pNode = m_pNode->m_pkPrevious;
        }
        return tmpIter;
    }

    //-------------------------------------
    /**
        Used to decrement Iterator. Prefix version.
        Iterator is pointing to previous list element.
        @return New iterator after decrementation. Invalid iterator if beyond the beginning.
                The result (new iterator) is used when the incremented iterator
                ('this') is assigned or copied.
    */
    eC_TIterator& operator--()
    {
        if ( m_pNode )
        {
            m_pNode = m_pNode->m_pkPrevious;
        }

        return *this;
    }

    //-------------------------------------
    /**
        Used to get reference to element, iterator is pointing at.
        Only works if iterator is valid (after deleting element iterator is pointing at, it is not).

        @return Reference to element iterator is pointing at
    */
    T & operator*()
    {
        if ( m_pNode )
        {
            return m_pNode->m_tValue;
        }

        throw 0; // referencing deleted node
    }

protected:

    /**
        Get this node as a pointer
        @return Node Pointer to this node
    */
    Node* GetNode() const
    {
        return m_pNode;
    }
};


//----------------------------------------------------------------------------
//----------------------safe Iterators----------------------------------------
//----------------------------------------------------------------------------
enum Update_t {REMOVED, ADDED, KILL, REVERSE_ORDER};

/// An iterator that stays valid even if elements are deleted from the list.

/**
    With safe iterators it is possible to remove elements while iterating through a list.

    With IsValid() it can be tested if the iterator may be dereferenced.
    If an item is deleted, the iterator is invalid, but you can still iterate
    through the list.
    After the next step (increment or decrement) the iterator is valid again (until you remove another element).
    After sorting and eC_TListDoubleLinked<T>::RemoveAll() all safe iterators are invalid and have to be set again.

    Example:

@code
eC_TListDoubleLinked<listT>::SafeIterator my_SafeIterator();

for (my_SafeIterator = m_list.GetBegin(); my_SafeIterator.InsideList(); ++my_SafeIterator)
{
    if (*my_SafeIterator == testValue)
    {
        m_list.Remove(testValue);
    }

    if (my_SafeIterator.IsValid())
    {
        // The iterator is still valid and can be used to get a reference to
        // the element.
        cout << *my_SafeIterator << endl;
    }
    else
    {
        // If testValue was removed, the iterator cannot be used to get
        // reference to the element.
        cout << "Value invalid" << endl;
    }
}
@endcode

    If you remove an item and add one at the position of the removed element (in one
    iteration step) it is possible that the new element is "lost" in that iteration.

    Example

    3-5-8

    5 is deleted
    7 is added

    3-7-8

    In this case 7 will be "lost" if iterator pointed at 5, because iterator is now
    pointing at 7, and after next iteration step it will be pointing at 8.

    Note: This iterator does not have postfix increment/decrement operators
    because implementing them for the current approach for this safe iterator
    is not effective.
*/
template <class T>

class eC_TSafeIterator : public eC_TIterator<T>
{
    friend class eC_TListDoubleLinked<T>;
    eC_TListDoubleLinked<T>* m_pList;    ///< pointer to list that this iterator is used with (needed for deregistration)
    typename eC_TIterator<T>::Node* m_pSaveNextNode; ///< pointer that is necessary to be able to iterate after removing
    typename eC_TIterator<T>::Node* m_pSavePrevNode; ///< pointer that is necessary to be able to iterate after removing

    //------------------------------

    //this function updates a safe iterator, if list was modified.
    //function is called by list, if iterator has been registered (e.g. while initialized).
    void Validate( typename eC_TIterator<T>::Node* pkUpdateNode, Update_t eUpdate )
    {
        typename eC_TIterator<T>::Node * pkTempNode;

        switch ( eUpdate )
        {
        case REMOVED :
            if ( eC_TIterator<T>::m_pNode == pkUpdateNode )     //current element was removed
            {
                eC_TIterator<T>::m_pNode = NULL; //invalidate iterator until next iteration
            }

            else if ( m_pSaveNextNode == pkUpdateNode )     //next element was removed
            {
                assert(m_pSaveNextNode != NULL);
                m_pSaveNextNode = m_pSaveNextNode->m_pkNext;
            }

            else if ( m_pSavePrevNode == pkUpdateNode )     //previous element was removed
            {
                assert(m_pSavePrevNode != NULL);
                m_pSavePrevNode = m_pSavePrevNode->m_pkPrevious;
            }
            break;

        case KILL :      // iterator is invalid (after sorting and RemoveAll in list)
            eC_TIterator<T>::m_pNode = NULL;
            m_pSavePrevNode = NULL;
            m_pSaveNextNode = NULL;
            break;

        case REVERSE_ORDER :
            pkTempNode = m_pSaveNextNode;
            m_pSaveNextNode = m_pSavePrevNode;
            m_pSavePrevNode = pkTempNode;
            break;

        case ADDED :
            // The node "pkUpdateNode" was added:
            // if new element is added before current element that is associated with this iterator

            if ( eC_TIterator<T>::m_pNode == pkUpdateNode->m_pkNext  // current element is next element of updated node
                    && eC_TIterator<T>::m_pNode != NULL )                   // current element is not invalid
            {
                m_pSavePrevNode = pkUpdateNode;   // previous node is the new node
            }

            // if new element is added after current element that is associated with this iterator
            else if ( eC_TIterator<T>::m_pNode == pkUpdateNode->m_pkPrevious
                        && eC_TIterator<T>::m_pNode != NULL )
            {
                m_pSaveNextNode = pkUpdateNode;
            }

            // if formerly current node was deleted before and other element was added at this position
            else if ( eC_TIterator<T>::m_pNode == NULL )
            {
                if ( m_pSavePrevNode == pkUpdateNode->m_pkPrevious
                        && m_pSaveNextNode == pkUpdateNode->m_pkNext )
                {
                    eC_TIterator<T>::m_pNode = pkUpdateNode;
                }
            }
            break;
        }
    }

    void SetList(eC_TListDoubleLinked<T>* pList)
    {
        if( m_pList != pList)
        {
            eC_TListDoubleLinked<T>* pTempList = m_pList;
            m_pList = pList;
            if( pTempList )
                pTempList->UnregisterIterator(this);
            if( pList )
                pList->RegisterIterator(this);
        }
    }

public:
    /**
        Constructor needs pointer to list, iterator will be used with.
        This is necessary to register iterator in this list, so that iterators can
        be updated if list is modified.
    */
    eC_TSafeIterator() 
        : m_pList(NULL),
        m_pSaveNextNode(NULL),
        m_pSavePrevNode(NULL)
    {}

    /** 
        Copy constructor.
        @param kIter Where to copy from
    */
    eC_TSafeIterator( const eC_TSafeIterator<T>& kIter )
        : eC_TIterator<T>(kIter),
          m_pList(NULL)
    {
        SetList( kIter.m_pList);
        m_pSaveNextNode = kIter.m_pSaveNextNode;
        m_pSavePrevNode = kIter.m_pSavePrevNode;
    }

    //------------------------------
    virtual ~eC_TSafeIterator()
    {
        SetList(NULL);
    }

    /**
        Assignment operator.
        @return eC_TSafeIterator& to local node after assignment
        @param kIter Where to copy from
    */
    eC_TSafeIterator& operator=( const eC_TSafeIterator& kIter )
    {
        eC_TIterator<T>::m_pNode = kIter.m_pNode;
        SetList( kIter.m_pList);
        m_pSaveNextNode = kIter.m_pSaveNextNode;
        m_pSavePrevNode = kIter.m_pSavePrevNode;

        return *this;
    }

    //-------------------------------------
    /**
        Used to compare two iterators.
        @param kIter What to compare with
        @return True, if iterators are pointing to the same list element and iterator would behave in the same way in case of iteration, False if not
    */
    eC_Bool operator==( const eC_TSafeIterator &kIter ) const
    {
        return ( eC_TIterator<T>::m_pNode == kIter.eC_TIterator<T>::m_pNode
                    && ( m_pSavePrevNode == kIter.m_pSavePrevNode )
                    && ( m_pSaveNextNode == kIter.m_pSaveNextNode ));
    }

    //-------------------------------------
    /**
        Used to compare two Iterators.
        @param kIter What to compare with
        @return False, if iterators are pointing to the same list element and iterator would behave in the same way in case of iteration, True if not
    */
    eC_Bool operator!=( const eC_TSafeIterator &kIter )
    {
        return ( eC_TIterator<T>::m_pNode != kIter.eC_TIterator<T>::m_pNode
                    || ( m_pSavePrevNode != kIter.m_pSavePrevNode )
                    || ( m_pSaveNextNode != kIter.m_pSaveNextNode ) );
    }

    //-------------------------------------
    /**
        Used to increment Iterator. Prefix version.
        Iterator is pointing to next list element.
        @return Unchanged iterator.
            If current element is beyond the end of the list then an interator
            will be returned that will evaluate IsInsideList() to false, as before.
    */
    eC_TSafeIterator& operator++()
    {
        typename eC_TIterator<T>::Node * pOldNode = eC_TIterator<T>::m_pNode;
        eC_TIterator<T>::m_pNode = m_pSaveNextNode;

        if( pOldNode)
            m_pSavePrevNode = pOldNode;

        if ( m_pSaveNextNode != NULL )
        {
            m_pSaveNextNode = m_pSaveNextNode->m_pkNext;
        }

        return *this;
    }

    //-------------------------------------
    /**
        Used to decrement Iterator. Prefix version.
        Iterator is pointing to previous list element.
        @return Unchanged iterator.
            If current element is beyond the beginning of the list then an interator
            will be returned that will evaluate IsInsideList() to false, as before.
    */
    eC_TSafeIterator& operator--()
    {
        typename eC_TIterator<T>::Node * pOldNode = eC_TIterator<T>::m_pNode;
        eC_TIterator<T>::m_pNode = m_pSavePrevNode;

        if( pOldNode)
            m_pSaveNextNode = pOldNode;

        if ( m_pSavePrevNode != NULL )
        {
            m_pSavePrevNode = m_pSavePrevNode->m_pkPrevious;
        }

        return *this;
    }

    //-------------------------------------
 /**
        !DO NOT USE WHEN ITERATING BACKWARDS AND DELETING NODES! Use FOR_ALL_BACKWARD_SAFE instead.
        Used to test if iterator is not pointing beyond list.
        @return True, if iterator is pointing an element of the list, false
            if iterator is pointing in front of first or behind last list element.
    */
    virtual eC_Bool IsInsideList() const
    {
        // Note, we may also be inside of list when current element is invalid
        return !( ( eC_TIterator<T>::m_pNode == NULL && m_pSaveNextNode == NULL )
               || ( eC_TIterator<T>::m_pNode == NULL && m_pSavePrevNode != NULL ) );
    }


    /// Returns if a valid node is available in front of the current node.
    eC_Bool IsPreviousValid() const { return m_pSavePrevNode != NULL;}

    /// Returns if a valid node is available behind the current node.
    eC_Bool IsNextValid() const{ return m_pSaveNextNode != NULL;}
};

#endif // ECTLISTITERATORS_H
