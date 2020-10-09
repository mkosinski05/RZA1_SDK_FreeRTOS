/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_LAYOUTER_LIST__H_
#define GUI_LAYOUTER_LIST__H_

#include "GUIGroupLayouter.h"
#include "GUILayoutHelper.h"

/** This is a layouter, which implements repositioning of the children of
    the associated object in a list according to the size of the associated object.
    If a line or row is completely filled it wraps to another one.
    @brief Layouter for list arangement, resizes to asscoiated object
    @ingroup GUILIANI_LAYOUTER
    */
class CGUILayouterList
    : public CGUIGroupLayouter
{
public:

    /** Constructs a List-Layouter.
        @param pObject Pointer to the object to which the layouter is attached.
        @param eBasePoint The base point on which the list positioning is based.
        @param bTakeOverParentLayout True if LayoutAction() shall take over the
            size of the associated object's parent to the associated object and set
            the x and y position of the associated to zero.
    */
    CGUILayouterList(
        CGUICompositeObject *pObject,
        CGUILayoutHelper::ListAlignment_t eBasePoint,
        eC_Bool bTakeOverParentLayout);

    /// Default Constuctor.
    CGUILayouterList();

    /// Destructor a List-Layouter.
    virtual ~CGUILayouterList() {}

    /** Repositions child objects and resizes associated object.
        @param eMovedEdges Reason for layouter call
    */
    virtual void DoLayout(eMovedEdges_t eMovedEdges);

    virtual void InitLayouter(eMovedEdges_t eMovedEdges) {}

    /// Set 'BasePoint' repositioning parameter, see CGUILayoutHelper::AdjustListArrangement().
    void SetBasePoint(const CGUILayoutHelper::ListAlignment_t eBasePoint) { m_eBasePoint = eBasePoint; }

    /// Get 'BasePoint' repositioning parameter, see CGUILayoutHelper::AdjustListArrangement().
    eC_UInt GetBasePoint() const { return m_eBasePoint; }

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    static const eC_UInt LAYOUTER_LIST_CLASS_VERSION;///< Class version of list layouter class
#endif

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

private:
    void LayoutAction();

    CGUILayoutHelper::ListAlignment_t m_eBasePoint;
};

#endif
