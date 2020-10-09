/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef REF_COUNTED_H
#define REF_COUNTED_H

#include "eC_Types.h"
#include "GUITrace.h"
#include "eC_Atomic.h"


///Contains pointer class templates.
namespace NSmartPtr
{

/**
    The RefCounted class is used in conjunction with SharedPtr to implement
    reference counting and automatic deletion for user classes.
    Derive any other class from this one to add the reference counting
    methods. Objects of this class delete themselves when the reference count
    reaches zero.

    @ingroup FRAMEWORK_SMART_PTR
*/
class RefCounted ///The reference counted pointer class used with SharedPtr
{
public:
    /// Create RefCounted object. The reference count is initialized to 0.
    RefCounted()
    : m_iRefCount(0)
    {
    }

    /// Destroy reference counted object.
    virtual ~RefCounted()
    {
        if (m_iRefCount != 0)
            GUILOG(GUI_TRACE_WARNING, "RefCounted::~RefCounted: reference count not 0.");
    }

    /// Increase reference count.
    inline void AddRef()
    {
        eC_Atomic::increment( &m_iRefCount );
    }

    /**
    Decrease reference count.

    @note pointee is deleted if the reference count is zero.
    */
    inline void Release()
    {
        eC_Int newCount = eC_Atomic::decrement( &m_iRefCount );
        if (newCount == 0)
        {
            delete this;
        }
    }

    /**
    Check whether this instance is referenced once.

    @return True if instance is referenced once, False if
    multiple references to this instance exist.

    @note In a multithreaded environment this method should be used
    with care, because the reference count may change at any time.
    */
    inline bool Unique() const { return (m_iRefCount == 1); }

    /**
    Get the current reference count.

    @return The current reference count for this object.

    @note In a multithreaded environment this method should be used
    with care, because the reference count may change at any time.
    */
    inline eC_Int RefCount() const { return static_cast<eC_Int const volatile &>( m_iRefCount ); }

private:
    mutable eC_Int m_iRefCount;
}; // class RefCounted

} // namespace NSmartPtr

#endif // REF_COUNTED_PTR_H
