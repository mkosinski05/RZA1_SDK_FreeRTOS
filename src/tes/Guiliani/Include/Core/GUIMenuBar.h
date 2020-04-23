/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef CGUIMENUBAR__H_
#define CGUIMENUBAR__H_

#include "GUIMenuBase.h"


/// A horizontal menu bar. It can contain several Menu-items, which can open pulldown menus.

/** The menu bar has a list that contains several CGUIMenuAndItem objects which contain a bar item and its corresponding menu
    if available. In order to add menu items and menus to the menu bar, please use the AddItem method.

    If a menu item is clicked, the corresponding menu (if available) is opened and the menu bar's position, width and height are adjusted
    to be the same as its parent. After the menu is closed, the menu bar is changed back to be the original position, width and height.

    - Focussing
    CGUIMenuBar implements a specific focusing behaviour in its FourWayNext method. It intercepts FOCUS_UP and FOCUS_DOWN events and
    will open the currently selected menu in response to them.

    - DoClick
    Clicking on a menu item will open the corresponding menu (if any)

    - DoMouseEnter
    CGUIMenuBar overrides the DoMouseEnter slot in order to enhance usability by mouse. Moving the mouse across menu items will move
    the focus and selection as well, if the focus already was inside the menu.

    Example:
@code
    // The following code creates a basic menubar with two menus, of which the second one has a submenu.
    // The fully expanded menu will look roughly like this:
    //    [  BAR1  ] [  BAR2   ]
    //               [  ITEM1  ]
    //               [ SUBMENU ] [ ITEM2 ]
    //                           [ ITEM3 ]

    // Create a menu bar.
    CGUIMenuBar *pkMenuBar = new CGUIMenuBar(this,
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(300), eC_FromInt(20),
                                        NO_HANDLE);
    // Add first bar item and a menu.
    CGUIMenuItem *pkBarItem1 = new CGUIMenuItem(NULL,
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(100), eC_FromInt(20),
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(50), eC_FromInt(0),
                                        "bar1",
                                        NULL,   // Replace NULL with your command.
                                        NO_HANDLE);

    // Set text into a centered position
    pkBarItem1->GetLabel()->SetRelXPos(eC_FromInt(20));

    // Set menu item standard images
    pkBarItem1->SetStandardImages();

    // Create menu element
    CGUIMenu *pkMenu1 = new CGUIMenu(NULL, 
                                    eC_FromInt(0), eC_FromInt(0), 
                                    eC_FromInt(100), eC_FromInt(0));

    // Add menu element to menu bar 1
    pkMenuBar->AddItem(pkBarItem1, pkMenu1);

    // Create second menu bar
    CGUIMenuItem *pkBarItem2 = new CGUIMenuItem(NULL,
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(100), eC_FromInt(20),
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(50), eC_FromInt(0),
                                        "bar2",
                                        NULL,   // Replace NULL with your command.
                                        NO_HANDLE);

    // Set text into a centered poristion
    pkBarItem2->GetLabel()->SetRelXPos(eC_FromInt(20));

    // Set menu bar standard images
    pkBarItem2->SetStandardImages();

    // Create menu element
    CGUIMenu *pkMenu2 = new CGUIMenu(NULL,
                                    eC_FromInt(0), eC_FromInt(0),
                                    eC_FromInt(100), eC_FromInt(0),
                                    NO_HANDLE);

    // Create first item for the menu element
    CGUIMenuItem *pkMenuItem1 = new CGUIMenuItem(NULL,
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(100), eC_FromInt(20),
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(50), eC_FromInt(0),
                                        "item1",
                                        NULL,   // Replace NULL with your command.
                                        NO_HANDLE);

    // Set text into a centered position
    pkMenuItem1->GetLabel()->SetRelXPos(eC_FromInt(20));

    // Set menu item standard images
    pkMenuItem1->SetStandardImages();

    // Create a menu item separator into the new menu
    CGUIMenuItemSeparator* pkMenuItemSeparator = new CGUIMenuItemSeparator(NULL,
                                                        eC_FromInt(0), eC_FromInt(0),
                                                        eC_FromInt(100), eC_FromInt(0),
                                                        NO_HANDLE);

    // Add two items and a separator into Menu2
    pkMenu2->AddItem( pkMenuItem1, NULL, pkMenuItemSeparator);

    // Add menu2 into the menu bar
    pkMenuBar->AddItem(pkBarItem2, pkMenu2);

    // Add third bar item and a menu.
    CGUIMenuItem *pkSubMenuItem = new CGUIMenuItem(NULL,
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(100), eC_FromInt(20),
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(50), eC_FromInt(0),
                                        "Submenu",
                                        NULL,   // Replace NULL with your command.
                                        NO_HANDLE);

    // Set text into a centered position
    pkSubMenuItem->GetLabel()->SetRelXPos(eC_FromInt(20));

    // Set menu item standard images
    pkSubMenuItem->SetStandardImages();

    // Create new menu element
    CGUIMenu *pkMenu3 = new CGUIMenu(NULL,
        eC_FromInt(0), eC_FromInt(0),
        eC_FromInt(100), eC_FromInt(0),
        NO_HANDLE);

    // Add menu element to menu bar
    pkMenu2->AddItem(pkSubMenuItem, pkMenu3);

    // Create new item for the menu element
    CGUIMenuItem *pkMenuItem2 = new CGUIMenuItem(NULL,
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(100), eC_FromInt(20),
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(50), eC_FromInt(0),
                                        "item2",
                                        NULL,   // Replace NULL with your command.
                                        NO_HANDLE);

    // Set text into a centered position
    pkMenuItem2->GetLabel()->SetRelXPos(eC_FromInt(20));

    // Set menu item standard images
    pkMenuItem2->SetStandardImages();
      
    // Add menu item into menu element  
    pkMenu3->AddItem( pkMenuItem2, NULL);

    // Create a menu item for the menu
    CGUIMenuItem *pkMenuItem3 = new CGUIMenuItem(NULL,
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(100), eC_FromInt(20),
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(50), eC_FromInt(0),
                                        "item3",
                                        NULL,   // Replace NULL with your command.
                                        NO_HANDLE);

    // Set text into a centered position
    pkMenuItem3->GetLabel()->SetRelXPos(eC_FromInt(20));

    // Set menu item standard images
    pkMenuItem3->SetStandardImages();

    // Add menu item into menu element
    pkMenu3->AddItem( pkMenuItem3, NULL);

    // Set the background image.
    // Before calling this method, please add an image id to GUIImageResource.h or UserImageResource.h.
    // After that call GETRESMANAGER.RegisterImageResource method to register an image for image id.
    pkMenuBar->SetBgImage(IMG_MENUBAR_BACKGND);

@endcode
*/

class CGUIMenuBar
    :public CGUIMenuBase
{
public:

     /** CGUIMenuBar constructor.
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the CGUIMenuBar object
        @param vHeight Height of the CGUIMenuBar object
        @param eID Object Identifier of this composite object (choose NO_HANDLE if none is required)
    */
    CGUIMenuBar(CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIMenuBar constructor.
        @param pParent Pointer to the designated parent object.
        @param kRect Bounding rectangle relative to parent.
        @param eID Object Identifier of this CGUIMenuBar object (choose NO_HANDLE if none is required)
    */
    CGUIMenuBar(CGUICompositeObject *const pParent, const CGUIRect &kRect, const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIMenuBar standard constructor.
        @see CGUIObject().
    */
    CGUIMenuBar();

    /// This destructor is automatically virtual, as the base class destructor is virtual.
    virtual ~CGUIMenuBar();

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif // GUILIANI_STREAM_GUI

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif // GUILIANI_WRITE_GUI

    virtual eC_Bool DoClick(const eC_Value &vAbsX = eC_FromInt(-1), const eC_Value &vAbsY = eC_FromInt(-1));

    /** Add an item
        @see CGUIMenuBase::AddItem()
        @param pBarItem Item to add
        @param pMenu Menu to add to
        @return True if successful, False otherwise
    */
    eC_Bool AddItem(CGUIMenuItem *pBarItem, CGUIMenu *pMenu );

    /** Remove a menu item
        @param pMenuItem Item to remove
    */
    void RemoveItem(CGUIMenuItem *pMenuItem);

    virtual eC_Bool DoDraw();

    /// Specific implementation of focus handling, which will open the corresponding menu in case of FOCUS_UP or FOCUS_DOWN evetns
    virtual eC_Bool FourWayNext(FourWayFocus_t eFocusDirection);

    /** Specific DoMouseenter implementation to enhance usability by mouse
        @param vAbsX X-position in screen coordinates where the mouse entered the object
        @param vAbsY Y-position in screen coordinates where the mouse entered the object
        @return True If the event has been handled.<BR>
                False Else.
    */
    virtual eC_Bool DoMouseEnter(const eC_Value &vAbsX,const eC_Value &vAbsY);

    /// Customized focus handling which ensures that focus and selection handling works correctly within menus
    virtual void GetFocus();

    /** Check whether the given object is a bar item.
        @param pObj The given object pointer.
        @return True, if the given object is a bar item; False otherwise.
    */
    eC_Bool IsObjBarItem(const CGUIObject* pObj) const;

    /// Deselects any potentially selected item in the menu bar and automatically closes any related menus
    void ReleaseSelection();

    /** Returns the menu item within the menu bar which is currently focused.
        If none is focused, a dummy menu item is returned: CGUIMenuAndItem kDummy(NULL, NULL)
        @return The menu and item
    */
    CGUIMenuAndItem GetFocussedMenuAndItem();

    /** Returns the menu item within the menu bar which is currently selected. If there is a menu open, the corresponding
        menu item will always be selected. If none is selected, a dummy menu item is returned: CGUIMenuAndItem kDummy(NULL, NULL)
        @return The menu and item
    */
    CGUIMenuAndItem GetSelectedMenuAndItem();

    /** This reimplementation of DoUserEvent will close any currently opened menus if an event of type ET_LBUTTONDOWN
        on any object outside of the menubar is received
        @copydoc CGUIBehaviour::DoUserEvent
    */
    virtual eC_Bool DoUserEvent(CGUIEvent *const pEvent);

    virtual CGUIMenuBase* GetMenuBar()
    {
        return this;
    }
    
protected:

    /** Opens a main menu. The position, width and height of the control are changed to be the same as the parent's.
        @param pMenu Menu to be opened.
        @param pBarItem Item whose bounding rectangle is used as a reference for placing the menu on screen.
      */
    void OpenMenu(CGUIMenu* pMenu, CGUIMenuItem* pBarItem);

    /** Closes a main menu. The position, width and height of the control are changed back to their original values.
        */
    void CloseMenu();

private:
    /// Helper for constructors.
    void Init();

    /** Copy constructor of CGUIMenuBar.
        This constructor is private, because we do not know the bar items and corresponding menus for this control,
        thus we can not copy all the items.
        @param kSource Source object to be copied.
    */
    CGUIMenuBar(const CGUIMenuBar& kSource);

    /** Assignment operator.
        This operator is private, because we do not know the bar items and corresponding menus for this control,
        thus we can not copy all the items.
        @param kSource Source object to be copied.
        @return This object.
    */
    CGUIMenuBar& operator=(const CGUIMenuBar& kSource);

    /// Re-implemented to prevent adding arbitrary objects.
    eC_Bool AddObject(CGUIObject *pObject) { return CGUIRepositionCompositeObject::AddObject(pObject); }

    /// Re-implemented to prevent removing of arbitrary objects.
    void RemoveObject(CGUIObject *pObject) { CGUICompositeObject::RemoveObject(pObject); }

    /// Initial height of the menu bar. The bar will always be reset to this size when closing the menu
    eC_Value m_vInitHeight;
};
#endif
