/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUIEDITABLETEXT__H_)
#define GUIEDITABLETEXT__H_

#include "GUIText.h"
#include "GUIRect.h"
#include "eC_TList_doubleLinked.h"

/// Specialization of CGUIText that allows for modification and selection of text.

/** This text maintains a cursor position and a selection, which can be thought
    of as two cursor positions. A cursor position is defined as a point (described
    by CursorPos_t) denoting the position of a character. Cursor position (W, Y)
    indicates that the cursor is to the left of the W-th character in Line Y.
    The default cursor position after constructing an instance of this class
    is at the end of the text.

    The cursor position can be visualized by drawing a cursor whose width and
    visibility can be influenced. The cursor is represented by a vertical line
    that has the same height as one text line and a configurable width (default
    is 1 pixel).

    The color of the base class (CGUIText) standard text is used.
    Note that since this text is no CGUIObject it does not perform a blinking
    animation for the cursor, it rather draws the cursor or not. See CGUIEdit or
    CGUIMultiLineEdit for controls that use this text to display a blinking cursor.

    A continuous portion of the text can be selected. The selection range
    is described by two cursor positions, a 'start' and an 'end' position
    relative to the cursor position. If these two values are the same, no text
    is selected. If they differ, the text from 'start' to 'end' is selected
    and is marked visually by a different background color, which is also
    configurable, see SetSelectionColor().
    There are various methods to influence the selection from outside and to
    obtain information about the current selection.

    It is possible to insert a string into the text at current cursor position or delete
    string from the text by giving start and end cursor positions.

    The word wrap functionality is enabled at construction time. It can also be
    configured with SetWordWrap().

    The password mode enables the editable text to obfuscate the input using a
    specified character that is displayed instead of the actual user input.

    Note that only left-aligned text is allowed within GUIEditableText.
*/
class CGUIEditableText : public CGUIText
{
public:
    /// Holds a cursor position.
    struct CursorPos_t{
        eC_UInt m_uiCharIndex; ///< Cursor position of current line starting from index 0.
        eC_UInt m_uiLineIndex; ///< The line where the cursor is located.
        /** Constructor.
            @param uiCharIndex Index of the character within the line.
            @param uiLineIndex Index of line within the text.
          */
        CursorPos_t(eC_UInt uiCharIndex = 0, eC_UInt uiLineIndex = 0)
            : m_uiCharIndex(uiCharIndex), m_uiLineIndex(uiLineIndex)
        {
        }
    };

    /** Constructor.
    With word wrap is enabled, the default width is set to be given width.
    If word wrap is disabled, the default width is set to be the required width
    of the given text string.
    @param pParentObject Pointer to object containing the text.
    @param kText         Reference of text string.
    @param vXPos         X position of text relative to the parent.
    @param vYPos         Y position of text relative to the parent.
    @param vTextWidth    The width of the text.
    @param bWordWrap     false: text is always displayed in a single line,
                         true: lines can be wrapped.
    */
    CGUIEditableText(
        const CGUIObject* const pParentObject,
        const eC_String &kText,
        const eC_Value &vXPos=eC_FromInt(0),
        const eC_Value &vYPos=eC_FromInt(0),
        const eC_Value &vTextWidth = eC_FromInt(0),
        eC_Bool bWordWrap = false);

    /** Constructor.
    With word wrap is enabled, the default width is set to be given width.
    If word wrap is disabled, the default width is set to be the required width
    of the given text string.
    @param pParentObject Pointer to object containing the text.
    @param pkText        Pointer of text string.
    @param vXPos         X position of text relative to the parent.
    @param vYPos         Y position of text relative to the parent.
    @param vTextWidth    The width of the text.
    @param bWordWrap     false: text is always displayed in a single line,
                         true: lines can be wrapped.
    */
    CGUIEditableText(
        const CGUIObject* const pParentObject,
        const eC_String *const pkText,
        const eC_Value &vXPos=eC_FromInt(0),
        const eC_Value &vYPos=eC_FromInt(0),
        const eC_Value &vTextWidth = eC_FromInt(0),
        eC_Bool bWordWrap = false);

    /** Copy constructor.
        @param rkSource The text instance to be copied.
    */
    CGUIEditableText(const CGUIEditableText& rkSource);

    /** Destructor.
      */
    ~CGUIEditableText();

    /** Assignment operator.
        @param rkSource Text instance, which is the source of the assignment.
        @return The assignment destination
    */
    CGUIEditableText& operator=(const CGUIEditableText& rkSource);

    /** Inserts characters at the current cursor position.
        After this call returns, the cursor is positioned at the end of the
        inserted text.
        @param kInsChars The character string to be inserted.
      */
    void Insert(const eC_String& kInsChars);

    /** Delete strings from the start cursor position of given start line and column to the end
        cursor position of given end line and column.
        @param uiCursorPosAtStartLine The cursor position index of the line where the deletion starts.
        @param uiCursorPosAtEndLine The cursor position index of the line where the deletion finishes.
        @param uiStartLine Line where the deletion starts.
        @param uiEndLine Line where the deletion finishes.
      */
    void Delete(eC_UInt uiCursorPosAtStartLine, eC_UInt uiCursorPosAtEndLine,
        eC_UInt uiStartLine = 0, eC_UInt uiEndLine = 0);

    /** Draws the text and everything that belongs to it.
        This includes drawing the selection if there is one, calling the base
        class implementation to print the text and drawing the cursor if it is
        set to be visible.
        @see SetSelectionColor()
        @see SetCursorVisible()
        @see CGUIText::PrintText()
      */
    virtual void PrintText();

    /** Get the absolute rectangle of the cursor.
        @return The absolute rectangle of the cursor.
    */
    CGUIRect GetCursorAbsRect();

    /** Retrieves the password. If the text is currently not in password mode,
        the visible text is returned.
        @return A pointer to the password or visible text.
    */
    virtual const eC_String* const GetPassword();

    /** Get the cursor position index and the line where the cursor locates.
        @return The cursor position and current line stored in CursorPos_t structure.
        @see CursorPos_t
    */
    CursorPos_t GetCursorPosition();

    /** Get the selected start cursor position and the start line.
        @return The cursor position and start line stored in CursorPos_t structure.
        @see CursorPos_t
      */
    CursorPos_t GetSelectionStart();

    /** Get the selected end cursor position and the end line.
        Note: The cursor index starts at 0.
        @return The cursor position and end line stored in CursorPos_t structure.
        @see CursorPos_t
      */
    CursorPos_t GetSelectionEnd();

    /** Get the cursor position by the char index of the current text string.
        @param uiCharIndex The char index of the current text.
               Note: The char index starts from 1.
        @return The cursor position to be found.
        @see CursorPos_t
      */
    CursorPos_t GetCursorPosByCharIndex(eC_UInt uiCharIndex);

    /** Replaces the current text with another one.
        This results in deleting the current text and inserting the new one.
        The cursor position is at the beginning of the new text and
        ResetSelection() is called.
        @note This is a time-consuming function. 
        @param rkTextString The new text.
      */
    virtual void SetTextString(const eC_String& rkTextString);

    /** Set the cursor position of given line.
        Note: The cursor position starts at 0.
        @param uiCursorPosAtLine The cursor position.
        @param uiCurrentLine The line where the cursor locates.
      */
    void SetCursorPosition(eC_UInt uiCursorPosAtLine,
                           eC_UInt uiCurrentLine = 0);

    /** Finds the closest cursor position to an arbitrary pixel position, for
        instance a mouse position.
        @param vRelXPos X position relative to this text's containing CGUIObject.
        @param vRelYPos Y position relative to this text's containing CGUIObject.
        @return The cursor position to be obtained.
        @see CursorPos_t
      */
    CursorPos_t GetBestCursorPosition(const eC_Value &vRelXPos, const eC_Value &vRelYPos);

    /** Determines whether this text currently has a selection.
        @return True if there is a selection, otherwise False.
      */
    inline eC_Bool HasSelection() { return m_kSelStartIndexOfWidthList != m_kSelEndIndexOfWidthList; }

    /** Selects all characters and positions the cursor at the end of the text.
      */
    void SelectAll();

    /** Enables or disables the word wrap functionalities. If the word wrap is enabled,
        the text width is set to be the given width. Otherwise, the text width is adjusted
        to be the required width of current text string.
        @param bWordWrap Indicate whether the word wrap is enabled or not.
        @param vWidth The width that is set to the control is the word wrap is enabled.
    */
    void SetWordWrap(eC_Bool bWordWrap, const eC_Value& vWidth = eC_FromInt(0));

    /** Deselects all characters so that there is no selection.
        @note Internally, this method sets the selection 'start' and 'end'
        positions equal to the current cursor position. This way, a subsequent
        call to ExtendSelection() starts the 'new' selection at this position.
      */
    void ResetSelection();

    /** Extends the current selection up to a specific position.
        The selection area is from the current cursor to the specific position,
        and the current cursor position becomes the specific position.
        @param uiCharIndex The character index up to which the selection is extended.
        @param uiLineIndex The line up to which the selection is extended.
      */
    void ExtendSelection(eC_UInt uiCharIndex, eC_UInt uiLineIndex = 0);

    /** Deletes the current selection. Results in a call to Delete() with the
        current selection start/end indices.
      */
    void DeleteSelection();

    /** Sets the cursor visibility. If the cursor is set to visible, it is
        drawn in the next call of PrintText().
        @param bVisible The visibility of the cursor.
      */
    void SetCursorVisible(eC_Bool bVisible);

    /** Toggles the cursor visibility. Convenience method for controls that
        implement a blinking cursor.
        @see SetCursorVisible()
      */
    inline void ToggleCursor() { SetCursorVisible(!IsCursorVisible()); }

    /** Determines the current visibility of the cursor.
        @return True if the cursor is currently visible, otherwise False.
      */
    inline eC_Bool IsCursorVisible() const { return m_bCursorVisible; }

    /** Sets the width of the cursor.
        @param vWidth The width of the vertical cursor line.
      */
    inline void SetCursorWidth(eC_Value vWidth) { m_vCursorWidth = vWidth; }

    /** Returns the width of the cursor.
        @return the width of the vertical cursor line.
      */
    inline eC_Value GetCursorWidth() const { return m_vCursorWidth; }

    /** Sets the color used for drawing the background of selected text.
        @param uiSelectionColor The background color of selected text.
      */
    inline void SetSelectionColor(eC_UInt uiSelectionColor) { m_uiSelectionColor = uiSelectionColor; }

    /** Returns the background color of selected text.
        @return the background color of selected text.
      */
    inline eC_UInt GetSelectionColor() const { return m_uiSelectionColor; }

    /** Calls the base class implementation and updates the character widths.
        @param eFontID The new font for the text.
      */
    virtual void SetFont(const FontResource_t &eFontID);

    /** Activate the password mode to obfuscate the input using a
        specified character that is displayed instead.
        @param kPwdStr The character that shall be displayed. Pass an empty
               string to disable the password mode and display readable text.
      */
    void SetPasswordCharacter(eC_String& kPwdStr);

    /** Set the password-mode or clear it
        @param bPasswordMode state of password-mode
    */
    void SetPasswordMode(eC_Bool bPasswordMode);

    /** Determines the last valid cursor position of this text (position after
        the last character).
        @return The last cursor position.
        @see CursorPos_t
    */
    CursorPos_t GetLastCursorPos();

    /** Get the string of the given line.
        @param uiCurrentLine The line number
        @return The string of the given line.
    */
    eC_String GetStrOfLine(eC_UInt uiCurrentLine) const;

    /** Returns the currently selected text.
        @return The selected text if there is selection, otherwise an empty string.
    */
    eC_String GetSelectedText();

    /// Returns the height of one line of text. Not to be confused with GetTextHeight().
    inline eC_Value GetLineHeight() { return m_vLineHeight; }

    /** Clones the instance of the GUIEditableText object.
        @return Pointer to the cloned GUIEditableText object. */
    virtual CGUIEditableText* Clone() const;

#ifdef GUILIANI_STREAM_GUI
    void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteTextType=false);
#endif

private:

    typedef eC_TListDoubleLinked<eC_Value> WidthList;

    typedef WidthList::Iterator WidthListIterator;

    /** Type to record current cursor index in the accumulated width list.
    */
    class CursorIndex_t
    {
    public:
        /** Constructor
            @param uiCursorIndex The cursor index of the accumulated width list.
            @param kCursorIter The iterator obtained from accumulated width list corresponding
                               to the given cursor index.
            @param pList The list into which kCursorIter points.
        */
        CursorIndex_t(eC_UInt uiCursorIndex, const WidthListIterator& kCursorIter, WidthList* pList)
            : m_uiCursorIndex(uiCursorIndex),
              m_kCursorIter(kCursorIter),
              m_pList(pList)
        {
        }

        /** Default constructor
        */
        CursorIndex_t()
            : m_uiCursorIndex(0), m_pList(NULL)
        {
        }

        /** Prefix increment operator.
            If the next cursor position is invalid, it does nothing.
            @return This cursor index object.
        */
        CursorIndex_t& operator++() // prefix
        {
            if ( (m_pList != NULL) && (m_uiCursorIndex+1 < m_pList->GetQuantity()))
            {
                ++m_kCursorIter;
                ++m_uiCursorIndex;
            }
            return *this;
        }

        /** Postfix increment operator.
            If the next cursor position is invalid, it does nothing.
            @return This cursor index object before the operation.
        */
        CursorIndex_t operator++(int) // postfix
        {
            CursorIndex_t kResult = *this;
            operator++();
            return kResult;
        }

        /** Prefix decrement operator.
            If the previous cursor position is invalid, it does nothing.
            @return This cursor index object.
        */
        CursorIndex_t& operator--() // prefix
        {
            if (m_uiCursorIndex > 0)
            {
                --m_uiCursorIndex;
                --m_kCursorIter;
            }
            return *this;
        }

        /** Postfix decrement operator.
            If the previous cursor position is invalid, it does nothing.
            @return The cursor index object before the operation.
        */
        CursorIndex_t operator--(int) // postfix
        {
            CursorIndex_t kResult = *this;
            operator--();
            return kResult;
        }

        /** Dereferencing operator.
            @return The accumulated width corresponding to the iterator.
        */
        eC_Value operator*()
        {
            if(m_kCursorIter.IsValid())
            {
                return *m_kCursorIter;
            }
            return eC_FromInt(0);
        }

        /** ==operator.
            @return True, if both the indices and iters are equal.
        */
        inline eC_Bool operator==(CursorIndex_t kCursorIndex)
        {
            return m_uiCursorIndex == kCursorIndex.m_uiCursorIndex &&
                m_kCursorIter == kCursorIndex.m_kCursorIter;
        }

        /** ==operator.
            @return True, If the indices are equal.
        */
        inline eC_Bool operator==(eC_UInt uiCursorIndex)
        {
            return m_uiCursorIndex == uiCursorIndex;
        }

        /** !=operator.
            @return True, if either the indices or iters are not equal.
        */
        inline eC_Bool operator!=(CursorIndex_t kCursorIndex)
        {
            return !operator==(kCursorIndex);
        }

        /** !=operator.
            @return True, If the indices are not equal.
        */
        inline eC_Bool operator!=(eC_UInt uiCursorIndex)
        {
            return m_uiCursorIndex != uiCursorIndex;
        }

        /** > operator.
            @param uiCursorIndex The given cursor index.
            @return True, If current index is larger than given one.
        */
        inline eC_Bool operator>(eC_UInt uiCursorIndex)
        {
            return m_uiCursorIndex > uiCursorIndex;
        }

        /** < operator.
            @param uiCursorIndex The given cursor index.
            @return True, If current index is smaller than given one.
        */
        inline eC_Bool operator<(eC_UInt uiCursorIndex)
        {
            return m_uiCursorIndex < uiCursorIndex;
        }

        /** Set the cursor index and iterator.
            @param uiCursorIndex The cursor index of the accumulated width list.
            @param kCursorIter The iterator obtained from accumulated width list corresponding
                               to the given cursor index.
            @param pList The list into which kCursorIter points.
        */
        inline void SetCursorIndex(eC_UInt uiCursorIndex, const WidthListIterator& kCursorIter, WidthList* pList)
        {
            m_uiCursorIndex = uiCursorIndex;
            m_kCursorIter = kCursorIter;
            m_pList = pList;
        }

        /** Get the cursor index.
            @return The cursor index of the accumulated width list.
        */
        inline eC_UInt GetCursorIndex() const {return m_uiCursorIndex;}

        /** Get the cursor index.
            @return The iterator obtained from accumulated width list corresponding
                    to the given cursor index.
        */
        inline WidthListIterator GetCursorIter() const {return m_kCursorIter;}

    private:
        /// The cursor index of the accumulated width list.
        eC_UInt m_uiCursorIndex;

        /** The iterator obtained from accumulated width list corresponding to the given cursor index.
        */
        WidthListIterator m_kCursorIter;

        /// The list into which m_kCursorIter points.
        WidthList* m_pList;
    };

    /** Hidden default constructor.
      */
    CGUIEditableText();

    /** Initializes members to defaults.
      */
    void Init(const eC_Value &vTextWidth);

    /** Copies all attributes from another text instance.
      */
    void CopyAttributes(const CGUIEditableText& rkSource);

    /** This method is hidden so that the user can not arbitrarily
        change the text width. The text width is given in constructor,
        if word wrap is enabled. It can also be changed after the word
        wrap is switched from disabled to enabled. If the word wrap is
        disabled, the text width is adjusted to be the same as the
        required space of the text.
      */
    inline void SetTextWidth(const eC_Value &vTextWidth)
    {
        CGUIText::SetTextWidth(vTextWidth);
    }

    /** This method is hidden. Please use SetWordWrap method.
      */
    virtual void SetTextSingleLine(const eC_Bool &bSingleLine)
    {
        CGUIText::SetTextSingleLine(bSingleLine);
    }

    virtual void CollectHyphenationMarks(eC_String& rkStr, eC_UInt uiOffset) 
    { /* Hyphenation is not supported for editable texts */}

    /** Draws the background rectangle for selected text.
      */
    void DrawSelection();

    /** Draws the vertical cursor line.
      */
    void DrawCursor();

    /** Sets the cursor to a specific index. There is a list to store
        the cursor position. This is to set the current cursor index of this list.
        @param uiIndexOfWidthList The new current index in the width list.
    */
    void SetIndexOfWidthList(eC_UInt uiIndexOfWidthList);

    /** Sets the current selection.
        Important: This method does not touch the cursor position, but for a
        good user experience it is necessary to keep the cursor at the beginning
        or end of the selection. It is the responsibility of the caller to make
        sure of that.
        @param uiStartIndexOfWidthList The starting index in the width list that marks the start selection.
        @param uiEndIndexOfWidthList The end index in the width list that marks the end of selection.
      */
    void SetSelection(eC_UInt uiStartIndexOfWidthList, eC_UInt uiEndIndexOfWidthList);

    /** Remove a string that starts with start index in the width list and end with the end index
        in the width list from the text.
        @param uiStartIndexOfWidthList Start index in the width list that marks the start of deletion.
        @param uiEndIndexOfWidthList End index in the width list that marks the end of the deletion.
               It must be equal to or greater than uiStartIndex and
               equal to or less than the number of characters in the text
               plus the quantity of lines.
    */
    void RemoveString(eC_UInt uiStartIndexOfWidthList, eC_UInt uiEndIndexOfWidthList);

    /** Get the index in the width list by the cursor position and line value.
        @param tCursorLinePos Indicate the cursor position.
        @return The index of the width list.
    */
    eC_UInt LinePosToIndexOfWidthList(const CursorPos_t &tCursorLinePos);

    /** Get the cursor position and line value from a width list index.
        @param uiIndexOfWidthList The index in the width list.
        @return The cursor position and its located line.
    */
    CursorPos_t GetCursorPositionFromIndex(eC_UInt uiIndexOfWidthList);

    /** This list contains the accumulated width for each character index of
        the current text.
        For example, in the first line, the first width is 0, the second width
        is the width of the first character, the third one is the width
        of the first plus the second character, and so on. If word wrap is enabled,
        for second line, the first width is 0 again, and the following widths are
        accumulated like in the first line, and so on.
        So the quantity of the accumulated widths is the number of characters in the
        current text plus the number of the lines.
      */
    WidthList m_kAccumulatedWidth;

    /** Updates the values in m_kAccumulatedWidth list. For text without word wrap, it just calculates the
        width from the beginning of the line to every character in the given adjusted string, and update the
        values in the width list. With word wrap, it calculates the width from the beginning of the
        current line to every character in the given adjusted string. It then checks whether the control width
        is enough to hold the line. If it is not, it calls AdjustWidths method to do the further adjustment,
        so that it fits current line and also the following lines.
        @note This repeatedly calls CGfxWrap::RequiredSpace() which is expensive,
        so do not call this method unless absolutely necessary.
        @param kAdjustedStr The string that is to be recalculated. Note: for insertion, it is the inserted string.
        @param uiStartIndexOfWidthList The start index in the width list from where the width value must be recalculated.
        @see m_kAccumulatedWidth
      */
    void ComputeWidths(const eC_String &kAdjustedStr, eC_UInt uiStartIndexOfWidthList);

    /** By deleting, inserting characters or change the word wrap, after the recalculation
        of the width list, the widths in the width list must be adjusted to check whether
        every line string fits to the text width. If it does not, then the line string is
        cut to fit the width and the rest text is inserted to the beginning of the
        next line and do the adjust further.
        @param StartStrIter The iterator in the string list where the adjustment starts.
        @param StartWidthIter The first width iterator for the currently to be adjusted line.
        @param EndWidthIter The last width iterator for the currently to be adjusted line.
    */
    void AdjustWidths(ExtendedStringList_t::Iterator &StartStrIter,
                      WidthListIterator &StartWidthIter,
                      WidthListIterator &EndWidthIter);

    /** In the situation of inserting or removing string in the text, there may be some incoherence
        the index and width iterator, thus the current width index have to be moved to a safe position
        where the incoherence does not happen. The safe cursor width index is the position where the
        last space character of the previous line locates if there is a previous line available.
        If there is no previous line, the current index of the width list is moved to the previous
        space of current line.
        @param uiCurrentWidthIndex The current width index.
        @param uiCursorPosAtCurrentLine Indicate the cursor position at the current line.
        @param StrIter The string iterator corresponds to the current line string.
        @return How many positions are moved, i.e. the quantities of charaters between
                current width index and result width index.
    */
    eC_UInt MoveCurrentWidthIndexToSafePos(eC_UInt uiCurrentWidthIndex,
        eC_UInt uiCursorPosAtCurrentLine, ExtendedStringList_t::Iterator &StrIter);

    /** Adjust the string. This method is called in the AdjustWidths method
        to cut string so that the result string is fit to the text width.
        @param[out] pkResultStr The result string that fits the text width.
        @param[in] pkStrToAnalyse The original string that is to be analysed.
        @param[out] pkRestStr The rest string that is cut of from the original analysed
                    string except the result string.
        @param[in,out] StartWidthIter For input it is the width iter indicating the
                       begining of the analysed string. For output it is the width iter
                       indicating the last position of the result string.
    */
    void AdjustStr(eC_String* pkResultStr, eC_String* pkStrToAnalyse,
        eC_String* pkRestStr, WidthListIterator &StartWidthIter);

    /** Ensures that the text's width is not smaller than the containing object's width. 
        This is required for text alignments. */
    void EnsureMinimumTextWidth();

    /// The current index of the width list.
    CursorIndex_t m_kIndexOfWidthList;

    /// The selection start index in the width list.
    CursorIndex_t m_kSelStartIndexOfWidthList;

    /// The selection end index in the width list.
    CursorIndex_t m_kSelEndIndexOfWidthList;

    /// The background color for selected text.
    eC_UInt m_uiSelectionColor;

    /// The width of the cursor.
    eC_Value m_vCursorWidth;

    /// The current visibility of the cursor.
    eC_Bool m_bCursorVisible;

    /// True if PasswordMode is enabled.
    eC_Bool m_bPasswordMode;

    /// Password string containing the plain readable text
    eC_String* m_pkPassword;

    /// The symbol used for representing single characters within the password
    eC_String m_kPwdChar;
};

#endif
