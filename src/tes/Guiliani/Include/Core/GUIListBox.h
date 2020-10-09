/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef __GUILISTBOX__H_
#define __GUILISTBOX__H_

#include "GUIScrollView.h"
#include "eC_TList_doubleLinked.h"
#include "GUIEventTypeResource.h"

class CGUIListItem;

/// Contains list items and can be scrolled if there is not enough space.
// @guiliani_doxygen toplevel_control List Box
/**
    <table border="0">
        <tr>
            <td width="200">@image html list_box.png</td>
            <td>
                The "list box" control is a scrollable container for a list of entries (list items). These entries can be selected or unselected Single and multiple selection of entries is supported. If the total height of the items is bigger than the container itself, a vertical scroll bar is shown (Class: CGUIListBox).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    This class is derived from CGUIScrollView and has all features of a scroll view.
    There is an enumeration type definition called Mode_t to indicate the show mode
    of the list box. It is possible to multiple select items inside the
    list box by specifying the parameter in the constructor.

    The items can be added by calling AddItem() and removed by RemoveItem() or RemoveItems().
    There are also interfaces for influencing all items. If multiple selection is enabled,
    an item can be selected by calling method AddSelection(). The selection of the item is released
    by calling method ReleaseSelection(). All selected items are released by
    ReleaseAllSelection().

    There are interfaces to handle the items such as GetItem(), GetNextItem, FindItem(), DeleteAllItems(),
    GetItemCount() and so on. You can obtain the selected item index list and string list by calling
    the GetSelectionIndexList() and GetSelectedItemsStrList(), respectively. If only the index or
    the item string of the first selected item is interesting, the GetSelection() and GetSelectedItemStr()
    can be used.

    If multiple selection is enabled, the user can select multiple items from the list box by dragging the mouse
    over items or by pressing SHIFT, CONTROL or both of them and clicking the mouse over item or dragging over
    items. By pressing SHIFT key and arrow key (UP, DOWN, LEFT or RIGHT key), it is also possible to select
    multiple items inside the list box.

    The arrangement of items in the list box can be adjusted by calling AdjustArrangement().
    If the width or height of the item is changed, this method should be called to adjust the arrangement.

    Example:
@code
    // Create a list box.
    CGUIListBox *pkListBox = new CGUIListBox( this,
                                    eC_FromInt(10), eC_FromInt(50),
                                    eC_FromInt(200), eC_FromInt(150),
                                    CGUIListBox::HOR_EXT,
                                    false,
                                    NO_HANDLE);

    // Fill the list with some items
    for( int i=0; i<11; i++)
    {
        CGUIListItem* pkItem = new CGUIListItem(NULL,
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(100), eC_FromInt(30),
                                        "item "+ eC_String(i),
                                        true,
                                        eC_FromInt(0), eC_FromInt(0),
                                        DUMMY_IMAGE,
                                        NO_HANDLE);
        // Vertically center the text
        pkItem->GetLabel()->SetAligned( CGUIText::V_CENTERED, CGUIText::H_LEFT);
        // Add some color to the items
        pkItem->SetBackgroundColor( 0xFFCCCCCC);
        pkItem->SetSelectedColor( 0x55FF6666);
        // Add an icon to every 2nd item
        if( i%2 == 0)
        {
            pkItem->SetIconImages( IMG_STDCTRL_RB_NOTSELECTED_NORMAL, IMG_STDCTRL_RB_SELECTED_NORMAL);
            pkItem->SetIconPos( pkItem->GetWidth()-eC_FromInt(20), eC_FromInt(5) );
        }
        // Add item into the list
        pkListBox->AddItem(pkItem);
    }

    // Update item layout
    pkListBox->AdjustArrangement();
@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIListBox
    : public CGUIScrollView
{
public:

    /// Type used for the item list that contains items.
    typedef eC_TListDoubleLinked<CGUIListItem*> ItemPtrList_t;

    /// Type used for the index of those selected items in the list box.
    typedef eC_TListDoubleLinked<eC_UInt> IndexList_t;

    /// Type used for the string of those selected items in the list box.
    typedef eC_TListDoubleLinked<eC_String> StringList_t;

    /** The enumeration specifies the layout mode of ListBoxes.
        The layout defines how items within a listbox are distributed within the available space.
        <p>
        VER_EXT: In this mode list items will be distributed row by row from left to right.
        If there is not enough room for all items to be displayed, a vertical scrollbar will be shown.
        <p>
        HOR_EXT: In this mode list items will be distributed column by column from top to bottom.
        If there is not enough room for all items to be displayed, a horizontal scrollbar will be shown.
    */
    enum Mode_t
    {
        VER_EXT,
        HOR_EXT
    };

   /** Constructor
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY X-position relative to its parent object
        @param vWidth Width of the input field
        @param vHeight Height of the input field
        @param eMode Specifies the List's layout mode
        @param bMultipleSelection Indicate whether the multiple selection is enabled or not.
        @param eID Object identifier.
    */
    CGUIListBox(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const Mode_t & eMode = VER_EXT,
        const eC_Bool & bMultipleSelection = false,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructor taking a rectangle as input.
        @param pParent Pointer to the designated parent object
        @param kRect Bounding rectangle relative to parent.
        @param eMode Specifies the List's layout mode
        @param bMultipleSelection Indicate whether the multiple selection is enabled or not.
        @param eID Object identifier.
    */
    CGUIListBox(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const Mode_t & eMode = VER_EXT,
        const eC_Bool & bMultipleSelection = false,
        const ObjectHandle_t &eID = NO_HANDLE);

    /// Destructor
    ~CGUIListBox();

    /** Add an item to the container object of this list box.
        If there is already at least one item in the list, the selected color
        used by that item is also used for the new item.
        @param pItem Pointer to the item which will be added to the container
                     object's childlist and the item list of this class.
        @param bIsAutoAdjustArrange Indicate whether after adding the item
                                    the arrangement of items is automatically
                                    adjusted or not. If it is true, the method
                                    AdjustArrangement() is called.
        @return True if the object was added, otherwise False. */
    eC_Bool AddItem(CGUIListItem* pItem, eC_Bool bIsAutoAdjustArrange = true);

    /// Removes an item from the scrolled container.
    /** Objects are removed from the internal container of scrolled objects
        and the item list.
        @param pItem Item to be removed.
        @param bIsAutoAdjustArrange Indicate whether the items arrange is adjusted automatically by the current mode.
        @return True if the item was removed, otherwise False. */
    eC_Bool RemoveItem(CGUIListItem* pItem, eC_Bool bIsAutoAdjustArrange = true);

    /** Removes bulk items, starting from start index and ending with end index, from the scrolled container.
        Objects are removed from the internal container of scrolled objects
        and the item list.
        @param uiStartIndex The start index of the bulk items.
        @param uiEndIndex The end index of the bulk items.
        @param bIsAutoAdjustArrange Indicate whether the items arrange is adjusted automatically by the current mode.
        @return True if the item was removed, otherwise False. */
    eC_Bool RemoveItems(eC_UInt uiStartIndex, eC_UInt uiEndIndex, eC_Bool bIsAutoAdjustArrange = true);

    /** Add an item to the selection list, if multiple selection is enabled.
        @param uiIndex Index of the item that is added to the selection.
        @return True if the item was selected; False otherwise.
    */
    eC_Bool AddSelection(eC_UInt uiIndex);

    /** Set the background color for this control.
        @param uiBgColor The background color of this control.
    */
    inline void SetBgColor(eC_UInt uiBgColor)
    {
        m_uiBgColor = uiBgColor;
        InvalidateArea();
    }

    /** Get the first selected item from the selected item index list.
        @return Index of currently selected item, or a value of -1 if there is no selection.
    */
    eC_Int GetSelection();

    /** Get the current selected item index list.
        @return Index list of currently selected item.
    */
    inline IndexList_t GetSelectionIndexList() { RegenerateIndexStringList(); return m_kSelIndexList; }

    /** Sets the item with given index as selected. The previously selected
        item will be deselected.
        @param uiIndex Index of the item to be selected.
    */
    void SetSelection(eC_UInt uiIndex);

    /** Deselect an item.
        @param uiIndex The index of item that is set to be deselected.
    */
    void ReleaseSelection(eC_UInt uiIndex);

    /** Release all selections.
    */
    void ReleaseAllSelection();

    /** Get the item string from the first selected item of the selected item list.
        @return The text string of the first selected item, or an empty
                string if there is no selection.
    */
    eC_String GetSelectedItemStr();

    /** Get the current selected item string list.
        @return String list of currently selected items.
    */
    inline StringList_t GetSelectedItemsStrList() { RegenerateIndexStringList(); return m_kSelStringList; }

    /** Set the background color for the selected items in this list.
        This method calls CGUIListItem::SetSelectedColor on all items that are
        currently in the list.
        @param uiItemSelectedColor Background color for selected items.
    */
    void SetItemSelectedColor(eC_UInt uiItemSelectedColor);

    /** Set the background color for the dragged frame.
        @param uiDraggedFrameBgColor Background color for the dragged frame.
    */
    void SetDraggedFrameBgColor(eC_UInt uiDraggedFrameBgColor)
    {
        m_uiDraggedFrameBgColor = uiDraggedFrameBgColor;
    }

    /** Get the number of the items that are in the item list.
        @return how many items in the list.
    */
    eC_UInt GetItemCount() const;

    /** Get item in the list box by given index.
        @param uiIndex The given index.
        @return The list item corresponding to the index, or NULL if the index
                is invalid.
    */
    CGUIListItem* GetItem(eC_UInt uiIndex);

    /** Get the next item of currently accessed item in the list box. You must call GetItem() prior to using GetNextItem().
        If the current accessed item iterator or the next item iterator is not valid, it returns NULL.
        @return The next item, or NULL if the current accessed item iterator or the next item
                iterator is not valid.
    */
    CGUIListItem* GetNextItem();

    /** Selects the given object if it is an item within the list.
        @param pObj Pointer to object which shall be selected
        @return True if it was selected, False otherwise (e.g. if the object is not an item in the list)
    */
    eC_Bool SelectObject(const CGUIObject* pObj);

    /** Check whether the given object is an item of the list box.
        @param pObj The object that is to be checked.
        @return True, if it is, False otherwise.
    */
    eC_Bool IsObjectAnItemInList(const CGUIObject* pObj) const;

    /** Reset the item list to delete all the items from the list.
    */
    void DeleteAllItems();

    /** Find the item index with the given item string.
        @param kItemStr The given item string that should be found.
        @return The index of the found item. If there is no item to be found, it returns -1.
    */
    eC_Int FindItem(const eC_String& kItemStr) const;

    /** Find the item index which fits the given search string best.
        This will also match items which contain the search string only as a substring.
        The "best match" will be the item which contains the search string at the smallest character index.
        @param kSearchString The (sub)string for which to search.
        @return The index of the found item. If there is no item to be found, it returns -1.
    */
    eC_Int FindItemBySubstring(const eC_String& kSearchString) const;

    /** Change the width of the control and arrange the items inside list box to
        be in order according to the arrange mode.
        @param vWidth The new width
    */
    void SetWidth(const eC_Value &vWidth);

    /** Change the height of the control and arrange the items inside list box to
        be in order according to the arrange mode.
        @param vHeight The new height
    */
    void SetHeight(const eC_Value &vHeight);

    /** Reimplemented to fill control with background color.
        @return True if something was painted, otherwise False.
    */
    virtual eC_Bool DoDraw();

    /** Reimplemented to draw a frame marking the selected region in multiselection mode.
        @return True if something was painted, otherwise False.
    */
    eC_Bool DoPostDraw();

    /** Draws a selection frame above the list's content if multiselection is enabled
        @param vAbsX Mouse position in X where event occurred
        @param vAbsY Mouse position in Y where event occurred
        @return Always false
    */
    virtual eC_Bool DoClick(const eC_Value &vAbsX = ((eC_Float)(-1)),
                            const eC_Value &vAbsY = ((eC_Float)(-1)));

    /** Requests the Focus for the selected item
        @return eC_Bool True if focus was obtained, False otherwise
    */
    virtual eC_Bool RequestFocusForSelection();

    /** Handle the key UP, DOWN event to select item.
        @param eKeyID The Guiliani key identifier.
        @param uiModifiers The key modifiers bitfield.
        @return True If the event has been handled.<BR>
                False Else.
    */
    eC_Bool DoKeyDown(
        const GUIKeyIdentifier_t& eKeyID,
        const eC_UInt& uiModifiers);

    /** Handle the event after the mouse enter to one of the item in the normal mode.
        In normal mode, the item that is highlighted can be selected.
        In other two mode, nothing happens.
        @param vAbsX Mouse position in X where event occurred
        @param vAbsY Mouse position in Y where event occurred
        @return Always False
    */
    virtual eC_Bool DoMouseEnter(
        const eC_Value &vAbsX,
        const eC_Value &vAbsY);

    /** According to the current layout mode (either HOR_EXT or VER_EXT), the method
        adjusts the arrangement of items within the list box.
        Please @see Mode_t for a definition of the available layout modes.
        If an item changes width or height, this method must be called to refresh the
        arrangement of the list box.
    */
    void AdjustArrangement();

    /** Handle the event after mouse button is pressed.
        In this method the position of the click-pointer is obtained.
        @param  vAbsX X-position in screen coordinates where the click occurred
        @param  vAbsY Y-position in screen coordinates where the click occurred
        @return True The event has been handled.<BR>
                False The event has not been handled.
    */
    virtual eC_Bool DoButtonDown(const eC_Value &vAbsX, const eC_Value &vAbsY);

    /** Handle selection after releasing the mouse button.
        @param vAbsX Mouse position in X where event occurred
        @param vAbsY Mouse position in Y where event occurred
        @return Always False
    */
    virtual eC_Bool DoDragEnd(const eC_Value &vAbsX, const eC_Value &vAbsY);

    /** If it is the multiple selection is enabled, this method calculates
        the dimension and position the dragged frame in the list box.
        @param vDeltaX Mouse movement in X since last frame
        @param vDeltaY Mouse movement in X since last frame
        @param vAbsX Mouse position in X where event occurred
        @param vAbsY Mouse position in Y where event occurred
        @return Always False
    */
    virtual eC_Bool DoDrag(const eC_Value &vDeltaX, const eC_Value &vDeltaY, const eC_Value &vAbsX, const eC_Value &vAbsY);

    /** Returns the index of the given object within the list of items.
        @param pObj The object
        @return Index within item-list or -1 if the object is not within the list.
    */
    eC_Int GetItemIndexOfObject(const CGUIObject* pObj) const;


#ifdef GUILIANI_STREAM_GUI
    /** Standard constructor. Only to be called by factory.
        No user code should call this constructor, not even in
        streaming mode (that is, when GUILIANI_STREAM_GUI is defined)!
        @see CGUISlider()
    */
    CGUIListBox();

    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

    /** Enable or disable multiple selection.
        @param bMultiSelection New value
    */
    inline void SetMultipleSelection(eC_Bool bMultiSelection)
    {
        m_bMultipleSelection = bMultiSelection;
    }

    /** Enables or disables the selection by highlighting functionality.
        @param bSelectedByHighlighting New value
    */
    inline void SetSelectedByHighlighting(eC_Bool bSelectedByHighlighting)
    {
        m_bSelectedByHighlighting = bSelectedByHighlighting;
    }

    /** Defines whether all list items will automatically be resized to the largest item's dimensions.
        @param bUnifyItemSize New value
    */
    void SetUnifyItemSize( const eC_Bool bUnifyItemSize) { m_bUnifyItemSize = bUnifyItemSize; }

    /** Sets the focus to a currently selected item within the list (if there IS a selection)
        @return True if focus was obtained, False otherwise
    */
    eC_Bool FocusAny()
    {
        if( GetSelection() != -1)
            return RequestFocusForSelection();
        else
            return CGUIScrollView::FocusAny();
    }

    /** Add an observer to get informed when the selection has changed.
        CGUIObserver::OnNotification(const CGUIObject* const pkUpdatedObject)
        will be called whenever the selected indices have changed.
        @param pObserver The Observer that wants to subscribe.
        @ingroup GUILIANI_SUBJECTS
    */
    void AddSelectionObserver(CGUIObserver* pObserver)
    {
        m_kSelectedIndexSubject.AddObserver(pObserver);
    }

    /** Unsubscribe a selection observer.
        @param pObserver The Observer that wants to unsubscribe.
    */
    void RemoveSelectionObserver(CGUIObserver* pObserver)
    {
        m_kSelectedIndexSubject.RemoveObserver(pObserver);
    }

protected:
    /// General initialization
    void Init();

    /// Reimplemented to handle selection behaviour when being operated via keyboard.
    virtual void GetFocus();

    /** Updates the selection status of the given object with regard to the corresponding event.
        This takes multi-selection via CTRL/SHIFT into account.
        @param pObject Pointer to object which shall be (de)selected
        @param pEvent Pointer to event which caused the (de)selection. Required for checking CTRL/SHIFT states. */
    void UpdateSelection(const CGUIObject* pObject, const CGUIEvent* pEvent);

private:
    /** This prototype is declared only to prevent auto-generation by the compiler.
        There is no implementation for this method. */
    CGUIListBox(const CGUIListBox& kSource);

    /** This prototype is declared only to prevent auto-generation by the compiler.
        There is no implementation for this method. */
    CGUIListBox& operator =(const CGUIListBox &kSource);

    /** Override this method to prevent adding any object to this class.
        To add item to the list, the AddItem method must be used.*/
    eC_Bool AddObject( CGUIObject* pObject){return CGUIScrollView::AddObject(pObject);}

    /// Override this method to prevent removing any object from this class.
    /// To remove an item from the list, the RemoveItem method can be used.
    void RemoveObject(CGUIObject *pObject){return CGUIScrollView::RemoveObject(pObject);}

    /** Adjust arrangement in VER_EXT mode.
        @param bVerBarIsVisible Indicates whether the vertical bar is visible or not.
    */
    void AdjustArrangementVER_EXT(eC_Bool bVerBarIsVisible = false);

    /** Adjust arrangement in the HOR_EXT mode.
        @param bHorBarIsVisible Indicates whether the horizontal bar is visible or not.
    */
    void AdjustArrangementHOR_EXT(eC_Bool bHorBarIsVisible = false);

    /** if multiple selection is enabled, this method selects items overlapped with the dragged frame.
        @param bShiftPressed Indicates whether the SHIFT key is pressed or not.
        @param bCtrlPressed Indicates whether the CONTROL key is pressed or not.
    */
    void DragAndSelectItems(const eC_Bool bShiftPressed, const eC_Bool bCtrlPressed);

    /** Helps handling the LEFT and RIGHT events in the HOR_EXT mode,
        or handling the UP and DOWN events in the VER_EXT mode. The next
        item that is to be slected is the left or right neighbour in HOR_EXT mode
        and the over or under neighbour in VER_EXT mode. This function is used to
        check whether the given checked index is the item going to be selected.
        If it is the one that is going to be selected, the method returns true.
        Otherwise, it returns False.
        @param uiCurIndex Indicates the index of the current focused item.
        @param uiCheckedIndex Indicates the index of the checked item that is
               is going to be selected.
        @return True if the checked item is found; otherwise False.
    */
    eC_Bool HandleEventInMode(const eC_UInt &uiCurIndex, const eC_UInt &uiCheckedIndex);

    /** Helps handling the UP and DOWN events in the HOR_EXT mode,
        handling the LEFT and RIGHT events in the VER_EXT mode.
        or the UP, LEFT, DOWN and RIGHT int the normal mode.
        @param uiCurIndex Indicates the index of the current focused item.
        @param eKeyID Indicates the key ID.
        @param bShiftPressed Indicates whether the SHIFT key is pressed or not.
        @param bCtrlPressed Indicates whether the CONTROL key is pressed or not.
        @return True indicates the given item does exist and selected; otherwise False.
    */
    eC_Bool HandleNormalEvent(const eC_UInt &uiCurIndex, const GUIKeyIdentifier_t &eKeyID,
                              const eC_Bool bShiftPressed, const eC_Bool bCtrlPressed);

    /** Handles the combination of KEY_DOWN SHIFT EVENT and mouse click event or
        the combination of KEY_DOWN CONTROL EVENT and mouse click event or
        the combination of KEY_DOWN SHIFT and KEY_DOWN CONTROL key and mouse click event.
        @param uiItemPressedIndex The current pressed item index.
        @param bShiftPressed Indicates whether the SHIFT key is pressed or not.
        @param bCtrlPressed Indicates whether the CONTROL key is pressed or not.
        @return True, if everything does smoothly; False otherwise.
    */
    eC_Bool HandleShiftAndCtrlClickEvent(const eC_UInt& uiItemPressedIndex,
                                         const eC_Bool bShiftPressed,
                                         const eC_Bool bCtrlPressed);

    /** Helps function for handling the SHIFT key event.
        @param uiCurFocusedIndex The current focused item index.
        @param uiNextFocusedIndex The next focused item index.
        @param bShiftPressed Indicates whether the SHIFT key is pressed or not.
        @param bCtrlPressed Indicates whether the CONTROL key is pressed or not.
    */
    void HelpHandleShiftKeyEvent(const eC_UInt& uiCurFocusedIndex, const eC_UInt& uiNextFocusedIndex,
                                 const eC_Bool bShiftPressed, const eC_Bool bCtrlPressed);

    /** Regenerate the index list and the string list.
    */
    void RegenerateIndexStringList();

    /** List containing all the items that are in the list box.
    */
    ItemPtrList_t m_kItemList;

    /** List of indices of currently selected items
    */
    IndexList_t m_kSelIndexList;

    /** List of strings of currently selected items
    */
    StringList_t m_kSelStringList;

    /** The background color of this list box.
    */
    eC_UInt m_uiBgColor;

    /** The background color of the dragged frame.
    */
    eC_UInt m_uiDraggedFrameBgColor;

    /** The layout mode of the list box.
    */
    Mode_t m_eMode;

    /** Indicates whether multiple selection is enabled or not.
    */
    eC_Bool m_bMultipleSelection;

    /// Rectangle of the selection frame when selecting various items via the mouse
    CGUIRect m_kSelectionFrame;

    /** The absolute X position of the last ButtonDown-Event within the listbox (used for dragging)
    */
    eC_Value m_vAbsXPosClickPoint;

    /** The absolute Y position of the last ButtonDown-Event within the listbox (used for dragging)
    */
    eC_Value m_vAbsYPosClickPoint;

    /** Indicate whether the index and the string list must be updated.
    */
    eC_Bool m_bIndexStringListDirty;

    /// The currently accessed item iterator. It is updated every time, an item is obtained.
    ItemPtrList_t::Iterator m_kCurAccessedItemIter;

    /// Indicates whether the list box can be selected by highlighting the item.
    eC_Bool m_bSelectedByHighlighting;

    /// Defines whether all list items will automatically be resized to the largest item's dimensions
    eC_Bool m_bUnifyItemSize;

    /// Subject informing observers whenever the selected index changes.
    CGUISubject m_kSelectedIndexSubject;
};
#endif
