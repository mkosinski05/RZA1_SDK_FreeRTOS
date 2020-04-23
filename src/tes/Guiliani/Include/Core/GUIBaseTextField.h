/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIBASETEXTFIELD__H_
#define GUIBASETEXTFIELD__H_

#include "GUIObject.h"
#include "GUITextResource.h"

class CGUI;
class CGUIText;

/// Represents a simple GUI object containing a text.

/** Like all the "BaseXXXXX"-Classes, this serves primarily as base-class, from
    which you can derive your own customized objects.
    For a textfield implementation which uses images to visualize itself, please refer
    to CGUITextField.
    
    Objects of this class can be used as a single text somewhere in the GUI, when there is no
    other control around it. For instance, you might want to print a text directly
    on a window, not inside a button (which can have its own text) or another
    control.

    In addition to printing a text, this control paints its area with a configurable color.

    CGUIBaseTextField should *not* be used to represent texts inside custom controls.
    To do that, have a look at CGUIText.

    For details on the methods of this class also see the description of its base
    class CGUIObject.

    Example:
@code

    // Create base text field.
    CGUIBaseTextField* pkBaseTextField = new CGUIBaseTextField(this,
            eC_FromInt(0), eC_FromInt(0),
            eC_FromInt(500), eC_FromInt(30),
            "Hello World!",
            false,
            0xFFFFFFFF,
            NO_HANDLE);

    // Set the background color of the TextField.
    pkBaseTextField->SetBackgoundColor(0x77FFFFFF);

    // Change the internal text object.
    CGUIText* pkText = new CGUIText(this,"Good!",eC_FromInt(0), eC_FromInt(0),true);
    pkBaseTextField->SetText(pkText);

@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIBaseTextField
    :   public CGUIObject
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
        @param uiBackgroundColor color of the background
        @param eID Object Identifier of this textfield (choose NO_HANDLE if none is required)
    */
    CGUIBaseTextField(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const eC_String &kText,
        const eC_Bool& bMultiLine = false,
        const eC_UInt &uiBackgroundColor = 0x00000000,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructs a text field from a string and a rectangle area.
        @param pParent Pointer to the designated parent object
        @param kRect Position and size of the text field
        @param kText text string to be displayed on the textfield
        @param bMultiLine Whether the internal CGUIText is multi-line
        @param uiBackgroundColor color of the background
        @param eID Object Identifier of this textfield (choose NO_HANDLE if none is required)
    */
    CGUIBaseTextField(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const eC_String &kText,
        const eC_Bool& bMultiLine = false,
        const eC_UInt &uiBackgroundColor = 0x00000000,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructs a text field from a text ID and area values.
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY Y-position relative to its parent object
        @param vWidth Width of the textfield
        @param vHeight Height of the textfield
        @param eTextID ID of text string to be displayed on the text field
        @param bMultiLine Whether the internal CGUIText is multi-line
        @param uiBackgroundColor color of the background
        @param eID Object Identifier of this textfield (choose NO_HANDLE if none is required)
    */
    CGUIBaseTextField(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const TextResource_t &eTextID,
        const eC_Bool& bMultiLine = false,
        const eC_UInt &uiBackgroundColor = 0x00000000,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Constructs a text field from a text ID and a rectangle area.
        @param pParent Pointer to the designated parent object
        @param kRect Position and size of the text field
        @param eTextID ID of text string to be displayed on the text field
        @param bMultiLine Whether the internal CGUIText is multi-line
        @param uiBackgroundColor color of the background
        @param eID Object Identifier of this textfield (choose NO_HANDLE if none is required)
    */
    CGUIBaseTextField(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const TextResource_t &eTextID,
        const eC_Bool& bMultiLine = false,
        const eC_UInt &uiBackgroundColor = 0x00000000,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Destructor. */
    ~CGUIBaseTextField();

    /** CGUIBaseTextField copy-constructor.
        @param kSource Source object to be copied
    */
    CGUIBaseTextField(const CGUIBaseTextField& kSource);

    /** CGUIBaseTextField operator= to copy the whole CGUIBaseTextField.
        @param kSource Source object to be copied
        @return Copied object
    */
    CGUIBaseTextField& operator=(const CGUIBaseTextField& kSource);

    /** Sets the text label within the Textfield
        @see CGUIText::SetTextString()
        @param rkValue The CGUIValue containing the new value(s)
        @return Always True.
    */
    virtual eC_Bool SetValue( const CGUIValue& rkValue);

    eC_Bool DoDraw();

    /// return const CGUIText* The current label
    const CGUIText* GetLabel() const { return m_pkText; }

    /// return CGUIText* The current label
    CGUIText* GetLabel() { return m_pkText; }

    /** Sets the background color of the TextField via a 32 Bit value in the Format 0xAARRGGBB
        @param uiBackgroundColor color of the background to be set.
    */
    void SetBackgroundColor(const eC_UInt &uiBackgroundColor);

    /** Changes the text displayed by this text field.
        @param eTextID New content of the text field
    */
    void SetLabel(const TextResource_t &eTextID);

    /** Changes the text displayed by this text field.
        @param rkText New content of the text field
    */
    void SetLabel(const eC_String &rkText);

    /** Changes the internal text object. This method deletes the old text.
        Use this to exchange the default CGUIText control created by this
        control object with another (possibly user-implemented) text.
        If you only want to change the text content, use SetLabel().
        @param pkText The new text object.
      */
    void SetText(CGUIText* pkText);

    /** Calls base class implementation and adjusts the size of the CGUIText
        with the same width difference (i.e., the pixel distance of the text's
        right edge to this control's right edge stays the same).
        @param vNewWidth The new width.
      */
    virtual void SetWidth(const eC_Value& vNewWidth);

    /** Calls base class implementation and adjusts the size of the CGUIText
        with the same height difference (i.e., the pixel distance of the text's
        bottom edge to this control's bottom edge stays the same).
        @param vNewHeight The new height.
      */
    virtual void SetHeight(const eC_Value& vNewHeight);

    /** Standard constructor.
        @see CGUIObject().
    */
    CGUIBaseTextField();

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    static const eC_Char XMLTAG_BASETEXTFIELD_BACKGROUNDCOLOR[];
    static const eC_Char XMLTAG_BASETEXTFIELD[];
#endif

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    /// The text displayed by this text field.
    CGUIText *m_pkText;

    /// The color of the background.
    eC_UInt m_uiBackgroundColor;

private:
    /** CGUIBaseTextField CopyAttributes method for copy the attributes in this class
        @param kSource Source object
    */
    void CopyAttributes(const CGUIBaseTextField& kSource);

    /** Helper function for constructor.
        Initializes the internal text and sets it non-focusable.
        @param kText Text content to be displayed.
        @param bMultiLine Whether the text is initially multi-line.
    */
    void Init(const eC_String &kText, eC_Bool bMultiLine);

    /** Helper function for constructor.
        Initializes the internal text and sets it non-focusable.
        @param kText Text content to be displayed.
        @param bMultiLine Whether the text is initially multi-line.
    */
    void Init(const TextResource_t &eTextID, eC_Bool bMultiLine);

    /// Helper for destructor and ReadFromStream.
    void DeInit();
};
#endif
