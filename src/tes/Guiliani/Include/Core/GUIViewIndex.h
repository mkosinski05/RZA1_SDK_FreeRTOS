/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined GUI_VIEW_INDEX_H_
#define GUI_VIEW_INDEX_H_

#include "eC_Types.h"

/// The index of an item in a table view.
/** The index consists of a column index and a row index.
  */
class CGUIViewIndex
{
public:
    /// Constructs an invalid view index.
    CGUIViewIndex()
        : m_uiColumn(0xffffffff),
        m_uiRow (0xffffffff) {}

    /** Constructs a view index for a specific item position.
        @param uiColumn The view column index.
        @param uiRow    The view row index.
      */
    CGUIViewIndex( eC_UInt uiColumn, eC_UInt uiRow)
        : m_uiColumn(uiColumn),
        m_uiRow (uiRow) {}

    /** @return The column index. */
    eC_UInt GetColumn() const { return m_uiColumn; }

    /** @return The row index. */
    eC_UInt GetRow() const { return m_uiRow; }

    /** Setter for row
    @param uiRow row to set
    */
    void SetRow(eC_UInt uiRow ) 
    { 
        m_uiRow = uiRow; 
    }

    /** Setter for column
    @param uiColumn column to set
    */
    void SetColumn(eC_UInt uiColumn) 
    {
        m_uiColumn = uiColumn;
    }

    /** Setter for row and column
    @param uiRow Row to set
    @param uiColumn column to set
    */
    void Set(eC_UInt uiRow, eC_UInt uiColumn) 
    {
        m_uiRow = uiRow; 
        m_uiColumn = uiColumn;
    }

    /** Comparison operator.
        @param rkIndex Other index to compare this one to.
        @return True if both column and row index are the same, otherwise False.
      */
    eC_Bool operator==(const CGUIViewIndex& rkIndex) const
    {
        return ((rkIndex.GetColumn() == m_uiColumn) && (rkIndex.GetRow() == m_uiRow));
    }

    /** Comparison operator.
        @param rkIndex Other index to compare this one to.
        @return True if row before the > symbol is greater than the row of the index passed as parameter.
        In the event of rows being the same number, compare the column.
    */
    eC_Bool operator>(const CGUIViewIndex& rkIndex) const
    {
        return ((m_uiRow > rkIndex.GetRow()) || ((m_uiRow == rkIndex.GetRow())&&(m_uiColumn > rkIndex.GetColumn())));
    }

    /** @return True if this index is valid. This method does <i>not</i> check
                the actual index against the actual view.
    */
    eC_Bool IsValid() const
    {
        return ((m_uiColumn != 0xffffffff) && (m_uiRow != 0xffffffff));
    }

private:

    eC_UInt m_uiColumn; ///< The column index.
    eC_UInt m_uiRow;    ///< The row index.
};

#endif
