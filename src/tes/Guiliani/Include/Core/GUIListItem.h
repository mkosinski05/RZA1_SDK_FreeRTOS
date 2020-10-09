/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef CGUILISTITEM_H_
#define CGUILISTITEM_H_

#include "GUITextField.h"

class CGUIListBox;

/// A list item is a control that is shown in a list as an element.
/** It has a text inside it to describe the element.

    The additonal feature of the list item to the GUIBaseTextField is that
    it can change color after the user highlighted it or the element is focused.
    This state is called selected in this control.

    This control shows an additional icon on the text field. The user is responsible
    for setting the icon image size and its position with SetIconImages and SetIconPos
    methods.

    The SetSelected() method is written to set the selected state.
    The selected state of this control can be known by calling the method IsSelected.
    The color of the selected state of this control can be changed by calling method
    SetSelectedColor().

    Example:
@code

    // Create a list item.
    CGUIListItem *pkListItem = new CGUIListItem(this,
                                        eC_FromInt(0), eC_FromInt(0),
                                        eC_FromInt(110), eC_FromInt(30),
                                        "",
                                        false,
                                        eC_FromInt(0), eC_FromInt(0),
                                        DUMMY_IMAGE,
                                        OBJ_LISTITEM);

    // Set the selected state of this control.
    pkListItem->SetSelected(true);

    // Set position of icon inside the control.
    pkListItem->SetIconPos( eC_FromInt(10), eC_FromInt(10));

    // Set icons of the controls.
    // Before calling this method, please add image IDs to UserImageResource.h.
    // After that call GETRESMANAGER.RegisterImageResource() to register images for image IDs.
    pkListItem->SetIconImages(
                    ICON_NORMAL,
                    ICON_SELECTED);

@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIListItem :
    public CGUITextField
{
public:
    /** Constructs a list item from a string and area values.
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the list item
        @param vHeight Height of the list item
        @param kText text string to be displayed on the list item
        @param bMultiLine Whether the internal CGUIText is multi-line
        @param vRelXPosIcon X position of the icon relative to this control.
        @param vRelYPosIcon Y position of the icon relative to this control.
        @param eBackgroundImg image-id of the background image
        @param eID Object Identifier of this list item (choose NO_HANDLE if none is required)
    */
    CGUIListItem(
        CGUIListBox *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const eC_String &kText,
        const eC_Bool& bMultiLine = false,
        const eC_Value &vRelXPosIcon = eC_FromInt(0),
        const eC_Value &vRelYPosIcon = eC_FromInt(0),
        const ImageResource_t &eBackgroundImg = DUMMY_IMAGE,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructs a list item from a string and a rectangle area.
        @param pParent Pointer to the designated parent object
        @param kRect Position and size of the list item
        @param kText text string to be displayed on the list item
        @param bMultiLine Whether the internal CGUIText is multi-line
        @param vRelXPosIcon X position of the icon relative to this control.
        @param vRelYPosIcon Y position of the icon relative to this control.
        @param eBackgroundImg image-id of the background image
        @param eID Object Identifier of this list item (choose NO_HANDLE if none is required)
    */
    CGUIListItem(
        CGUIListBox *const pParent,
        const CGUIRect &kRect,
        const eC_String &kText,
        const eC_Bool& bMultiLine = false,
        const eC_Value &vRelXPosIcon = eC_FromInt(0),
        const eC_Value &vRelYPosIcon = eC_FromInt(0),
        const ImageResource_t &eBackgroundImg = DUMMY_IMAGE,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructs a list item from a text ID and area values.
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the list item
        @param vHeight Height of the list item
        @param eTextID ID of text string to be displayed on the list item
        @param bMultiLine Whether the internal CGUIText is multi-line
        @param vRelXPosIcon X position of the icon relative to this control.
        @param vRelYPosIcon Y position of the icon relative to this control.
        @param eBackgroundImg image-id of the background image
        @param eID Object Identifier of this list item (choose NO_HANDLE if none is required)
    */
    CGUIListItem(
        CGUIListBox *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const TextResource_t &eTextID,
        const eC_Bool& bMultiLine = false,
        const eC_Value &vRelXPosIcon = eC_FromInt(0),
        const eC_Value &vRelYPosIcon = eC_FromInt(0),
        const ImageResource_t &eBackgroundImg = DUMMY_IMAGE,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructs a list item from a text ID and a rectangle area.
        @param pParent Pointer to the designated parent object
        @param kRect Position and size of the list item
        @param eTextID ID of text string to be displayed on the list item
        @param bMultiLine Whether the internal CGUIText is multi-line
        @param vRelXPosIcon X position of the icon relative to this control.
        @param vRelYPosIcon Y position of the icon relative to this control.
        @param eBackgroundImg image-id of the background image
        @param eID Object Identifier of this list item (choose NO_HANDLE if none is required)
    */
    CGUIListItem(
        CGUIListBox *const pParent,
        const CGUIRect &kRect,
        const TextResource_t &eTextID,
        const eC_Bool& bMultiLine = false,
        const eC_Value &vRelXPosIcon = eC_FromInt(0),
        const eC_Value &vRelYPosIcon = eC_FromInt(0),
        const ImageResource_t &eBackgroundImg = DUMMY_IMAGE,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIListItem copy-constructor.
        @param kSource Source object to be copied
    */
    CGUIListItem(const CGUIListItem& kSource);

    /** Standard constructor.
         @see CGUIObject().
    */
    CGUIListItem();

    /** Destructor. */
    virtual ~CGUIListItem();

    /** CGUIListItem operator= to copy the whole CGUIListItem.
        @param kSource Source object to be copied
        @return Copied object
    */
    CGUIListItem& operator=(const CGUIListItem& kSource);

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

    eC_Bool DoDraw();

    /** Sets the color of the selected state of this control.
        @param uiSelectedColor the color in ARGB coding(Form: 0xAARRGGBB,e.g.0xFFFF0000 = Red(fully opaque)).
               (Alpha, Red, Green, and Blue, simple:ARGB.)
    */
    inline void SetSelectedColor(const eC_UInt & uiSelectedColor)
    {
        if( m_uiSelectedColor != uiSelectedColor)
        {
            m_uiSelectedColor = uiSelectedColor;
            InvalidateArea();
        }
    }

    /** Retrieves the color of the selected state of this control.
        @return The color
    */
    inline eC_UInt GetSelectedColor()const { return m_uiSelectedColor;}

    /** Set the selected state of this control.
        @param bSelected The color when selected
    */
    inline void SetSelected(const eC_Bool& bSelected)
    {
        if( m_bSelected != bSelected)
        {
            m_bSelected = bSelected;
            InvalidateArea();
        }
    }

    /** Return whether the list item is currently selected.
        @return True if selected
    */
    inline eC_Bool IsSelected()const { return m_bSelected; }

    /** Function to set position of icon inside the control.
        @param vRelXPosIcon X Position of icon relative to this control.
        @param vRelYPosIcon Y Position of icon relative to this control.
    */
    void SetIconPos(const eC_Value &vRelXPosIcon, const eC_Value &vRelYPosIcon);

    /** Function to set icons of the controls.
        @param eIconNormal Image of the icon when item is in normal state.
        @param eIconSelected Image of the icon when item is in selected state.
    */
    void SetIconImages(
        const ImageResource_t& eIconNormal,
        const ImageResource_t& eIconSelected);

    /** Returns the list item's icon images
        @param eIconNormal Image of the icon when item is in normal state.
        @param eIconSelected Image of the icon when item is in selected state.
    */
    void GetIconImages(
        ImageResource_t& eIconNormal,
        ImageResource_t& eIconSelected)
    {
        eIconNormal=m_eIconNormal;
        eIconSelected=m_eIconSelected;
    }

protected:

    ///Indicate whether the control is selected or not.
    eC_Bool m_bSelected;

    ///The color of the selected state.
    eC_UInt m_uiSelectedColor;

    ///The icon image drawn in standard state.
    ImageResource_t m_eIconNormal;

    ///The icon image drawn in selected state.
    ImageResource_t m_eIconSelected;

    ///Stores X-Position of icon. This is relative to the control.
    eC_Value m_vRelXPosIcon;

    ///Stores Y-Position of icon. This is relative to the control.
    eC_Value m_vRelYPosIcon;

private:

    /** Helper function for constructor.*/
    void Init(CGUIListBox* pParent, const eC_Value &vRelXPosIcon,const eC_Value &vRelYPosIcon);

    /** Helper function for destructor.*/
    void DeInit();

    /** For copying the attributes in this class.*/
    void CopyAttributes(const CGUIListItem & kSource);
};
#endif
