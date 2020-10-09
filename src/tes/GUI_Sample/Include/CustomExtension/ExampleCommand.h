/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(EXAMPLE_COMMAND_H)
#define EXAMPLE_COMMAND_H

#include "GUICommand.h"
#include "GUIObjectHandleResource.h"

/** An example for a custom command that changes the size and position of an
    object.

    The target object is referenced by an object ID that has to be passed to
    the constructor. When this command is executed, it tries to find the
    specified object. If no object is found, this command does nothing.
    If the object is found, each of its edges is moved by an amount of pixels
    that is also passed to the constructor.
  */
class ExampleCommand: public CGUICommand
{
public:
    /** Constructor.
        @param eTargetObject The ID of the object to be modified by this
               command.
        @param vStepSize The number of (sub) pixels that the target object's
               boundary rectangle is expanded by. See CGUIRect::Expand().
      */
    ExampleCommand(ObjectHandle_t eTargetObject, eC_Value vStepSize);

    /// Default constructor to be used by a factory.
    ExampleCommand();

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

    /// Number of pixels to expand the target object's boundary rectangle.
    eC_Value m_vStepSize;
};

#endif // EXAMPLE_COMMAND_H
