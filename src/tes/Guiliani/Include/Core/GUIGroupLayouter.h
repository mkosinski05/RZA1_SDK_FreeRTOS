/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_GROUP_LAYOUTER__H_
#define GUI_GROUP_LAYOUTER__H_

#include "GUILayouter.h"

class CGUICompositeObject;

/// This is the group layouter base class
///@ingroup GUILIANI_LAYOUTER
class CGUIGroupLayouter
    : public CGUILayouter
{
public:

    /// Default constuctor.
    CGUIGroupLayouter();

    /** Constructs a Group-Layouter.
        @param pObject Pointer to the object to which the layouter is attached.
        @param bTakeOverParentLayout True if LayoutAction() shall take over the
            size of the associated object's parent to the associated object and set
            the x and y position of the associated to zero.
    */
    CGUIGroupLayouter(
        CGUICompositeObject *pObject,
        eC_Bool bTakeOverParentLayout);

    /// Destructor a Reposition-Layouter.
    virtual ~CGUIGroupLayouter() {}

    virtual eC_Bool IsDependentOnParentSize() {return m_bTakeOverParentLayout;}

    virtual void SetAssociatedObject(CGUIObject * const pObject );

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    static const eC_UInt GROUP_LAYOUTER_CLASS_VERSION;///< Class version of group layouter class
#endif

protected:
    /** LayoutAction() takes over the size of the associated object's parent
        to the associated object and sets the x and y position of the associated to zero.
        @return True if successful, FALSE if not. If FALSE no layout changed.
    */
    eC_Bool LayoutAction();

private:
    eC_Bool m_bTakeOverParentLayout;
    void AssertAssociatedObjectValid(const CGUIObject * const pObject);
};

#endif
