/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUISINGLECMDBEHAVIOUR__H_
#define GUISINGLECMDBEHAVIOUR__H_

#include "GUIBehaviourDecorator.h"
#include "GUICommand.h"


/** Base class which Associates a command with one event slot. 
The event will be forwarded to the associated object.
@see CGUISingleCmdBehaviour, CGUIMultiCmdBehaviour
*/

class CGUICmdBehaviour
    : public CGUIBehaviourDecorator
{
protected:
    /**
        Constructor.
    */
    CGUICmdBehaviour() {}

    /**
        Destructor.
    */
    virtual ~CGUICmdBehaviour() {}

    virtual eC_Bool DoClick(
        const eC_Value &vAbsX = eC_FromInt(-1),
        const eC_Value &vAbsY = eC_FromInt(-1));

    virtual eC_Bool DoLongClick(
        const eC_Value &vAbsX = eC_FromInt(-1),
        const eC_Value &vAbsY = eC_FromInt(-1));

    virtual eC_Bool DoDoubleClick(
        const eC_Value &vAbsX = eC_FromInt(-1),
        const eC_Value &vAbsY = eC_FromInt(-1));

    virtual eC_Bool DoButtonDown(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    virtual eC_Bool DoButtonUp(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    virtual eC_Bool FocusPrevious(
        CGUIObject* const pRefObj = NULL,
        const eC_Bool &bSubTreeOfRefObjDone = false);

    virtual eC_Bool FocusNext(
        CGUIObject* const pRefObj = NULL,
        const eC_Bool &bSubTreeOfRefObjDone = false);

    virtual eC_Bool Decrease();

    virtual eC_Bool Increase();

    virtual eC_Bool DoDrag(
        const eC_Value &vDeltaX,
        const eC_Value &vDeltaY,
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    virtual eC_Bool DoDragEnd(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    virtual eC_Bool DoMouseEnter(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    virtual eC_Bool DoMouseLeave(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    virtual eC_Bool DoScrollUp(const eC_Value& vAbsX, const eC_Value& vAbsY);
    virtual eC_Bool DoScrollDown(const eC_Value& vAbsX, const eC_Value& vAbsY);
    virtual eC_Bool DoScrollLeft(const eC_Value& vAbsX, const eC_Value& vAbsY);
    virtual eC_Bool DoScrollRight(const eC_Value& vAbsX, const eC_Value& vAbsY);

    virtual eC_Bool DoKeyDown(
        const GUIKeyIdentifier_t& eKeyID,
        const eC_UInt& uiModifiers);

    virtual eC_Bool DoKeyUp(
        const GUIKeyIdentifier_t& eKeyID,
        const eC_UInt& uiModifiers);

    virtual eC_Bool DoChar(
        const eC_UInt& uiKey,
        const GUIKeyIdentifier_t& eKeyID,
        const eC_UInt& uiModifiers);

    virtual void GetFocus();

    virtual void LoseFocus();

    /**
        @brief Executes the command for the given behaviour, if it is equal to the stored one.
        @param eFromBehaviour Behaviour slot from which this method was called.
    */
    virtual void ExecuteCommand(const BehaviourType_t& eFromBehaviour) = 0;
};


/**
    This class gives the possibility to execute a command when a specific
    event (e.g. click event, drag event, etc.) occurs.
    @brief Execute a command when a specific event occurs.
*/
class CGUISingleCmdBehaviour
    : public CGUICmdBehaviour
{
public:
    /**
        Constructor.
    */
    CGUISingleCmdBehaviour();

    /**
        Destructor.
    */
    virtual ~CGUISingleCmdBehaviour() {}

    /**
        Sets the command for a specific behaviour.
        @param pCommand The command to execute.
        @param eBehaviourType The behaviour type.
    */
    void SetCmdForBehaviour(CGUICommandPtr pCommand,
        const BehaviourType_t& eBehaviourType);

    virtual CGUIBehaviourDecorator* Clone() const;

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID=false);
#endif

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    /// XML tag to be used when writing a behaviour type into a stream.
    static const eC_Char XMLTAG_BEHAVIOURTYPE[];
#endif

protected:
    /**
        Executes the command for the given behaviour, if it is equal
        to the stored one.
        @param eFromBehaviour Behaviour slot from which this method was called.
    */
    virtual void ExecuteCommand(const BehaviourType_t& eFromBehaviour);

private:
    /** Copy-constructor.
        @param kSource Source object to be copied.
    */
    CGUISingleCmdBehaviour(const CGUISingleCmdBehaviour& kSource);

    /** Operator= method.
        @param kSource Source object to be copied.
        @return This instance.
    */
    CGUISingleCmdBehaviour& operator=(const CGUISingleCmdBehaviour& kSource);

    /// Stores the behaviour type.
    BehaviourType_t m_eBehaviourType;

    /// Stores the command for the behaviour;
    CGUICommandPtr m_pCommand;
};
#endif
