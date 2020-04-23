/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUIREFCNTPTR_H__)
#define GUIREFCNTPTR_H__

#include "RefCounted.h"

///Contains pointer class templates.
namespace NSmartPtr
{
/**
 * This class is used to handle pointers automatically. It uses a
 * reference counter to track the number of currently active copies
 * of this pointer. When RefCount reaches 0, the pointer will be
 * destroyed automatically using 'delete' statements.
 *
 * It should be handled like normal pointers with nearly all advantages
 * and disadvantages, furthermore it destroys itself, if last reference is
 * removed.
 *
 * A pointer given to an instance of this class assigns control for its
 * destruction completely to CGUIRefCntPtr. Giving the same pointer to
 * another instance of CGUIRefCntPtr or deleting it, definitely results in
 * an unexpected behavior.
 *
 * The controlled pointer can be assigned to another type of CGUIRefCntPtr
 * using the provided cast methods StaticCast() and DynamicCast().
 *
 * CGUIRefCntPtr is valid to handle NULL pointers also, so it is up to the
 * developer to check for NULL, before applying operations to the controlled
 * pointer using overloaded methods like operator*() or operator->().
 * \code
 *  CGUIRefCntPtr<CStack> p = CGUIRefCntPtr(new CStack());
 *  if(p != NULL)
 *  {
 *    p->Push();
 *  }
 * \endcode
 *
 * The example above creates a pointer to a stack management class and
 * assigns it to an instance of CGUIRefCntPtr. From now on we can copy the
 * pointer by assigning it to other CGUIRefCntPtr instances.
 *
 * \code
 *  // Assign pointer to another CGUIRefCntPtr and increment references.
 *  CGUIRefCntPtr<CStack> p2 = p;
 *  // Assignment can also be done using copy constructor
 *  CGUIRefCntPtr<CStack> p2(p);
 * \endcode
 *
 * The pointer can be accessed using the methods operator*(), operator->()
 * and Get(). Get() returns the raw pointer for further purposes, but control
 * to it is not released, so do not delete or assign it to another
 * CGUIRefCntPtr directly.
 *
 * @note Avoid 'memcpy' or similar operations applied to instances of this
 *       class. It can result in undesirable behavior or leaks in the reference
 *       counter.
 *
 * As this class handles pointers to every type of class that templates can
 * handle, there exists another implementation which handles pointers to
 * classes that bring their own RefCounted implementation.
 * @see SharedPtr
 *
 * @ingroup FRAMEWORK_SMART_PTR
 */
template<typename T>
class CGUIRefCntPtr ///The Guiliani reference counted pointer class
{
public:
    /**
        Constructs a ref counter and associates given pointer to it.
        @param p The object to be held
    */
    explicit CGUIRefCntPtr(T* p=(T*)0) throw()
        : m_h(new Holder(p))
    { m_h->AddRef(); }

    /**
        Destructor.
    */
    ~CGUIRefCntPtr()
    { m_h->Release(); }

    /**
        Copy constructor.
        @param rkSource The source to copy from
    */
    CGUIRefCntPtr(const CGUIRefCntPtr<T>& rkSource)
    {
        rkSource.m_h->AddRef();
        m_h = rkSource.m_h;
    }

    /**
        Gives no. of current RefCnts pointing to this object.
        @return NOF references of this object.
    */
    eC_Int GetNOFReferences() const
    { return m_h->RefCount(); }

    /**
        Check whether the pointer managed by this class only has one reference.
        @return True, if the managed pointer is unique, otherwise False.
    */
    eC_Bool IsUnique() const
    { return m_h->Unique(); }

    /**
        Static cast of ref counted pointer type to another one. Try to call
        this method with an unrelated type will end up in compile errors. So
        type-safety is given.
     
        @tparam T2 Target class the static cast is to be applied to.
        @return Casted ref counted pointer.
    */
    template<typename T2>
    CGUIRefCntPtr<T2> StaticCast()
    {
        // This cast is a type safety check to avoid casts of this
        // pointer to unrelated types.
        if(static_cast<T2*>(m_h->m_ptr) != NULL) { }
        return CGUIRefCntPtr<T2>(
            reinterpret_cast<CGUIRefCntPtr<T2>&>(*this));
    }

#if defined(GUILIANI_USE_RTTI)
    /**
        Dynamic cast of ref counted pointer type to another one.

        Try to call this method with an unrelated type will end up in compile
        errors. So type-safety is given. Additionally to StaticCast() a
        DynamicCast() returns a NULL pointer in case of the casted class is not
        of the same derivation path at runtime.

        @tparam T2 Target class the dynamic cast is to be applied to.
        @return Casted ref counted pointer or NULL, if derivation path for this
                cast is not straightforward.
    */
    template<typename T2>
    CGUIRefCntPtr<T2> DynamicCast()
    {
        T2* p = dynamic_cast<T2*>(m_h->m_ptr);
        if(p != NULL)
        {
            return CGUIRefCntPtr<T2>(
                reinterpret_cast<CGUIRefCntPtr<T2>&>(*this));
        }
        return CGUIRefCntPtr<T2>(NULL);
    }
#endif // GUILIANI_USE_RTTI

    /**
        Assignment operator for CGUIRefCntPtr.
        Reference counter is incremented. Input and output refcount pointer
        classes needs to be of same type.
        @param rkSource Input class to be assigned to this one.
        @return Reference to this class, pointer assigned and refcount
                incremented.
    */
    CGUIRefCntPtr<T>& operator=(const CGUIRefCntPtr<T>& rkSource)
    {
        rkSource.m_h->AddRef();
        m_h->Release();
        m_h = rkSource.m_h;
        return *this;
    }

    /**
        Access managed object by -> operator.
        @return Pointer to managed object.
    */
    T* operator->() const
    { return m_h->m_ptr; }

    /**
        Access managed object by * operator.
        @return Reference to managed object.
    */
    T& operator*() const
    { return *m_h->m_ptr; }

    /**
        Access managed object by * operator. Note that management of the pointer
        will not be released by calling this method.
        @return Raw pointer managed by this class.
    */
    T* Get() const
    { return m_h->m_ptr; }

    /**
        Check for same pointer.
        @param rkRight Pointer to compare with
        @result True if identical
    */
    eC_Bool operator==(const CGUIRefCntPtr<T>& rkRight) const
    { return m_h->m_ptr == rkRight.m_h->m_ptr; }

    /**
        Check for same pointer.
        @param rkRight Pointer to compare with
        @result True if identical
    */
    template<typename T2>
    eC_Bool operator==(const CGUIRefCntPtr<T2>& rkRight) const
    { return m_h->m_ptr == static_cast<T*>(rkRight.m_h->m_ptr); }

    /**
        Check for same pointer.
        @param pRight Pointer to compare with
        @result True if identical
    */
    eC_Bool operator==(const T* pRight) const
    { return m_h->m_ptr == pRight; }

    /**
        Check for different pointer.
        @param rkRight Pointer to compare with
        @result True if different
    */
    eC_Bool operator!=(const CGUIRefCntPtr<T>& rkRight) const
    { return !(operator==(rkRight)); }

    /**
        Less operator, to enable using an object of this class as a key in an associative container.
        @param rkRight Pointer to compare with
        @result True if less than
    */
    eC_Bool operator<(const CGUIRefCntPtr<T>& rkRight) const
    { return *m_h->m_ptr < *rkRight.m_h->m_ptr; }

    /**
        Check for different pointer.
        @param rkRight Pointer to compare with
        @result True if different
    */
    template<typename T2>
    eC_Bool operator!=(const CGUIRefCntPtr<T2>& rkRight) const
    { return !(operator==(static_cast<T*>(rkRight.m_h->m_ptr))); }

    /**
        Check for different pointer.
        @param pRight Pointer to compare with
        @result True if different
    */
    eC_Bool operator!=(const T* pRight) const
    { return !(operator==(pRight)); }

private:
    /**
        This nested class manages the pointer and reference counts to it. Every
        instance of CGUIRefCntPtr holds a pointer to Holder, which represents
        the managed pointer.
    */
    class Holder : public RefCounted
    {
    public:
        Holder(T* ptr)
            : RefCounted(), m_ptr(ptr)
        { }

        ~Holder()
        { if(m_ptr) delete m_ptr; m_ptr = NULL; };

        T* m_ptr;
    };

    /**
        Pointer to the pointer management class.
    */
    Holder* m_h;
};
} // namespace NSmartPtr

#endif // GUIREFCNTPTR_H__
