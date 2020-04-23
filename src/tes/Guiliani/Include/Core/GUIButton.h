/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIBUTTON__H_
#define GUIBUTTON__H_

#include "GUIBaseButton.h"
#include "GUIImageResource.h"
#include "GUINinePatch.h"

/// Button specialization that uses images for graphical representation.

// @guiliani_doxygen toplevel_control Button
/**
<table border="0">
<tr>
<td width="200">@image html button.png</td>
<td>
The "button" control is used to trigger commands. The individual states of the button are visualized by different images.
Five different states are supported: Normal, highlighted, pressed, grayed out and focused. The "button" control is one of the central elements
of a touchscreen based application (Class: CGUIButton). The "button" control is offering comfortable means to zoom a button with only minimal impact on
the button borders. The button can be divided into fully user-defined areas (Top, Bottom, Left, Right).
The corner areas will remain completely untouched, while the border patches will be stretched in one direction only.
Only the center area will be stretched in two dimensions (CGUINinePatch).
</td>
</tr>
</table>
*/
// @endguiliani_doxygen
/**
Please refer to the base class (GUIBaseButton) for a general explanation of the button object.
This specialization differs from its base class by visualizing itself using images. You may assign
one image to each of the button's states using the SetImages() method.
Note that the supplied images will be stretched to the Button's size, which may result in reduced visual
quality if the image's size and the button's size differ significantly.
If you need to stretch your button, consider using its Nine Patch functionality. (see CGUINinePatch)

Example:
@code

// Create a button.
CGUIButton* pkButton = new CGUIButton(this,
eC_FromInt(0), eC_FromInt(0),
eC_FromInt(15), eC_FromInt(15),
"OK",
NULL,   // Replace the NULL with your command.
OBJ_BUTTON);
// Set the images of the button.
// Before calling this method, please add image IDs to UserImageResource.h.
// After that call GETRESMANAGER.RegisterImageResource() method to register images for the IDs.
pkButton->SetImages(
IMG_BT_STANDARD,
IMG_BT_PRESSED,
IMG_BT_HIGHLIGHTED,
IMG_BT_GRAYEDOUT,
IMG_BT_FOCUSED );

@endcode

@ingroup GUILIANI_CONTROLS
*/
class CGUIButton :
    public CGUIBaseButton
{
public:
    /** CGUIButton constructor.
    @param pParent Pointer to the designated parent object
    @param vX X-position relative to its parent object
    @param vY Y-position relative to its parent object
    @param vWidth Width of the button
    @param vHeight Height of the button
    @param rkText Text string that will be displayed on the Button
    @param pCommandObject Pointer to command object which will be executed in reaction to a click
    @param eID Object Identifier of this button
    */
    CGUIButton(
        CGUICompositeObject *const pParent,
        const eC_Value &vX,
        const eC_Value &vY,
        const eC_Value &vWidth,
        const eC_Value &vHeight,
        const eC_String& rkText,
        CGUICommandPtr pCommandObject,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIButton constructor.
    @param pParent Pointer to the designated parent object
    @param kRect Boundary rectangle of the button relative to the parent.
    @param rkText Text string that will be displayed on the Button
    @param pCommandObject Pointer to command object which will be executed in reaction to a click
    @param eID Object Identifier of this button
    */
    CGUIButton(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const eC_String& rkText,
        CGUICommandPtr pCommandObject,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIButton constructor.
    @param pParent Pointer to the designated parent object
    @param vX X-position relative to its parent object
    @param vY Y-position relative to its parent object
    @param vWidth Width of the button
    @param vHeight Height of the button
    @param eTextID ID of text string to be displayed on the Button
    @param pCommandObject Pointer to command object which will be executed in reaction to a click
    @param eID Object Identifier of this button (choose NO_HANDLE if none is required)
    */
    CGUIButton(
        CGUICompositeObject *const pParent,
        const eC_Value &vX,
        const eC_Value &vY,
        const eC_Value &vWidth,
        const eC_Value &vHeight,
        const TextResource_t &eTextID,
        CGUICommandPtr pCommandObject,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIButton constructor which sets width and height depending on the
    size of the given standard image.
    @param pParent Pointer to the designated parent object
    @param vX X-position relative to its parent object
    @param vY Y-position relative to its parent object
    @param pCommandObject Pointer to command object which will be executed
    in reaction to a click
    @param eBtnStandard ImageID for button in standard state
    @param eBtnPressed ImageID for button in pressed state
    @param eBtnHighlighted ImageID for button in highlight state
    @param eBtnGrayedOut ImageID for button in grayed out state
    @param eBtnFocused ImageID for button in focused state
    @param eID Object Identifier of this button.
    */
    CGUIButton(
        CGUICompositeObject *const pParent,
        const eC_Value &vX,
        const eC_Value &vY,
        CGUICommandPtr pCommandObject,
        const ImageResource_t &eBtnStandard,
        const ImageResource_t &eBtnPressed = DUMMY_IMAGE,
        const ImageResource_t &eBtnHighlighted = DUMMY_IMAGE,
        const ImageResource_t &eBtnGrayedOut = DUMMY_IMAGE,
        const ImageResource_t &eBtnFocused = DUMMY_IMAGE,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIButton constructor.
    @param pParent Pointer to the designated parent object
    @param kRect Boundary rectangle of the button relative to the parent.
    @param eTextID ID of text string to be displayed on the Button
    @param pCommandObject Pointer to command object which will be executed in reaction to a click
    @param eID Object Identifier of this button (choose NO_HANDLE if none is required)
    */
    CGUIButton(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const TextResource_t &eTextID,
        CGUICommandPtr pCommandObject,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIButton copy-constructor.
    @param kSource Source object to be copied
    */
    CGUIButton(const CGUIButton& kSource);

    /** CGUIButton operator= to copy the whole CGUIButton.
    @param kSource Source object to be copied
    @return Copied object
    */
    CGUIButton& operator=(const CGUIButton& kSource);
    /**    Standard constructor
    @see CGUIObject().
    */
    CGUIButton();

    /** CGUIButton destructor */
    virtual ~CGUIButton();

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif
    eC_Bool DoDraw();

    /**
    With this function images which will be drawn for representation can be set.
    Five images for the five supported states of an image button.
    @param eBtnStandard ImageID for button in standard state
    @param eBtnPressed ImageID for button in pressed state
    @param eBtnHighlighted ImageID for button in highlight state
    @param eBtnGrayedOut ImageID for button in grayed out state
    @param eBtnFocused ImageID for button in focused state
    */
    void SetImages(
        const ImageResource_t &eBtnStandard,
        const ImageResource_t &eBtnPressed,
        const ImageResource_t &eBtnHighlighted,
        const ImageResource_t &eBtnGrayedOut,
        const ImageResource_t &eBtnFocused);

    /** Retrieves currently set images
    @param eBtnStandard ImageID for button in standard state
    @param eBtnPressed ImageID for button in pressed state
    @param eBtnHighlighted ImageID for button in highlight state
    @param eBtnGrayedOut ImageID for button in grayed out state
    @param eBtnFocused ImageID for button in focused state
    */
    void GetImages(
        ImageResource_t &eBtnStandard,
        ImageResource_t &eBtnPressed,
        ImageResource_t &eBtnHighlighted,
        ImageResource_t &eBtnGrayedOut,
        ImageResource_t &eBtnFocused);

    /** Sets all images to the framework's standard images. */
    void SetStandardImages();

    /** Grants access to the object's nine patch.
    @return Nine patch used to display the object */
    CGUINinePatch& GetNinePatch()
    {
        return m_kNinePatch;
    }

protected:
    ImageResource_t m_eBtnNormal; ///< the standard image
    ImageResource_t m_eBtnHighlighted; ///< the highlighted image
    ImageResource_t m_eBtnPressed; ///< the pressed image
    ImageResource_t m_eBtnGrayedOut; ///< the grayed out image
    ImageResource_t m_eBtnFocused; ///< the focused image

    CGUINinePatch   m_kNinePatch; ///< the nine patch

private:
    /// Helper for constructors.
    void Init();

    /// Helper for destructor and ReadFromStream.
    void DeInit();
};
#endif
