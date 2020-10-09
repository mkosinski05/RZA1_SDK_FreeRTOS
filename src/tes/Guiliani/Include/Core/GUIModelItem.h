/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUIMODELITEM_H)
#define GUIMODELITEM_H

#include "GUIValue.h"

/// Base class for items that are managed by a CGUIBaseModel.
/** Each instance of this class represents one item in a model.
The view items of a view read the data of a model item to update
the view. Beside the data, CGUIModelItem indicates if the item is
editable and selectable.
*/

class CGUIModelItem
{
public:
    /// Constructor.
    CGUIModelItem(const CGUIValue& kValue = CGUIValue(),
        const eC_Bool bIsEditable = true,
        const eC_Bool bIsSelectable = true)
        :m_kValue(kValue),
        m_bEditable(bIsEditable),
        m_bSelectable(bIsSelectable)
    {};

    /// Destructor 
    virtual ~CGUIModelItem() {};

    /**
    Retrieve the data value of the ModelItem.
    @return The value of the ModelItem.
    */
    const CGUIValue& GetValue() const { return m_kValue;}

    /** Set the value for the ModelItem.
    @param kValue The new value for the ModelItem.
    @return True if the value was set, False if it was not set because the model item is not editable.
    */
    eC_Bool SetValue(const CGUIValue& kValue)
    {
       if (m_bEditable == true)
       {
            m_kValue = kValue;
            return true;
       }
       return false;
    }

    /**
        Set the Editable flag of the ModelItem.
        The View will ask the model if the value is editable or not.
        @param bEditable True if editable, False else.
    */
    void SetEditable(const eC_Bool& bEditable)
    {
        m_bEditable = bEditable;
    }

    /**
        Check if the ModelItem is editable or not.
        @return True if the ModelItem is editable, False else.
    */
    eC_Bool IsEditable() const {return m_bEditable;}

    /**
        Set the Selectable flag of the ModelItem.
        The View will ask the model if the value is selectable or not.
        @param bSelectable True if selectable, False else.
    */
    void SetSelectable(const eC_Bool& bSelectable)
    {
        m_bSelectable = bSelectable;
    }

    /**
        Check if the ModelItem is selectable or not.
        @return True if the ModelItem is selectable, False else.
    */
    eC_Bool IsSelectable() const {return m_bSelectable;}

private:

    /// The data stored for this model item.
    CGUIValue m_kValue;

    /// Indicates whether editing is allowed.
    eC_Bool m_bEditable;

    /// Indicates whether the item is selectable.
    eC_Bool m_bSelectable;
};

#endif // GUIMODELITEM_H
