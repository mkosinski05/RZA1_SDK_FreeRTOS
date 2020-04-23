/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUITREEMODEL_H)
#define GUITREEMODEL_H

#include "GUITableModel.h"

/** 
    Please refer to CGUITreeView for detailed examples.
    @brief Stores data that can be displayed in a CGUITreeView
*/
class CGUITreeModel : public CGUITableModel
{
public:
    enum { notfound = 0xffffffff };

    // Access via Row pointers ---------------------------------------------------------------------------

    /** Appends a new row to the end of the model. The new row will be attached directly to the root of the tree,
        i.e. it will not be appended as a child to the previously added row.
        @return Pointer to newly appended row
    */
    ModelRowPtr Append();

    /** Inserts a new row into the model. The row will be a sibling to the given row, and will be inserted right before it.
        @param pkSiblingRow Row before which the new row will be inserted.
        @return Pointer to newly inserted row
    */
    ModelRowPtr InsertSiblingBefore(ModelRowPtr pkSiblingRow);

    /** Creates and appends a child row to the given row.
        The new row will be appended as the last child object.
        @param pkParentRow Pointer to the row to which the newly created row will be appended
        @return Pointer to the newly created row */
    ModelRowPtr AppendAsChild( ModelRowPtr pkParentRow);

    /** Creates and appends a sibling after the given row.
        The newly created sibling will be appended directly after the given row.
        @param pkSiblingRow Pointer to the row after which the newly created row will be appended
        @return Pointer to the newly created row */
    ModelRowPtr AppendAsSibling( ModelRowPtr pkSiblingRow);

    /** Returns a pointer to the given row's parent row
        @param pkChildRow Pointer to child row to check
        @return Pointer to parent row (Root objects will return NULL)
    */
    ModelRowPtr GetParent( ModelRowPtr pkChildRow);

    /** Checks if the given row has any children
        @param pkParentRow Pointer to the parent row to check
        @return True if the row has any children, False otherwise
    */
    eC_Bool HasChildren( ModelRowPtr pkParentRow);

    /** Returns a pointer to the next sibling (if any)
        @param pkSiblingRow Pointer to the row whose siblings shall be found
        @return Pointer to the next sibling row (or 0 if no siblings were found)*/
    ModelRowPtr GetNextSibling( ModelRowPtr pkSiblingRow);

    /** Returns a Pointer to the first child (if any)
        @param pkParentRow Pointer to the row whose child shall be found
        @return Pointer to the first child row (or 0 if no children were found)*/
    ModelRowPtr GetFirstChild( ModelRowPtr pkParentRow);

    /** Returns a pointer to the very first entry in the TreeModel
        @return Pointer to the very first entry in the TreeModel */
    ModelRowPtr GetRoot();

    /** Returns a pointer to the next entry in the TreeModel (regardless if child, sibling, or next parent)
        @return Pointer to the next entry in the TreeModel
        @param pkRow Pointer to the row
        */
    ModelRowPtr GetNext(ModelRowPtr pkRow);

    // Access via Row indices ---------------------------------------------------------------------------

    /** Creates and appends a child row to the given row.
        The new row will be appended as the last child object.
        @param uiRowIndexOfParent Index of the row to which the newly created row will be appended
        @return Index of the newly created row */
    eC_UInt AppendAsChild( eC_UInt uiRowIndexOfParent);

    /** Inserts a new row into the model. The row will be a sibling to the given row, and will be inserted right before it.
        @param uiRowIndexOfSibling Index of the row before which the new row will be inserted.
        @return Index of the newly inserted row
    */
    eC_UInt InsertSiblingBefore(eC_UInt uiRowIndexOfSibling);

    /** Creates and appends a sibling after the given row.
        The newly created sibling will be appended directly after the given row.
        @param uiRowIndexOfSibling Index of the row after which the newly created row will be appended
        @return Index of the newly created row */
    eC_UInt AppendAsSibling( eC_UInt uiRowIndexOfSibling);

    /** Returns the index of the given row's parent row
        @param uiRowIndexOfChild Row index of the row to check
        @return Row Index of parent row (Root objects will return CGUITreeModel::notfound)
    */
    eC_UInt GetParent( eC_UInt uiRowIndexOfChild);

    /** Checks if the given row has any children
        @param uiRowIndexOfParent Row index of the row to check
        @return True if the row has any children, False otherwise
    */
    eC_Bool HasChildren( eC_UInt uiRowIndexOfParent);

    /** Returns the row index of the next sibling (if any)
        @param uiRowIndex Index of the row whose siblings shall be found
        @return Index of the next sibling row (or 0 if no siblings were found)*/
    eC_UInt GetNextSibling( eC_UInt uiRowIndex);

    /** Returns the row index of the first child (if any)
        @param uiRowIndex Index of the row whose child shall be found
        @return Index of the first child row (or 0 if no children were found)*/
    eC_UInt GetFirstChild( eC_UInt uiRowIndex);

};

#endif
