/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIMULTICMDBEHAVIOUR__H_
#define GUIMULTICMDBEHAVIOUR__H_

#include "GUISingleCmdBehaviour.h"
#include "GUICommand.h"


/// Associates one command with each event slot.

/**
    This class gives the possibility to execute a separate command for each
    event (e.g. click event, drag event, etc.).
*/
class CGUIMultiCmdBehaviour
    : public CGUICmdBehaviour
{
public:
    /**
        Constructor.
    */
    CGUIMultiCmdBehaviour();

    /**
        Destructor.
    */
    virtual ~CGUIMultiCmdBehaviour() {}

    virtual CGUIBehaviourDecorator* Clone() const;

    /**
        Sets the command for a specific behaviour.
        @param pCommand The command to execute.
        @param eBehaviourType The behaviour type.
    */
    void SetCmdForBehaviour(CGUICommandPtr pCommand,
        const BehaviourType_t& eBehaviourType);

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
        Executes the command for the given behaviour, if there is one
        set.
        @param eFromBehaviour Behaviour slot from which this method was called.
    */
    virtual void ExecuteCommand(const BehaviourType_t& eFromBehaviour);

private:
    /** Copy-constructor.
        @param kSource Source object to be copied.
    */
    CGUIMultiCmdBehaviour(const CGUIMultiCmdBehaviour& kSource);

    /** Operator= method.
        @param kSource Source object to be copied.
        @return This instance.
    */
    CGUIMultiCmdBehaviour& operator=(const CGUIMultiCmdBehaviour& kSource);

    /// The array of commands. The commands are mapped in the following way:
    /// m_apCommands[BehaviourType_t]. That means, for example, that you can
    /// access the command for dragging that way: m_apCommands[BT_DRAG].
    eC_TArray<CGUICommandPtr> m_apCommands;
};
#endif
