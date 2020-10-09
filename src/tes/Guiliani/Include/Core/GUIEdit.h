/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIEDIT_H
#define GUIEDIT_H

#include "GUIObject.h"
#include "GUIText.h" // for alignment enums

class CGUIEditableText;
class CGUIKeyboardEvent;

/// No-frills text input control, not to be used on its own.

/** If you want to have a fully functional input control, use CGUIBaseInputField
    or a derived class. Using CGUIEdit directly will lead to problems;
    especially focusing issues will occur.

    This control maintains (and clips) a CGUIEditableText. It deals with
    keyboard input. It inserts, deletes and selects text as needed. It also
    animates the cursor of the text so that it appears to be blinking. The
    cursor position is also maintained here, performing the necessary
    operations when this control receives mouse events (button down sets
    the cursor position, dragging changes the selection, double clicking
    selects everything, SHIFT key combined with button down selects text).

    When pressing the 'activate' (ENTER) key while this control is focused,
    the cursor blinking is stopped, and the event is transferred to the parent
    to be handled.

    Pressing the BACKSPACE or DELETE key deletes the current selection. If
    no text is selected, a character next to the cursor is deleted.

    The cursor can be moved around by pressing the cursor keys, HOME and END.
    If the SHIFT key is held down while pressing either of these keys, the text
    between the previous and the new cursor position is selected.

    The set of characters that are accepted as input (see #AcceptedChars_t) can
    be set when creating an object of this class. It may be changed later by
    calling SetAcceptedChars(). This attribute is only used for direct user
    input (i.e., DoChar()). The characters are not checked when changing the
    text in this control in other ways, like calling SetTextString(),
    InsertTextString() or streaming.

    @ingroup GUILIANI_CONTROLS

<h3>Platform Specific Limitations </h3>
*/

// @guiliani_doxygen limitations CGUIEdit
/**
GUIEdit does only work for horizontally left-alignment text (class CGUIEdit).
*/
// @endguiliani_doxygen
/**
    See @ref subsec_limitations_CGUIEdit "CGUIEdit Platform Specific Limitations"
*/

class CGUIEdit
    : public CGUIObject
{
public:
    /// Sets of characters that are accepted as input in a CGUIEdit.
    enum AcceptedChars_t
    {
        /// All characters.
        AC_ALL,
        /// All letters of the Latin alphabet and whitespace characters.
        AC_LETTERS,
        /// Arabic digits.
        AC_DIGITS,
        /// Latin letters, arabic digits and whitespace characters.
        AC_ALPHANUMERIC,
        /// No characters.
        AC_NONE
    };

    /** CGUIEdit constructor.
        @param pkParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object.
        @param vY X-position relative to its parent object.
        @param vWidth Width of the input field.
        @param vHeight Height of the input field.
        @param rkText Initial text to be shown in the input field.
        @param eAcceptedChars The character set accepted as input.
        @param eID Object Identifier of this input field (use NO_HANDLE if none is required).
    */
    CGUIEdit(
        CGUICompositeObject *const pkParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const eC_String &rkText,
        AcceptedChars_t eAcceptedChars = AC_ALL,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIEdit constructor.
        @param pkParent Pointer to the designated parent object.
        @param kRect Client rectangle of the new CGUIEdit relative to the parent.
        @param rkText Initial text to be shown in the input field.
        @param eAcceptedChars The character set accepted as input.
        @param eID Object Identifier of this input field (use NO_HANDLE if none is required).
    */
    CGUIEdit(
        CGUICompositeObject *const pkParent,
        const CGUIRect &kRect,
        const eC_String &rkText,
        AcceptedChars_t eAcceptedChars = AC_ALL,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Copy constructor.
        @param rkSource Original instance which is copied.
      */
    CGUIEdit(const CGUIEdit& rkSource);

    /** CGUIEdit destructor */
    ~CGUIEdit();

    /** Assignment operator method of the CGUIEdit class.
        @param rkSource Source object to be copied.
        @return Copied object.
    */
    CGUIEdit& operator=(const CGUIEdit& rkSource);

    virtual eC_Bool DoDraw();

    /// Edits the text in reaction to keyboard events.
    /** This implementation does the following:
        - If cursor left or right is pressed, one of the following may happen:
          -# If there is a selection and the SHIFT key is pressed,
             the selection may be extended.
             Otherwise, the selected text is deselected.
          -# If there is no selection, the cursor is moved one character to the
             left or right, depending on the pressed key.
        - If ESCAPE key is pressed and there is a selection,
          the selected text is deselected.
        - If HOME or END key is pressed, one of the following may happen:
          -# If SHIFT key is pressed, the text between the current cursor
             and the beginning or end of the line is selected.
          -# If SHIFT key is not pressed, the cursor goes to the beginning
             or the end of the line.
        @param eKeyIdentifier The Guiliani key identifier.
        @param uiModifiers Modifiers bitfield.
        @return different values based on the action carried out:
        - True if cursor left or cursor right
          was pressed. This is necessary to keep focusing reasonable.
        - false for any other type of event to allow further processing by
          parent objects.
    */
    eC_Bool DoKeyDown(const GUIKeyIdentifier_t &eKeyIdentifier, const eC_UInt &uiModifiers);

    /** This implementation does the following:
          -# If there is a selection, the selected text is replaced by the
             character.
          -# If there is no selection, the character is inserted at the current
             cursor position.
        @param uiKey The code of the pressed key.
        @param eKeyIdentifier The Guiliani key identifier.
        @param uiModifiers Modifiers bitfield.
        @return Always False.
    */
    eC_Bool DoChar(
        const eC_UInt &uiKey,
        const GUIKeyIdentifier_t &eKeyIdentifier,
        const eC_UInt &uiModifiers);

    /** Get the CGUIEditableText label of the input field.
        @return Pointer to the label.
    */
    CGUIEditableText* GetLabel() { return m_pkText; }

    /** Changes the internal text. Deletes the previous text and stores the
        pointer passed to this method.
        @param pkText The new text to be used.
      */
    void SetLabel(CGUIEditableText* pkText);

    /** Set the height of the edit field. Reimplemented because this affects
        the internal text and its cursor.
        @param vY New height of the edit field.
    */
    void SetHeight(const eC_Value &vY);

    /** Set the maximum length of the input text.
        @param iMaxLength The maximum length of input text.
      */
    void SetMaxLength(eC_Int iMaxLength);

    /** @return the maximum length of the input text. */
    inline eC_Int GetMaxLength() const { return m_iMaxLength; }

    /** Set the type of characters that are accepted by DoChar().
        This method does not check the current text and does not modify it.
        @param eAcceptedChars The character set accepted as input.
        @see GetAcceptedChars()
      */
    void SetAcceptedChars(AcceptedChars_t eAcceptedChars);

    /** @return The current set of characters accepted as input as set by
                SetAcceptedChars() or the constructor.
      */
    inline AcceptedChars_t GetAcceptedChars() const { return m_eAcceptedChars; }

    /** Set the text. If there already is text, it is replaced completely.
        If the text's length exceeds the max length, it is truncated.
        @param rkTextString The string to be assigned to this object.
    */
    virtual void SetTextString(const eC_String& rkTextString);

    /** This sets the text string associated with the given TextID.
        @note This will NOT use the actual TextID and will therefore NOT be updated
        if the TextID's text changes.
        @param eTextID TextID whose associated text shall be shown on the GUIEdit. */
    virtual void SetTextID(const TextResource_t &eTextID);

    /** Deletes selected text (if any) and inserts text at the cursor's position.
        If the total length after inserting would exceed the max length, the
        text passed text is truncated before inserting (existing text is not
        deleted).
        @param rkTextString The string to be inserted.
    */
    void InsertTextString(const eC_String& rkTextString);

    /** Gets the selected text.
        @return The currently selected text.
    */
    eC_String GetSelectedTextString();

    /** Deletes the selected text (if any).
        @return The selected text string before deletion.
    */
    eC_String DeleteSelectedTextString();

    /** Starts the cursor blinking animation.
      */
    void GetFocus();

    /** Deselects all text by resetting the cursor position to 0 and stops the
        cursor blinking animation.
        @see CGUIEditableText::SetCursorPosition()
      */
    void LoseFocus();

    /** Standard constructor
        @see CGUIObject().
    */
    CGUIEdit();

    /** Sets the cursor position to the position returned by
        CGUIEditableText::GetBestCursorPosition() and starts the cursor
        blinking animation. If SHIFT is pressed, the text between the current
        cursor position and clicked-position is selected.
        @param  vAbsX X-position in screen coordinates where the click occurred
        @param  vAbsY Y-position in screen coordinates where the click occurred
        @return True The event has been handled.<BR>
                False The event has not been handled.
    */
    eC_Bool DoButtonDown(const eC_Value& vAbsX, const eC_Value& vAbsY);

    /** Selects the whole text.
        @param  vAbsX X-position in screen coordinates where the click occurred
        @param  vAbsY Y-position in screen coordinates where the click occurred
        @return True The event has been handled.<BR>
                False The event has not been handled.
    */
    eC_Bool DoDoubleClick(const eC_Value& vAbsX=eC_FromInt(-1), const eC_Value& vAbsY=eC_FromInt(-1));

    /** Extends the text selection.
        @param vDeltaX Mouse movement in X since last frame
        @param vDeltaY Mouse movement in Y since last frame
        @param vAbsX Mouse position in X where event occurred
        @param vAbsY Mouse position in Y where event occurred
        @return Always True
    */
    eC_Bool DoDrag(const eC_Value& vDeltaX, const eC_Value& vDeltaY, const eC_Value& vAbsX, const eC_Value& vAbsY);

    /** Animates the cursor by toggling the internal CGUIEditableText's cursor.
        @param vTimes Correction factor
    */
    virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1));

    /** Add an observer to get informed when the text of the edit field is changed.
        CGUIObserver::OnNotification(const CGUIValue&, const CGUIObject* const, const eC_UInt, const eC_UInt)
        will be called whenever the text has changed.
        @param pObserver The Observer that wants to subscribe.
        @ingroup GUILIANI_SUBJECTS
    */
    void AddTextChangedObserver(CGUIObserver* pObserver);

    /** Unsubscribe an observer that subscribed as TextChanged observer.
        @param pObserver The Observer that wants to unsubscribe.
    */
    void RemoveTextChangedObserver(CGUIObserver* pObserver);

    /** Computes the cursor pixel position from the current cursor position.
        If the cursor is outside the containing object so that it would be
        clipped, this method ensures the cursor's visibility by scrolling the
        text vertically.
      */
    void EnsureCursorVisible();

    /** Specifies whether the cursor position and blinking state will be reset when the focus is lost.
        By default, a CGUIEdit will reset the cursor when it loses the focus. For use cases with onscreen keyboards
        this behaviour might not be desired.
        @param bResetCursor True will reset the cursor, False will leave it untouched.
    */
    void SetResetCursorWhenFocusLost( const eC_Bool bResetCursor) { m_bResetCursorWhenFocusLost = bResetCursor; }
    
#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

private:

    /** Helper function for constructor. */
    void Init();

    /** Copies private attributes. Helper for copy construction.
        @param rkSource The source object to be copied.
      */
    void CopyAttributes(const CGUIEdit& rkSource);

    /** Starts the cursor blinking animation. It also sets the cursor visible
        so that it can be used as a blinking restart method, too.
      */
    void StartCursorBlinking();

    /** Stops the cursor blinking animation and sets the cursor invisible.
      */
    void StopCursorBlinking();

    /** Helper function to determine whether a character is allowed according
        to the current 'set of accepted characters' (m_eAcceptedChars).
        @param uiChar The character to be checked.
        @return True if the character is accepted, otherwise False.
      */
    eC_Bool IsCharAllowed(eC_UInt uiChar) const;

    /** The text that is displayed by this CGUIEdit.
      */
    CGUIEditableText *m_pkText;

    /** Whether the previous input character was an intermittent character,
        i.e. the character is not 'finished' yet and may be changed by the
        next key event.
      */
    eC_Bool m_bPreviousCharacterWasIntermittent;

    /// The set of characters accepted as input for this CGUIEdit.
    AcceptedChars_t m_eAcceptedChars;

    ///Subject informing observers whenever the text of the GUIEdit changes.
    CGUISubject m_kTextChangedSubject;

    /// Maximum length of the input string.
    eC_Int m_iMaxLength;

    /// When True, the cursor position and blinking state will be reset when the focus is lost
    eC_Bool m_bResetCursorWhenFocusLost;
};

#endif
