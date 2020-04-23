/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUI_MULTI_LINE_EDIT__H_
#define GUI_MULTI_LINE_EDIT__H_

#include "GUIScrollView.h"
#include "eC_TList_doubleLinked.h"
#include "GUIRect.h"
#include "GUIEditableText.h"
class eC_String;
class CGUIKeyboardEvent;

/** Type to define list that contains multiple CGUIEditableText object pointers.
*/
typedef eC_TListDoubleLinked<CGUIEditableText*> TextPtrList;

/// Helper for CGUIMultiLineEdit.

// @guiliani_doxygen toplevel_control Multi Line Edit
/**
    <table border="0">
        <tr>
            <td width="200">@image html multiple_line_edit.png</td>
            <td>
                The "multi line ldit" control contains an editable area in which the user can enter text. If the text is longer than the container itself, a vertical scroll bar will appear to scroll through the text.
                If word wrapping is enabled, the width of the text area is fixed, otherwise a horizontal scroll bar is shown if the text is wider than the text area (Class: CGUIMultiLineEdit).
                The "multi line edit" control supports nine patch images (CGUINinePatch).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen
/**
    A text container has a list of editable texts that are printed in this control.
*/
class CGUITextContainer : public CGUIObject
{
public:
    friend class CGUIMultiLineEdit;
    /** CGUITextContainer constructor.
        @param pParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object.
        @param vY Y-position relative to its parent object.
        @param vWidth Width of the composite object.
        @param vHeight Height of the composite object.
        @param eID Object Identifier of this composite object (choose NO_HANDLE if none is required).
    */
    CGUITextContainer(CGUICompositeObject *const pParent, const eC_Value &vX,
                      const eC_Value &vY, const eC_Value &vWidth,
                      const eC_Value &vHeight, const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUITextContainer constructor.
        @param pParent Pointer to the designated parent object.
        @param kRect Bounding rectangle relative to parent.
        @param eID Object Identifier of this composite object (choose NO_HANDLE if none is required).
    */
    CGUITextContainer(CGUICompositeObject *const pParent, const CGUIRect &kRect,
                      const ObjectHandle_t &eID = NO_HANDLE);

    /// Default constructor
    CGUITextContainer() : CGUIObject() {}

    /** Destructor
    */
    virtual ~CGUITextContainer();

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

private:

    /** Free the text list.
    */
    void FreeTextList();

    /** print the list of texts.
        @return True if something was painted, otherwise False.
    */
    virtual eC_Bool DoDraw();

    /** Obtains the list of text.
        @param The reference of the list of text.
    */
    TextPtrList& GetTextList() {return m_kTextList;}

    /** Copy constructor.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGUITextContainer(const CGUITextContainer& kSource);

    /** Assignment operator.
        Dummy implementation to avoid unintended use of compiler-generated default
    */
    CGUITextContainer& operator=(const CGUITextContainer& kSource);

    /** The list of texts that are displayed in the container.
    */
    TextPtrList m_kTextList;
};

/// A multiple line edit control.

/** A multiple line edit contains an editable area in which the user can enter text.
    It supports word wrapping. If word wrapping is enabled, the width of the text area
    is fixed. Manual line breaks can be inserted by pressing ENTER. If word wrapping is
    disabled, a horizontal scrollbar is shown if the text is longer than the text area.

    The user can navigate through the text by using the mouse and keyboard. Clicking the
    mouse in the text area sets the editing cursor to a position as close as possible to
    the mouse cursor position. The cursor keys can be used to move the cursor left and right
    by one character or up and down by one line. The page up/down keys can be used to scroll
    the text by one page.

    Text can be <i>selected</i> by dragging the mouse over the text or by using the movement
    keys in conjunction with the SHIFT modifier. Whenever text is selected, the next movement
    or character input influences the selection. If the cursor is moved while text is selected,
    the text is deselected. If a character key is pressed while text is selected, the selected
    text is replaced by the entered character. Selected text can be deleted by pressing DELETE
    or BACKSPACE.

    Basic cut, copy and paste functionality is implemented here. The selected text can be copied
    by pressing CTRL+C and cut by pressing CTRL+X. These functions transfer the text into a
    <i>clip board</i>. The content of this clip board can be pasted (inserted at the current
    cursor position) by pressing CTRL+V. If any text is selected at this point, the selected text
    is replaced by the clip board content.

    When constructing an instance of this class, initial text can be passed directly to the
    constructor. Later, the text can be changed with SetText(). The edit can be emptied
    by calling Empty(). You can obtain the entire text with GetEntireText().

    @remark The following issues with this control are known:
    - No interfaces for getting or setting the cursor position.
    - Noticeably slow when editing long lines without line breaks.

    @ingroup GUILIANI_CONTROLS
    @ingroup GUILIANI_TEXT
    */
class CGUIMultiLineEdit
    : public CGUIScrollView
{
public:

    /** CGUIMultiLineEdit constructor.
        @param pkParent Pointer to the designated parent object.
        @param vX X-position relative to its parent object.
        @param vY X-position relative to its parent object.
        @param vWidth Width of the input field.
        @param vHeight Height of the input field.
        @param kInitStr The initial string to be set to the string.
        @param bWordWrap Indicate whether the word wrap is enabled or not.
        @param bEditable Create editable or read-only edit field.
        @param eID Object Identifier of this input field (use NO_HANDLE if none is required).
    */
    CGUIMultiLineEdit(
        CGUICompositeObject *const pkParent,
        const eC_Value &vX, const eC_Value &vY,
        const eC_Value &vWidth, const eC_Value &vHeight,
        const eC_String& kInitStr = "",
        eC_Bool bWordWrap = false,
        eC_Bool bEditable = true,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** CGUIMultiLineEdit constructor.
        @param pkParent Pointer to the designated parent object.
        @param kRect Client rectangle of the new CGUIMultiLineEdit relative to the parent.
        @param kInitStr The initial string to be set to the string.
        @param bWordWrap Indicate whether the word wrap is enabled or not.
        @param bEditable Create editable or read-only edit field.
        @param eID Object Identifier of this input field (use NO_HANDLE if none is required).
    */
    CGUIMultiLineEdit(
        CGUICompositeObject *const pkParent,
        const CGUIRect &kRect,
        const eC_String& kInitStr = "",
        eC_Bool bWordWrap = false,
        eC_Bool bEditable = true,
        const ObjectHandle_t &eID = NO_HANDLE);

    /** Copy constructor.
        @param rkSource Original instance which is copied.
      */
    CGUIMultiLineEdit(const CGUIMultiLineEdit& rkSource);

    /** Assignment operator
        @param rkSource Source object to be copied.
        @return Copied object.
    */
    CGUIMultiLineEdit& operator=(const CGUIMultiLineEdit& rkSource);

    /** CGUIMultiLineEdit destructor */
    virtual ~CGUIMultiLineEdit();

    /** Enable or disable word wrap.
        @param bWordWrap Indicate whether the word wrap is enabled or disabled.
    */
    void SetWordWrap(eC_Bool bWordWrap);

    /** Determine whether word wrap is currently enabled.
        @return True if word wrap is enabled.
      */
    inline eC_Bool IsWordWrap() { return m_bWordWrap; }

    /** Adjust the Y position of the text in the scroll view.
        @param pCurEditableText Current EditableText
    */
    void AdjustYPositionOfText(CGUIEditableText* pCurEditableText);

    /** Handles the keyboard events.
        The following KEYDOWN events are handled:
        -# ESCAPE:      Release the selection.
        -# DOWN:        Move cursor to next line if current line is not the last line.
                        If SHIFT key is pressed, it extends the selection to next line.
                        If SHIFT key is not pressed, the selection is released.
        -# UP:          Move cursor to previous line if current line is not the first line.
                        If SHIFT is pressed, it extends the selection to next line.
                        If SHIFT key is not pressed, the selection is released.
        -# LEFT:        Move cursor one character left. If current cursor is at the beginning
                        of the line, it moves to the end of the previous line, if current line is
                        not the first line. If SHIFT key is pressed, it extends the selection
                        correspondingly. If SHIFT key is not pressed, the selection is released.
        -# RIGHT:       Move cursor one character right. If current cursor is at the end
                        of the line, it moves to the beginning of the next line, if current line is
                        not the last line. If SHIFT key is pressed, it extends the selection
                        correspondingly. If SHIFT key is not pressed, the selection is released.
        -# HOME:        Move cursor to the beginning of the current line. If SHIFT key is pressed,
                        it extends the selection correspondingly.
                        If SHIFT key is not pressed, the selection is released.
        -# END:         Move cursor to the end of the current line. If SHIFT key is pressed,
                        it extends the selection correspondingly.
                        If SHIFT key is not pressed, the selection is released.
        -# PAGEDOWN:    Move cursor one page down and scroll to show the cursor. If it can not be scrolled,
                        it does nothing. If SHIFT key is pressed, it extends the selection correspondingly.
                        If SHIFT key is not pressed, the selection is released.
        -# PAGEUP:      Move cursor one page up and scroll to show the cursor. If it can not be scrolled,
                        it does nothing. If SHIFT key is pressed, it extends the selection correspondingly.
                        If SHIFT key is not pressed, the selection is released.
        -# ENTER:       Delete the selected text if available and cut current line into two lines. The cursor stays in next line.
        -# BACK:        Delete the selected text if available. If there is no selection, it deletes one character before cursor. If the cursor
                        is at beginning of a line, the line is merged with the previous line, if current line is not the first line.
        -# DELETE:      Delete the selected text if available. If there is no selection, it deletes one character after the cursor. If the cursor
                        is at end of a line, the line is merged with the next line, if current line is not the last line.
        -# Valid-Char:  Delete the selected text if available. Insert typed character.
        -# SELECT_ALL:  Select all text.
        -# COPY:        Copy the selected text to clipboard.
        -# CUT:         Cut the selected text to clipboard.
        -# PASTE:       Delete selected text if available and paste recently copied string to cursor position.
        @param pEvent The event to be handled
        @return True if processed, False otherwise
    */
    virtual eC_Bool DoUserEvent(CGUIEvent* const pEvent);

    /** Set the text string to the object. The old string is deleted
        and the new string is separated to multiple line if there are line
        breaks inside the string.
        @param kWholeStr The string to be assigned to this object.
    */
    void SetText(const eC_String& kWholeStr);

    /** Remove all text from the input area.
    */
    void Empty();

    /** Start the cursor blinking after the control gets the focus.
    */
    void GetFocus();

    /** Stop the cursor blinking after the control loses the focus.
    */
    void LoseFocus();

    /** If mouse is clicked over the editing area, the following happens:
        -# If the SHIFT key is not held down, the cursor is set to the position at which
           the mouse was clicked. The selection is released.
        -# If the SHIFT key is held down, the selection is extended to
           the position of the text where the mouse was clicked.
        @param  vAbsX X-position in screen coordinates where the click occurred
        @param  vAbsY Y-position in screen coordinates where the click occurred
        @return True The event has been handled.<BR>
                False The event has not been handled.
    */
    virtual eC_Bool DoButtonDown(const eC_Value& vAbsX, const eC_Value& vAbsY);

    /** The previous selection is released and the line in which the mouse was
        clicked is selected.
        @param  vAbsX X-position in screen coordinates where the click occurred
        @param  vAbsY Y-position in screen coordinates where the click occurred
        @return True The event has been handled.<BR>
                False The event has not been handled.
    */
    virtual eC_Bool DoDoubleClick(const eC_Value& vAbsX=eC_FromInt(-1),
                          const eC_Value& vAbsY=eC_FromInt(-1));

    /** Selects the text over which the mouse is dragged.
        @param vDeltaX Mouse movement in X since last frame
        @param vDeltaY Mouse movement in X since last frame
        @param vAbsX Mouse position in X where event occurred
        @param vAbsY Mouse position in Y where event occurred
        @return True If the event has been handled.<BR>
                False Else.
    */
    virtual eC_Bool DoDrag(const eC_Value& vDeltaX, const eC_Value& vDeltaY,
                   const eC_Value& vAbsX, const eC_Value& vAbsY);

    /** Toggles the cursor visibility to make it blink.
        @param vTimes The correction factor
    */
    virtual void DoAnimate(const eC_Value& vTimes = eC_FromInt(1));

    /** Get the entire text of this edit. If the user had entered manual line
        breaks in the text, the returned string contains line break characters
        at the respective positions.
        @return The result string.
    */
    eC_String GetEntireText() const;

    /** Sets the font for the entire text.
        @param eFontID The identifier of the font to be used.
    */
    void SetFont(const FontResource_t &eFontID);

    /** Sets the font spacing for the entire text.(space between letters).
        @param fFontSpacing Font spacing in subpixels (1.0 subpixels equals 1
        pixel).
    */
    void SetFontSpacing(const eC_Float &fFontSpacing);

    /** Sets the color for the entire text. All colors are in ARGB format.
        @param uiTextColorStandard Standard text color.
        @param uiTextColorHighlighted Text color used if the
               parent object is highlighted.
        @param uiTextColorGrayedOut Text color used if the parent
               object is grayed out.
        @param uiTextColorPressed Text color used if the parent
               object is pressed.
    */
    void SetTextColor(const eC_UInt &uiTextColorStandard,
                      const eC_UInt &uiTextColorHighlighted,
                      const eC_UInt &uiTextColorGrayedOut,
                      const eC_UInt &uiTextColorPressed);

    /** Sets the background color of the selected text.
        @param uiSelectionColor  Background color in ARGB format.
    */
    void SetSelectionColor(eC_UInt uiSelectionColor);

    /** Sets the multiline edit read only or writeable.
        @param bEditable True to activate write mode, false for
         a read only edit field.
    */
    void SetEditable(eC_Bool bEditable);

#ifdef GUILIANI_STREAM_GUI
    /** Standard constructor
        @see CGUICompositeObject().
    */
    CGUIMultiLineEdit();

    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteClassID = false);
#endif

    /** Insert a given string at the current cursor position.
        After inserting, the cursor is positioned behind the inserted text.
        @param kInsertStr The string to be inserted.
    */
    void InsertString(const eC_String& kInsertStr);

    /** Positions the cursor at the very end of the currently existing text and
        calls InsertString().
        @param rkText The text to be appended.
      */
    void Append(const eC_String& rkText);

    /** Deliberately does nothing. Re-implemented to prevent jumping scroll bars.
        This needs to be done because the base implementation relies on various
        focusable child objects in the container. The CGUIMultiLineEdit only
        has one child, the text container, which is also focusable, but the
        scroll bar position depends on the cursor position within that
        container, not on the container itself.
      */
    virtual void ScrollToFocussedObject();

    /// Reimplemented for detecting removal of specific internal children.
    void RemoveObject(CGUIObject *pObject);

private:

    /** Creates a new CGUIEditableText instance and configures it.
        @param kNewTextStr The given new text string.
        @param vYPos The relative y position for the new text.
        @return The new generated editable text pointer.
    */
    CGUIEditableText* GenerateNewText(const eC_String& kNewTextStr, const eC_Value& vYPos);

    /** Sets the scrollbar policy for the vertical scrollbar. The vertical
        scrollbar can only be set to be always visible or invisible. If
        the policy is CGUIScrollView::AUTOMATIC, the policy is not changed.
        @param ePolicy Policy for the vertical scrollbar.
    */
    virtual void SetVerticalScrollbarPolicy( const ScrollBarPolicy ePolicy)
    {
        if(ePolicy == AUTOMATIC) return;
        m_eVerticalScrollBarPolicy = ePolicy;
    }

    /** Sets the scrollbar policy for the horizontal scrollbar. The horizontal
        scrollbar can only be set to be always visible or invisible. If
        the policy is CGUIScrollView::AUTOMATIC, the policy is not changed.
        @param ePolicy Policy for the horizontal scrollbar.
    */
    virtual void SetHorizontalScrollbarPolicy( const ScrollBarPolicy ePolicy)
    {
        if(ePolicy == AUTOMATIC) return;
        m_eHorizontalScrollBarPolicy = ePolicy;
    }

    /** Helper for handling key down events.
        @param pEvent The keyboard event.
    */
    eC_Bool UserEventKeyDown(CGUIKeyboardEvent* pEvent);

    /** Helper for handling key up events.
        @param pEvent The keyboard event.
    */
    eC_Bool UserEventKeyUp(CGUIKeyboardEvent* pEvent);

    /** Helper for handling char events.
        @param pEvent The keyboard event.
    */
    eC_Bool UserEventChar(CGUIKeyboardEvent* pEvent);

    /** Scroll the cursor into the visible editing area if necessary.
        This method also calls CGUIScrollView::AdjustScrollBars() which in turn
        calls ScrollToFocussedObject().
    */
    void EnsureCursorVisible();

    /** Determine the line index that corresponds to an absolute y position.
        @param vAbsYPos The absolute y position.
        @return The line index.
    */
    eC_UInt GetTextIndexByAbsYPos(const eC_Value& vAbsYPos);

    /** Extend the selection to the given cursor position of the new line.
        @param uiNewTextIndex The new text index the selection is extended to.
        @param kCursorPos The new cursor position.
    */
    void ExtendSelection(eC_UInt uiNewTextIndex, const CGUIEditableText::CursorPos_t& kCursorPos);

    /** Release the selection.
    */
    void ReleaseSelection();

    /** Get the absolute rectangle of the cursor.
        @param The absolute rectangle of the cursor.
    */
    CGUIRect GetCursorAbsRect();

    /** Starts the cursor blinking animation. It also sets the cursor visible
        so that it can be used as a blinking restart method.
      */
    void StartCursorBlinking();

    /** Stops the cursor blinking animation and sets the cursor to visible or not
        according to the given parameter.
        @param bCursorVisible Indicate whether the cursor is set to be visible or not.
    */
    void StopCursorBlinking(eC_Bool bCursorVisible);

    /** Helper function for constructor.
        @param bWordWrap Indicate whether the word wrap is enabled or not.
        @param kInitStr The initial string that is to be set to the edit.
        @param bEditable Set true to create an editable edit field, false
         creates a read-only one.
    */
    void Init(eC_Bool bWordWrap, const eC_String& kInitStr, eC_Bool bEditable);

    /// Helper that sets scroll bar policies based on m_bWordWrap.
    void InitScrollBars();

    /** Copies private attributes. Helper for copy construction.
        @param rkSource The source object to be copied.
      */
    void CopyAttributes(const CGUIMultiLineEdit& rkSource);

    /** Handle the UP or DOWN key event.
        @param bKeyDown Indicate whether it is to handle the key up
                        event or key down event.
        @param bShiftPressed Indicate whether the shift key is pressed or not.
    */
    void HandleKeyUPOrDown(eC_Bool bKeyDown, eC_Bool bShiftPressed);

    /** Handle the LEFT key event.
        @param bShiftPressed Indicate whether the shift key is pressed or not.
    */
    void HandleKeyLeft(eC_Bool bShiftPressed);

    /** Handle the RIGHT key event.
    */
    void HandleKeyRight(eC_Bool bShiftPressed);

    /** Handle the HOME key event.
        @param bShiftPressed Indicate whether the shift key is pressed or not.
        @param bCtrlPressed Indicate whether the control key is pressed or not.
    */
    void HandleKeyHome(eC_Bool bShiftPressed, eC_Bool bCtrlPressed);

    /** Handle the END key event.
        @param bShiftPressed Indicate whether the shift key is pressed or not.
        @param bCtrlPressed Indicate whether the control key is pressed or not.
    */
    void HandleKeyEnd(eC_Bool bShiftPressed, eC_Bool bCtrlPressed);

    /** Handle the ACTION key event.
    */
    void HandleKeyAction();

    /** Handle the BACK key event.
    */
    void HandleKeyBack();

    /** Handle the DELETE key event.
    */
    void HandleKeyDelete();

    /** Handle the PAGEUP or PAGEDOWN key event.
        @param bKeyPageDown Indicate whether it is to handle
                            the PAGEUP or PAGEDOWN event.
        @param bShiftPressed Indicate whether the shift key is pressed or not.
    */
    void HandleKeyPageUpOrDown(eC_Bool bKeyPageDown, eC_Bool bShiftPressed);

    /** Delete the selected text.
    */
    void DeleteSelectedText();

    /// Helper for updating the y positions of all texts in the container.
    void UpdateTextYPositions();

    /** Check whether there is a selection.
        @return True, if any text is selected, False otherwise.
    */
    eC_Bool HasSelection();

    /** Hidden to prevent adding any object to
        the container. The container contains only CGUIEditableText pointers.
    */
    eC_Bool AddObject( CGUIObject* pObject){return CGUIScrollView::AddObject(pObject);}

    /// Indicate whether the word wrap is enabled or not.
    eC_Bool m_bWordWrap;

    /// Index of the editing editable text that the cursor is on. Zero-based.
    eC_UInt m_uiCurrentTextIndex;

    /// Index at which the selection starts.
    eC_UInt m_uiSelectionStartIndex;

    /// Index at which the selection ends.
    eC_UInt m_uiSelectionEndIndex;

    /// Stores the original line and position of the cursor before the selection was changed.
    CGUIEditableText::CursorPos_t m_kOrgPosBeforeSelection;

    /// The font ID for the entire text.
    FontResource_t m_eFontID;

    /// The font spacing for the entire text.
    eC_Float m_fFontSpacing;

    /// The standard text color for the entire text.
    eC_UInt m_uiTextColorStandard;

    /// The highlighted text color for the entire text.
    eC_UInt m_uiTextColorHighlighted;

    /// The grayedout text color for the entire text.
    eC_UInt m_uiTextColorGrayedOut;

    /// The pressed text color for the entire text.
    eC_UInt m_uiTextColorPressed;

    /// The background color for selected text.
    eC_UInt m_uiSelectionColor;

    /// Whether the multiline edit is in read-only or write mode.
    eC_Bool m_bEditable;

    CGUITextContainer* m_pTextContainer;
};

#endif
