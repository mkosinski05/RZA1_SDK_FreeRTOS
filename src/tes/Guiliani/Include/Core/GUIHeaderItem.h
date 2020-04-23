/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_HEADER_ITEM__H_
#define GUI_HEADER_ITEM__H_

#include "GUIBaseTextField.h"

class CGUITableView;

/**
    Responsible for displaying the column name @see CGUIModel::GetColumnName and resizing the column width.
    @brief Helper class for CGUITableView.
*/
class CGUIHeaderItem : public CGUIBaseTextField
{

public:
    /** Constructor
        @param pkParent Parent object
        @param uiViewColumnIndex Column index
        @param rkView Table view
        @param cuiModelColumnIndex Model column index
    */
    CGUIHeaderItem(CGUICompositeObject* pkParent, eC_UInt uiViewColumnIndex, CGUITableView& rkView, const eC_UInt cuiModelColumnIndex);

    eC_Bool DoDraw();

    eC_Bool DoDrag(
        const eC_Value &vDeltaX,
        const eC_Value &vDeltaY,
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    eC_Bool DoButtonDown(
        const eC_Value& vAbsX,
        const eC_Value& vAbsY);

    eC_Bool DoDragEnd(
        const eC_Value& vAbsX,
        const eC_Value& vAbsY);

    /// refresh the width of the header item and the string.
    void Update();

private:

    eC_Value CalculateDragRegionX();

    CGUITableView& m_rkView;
    eC_UInt m_uiViewColumnIndex;
    const eC_UInt m_cuiModelColumnIndex;
    eC_Value m_vDragStartPoint; //-1 means drag inactive.
};

#endif
