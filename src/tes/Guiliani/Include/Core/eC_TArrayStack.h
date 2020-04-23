/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(EC_TARRAYSTACK_H)
#define EC_TARRAYSTACK_H

#include "GUIException.h"
#include "eC_Types.h"
#include "eC_TArray.h"

/**
 * @brief Exception thrown when read operations Pop() / Top() are called on an empty stack.
 */
class CGUIStackUnderflowException : public CGUIException
{ };

/**
 * This simple stack implementation provides an array based stack container.
 *
 * Performance: Push: O(n) - Pop: O(1) - Top: O(1)
 *
 * Memory consumption: O(n)
 *
 * NOTE: Do not assign objects to this stack that counts their references
 *       or depends on copy-constructors/assign operators for creating
 *       copies of itself.
 * @brief Am array based stack container
 */
template<typename T>
class eC_TArrayStack
{
public:
    /**
     * Default constructor creates an empty stack.
     */
    eC_TArrayStack()
        : m_uiStackPointer(0),
          m_uiResizeBy(5)
    { }

    /**
     * Copy constructor. Copies every element from given stack to this one.
     *
     * @param rkSource Stack to copy from.
     */
    eC_TArrayStack(const eC_TArrayStack<T>& rkSource)
        : m_uiStackPointer(0),
          m_uiResizeBy(5)
    {
        m_aElements.SetQuantity(rkSource.GetQuantity(), false);
        eC_Memcpy(m_aElements.GetArray(), rkSource.m_aElements.GetArray(),
            m_aElements.GetQuantity() * sizeof(T));
    }

    /**
     * Destructor forces all elements of the stack to be removed.
     */
    ~eC_TArrayStack()
    { }

    /**
     * Assign operator. Copies every element from given stack to this one.
     *
     * @param rkSource Stack to copy from.
     * @return Reference to this class.
     */
    eC_TArrayStack<T>& operator=(const eC_TArrayStack<T>& rkSource)
    {
        m_aElements.SetQuantity(rkSource.GetQuantity(), false);
        eC_Memcpy(m_aElements.GetArray(), rkSource.m_aElements.GetArray(),
            m_aElements.GetQuantity() * sizeof(T));
        m_uiStackPointer = rkSource.m_uiStackPointer;
        return *this;
    }

    /**
     * Get number of elements this stack currently contains.
     *
     * @return NOF elements in the stack.
     */
    inline eC_UInt GetQuantity() const
    { return m_uiStackPointer; }

    /**
     * Test whether the container is empty.
     *
     * @return True, if the stack is empty, otherwise False.
     */
    inline eC_Bool IsEmpty() const
    { return (m_uiStackPointer == 0); }

    /**
     * Add element on top of the stack.
     *
     * @param e Element to add.
     */
    inline void Push(const T& e)
    {
        if(m_uiStackPointer >= m_aElements.GetQuantity())
        {
            m_aElements.SetQuantity(
                m_aElements.GetQuantity() + m_uiResizeBy, true);
        }
        m_aElements[m_uiStackPointer] = e;
        m_uiStackPointer++;
    }

    /**
     * Removes element from top of the stack and return this element.
     *
     * @return Element that has been removed.
     * @throw CGUIStackUnderflowException In case of stack does not
     *  contain elements.
     */
    inline T& Pop()
    {
        if(m_uiStackPointer == 0)
        {
            throw CGUIStackUnderflowException();
        }
        m_uiStackPointer--;
        return m_aElements[m_uiStackPointer];
    }

    /**
     * Get stack top element.
     *
     * @return Element on top of the stack.
     * @throw CGUIStackUnderflowException In case of stack does not
     *  contain elements.
     */
    inline T& Top() const
    {
        if(m_uiStackPointer == 0)
        {
            throw CGUIStackUnderflowException();
        }
        return m_aElements[m_uiStackPointer - 1];
    }

private:
    /** Array of stack values. */
    eC_TArray<T> m_aElements;
    /** Stack pointer is an array offset pointing to the top element + 1. */
    eC_UInt m_uiStackPointer;
    /**
     * If stack overflows specify nof of elements the stacksize is to be
     * incremented here.
     */
    const eC_UInt m_uiResizeBy;
};

#endif // EC_TARRAYSTACK_H
