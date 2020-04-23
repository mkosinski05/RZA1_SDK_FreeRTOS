/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUIMODELINDEX_H)
#define GUIMODELINDEX_H

#include "eC_Types.h"

class CGUITableModel;

/// Index into a CGUITableModel.
/** Provides a unified interface for communicating between model and view.
Instances of this class can be used to make sure an existing index is
referenced in a model. A view also uses this to check that an index indeed
points to the model it is currently displaying.

Treat instances of this class as temporary. Do not keep them around for
longer times as they may become invalid due to changes to the model,
instead pass them around by copy and discard them when no longer needed.

Do not construct instances of this class in user code. Call
CGUITableModel::GetIndex to obtain an instance.
*/
class CGUIModelIndex
{
public:

    /** The default constructor generates an invalid model index and has to be initialized correctly.
    */
    CGUIModelIndex()
        : m_pkModel(NULL), m_uiRow(0), m_uiColumn(0)
    {}

    /** Constructs a model index.
    @param pkModel The model into which this index points.
    @param uiRow Model row index.
    @param uiColumn Model column index.
    */
    CGUIModelIndex(CGUITableModel* pkModel, eC_UInt uiRow, eC_UInt uiColumn)
        : m_pkModel(pkModel), m_uiRow(uiRow), m_uiColumn(uiColumn)
    {}

    /** Copy constructor.
    @param rkSource source instance.
    */
    CGUIModelIndex(const CGUIModelIndex& rkSource)
        : m_pkModel(rkSource.m_pkModel), m_uiRow(rkSource.m_uiRow),
        m_uiColumn(rkSource.m_uiColumn)
    {}

    /// @return the row index.
    inline eC_UInt GetRow() const { return m_uiRow; }
    /// @return the column index.
    inline eC_UInt GetColumn() const { return m_uiColumn; }
    /// @return The model that this index points to.
    inline CGUITableModel* const GetModel() const { return m_pkModel; }

    /** Checks validity of the index.
    @return True if row and column indices are valid for the model that
    this index points to, otherwise False.
    */
    eC_Bool IsValid() const;

    /** Assignment operator
        @param kModelIndex Where to copy from
        @return Where to assign to
    */
    CGUIModelIndex& operator=(const CGUIModelIndex& kModelIndex)
    {
        m_pkModel = kModelIndex.GetModel();
        m_uiColumn = kModelIndex.GetColumn();
        m_uiRow = kModelIndex.GetRow();
        return *this;
    }

    /** Comparison operator
        @param kModelIndex What to compare with
        @return True if identical, otherwise False.
    */
    eC_Bool operator ==(const CGUIModelIndex& kModelIndex)
    {
        return (
               (m_pkModel == kModelIndex.GetModel())
            && (m_uiColumn == kModelIndex.GetColumn())
            && (m_uiRow == kModelIndex.GetRow())
            );
    }

private:
    CGUITableModel* m_pkModel;
    eC_UInt m_uiRow;
    eC_UInt m_uiColumn;
};

#endif // GUIMODELINDEX_H
