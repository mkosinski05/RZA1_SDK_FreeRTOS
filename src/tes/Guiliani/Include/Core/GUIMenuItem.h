/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef CGUIMENUITEM_H_
#define CGUIMENUITEM_H_

#include "GUIIconButton.h"
#include "GUIImageResource.h"

/// Specialization of CGUIIconButton that represents an item for the menu and menu bar.

/** This class can be used for both menu bar items (see CGUIMenuBar) and menu items (see CGUIMenu).

    Each menu item may have an additional submenu icon. The submenu icon should be used to show
    that this menu item opens a submenu.
    Use SetSubMenuIconImage() to set this icon image.
    SetFunctionIconImages() method is applied to set the function icon of the control.
    SetBgImages() method is applied to set the background images of the control. Standard
    background images are set by default if GUI_PROP_USE_STANDARD_IMAGES property is set.

    A menu item can be <i>selected</i> which means that it is focused or highlighted and
    may therefore be activated by a click or by pressing the Guiliani action key.
    The selected state is also used for visualizing menu items which have opened a sub-menu.
    SetSelected() method is applied to set the selected state of this control.
    Use IsSelected() to check whether the list item is currently selected.

    The default alignment of the text is horizontally left and vertically centered.

    DoClick() method is applied to handle the event after user clicks the control.

    Example:
@code

    // Create a menu item.
    CGUIMenuItem* pkMenuItem = new CGUIMenuItem(this,
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(100), eC_FromInt(30),
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(90), eC_FromInt(0),
                                        "item1",
                                        NULL,   // Replace the NULL with your command.
                                        OBJ_MENUITEM);

    // Set the SubMenu-icon of the control.
    // Before calling this method, please add an image id to UserImageResource.h.
    // After that call GETRESMANAGER.RegisterImageResource() to register an image for image id.
    pkMenuItem->SetSubMenuIconImage(ICON_SUBMENU);

    // Set position of the second icon inside the control.
    pkMenuItem->SetSubMenuIconImagePos(eC_FromInt(90), eC_FromInt(10));

@endcode

*/
class CGUIMenuItem
    :public CGUIIconButton
{
public:

    /// Allows both of the CGUIMenu and CGUIMenuBar classes to
    /// access the private SetIsToOpenMenu method.
    friend class CGUIMenuBase;
    friend class CGUIMenuBar;
    /** CGUIMenuItem constructor.
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object.
        @param vY Y-position relative to its parent object.
        @param vWidth Width of the MenuItem.
        @param vHeight Height of the MenuItem.
        @param vXFunctionIconPos X-position of image relative this object.
        @param vYFunctionIconPos Y-position of image relative this object.
        @param vXSubMenuIconPos X-position of SubMenu-image relative this object.
        @param vYSubMenuIconPos Y-position of SubMenu-image relative this object.
        @param pkText Text string that will be displayed on the MenuItem.
        @param pCommandObject Pointer to command object which will be executed in reaction to a click.
        @param eID Object Identifier of this MenuItem (choose NO_HANDLE if none is required).
    */
    CGUIMenuItem(CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth,
        const eC_Value &vHeight,
        const eC_Value &vXFunctionIconPos,
        const eC_Value &vYFunctionIconPos,
        const eC_Value &vXSubMenuIconPos,
        const eC_Value &vYSubMenuIconPos,
        const eC_String& pkText,
        CGUICommandPtr pCommandObject,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIMenuItem constructor.
        @param pParent Pointer to the designated parent object.
        @param kRect dimensions of object with respect to parent object.
        @param vXFunctionIconPos X-position of function icon image relative this object.
        @param vYFunctionIconPos Y-position of function icon image relative this object.
        @param vXSubMenuIconPos X-position of SubMenu-image relative this object.
        @param vYSubMenuIconPos Y-position of SubMenu-image relative this object.
        @param rkText Text string that will be displayed on the MenuItem.
        @param pCommandObject Pointer to command object which will be executed in reaction to a click.
        @param eID Object Identifier of this MenuItem (choose NO_HANDLE if none is required).
    */
    CGUIMenuItem(CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const eC_Value &vXFunctionIconPos,
        const eC_Value &vYFunctionIconPos,
        const eC_Value &vXSubMenuIconPos,
        const eC_Value &vYSubMenuIconPos,
        const eC_String& rkText,
        CGUICommandPtr pCommandObject,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIMenuItem constructor.
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object.
        @param vY Y-position relative to its parent object.
        @param vWidth Width of the MenuItem.
        @param vHeight Height of the MenuItem.
        @param vXFunctionIconPos X-position of function icon image relative this object.
        @param vYFunctionIconPos Y-position of function icon image relative this object.
        @param vXSubMenuIconPos X-position of SubMenu-image relative this object.
        @param vYSubMenuIconPos Y-position of SubMenu-image relative this object.
        @param eTextID ID of text string to be displayed on the MenuItem.
        @param pCommandObject Pointer to command object which will be executed in reaction to a click.
        @param eID Object Identifier of this MenuItem (choose NO_HANDLE if none is required).
    */
    CGUIMenuItem(CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth,
        const eC_Value &vHeight,
        const eC_Value &vXFunctionIconPos,
        const eC_Value &vYFunctionIconPos,
        const eC_Value &vXSubMenuIconPos,
        const eC_Value &vYSubMenuIconPos,
        const TextResource_t &eTextID,
        CGUICommandPtr pCommandObject,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIMenuItem constructor.
        @param pParent Pointer to the designated parent object.
        @param kRect dimensions of object with respect to parent object.
        @param vXFunctionIconPos X-position of function icon image relative this object.
        @param vYFunctionIconPos Y-position of function icon image relative this object.
        @param vXSubMenuIconPos X-position of SubMenu-image relative this object.
        @param vYSubMenuIconPos Y-position of SubMenu-image relative this object.
        @param eTextID ID of text string to be displayed on the MenuItem.
        @param pCommandObject Pointer to command object which will be executed in reaction to a click.
        @param eID Object Identifier of this MenuItem (choose NO_HANDLE if none is required).
    */
    CGUIMenuItem(CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const eC_Value &vXFunctionIconPos,
        const eC_Value &vYFunctionIconPos,
        const eC_Value &vXSubMenuIconPos,
        const eC_Value &vYSubMenuIconPos,
        const TextResource_t &eTextID,
        CGUICommandPtr pCommandObject,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Copy-constructor of the CGUIMenuItem class.
        @param kSource Source object to be copied.
    */
    CGUIMenuItem(const CGUIMenuItem& kSource);


    /** Operator= method of the CGUIMenuItem class.
        @param kSource Source object to be copied.
        @return Copied object.
    */
    CGUIMenuItem& operator=(const CGUIMenuItem& kSource);

    /** Destructor */
    ~CGUIMenuItem();

#ifdef GUILIANI_STREAM_GUI
   /** Standard constructor
        @see CGUIObject().
    */
    CGUIMenuItem();

    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

    ///Draw the icon images and text.
    eC_Bool DoDraw();

    eC_Bool DoClick(const eC_Value &vAbsX = eC_FromInt(-1), const eC_Value &vAbsY = eC_FromInt(-1));

    /** Set the function-icon images of the control.
        @param eIconNormal ImageID of the icon in the normal status.
        @param eIconPressed ImageID of the icon in the pressed status.
        @param eIconHighlighted ImageID of the icon in the highlighted status.
        @param eIconGrayedOut ImageID of the icon in the grayedout status.
        @param eIconFocussed ImageID of the icon in the focused status.
        @param eIconSelected ImageID of the icon in the selected status.
    */
    void SetFunctionIconImages(const ImageResource_t eIconNormal, const ImageResource_t eIconPressed,
                       const ImageResource_t eIconHighlighted, const ImageResource_t eIconGrayedOut,
                       const ImageResource_t eIconFocussed, const ImageResource_t& eIconSelected);

    /** Set the background images of the control.
        @param eItemStandard ImageID of the background image in the normal status.
        @param eItemPressed ImageID of the background image in the pressed status.
        @param eItemHighlighted ImageID of the background image in the highlighted status.
        @param eItemGrayedOut ImageID of the background image in the grayedout status.
        @param eItemFocused ImageID of the background image in the focused status.
        @param eItemSelected ImageID of the background image in the selected status.
    */
    void SetBgImages(const ImageResource_t &eItemStandard, const ImageResource_t &eItemPressed,
                   const ImageResource_t &eItemHighlighted, const ImageResource_t &eItemGrayedOut,
                   const ImageResource_t &eItemFocused, const ImageResource_t &eItemSelected);


    /** Set the SubMenu-icon of the control.
        @param eSubMenuIcon ImageID of the icon when MenuItem has a sub-menu.
    */
    void SetSubMenuIconImage(const ImageResource_t &eSubMenuIcon);

    /** Set the selected state of this control.
        @param bSelected New state
    */
    void SetSelected(const eC_Bool& bSelected);

    /** Return whether the menu item is currently selected.
        @return True if selected
    */
    inline eC_Bool IsSelected()const { return m_bSelected; }

    /** Return whether the menu item has a submenu.
        @return True if item has a submenu
    */
    inline eC_Bool IsToOpenMenu() const { return m_bIsToOpenMenu; }

    /** Function to set position of the second icon inside the control.
        @param vSubMenuIconPosX X Position of icon relative to control.
        @param vSubMenuIconPosY Y Position of icon relative to control.
    */
    void SetSubMenuIconImagePos( const eC_Value &vSubMenuIconPosX, const eC_Value &vSubMenuIconPosY);

    /** Returns the position of the second icon.
        @param[out] vSubMenuIconPosX X Position of icon relative to control.
        @param[out] vSubMenuIconPosY Y Position of icon relative to control.
    */
    void GetSubMenuIconImagePos( eC_Value &vSubMenuIconPosX, eC_Value &vSubMenuIconPosY) const;

    /** Set the bool value to indicate whether the menu item has a separator after it or not.
        @param bHasSeparator New parameter
   */
    inline void SetHasSeparator(eC_Bool bHasSeparator) {m_bHasSeparator = bHasSeparator;}

    /** Return whether the menu item has a separator after it.
        @return True if item has a separator
    */
    inline eC_Bool HasSeparator() const { return m_bHasSeparator; }

    /// Set the standard images for the background of this control.
    void SetStandardImages();

protected:
    /** If the menu item has a submenu, the parameter will may be set to true for checking
        whether it is to open a submenu.
        @param bIsToOpenMenu New parameter
    */
    inline void SetIsToOpenMenu(eC_Bool bIsToOpenMenu) { m_bIsToOpenMenu = bIsToOpenMenu; }

    ///The image ID of the submenu icon.
    ImageResource_t m_eSubMenuIconImage;

    /// The image id of the Function icon with selected state.
    ImageResource_t m_eFunctionIconSelected;

    /// The image ID for the selected status.
    ImageResource_t m_eItemSelected;

    ///X-position of SubMenu-image relative this object.
    eC_Value m_vSubMenuIconPosX;

    ///Y-position of SubMenu-image relative this object
    eC_Value m_vSubMenuIconPosY;

    ///Indicate whether the control is selected or not.
    eC_Bool m_bSelected;

    ///Indicate whether the item has a submenu.
    eC_Bool m_bIsToOpenMenu;

    /// Indicate whether there is a separator after the item.
    eC_Bool m_bHasSeparator;

private:
    /// Helper for constructors.
    void Init(const eC_Value &vXSubMenuIconPos, const eC_Value &vYSubMenuIconPos);

    /// Helper for destructor and ReadFromStream.
    void DeInit();

    /** For copying the attributes in this class.*/
    void CopyAttributes(const CGUIMenuItem & kSource);
};
#endif
