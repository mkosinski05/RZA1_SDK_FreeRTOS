/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUICOMBOBOXHEADER_H_
#define GUICOMBOBOXHEADER_H_

#include "GUICompositeObject.h"
#include "GUIResourceManager.h"

class CGUIInputField;
class CGUIEditableText;
class CGUIButton;
class CGUIValidator;

/// Represents the header part of a combo box control.

/** The GUIComboBoxHeader represents the part of a ComboBox which is visible even while the ComboBox is closed.
    It contains an input field (showing the currently selected entry and offering search-functionality) and a button
    which opens/closes the List of entries within the ComboBox.
    Typically, users will not use this class directly, but instead make use of the CGUIComboBox class.

    Example:
@code

    // Create combo box header.
    CGUIComboBoxHeader *pComboBoxHeader = new CGUIComboBoxHeader(this,
                    eC_FromInt(0), eC_FromInt(0),
                    eC_FromInt(110), eC_FromInt(30),
                    "", eC_FromInt(30),
                    true,OBJ_COMBOBOXHEADER);

    // Set the width of the button in the right side of the header.
    pComboBoxHeader->SetButtonWidth(eC_FromInt(20));

    // Set the input field as not be editable.
    pComboBoxHeader->SetEditable(false);

@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIComboBoxHeader : public CGUICompositeObject
{
public:

    /** CGUIComboBoxHeader constructor.
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY X-position relative to its parent object
        @param vWidth Width of the combobox header
        @param vHeight Height of the combobox header
        @param rkText Initial text to be shown in the input field
        @param vButtonWidth The width of the button.
        @param eID Object Identifier of this combobox header (use NO_HANDLE if none is required)
    */
    CGUIComboBoxHeader(
        CGUICompositeObject * const pParent ,
        const eC_Value &vX,const eC_Value &vY,
        const eC_Value &vWidth,const eC_Value& vHeight,
        const eC_String &rkText,
        const eC_Value &vButtonWidth,
        const ObjectHandle_t&eID = NO_HANDLE);

    /** CGUIComboBoxHeader constructor.
        @param pParent Pointer to the designated parent object
        @param kRect The client rectangle (control position and size).
        @param rkText Initial text to be shown in the input field
        @param vButtonWidth The width of the button.
        @param eID Object Identifier of this combobox header (use NO_HANDLE if none is required)
    */
    CGUIComboBoxHeader(
        CGUICompositeObject * const pParent,
        const CGUIRect &kRect,
        const eC_String& rkText,
        const eC_Value &vButtonWidth ,
        const ObjectHandle_t&eID = NO_HANDLE);

    /** CGUIComboBoxHeader copy-constructor.
        @param kSource Source object to be copied
    */
    CGUIComboBoxHeader(const CGUIComboBoxHeader& kSource);

    /** CGUIComboBoxHeader operator= to copy the whole CGUIComboBoxHeader.
        @param kSource Source object to be copied
        @return Copied object
    */
    CGUIComboBoxHeader& operator=(const CGUIComboBoxHeader& kSource);

    /** Standard constructor.
    @see CGUIComboBoxHeader().
    */
    CGUIComboBoxHeader(void);

    /** CGUIComboBoxHeader destructor.*/
    ~CGUIComboBoxHeader(void);

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

    /** Calls the base class implementation and adjusts the button's position.
        @param vWidth The new width.
      */
    virtual void SetWidth(const eC_Value& vWidth);

    /** Calls the base class implementation and adjusts the button's height.
        @param vHeight The new height.
      */
    virtual void SetHeight(const eC_Value& vHeight);

    ///Set the width of the button in the right side of the header.
    void SetButtonWidth(const eC_Value& vWidth);

    /// Set the input field as editable or not.
    void SetEditable(const eC_Bool& bEditable);

    ///Get the drop down button pointer.
    inline CGUIButton* GetButton()const { return m_pButton;}

    ///Get the Inputfield's pointer.
    inline CGUIInputField* GetInputField() const { return m_pEdit;}

    /**
        With this function images for the input field can be set.
        @param eStandard Standard image
        @param eHighlighted Highlighted image
        @param eFocussed Focussed image
        @param eGrayedOut Grayed-out image
    */
    void SetInputFieldImages(const ImageResource_t &eStandard, const ImageResource_t &eHighlighted, const ImageResource_t &eFocussed, const ImageResource_t &eGrayedOut);

    /**
        With this function images for the button can be set.
        @param eBtnStandard Standard image
        @param eBtnPressed Standard image
        @param eBtnHighlighted Highlighted image
        @param eBtnGrayedOut Grayed-out image
        @param eBtnFocused Focussed image
    */
    void SetButtonImages(const ImageResource_t &eBtnStandard, const ImageResource_t &eBtnPressed, const ImageResource_t &eBtnHighlighted, const ImageResource_t &eBtnGrayedOut, const ImageResource_t &eBtnFocused);

    /** Set the widths of the inner borders of the input field.
        The border width is the distance of this control's edges to the edges
        of the internal CGUIEdit.
        @param vLeftBorder Width of the left inner border.
        @param vRightBorder Width of the right inner border.
        @param vTopBorder Width of the top inner border.
        @param vBottomBorder Width of the bottom inner border.
      */
    void SetInputFieldBorderWidths(eC_Value vLeftBorder, eC_Value vRightBorder, eC_Value vTopBorder, eC_Value vBottomBorder);


    /** Set the text.
        @param rkTextString The string to be assigned to this object.
    */
    virtual void SetText(const eC_String& rkTextString);

    /** Returns the current text content of the input field.
        In password mode, this method returns the string of obfuscation
        characters. To obtain the entered password, use GetPassword().
        @return a copy of the current text content of the input field.
      */
    eC_String GetText();

    /** Changes the internal text. This call is directly passed through to
        CGUIEdit::SetLabel.
        @param pkText The new text to be used by the internal edit control.
      */
    void SetLabel(CGUIEditableText* pkText);

    /** Get the label of the input field.
        Use this to obtain a pointer to the internal text for formatting etc.
        @return Pointer to the internal text label.
    */
    CGUIEditableText* GetLabel();

    /** Sets the color used as the background for selected text.
        @param uiCol The selected text color.
        @see CGUIEditableText::SetSelectionColor()
      */
    void SetSelectedTextColor(eC_UInt uiCol);

    /** Get the color currently used as the background for selected text.
        @return the selected text color.
        @see CGUIEditableText::GetSelectionColor()
      */
    eC_UInt GetSelectedTextColor();

    /** Add an observer to get informed when the user started the input.
        CGUIObserver::OnNotification(const CGUIValue& kObservedValue,
            const CGUIObject* const pkUpdatedObject) 
        will be called whenever the user started.
        @param pObserver The Observer that wants to subscribe.
        @ingroup GUILIANI_SUBJECTS
    */
    void AddInputStartedObserver(CGUIObserver* pObserver);

    /** Unsubscribe an observer that subscribed as InputStarted observer.
            @param pObserver The Observer that wants to unsubscribe.
    */
    void RemoveInputStartedObserver(CGUIObserver* pObserver);

    /** Add an observer to get informed when the user finished the input.
        CGUIObserver::OnNotification(const CGUIValue& kObservedValue,
            const CGUIObject* const pkUpdatedObject) 
        will be called whenever the user finished.
        @param pObserver The Observer that wants to subscribe.
        @ingroup GUILIANI_SUBJECTS
    */
    void AddInputFinishedObserver(CGUIObserver* pObserver);

    /** Unsubscribe an observer that subscribed as InputFinished observer.
            @param pObserver The Observer that wants to unsubscribe.
    */
    void RemoveInputFinishedObserver(CGUIObserver* pObserver);

    /** Sets an validator for the input enabled combobox header.
        @param pValidator The validator to set.
    */
    void SetValidator(CGUIValidator* pValidator);

private:

    /// Helper for constructors.
    void Init(const eC_Value& vButtonWidth, const eC_String &rkText);

    /// Helper for destructor and ReadFromStream.
    void DeInit();

    /** For copying the attributes in this class.*/
    void CopyAttributes(const CGUIComboBoxHeader& kSource);

    /** Reimplemented to avoid this event being handled in CGUIBaseInputField.
        @param eKeyID The Guiliani key identifier.
        @param uiModifiers The key modifiers bitfield.
        @return True If the event has been handled.<BR>
                false Else.
    */
    eC_Bool DoKeyDown(
        const GUIKeyIdentifier_t& eKeyID,
        const eC_UInt& uiModifiers);

    CGUIInputField* m_pEdit;

    ///The button in the right side of the header.
    CGUIButton*     m_pButton;
};
#endif
