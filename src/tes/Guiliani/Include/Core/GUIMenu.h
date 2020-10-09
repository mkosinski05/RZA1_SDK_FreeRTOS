/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef CGUIMENU__H_
#define CGUIMENU__H_

#include "GUIMenuBase.h"
#include "GUIRepositionCompositeObject.h"
#include "GUIMenuAndItem.h"


/// An image-based pull-down menu.

/** A CGUIMenu serves as a pull-down menu containing numerous menu-items which can open additional sub-menus.
    Instances of CGUIMenu may only be attached to either another CGUIMenu (=they are sub-menus) or directly
    to a CGUIMenuBar (=they are main-menus).

    Add a menu item, item separator and/or corresponding submenu by using the AddItem method, and remove them
    by using the RemoveItem method.

    - Focussing
    CGUIMenu implements a specific focusing behaviour in its FourWayNext method. It intercepts FOCUS_LEFT and FOCUS_RIGHT events.
    These will either open the submenu for the currently selected menu item (if any) or close the current menu and move to the next
    entry within the menubar.

    - DoClick
    Clicking on a menu item will open the corresponding menu (if any)

    - DoMouseEnter
    Hovering with the mouse over a menu item which leads to a sub menu will open this menu automatically. Hovering over a menu item
    will also try to set the focus to this item.

    @see CGUIMenuBar for a comprehensive source-code example
*/

class CGUIMenu
    :public CGUIMenuBase
{
public:
    /** CGUIMenu constructor.
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the CGUIMenu object
        @param vHeight Height of the CGUIMenu object
        @param eID Object Identifier of this composite object (choose NO_HANDLE if none is required)
    */
    CGUIMenu(CGUIMenuBase *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIMenu constructor.
        @param pParent Pointer to the designated parent object.
        @param kRect Bounding rectangle relative to parent.
        @param eID Object Identifier of this CGUIMenu object (choose NO_HANDLE if none is required)
    */
    CGUIMenu(CGUIMenuBase *const pParent, const CGUIRect &kRect, const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIMenu standard constructor.
        @see CGUIObject().
    */
    CGUIMenu();

    /// This destructor is automatically virtual, as the base class destructor is virtual
    ~CGUIMenu();

    /** Specialized SetParent method which will only accept objects of type CGUIMenuBase as parents
        This ensures that Menus will only get attached to MenuBars or other Menus.
        @param pParent The parent object
    */
    void SetParent(CGUIMenuBase *const pParent);

    virtual eC_Bool DoClick(const eC_Value &vAbsX = eC_FromInt(-1), const eC_Value &vAbsY = eC_FromInt(-1));

    /** Specific DoMouseEnter implementation to enhance usability by mouse
        @param vAbsX X-position in screen coordinates where the mouse entered the object
        @param vAbsY Y-position in screen coordinates where the mouse entered the object
        @return True If the event has been handled.<BR>
                False Else.
    */
    virtual eC_Bool DoMouseEnter(const eC_Value &vAbsX,const eC_Value &vAbsY);

    /** Transfer the focus to the currently active item or the first item
        that can be focused if there is on one is active, otherwise to the parent object.
    */
    virtual void GetFocus();

    /** If the focus moves out of this menu and there is no item currently selected, the menu shall close.
    */
    virtual void LoseFocus();

    /** Adjust the position of the menu to show the menu in the best place in the parent.
        @param pkRefObj The reference object used to determine the position where this menu is adjusted to.
    */
    virtual void SetOptimalPosition(const CGUIMenuItem *pkRefObj);

    /** Open the menu and adjust it to the best position according to the given rectangle.
        @param pkRefObj The reference object used to determine the position where this menu is adjusted to.
        @param pkObjOpensThisMenu The object that opens this menu.
    */
    void Open(const CGUIMenuItem *pkRefObj, CGUIMenuBase* pkObjOpensThisMenu);

    /** Close the menu and adjust the menu back to the original position.
    */
    void Close();

    virtual eC_Bool FourWayNext(FourWayFocus_t eFocusDirection);

    /// Finds the first selectable item and selects it.
    void SelectFirstItemForward();

    /// Finds the first selectable item from the end of the list and selects it.
    void SelectFirstItemBackward();

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif // GUILIANI_STREAM_GUI

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif // GUILIANI_WRITE_GUI

protected:

    /** Find an opened menu that contains the given point. If no opened menu is found,
        NULL is returned.
        @param vXPos absolute X position.
        @param vYPos absolute Y position.
        @return found menu, or NULL if no one is found.
    */
    CGUIMenu* FindMenuByPos(const eC_Value& vXPos, const eC_Value& vYPos);

    /// Helper for constructors.
    void Init();

    /// Re-implemented to prevent adding arbitary objects.
    eC_Bool AddObject(CGUIObject *pObject) { return CGUIRepositionCompositeObject::AddObject(pObject); }

    /// Re-implemented to prevent removing arbitray objects.
    void RemoveObject(CGUIObject *pObject) { CGUIRepositionCompositeObject::RemoveObject(pObject); }

private:

    /** Copy constructor of CGUIMenu.
        This constructor is private because we do not know the menu items and corresponding submenus for this control,
        thus we can not copy all the items.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGUIMenu(const CGUIMenu& kSource);

    /** Assignment operator of CGUIMenu.
        This operator is private because we do not know the menu items and corresponding submenus for this control,
        thus we can not copy all the items.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGUIMenu& operator=(const CGUIMenu& kSource);

    /// Declare the standard SetParent method as private to prevent users from accidentally adding menus to illegal parents
    void  SetParent(CGUICompositeObject *const pParent);

};
#endif
