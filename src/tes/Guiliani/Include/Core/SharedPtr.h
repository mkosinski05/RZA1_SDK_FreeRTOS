/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef SHARED_PTR_H
#define SHARED_PTR_H

/**
    @defgroup FRAMEWORK_SMART_PTR Smart Pointers
    @brief Collection of portable smart pointer implementations

    An "intelligent" pointer that can be used instead of raw C++ pointers
    if resources should be automatically released. The main purpose of
    NSmartPtr::SharedPtr is for automatic destruction of an object if it is not used
    anymore. However, it can also be used for more advanced tasks like
    object caching or pooling.

    Instead of a general purpose smart pointer that can wrap any raw
    C++ pointer, as provided by some C++ libraries, the NSmartPtr::SharedPtr
    requires the pointer class to implement two methods (AddRef,
    Release). For different use cases these two methods may be added in
    different ways.

    The easiest way to add the methods is to use NSmartPtr::RefCounted as a base class:

    @code
    class MyRefCountedClass : public RefCounted
    {
        virtual ~MyRefCountedClass();
    };

    typedef SharedPtr<MyRefCountedClass> SharedMyClassPtr;
    @endcode

    The Release method deletes the instance if the reference count is zero.

    A class is also free to provide an own implementation for AddRef
    and Release. This can be useful to implement other mechanisms for
    resource management, e.g. instead of deletion the objects can be
    pooled or cached, or instead of reference counting a linked list in
    conjunction with explicit garbage collection may be used.

    The following MyClass definition contains own AddRef and Release
    methods:

    @code
    class MyClass
    {
    public:
        void AddRef();
        void Release();
    }

    typedef SharedPtr<MyClass> SharedMyClassPtr;
    @endcode

    Additionally, the CGUIRefCntPtr is provided. It differs from SharedPtr in
    that way that the referee class does not need to implement its own
    reference interface, i.e. AddRef() and Release(). Pointers to every class
    instance can be passed to it, CGUIRefCntPtr becomes responsible for it
    then. If CGUIRefCntPtr is destroyed, the reference counter is decremented.
    A zero reference destructs the pointer of the referee class using the
    delete operator automatically.
*/

///Contains pointer class templates.
namespace NSmartPtr
{

/**
    A SharedPtr manages access to a shared object of a pointer type C.

    The pointee type is specified as template parameter.

    The SharedPtr calls C::AddRef a construction and C::Release on
    destruction. The implementor of the class is responsible to provide
    the AddRef and Release methods. A convenient way to provide these
    methods is the RefCounted class, which provides an automatic
    destruction of allocated objects based on reference counts.

    For most operations a SharedPtr behaves like a raw C++ pointer:

    <UL>
    <LI> A SharedPtr can be NULL.
    <LI> The -> operator of a SharedPtr returns the raw pointer. If the
         pointee class type is const, the returned pointer is also const.
    <LI> The * operator of a SharedPtr dereferences the raw pointer.
    <LI> Shared pointers are equal if the raw pointers are equal. The
         == operator is used to check for equality. The == operator can
        also be used to compare shared pointers and raw pointers.
    <LI> Shared pointers are assignment compatible if the raw pointers
         are assignment compatible, e.g. a SharedPtr<Derived> class may
         be assigned to SharedPtr<Base> if class Derived is derived
         from Base.
    </UL>

    However, there are some differences between shared pointers and raw
    pointers:
    <UL>
    <LI> Shared pointers can only be defined for pointee types that
         provide an AddRef and Release method.
    <LI> Explicit casts (static_cast, const_cast, dynamic_cast) for
         SharedPtrs are currently not provided.
    <LI> A SharedPtr is always typed. Defining a SharedPtr for a
         generic pointee type, like SharedPtr<void*> is not possible.
    </UL>

    @ingroup FRAMEWORK_SMART_PTR
*/
template<class C>
class SharedPtr ///The reference counted pointer class used with RefCounted
{
public:
    /// Creates a shared NULL pointer.
    inline SharedPtr();

    /// Creates a shared pointer for a given raw pointer.
    inline SharedPtr( C* rawPtr );

    /// Creates a copy of a shared pointer.
    inline SharedPtr( const SharedPtr& rhs )
        : m_RawPtr( rhs.m_RawPtr )
    {
        if( m_RawPtr )
        {
            m_RawPtr->AddRef();
        }
    }

    /** Creates a copy of a shared pointer with implicit conversion from
        derived type.
        @param otherSharedPtr The source to copy from
    */
    template< class C2 >
    inline SharedPtr( const SharedPtr<C2>& otherSharedPtr )
        : m_RawPtr( otherSharedPtr.RawPtr() )
    {
        if (m_RawPtr)
        {
            m_RawPtr->AddRef();
        }
    }

    /// Destroys the SharedPtr.
    inline ~SharedPtr();

    /// Access the raw pointer.
    inline C* operator->() const;

    /// Access the dereferenced raw pointer.
    inline C& operator*() const;

    /// Assignment operator with shared pointer of same type.
    SharedPtr&
        operator =( const SharedPtr& rhs )
    {
        Assign( rhs.RawPtr() );

        return *this;
    }

    /// Assignment of SharedPtr of derived type.
    template<class C2>
    inline SharedPtr<C>& operator=( const SharedPtr<C2>& otherSharedPtr )
    {
        Assign( otherSharedPtr.RawPtr() );

        return *this;
    }

    /** Assignment of raw pointer to this SharedPtr with implicit
        conversion from derived type.
        @param rawPtr The source of the assignment
        @return The addignment destination
    */
    template<class C2>
    inline SharedPtr& operator=( C2* rawPtr )
    {
        Assign( rawPtr );

        return *this;
    }

    /** Check for NULL pointer.
        @return True if this SharedPtr is NULL, else False.
    */
    inline bool operator!() const;

    /** Check for NULL pointer.
        @return False if this SharedPtr is NULL, else True.
    */
    inline operator bool() const;

    /** Get the raw pointer.
        @return The raw pointer.
    */
    inline C* RawPtr() const;

private:
    /// The raw pointer managed by this SharedPtr.
    C* m_RawPtr;

    inline void Assign( C* rawPtr );
}; // class SharedPtr

/** Two SharedPtrs are non-equal if their raw pointers are non-equal.
    @ingroup FRAMEWORK_SMART_PTR
    @param a pointer A
    @param b pointer B
    @return True if different
*/
template<class C1, class C2>
inline bool operator!=( const NSmartPtr::SharedPtr<C1>& a,
    const NSmartPtr::SharedPtr<C2>& b )
{
    return (a.RawPtr() != b.RawPtr());
}

/** Two SharedPtrs are equal if their raw pointers are equal.
    @ingroup FRAMEWORK_SMART_PTR
    @param a pointer A
    @param b pointer B
    @return True if equal
*/
template<class C1, class C2>
inline bool operator==( const NSmartPtr::SharedPtr<C1>& a,
    const NSmartPtr::SharedPtr<C2>& b )
{
    return (a.RawPtr() == b.RawPtr());
}

/** Static cast operator
    @param source Source
    @return Result
*/
template< class DEST_TYPE, class SOURCE_TYPE >
NSmartPtr::SharedPtr< DEST_TYPE >
    StaticCast ( const NSmartPtr::SharedPtr< SOURCE_TYPE >& source )
{
    return static_cast< DEST_TYPE* >( source.RawPtr() );
}

/////////////////////////////////////////
// inline methods
/////////////////////////////////////////

template<class C>
inline SharedPtr<C>::SharedPtr()
    : m_RawPtr(0)
{
}

template<class C>
inline SharedPtr<C>::SharedPtr( C* rawPtr )
    : m_RawPtr( rawPtr )
{
    if (rawPtr)
    {
        m_RawPtr->AddRef();
    }
}

template<class C>
inline SharedPtr<C>::~SharedPtr()
{
    if (m_RawPtr)
    {
        m_RawPtr->Release();
    }
}

template<class C>
inline C* SharedPtr<C>::operator->() const
{
    return m_RawPtr;
}

template<class C>
inline C& SharedPtr<C>::operator*() const
{
    return *m_RawPtr;
}

template<class C>
bool SharedPtr<C>::operator! () const
{
    return (m_RawPtr == NULL);
}

template<class C>
SharedPtr<C>::operator bool () const
{
    return (m_RawPtr != NULL);
}

template<class C>
C* SharedPtr<C>::RawPtr() const
{
    return m_RawPtr;
}

template<class C>
inline void SharedPtr<C>::Assign( C* rawPtr )
{
    // call AddRef before Release, because rawPtr may be equal m_RawPtr
    if (rawPtr)
    {
        rawPtr->AddRef();
    }
    if (m_RawPtr)
    {
        m_RawPtr->Release();
    }
    m_RawPtr = rawPtr;
}

} // namespace NSmartPtr

#endif // SMART_PTR_H
