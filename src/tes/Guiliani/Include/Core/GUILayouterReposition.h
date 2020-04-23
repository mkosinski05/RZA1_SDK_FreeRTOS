/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_LAYOUTER_REPOSITION__H_
#define GUI_LAYOUTER_REPOSITION__H_

#include "GUIGroupLayouter.h"
#include "GUILayoutHelper.h"

/// This is a layouter, which implements repositioning of the children like CGUIRepositionCompositeObject does.
///@ingroup GUILIANI_LAYOUTER
class CGUILayouterReposition
    : public CGUIGroupLayouter
{
public:

    /** Constructs a Reposition-Layouter.
        @param pObject Pointer to the object to which the layouter is attached.
        @param eBasePoint The base point on which the repositioning is based.
        @param vGap The gap between the children.
        @param bTakeOverParentLayout True if LayoutAction() shall take over the
         size of the associated object's parent to the associated object and set
         the x and y position of the associated to zero.
    */
    CGUILayouterReposition(
        CGUICompositeObject * const pObject = NULL,
        const CGUILayoutHelper::RepositionAlignment_t &eBasePoint = CGUILayoutHelper::REPOSITION_ALIGN_INVALID,
        const eC_Value &vGap = eC_FromInt(0),
        eC_Bool bTakeOverParentLayout = false);

    /// Destructor a Reposition-Layouter.
    virtual ~CGUILayouterReposition() {}

    /** Repositions child objects and resizes associated object.
        @param eMovedEdges Reason for layouter call
    */
    virtual void DoLayout(eMovedEdges_t eMovedEdges);

    virtual void InitLayouter(eMovedEdges_t eMovedEdges) {}

    /// Sets BasePoint on which repositioning is based.
    void SetBasePoint( CGUILayoutHelper::RepositionAlignment_t eNewBasePoint) { m_eBasePoint = eNewBasePoint; }

    /// Set Border spacing
    void SetBorder( const eC_Value vBorder) { m_vBorder = vBorder; }

    /// Set Gap between child objects
    void SetGap( const eC_Value vGap) { m_vGap = vGap; }

    /// Get Gap between child objects
    eC_Value GetGap() const { return m_vGap; }

    /// Specifies if the associated object shall automatically be resized to be exactly large enough to contain its children
    void SetResizeObject( const eC_Bool bResize) { m_bResizeObject = bResize; }


#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    static const eC_UInt LAYOUTER_REPOSITION_CLASS_VERSION;///< Class version of reposition layouter class
#endif

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

private:
    void LayoutAction();

    ///Base point mode
    CGUILayoutHelper::RepositionAlignment_t m_eBasePoint;
    ///Gap between two children.
    eC_Value m_vGap;
    ///Space between top/left border and first child object, and right/bottom border and last child object
    eC_Value m_vBorder;
    /// Indicates if the associated object shall automatically be resized to be exactly large enough to contain its children
    eC_Bool m_bResizeObject;
};
#endif
