/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_LAYOUTER_GRID__H_
#define GUI_LAYOUTER_GRID__H_

#include "GUIGroupLayouter.h"

/** This is a layouter which implements repositioning of the children of
    the associated object in a grid according to the size of the associated object.
    @brief Layouter for size-dependant grid
    @ingroup GUILIANI_LAYOUTER
    */
class CGUILayouterGrid
    : public CGUIGroupLayouter
{
public:

    /** Constructs a Grid-Layouter.
        @param pObject Pointer to the object to which the layouter is attached.
        @param uiRow The number of rows.
        @param uiColumn The number of columns.
        @param bMakeSameSize Indicate whether the children change their
            dimensions to be the same as the grid or not.
        @param bTakeOverParentLayout True if LayoutAction() shall take over the
            size of the associated object's parent to the associated object and set
            the x and y position of the associated to zero.
    */
    CGUILayouterGrid(
        CGUICompositeObject * const pObject,
        eC_UInt uiRow,
        eC_UInt uiColumn,
        eC_Bool bMakeSameSize,
        eC_Bool bTakeOverParentLayout);

    CGUILayouterGrid();

    /// Destructor a Reposition-Layouter.
    virtual ~CGUILayouterGrid() {}

    /** Repositions child objects and resizes associated object.
        @param eMovedEdges Reason for layouter call
    */
    virtual void DoLayout(eMovedEdges_t eMovedEdges);

    virtual void InitLayouter(eMovedEdges_t eMovedEdges) {}

    /// Set 'row' repositioning parameter, see CGUILayoutHelper::AlignToGrid().
    void SetRow(const eC_UInt uiRow) { m_uiRow = uiRow; }

    /// Set 'column' repositioning parameter, see CGUILayoutHelper::AlignToGrid().
    void SetColumn(const eC_UInt uiColumn) { m_uiColumn = uiColumn; }

    /// Set 'MakeSameSize' repositioning parameter, see CGUILayoutHelper::AlignToGrid().
    void SetMakeSameSize(const eC_Bool bMakeSameSize) { m_bMakeSameSize = bMakeSameSize; }

    /// Get 'row' repositioning parameter, see CGUILayoutHelper::AlignToGrid().
    eC_UInt GetRow() const { return m_uiRow; }

    /// Get 'column' repositioning parameter, see CGUILayoutHelper::AlignToGrid().
    eC_UInt GetColumn() const { return m_uiColumn; }

    /// Get 'MakeSameSize' repositioning parameter, see CGUILayoutHelper::AlignToGrid().
    eC_Bool GetMakeSameSize() const { return m_bMakeSameSize; }

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    static const eC_UInt LAYOUTER_GRID_CLASS_VERSION;///< Class version of grid layouter class
#endif

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

private:
    void LayoutAction();

    eC_UInt m_uiRow;
    eC_UInt m_uiColumn;
    eC_Bool m_bMakeSameSize;
};

#endif
