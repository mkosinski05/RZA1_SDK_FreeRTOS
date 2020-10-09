/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef CGUIMENUBASE__H_
#define CGUIMENUBASE__H_

#include "GUIRepositionCompositeObject.h"
#include "eC_TList_doubleLinked.h"
#include "GUIMenuAndItem.h"
#include "GUIImageResource.h"

class CGUIMenuBar;

/// Base class for Menus

/** This class serves as a base class for menus. You must never instantiate objects of this class directly.
    Instead please refer to CGUIMenuBar and CGUIMenu.
*/
class CGUIMenuBase
    : public CGUIRepositionCompositeObject
{
public:

    /// This will set the focus to the currently selected menu item (if any) and call the base class implementation otherwise
    virtual eC_Bool FocusAny();

    /// Deselects any potentially selected item in the menu and automatically closes any related menus
    virtual void ReleaseSelection();

    /** Indicate whether the menu is a submenu.
        @return True if it is a submenu, otherwise False.
    */
    inline eC_Bool IsSubmenu() const { return m_bIsSubmenu; }

    /// Indicates whether the menu is currently open or closed
    eC_Bool IsOpen() { return !IsInvisible(); }

    /** Adds a menu item, item separator and corresponding submenu to the list.
        If there is no item separator or no submenu, pass NULL.
        @param pMenuItem Pointer to a CGUIMenuItem object.
        @param pItemSeparator Pointer to an item separator object, default NULL.
        @param pSubMenu Pointer to a sub CGUIMenu object, default NULL.
        @return True if successful, False otherwise
    */
    eC_Bool AddItem(CGUIMenuItem *pMenuItem, CGUIMenu *pSubMenu = NULL, CGUIMenuItemSeparator* pItemSeparator = NULL);

    /** Removes the given menu item, item separator and corresponding submenu from the list.
        The menu item pointer, item separator pointer and the submenu pointer will be deleted
        and must not be used anymore.
        @param pMenuItem Pointer to a CGUIMenuItem object.
    */
    void RemoveItem(CGUIMenuItem *pMenuItem);

    /** Indicate the menu is a submenu.
        @param bIsSubmenu Indicate whether the menu is a submenu (true) or not (false).
    */
    inline void SetIsSubmenu(const eC_Bool bIsSubmenu) { m_bIsSubmenu = bIsSubmenu; }

    /** Set the background image for this control.
        @param eBgImage The image id of the background image.
    */
    void SetBgImage(const ImageResource_t& eBgImage);

    /** The image is always stretched to the original size of this control, regardless of the current
        size that might be adjusted to match an opened menu.
        @return True if something was painted, otherwise False.
    */
    virtual eC_Bool DoDraw();

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif // GUILIANI_STREAM_GUI

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif // GUILIANI_WRITE_GUI

    eC_Bool DoKeyDown(
        const GUIKeyIdentifier_t &eKeyIdentifier,
        const eC_UInt &uiModifiers);

protected:

    /// Marks the given menu item as being selected and releases any previous selection
    void SelectMenuItem( CGUIMenuItem* pMenuItem);

    /// Returns a pointer to the menu bar. Every menu MUST be associated with a CGUIMenuBar.
    virtual CGUIMenuBase* GetMenuBar();


    /** CGUIMenuBase constructor.
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the menu object
        @param vHeight Height of the menu object
        @param eBasePoint Menu alignment
        @param eID Object Identifier of this composite object (choose NO_HANDLE if none is required)
    */
    CGUIMenuBase(CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const CGUILayoutHelper::RepositionAlignment_t eBasePoint,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIMenuBase constructor.
        @param pParent Pointer to the designated parent object.
        @param kRect Bounding rectangle relative to parent.
        @param eBasePoint Menu alignment
        @param eID Object Identifier of this CGUIMenu object (choose NO_HANDLE if none is required)
    */
    CGUIMenuBase(CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const CGUILayoutHelper::RepositionAlignment_t eBasePoint,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIMenuBase standard constructor.
        @see CGUIObject().
    */
    CGUIMenuBase();

    virtual ~CGUIMenuBase();

    /// Initialization helper
    void Init();

    ///The list stores the menu item pointer, item separator and corresponding submenu pointer, if available.
    CGUIMenuAndItem::MenuAndItemList m_kMenuItemList;

    /** The index of the selected item in the m_kSubMenuItemList.*/
    eC_Int m_iSelectedIndex;

    ///Indicate whether the menu is a submenu or not.
    eC_Bool m_bIsSubmenu;

    /// Background image for this menu.
    ImageResource_t m_eBgImage;

    /** The object that opens this menu. It may be the menu bar
        or the parent menu if this menu is a sub menu.
    */
    CGUIMenuBase* m_pkObjOpensThisMenu;
};

#endif
