/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __SAFEGUIOBJECTPTR_H__
#define __SAFEGUIOBJECTPTR_H__

#include "eC_TList_doubleLinked.h"

class CGUIObject;


/** This class is useful if you are accessing an object which is not under your sole control, i.e. it could be deleted
    by other code, rendering your local pointer invalid.

    Warning: You shall not instantiate objects of this class as members of static classes, since CSafeGUIObjectPtr requires the framework
    to be fully initialized!

    Performance note: Compared to a plain GUIObject* this class will impose a performance overhead, so use it sparingly and stick with
    simple CGUIObject-pointers when possible!

    Example:
@code
        // Create test container
        CGUICompositeObject* pContainer = new CGUICompositeObject( &GETGUI, 0,0,100,100);
        // Create test object and store pointer in CSafeGUIObjectPtr
        CSafeGUIObjectPtr pTest = new CGUIObject( pContainer, 0,0,10,10);
        // delete container (and thus also the contained object)
        delete pContainer;
        // pTest will now automatically point to NULL
        if( pTest)
        {
            // Do something with pTest...
        }
@endcode
    @brief Smart pointer to a CGUIObject which will automatically be set to NULL if the corresponding GUIObject gets destructed.
*/
class CSafeGUIObjectPtr
{
public:
    /** Default constructor
    */
    CSafeGUIObjectPtr()
        : m_ptr(NULL)
    {
        // Add to list of all existing CSafeGUIObjectPtrs
        ms_kSafeGUIObjectPointerList.AddAtEnd( this);
    }

    /** Constructor
        @param ptr Source pointer
    */
    CSafeGUIObjectPtr(CGUIObject* ptr)
        : m_ptr(ptr)
    {
        // Add to list of all existing CSafeGUIObjectPtrs
        ms_kSafeGUIObjectPointerList.AddAtEnd( this);
    }

    /** Constructor
        @param ptr Source pointer
    */
    CSafeGUIObjectPtr(const CSafeGUIObjectPtr& ptr)
    {
        m_ptr = ptr.get();
        // Add to list of all existing CSafeGUIObjectPtrs
        ms_kSafeGUIObjectPointerList.AddAtEnd( this);
    }

    /** Destructor
    */
    ~CSafeGUIObjectPtr()
    {
        Reset();
        ms_kSafeGUIObjectPointerList.Remove( this);
    }

    /** Registers this CSafeGUIObjectPtr for being notified if its referenced object gets destructed.
        This is automatically done during CSafeGUIObjectPtr's construction and only needs to be done manually in case
        an object of CSafeGUIObjectPtr could not successfully register itself upon construction. */
    void Register()
    {
        // Add uniquely to list of all existing CSafeGUIObjectPtrs
        ms_kSafeGUIObjectPointerList.AddUniqueAtEnd( this);
    }


    /** Assignment operator
        @param kSource Source of assignment as a CSafeGUIObjectPtr
        @return Destination of assignment
    */
    CSafeGUIObjectPtr& operator = (const CSafeGUIObjectPtr& kSource)
    {
        m_ptr = kSource.m_ptr;
        return *this;
    }

    /** Assignment operator
        @param pkSource Source of assignment as a CGUIObject*
        @return Destination of assignment
    */
    CSafeGUIObjectPtr& operator = (CGUIObject* pkSource)
    {
        m_ptr = pkSource;
        return *this;
    }

    /** Cast operator to CGUIObject*
        @return Internal pointer cast to CGUIObject*
    */
    operator CGUIObject* () const {return m_ptr;}

    /** Dereference operator
        @return Internal pointer
    */
    CGUIObject& operator* () const {return *m_ptr;}

    /** Arrow operator
        @return Internal pointer
    */
    CGUIObject* operator-> () const {return m_ptr;}

    /** Extract pointer
        @return Internal pointer
    */
    CGUIObject* get() const {return m_ptr;}

    /// Helper function to reset internal pointer
    void Reset()
    {
        // Set internal CGUIObject pointer to NULL
        m_ptr = NULL;
    }

private:
    friend class CGUIObject;

    /// This is called when a CGUIObject gets destructed in order to reset all CSafeGUIObjectPtrs referencing this object
    static void NotifyOfDestruction(const CGUIObject* ptr);

    /** List of all existing CSafeGUIObjectPtr objects.
        Whenever a GUIObject gets destructed, this list will be searched for any CSafeGUIObjectPtrs referencing it.*/
    static eC_TListDoubleLinked< CSafeGUIObjectPtr*> ms_kSafeGUIObjectPointerList;

    /// internal pointer to the actual CGUIObject that this object is pointing to
    CGUIObject* m_ptr;
};


#endif //__SAFEGUIOBJECTPTR_H__
