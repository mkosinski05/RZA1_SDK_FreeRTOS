/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUISUBJECT__H_
#define GUISUBJECT__H_

#include "eC_TList_doubleLinked.h"
#include "eC_String.h"
#include "GUIValue.h"

class CGUIObserver;
class CGUIObject;

/** @defgroup GUILIANI_SUBJECTS Observable subjects in GUILIANI
    @brief List of objects within the framework which serve as Subjects that can be observed and will trigger notifications if their value/state changes.
*/

/// Subject Base class for Observer-Design-Pattern within Guiliani.

/**
Use this base class if you wish to follow the observer-design-pattern within your application.
Simply derive the class that you want to act as a subject from CGUISubject and whenever you want
to inform any potential observers about changes within your object, call NotifyObservers().
The AddObserver() and RemoveObserver() methods are used only to handle the list of objects observing
your subject and do not need to be overridden in your derived class.
*/
class CGUISubject
{
    friend class CGUIObserver;

public:

    /// Default constructor.
    CGUISubject();

    /// Default destructor.
    virtual ~CGUISubject();

    /** Adds an observer to the internal observer list.
        The observer will be notified whenever the subject triggers an update.
        @param pObserver The observer to add.
    */
    void AddObserver( CGUIObserver *pObserver );

    /** Removes a registered observer from the internal observer list.
        @param pObserver The observer to remove.
    */
    void RemoveObserver ( CGUIObserver *pObserver);

    ///Removes all observers from the internal list.
    void ClearObservers();

    /**
        Triggers notification of all registered observers.
        CGUIObserver::OnNotification() will be called for each.
    */
    void NotifyObservers ();

    /**
        Triggers notification of all registered observers.
        CGUIObserver::OnNotification(const eC_String&)
        will be called for each.
        @param kMessage Will be passed to CGUIObserver::OnNotification(const eC_String&)
    */
    void NotifyObservers (const eC_String& kMessage);

    /**
        Triggers notification of all registered observers.
        CGUIObserver::OnNotification(const CGUIObject* const)
        will be called for each.
        @param pkUpdatedObject Will be passed to CGUIObserver::OnNotification(const CGUIObject* const)
    */
    void NotifyObservers (const CGUIObject* const pkUpdatedObject);

    /**
        Triggers notification of all registered observers.
        CGUIObserver::OnNotification(const CGUIValue&, const CGUIObject* const, const eC_UInt, const eC_UInt)
        will be called for each.
        @param kObservedValue the updated value
        @param pkUpdatedObject the updated object (can be NULL)
        @param uiX X-Index of value to be set in case the updated value is part of a multidimensional array
        @param uiY Y-Index of value to be set in case the updated value is part of a multidimensional array
    */
    void NotifyObservers (const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX=0, const eC_UInt uiY=0);

    /**
        @return The list of all observers that are currently observing this subject.
    */
    const eC_TListDoubleLinked <CGUIObserver*>& GetObserverList() const
    {
        return m_pObserverList;
    }

private:

    /// Called by the observer base class to remove the Observer reference.
    void NotifyOfObserverDestruction(CGUIObserver *pObserver);

    /** Copy-constructor. Should not be used.
    *   Dummy declaration with no implementation, just to hide the function.
        @param kSource Source object to be copied.
    */
    CGUISubject(const CGUISubject& kSource);

    /** Operator= method. Should not be used.
    *   Dummy declaration with no implementation, just to hide the function.
        @param kSource Source object to be copied.
        @return This object.
    */
    CGUISubject& operator=(const CGUISubject& kSource);

    /// Holds all registered observers.
    eC_TListDoubleLinked <CGUIObserver*> m_pObserverList;
};

#endif
