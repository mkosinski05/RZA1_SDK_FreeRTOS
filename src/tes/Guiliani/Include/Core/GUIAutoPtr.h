/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUIAUTOPTR_H__
#define __GUIAUTOPTR_H__

#include "eC_Types.h"

/// Guiliani-implementation of an auto-pointer

/** The CGUIAutoPtr template class describes an object that stores a pointer to a single allocated object of type T that ensures
    that the object to which it points gets destroyed automatically when control leaves a scope.
    In other words: If you allocate memory and assign it to an auto-pointer, the auto-pointer will delete this memory if it gets destructed.

    A typical use-case example would be:
@code
    // Demo scope
    {
        // Construct a CGUIObject and hand over the pointer to an auto-pointer
        CGUIAutoPtr<CGUIObject> pObject( new CGUIObject(), false );
        // pObject gets destroyed at the end of scope and will automatically delete the CGUIObject which we created
    }
@endcode

    A demo use-case where a UNICODE-String gets converted to an array of ASCII-characters:
@code
    eC_String kString("I am selling these fine leather jackets");
    // Demo scope
    {
        // Convert string to ASCII. Note that we are allocating an ARRAY and must inform the auto-pointer about this,
        // by supplying true as the second parameter.
        CGUIAutoPtr<eC_Char> pTempString(kString.ToASCII_Alloc(), true);
        // pTempString gets destroyed at the end of scope and will automatically delete the temporary eC_Char buffer
    }
@endcode

*/
template<class T>
class CGUIAutoPtr
{
public:

    /// typedef of element type
    typedef T element_type;

    /** Constructor
        @param p Pointer to object
        @param bIsArray Whether to allocate as array
    */
    explicit CGUIAutoPtr(element_type* p, eC_Bool bIsArray) throw()
        : m_ptr(p),
          m_bIsArray(bIsArray)
    { }

    /** Copy-constructor.
        @param kSource Source object to be copied.
               The pointer in the source will be set to NULL.
    */
    CGUIAutoPtr(CGUIAutoPtr<T>& kSource) throw()
    {
        m_ptr = kSource.m_ptr;
        kSource.m_ptr = NULL;
        m_bIsArray = kSource.m_bIsArray;
    }

    /** Destructor.
    */
    ~CGUIAutoPtr() throw()
    {
        DestroyPointer();
    }

    /** Operator= method.
        @param kSource Source object to be copied.
               The source pointer will be set to NULL.
        @return This instance.
    */
    CGUIAutoPtr<T>& operator= (CGUIAutoPtr<T>& kSource) throw()
    {
        m_ptr = kSource.m_ptr;
        kSource.m_ptr = NULL;
        m_bIsArray = kSource.m_bIsArray;
        return *this;
    }

    /// dereference operator
    element_type& operator* () const throw() {return *m_ptr;}

    /// arrow operator
    element_type* operator-> () const throw() {return m_ptr;}

    /// Extract pointer.
    element_type* get() const throw() {return m_ptr;}

    /// Release pointer. This class will not control the pointer anymore.
    element_type* release() throw() {element_type* p=m_ptr; m_ptr = NULL; return p;}

    /// Reset the pointer to other one, the original pointer will be deleted.
    void reset(element_type* p=0) throw() {if(p!=m_ptr){DestroyPointer(); m_ptr=p;}}

    /// Overloaded operator = deletes the old pointer and sets the pointer.
    CGUIAutoPtr& operator = (element_type* p){if(p!=m_ptr){DestroyPointer(); m_ptr=p;}return *this;}

    /// Convert this auto pointer with type T to auto pointer with type Y.
    template<class Y> operator CGUIAutoPtr<Y>() throw()
    {
        CGUIAutoPtr<Y> kYAutoPtr = *this;
        return kYAutoPtr;
    }

private:
    /** Destruct the pointer using the correct delete operator. */
    void DestroyPointer()
    {
        if(m_bIsArray == true)
        {
            delete[] m_ptr;
        }
        else
        {
            delete m_ptr;
        }
        m_ptr = NULL;
    }

    /// The pointer to be controlled.
    element_type* m_ptr;
    /// Is the pointer an array, i.e. needs it to be deallocated by delete[].
    eC_Bool m_bIsArray;
};
#endif //__GUIAUTOPTR_H__
