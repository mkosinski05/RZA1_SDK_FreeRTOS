/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIBASEINPUTFIELD__H_
#define GUIBASEINPUTFIELD__H_

#include "GUICompositeObject.h"
#include "GUIEditableText.h"
#include "GUIEdit.h"
#include "GUICommand.h"

class CGUIValidator;

/// Base class for text input controls.

/** Like all the "BaseXXXXX"-Classes, this serves primarily as base-class, from
    which you can derive your own customized objects.
    For an inputfield implementation which uses images to visualize itself, please refer
    to CGUIInputField.
    
    This is a wrapper control for the very basic CGUIEdit. It manages focusing
    and editability and serves as a base for more sophisticated input fields,
    for instance with background images, additional icons etc.

    This control is a composite object whose only child is a CGUIEdit. The size
    of the edit field is determined by the size of this control and configurable
    border widths (see SetBorderWidths()).
    These widths define the distances of this control's edges to
    the edges of the internal edit field. By default, the border widths are set
    to 2 pixels for each edge. Since this control draws a frame and a background
    rectangle, this results in a 1-pixel space between the frame and the edit
    field. The frame and background colors are configurable.

    The internal edit field is by default not focusable so that keyboard
    focusing only finds this control but not the child. Clicking this control
    makes the internal edit field focusable and requests the focus for it,
    setting the cursor at the clicked position. Pressing the 'activate' key
    on this control while it is focused also makes the internal edit field
    focusable, transfers the focus to the edit field and selects all text.

    The text content of the input field can be set and read by calling the
    methods SetText() and GetText(). Alternatively, a pointer to the internal
    CGUIEditableText can be obtained by calling GetLabel(). This can also be
    used for more detailed formatting of the text. It is also possible to
    change the internal text object by calling SetLabel().

    The password mode enables the input field to obfuscate the input using a
    specified character that is displayed instead of the actual input.

    A CGUICommand can be executed after the user enters
    text in the input field and presses the action (ENTER) key. The command can
    be attached to the input field by calling the SetActionCommand().

    User input can be validated by attaching a CGUIValidator (SetValidator()).
    The input is validated and potentially changed whenever the user finishes
    input by either pressing the action key or when leaving the input field,
    i.e. by passing the focus to a focusable control outside of the input
    field. Validation is done by calling Validate(). This method may also be
    called at other times if this is needed by the application.

    Example:
@code

    // Create base input field.
    CGUIBaseInputField *pBaseInputField = new CGUIBaseInputField(this,
                    eC_FromInt(0), eC_FromInt(0),
                    eC_FromInt(110), eC_FromInt(30),
                    "", CGUIEdit::AC_ALL,
                    OBJ_BASEINPUTFIELD);

    // Set the text.
    pBaseInputField->SetText("Hello World!");

    // Get the text.
    eC_String kStr = pBaseInputField->GetText();

    // Sets uniform border widths for all four edges.
    pBaseInputField->SetBorderWidth(eC_FromInt(3));

    // Limit user input to numbers between 10 and 20.
    pBaseInputField->SetValidator(new CGUIIntValidator(10, 20));

@endcode

    @ingroup GUILIANI_CONTROLS
*/
class CGUIBaseInputField
    : public CGUICompositeObject
{
public:
    /** CGUIBaseInputField constructor.
        @param pParent Pointer to the designated parent object
        @param vX X-position relative to its parent object
        @param vY X-position relative to its parent object
        @param vWidth Width of the input field
        @param vHeight Height of the input field
        @param rkText Initial text to be shown in the input field
        @param eAcceptedChars The character set accepted as input.
        @param eID Object Identifier of this input field (use NO_HANDLE if none is required)
    */
    CGUIBaseInputField(
        CGUICompositeObject *const pParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const eC_String &rkText,
        CGUIEdit::AcceptedChars_t eAcceptedChars = CGUIEdit::AC_ALL,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIBaseInputField constructor.
        @param pParent Pointer to the designated parent object
        @param kRect The client rectangle (control position and size).
        @param rkText Initial text to be shown in the input field
        @param eAcceptedChars The character set accepted as input.
        @param eID Object Identifier of this input field (use NO_HANDLE if none is required)
    */
    CGUIBaseInputField(
        CGUICompositeObject *const pParent,
        const CGUIRect &kRect,
        const eC_String &rkText,
        CGUIEdit::AcceptedChars_t eAcceptedChars = CGUIEdit::AC_ALL,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIBaseInputField destructor */
    ~CGUIBaseInputField();

    /** Copy-constructor of the CGUIBaseInputField class.
        @param kSource Source object to be copied.
    */
    CGUIBaseInputField(const CGUIBaseInputField& kSource);

    /** Operator= method of the CGUIBaseInputField class.
        @param kSource Source object to be copied.
        @return Copied object.
    */
    CGUIBaseInputField& operator=(const CGUIBaseInputField& kSource);

    /** Sets the text label within the InputField's CGUIEdit
        @see CGUIEditableText::SetTextString()
        @param rkValue The CGUIValue containing the new value(s)
        @return Always True.
    */
    virtual eC_Bool SetValue( const CGUIValue& rkValue)
    {
        if( GetLabel())
        {
            GetLabel()->SetTextString( rkValue.ToString() );
            InvalidateArea();
        }
        CGUIObject::SetValue( rkValue);
        return true; 
    }

    /** Set the text.
        @note This will not trigger notifications for observers of this InputField.
        @param rkTextString The string to be assigned to this object.
    */
    virtual void SetText(const eC_String& rkTextString);

    /** Returns the current text content of the input field.
        In password mode, this method returns the string of obfuscation
        characters. To obtain the entered password, use GetPassword().
        @return a copy of the current text content of the input field.
      */
    inline eC_String GetText() const
    {
        if (NULL != m_pkEdit)
        {
            return *(m_pkEdit->GetLabel()->GetText());
        }
        else
        {
            return "";
        }
    }

    /** Retrieves a pointer to the Inputfield#s internal CGUIEdit. 
        @return Pointer to internal CGUIEdit instance
    */
    inline CGUIEdit* GetEdit() const
    {
        return m_pkEdit;
    }

    /** Transparent call to CGUIEditableText::GetPassword().
        @return a copy of the password, or a copy of the visible text if the
                input field is not in password mode.
      */
    inline eC_String GetPassword() const
    {
        if (NULL != m_pkEdit)
        {
            return *(m_pkEdit->GetLabel()->GetPassword());
        }
        else
        {
            return "";
        }
    }

    /** Set the width of the InputField.
        Automatically adjusts size of internal objects.
        @param vWidth New width of the InputField
    */
    void SetWidth(const eC_Value &vWidth);

    /** Set the height of the InputField.
        Automatically adjusts size of internal objects.
        @param vHeight New height of the InputField
    */
    void SetHeight(const eC_Value &vHeight);

    /** Set the widths of the inner borders.
        The border width is the distance of this control's edges to the edges
        of the internal CGUIEdit.
        @param vLeftBorder Width of the left inner border.
        @param vRightBorder Width of the right inner border.
        @param vTopBorder Width of the top inner border.
        @param vBottomBorder Width of the bottom inner border.
        @see SetBorderWidth()
      */
    void SetBorderWidths(eC_Value vLeftBorder, eC_Value vRightBorder, eC_Value vTopBorder, eC_Value vBottomBorder);

    /** Sets uniform border widths for all four edges.
        @param vBorderWidth The inner border width for each border.
        @see SetBorderWidths()
      */
    inline void SetBorderWidth(eC_Value vBorderWidth)
    {
        SetBorderWidths(vBorderWidth, vBorderWidth, vBorderWidth, vBorderWidth);
    }

    /** Returns the border widths that have been set with SetBorderWidth() or
        SetBorderWidths().
        @param[out] vLeft   Width of the left border.
        @param[out] vRight  Width of the right border.
        @param[out] vTop    Width of the top border.
        @param[out] vBottom Width of the bottom border.
      */
    void GetBorderWidths(eC_Value& vLeft, eC_Value& vRight, eC_Value& vTop, eC_Value& vBottom) const;

    /// @return Width of the left border.
    inline eC_Value GetBorderWidthLeft() const
    {
        if (NULL != m_pkEdit)
        {
            return m_pkEdit->GetRelXPos();
        }
        else
        {
            return eC_FromInt(0);
        }
    }

    /// @return Width of the right border.
    inline eC_Value GetBorderWidthRight() const
    {
        if (NULL != m_pkEdit)
        {
            return GetWidth() - m_pkEdit->GetRelXPosREdge();
        }
        else
        {
            return eC_FromInt(0);
        }
    }

    /// @return Width of the top border.
    inline eC_Value GetBorderWidthTop() const
    {
        if (NULL != m_pkEdit)
        {
            return m_pkEdit->GetRelYPos();
        }
        else
        {
            return eC_FromInt(0);
        }
    }

    /// @return Width of the bottom border.
    inline eC_Value GetBorderWidthBottom() const
    {
        if (NULL != m_pkEdit)
        {
            return GetHeight() - m_pkEdit->GetRelYPosBEdge();
        }
        else
        {
            return eC_FromInt(0);
        }
    }

    /** Sets the color used as the background for selected text.
        @param uiCol The selected text color.
        @see CGUIEditableText::SetSelectionColor()
      */
    inline void SetSelectedTextColor(eC_UInt uiCol) const
    {
        if (NULL != m_pkEdit)
        {
            m_pkEdit->GetLabel()->SetSelectionColor(uiCol);
        }
    }

    /** Get the color currently used as the background for selected text.
        @return the selected text color.
        @see CGUIEditableText::GetSelectionColor()
      */
    inline eC_UInt GetSelectedTextColor() const
    {
        if (NULL != m_pkEdit)
        {
            return m_pkEdit->GetLabel()->GetSelectionColor();
        }
        else
        {
            return 0;
        }
    }

    /** Set the maximum length of the input text.
        @param iMaxLength The maximum length of input text.
      */
    void SetMaxLength(eC_Int iMaxLength);

    /** @return the maximum length of the input text. */
    eC_Int GetMaxLength() const;

    /** Set the type of characters that are accepted by CGUIEdit::DoChar().
        This call is forwarded to CGUIEdit::SetAcceptedChars().
        @param eAcceptedChars The character set accepted as input.
        @see GetAcceptedChars()
      */
    void SetAcceptedChars(CGUIEdit::AcceptedChars_t eAcceptedChars);

    /** @return The current set of characters accepted as input as set by
                SetAcceptedChars() or the constructor. If the internal CGUIEdit
                control does not exist, CGUIEdit::AC_NONE is returned.
        @see CGUIEdit::GetAcceptedChars()
      */
    CGUIEdit::AcceptedChars_t GetAcceptedChars() const;

    /** Activate the password mode to obfuscate the input using a
        specified character that is displayed instead.
        @param kPwdStr The character that shall be displayed.
    */
    inline void SetPasswordMode(eC_String& kPwdStr)
    {
        if (NULL != m_pkEdit)
        {
            GetLabel()->SetPasswordCharacter(kPwdStr);
            if (kPwdStr.IsEmpty())
                GetLabel()->SetPasswordMode(false);
            else
                GetLabel()->SetPasswordMode(true);
            InvalidateArea();
        }
    }

    /** Get the label of the input field.
        Use this to obtain a pointer to the internal text for formatting etc.
        @return Pointer to the internal text label.
    */
    CGUIEditableText* GetLabel()
    {
        if (NULL != m_pkEdit)
        {
            return m_pkEdit->GetLabel();
        }
        else
        {
            return NULL;
        }
    }

    /** Changes the internal text. This call is directly passed through to
        CGUIEdit::SetLabel.
        @param pkText The new text to be used by the internal edit control.
      */
    void SetLabel(CGUIEditableText* pkText);

    /** Sets a validator for checking any user input in this input field.
        The input fields owns the validator, i.e. it deletes it when necessary.
        @param pValidator The validator to be set. If there is a previous
               validator it is deleted. Pass NULL to remove the current
               validator.
      */
    void SetValidator(CGUIValidator* pValidator);

    /** @return The currently set validator.
        @see SetValidator()
      */
    CGUIValidator* GetValidator() { return m_pkValidator; }

    /** Uses the validator (see SetValidator()) to validate the current text.
        If the value is not valid, the validator is asked to make the value
        valid. In this case, this method then calls SetText() to apply the
        new value.

        If there is no validator, this method does nothing.
      */
    void Validate();

    /** Sets the internal edit field not focusable. Required for correct
        behaviour when leaving the input field via direction keys.
      */
    void LoseFocus();

    /** When the Input field receives the focus, the edit field's contest will be selected */
    void GetFocus();

    /** Draws a frame and background rectangle.
        @return True if something was painted, otherwise False.
      */
    virtual eC_Bool DoDraw();

    /** Handles incoming keyboard events.
        For all keys in the range of GK_A to GK_DIVIDE, StartEditing() will be
        called and the key will be handled by the internal edit field.   
        If the 'activate' button is pressed, two things might happen:
        - If this control (the input field) is currently focused, the focus is
        transferred to the internal edit field, all text is selected and the
        cursor is positioned at the end.
        - If the internal edit field is currently focused, the focus is
        transferred to this control (input field), effectively ending the
        editing operation. The edit field is set to 'not focusable'. The
        command that was set with SetActionCommand() is executed.

        @see CGUIEdit::SelectAll()
        @see StartEditing()
        @param eKeyIdentifier The Guiliani key identifier.
        @param uiModifiers The key modifiers bitfield.
        @return Always False.
      */
    eC_Bool DoKeyDown(const GUIKeyIdentifier_t &eKeyIdentifier, const eC_UInt &uiModifiers);

    /** If not already done, the input field is set to editing mode.
        All content is selected.
    */
    virtual void StartEditing();

    /** If this control receives a button down event, it means that the internal
        edit field is currently not focusable but the user wants to edit it.
        This method sets the edit field focusable and transfers the focus to it.
        @param  vAbsX X-position in screen coordinates where the click
            occurred
        @param  vAbsY Y-position in screen coordinates where the click
            occurred
        @return True The event has been handled.<BR>
                False The event has not been handled.
    */
    eC_Bool DoButtonDown(const eC_Value& vAbsX, const eC_Value& vAbsY);

    /** Standard constructor.
    */
    CGUIBaseInputField();

    /** Attaches an action command to the input field.
        This command is executed whenever the action key is pushed.
        @param pCommand The command to be attached.
    */
    inline void SetActionCommand(CGUICommandPtr pCommand) { m_pActionCmd = pCommand; }

    /** Add an observer to get informed when the text of the inputfield is changed.
        CGUIObserver::OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX=0, const eC_UInt uiY=0))
        will be called whenever the text has changed.
        @param pObserver The Observer that wants to subscribe.
        @ingroup GUILIANI_SUBJECTS
    */
    void AddTextChangedObserver(CGUIObserver* pObserver);

    /** Unsubscribe an observer that subscribed as TextChanged observer.
        @param pObserver The Observer that wants to unsubscribe.
    */
    void RemoveTextChangedObserver(CGUIObserver* pObserver);

    /** Add an observer to get informed when the user started the input.
        CGUIObserver::OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX=0, const eC_UInt uiY=0)
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
        CGUIObserver::OnNotification(const CGUIValue& kObservedValue, const CGUIObject* const pkUpdatedObject, const eC_UInt uiX=0, const eC_UInt uiY=0))
        will be called whenever the user finished.
        @param pObserver The Observer that wants to subscribe.
        @ingroup GUILIANI_SUBJECTS
    */
    void AddInputFinishedObserver(CGUIObserver* pObserver);

    /** Unsubscribe an observer that subscribed as InputFinished observer.
        @param pObserver The Observer that wants to unsubscribe.
    */
    void RemoveInputFinishedObserver(CGUIObserver* pObserver);

    /** Calls the base class implementation and takes care of resetting
        internal pointers if specific children are removed.
        @param pObject The child object to be removed.
    */
    virtual void RemoveObject(CGUIObject* pObject);

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

protected:
    /// The internal edit field.
    CGUIEdit *m_pkEdit;

    /** The command to be executed after the user pushes the action (ENTER) key.
    */
    CGUICommandPtr m_pActionCmd;

    ///Subject informing observers whenever the input has started.
    CGUISubject m_kInputStartedSubject;

    ///Subject informing observers whenever the input is finished.
    CGUISubject m_kInputFinishedSubject;

private:
    /** Helper functions for constructor. */
    void Init(const eC_String& rkString, CGUIEdit::AcceptedChars_t eAcceptedChars);

    /** Copy attributes method of the CGUIBaseInputField class.
        @param kSource Source object to be copied.
    */
    void CopyAttributes(const CGUIBaseInputField& kSource);

    /// The currently set validator for this input field.
    CGUIValidator* m_pkValidator;
};
#endif
