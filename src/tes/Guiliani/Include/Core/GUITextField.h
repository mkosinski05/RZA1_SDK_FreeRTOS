/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUITEXTFIELD__H_
#define GUITEXTFIELD__H_

#include "GUIObject.h"
#include "GUITextResource.h"
#include "GUIImageResource.h"
#include "GUINinePatch.h"
#include "GUIBaseTextField.h"

class CGUI;
class CGUIText;

/// Represents a simple GUI object containing a text.

// @guiliani_doxygen toplevel_control Text Field
/**
    <table border="0">
        <tr>
            <td width="200">@image html text_field.png</td>
            <td>
                The "text field" control is used to display a single text. It supports an optional image that can be drawn as background of the text (Class: CGUITextField).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    Objects of this class can be used as a single text somewhere in the GUI, when there is no
    other control around it. For instance, you might want to print a text directly
    on a window, not inside a button (which can have its own text) or another
    control.

    In addition to its base class CGUIBaseTextField, this control manages an image
    that is drawn as the background of the actual text. If no image is set, the
    draw method of the base class is called (CGUIBaseTextField::DoDraw()).

    CGUITextField should *not* be used to represent texts inside custom controls.
    To do that, have a look at CGUIText.

    For details on the methods of this class also see the description of its base
    class CGUIBaseTextField.

    Example:
@code

    // Create text field.
    CGUITextField* pkTextField = new CGUITextField(this,
            eC_FromInt(0), eC_FromInt(0),
            eC_FromInt(500), eC_FromInt(30),
            "Hello World!",
            false,
            DUMMY_IMAGE,
            NO_HANDLE);

    // Set the background image of the TextField.
    // Before calling this method, please add image ID to UserImageResource.h and
    // call GETRESMANAGER.RegisterImageResource() to register an image for this image ID.
    pkTextField->SetBackgroundImage(IMG_BG_TEXTFIELD);

    // Change the displayed text string
    CGUIText* pkLabel = pkTextField->GetLabel();
    if( pkLabel) pkLabel->SetTextString("New Text");

    // Change the internal text object.
    CGUIText* pkText = new CGUIText(this,"Good!",eC_FromInt(0), eC_FromInt(0),true);
    pkTextField->SetText(pkText);

@endcode

    @ingroup GUILIANI_CONTROLS
    @ingroup GUILIANI_TEXT
    */
class CGUITextField
    :   public CGUIBaseTextField
{
public:
    /** Constructs a text field from a string and area values.
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the textfield
        @param vHeight Height of the textfield
        @param kText text string to be displayed on the textfield
        @param bMultiLine Whether the internal CGUIText is multi-line
        @param eBackgroundImg image-id of the background image
        @param eID Object Identifier of this textfield (choose NO_HANDLE if none is required)
    */
    CGUITextField(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const eC_String &kText,
        const eC_Bool& bMultiLine = false,
        const ImageResource_t &eBackgroundImg = DUMMY_IMAGE,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructs a text field from a string and a rectangle area.
        @param pParent Pointer to the designated parent object
        @param kRect Position and size of the text field
        @param kText text string to be displayed on the textfield
        @param bMultiLine Whether the internal CGUIText is multi-line
        @param eBackgroundImg image-id of the background image
        @param eID Object Identifier of this textfield (choose NO_HANDLE if none is required)
    */
    CGUITextField(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const eC_String &kText,
        const eC_Bool& bMultiLine = false,
        const ImageResource_t &eBackgroundImg = DUMMY_IMAGE,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructs a text field from a text ID and area values.
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the textfield
        @param vHeight Height of the textfield
        @param eTextID ID of text string to be displayed on the text field
        @param bMultiLine Whether the internal CGUIText is multi-line
        @param eBackgroundImg image-id of the background image
        @param eID Object Identifier of this textfield (choose NO_HANDLE if none is required)
    */
    CGUITextField(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const TextResource_t &eTextID,
        const eC_Bool& bMultiLine = false,
        const ImageResource_t &eBackgroundImg = DUMMY_IMAGE,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructs a text field from a text ID and a rectangle area.
        @param pParent Pointer to the designated parent object
        @param kRect Position and size of the text field
        @param eTextID ID of text string to be displayed on the text field
        @param bMultiLine Whether the internal CGUIText is multi-line
        @param eBackgroundImg image-id of the background image
        @param eID Object Identifier of this textfield (choose NO_HANDLE if none is required)
    */
    CGUITextField(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const TextResource_t &eTextID,
        const eC_Bool& bMultiLine = false,
        const ImageResource_t &eBackgroundImg = DUMMY_IMAGE,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Destructor. */
    ~CGUITextField();

    /** CGUITextField copy-constructor.
        @param kSource Source object to be copied
    */
    CGUITextField(const CGUITextField& kSource);

    /** CGUITextField operator= to copy the whole CGUITextField.
        @param kSource Source object to be copied
        @return Copied object
    */
    CGUITextField& operator=(const CGUITextField& kSource);

    eC_Bool DoDraw();

    /** Set the background image of the TextField.
        @param eBackgroundImg ID of the background image to be set.
    */
    void SetBackgroundImage(const ImageResource_t &eBackgroundImg);

    /** Standard constructor. Only to be used by factories.
    */
    CGUITextField();

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    /// ID of the background image.
    ImageResource_t m_eBackgroundImg;
    CGUINinePatch   m_kNinePatch; ///< the nine patch

private:
    /// Helper for constructors.
    void Init();

    /// Helper for destructor and ReadFromStream().
    void DeInit();
};
#endif
