/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUI_VIEWITEM_H)
#define GUI_VIEWITEM_H

#include "GUIModelIndex.h"
#include "GUIText.h"
#include "GUIBaseTextField.h"
#include "GUICheckBox.h"
#include "GUITableView.h"
#include "GUIComboBox.h"
#include "GUIBaseButton.h"
#include "GUITreeView.h"
#include "GUIRepositionCompositeObject.h"
#include "GUIImage.h"
#include "GUIInputField.h"

/// Base class for a ViewItem.
/**  A ViewItem is used by CGUITableView to manage the visual representation of its
model.
A ViewItem provides a CGUIObject (CreateViewObject(), GetViewObject()) which is displayed by the CGUITableView.
It holds a CGUIViewIndex and can be triggerd to update its data, which is read from the
model. The AutoResize flag indicates if the view shall resize the item to match the
table's cell width and height.

The ViewItem is also responsible to inform the table view about changed values.
Therefore, the ViewItem observes the ViewObject (if it is editable) or the EditorObject
which is created by the ViewItemGenerator (see CGUITableView for details).
The ViewObject or the EditorObject calls OnNotification. This method should inform the
model by calling CGUITableView::ItemEdited.

The ViewItem also provides the interface to set the ViewObject selected or focused,
which is responsible to adapt the visual appearance of the ViewObject.

Derive your own view item classes from this base.
*/

class CGUIViewItem : public CGUIObserver
{
public:

    ///Set the internal pointer to the GUIObject to NULL.
    virtual void ResetViewObject() = 0;

    /** Interface to trigger an update of the displayed data. The ViewItem has
        to implement the update of its ViewObject.
        @param kNewValue The new value
    */
    virtual void SetData(const CGUIValue& kNewValue) = 0;


    /** Get the ViewIndex.
        @return The ViewIndex of the ViewItem.
    */
    CGUIViewIndex GetViewIndex() const {return m_kViewIndex;}

    /**
        Change the row index for an existing view item.
        This is used when rows are deleted from or inserted into the table.
        This means that the view items can be repositioned based on the new
        row index and do not need to be regenerated.
        @param uiRowIndex The new row index for the ViewItem.
    */
    void SetRowIndex(eC_UInt uiRowIndex)
    {
        SetViewIndex( CGUIViewIndex(m_kViewIndex.GetColumn(), uiRowIndex) );
    }

    /**
        Get the View which uses the view item.
        @return Pointer to the View.
    */
    inline CGUITableView* GetView() const {return m_pkView;}

    /**
        Set View index to which this ViewItem is associated.
        @param kViewIndex The new ViewIndex
    */
    void SetViewIndex( const CGUIViewIndex &kViewIndex) { m_kViewIndex = kViewIndex; }

    /**
        Get the AutoResize flag. The AutoResize flag is used by the table view
        to determine if ViewObjects shall be expanded to the cell's width and height.
        If true the CGUITableView will resize the ViewObject to the table cell dimensions
        after creation and whenever the cell dimensions are updated.
        @return The AutoResize flag of the ViewItem.
    */
    bool GetAutoResize() const {return m_bAutoResize;}

    /**
        Called by a subject whenever the observed value changes. To ensure that this is done,
        a view item has to observe its ViewObject's value or the EditorObject.
        @see CGUIViewItemGenerator::GetEditorObject().
        This function has to inform the view about new value by calling CGUITableView::ItemEdited.
        This implementation only calls CGUITableView::ItemEdited when the value is changed.
        CGUITableView::StopEditing will be called to hide the current EditorObject if active.
        @param kObservedValue The update observed value.
        @param pkUpdatedObject The observed object.
        @param uiX X-Index of value to be set within eDataID (if it is a multidimensional DataPoolEntry)
        @param uiY Y-Index of value to be set within eDataID (if it is a multidimensional DataPoolEntry)
    */
   virtual void OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX=0, const eC_UInt uiY=0);

    /**
        Set the view object focused. This function shall toggle the ViewObject's "focused" appearance.
        @param bFocused true to activate, false to deactivate focused mode.
    */
   virtual void SetFocused(eC_Bool bFocused){}

    /**
        Set the view object selected. This function shall toggle the ViewObject's "selected" appearance.
        @param bSelected true to activate, false to deactivate selected mode.
    */
    virtual void SetSelected(eC_Bool bSelected) {}


    /**
        Provides the internal pointer to the ViewObject that is managed by the ViewItem.
        @return The Pointer to the ViewObject or NULL if the ViewObject was not yet created.
    */
    virtual CGUIObject* GetViewObject() = 0;

    /**
        Creates the ViewObject for the ViewItem. The object is owned by the table view.
        The ViewItem holds a pointer to its ViewObject to realise data updates, selection, focus handling and positioning.
        If the CGUITableView deletes the ViewObject, it will inform the view item by calling ResetViewObject().
        Reimplement this method to create your own view objects.
        @return The pointer to the view object.
        @see SetData()
        @see SetFocused()
        @see SetSelected()
        @see GetViewObject()
        @see RepositionViewObject()

    */
    virtual CGUIObject* CreateViewObject()  = 0;

    /**
        Reposition the view object based on the target cell and the AutoResize flag.
        This function is called whenever a view object needs to be repositioned. This is the case
        when rows or columns are added or removed, when rows are hidden, or when the column width is changed.
        The default implementation makes sure that the CGUIObject does not overlap the cell region
        and resizes it to the column width if GetAutoResize() returns true.
        The implementation needs to make sure to invalidate the object when its position was updated.
        @param rkCellRect The new bounding rect of the table cell.
    */
    virtual void RepositionViewObject(const CGUIRect& rkCellRect);

    /**
        This function indicates if the ViewObject itself is an editor and notifies the view of value changes,
        or if the CGUIViewItemGenerator has to provide another object for editing the value of the view item.
        @see CGUIViewItemGenerator::GetEditorObject()
        @return True if this view item is an editor
    */
    virtual eC_Bool IsEditable() = 0;

    virtual ~CGUIViewItem()
    {
        m_pkView = NULL;
    }

    /**
        Reads the data for the view index from the model and updates the view item by calling SetData.
        This is used to initialize the ViewObject.
    */
    void UpdateData();

protected:

     /**
        Constructor. Sets the ViewIndex of the ViewItem.
        @param kViewIndex The ViewIndex to set.
        @param pkView pointer to the containing view (parent).
        @param bAutoResize If true (default), an item will always be resized to it's column width and row height.
                           If false, the object will be regenerated by the generator each time, to make sure that its default width is used as maximum.
                           If the generator generates an object that does not fit into the cell, the object will be resized, so that it fits into the cell.
                           @see CGUITableView::AddColumn to make sure that a minimum size of a view object is used.
     */
     CGUIViewItem(const CGUIViewIndex& kViewIndex, CGUITableView* pkView, bool bAutoResize = true)
        : m_kViewIndex(kViewIndex),
          m_pkView(pkView),
          m_bAutoResize(bAutoResize)
     {}

    /// Initialize the view object
    virtual void InitalizeViewObject() = 0;

    ///The index of the view item.
    CGUIViewIndex   m_kViewIndex;

    ///Pointer to the view object used to update data, selection and focus.
    CGUITableView*  m_pkView;

    /**Indicates if the view object shall be resized by the table view.
    If true the GUIObject's width and height is fitted into the cell.*/
    eC_Bool m_bAutoResize;
};

/**
    It holds a pointer to a CGUIViewItemGeneratorText to receive color
    and font information for the graphical representation.
    The TextField itself is not an editor. The default implementation of CGUIViewItemGeneratorText
    will provide a CGUIInputField for editing this ViewItem.
    @brief ViewItemText using a TextField as ViewObject
*/
class CGUIViewItemText : public  CGUIViewItem
{
public:
    /**
        Constructor
        @param pkGenerator The view item generator
        @param kViewIndex The view index
        @param pkView The table view
    */
    CGUIViewItemText(
        const CGUIViewItemGeneratorText* pkGenerator,
        CGUIViewIndex kViewIndex,
        CGUITableView* pkView);

    virtual ~CGUIViewItemText();

    void ResetViewObject() {m_pkViewObject = NULL;}

    void SetData(const CGUIValue& kNewValue);

    void SetSelected(eC_Bool bSelected);

    CGUIObject* CreateViewObject();

    CGUIObject* GetViewObject()
    {
        return m_pkViewObject;
    };

    virtual eC_Bool IsEditable();

protected:
    virtual void InitalizeViewObject();

    CGUIBaseTextField* m_pkViewObject;///< holds the view object
    const CGUIViewItemGeneratorText* m_pkGenerator;///< holds the view item generator
};



/**
    This creates a CGUIRepositionComposite container with three default children in it, namely:
    - a CGUICheckbox: Used to visualize and change the expanded/collapsed state of branches in the TreeView
    - a CGUIImage: To simplify customized tree view graphics (e.g. for file-browsers)
    - a CGUIBaseTextfield: Displaying the actual value in the corresponding cell of the model

    Feel free to derive your own customized TreeView-items from this base class.
    By default, the Image-object is unused and remains available for customization by derived classes.
    @brief A ViewItem for use within TreeViews
*/
class CGUIViewItemTreeElement : public CGUIViewItem
{
public:
    /// constructor
    CGUIViewItemTreeElement( const CGUIViewItemGeneratorText* pkGenerator, CGUIViewIndex kViewIndex, CGUITreeView* pkView);

    void InitalizeViewObject();

    void ResetViewObject() {m_pkViewObject = NULL;}

    virtual void SetData(const CGUIValue& kNewValue);

    void OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX=0, const eC_UInt uiY=0);

    CGUIObject* CreateViewObject();

    CGUIObject* GetViewObject()
    {
        return m_pkViewObject;
    }

    eC_Bool IsEditable();

    void SetSelected(eC_Bool bSelected);

    /// Informs the treeitem that it is Expandable (=it has got child objects within the tree)
    virtual void SetExpandable( const eC_Bool bExpanded);

    /// Retrieve internal checkbox object
    CGUICheckBox*       GetCheckBox()   { return m_pkExpandCheckBox; }

    /// Retrieve internal image object (Can be used as a customized icon in derived ViewItems)
    CGUIImage*          GetIcon()       { return m_pkIcon; }

    /// Retrieve internal textfield object
    CGUIBaseTextField*  GetTextField()  { return m_pkText; }

protected:
    /**
        Sets the text attributes
        @param bSelected True if the test is selected
    */
    void SetTextAttributes( const eC_Bool bSelected);

    CGUIRepositionCompositeObject*  m_pkViewObject;///< The view object
    CGUICheckBox*                   m_pkExpandCheckBox;///< The check box
    CGUIImage*                      m_pkIcon;///< The icon image
    CGUIBaseTextField*              m_pkText;///< The text field
    // Required for retrieving text color attributes
    const CGUIViewItemGeneratorText* m_pkGenerator;///< The view item generator
};


/**
    The CheckBox is also the editor for the ViewItem.
    The ViewItem is added as observer of the CheckBox.
    The Subject of the CheckBox will therefore call OnNotification of the ViewItem base class.
    This base implementation does not provide a "selected" representation.
    @brief A ViewItem which uses a checkbox as ViewObject
*/
class CGUIViewItemCheckBox : public  CGUIViewItem
{
public:
    /** Constructor
        @param kViewIndex The view Index
        @param pkView The table view
    */
    CGUIViewItemCheckBox(CGUIViewIndex kViewIndex, CGUITableView* pkView);

    void InitalizeViewObject();

    void ResetViewObject() {m_pkViewObject = NULL;}

    void SetData(const CGUIValue& kNewValue);

    CGUIObject* CreateViewObject();

    void RepositionViewObject(const CGUIRect& rkCellRect);

    CGUIObject* GetViewObject()
    {
        return m_pkViewObject;
    }

    eC_Bool IsEditable();

protected:

    CGUICheckBox* m_pkViewObject;///< The view object
};

/**
    The ComboBox is also the editor for the ViewItem.
    The ViewItem is added as observer of the ComboBox.
    The Subject of the ComboBox will therefore call OnNotification of the ViewItem base class.
    This base implementation does not provide a "selected" representation.
    @brief  A ViewItem which uses a ComboBox as ViewObject
*/

class CGUIViewItemComboBox : public  CGUIViewItem
{
public:

    /** Constructor
        @param kViewIndex The view Index
        @param pkView The table view
    */
    CGUIViewItemComboBox(CGUIViewIndex kViewIndex, CGUITableView* pkView);

    void InitalizeViewObject();

    void ResetViewObject() {m_pkViewObject = NULL;}

    void SetData(const CGUIValue& kNewValue);

    CGUIObject* CreateViewObject();

    CGUIObject* GetViewObject()
    {
        return m_pkViewObject;
    }

    eC_Bool IsEditable();

    /**
    Adds a new entry to the combo box. If the ViewObject was not yet generated,
    CreateViewObject() is called to generate it.
        @param rkEntry The new Combobox entry to add.
        @param bIsAutoAdjustArrange Flag to enable and disable the autoadjust.
                     Defaults to true = enable autoadjust.
    */
    virtual void AddComboBoxEntry(const eC_String& rkEntry, eC_Bool bIsAutoAdjustArrange = true);

    /**
    Adds a new entry to the combo box. If the ViewObject was not yet generated,
    CreateViewObject() is called to generate it.
        @param eText The new Combobox entry to add.
        @param bIsAutoAdjustArrange Flag to enable and disable the autoadjust.
                     Defaults to true = enable autoadjust.
    */
    virtual void AddComboBoxEntry(const TextResource_t eText, eC_Bool bIsAutoAdjustArrange = true);

    /**
    Arranges the entries of the combo box vertically. Call this method after
    adding entries without activated auto adjustment.
    @see AddComboBoxEntry()
    */
    void AdjustArrange();
protected:

    CGUIComboBox* m_pkViewObject;///< The view object
};

/**
    It is possible to set a command which is set in the view object.
    @brief A ViewItem which uses a Button as ViewObject
*/
class CGUIViewItemButton : public  CGUIViewItem
{
public:

    /** Constructor
        @param kViewIndex The view Index
        @param pkView The table view
    */
    CGUIViewItemButton(CGUIViewIndex kViewIndex, CGUITableView* pkView);

    void InitalizeViewObject() {}

    void ResetViewObject() {m_pkViewObject = NULL;}

    void SetData(const CGUIValue& kNewValue);

    CGUIObject* CreateViewObject();

    CGUIObject* GetViewObject()
    {
        return m_pkViewObject;
    }

    //by default a button does not allow editing.
    eC_Bool IsEditable() {return true;}

    /**
        Set a command to the ViewObject.
        @see CGUIBaseButton::SetCommand()
        @param pCommand The command to be set.
    */
    void SetCommand(CGUICommandPtr pCommand);

protected:

    CGUIBaseButton* m_pkViewObject; ///< The view object
    CGUICommandPtr m_pkCommand; ///< The used command
};

/**
    The CGUIImage (icon) is also the editor for the ViewItem.
    The ViewItem is added as observer of the CGUIImage.
    The Subject of the CGUIImage will therefore call OnNotification of the ViewItem base class.
    This base implementation does not provide a "selected" representation.
    @brief A ViewItem uing a CGUIImage (icon) as ViewObject.
*/
class CGUIViewItemImage : public  CGUIViewItem
{

public:

    /** Constructor
        @param cpkGenerator The view item generator
        @param kViewIndex The view index
        @param pkView The table view
    */
    CGUIViewItemImage(
        const CGUIViewItemGeneratorImage* cpkGenerator,
        CGUIViewIndex kViewIndex,
        CGUITableView* pkView);

    void ResetViewObject(void) {m_pkViewObject = NULL;}

    void SetData(const CGUIValue& crkNewValue);

    CGUIObject* CreateViewObject();

    CGUIObject* GetViewObject(void) {return m_pkViewObject;}

    eC_Bool IsEditable(void){return false;}

protected:

    virtual void InitalizeViewObject(void);

    CGUIImage* m_pkViewObject; ///< The view object
    const CGUIViewItemGeneratorImage* m_cpkGenerator; ///< The view item generator
};

/**
    It holds an pointer to a ViewItemGeneratorInputField to receive color
    and font information for the graphical representation.
    The InputField itself is an editor.
    @brief ViewItemInputField using an InputField as ViewObject 
*/
class CGUIViewItemInputField : public CGUIViewItem
{
public:

    /** Constructor
        @param cpkGenerator The view item generator
        @param kViewIndex The view index
        @param pkView The table view
    */
    CGUIViewItemInputField(
        const CGUIViewItemGeneratorInputField* cpkGenerator,
        CGUIViewIndex kViewIndex,
        CGUITableView* pkView);

    void ResetViewObject(void) {m_pkViewObject = NULL;}

    void SetData(const CGUIValue& crkNewValue);

    void SetSelected(eC_Bool bSelected);

    CGUIObject* CreateViewObject(void);

    CGUIObject* GetViewObject(void){return m_pkViewObject;}

    eC_Bool IsEditable(void);

protected:

    virtual void InitalizeViewObject(void);

    CGUIInputField*    m_pkViewObject;///<The view object
    const CGUIViewItemGeneratorInputField* m_cpkGenerator;///<The view item generator
};

#endif // GUI_VIEWITEM_H
