/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIBEHAVIOURDECORATOR__H_
#define GUIBEHAVIOURDECORATOR__H_

#include "GUIBehaviour.h"
#include "GUIObject.h"

class CGUICompositeBehaviour;

/// GUIBehaviourDecorator base class.

/**
    Behavioural decorators enable the developer to implement standard object behaviours which are not
    integral part of a specific control, but can in fact be appended to any CGUIObject. A behaviour's event
    handling slots are always called before those of its corresponding CGUIObject. You can thus
    override the object's reaction to a given event without having the need to derive a whole new
    class for this task. As always, a method's return value indicates whether the corresponding
    object's event-handling method will still be called (=false) or the event was already
    completed and no further treatment is required (=true).

    The following example shows a simple behaviour decorator which prints out a debug message whenever the
    GUIObject to which it is attached is being clicked. Note that the DoClick()-implementation returns false,
    which means that the click will still be forwarded to the corresponding GUIObject afterwards.

    You can even attach such a behaviour-decorator to objects which do otherwise not implement any standard
    behaviour. For instance, you could attach the example behaviour to a CGUIImage-object, let it receive
    events by calling pPointerToImageObject->SetDisabled(false), and the behaviour will print out a
    debug-message upon every click on the corresponding GUIImage:
@code
class CExampleBehaviour
    : public CGUIBehaviourDecorator
{
public:
    eC_Bool DoClick(const eC_Value &vAbsX, const eC_Value &vAbsY)
    {
        GUILOG( GUI_TRACE_DEBUG, "some incredibly impressive clicking behaviour \n");
        return false;
    }
};
@endcode

    The following line shows how to attach a BehaviourDecorator to an object:
@code
    pPointerToObject->SetBehaviour( new CExampleBehaviour() );
@endcode

@section sec_behaviour_decorator_ownership Ownership
    Note that there is always a one-on-one relationship between a CGUIBehaviourDecorator and its associated CGUIObject OR its parent
    CGUICompositeBehaviour. That means if you first attach your behaviour to an object within the GUI, and then add it as a child-object to
    a composite-behaviour, it will be <b>removed</b> from the CGUIObject.

    Note also that a behaviour <b>will automatically delete itself</b> if nobody (i.e. neither any CGUIObject, nor any CGUICompositeBehaviour)
    is still using it!

    The following code demonstrates the ownership philosophy
@code
    // Create a behaviour decorator
    CGUIBehaviourDecorator* pDemoBhv = new CGUISingleCmdBehaviour();
    // Create an object
    CGUIObject* pObject = new CGUIObject( &GETGUI, 0,0,100,10);
    // Assign the behaviour to the object
    pObject->SetBehaviour( pDemoBhv);
    // Now we delete the object ... This deletes the behaviour as well!
    delete pObject;
@endcode

*/

class CGUIBehaviourDecorator
    : public CGUIBehaviour
{
    friend class CGUICompositeBehaviour;
    friend void CGUIObject::SetBehaviour(CGUIBehaviourDecorator* pBehaviour);
    friend CGUIObject::~CGUIObject();
#ifdef GUILIANI_STREAM_GUI
    friend void CGUIObject::ReadFromStream();
#endif

public:
    /** Creates an exact copy of this behaviour. Should be re-implemented by
        each derived class to return the correct type of behaviour.
        @return the created copy.
      */
    virtual CGUIBehaviourDecorator* Clone() const;

    /** Destructor. The Behaviour will unregister itself from its associated object and/or composite behaviour.
    */
    virtual ~CGUIBehaviourDecorator();

    /** Indicates whether this is a CGUICompositeBehaviour.
        Please <i>do not</i> reimplement this method in any derived class.
        It is only used in the CGUIBehaviourDecorator and CGUICompositeBehaviour
        to distinguish those two basic types.
        @return False.
    */
    virtual eC_Bool IsCompositeBehaviour() const {return false;}

protected:
    /** Constructor. If the given object parameter is not NULL, it calls the SetBehaviour
        method of the object.
        @param pObject The object that contains this behaviour.
    */
    CGUIBehaviourDecorator(CGUIObject * const pObject = NULL);

    /** Gets the object that contains this behaviour. If this behaviour is a child
        of a CGUICompositeBehaviour, the parent's associated object is returned.
        @return pObject The object that contains this behaviour.
    */
    CGUIObject* GetAssociatedObject() const;

    /** This slot is being called whenever the associated object pointer is changing.
        The internal m_pObject pointer will still point to the "old" associated object, while
        the supplied parameter points to the "new" one.
        @param pNewAssociatedObject Pointer to new associated object */
    virtual void AssociatedObjectChanges(CGUIObject * const pNewAssociatedObject);

    /** This is the empty implementation of the copy-constructor. All attributes will be initialized with NULL.
        @param kSource Source behaviour. */
    CGUIBehaviourDecorator(const CGUIBehaviourDecorator& kSource);

private:
    /** This prototype is declared only to prevent auto-generation by the compiler.
        There is no implementation for this method. */
    CGUIBehaviourDecorator& operator=(const CGUIBehaviourDecorator& kSource);

    /// Set the associated object
    void SetObject(CGUIObject * const pObject);

    /** Sets this behaviour's parent behaviour.
        @pParent The behaviour of which this one will become a child.
      */
    void SetParentBehaviour(CGUICompositeBehaviour* pParent);

    /// Pointer to associated object
    CGUIObject *m_pObject;

    /** The current parent behaviour. NULL if this behaviour is not a child of
        any CGUICompositeBehaviour.
      */
    CGUICompositeBehaviour* m_pParent;
};

#endif
