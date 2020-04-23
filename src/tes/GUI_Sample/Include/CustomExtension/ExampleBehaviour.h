/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(EXAMPLE_BEHAVIOUR_H)
#define EXAMPLE_BEHAVIOUR_H

#include "GUIBehaviourDecorator.h"

/** Example for a custom behaviour that allows for dragging or resizing its
    associated object with the mouse.
  */
class ExampleBehaviour: public CGUIBehaviourDecorator
{
public:
    enum DragAction
    {
        DA_MOVE, ///< Moves the object around when dragging with the mouse.
        DA_SIZE  ///< Changes the object's size when dragging with the mouse.
    };

    ExampleBehaviour(DragAction eAction, CGUIObject* const pObject = NULL);

    /// Default constructor to be used by the factory.
    ExampleBehaviour();

    /** Re-implementation of the CGUIBehaviour interface for mouse drag events.
        This implementation either moves or resizes the associated object,
        based on the DragAction value passed to the constructor.
      */
    virtual eC_Bool DoDrag(
        const eC_Value &vDeltaX,
        const eC_Value &vDeltaY,
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

#if defined(GUILIANI_STREAM_GUI)
    /** Reads all behaviour attributes from streaming file.
        This method is called by CGUIFactoryManager after one of the registered
        factories has created an instance of this class.
    */
    virtual void ReadFromStream();
#endif

#if defined(GUILIANI_WRITE_GUI)
    /** Writes all behaviour attributes to the streaming file. A CGUIStreamWriter
        has to be initialized first.
        @param bWriteClassID This flag is used to select if writing of ControlID,
               leading and trailing tags is performed.
    */
    virtual void WriteToStream(const eC_Bool bWriteClassID=false);
#endif

protected:
    DragAction m_eDragAction;
};

#endif // EXAMPLE_BEHAVIOUR_H
