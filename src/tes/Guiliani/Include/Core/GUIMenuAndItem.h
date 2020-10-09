/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef CGUIMENUANDITEM_H_
#define CGUIMENUANDITEM_H_

class CGUIMenu;
class CGUIMenuItem;
class CGUIMenuItemSeparator;

/** This class is used as a list member in the CGUIMenu and CGUIMenuBar
    class.

    Example:
@code

    // Create a MenuAndItem.
    CGUIMenuAndItem pkMenuAndItem = new CGUIMenuAndItem();

    // Set a menu.
    CGUIMenu *pkMenu = new CGUIMenu(this,
                                eC_FromInt(0), eC_FromInt(0),
                                eC_FromInt(300), eC_FromInt(300),
                                NO_HANDLE);
    pkMenuAndItem->SetMenu(pkMenu);

    // Set an item separator.
    CGUIMenuItemSeparator* pkItemSeparator = new CGUIMenuItemSeparator(this,
                                                    eC_FromInt(0), eC_FromInt(0),
                                                    eC_FromInt(300), eC_FromInt(1),
                                                    NO_HANDLE);
    pkMenuAndItem->SetItemSeparator(pkItemSeparator);

@endcode
@brief Used to build up a construct that has a menu item and its corresponding menu and item separator
*/
class CGUIMenuAndItem
{
public:
    /// Type used for the MenuAndBarItem_t list that contains menus and items.
    typedef eC_TListDoubleLinked<CGUIMenuAndItem> MenuAndItemList;

    /** Default constructor
    */
    CGUIMenuAndItem()
        :m_pkMenuItem(NULL),m_pkMenu(NULL),m_pkMenuItemSeparator(NULL){}

    /** Constructor
        @param pkMenuItem The menu item
    */
    CGUIMenuAndItem(CGUIMenuItem *pkMenuItem)
        :m_pkMenuItem(pkMenuItem),m_pkMenu(NULL),m_pkMenuItemSeparator(NULL){}

    /** Constructor
        @param pkMenuItem The menu item
        @param pkMenu The menu
    */
    CGUIMenuAndItem(CGUIMenuItem *pkMenuItem,CGUIMenu *pkMenu)
        :m_pkMenuItem(pkMenuItem),m_pkMenu(pkMenu),m_pkMenuItemSeparator(NULL){}

    /** Constructor
        @param pkMenuItem The menu item
        @param pkMenu The menu
        @param pkMenuItemSeparator The menu item separator
    */
    CGUIMenuAndItem(CGUIMenuItem *pkMenuItem,CGUIMenu *pkMenu, CGUIMenuItemSeparator *pkMenuItemSeparator)
        :m_pkMenuItem(pkMenuItem),m_pkMenu(pkMenu),m_pkMenuItemSeparator(pkMenuItemSeparator){}

    ///@return The menu item
    CGUIMenuItem* GetMenuItem() const {return m_pkMenuItem;}

    ///@return The menu
    CGUIMenu* GetMenu() const {return m_pkMenu;}

    ///@return The menu item separator
    CGUIMenuItemSeparator* GetMenuItemSeparator() const {return m_pkMenuItemSeparator;}

    ///@param pkMenu The menu
    void SetMenu(CGUIMenu* pkMenu){m_pkMenu = pkMenu;}

    ///@param pkItemSeparator The item separator
    void SetItemSeparator(CGUIMenuItemSeparator* pkItemSeparator){m_pkMenuItemSeparator = pkItemSeparator;}

private:
    CGUIMenuItem *m_pkMenuItem;
    CGUIMenu *m_pkMenu;
    CGUIMenuItemSeparator* m_pkMenuItemSeparator;
};
#endif
