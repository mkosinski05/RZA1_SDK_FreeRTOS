/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIEXPANDINGCOMPSITE__H_
#define GUIEXPANDINGCOMPSITE__H_

#include "GUIRepositionCompositeObject.h"
#include "GUIBehaviourDecorator.h"
#include "GUICheckBox.h"

/**
This behaviour is used to process click-events on the group-heading to expand/collapse the client-area
*/
class CGUIExpandingCompositeBehaviour : public CGUIBehaviourDecorator
{
public:
    /**
    Special constructor for the group-heading
    @param bExpanded initial state of the client-area
    @param vContainerSize initial size of the client-area
    */
    CGUIExpandingCompositeBehaviour(const eC_Bool& bExpanded, const eC_Value& vContainerSize) :
        CGUIBehaviourDecorator(),
        m_bExpanded(bExpanded),
        m_vContainerSize(vContainerSize)
    {
    }

    virtual eC_Bool DoClick(const eC_Value &vAbsX = eC_FromInt(-1), const eC_Value &vAbsY = eC_FromInt(-1));

    /// Refresh the client-area after a click-event
    void Refresh(void);

    /**
    Set the size of the client-area to a specific value
    @param vContainerSize size of the client-area
    */
    void SetContainerSize(const eC_Value& vContainerSize)
    {
        // @todo: maybe not an ideal solution
        if (vContainerSize > eC_FromInt(0))
            m_vContainerSize = vContainerSize;
    }

private:
    eC_Bool m_bExpanded; ///< expanded-state
    eC_Value m_vContainerSize; ///< size of the client-area
};

/**
A CGUIExpandingComposite is a container which can expand/collapse via a click-event on its group-heading.
This can be used to save space when different groups with many child-elements are displayed.
*/
class CGUIExpandingComposite : public CGUICompositeObject
{
public:
    /** this enum is used to define the orientation of the splitterhandle
    */
    enum SplitterOrientation_t
    {
        ORIENTATION_HORIZONTAL,
        ORIENTATION_VERTICAL
    };

    /** CGUICompositeObject standard constructor.
    @see CGUIObject().
    */
    CGUIExpandingComposite();

    /** CGUICompositeObject constructor.
    @param pParent Pointer to the designated parent object.
    @param vX X-position relative to its parent object
    @param vY Y-position relative to its parent object
    @param vWidth Width of the composite object
    @param vHeight Height of the composite object
    @param kHeadline Text to be displayed in the group-heading
    @param eID Object Identifier of this composite object (choose NO_HANDLE if none is required)
    */
    CGUIExpandingComposite(CGUICompositeObject *const pParent, const eC_Value &vX, const eC_Value &vY, const eC_Value &vWidth, const eC_Value &vHeight, const eC_String& kHeadline = "Headline", const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUICompositeObject constructor.
    @param pParent Pointer to the designated parent object.
    @param kRect Bounding rectangle relative to parent.
    @param kHeadline Text to be displayed in the group-heading
    @param eID Object Identifier of this composite object (choose NO_HANDLE if none is required)
    */
    CGUIExpandingComposite(CGUICompositeObject *const pParent, const CGUIRect &kRect, const eC_String& kHeadline = "Headline", const ObjectHandle_t &eID = NO_HANDLE);

    /**
    Add a new object to the container. This method is overridden, since new objects should be inserted into the client-area
    @param pkObject object to add
    @return true if successful, else false
    */
    eC_Bool AddObject(CGUIObject* pkObject);

    /// This destructor is automatically virtual, as the base class destructor is virtual
    virtual ~CGUIExpandingComposite();

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif // GUILIANI_STREAM_GUI

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif // GUILIANI_WRITE_GUI

    virtual void SetWidth(const eC_Value &vX);

    virtual void SetHeight(const eC_Value &vY);

    /**
    Get the group-heading (CGUICheckBox) to modify attributes
    @return pointer to the group-heading
    */
    CGUICheckBox* GetExpandButton() const;

private:
    /** Helper function for constructors. */
    void Init();

    /**
    Update internal elements which is the CGUICheckBox for the group-heading and the CGUICompositeObject for the client-area-container
    */
    void UpdateElements();

    eC_String m_kHeadline;

    CGUICheckBox* m_pkExpandButton;
    CGUIRepositionCompositeObject* m_pkContainer;
    CGUIExpandingCompositeBehaviour* m_pkExpandBehaviour;

    eC_Value m_vContainerHeight;
};

#endif
