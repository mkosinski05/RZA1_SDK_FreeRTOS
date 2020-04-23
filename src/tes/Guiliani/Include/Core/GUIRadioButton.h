/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIRADIOBUTTON__H_
#define GUIRADIOBUTTON__H_

#include "GUIImageResource.h"
#include "GUIRadioButtonGroup.h"
#include "GUIBaseRadioButton.h"

#include "GUINinePatch.h"

/// Radio button specialization that uses images for graphical representation.
// @guiliani_doxygen toplevel_control Radio Button
/**
<table border="0">
<tr>
<td width="200">@image html radio_button.png</td>
<td>
The "radio button" control has two states (checked or unchecked). It is arranged in a group of radio buttons. If one of the radio buttons is checked by the user, all other buttons in the group of radio buttons will become unchecked instantly. The "radio button" control is typically used in applications where the user has to select a single option out of various alternatives. (Class: CGUIRadioButton).
</td>
</tr>
</table>
*/
// @endguiliani_doxygen
/**
Example:
@code

// Create a radio button.
CGUIRadioButton *pkRadioButton = new CGUIRadioButton(pkRadioButtonGroup, // parent must be a CGUIRadioButtonGroup
eC_FromInt(0), eC_FromInt(0),
eC_FromInt(60), eC_FromInt(20),
false, OBJ_RADIOBUTTON);

// Set the images of the radio button.
// Before calling this method, please add image IDs to UserImageResource.h.
// After that call GETRESMANAGER.RegisterImageResource() to register images for image IDs.
pkRadioButton->SetImages(
IMG_BT_SEL_FOCUSED,
IMG_BT_SEL_HIGHLIGHTED,
IMG_BT_SEL_PRESSED,
IMG_BT_SEL_STANDARD,
IMG_BT_SEL_GRAYEDOUT,
IMG_BT_NOTSEL_FOCUSED,
IMG_BT_NOTSEL_HIGHLIGHTED,
IMG_BT_NOTSEL_PRESSED,
IMG_BT_NOTSEL_STANDARD,
IMG_BT_NOTSEL_GRAYEDOUT );

@endcode
@see CGUIRadioButtonGroup
@ingroup GUILIANI_CONTROLS
*/
class CGUIRadioButton :
    public CGUIBaseRadioButton
{
public:
    /** Constructor
    To set the standard Guiliani images, please call the SetStandardImages() method.
    To Set the user specific images, please use the SetImages() method.
    @param pParent Pointer to parent object.
    @param vX X position relative to its parent object.
    @param vY Y position relative to its parent object.
    @param vWidth Width of the image radio button.
    @param vHeight Height of the image radio button.
    @param bSelected Status of the image radio button to indicate whether it is selected or not. Selected, if true, not selected, otherwise
    @param eID Object Identifier of this image radio button
    */
    CGUIRadioButton(
        CGUIRadioButtonGroup *const pParent,
        const eC_Value &vX,
        const eC_Value &vY,
        const eC_Value &vWidth,
        const eC_Value &vHeight,
        const eC_Bool &bSelected = false,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructor.
    @param pParent Pointer to parent object.
    @param kRect Structure containing x- and y-coordinates as well as width and height.
    @param bSelected Status of the image radio button to indicate whether it is selected or not. Selected, if true, not selected, otherwise
    @param eID Object Identifier of this image radio button
    */
    CGUIRadioButton(
        CGUIRadioButtonGroup *const pParent,
        const CGUIRect &kRect,
        const eC_Bool &bSelected = false,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Copy-constructor of the CGUIRadioButton class.
    @param kSource Source object to be copied.
    */
    CGUIRadioButton(const CGUIRadioButton& kSource);

    /** Operator= method of the CGUIRadioButton class.
    @param kSource Source object to be copied.
    @return Copied object.
    */
    CGUIRadioButton& operator=(const CGUIRadioButton& kSource);

    /** Standard constructor.
    @see CGUIObject().
    */
    CGUIRadioButton();
#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif
#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

    /** Destructor */
    ~CGUIRadioButton(void);

    /** Method to change the images used for the radio button representation.
    @param eSelectedFocussed ID of image of the radio button in state
    "focused and selected before".
    @param eSelectedHighlighted ID of image of the radio button in state
    "highlighted and selected before".
    @param eSelectedPressed ID of image of the radio button in state
    "pressed and selected before".
    @param eSelectedNormal ID of image of the radio button in state
    "not pressed, not highlighted, not focused
    but selected before".
    @param eSelectedGrayedOut ID of image of the radio button in state
    "selected before but now grayed out".
    @param eNotSelectedFocussed ID of image of the radio button in state
    "focused but not selected before".
    @param eNotSelectedHighlighted ID of image of the radio button in state
    "highlighted but not selected before".
    @param eNotSelectedPressed ID of image of the radio button in state
    "pressed but not selected before".
    @param eNotSelectedNormal ID of image of the radio button in state
    "not pressed, not highlighted, not focused,
    not selected before".
    @param eNotSelectedGrayedOut ID of image of the radio button in state
    "not selected before but now grayed out".
    */
    void SetImages(
        const ImageResource_t &eSelectedFocussed,
        const ImageResource_t &eSelectedHighlighted,
        const ImageResource_t &eSelectedPressed,
        const ImageResource_t &eSelectedNormal,
        const ImageResource_t &eSelectedGrayedOut,

        const ImageResource_t &eNotSelectedFocussed,
        const ImageResource_t &eNotSelectedHighlighted,
        const ImageResource_t &eNotSelectedPressed,
        const ImageResource_t &eNotSelectedNormal,
        const ImageResource_t &eNotSelectedGrayedOut);

    /** Grants access to the object's nine patch.
    @return Nine patch used to display the object */
    CGUINinePatch& GetNinePatch()
    {
        return m_kNinePatch;
    }

    eC_Bool DoDraw();

private:

    /** Copy attributes method of the CGUIRadioButton class.
    @param kSource Source object to be copied.
    @return Copied object.
    */
    void CopyAttributes(const CGUIRadioButton& kSource);

    /** Set the standard Guiliani images to this radio button.
    */
    void SetStandardImages();

    // This private method assigns dummy images to all attributes intended for images.
    void SetDummyImages(void);

    /// Helper for destructor and ReadFromStream.
    void DeInit();

    ImageResource_t m_eSelectedNormal;
    ImageResource_t m_eSelectedHighlighted;
    ImageResource_t m_eSelectedPressed;
    ImageResource_t m_eSelectedGrayedOut;
    ImageResource_t m_eSelectedFocussed;

    ImageResource_t m_eNotSelectedNormal;
    ImageResource_t m_eNotSelectedHighlighted;
    ImageResource_t m_eNotSelectedPressed;
    ImageResource_t m_eNotSelectedGrayedOut;
    ImageResource_t m_eNotSelectedFocussed;

    CGUINinePatch   m_kNinePatch; ///< Nine Patch for the slider knob
};
#endif
