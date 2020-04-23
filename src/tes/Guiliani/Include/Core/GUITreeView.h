/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUITREEVIEW_H)
#define GUITREEVIEW_H

#include "GUITableView.h"

class CGUITableView;

///@brief A view for instances of CGUITreeModel.
// @guiliani_doxygen toplevel_control Tree View
/**
    <table border="0">
        <tr>
            <td width="200">@image html tree_view.png</td>
            <td>
                The "tree view" is used for presenting controls in a tree structure. Every cell of the tree view can contain a different control (Class: CGUITreeView).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    The tree view provides a dynamic view upon a tree model.
    It manages the actual GUI objects that display the data of a tree, and offers interfaces
    to expand / collapse the tree, or specific parts of it.

@section sec_cguitreeview_typical_usecase A typical use case
Typical usage is as follows:
- Create an instance of this class.
- Set ViewItemGenerators to specify the visual representation of the data. (or rely on defaults)
- Append sibling or childes for the last appended item to the model.
- Attach the above created model by calling SetModel. At this point, all data that is
already present in the model is shown in the view.

Example:
@code
    // Some defines just for convenience
    #define TREE_FIRST_COL  0
    #define TREE_SECOND_COL 1

    // Create Treeview
    CGUITreeView* pkTreeView = new CGUITreeView( pkPage5, eC_FromInt(10), eC_FromInt(10), eC_FromInt(420), eC_FromInt(240), NO_HANDLE);

    // Customize its appearance
    pkTreeView->SetGridLineWidth(eC_FromInt(2));
    pkTreeView->SetHeaderColor(0xFF5577EE);
    pkTreeView->EnableGrid(false, true);

    // Enable multi-selection mode
    pkTreeView->SetSelectionMode(CGUITableView::MULTI_SELECTION);

    // Append some columns to the treeview, which shall be selectable via Longclicks and Editable via double-clicks
    pkTreeView->AppendColumn(TREE_FIRST_COL,  eC_FromInt(180),0, CGUITableView::TRIGGER_LONG_CLICK, CGUITableView::TRIGGER_DOUBLE_CLICK);
    pkTreeView->AppendColumn(TREE_SECOND_COL, eC_FromInt(60), 0, CGUITableView::TRIGGER_LONG_CLICK, CGUITableView::TRIGGER_DOUBLE_CLICK);

    // create model
    CGUITreeModel* pkTreeModel = new CGUITreeModel();

    // The model shall have three columns
    pkTreeModel->AddColumn("River");
    pkTreeModel->AddColumn("Length");

    // add some data to the model
    CGUITableModel::ModelRowPtr pkDonauRow = pkTreeModel->Append();
    pkTreeModel->SetItem( pkDonauRow, TREE_FIRST_COL, new CGUIModelItem("Donau"));
    pkTreeModel->SetItem( pkDonauRow, TREE_SECOND_COL, new CGUIModelItem("2811"));

    // first tributaries of the river donau
    CGUITableModel::ModelRowPtr pkAltmuehlRow = pkTreeModel->AppendAsChild( pkDonauRow);
    pkTreeModel->SetItem(pkAltmuehlRow, TREE_FIRST_COL, new CGUIModelItem("Altmuehl"));
    pkTreeModel->SetItem(pkAltmuehlRow, TREE_SECOND_COL, new CGUIModelItem("58"));

    CGUITableModel::ModelRowPtr pkWiesethRow = pkTreeModel->AppendAsChild( pkAltmuehlRow);
    pkTreeModel->SetItem(pkWiesethRow, TREE_FIRST_COL, new CGUIModelItem("Wieseth"));
    pkTreeModel->SetItem(pkWiesethRow, TREE_SECOND_COL, new CGUIModelItem("27"));

    CGUITableModel::ModelRowPtr pkSulzRow = pkTreeModel->AppendAsSibling( pkWiesethRow);
    pkTreeModel->SetItem(pkSulzRow, TREE_FIRST_COL, new CGUIModelItem("Sulz"));
    pkTreeModel->SetItem(pkSulzRow, TREE_SECOND_COL, new CGUIModelItem("31"));

    CGUITableModel::ModelRowPtr pkIsarRow = pkTreeModel->AppendAsSibling( pkDonauRow);
    pkTreeModel->SetItem(pkIsarRow, TREE_FIRST_COL, new CGUIModelItem("Isar"));
    pkTreeModel->SetItem(pkIsarRow, TREE_SECOND_COL, new CGUIModelItem("295"));

    CGUITableModel::ModelRowPtr pkLafatscherbachRow = pkTreeModel->AppendAsSibling(pkAltmuehlRow);
    pkTreeModel->SetItem(pkLafatscherbachRow, TREE_FIRST_COL, new CGUIModelItem("Lafatscherbach"));
    pkTreeModel->SetItem(pkLafatscherbachRow, TREE_SECOND_COL, new CGUIModelItem("7"));

    CGUITableModel::ModelRowPtr pkGleirschbachRow = pkTreeModel->AppendAsChild(pkLafatscherbachRow);
    pkTreeModel->SetItem(pkGleirschbachRow, TREE_FIRST_COL, new CGUIModelItem("Gleirschbach"));
    pkTreeModel->SetItem(pkGleirschbachRow, TREE_SECOND_COL, new CGUIModelItem("12"));

    CGUITableModel::ModelRowPtr pkSemptRow = pkTreeModel->AppendAsChild(pkSulzRow);
    pkTreeModel->SetItem(pkSemptRow, TREE_FIRST_COL, new CGUIModelItem("Sempt"));
    pkTreeModel->SetItem(pkSemptRow, TREE_SECOND_COL, new CGUIModelItem("39"));

    CGUITableModel::ModelRowPtr pkNaabRow = pkTreeModel->AppendAsChild(pkIsarRow);
    pkTreeModel->SetItem(pkNaabRow, TREE_FIRST_COL, new CGUIModelItem("Naab"));
    pkTreeModel->SetItem(pkNaabRow, TREE_SECOND_COL, new CGUIModelItem("165"));

    // Assign the model to the TreeView.
    // It is recommended to do this after the model is completely initialized, to achieve optimum performance
    pkTreeView->SetModel(pkTreeModel);

    // Expand the entire tree
    pkTreeView->ExpandAll();
@endcode

@section sec_cguitreeview_using_custom_viewitems Using customized ViewItems within a TreeView

While the default ViewItemGenerators should suffice for most cases, it is possible to use customized ViewItems
within TreeViews as well.

The following code snippets show how to implement a custom TreeItem and a corresponding generator.
This demo implementation will change the icon contained within the standard CGUIViewItemTreeElement depending
on the value of the associated model value.

@code
#include "GUIViewItem.h"
class CMyTreeItem : public CGUIViewItemTreeElement
{
public:

    CMyTreeItem( const CGUIViewItemGeneratorText* pkGenerator, CGUIViewIndex kViewIndex, CGUITreeView* pkView)
        : CGUIViewItemTreeElement( pkGenerator, kViewIndex, pkView)
    {
    }

    void SetData(const CGUIValue& kNewValue)
    {
        // Call base class implementation
        CGUIViewItemTreeElement::SetData( kNewValue);

        // Example code to set some custom icons with regard to some external dependency.
        // Following this approach e.g., folder and file icons in a filebrowser can be implemented
        if( kNewValue.ToString().GetLength() % 3 == 0)
            m_pkIcon->SetImage( DUMMY_IMAGE );
        else if( kNewValue.ToString().GetLength() % 2 == 0)
            m_pkIcon->SetImage( IMG_STDCTRL_RB_NOTSELECTED_FOCUSSED );
        else
            m_pkIcon->SetImage( IMG_STDCTRL_PLACEHOLDER );

        // Check if icon visibility has changed
        eC_Bool bInvisible = m_pkIcon->IsInvisible();
        if( m_pkIcon->GetImageID() == DUMMY_IMAGE)
            m_pkIcon->SetInvisible(true);
        else
            m_pkIcon->SetInvisible(false);

        // Visibility has changed. Update container
        if( bInvisible != m_pkIcon->IsInvisible() )
            m_pkViewObject->RepositionChildren();

        // Trigger refresh
        m_pkViewObject->InvalidateArea();
    }
};
@endcode

Here is the code for the ViewItemGenerator which creates the custom view item shown above.
Note that it implements both the CreateViewItem and CreateTreeViewItem APIs, so that it can easily
be used within TableViews as well.

@code
class CMyViewItemGeneratorTreeElement : public CGUIViewItemGeneratorTreeElement
{
public:
    /// Default constructor.
    CMyViewItemGeneratorTreeElement() {};

    /// This is being called when this generator is used within TableViews
    virtual CGUIViewItem* CreateViewItem(CGUITableView* pkView, const CGUIViewIndex& kViewIndex) const
    {
        return new CGUIViewItemButton(kViewIndex, pkView);
    }

    /// This is being called when this generator is used within TreeViews
    virtual CGUIViewItem* CreateTreeViewItem(CGUITreeView* pkView, const CGUIViewIndex& kViewIndex) const
    {
        return new CMyTreeItem(this, kViewIndex, pkView);
    }
};
@endcode


@section sec_cguitreeview_technical_details Technical details on CGUITreeView

The tree view is derived from CGUITableView and the tree model is in fact a CGUITableModel
with an hierarchical level assigned to each row. This hierarchy depth reflects the relationships
within rows of the tree. A row with a hierarchy level of e.g., 4 will be a child object to the
first preceeding object with a level of 3.
<ol>
<li>Donau (Hierarchy depth = 0)</li>
  <ol>
    <li>Altmuehl (Hierarchy depth = 1)</li>
      <ol>
        <li>Wieseth (Hierarchy depth = 2)</li>
      </ol>
    <li>Lafatscherbach (Hierarchy depth = 1)</li>
  </ol>
<li>Isar (Hierarchy depth = 0)</li>
</ol>


@ingroup GUILIANI_CONTROLS
*/
class CGUITreeView : public CGUITableView
{
public:
    /** Constructs the table view by construction of the table view and preparing a column for the expansion flag and
        a View Item Generator for the first column.
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY X-position relative to its parent object
        @param vWidth Width of the object
        @param vHeight Height of the object
        @param eID Object Identifier of this object
    */
    CGUITreeView(
        CGUICompositeObject *const pParent,
        const eC_Value &vX,
        const eC_Value &vY,
        const eC_Value &vWidth,
        const eC_Value &vHeight,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Sets the column of the treeview which shall contain the tree of expandable/collapsable items.
        @param uiColumn Index of the tree column (starting with 0) */
    void SetTreeColumn(eC_UInt uiColumn);

    eC_Bool InsertColumn(eC_UInt uiColumnIndex, eC_UInt uiModelColumnIndex, eC_Value vWidth, eC_Value vMinWidth = 10, TriggerEvent_t eSelectionTrigger = TRIGGER_CLICK, TriggerEvent_t eEditTrigger = TRIGGER_DOUBLE_CLICK);

    eC_Bool RemoveColumn(eC_UInt uiColumnIndex);

    /** Returns the column of the treeview which contains the tree of expandable/collapsable items.
        @return Index of the tree column (starting with 0) */
    eC_UInt GetTreeColumn() { return m_uiTreeColumn;}

    /** Checks whether the given row is currently expanded or not
        @param uiRow Index of the row whose "expanded" status shall be checked
        @return True if it is expanded, False otherwise */
    eC_Bool IsExpanded(eC_UInt uiRow);

    /** Expands/Collapses the given Row
        @param uiRow Index of the row which shall be expanded / collapsed
        @param bExpanded TRUE expands the given row, FALSE collapses it */
    void SetExpanded( eC_UInt uiRow, eC_Bool bExpanded);

    /** Set visibility according to expansion states for the whole tree
        @param uiStartRow Row index from which on downwards the tree visibility will be updated
    */
    void HandleTreeVisibility(eC_UInt uiStartRow =0 );

    /// Expands all subtrees
    void ExpandAll();

    /// Collapses the entire tree
    void CollapseAll();

    /** Generates a view item for use within a tree view
        @param kViewIndex ViewIndex for which to create the view item
        @return Pointer to a newly created ViewItem
    */
    CGUIViewItem* GenerateViewItem(const CGUIViewIndex& kViewIndex)
    {
        if( kViewIndex.GetColumn() == m_uiTreeColumn)
            return m_kGeneratorContainer.GetViewItemGenerator(kViewIndex)->CreateTreeViewItem(this, kViewIndex);
        else
            return m_kGeneratorContainer.GetViewItemGenerator(kViewIndex)->CreateViewItem(this, kViewIndex);
    }

    /** Sets the indentation (in pixels) per level of hierarchy
        @param vIndentation Indentation (in pixels) */
    void SetIndentation( const eC_Value vIndentation) { m_vIndentation = vIndentation; }

protected:
    virtual RowInfo CreateRowInfo(eC_UInt uiRow);

    /** Indents the cell rectangle to represent the hierarchical tree structure within the model
        @param CellRect reference to cell rectangle, contains indented rectangle after execution.
        @param pItem points to the table item
    */
    void ModifyCellRect(CGUIRect & CellRect, CGUIViewItem * pItem);

    /// Index of the Column which shall contain the tree
    eC_UInt m_uiTreeColumn;

    /// indentation (in pixels) per level of hierarchy
    eC_Value m_vIndentation;
};

#endif // GUITREEVIEW_H
