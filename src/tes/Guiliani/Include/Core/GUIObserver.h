/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIOBSERVER__H_
#define GUIOBSERVER__H_

#include "eC_TList_doubleLinked.h"
#include "GUIValue.h"

class CGUISubject;
class CGUIObject;

/// Observer Base class for Observer-Design-Pattern within Guiliani.

/**
Use this base class if you wish to follow the observer-design-pattern within your application.
Simply derive the class that you want to act as an observer from CGUIObserver, and implement
the desired functionality within OnNotification(). This method will be called whenever an
observed subject changes.
*/
class CGUIObserver
{
    friend class CGUISubject;

public:
    /// Default constructor.
    CGUIObserver() : m_bAutoDelete(false) {}

    /// Default destructor. Notifies all subjects of destruction.
    virtual ~CGUIObserver();

    /// Called by CGUISubject whenever an observed object triggers an update.
    virtual void OnNotification() {}

    /** Called by CGUISubject whenever an observed object triggers an update.
        @param kMessage The updated string value.
    */
    virtual void OnNotification (const eC_String& kMessage) {}

    /**
        Called by a subject whenever the observed object triggers update.
        @param pkUpdatedObject The observed object (can be NULL)
    */
    virtual void OnNotification(const CGUIObject* const pkUpdatedObject) {}

    /**
        Called by a subject whenever the observed value changes.
        @param kObservedValue The updated observed value.
        @param pkUpdatedObject The observed object (can be NULL)
        @param uiX X-Index Additional X-Index in case the updated value is part of a multidimensional array
        @param uiY Y-Index Additional Y-Index in case the updated value is part of a multidimensional array
    */
    virtual void OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX=0, const eC_UInt uiY=0) {}

    /** @return The list of all subjects to which this observer is registered
    */
    const eC_TListDoubleLinked<CGUISubject*>& GetSubjectList() const
    {
        return m_kSubjectList;
    }

    /** Specifies if this observer will automatically delete itself when the number of observed subjects reaches zero.
        @param bAutoDelete If True, this observer will delete itself when RemoveSubject() is called for the last observed subject.
    */
    void SetAutoDelete(const eC_Bool bAutoDelete)
    {
        m_bAutoDelete = bAutoDelete;
    }

protected:

    /** Called on destruction of the subject to reset the internal Subject
        pointer.
        ATTENTION: Call base implementation when deriving from this method.
        @param pSubjectToRemove The Subject that shall be removed from the
         internal list.
    */
    virtual void RemoveSubject(CGUISubject* pSubjectToRemove);

    /** Called by the subject when the Observer is added.
        ATTENTION: Call base implementation when deriving from this method.
        @param pSubjectToAdd The Subject that shall be added to the internal
         list.
    */
    virtual void AddSubject(CGUISubject* pSubjectToAdd);

private:

    /** Copy-constructor. Should not be used.
    *   Dummy declaration with no implementation, just to hide the function.
        @param kSource Source object to be copied.
    */
    CGUIObserver(const CGUIObserver& kSource);

    /** Operator= method. Should not be used.
    *   Dummy declaration with no implementation, just to hide the function.
        @param kSource Source object to be copied.
        @return This object.
    */
    CGUIObserver& operator=(const CGUIObserver& kSource);

    /** Holds the pointers to the Subjects subscribed to,
        to unsubscribe the Observer on destruction.
    */
    eC_TListDoubleLinked<CGUISubject*> m_kSubjectList;

    /** Indicates if this observer will automatically delete itself when the number of observed subjects reaches zero */
    eC_Bool     m_bAutoDelete;
};

#endif
