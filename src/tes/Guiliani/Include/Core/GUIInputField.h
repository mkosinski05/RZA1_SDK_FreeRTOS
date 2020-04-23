/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIINPUTFIELD__H_
#define GUIINPUTFIELD__H_

#include "GUIBaseInputField.h"
#include "GUINinePatch.h"
#include "GUIImageResource.h"

/// Input field specialization that uses images for graphical representation.

// @guiliani_doxygen toplevel_control Input Field
/**
<table border="0">
<tr>
<td width="200">@image html input_field.png</td>
<td>
The "input field" control is used for single line text input. To easily scale the dimension of the input field,
it is composed out of three different images. One for the left side, one for the right side and one for the center (Class: CGUIInputField).
The "input field" supports nine patch images (CGUINinePatch).
</td>
</tr>
<tr>
<td width="200">@image html input_field_password.png</td>
<td>
The input field can also be used for inserting passwords. Then you will see an asterisk for each character you typed in (Class CGUIInputField).
</td>
</tr>
</table>
*/
// @endguiliani_doxygen
/**
The input field consists of three different images. One for the left side,
one for the right side and one for the center. The side images are optional.
The center image is mandatory and will be automatically stretched to the correct size.
If Guiliani standard images are enabled, a default background image is set automatically.

These images can be set in four different states (highlighted, focused,
grayed out and standard).

Example:
@code

// Create an input field.
CGUIInputField *pkInputField = new CGUIInputField(this,
eC_FromInt(0), eC_FromInt(0),
eC_FromInt(110), eC_FromInt(30),
"Hello", CGUIEdit::AC_ALL,
OBJ_INPUTFIELD);

// Set the images
// Before calling this method, please add image IDs to UserImageResource.h.
// After that call GETRESMANAGER.RegisterImageResource() to register images for image IDs.
pkInputField->SetImages( IMG_STANDARD, IMG_HIGHLIGHTED, IMG_FOCUSSED, IMG_GRAYED_OUT );

@endcode

@ingroup GUILIANI_CONTROLS
*/
class CGUIInputField
    : public CGUIBaseInputField
{
public:
    /** Constructor.
    @param pParent Pointer to the designated parent object
    @param vX X-position relative to its parent object
    @param vY X-position relative to its parent object
    @param vWidth Width of the input field
    @param vHeight Height of the input field
    @param rkText Initial text to be shown in the input field
    @param eAcceptedChars The character set accepted as input.
    @param eID Object Identifier of this input field (use NO_HANDLE if none is required)
    */
    CGUIInputField(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const eC_String &rkText,
        CGUIEdit::AcceptedChars_t eAcceptedChars = CGUIEdit::AC_ALL,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructor.
    @param pParent Pointer to the designated parent object
    @param kRect The client rectangle (control position and size).
    @param rkText Initial text to be shown in the input field
    @param eAcceptedChars The character set accepted as input.
    @param eID Object Identifier of this input field (use NO_HANDLE if none is required)
    */
    CGUIInputField(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const eC_String &rkText,
        CGUIEdit::AcceptedChars_t eAcceptedChars = CGUIEdit::AC_ALL,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Destructor */
    virtual ~CGUIInputField();

    /** Copy constructor.
    @param kSource Source object to be copied.
    */
    CGUIInputField(const CGUIInputField& kSource);

    /** Assignment operator method of the CGUIBaseInputField class.
    @param kSource Source object to be copied.
    @return Copied object.
    */
    CGUIInputField& operator=(const CGUIInputField& kSource);

    /** Standard constructor.
    @see CGUIObject().
    */
    CGUIInputField();

    /**
    Use this function to set standard images for all states.
    */
    void SetStandardImages();

    /// This functions sets the images to use if the control is in the respective state.
    void SetImages(const ImageResource_t& eStandard, const ImageResource_t& eHighlighted, const ImageResource_t& eFocussed, const ImageResource_t& eGrayedOut);

    /** Grants access to the object's nine patch.
    @return Nine patch used to display the object */
    CGUINinePatch& GetNinePatch()
    {
        return m_kNinePatch;
    }

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    /**
    Draws the images if set, otherwise calls the base class implementation.
    @return True if something was painted, otherwise False.
    */
    virtual eC_Bool DoDraw();

    CGUINinePatch   m_kNinePatch;///< The nine patch

    ImageResource_t m_eNormal;///< The standard image
    ImageResource_t m_eHighlighted;///< The highlighted image
    ImageResource_t m_eGrayedOut;///< The grayed out image
    ImageResource_t m_eFocussed;///< The focused image

private:
    /** Helper function for constructor. */
    void Init();

    /** Helper function for destructor and ReadFromStream. */
    void DeInit();

    /** Copies attributes.
    @param kSource Source object to be copied.
    */
    void CopyAttributes(const CGUIInputField& kSource);
};
#endif
