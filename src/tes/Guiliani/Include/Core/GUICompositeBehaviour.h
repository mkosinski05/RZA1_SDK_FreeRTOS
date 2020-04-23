/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined __COMPOSITEBHVR__H_
#define __COMPOSITEBHVR__H_

#include "GUIBehaviourDecorator.h"
#include "GUIDeleteNotification.h"

class CGUIEvent;

/** @brief Contains other behaviours and forwards events to them.

    If you have implemented reusable functionalities in various behaviours and
    want to attach them all to the same object, you may add instances of these
    behaviours to a CGUICompositeBehaviour and attach this one to the object.

    All event handling method calls are forwarded to the child behaviours in
    the order in which they have been added, until one of them returns true or
    the end of the list is reached. So for instance a click on an object will
    result in calls of the DoClick() methods of all attached behaviours.

    This mechanism works for all methods declared in the CGUIBehaviour class.

    Example:
@code
// Create a composite behaviour instance.
CGUICompositeBehaviour* pCompositeBhvr = new CGUICompositeBehaviour();

// Add two other behaviours to the composite.
pCompositeBhvr->AddBehaviour( new MyCustomBehaviour() );
pCompositeBhvr->AddBehaviour( new MyOtherBehaviour() );

// Create an object and attach the composite behaviour to it.
CGUIObject* pObject = new CGUIObject(...);
pObject->SetBehaviour(pCompositeBhvr);
@endcode
  */
class CGUICompositeBehaviour : public CGUIBehaviourDecorator, protected CDeleteNotification
{
public:
    /** CGUICompositeBehaviour constructor.
      */
    CGUICompositeBehaviour();

    /** CGUICompositeBehaviour destructor. Deletes all attached behaviours.
      */
    virtual ~CGUICompositeBehaviour();

    /** Adds a new behaviour to this CGUICompositeBehaviour.
        The added behaviour will be deleted on destruction of this parent behaviour.
        If the behaviour is already in the child list, this method does nothing.
        @param[in] pBehaviour Pointer the new behaviour.
        @param[in] bAtFront adds the behaviour at the front of the child
                    list (true) or at the end (false).
      */
    void AddBehaviour(CGUIBehaviourDecorator* pBehaviour, eC_Bool bAtFront = false);

    /** Removes a behaviour from this CGUICompositeBehaviour, causing the behaviour to be deleted.
        @note The behaviour will automatically delete itself if it is no longer attached to any
        CGUIObject or CGUICompositeBehaviour.
        @param[in] pBehaviour The behaviour to be removed.
      */
    void RemoveBehaviour(CGUIBehaviourDecorator* pBehaviour);

    /// Removes all behaviours from this CGUICompositeBehaviour, causing them to be deleted.
    void RemoveAllBehaviours();

    /** Decide whether the returned CGUIBehaviourDecorator is CGUICompositeBehaviour.
        Please <i>do not</i> reimplement this method in any derived class.
        It is only used in the CGUIBehaviourDecorator and CGUICompositeBehaviour
        to distinguish those two basic types.
        @return True.
    */
    virtual eC_Bool IsCompositeBehaviour() const {return true;}

    /// Creates a copy of this behaviour and all attached behaviours.
    virtual CGUIBehaviourDecorator* Clone() const;

    // All following event handling methods forward the calls to child behaviours.

    virtual eC_Bool DoClick(const eC_Value &vAbsX = eC_FromInt(-1),
                            const eC_Value &vAbsY = eC_FromInt(-1));

    virtual eC_Bool DoLongClick(const eC_Value &vAbsX = eC_FromInt(-1),
                                const eC_Value &vAbsY = eC_FromInt(-1));

    virtual eC_Bool DoDoubleClick(const eC_Value &vAbsX = eC_FromInt(-1),
                                  const eC_Value &vAbsY = eC_FromInt(-1));

    virtual eC_Bool DoButtonDown(const eC_Value &vAbsX,
                                 const eC_Value &vAbsY);

    virtual eC_Bool DoButtonUp(const eC_Value &vAbsX,
                               const eC_Value &vAbsY);

    virtual eC_Bool FocusPrevious(CGUIObject* const pRefObj = NULL,
                                  const eC_Bool &bSubTreeOfRefObjDone = false);

    virtual eC_Bool FocusNext(CGUIObject* const pRefObj = NULL,
                              const eC_Bool &bSubTreeOfRefObjDone = false);

    virtual eC_Bool Decrease();

    virtual eC_Bool Increase();

    virtual eC_Bool DoDrag(const eC_Value &vDeltaX,
                           const eC_Value &vDeltaY,
                           const eC_Value &vAbsX,
                           const eC_Value &vAbsY);

    virtual eC_Bool DoDragEnd(  const eC_Value &vAbsX,
                                const eC_Value &vAbsY);

    virtual eC_Bool DoMouseEnter(const eC_Value &vAbsX,
                                 const eC_Value &vAbsY);

    virtual eC_Bool DoMouseLeave(const eC_Value &vAbsX,
                                 const eC_Value &vAbsY);

    virtual eC_Bool DoMouseMove(const eC_Value &vAbsX,
                                const eC_Value &vAbsY);

    virtual eC_Bool DoUserEvent(CGUIEvent* const pEvent);

    virtual eC_Bool DoKeyDown(
        const GUIKeyIdentifier_t &eKeyIdentifier,
        const eC_UInt &uiModifiers);

    virtual eC_Bool DoKeyUp(
        const GUIKeyIdentifier_t &eKeyIdentifier,
        const eC_UInt &uiModifiers);

    virtual eC_Bool DoChar(
        const eC_UInt &uiKey,
        const GUIKeyIdentifier_t &eKeyIdentifier,
        const eC_UInt &uiModifiers);

    virtual eC_Bool DoScrollUp(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    virtual eC_Bool DoScrollDown(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    virtual eC_Bool DoScrollLeft(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    virtual eC_Bool DoScrollRight(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    virtual void GetFocus();

    virtual void LoseFocus();

    ///Assignment operator
    CGUICompositeBehaviour& operator=(const CGUICompositeBehaviour& rSource);

#if defined(GUILIANI_STREAM_GUI)
    virtual void ReadFromStream();
#endif

#if defined(GUILIANI_WRITE_GUI)
    virtual void WriteToStream(const eC_Bool bWriteClassID=false);
#endif

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    /// XML tag to be used when writing the number of following behaviours into a stream.
    static const eC_Char XMLTAG_NUMBER_OF_BEHAVIOURS[];
#endif

protected:
    /**
        Helper method that copies the internal list of behaviours.
        @param[in] rSource reference to the composite behaviour whose
                   behaviour list should be copied.
      */
    void CopyBehaviours(const CGUICompositeBehaviour& rSource);

    /// A Behaviour list is a double-linked list of behaviour decorator pointers
    typedef eC_TListDoubleLinked<CGUIBehaviourDecorator*> BehaviourList;

    BehaviourList m_Behaviours; ///< List of the attached behaviours.
};

#endif //__COMPOSITEBHVR__H_
