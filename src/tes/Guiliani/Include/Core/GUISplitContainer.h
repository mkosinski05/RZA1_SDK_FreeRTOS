/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUISPLITCONTAINER__H_
#define GUISPLITCONTAINER__H_

#include "GUICompositeObject.h"
#include "GUIGeometryObject.h"
#include "GUIImageResource.h"

#include "GUIEventHandler.h"
#include "GUIBehaviourDecorator.h"
#include "GUIButton.h"

#include "eC_TList_doubleLinked.h"

class CGUISplitContainer;

/**
This class is used as the division between the two parts of a CGUISplitContainer.
It can be moved via Drag and switched from one side to the other via a DoubleClick.
*/
class CGUISplitContainerHandleBehaviour : public CGUIBehaviourDecorator
{
public:
    /**
    Specific constructor
    @param pkContainer Pointer to the container
    */
    CGUISplitContainerHandleBehaviour(CGUISplitContainer* pkContainer);

    virtual eC_Bool DoDrag(const eC_Value &vDeltaX, const eC_Value &vDeltaY, const eC_Value &vAbsX, const eC_Value &vAbsY);

    virtual eC_Bool DoDoubleClick(const eC_Value &vAbsX, const eC_Value &vAbsY);

private:
    CGUISplitContainer* m_pkContainer;
};

/**
A CGUISplitContainer can be used to create container with two or more subsections and to arrange various controls.
The dividing handle can either be horizontal or vertical. Even more complex divisions can be created by using additional CGUISplitContainer within the client-area.
*/
class CGUISplitContainer : public CGUICompositeObject
{
public:
    /** this enum is used to define the orientation of the splitterhandle
    */
    enum SplitterOrientation_t
    {
        ORIENTATION_HORIZONTAL, ///< horizontal
        ORIENTATION_VERTICAL ///< vertical
    };

    /** CGUICompositeObject standard constructor.
    @see CGUIObject().
    */
    CGUISplitContainer();

    /** CGUICompositeObject constructor.
    @param pParent Pointer to the designated parent object.
    @param vX X-position relative to its parent object
    @param vY Y-position relative to its parent object
    @param vWidth Width of the composite object
    @param vHeight Height of the composite object
    @param eID Object Identifier of this composite object (choose NO_HANDLE if none is required)
    */
    CGUISplitContainer(CGUICompositeObject *const pParent, const eC_Value &vX, const eC_Value &vY, const eC_Value &vWidth, const eC_Value &vHeight, const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUICompositeObject constructor.
    @param pParent Pointer to the designated parent object.
    @param kRect Bounding rectangle relative to parent.
    @param eID Object Identifier of this composite object (choose NO_HANDLE if none is required)
    */
    CGUISplitContainer(CGUICompositeObject *const pParent, const CGUIRect &kRect, const ObjectHandle_t &eID = NO_HANDLE);

    /// This destructor is automatically virtual, as the base class destructor is virtual
    virtual ~CGUISplitContainer();

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif // GUILIANI_STREAM_GUI

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif // GUILIANI_WRITE_GUI

    virtual void SetWidth(const eC_Value &vX);

    virtual void SetHeight(const eC_Value &vY);

    /**
    sets the position of the splitter-handle after a drag
    @param vAbsX x-position of drag
    @param vAbsY y-position of drag
    */
    void SetHandlePosition(const eC_Value &vAbsX, const eC_Value &vAbsY);

    /**
    centers the position of the handle after a doubleclick
    */
    void CenterSplitterHandle();

    /**
    sets the size of the splitter-handle
    @param vSize size to use for the splitter-handle
    */
    void SetHandleSize(const eC_Value& vSize);

    /**
    get the size of the splitter-handle
    @return size of the splitter-handle
    */
    eC_Value GetHandleSize();

    /**
    set the orientation of the splitter-handle
    @param eOrientation orientation to use for the splitter-handle
    */
    void SetHandleOrientation(const SplitterOrientation_t& eOrientation);

    /**
    get the orientation of the splitter-handle
    @return orientation of the splitter-handle
    */
    SplitterOrientation_t GetHandleOrientation();

    /**
    set the position of the splitter-handle
    @param vPosition position to use for the splitter-handle
    */
    void SetHandlePosition(const eC_Value& vPosition);

    /**
    get the position of the splitter-handle
    @return position of the splitter-handle
    */
    eC_Value GetHandlePosition();

private:
    /** Helper function for constructors. */
    void Init();

    /** adjusts the containers' position and size after something changed
    */
    void AdjustContainers();

private:
    SplitterOrientation_t m_eSplitterOrientation;   ///< orientation for splitter-handle
    eC_Value m_vSplitterPosition;                   ///< position of splitter-handle
    eC_Value m_vSplitterSize;                       ///< the size of the splitter-handle

    CGUICompositeObject* m_pkContainer1;
    CGUICompositeObject* m_pkContainer2;
    CGUIGeometryObject* m_pkSplitterHandle;
    CGUISplitContainerHandleBehaviour* m_pkHandleBehaviour;
};

#endif
