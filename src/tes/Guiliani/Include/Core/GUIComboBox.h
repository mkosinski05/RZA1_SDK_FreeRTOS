/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUICOMBOBOX__H_
#define GUICOMBOBOX__H_

#include "GUICompositeObject.h"
#include "GUICommand.h"
#include "GUIComboBoxHeader.h"
#include "GUIListBox.h"
class CGUIListItem;

/// Class for construction of a combo box control.
// @guiliani_doxygen toplevel_control Combo Box
/**
    <table border="0">
        <tr>
            <td width="200">@image html combo_box.png</td>
            <td>
                The "combo box" control consists of a scrollable list combined with an input field. 
                The list portion of the combo box may be displayed unfold or fold when the user selects the drop-down button 
                next to the control. The "combo box" can be used for e.g., drop-down selection lists (Class: CGUIComboBox).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    A combo box consists of a scrollable list combined with an input field as header that is either
    editable or not. The list portion of the control may be displayed dropped down or dropped up when the user selects
    the drop-down button next to the control. The currently selected item (if any) in the list is displayed.
    In addition, if the user types a character in the header, the list box is opened. If the type of
    combo box is to insert an input string, a new item is created and inserted to the list. After the combo box is
    opened, the next item is selected that matches that initial string in the header. The list is implemented using
    CGUIListBox class and the header is implemented with CGUIComboBoxHeader.

    The combo box has two possible layouts, either the list is under header or above the header. The layout is decided
    automatically according to the space under the header or above the header. If there is enough space under header,
    the combo box has the layout of having list box under header, otherwise it checks whether there is enough space
    above header. It there is, then it turns out to have the list box above header.

    The height of the combo box is adjusted according to the items that are inserted into the list box. If there is any
    change affect the height of the combo box, the height of the combo box is adjusted automatically.

    It is possible to open or close the list box if the user clicks the drop-down button in the header.
    The list will also be opened when the user types character in the editable input field.
    The item can be selected into the combo box by clicking the item in the list box or pressing the GK_ACTION key if
    the selected item in the list box is focused.

    The items in the combo box can be inserted by AddItem and removed by RemoveItem methods.

    Key events are handled as follows:
    -# If the drop down button is focused and the GK_ACTION key is pressed, the list is opened and the selected item is focused.
    -# Entering characters in the header (if it is set to editable) will either search for an entry in the listbox which matches
       the entered search-string (if mode is INPUT_SEARCHED) or create a new entry within the ListBox (if mnode is INPUT_INSERTED).
    -# If the user hits the GK_ACTION key while focusing an entry in the listbox, the respective entry is selected and the text of
       header bar is updated accordingly. The list is then closed.
    -# Pressing GK_ESCAPE key closes the list, if is currently open.

    It is possible to execute a user callback after an item is selected into header.

    Example:
@code
    // Create a combo box
    CGUIComboBox *pComboBox = new CGUIComboBox( &GETGUI, eC_FromInt(100), eC_FromInt(40), eC_FromInt(110), eC_FromInt(20), eC_FromInt(20));
    // Create some dummy content
    const eC_Char* kContent[] = { "Earth", "Mars", "Venus", "Jupiter", "Saturn" };
    for(int i=0;i<5;i++)
    {
        CGUIListItem * pListItem = new CGUIListItem(NULL, 0, 0, eC_FromInt(80), eC_FromInt(20), kContent[i] );
        // Modify the newly created item's label
        pListItem->GetLabel()->SetAligned(CGUIText::V_CENTERED);
        pListItem->GetLabel()->SetTextColor(0xff000000, 0xffffffff, 0xff000000, 0xffffffff);
        // Add it to the ComboBox
        pComboBox->AddItem(pListItem);
    }
    // Set some visualization parameters. (e.g. Colors and Images)
    pComboBox->SetItemSelectedColor(0xff0000cc);
    pComboBox->SetHeaderButtonImages(
        IMG_STDCTRL_IMGBTN_STANDARD,
        IMG_STDCTRL_IMGBTN_PRESSED,IMG_STDCTRL_IMGBTN_HIGHLIGHTED,
        IMG_STDCTRL_IMGBTN_GRAYED_OUT,IMG_STDCTRL_IMGBTN_FOCUSED);
    pComboBox->GetHeader()->SetInputFieldImages(
        IMG_STDCTRL_INPUTFIELD_STANDARD, 
        IMG_STDCTRL_INPUTFIELD_HIGHLIGHTED, 
        IMG_STDCTRL_INPUTFIELD_FOCUSSED, 
        IMG_STDCTRL_INPUTFIELD_GRAYEDOUT);
    // Select an item by index
    pComboBox->SetSelection(2);
    // Make the comboxbox-header editable, so that users can enter a search-string
    pComboBox->SetHeaderEditable(true);
@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIComboBox : public CGUICompositeObject, public CGUIObserver
{
public:

    /// Decide whether the input string in the header is taken as an element or the input string is just searched
    /// in the list and the most matched element is selected into header.
    enum InputActionType_t
    {
        INPUT_INSERTED, ///< The input string is inserted to the list after the user finishes typing
        INPUT_SEARCHED  ///< The typed input string is searched and the matched string is selected into header.
    };

    /** CGUIComboBox standard constructor.
        @see CGUICompositeObject().
    */
    CGUIComboBox();

    /** CGUIComboBox constructor.
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object
        @param vY Y-position of the combo box left top corner of the header relative to the combo box's parent object
        @param vWidth Width of the composite object
        @param vHeight The height of the combo box when it is closed. It equals to the height of the header.
        @param vHeaderButtonWidth the width of the header button.
        @param eID Object Identifier of this composite object (choose NO_HANDLE if none is required)
    */
    CGUIComboBox(CGUICompositeObject *const pParent, const eC_Value &vX,
        const eC_Value &vY, const eC_Value &vWidth, const eC_Value &vHeight,
        const eC_Value &vHeaderButtonWidth, const ObjectHandle_t &eID = NO_HANDLE);

    /// This destructor is automatically virtual, as the base class destructor is virtual
    virtual ~CGUIComboBox();

#ifndef GUILIANI_NO_DATAPOOL
    /** If the given DataPoolEntry contains an array of values, they will be added into the List of the CGUIComboBox.
        If it contains a single value, it will be entered into the Header of the CGUIComboBox.
        @see AddItem(), SetHeaderText()
        @param rkValue The DataPoolEntry containing the new value(s)
        @return Always True.
    */
    virtual eC_Bool SetValue( CDataPoolEntry& rkValue);
#endif

    /** Sets the text within the Header of the CGUIComboBox.
        @see SetHeaderText()
        @param rkValue The CGUIValue containing the new value
        @return Always True.
    */
    virtual eC_Bool SetValue( const CGUIValue& rkValue)
    {
        SetHeaderText(rkValue.ToString());
        InvalidateArea();
        CGUIObject::SetValue( rkValue);
        return true; 
    }

    /** Opens the combo box by setting its drop-down list visible. It also determines
        the opening direction of the combo box according to the location and height
        of all parents to avoid clipping of the opened list box. It also
        selects an item in the list. The item is chosen either
        by exactly matching the current header text or by searching for the first item
        containing the current header text.

        If the list box is empty (no combo box items present), the box is never
        opened.
    */
    virtual void Open();

    /** Closes the combo box by setting its drop-down list invisible.
        The focus is transferred to the header. The height of the combo box
        is changed to be same as the header.

        If the combo box is not open, this method does nothing.
      */
    virtual void Close();

    /** Check whether the combo box is open.
        @return True if it is open; otherwise False.
    */
    eC_Bool IsOpen() const;

    /** Add an item to the container object of this list box. 
        The height of the combo box is automatically adjusted if desired.
        @param pItem Pointer to the item which will be added to the container object's childlist
                     and the item list of this class.
        @param bIsAutoAdjustArrange Flag to enable and disable the autoadjust.
                     Defaults to true = enable autoadjust.
        @return True if the object was added, otherwise False. */
    eC_Bool AddItem(CGUIListItem* pItem, eC_Bool bIsAutoAdjustArrange = true);

    /** Add an item to the container object of this list box.
        The height of the combo box is automatically adjusted if desired.
        @param kText Text which shall be shown on the new item
        @param bIsAutoAdjustArrange Flag to enable and disable the autoadjust.
                     Defaults to true = enable autoadjust.
        @return true if the object was added, otherwise false. */
    eC_Bool AddItem(const eC_String& kText, eC_Bool bIsAutoAdjustArrange = true);

    /** Removes an item from the scrolled container.
        The height of the combo box is automatically adjusted if desired.
        Objects are removed from the internal container of scrolled objects
        and the item list.
        @param pItem Item to be removed.
        @param bIsAutoAdjustArrange Flag to enable and disable the autoadjust.
                     Defaults to true = enable autoadjust.
        @return True if the item was removed, otherwise False. */
    eC_Bool RemoveItem(CGUIListItem* pItem, eC_Bool bIsAutoAdjustArrange = true);

    /** Removes an item with given item string from the scrolled container.
        Objects are removed from the internal container of scrolled objects
        and the item list. If there are more than one items with same text,
        only the first item is removed.
        The height of the combo box is automatically adjusted if necessary.
        @param kItemStr The given item string.
        @param bIsAutoAdjustArrange Flag to enable and disable the autoadjust.
                     Defaults to true = enable autoadjust.
        @return True if there is one item is removed, otherwise False. */
    eC_Bool RemoveItem(const eC_String& kItemStr, eC_Bool bIsAutoAdjustArrange = true);

    /// Removes all list items and the header text.
    /// The height of the combo box is automatically adjusted if necessary.
    void Reset();

    /** Reimplemented to fill the control with background color.
        If multiple selection mode is set, it draws the dragged frame.
        @return True if something was painted, otherwise False.
    */
    virtual eC_Bool DoDraw();

    /** Get the current selected item index.
        @return The index of the currently selected item.
    */
    inline eC_Int GetSelection() const
    {
        if(m_pListBox) return m_pListBox->GetSelection();
        return -1;
    }

    /** Set the selected item. This call is directly passed to
        CGUIListBox::SetSelection.
        @param iIndex Index of the item to be selected.
    */
    void SetSelection(eC_Int iIndex);

    /** Set the color for the selected item. Directly passed to
        CGUIListBox::SetItemSelectedColor.
        @param uiItemSelectedColor Background color for selected items.
    */
    inline void SetItemSelectedColor(eC_UInt uiItemSelectedColor) const
    {
        if(m_pListBox) m_pListBox->SetItemSelectedColor(uiItemSelectedColor);
    }

    /** Get the number of the items that are in the item list.
        @return count of items in the list.
    */
    inline eC_UInt GetItemCount() const
    {
        if(m_pListBox) return m_pListBox->GetItemCount();
        return 0;
    }

    /** Get item in the list box by given index.
        @param uiIndex The given index.
        @return The list item corresponds to the index.
    */
    inline CGUIListItem* GetItem(eC_UInt uiIndex) const
    {
        if(m_pListBox) return m_pListBox->GetItem(uiIndex);
        return NULL;
    }

    /** Get the item string of the selected item.
        @return The string related to the selected item in the combo box.
    */
    inline eC_String GetSelectedItemStr() const
    {
        if(m_pListBox) return m_pListBox->GetSelectedItemStr();
        return "";
    }

    /** Returns the ListBox pointer.
        @return GUIListBox pointer.
    */
    inline CGUIListBox* GetListBox() const
    {
        return m_pListBox;
    }

    /** Find the item index with the given item string.
        @param kItemStr The given item string that should be found.
        @return The index of the found item, or -1 if no item was found.
    */
    inline eC_Int FindItem(const eC_String& kItemStr) const
    {
        if(m_pListBox) return m_pListBox->FindItem(kItemStr);
        return -1;
    }

    /** Set the width of the header button.
        @param vHeaderButtonWidth The width of the header button.
    */
    inline void SetHeaderButtonWidth(const eC_Value& vHeaderButtonWidth) const
    {
        if(m_pHeader) m_pHeader->SetButtonWidth(vHeaderButtonWidth);
    }

    /** Sets the images of the drop-down button in the header.
        Directly passed to CGUIComboBoxHeader::SetButtonImages.
        @param eBtnStandard Image used when in normal state
        @param eBtnPressed Image used when in pressed state
        @param eBtnHighlighted Image used when in highlighted state
        @param eBtnGrayedOut Image used when in grayed-out state
        @param eBtnFocused Image used when in focused state
    */
    inline void SetHeaderButtonImages(
        const ImageResource_t &eBtnStandard,
        const ImageResource_t &eBtnPressed,
        const ImageResource_t &eBtnHighlighted,
        const ImageResource_t &eBtnGrayedOut,
        const ImageResource_t &eBtnFocused) const
    {
        if(m_pHeader) m_pHeader->SetButtonImages(eBtnStandard,
            eBtnPressed, eBtnHighlighted, eBtnGrayedOut, eBtnFocused);
    }

    /// Set the header as editable or not.
    inline void SetHeaderEditable(const eC_Bool& bEditable) const
    {
        if(m_pHeader) m_pHeader->SetEditable(bEditable);
    }

    /** Set the string for the header.
        @param kStr New text for the header. */
    inline void SetHeaderText(const eC_String& kStr) const
    {
        if(m_pHeader) m_pHeader->SetText(kStr);
    }

    /** Get the text that is currently displayed in the header.
        @return The header text. */
    inline eC_String GetHeaderText() const
    {
        if(m_pHeader) return m_pHeader->GetText();
        return "";
    }

    /// Returns the ComboBoxHeader
    inline CGUIComboBoxHeader* GetHeader() const
    {
        return m_pHeader;
    }

    /** Sets the color used as the background for selected text.
        @param uiCol The selected text color.
        @see CGUIBaseInputField::SetSelectedTextColor()
      */
    inline void SetHeaderSelectedTextColor(eC_UInt uiCol) const
    {
        if(m_pHeader)
            m_pHeader->SetSelectedTextColor(uiCol);
    }

    /** Get the color currently used as the background for selected text.
        @return the selected text color.
        @see CGUIBaseInputField::GetSelectedTextColor()
      */
    inline eC_UInt GetSelectedTextColor() const
    {
        if(m_pHeader)
            return m_pHeader->GetSelectedTextColor();
        return 0;
    }

    /** Get the label of the header .
        Use this to obtain a pointer to the internal text for formatting etc.
        @return Pointer to the internal text label.
    */
    inline CGUIEditableText* GetHeaderLabel() const
    {
        if(m_pHeader)
            return m_pHeader->GetLabel();
        return NULL;
    }

    /** Changes the internal text. This call is directly passed through to
        CGUIHeader::SetLabel.
        @param pkText The new text to be used by the internal edit control of the header.
      */
    inline void SetLabel(CGUIEditableText* pkText) const
    {
        if(m_pHeader)
            m_pHeader->SetLabel(pkText);
    }

    /** Attaches a command to this control. This command is executed whenever
        one of the items is selected into the header.
        @param pCommand The command to attach to this control.
    */
    inline void SetCommand(CGUICommandPtr pCommand) {m_pCommand = pCommand;}

    /** Close the combobox if the user clicks anywhere outside of the ComboBox
        @param pEvent The event to process
        @return False
    */
    virtual eC_Bool DoUserEvent(CGUIEvent* const pEvent);

    /**
        Close the combobox in reaction to GK_ESCAPE events.
        @param eKeyID The Guiliani key identifier.
        @param uiModifiers The key modifiers bitfield.
        @return True if the combobox was closed.
    */
    virtual eC_Bool DoKeyDown(const GUIKeyIdentifier_t& eKeyID, const eC_UInt& uiModifiers);

    /** If the ComboBox is configured to run in mode INPUT_SEARCHED, then incoming character events will be used to search the
        entries of the Listbox for matches. If a matching entry is found, it will automatically be selected.
        @param uiKey The keycode.
        @param eKeyIdentifier The Guiliani key identifier.
        @param uiModifiers The key modifiers bitfield.
        @return Always false
    */
    virtual eC_Bool DoChar( const eC_UInt &uiKey, const GUIKeyIdentifier_t &eKeyIdentifier, const eC_UInt &uiModifiers);

    /** Observes CGUIComboBoxHeader to get informed if input finished.
        @param kObservedValue The update observed value.
        @param pkUpdatedObject The observed object.
        @param uiX X-Index Additional X-Index in case the updated value is part of a multidimensional array
        @param uiY Y-Index Additional Y-Index in case the updated value is part of a multidimensional array
    */
    virtual void OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX=0, const eC_UInt uiY=0);

    /** Calls the base class implementation and sets the children's width to
        the proper value. The width of items and list box is adjusted too.
        @param vWidth The new width.
      */
    virtual void SetWidth(const eC_Value& vWidth);

    /** Changes the height of the combo box when it is closed. The height of header is also changed
        to be the given height.
        @param vHeight The new height.
      */
    virtual void SetHeight(const eC_Value& vHeight);

    /** Get the previously selected item string.
        @return The previsouly selected item string.
    */
    inline eC_String GetPreviousSelection() const {return m_kPrevSelectedStr;}

    /** Set the input action type for the combo box.
        @param eInputActionType The input action type.
    */
    inline void SetInputActionType(const InputActionType_t& eInputActionType)
    {
        m_eInputActionType = eInputActionType;
    }

    /** Get the input action type of the combo box.
        @return The active input action type.
    */
    inline InputActionType_t GetInputActionType() const {return m_eInputActionType;}

    /** After the combo box loses focus and the focused object is not the descendant
        of this combo box, we close this combo box.
    */
    virtual void LoseFocus();

    /** Add an observer to get informed when the selected index is changed.
        CGUIObserver::OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject)
        will be called whenever the index has changed.
        @param pObserver The Observer that wants to subscribe.
        @ingroup GUILIANI_SUBJECTS
    */
    void AddSelectionObserver(CGUIObserver* pObserver);

    /** Unsubscribe a selection observer.
        @param pObserver The Observer that wants to unsubscribe.
    */
    void RemoveSelectionObserver(CGUIObserver* pObserver);

    /** Set the frame border width around the list box. The list box's width, height, position,
        the header's position and the height of the combo box may also be adjusted.
        @param vFrameBorderWidth The frame border width.
    */
    void SetFrameBorderWidth(const eC_Value& vFrameBorderWidth);

    /** Set the frame border color around the list box.
        @param uiFrameBorderColor The frame border color.
    */
    void SetFrameBorderColor(const eC_UInt& uiFrameBorderColor);

    /** FocusAny will call FocusSelectedItem internally.
        @return False if no item was selected, otherwise it returns the result of RequestFocus() on the selected item. */
    eC_Bool FocusAny();

    /** This will focus the currently selected item within the listbox (if any)
        @return False if no item was selected. Otherwise it returns the result of RequestFocus() on the selected item. */
    eC_Bool FocusSelectedItem();

    /**
        Toggle GrayedOut state of the combo box.
        @param bGrayedOut Defines if the combo box is grayed out or not.
    */
     void SetGrayedOut(const eC_Bool &bGrayedOut);
    /**
        Toggle Disabled state of the combo box.
        @param bDisabled Defines if the combo box is disabled out or not.
    */
     void SetDisabled(const eC_Bool& bDisabled);

    /**
        Arranges the entries of the combo box vertically. Call this method after
        adding or removing entries without activated auto adjustment.
        @see AddItem() and RemoveItem()
    */
    void AdjustComboBox();


#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif // GUILIANI_STREAM_GUI
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif // GUILIANI_WRITE_GUI

protected:
    /** Handle the event after user clicks the control.
        If the button is clicked, the list box is opened and the selected item gets the focused or closed.
        If an item in the list box is clicked, the item is selected and the text of the item is set into the
        header. If a command is available, it is executed.
        @param  vAbsX X-position in screen coordinates where the click occurred
        @param  vAbsY Y-position in screen coordinates where the click occurred
        @return True The event has been handled.<BR>
                False The event has not been handled.
    */
    virtual eC_Bool DoClick(const eC_Value &vAbsX = eC_FromInt(-1), const eC_Value &vAbsY = eC_FromInt(-1));

    /// Overridden to NULL internal object pointers in case the Header or List gets destroyed
    void RemoveObject(CGUIObject *pObject);

private:

    /// Possible positions of the open list relative to header.
    enum ListPosition_t
    {
        UNDER_HEADER,  ///< The list is under the header.
        ABOVE_HEADER   ///< The list is above the header.
    };

    /** Updates the string in the combobox header with a valid entry from within the Listbox.
        This will first check if the header string already exactly matches an entry in the Listbox.
        If not, it will search for an entry in the listbox which contains the given substring.
        If this fails, too, it will simply select the Listbox's first item and update the header accordingly. */
    void UpdateHeaderString();

    /** Determine whether the list must be opened under the header or above the header according
        to the location and size of all parents.
    */
    void DetermineListDirection();

    /** Copy-constructor of CGUIComboBox.
        This constructor is private, because we do not know the items for this combo box,
        thus we can not copy all the items.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGUIComboBox(const CGUIComboBox& kSource);

    /** Operator = method of CGUIComboBox.
        This constructor is private, because we do not know the items for this combo box,
        thus we can not copy all the items.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGUIComboBox& operator=(const CGUIComboBox& kSource);

    /// General initialization. Helper for constructors.
    void Init(const eC_Value &vHeaderButtonWidth);

    /** The combo box header 
        @note NULL is also valid here.
    */
    CGUIComboBoxHeader* m_pHeader;

    /** The list that contains items for this combo box. 
        @note NULL is also valid here.
    */
    CGUIListBox* m_pListBox;

    /// User action after the user selected one item from the combo box list into header.
    CGUICommandPtr m_pCommand;

    /// Stores the previously selected item string that is shown in header.
    eC_String m_kPrevSelectedStr;

    /** Decide whether the input string in the header is taken as an element or the input string is just searched
        in the list and the most matched element is selected into header. Default it searched in the list for
        the matched element.
    */
    InputActionType_t m_eInputActionType;

    /// keeps track of input field status.
    eC_Bool m_bEditing;

    /// Subject informing observers whenever the selected index changes.
    CGUISubject m_kSelectedIndexSubject;

    /// The frame border width around the list box.
    eC_Value m_vFrameBorderWidth;

    /// The frame background color to draw a frame around the list box.
    eC_UInt m_uiFrameBorderColor;
};

#endif
