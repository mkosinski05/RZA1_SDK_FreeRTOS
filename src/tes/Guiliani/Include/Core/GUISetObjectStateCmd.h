/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(SETGUIOBJECTSTATE_COMMAND_H)
#define SETGUIOBJECTSTATE_COMMAND_H

#include "GUICommand.h"
#include "GUIObjectHandleResource.h"

/** 
    The target object is referenced by an object ID that has to be passed to
    the constructor. When this command is executed, it tries to find the
    specified object. If no object is found, this command does nothing.
    If the object is found, the object states are changed according the given values.
    @brief An example for a custom command that changes the object states of an GUIObject.
  */
class CGUISetObjectStateCmd: public CGUICommand
{
public:
    /** Constructor.
        @param eTargetObject The ID of the object to be modified by this command.
        @param bToggle If true, all states marked as "True" will be toggled. Otherwise all states will be set to True/False exactly as given.
        @param bFocusable New value of the focusable object state.
        @param bInvisible New value of the invisible object state.
        @param bGrayedOut New value of the grayed out object state.
        @param bDisabled New value of the disabled object state.
      */
    CGUISetObjectStateCmd(ObjectHandle_t eTargetObject, eC_Bool bToggle, eC_Bool bFocusable, eC_Bool bInvisible, eC_Bool bGrayedOut, eC_Bool bDisabled);

    /// Default constructor to be used by a factory.
    CGUISetObjectStateCmd();

#if defined(GUILIANI_STREAM_GUI)
    /** Reads all attributes from streaming file.
        This method is called by CGUIFactoryManager after one of the registered
        factories has created an instance of this class.
    */
    virtual void ReadFromStream();
#endif

#if defined(GUILIANI_WRITE_GUI)
    /** Writes all attributes to the streaming file. A CGUIStreamWriter
        has to be initialized first.
        @param bWriteClassID This flag is used to select if writing of command
               class ID, leading and trailing tags is performed.
    */
    virtual void WriteToStream(const eC_Bool bWriteClassID=false);
#endif

protected:
    /// Implements the actual command functionality.
    void Do();

    /// Target object ID.
    ObjectHandle_t m_eTargetObject;

    /// Specifies whether the states shall be toggled on/off, or if they will be set exactly as given.
    eC_Bool m_bToggle;

    // Object states which can be changed through this command
    eC_Bool m_bFocussable; ///< Changeable state: focusable
    eC_Bool m_bInvisible; ///< Changeable state: Invisible
    eC_Bool m_bGrayedOut; ///< Changeable state: Grayed out
    eC_Bool m_bDisabled; ///< Changeable state: Disabled
};

#endif // SETGUIOBJECTSTATE_COMMAND_H
