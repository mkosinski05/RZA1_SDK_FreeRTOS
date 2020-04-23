/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUITABLEMODEL_H)
#define GUITABLEMODEL_H

#include "eC_TList_doubleLinked.h"
#include "eC_String.h"
#include "GUIModelIndex.h"
#include "GUIModelItem.h"
#include "GUITextResource.h"

class CGUITableView;

/// Stores data that can be displayed in a CGUITableView.
/** Objects of this class can be used to dynamically add and remove data at
    runtime.
    Instances of CGUITableView show the data in a model.
    Multiple views may show the same data.
    To attach a model to a view, call CGUITableView::SetModel.
    Whenever the model is changed, all views that show this model's data are
    notified to allow changing the visual representation.

    Data is organized in the form of rows, each of which has the same number of
    columns. To insert data in the model, use the following calls:
    - Call AddColumn() as often as needed to set up the layout of your table.
    - Repeatedly call AppendRow() or InsertRow() to create a new row and use
    SetItem to set individual data items.

    Items are owned by the model. The model takes care of deleting items that
    are removed or not used anymore.

    Indices into the table rows and columns are zero-based.

    @remarks The interface and functionality of this class should be considered
             preliminary and may be changed in the future.
  */
class CGUITableModel
{
    friend class CGUITreeView; // For performance-optimized direct access

    /// Helper class for managing the items in one table row.
    class CGUIModelRow
    {
        friend class CGUITableModel; // grant direct access only to the TableModel
    public:

        /** Gets the hierarchy depth of this row
            @return Hierarchy depth (0 for root objects, 1 for their children, etc...) */
        eC_UInt GetHierarchyDepth() {return m_uiHierarchyDepth;}

        /** Returns the item at the given index (column).
            @param uiIndex Column index.
            @return The uiIndex-th item in this row.
          */
        const CGUIModelItem& GetItem(eC_UInt uiIndex) const;

    protected:
        /** Constructs a model row with the given width (number of elements).
            Instances of CGUIModelItem are used for each item.
            @param uiNOFColumns The width of the row (number of columns, number of
                   items).
          */
        CGUIModelRow(eC_UInt uiNOFColumns);

        /// Destructor. Deletes all items in this row.
        ~CGUIModelRow();

        /** Resizes the row to the given number of columns
             @param uiNOFColumns New number of columns in this row */
        void SetNumberOfColumns(const eC_UInt uiNOFColumns);

        /** Sets a data item. The item occupying the given location is deleted.
            @param uiIndex Column index of the item.
            @param pkItem The data item to be set.
          */
        void SetItem(eC_UInt uiIndex, CGUIModelItem* pkItem);

        /** Sets the given value for the item in the column defined by uiIndex
            @param uiIndex Index of column
            @param rkNewValue New value to set for this column
            @return True if the item was updated, False if rkNewValue was equal to the current value.
        */
        eC_Bool UpdateItem(eC_UInt uiIndex, const CGUIValue& rkNewValue);

        /** Sets the hierarchy depth of this row
            @param uiDepth New hierarchy depth of the row. */
        void SetHierarchyDepth(eC_UInt uiDepth) {m_uiHierarchyDepth = uiDepth;}

    private:
        /// Do not allow default construction.
        CGUIModelRow();
        /// Do not allow copy construction.
        CGUIModelRow(const CGUIModelRow&);
        /// Do not allow assignment.
        CGUIModelRow& operator=(const CGUIModelRow&);

        /// Type of the array for storing items.
        typedef eC_TArray<CGUIModelItem*> ItemArray;
        /// The column array (items in this row).
        ItemArray m_kItems;

        /** Hierarchical depth of the row. (0 for root objects, 1 for their children, etc...)
            Example:
                ObjectA             (depth=0)
                    ObjectA1        (depth=1)
                    ObjectA2        (depth=1)
                        ObjectA21   (depth=2)
                ObjectB             (depth=0)
                    ObjectB1        (depth=1)
            This is only relevant within TreeViews and ignored when used within TableViews */
        eC_UInt m_uiHierarchyDepth;
    };

public:
    ///Typedef of a model row pointer
    typedef const CGUITableModel::CGUIModelRow* ModelRowPtr;

    /// Type of the list for storing view pointers.
    typedef eC_TListDoubleLinked<CGUITableView*> ViewList;

    /// Constructs an empty model.
    CGUITableModel();

    /// Destructor. Deletes all data items.
    virtual ~CGUITableModel();

    /** Adds a column. The new column is appended to the end of the column
        list, which means that after the method returns, the new column has the
        index GetColumnCount()-1.
        @param rkColName The name of the column.
        @see AddColumn(TextResource_t)
      */
    virtual void AddColumn(const eC_String& rkColName);

    /** Adds a column. The new column is appended to the end of the column
        list, which means that after the method returns, the new column has the
        index GetColumnCount()-1.
        @param eColNameID ID of localisation text for the name of the column.
        @see AddColumn(const eC_String&)
      */
    virtual void AddColumn(TextResource_t eColNameID);

    /** Removes the last column from the model.
        @return True if removal was successful, False otherwise */
    virtual eC_Bool RemoveColumn();

    /** Returns the name of a column.
        @param uiIndex Index of the column.
        @return Name of the column that was passed to AddColumn, or an empty
                string if uiIndex is out of range.
      */
    eC_String GetColumnName(eC_UInt uiIndex) const;

    /** Returns the text ID of the name of a column.
        @param uiIndex Index of the column.
        @return Text ID of the column, or DUMMY_TEXT if uiIndex is out of range.
      */
    TextResource_t GetColumnNameID(eC_UInt uiIndex) const;

    /** Returns the number of columns in this model.
        @return the number of columns.
      */
    inline const eC_UInt GetColumnCount() const { return m_kColNames.GetQuantity(); }

    /** Returns the number of rows in this model.
        @return the number of rows.
      */
    inline const eC_UInt GetRowCount() const { return m_kRows.GetQuantity(); }

    /** Appends a row at the end of the table.
        @return The index of the new row.
      */
    virtual eC_UInt AppendRow();

    /** Inserts a row at a specified location in the table.
        @param uiIndex Index before which the row is inserted. If this is
               higher than the current row count, the row is inserted at the
               end of the table and the return value indicates the new row's
               index.
        @return The index of the new row.
      */
    virtual eC_UInt InsertRow(eC_UInt uiIndex);

    /** Removes rows from the table.
        @param uiStartRow Index of the first row to be removed.
        @param uiEndRow Index of the row after the last one to be removed.
        @return True if the rows were successfully removed, otherwise False.
      */
    virtual eC_Bool RemoveRows(eC_UInt uiStartRow, eC_UInt uiEndRow);

    /**
        Clear the model.
        Removes all rows (NOT columns!) from the model.
        @return True if successful, False otherwise
    */
    virtual eC_Bool Clear() { return RemoveRows(0, GetRowCount());}

    /** Removes all columns from the Model
        @return True if successful, False otherwise
    */
    virtual void RemoveAllColumns();

    /** Returns a model index that is needed for interaction with table views.
        This method ensures that the index is valid by reducing indices that
        are out of range to the currently valid maximum.
        @param uiRow The row index.
        @param uiColumn The column index.
        @return The model index instance for the indicated location.
      */
    CGUIModelIndex GetIndex(eC_UInt uiRow, eC_UInt uiColumn);

    /** Returns the data item at a specified location.
        @param rkIndex The index of the item to be obtained.
        @return The item at the given location.
        @throws CGUIException if the index is invalid.
      */
    const CGUIModelItem& GetItemAt(const CGUIModelIndex& rkIndex) const;

    /** Sets a data item. The item occupying the given location is deleted.
        @param pkRow Pointer to model row
        @param uiColIndex Column index of the item.
        @param pkItem The data item to be set.
        @return True if successful, False otherwise
    */
    eC_Bool SetItem(ModelRowPtr pkRow, eC_UInt uiColIndex, CGUIModelItem* pkItem);

    /** Sets a data item.
        @param rkIndex The location at which the item is to be set.
        @param pkItem Pointer to the item to be set.
        @return True if the item was successfully set, otherwise False.
      */
    virtual eC_Bool SetItem(const CGUIModelIndex& rkIndex, CGUIModelItem* pkItem);

    /** Sets a data item.
        @param uiRow Row index of the designated item location.
        @param uiColumn Column index of the designated item location.
        @param pkItem Pointer to the item to be set.
        @return True if the item was successfully set, otherwise False.
      */
    virtual eC_Bool SetItem(eC_UInt uiRow, eC_UInt uiColumn, CGUIModelItem* pkItem);

    ///Sets a new value in the model and triggers update of views.
    virtual eC_Bool UpdateItem(const CGUIModelIndex& rkIndex, const CGUIValue& rkNewValue);

    ///Sets a new value in the model and triggers update of views.
    virtual eC_Bool UpdateItem(const eC_UInt& uiRow, const eC_UInt& uiColumn, const CGUIValue& rkNewValue);

    /** Registers a view to be notified whenever the model changes.
        @param pkView View to be registered.
        @remarks Calls CGUITableView::SetModel. It is recommended not to call
                 this method directly but calling CGUITableView::SetModel
                 instead.
      */
    void AddView(CGUITableView* pkView);

    /** Unregisters a view that was registered with AddView before.
        @param pkView View to be removed.
        @remarks This leads to a call to CGUITableView::SetModel(NULL). It is
                 recommended not to call this method directly but calling
                 CGUITableView::SetModel instead.
      */
    void RemoveView(CGUITableView* pkView);

    /** Returns a list of all registered views.
        @return const list of all views registered with this model.
    */
    const ViewList& GetViews()
    {
        return m_kViews;
    }

    /** Returns whether the given row has got any child-rows.
        @param uiRow Index of the row to check
        @return True if it has any child rows, False otherwise
    */
    eC_Bool RowHasChildren(eC_UInt uiRow);

    /** Retrieves the Index of a CGUIModelRow inside of the TableModel
        @param pkModelRow ModelRow whose index shall be returned
        @return Index of the given CGUIModelRow
    */
    eC_UInt GetRowIndex( const CGUIModelRow* pkModelRow);

    /** Gets the hierarchy depth of a row. Only relevant within TreeViews.
        @param uiRow Index of the row
        @return Hierarchy depth (0 for root objects, 1 for their children, etc...) */
    eC_UInt GetHierarchyDepth(eC_UInt uiRow);

protected:
    /** Adds a new column to all existing rows in the model */
    void AddColumnInternal();

    /** Sets the hierarchy depth of a row. Only relevant within TreeViews.
        @param uiRow Index of the row
        @param uiDepth New hierarchy depth of the row. */
    void SetHierarchyDepth(eC_UInt uiRow, eC_UInt uiDepth);

    /** Notifies all registered views that new rows have been inserted into the
        model.
        @param uiStartRow Index of the first new row.
        @param uiEndRow Index of the row after the last new one.
      */
    void NotifyRowInsertion(eC_UInt uiStartRow, eC_UInt uiEndRow) const;

    /** Notifies all registered views that rows have been removed from the
        model.
        @param uiStartRow Index of the first removed row.
        @param uiEndRow Index of the row after the last removed one.
      */
    void NotifyRowRemoval(eC_UInt uiStartRow, eC_UInt uiEndRow) const;

    /** Notifies all registered views that an item has been changed in the
        model.
        @param rkIndex Index of the changed item.
      */
    void NotifyItemUpdate(const CGUIModelIndex& rkIndex) const;

    /** Inserts a row with a given hierarchy depth at a specified location in the table
        @param uiIndex Index before which the row is inserted. If this is
               higher than the current row count, the row is inserted at the
               end of the table and the return value indicates the new row's
               index.
        @param uiHierarchyDepth Hierarchical depth of the inserted row
        @return The index of the new row.
      */
    virtual eC_UInt InsertRow(eC_UInt uiIndex, eC_UInt uiHierarchyDepth);

    /// Type of the list for storing row pointers.
    typedef eC_TListDoubleLinked<CGUIModelRow*> RowList;
    /// The list of rows in the table.
    RowList m_kRows;

    /// The list of registered views.
    ViewList m_kViews;

private:
    /// Do not allow copy construction.
    CGUITableModel(const CGUITableModel&);

    /// Do not allow assignment.
    CGUITableModel& operator=(const CGUITableModel&);

    /** Name of one column (maybe displayed in header). Offers a text ID and
        a string for flexibility.
      */
    struct ColumnName
    {
        /// Default constructor.
        ColumnName()
            : m_eColNameID(DUMMY_TEXT) {}

        /// Constructor.
        ColumnName(TextResource_t eColNameID, const eC_String& kColName)
            : m_eColNameID(eColNameID), m_kColName(kColName) {}

        /// Text ID for the column's name.
        TextResource_t m_eColNameID;

        /// Text for column's name. Only used if m_eColNameID is DUMMY_TEXT.
        eC_String      m_kColName;
    };

    typedef eC_TListDoubleLinked<ColumnName> ColumnNameList;

    /** The list of column names. At the same time used for keeping track of
        the column count.
      */
    ColumnNameList m_kColNames;


};

#endif // GUITABLEMODEL_H
