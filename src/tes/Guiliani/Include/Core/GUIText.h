/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUITEXT__H_
#define GUITEXT__H_

#include "GUIFontResource.h"
#include "GUITextResource.h"
#include "eC_TList_doubleLinked.h"
#include "GUIObserver.h"
#include "GUIStreamableObject.h"
#include "eC_Math.h"
#include "GUIProperties.h"
#include "GUIRect.h"

/// Character used to indicate line breaks in texts.
const eC_Char ccLinebreak = '\n';

///    Class for handling a text.

/** Objects of this class represent texts but are no CGUIObject instances. They
    are instead assigned to a "parent" GUI object that has to take care of
    printing the text in its DoDraw implementation when needed.
    Controls (CGUIObjects) that want to display text should use CGUIText. For
    an example of how to print a simple text somewhere in your GUI, have a look
    at CGUIBaseTextField.

    Each text object has basic attributes like font, text color and so on. It
    also has an area in which the text is aligned. This area is positioned
    relative to its containing ("parent") GUI object and can be adjusted by
    calling the methods SetRelXPos(), SetRelYPos(), SetTextWidth() and
    SetTextHeight(). The horizontal and vertical alignment can be set by
    calling SetAligned().

    Clipping is done at the borders of the assigned containing CGUIObject but
    no extra clipping is done for the area in which the text is aligned. The
    area in which the text is aligned does only matter for formatting and
    positioning the text (line wrapping, cutting with ellipsis, position offset
    from containing object) but not for clipping. Therefore, if part of
    CGUIText's area is outside of CGUIObject's area and text is placed into
    this part then part of the text will be invisible because of clipping
    CGUIObject.
    That is, the text graphics belong to the graphics of the assigned
    containing CGUIObject in the same way as all other graphic of that GUI
    object.

    A text is either a single-line or multi-line text (set either at
    construction time or later by calling SetTextSingleLine()). A single line
    text always shows only one line of text, whereas in multi-line mode the
    text may be wrapped at blank characters to achieve optimum line width
    usage. To further optimise line width usage, it is possible to enter "soft
    hyphenation" markers ("\-") in texts. These markers are not printed but
    enable multi-line texts to break words into pieces and insert hyphens in
    these places.

    To print the character sequence backslash minus ("\-"), another backspace
    has to be prefixed to escape the special meaning, like this: "\\-".

    To force linebreaks, enter ccLinebreak characters into the text.

    In both single and multi-line texts, the text is cut off if it is too long
    to be displayed in its CGUIText area and an ellipsis ("...") is appended at
    the end. Certainly the ellipsis will be inside CGUIText's area.

    Objects of this class can represent two different kinds of text: ID texts
    and string texts. An ID text is represented by a valid TextResource_t ID
    that is used to access the text in CGUILocalisationHandler whereas string
    texts manage an independent string on their own. Each text object internally
    has a text ID but only ID texts have a valid one; for string texts, the ID
    is invalid (TEXT_START_TAG). To determine what kind of text an object is,
    call IsTextID(). A CGUIText object can change its type during its lifetime
    if SetTextString() or SetTextID() is called.

    Example:
@code

    // Create a text.
    CGUIText* pkText = new CGUIText(this,                           // attach to "this" object
                                    "Hello World!",                 // Text to display in this text object
                                    eC_FromInt(0), eC_FromInt(0),   // Position at coordinates 0,0 relative to containing object
                                    true);                          // This text shall be single-line (=no word wrap)

     // Sets the font.
    pkText->SetFont(FontResource_t(GUI_PROP_DEFAULT_FONT));

    // The colors of the text. They are used for different states of the containing object.
    pkText->SetTextColor(0xffff0000, 0xffff00ff, 0xff0000ff, 0xff00ffff);

    // Print text. This method is normally called in the DoDraw method of the containing object.
    pkText->PrintText();

@endcode
@ingroup GUILIANI_TEXT
*/
class CGUIText
    :public CGUIObserver, public CGUIStreamableObject
{
public:

    /// Possible vertical alignments.
    enum VerAligned_t
    {
        V_TOP,      ///< The text is aligned at the top of its area.
        V_CENTERED, ///< The text is vertically centered in its area.
        V_BOTTOM    ///< The text is aligned at the bottom of its area.
    };

    /// Possible horizontal alignments.
    enum HorAligned_t
    {
        H_LEFT,     ///< The text is left-aligned.
        H_CENTERED, ///< The text is horizontally centered.
        H_RIGHT     ///< The text is right-aligned.
    };

    /// @brief Class containing a text, its width and its height
    class ExtendedString_t
    {
    public:
        // Constructs an extended string element.with the default parameters
        ExtendedString_t():
        m_kStrText(""),
        m_vWidth(),
        m_vHeight()
        {};

        /** Constructs an extended string element.
            @param kStrText    The text.
            @param vWidth      Width of the text.
            @param vHeight     Height of the text.
        */
        ExtendedString_t(const eC_String& kStrText, eC_Value vWidth, eC_Value vHeight):
        m_kStrText(kStrText),
        m_vWidth(vWidth),
        m_vHeight(vHeight)
        {}

        /** Retrieves the text.
            @return The text string.
        */
        const eC_String& GetText() const{return m_kStrText;}

        /** Retrieves the width.
            @return The text's width.
        */
        eC_Value GetWidth() const {return m_vWidth;}

        /** Retrieves the height.
            @return The text's height.
        */
        eC_Value GetHeight() const {return m_vHeight;}

        /** Sets the text.
            @param kStrText The text of the extended string.
        */
        void SetText(const eC_String& kStrText){m_kStrText= kStrText;}

        /** Sets the width.
            @param vWidth The width of the text.
        */
        void SetWidth(const eC_Value& vWidth){m_vWidth= vWidth;}

        /** Sets the height.
            @param vHeight The height of the text.
        */
        void SetHeight(const eC_Value& vHeight){m_vHeight=vHeight;}

    private:

        eC_String   m_kStrText;
        eC_Value    m_vWidth;
        eC_Value    m_vHeight;
    };

    /** Constructs a string text.
        By default, the text width and height are set to the width and height
        of the parent object minus the relative positions.
        @param pParentObject Pointer to object containing the text.
        @param kText         Reference of text string.
        @param vXPos         X position of text relative to the parent.
        @param vYPos         Y position of text relative to the parent.
        @param bSingleLine   true: text is always displayed in a single line,
                             false: lines can be wrapped.
    */
    CGUIText(
        const CGUIObject* const pParentObject,
        const eC_String &kText,
        const eC_Value &vXPos=eC_FromInt(0),
        const eC_Value &vYPos=eC_FromInt(0),
        const eC_Bool &bSingleLine = true);

    /** Constructs a string text.
        By default, the text width and height are set to the width and height
        of the parent object minus the relative positions.
        @param pParentObject Pointer to object containing the text.
        @param pkText        Pointer of text string.
        @param vXPos         X position of text relative to the parent.
        @param vYPos         Y position of text relative to the parent.
        @param bSingleLine   true: text is always displayed in a single line,
                             false: lines can be wrapped.
    */
    CGUIText(
        const CGUIObject* const pParentObject,
        const eC_String *const pkText,
        const eC_Value &vXPos=eC_FromInt(0),
        const eC_Value &vYPos=eC_FromInt(0),
        const eC_Bool &bSingleLine = true);

    /** Constructs an ID text.
        By default, the text width and height are set to the width and height
        of the parent object minus the relative positions.
        @param pParentObject Pointer to object containing the text.
        @param eTextID       ID of the internationalised text.
        @param vXPos         X position of text relative to the parent.
        @param vYPos         Y position of text relative to the parent.
        @param bSingleLine   true: text is always displayed in a single line,
                             false: lines can be wrapped.
    */
    CGUIText(
        const CGUIObject* const pParentObject,
        const TextResource_t &eTextID,
        const eC_Value &vXPos=eC_FromInt(0),
        const eC_Value &vYPos=eC_FromInt(0),
        const eC_Bool &bSingleLine = true);

    /** Copy constructor.
        @param kTextToBeCopied The text object to be copied.
    */
    CGUIText(const CGUIText& kTextToBeCopied);

    /// Destructor.
    virtual ~CGUIText();

    /** Retrieves the font.
        @return Font identifier.
    */
    inline FontResource_t GetFont() const {return m_eFont;}

    /** Retrieves the font size of set font.
        @return Font size.
    */
    eC_Int GetFontSize() const;

    /** Retrieves the font spacing (space between two letters).
        @return Font spacing in subpixels.
    */
    inline eC_Float GetFontSpacing() const {return m_fFontSpacing;}

    /** Retrieves the horizontal alignment.
        @return Horizontal alignment.
    */
    inline HorAligned_t GetHorAlign() const {return m_eHorAligned;}

    /** Retrieves the vertical alignment.
        @return Vertical alignment.
    */
    inline VerAligned_t GetVerAlign() const {return m_eVerAligned;}

    /** Get the height of the text area.
        @return The height of the text area.
    */
    virtual eC_Value GetTextHeight() const {return m_vTextHeight;}

    /** Get the width of the text area.
        @return The width of the text area.
    */
    virtual eC_Value GetTextWidth() const {return m_vTextWidth;}

    /** Get the required space for the text.
        If the text is a multi-line text, the required space for the wrapped
        text is returned. If the text is single-line, the width of the whole
        text is returned. Note that the returned width will also include
        potential ellipsis in case the text got truncated.
        @param[out] vTextWidth The required text width.
        @param[out] vTextHeight The required text height.
    */
    virtual void GetRequiredTextSpace(eC_Value& vTextWidth, eC_Value& vTextHeight);

    /** Get the required space for the text.
        Calculates the space required for printing the original text string in
        a single line. No line breaks or hyphenation markers are taken into
        account.
        @param[out] vTextWidth The required text width.
        @param[out] vTextHeight The required text height.
    */
    virtual void GetRequiredOrgTextSpace(eC_Value& vTextWidth, eC_Value& vTextHeight);

    /**
        Get the required height for the complete, uncut text for a given width.
        If the text is a multi-line text, the required height for the wrapped
        text is returned. If the text is single-line, height for the single line is returned.
        @param[in] vAvailableWidth The available width for the text. By default, the current width is used.
        @return The required height.
    */
    virtual eC_Value GetRequiredTextHeightForWidth(eC_Value vAvailableWidth = 0);

    /** Retrieves the text.
        The text is returned as an eC_String* and can be NULL.
        @return A pointer to the text of this object.
    */
    const eC_String* GetText() const;

    /** Retrieves the text color.
        @param uiTextColorStandard Standard Text color in ARGB coding
               (e.g. 0xFFFFFFFF).
        @param uiTextColorHighlighted Text color in ARGB coding if the parent
               object is highlighted.
        @param uiTextColorGrayedOut Text color in ARGB coding if the parent
               object is grayed out.
        @param uiTextColorPressed Text color in ARGB coding if the parent object
               is pressed.
    */
    void GetTextColor(eC_UInt &uiTextColorStandard,
                      eC_UInt &uiTextColorHighlighted,
                      eC_UInt &uiTextColorGrayedOut,
                      eC_UInt &uiTextColorPressed) const;

    /** Retrieves the TextID represented by this object.
        @return The TextID of the text attribute of this object.
                Attention! For a string text the ID is invalid. To determine the
                type of a text object, use IsTextID().
    */
    inline TextResource_t GetTextID() const {return m_eTextID;}

    /** Prints the text. This function may only be called from the DoDraw()
        implementation of the text's containing GUI object as it uses absolute
        positioning.
    */
    virtual void PrintText();

    /** Sets the font.
        @param eFontID The identifier of the font to set.
    */
    virtual void SetFont(const FontResource_t &eFontID);

    /** Sets the font spacing (space between letters).
        @param fFontSpacing Font spacing in subpixels (1.0 subpixels equals 1
        pixel).
    */
    void SetFontSpacing(const eC_Float &fFontSpacing);

    /** Sets the object which contains the text.
        This method just changes the internal pointer to the containing object
        without re-calculating the text layout. If necessary, adjust the text
        positioning and area size manually.
        @param pParent The object which contains the text.
    */
    inline void SetContainingObject(const CGUIObject* const pParent) {m_pParentObject=pParent;}

    /** Get the object which contains the text.
        @return The object which contains the text.
    */
    inline const CGUIObject* GetContainingObject() const {return m_pParentObject;}

    /** Sets the color of the text.
        The colors are used when printing the text. The state of the containing
        CGUIObject determines which color is used.
        @param uiTextColorStandard Standard Text color in ARGB coding
               (e.g. 0xFFFFFFFF).
        @param uiTextColorHighlighted Text color in ARGB coding used if the
               parent object is highlighted.
        @param uiTextColorGrayedOut Text color in ARGB coding used if the parent
               object is grayed out.
        @param uiTextColorPressed Text color in ARGB coding used if the parent
               object is pressed.
    */
    virtual void SetTextColor(const eC_UInt &uiTextColorStandard,
                              const eC_UInt &uiTextColorHighlighted,
                              const eC_UInt &uiTextColorGrayedOut,
                              const eC_UInt &uiTextColorPressed);

    /** Sets the standard color of the text.
        The colors are used when printing the text. The state of the containing
        CGUIObject determines which color is used.
        @param uiTextColorStandard Standard Text color in ARGB coding
        (e.g. 0xFFFFFFFF).
    */
    void SetTextColorStandard(const eC_UInt &uiTextColorStandard);

    /** Set a global color property as standard color.
        @param eTextColorPropertyStandard The id of the property.
    */
    void SetTextColorStandard(const GlobalProperty_t &eTextColorPropertyStandard);


    /** Sets the highlighted color of the text.
    The colors are used when printing the text. The state of the containing
    CGUIObject determines which color is used.
    @param uiTextColorHighlighted Text color in ARGB coding used if the
    parent object is highlighted.
    */
    void SetTextColorHighlighted(const eC_UInt &uiTextColorHighlighted);

    /** Set a global color property as highlighted color.
        @param eTextColorPropertyHighlighted The id of the property.
    */
    void SetTextColorHighlighted(const GlobalProperty_t &eTextColorPropertyHighlighted);

    /** Sets the grayed out color of the text.
    The colors are used when printing the text. The state of the containing
    CGUIObject determines which color is used.
    @param uiTextColorGrayedOut Text color in ARGB coding used if the parent
    object is grayed out.
    */
    void SetTextColorGrayedOut(const eC_UInt &uiTextColorGrayedOut);

    /** Set a global color property as grayed out color.
        @param eTextColorPropertyGrayedOut The id of the property.
    */
    void SetTextColorGrayedOut(const GlobalProperty_t &eTextColorPropertyGrayedOut);

    /** Sets the pressed color of the text.
    The colors are used when printing the text. The state of the containing
    CGUIObject determines which color is used.
    @param uiTextColorPressed Text color in ARGB coding used if the parent
    object is pressed.
    */
    void SetTextColorPressed(const eC_UInt &uiTextColorPressed);

    /** Set a global color property as pressed color.
        @param eTextColorPropertyPressed The id of the property.
    */
    void SetTextColorPressed(const GlobalProperty_t &eTextColorPropertyPressed);

    /** Sets the text ID.
        @param eTextID The ID of the text to assign to this object.
    */
    virtual void SetTextID(const TextResource_t &eTextID);

    /** Sets the string text to be represented by this object.
        @param pkTextString The string to be assigned to this object.
    */
    void SetTextString(const eC_String* pkTextString);

    /** Sets the string text to be represented by this object.
        @param kTextString The string to be assigned to this object.
    */
    virtual void SetTextString(const eC_String& kTextString);

    /** Invalidates the area of the text to be printed.
        Note that InvalidateArea has to be called before and after changing
        the text of this object!
        Example:
        @code
        pMyText->InvalidateArea();
        pMyText->SetTextString("Hello Guiliani!");
        pMyText->InvalidateArea();
        @endcode
    */
    void InvalidateArea();

    /** Sets the relative rectangle for this text
        @param kRect Rectangle relative to containing object which is available for the text
    */
    void SetRelRect(const CGUIRect& kRect)
    {
        SetRelXPos( kRect.GetX1() );
        SetRelYPos( kRect.GetY1() );
        SetTextWidth( kRect.GetWidth() );
        SetTextHeight( kRect.GetHeight() );
    }

    /** Sets the text width.
        @param vTextWidth The width assigned to the text.
    */
    virtual void SetTextWidth(const eC_Value &vTextWidth);

    /** Sets the text height.
        @param vTextHeight The height assigned to the text.
    */
    virtual void SetTextHeight(const eC_Value &vTextHeight);

    /** Sets the text line wrapping mode (single or multi-line).
        @param bSingleLine true: single line; false: multi-line.
    */
    virtual void SetTextSingleLine(const eC_Bool &bSingleLine);

    /** Checks whether this text object is an ID text or a string text.
     *  @return True if this text represents an ID, False if it represents a
     *          string.
     */
    eC_Bool IsTextID() const;

    /** Sets the text alignment.
        @param eVerAligned Vertical alignment
        @param eHorAligned Horizontal alignment
    */
    inline void SetAligned(const VerAligned_t &eVerAligned = V_TOP,
                           const HorAligned_t &eHorAligned = H_LEFT)
    {
        m_eVerAligned = eVerAligned;
        m_eHorAligned = eHorAligned;
    }

    /** Sets relative x position to the parent object. The effect depends on the
        alignment. If the text is left aligned, this value is used relative to
        the left border of the parent object, if it is right aligned, relative
        to the right border of the parent object. If the text is centered,
        changing this value has no effect.
        Attention: the text width will not be re-adjusted by this method. If
        required, please set it by using SetTextWidth().
        @param vRelXPos Relative x position.
    */
    virtual void SetRelXPos(const eC_Value &vRelXPos) {m_vRelXPos = vRelXPos;}

    /** Returns the relative x position to the parent object.
        @return The x position of this text relative to the parent object.
    */
    virtual eC_Value GetRelXPos() const {return m_vRelXPos;}

    /** Sets relative y position to the parent object. The effect depends on the
        alignment. If the text is top aligned, this value is used relative to
        the top border of the parent object, if it is bottom aligned, relative
        to the bottom border of the parent object. If the text is centered,
        changing this has no effect.
        Attention: the text height will not be re-adjusted by this method. If
        required, please set it by using SetTextHeight().
        @param vRelYPos Relative y position.
    */
    virtual void SetRelYPos(const eC_Value &vRelYPos) {m_vRelYPos = vRelYPos;}

    /** Returns the relative y position to the parent object.
        @return The y position of this text relative to the parent object.
    */
    virtual eC_Value GetRelYPos() const {return m_vRelYPos;}

    /** Determines whether this text is single- or multi-line.
        @return True if the text is single-line, False if it is multi-line.
    */
    inline eC_Bool IsTextSingleLine() const {return m_bSingleLine;}

    /** Determine whether the text is cut and an ellipsis (...) is appended.
        @return True if the string is cut and an ellipsis is appended, otherwise
        False.
    */
    inline eC_Bool IsTextCut() { AdaptText(); return m_bTextCut; }

    /** Set positional adjustment values.
        These values are only used by PrintText(). The values are added to the
        current relative x and y positions, but only for printing. A common use
        case for this feature is a button that displays its text one or two
        pixels displaced while the button is pressed down.
        @param vXAdjustment The adjustment in X direction.
        @param vYAdjustment The adjustment in Y direction.
    */
    inline void SetAdjustment(const eC_Value &vXAdjustment,
                              const eC_Value &vYAdjustment)
    {
        m_vXAdjustment = vXAdjustment;
        m_vYAdjustment = vYAdjustment;
    }

    /** Returns adjustment values.
        These values are only used by PrintText(). The values are added to the
        current relative x and y positions, but only for printing. A common use
        case for this feature is a button that displays its text one or two
        pixels displaced while the button is pressed down.
        @param vXAdjustment The adjustment in X direction.
        @param vYAdjustment The adjustment in Y direction.
    */
    inline void GetAdjustment(eC_Value& vXAdjustment, eC_Value& vYAdjustment) const
    {
        vXAdjustment = m_vXAdjustment;
        vYAdjustment = m_vYAdjustment;
    }

    /** Copies the base attributes from a source object.
        This text object will have the same containing object as the source.
        If you want to set another containing object, you should use
        SetContainingObject() to set it.
        @param kSource The source object whose attributes are copied.
        @return CGUIText& Reference to this object.
    */
    CGUIText& operator=(const CGUIText& kSource);

    /** Sets the marker which will be append if a text has to be cut-off because
        of insufficient space.
        @param kEllipsis String to be append on cut-off texts.
    */
    inline static void SetEllipsisString(const eC_String& kEllipsis) {ms_kEllipsis = kEllipsis;}

    /** Returns the marker which will be append if a text has to be cut-off because
        of insufficient space.
        @return String to be append on cut-off texts.
    */
    inline static eC_String GetEllipsisString() {return ms_kEllipsis;}

    /** Sets the hyphen string which will be used on cutting strings.
        @param kHyphen Hyphen string to use.
    */
    inline static void SetHyphenString(const eC_String& kHyphen) {ms_kHyphen = kHyphen;}

    /** Returns the hyphen string which will be used on cutting strings.
        @return Currently set hyphen string.
    */
    inline static eC_String GetHyphenString() {return ms_kHyphen;}

    /** Resizes the GUIText's height so that the contained text will be fully visible
        and not truncated. The text's width will be left untouched. */
    void FitHeightToContainedText();

    /** Clones the instance of the GUIText object.
        @return Reference to the cloned GUIText object. */
    virtual CGUIText* Clone() const;

    /** Sets the line spacing factor (default line spacing factor is 1).
        Valid values are in the range of 0-100.
        @param vLineSpacing The line spacing factor.
    */
    virtual void SetLineSpacing(const eC_Value vLineSpacing);

    /** Returns the current line spacing factor.
        @return Current line spacing factor.
    */
    inline virtual eC_Value GetLineSpacing() const {return m_vLineSpacing;}

    /** Set a clip rect that different from the relative text position and text dimension.
        Setting a rectangle of width or height of zero will disable the specific clip rect and default
        to clipping against the containing object.
        @param kRect The rect (in coordinates relative to the containing object) that shall be used for clipping the text.
    */
    inline void SetSpecificClipRect(const CGUIRect& kRect){m_kSpecificClipRect = kRect;}

    /** Get the currently set specific cliprect.
        @return The rect (if set), or an invalid rect ((0,0,0,0) if no specific cliprect was set.
    */
    inline const CGUIRect& GetSpecificClipRect() const {return m_kSpecificClipRect;}

    /** Returns the number of lines in this text.
        @return Total number of lines in text. */
    inline eC_UInt GetNumberOfLines() 
    { 
        if( IsTextSingleLine()) 
            return 1;  
        AdaptText(); 
        return m_kListStringsExtended.GetQuantity(); 
    }


#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    /** Called by the framework when writing streaming files.
        When deriving a new text class in an application, re-implement this
        method.
        @param bWriteTextType Indicates whether the ID needs to be written. If
               the framework calls this method, it passes true here. The base
               class implementation has to be called with false (default).
               If called with true, the first two tags written must be this
               text's XML name tag (see GetXMLTag()) and its text type ID.
               After all attributes are written, the name tag has to be written
               again as a closing tag (preceded by a slash).
      */
    virtual void WriteToStream(const eC_Bool bWriteTextType=false);
#endif

#if defined(GUILIANI_STREAM_GUI) || defined(GUILIANI_WRITE_GUI)
    /// XML tag to be used when writing a text type ID into a stream.
    static const eC_Char XMLTAG_TEXTTYPEID[];

    /// XML tag to be used when writing a text ID into a stream.
    static const eC_Char XMLTAG_TEXTID[];

    /// XML tag to be used when writing text colors into a stream.
    static const eC_Char XMLTAG_TEXT_COLOR_PROPERTY_FLAGS[];

    static const eC_Char XMLTAG_TEXT_COLOR_STANDARD[]; ///< XML tag for standard text
    static const eC_Char XMLTAG_TEXT_COLOR_HIGHLIGHTED[]; ///< XML tag for highlighted text
    static const eC_Char XMLTAG_TEXT_COLOR_GRAYEDOUT[]; ///< XML tag for grayed out text
    static const eC_Char XMLTAG_TEXT_COLOR_PRESSED[]; ///< XML tag for pressed text

#endif
    /// Enumeration indicating if a property or a color value is used for each text color.
    enum TextColorTypeFlags_t
    {
        TEXT_COLOR_STANDARD_IS_PROPERTY = 1,
        TEXT_COLOR_HIGHLIGHTED_IS_PROPERTY = 2,
        TEXT_COLOR_GRAYED_OUT_IS_PROPERTY = 4,
        TEXT_COLOR_PRESSED_IS_PROPERTY = 8
    };

protected:
#ifdef GUILIANI_STREAM_GUI
    /// Helper function for ReadFromStream().
    void ReadFontColors(/*const eC_UInt uiVersion*/);
#endif

#ifdef GUILIANI_WRITE_GUI
    /// Helper function for WriteStream().
    void WriteFontColors();
#endif

    /// The height of one text line with this text's font.
    eC_Value m_vLineHeight;

    /// The extended string list is a double-linked list of extended strings
    typedef eC_TListDoubleLinked <ExtendedString_t> ExtendedStringList_t;

    /// List of strings to be printed, and their cached width and height
    ExtendedStringList_t m_kListStringsExtended;

    /** Adapt the text to the given width of the text field. The only adaptation
        made is replacing some trailing part of the last text line by Ellipsis
        characters. No font attribute, like font size, is changed.
        The function also removes HyphenationMarks from a single line text and
        calculates the required text width and height.
        For multi line texts this method breaks the original text into a list
        of strings at positions indicated by the user.
        It then calls CreateStringList() for further processing.
        This method only performs calculations if m_bAdaptText is set to true.
        Therefore, any method that changes attributes related to the text
        layout (including font settings, area size and so on) should set this
        flag to true instead of calling this method directly. It is called in
        as few places as possible.
    */
    virtual void AdaptText();

    /** Get the absolute vertical position at which the first line of the text
        begins. Takes the alignment into account.
        @return The vertical absolute position at which the first line of text
        should be printed.
    */
    virtual eC_Value GetVerStartAbsYPos();

    /** Get the absolute horizontal position at which the text with the given
        width should be printed, with regard to the active horizontal alignment.
        @param vWidth width of the text to be printed.
        @return The X position where kString should be printed.
    */
    virtual eC_Value GetPrintLineAbsXPos(const eC_Value vWidth);

    /** Get the absolute horizontal position at which a line of a multi-line text should
        be printed, with regard to the active horizontal alignment.
        Calls GetPrintLineAbsXPos with the cached width of the given line of text.
        @param uiLine Index of the line of text which is to be printed.
        @return The X position where kString should be printed.
    */
    virtual eC_Value GetPrintLineAbsXPos(const eC_UInt uiLine);

    /** Get the absolute horizontal position at which the internal text should be
        printed with regard to the active horizontal alignment.
        Calls GetPrintLineAbsXPos with the cached width of the internal string.
        @return The X position where kString should be printed.
    */
    virtual eC_Value GetPrintLineAbsXPos();

    /** Fits a string to the area width of this text object.
        The goal of this method is to cut as few characters as possible from
        the string while using as much of the area width of this text object as
        possible.

        This method returns not only the result (fitting ExtendedString) but also the
        string that was cut off to make it available for further processing.

        The algorithm starts by searching for a fitting word boundary (blank
        characters). Starting from the beginning of the found word, it tries
        adjusting the width character by character. It then applies hyphenation
        and adjusts the result strings.

        @param[out] rkResultStrData The result of the analysis is stored here.
        @param[in]  rkStrToAnalyse The string that is to be analysed.
        @param[out] rkRestStr Returns the string that did not fit and was cut
                    off of pkStrToAnalyse.
        @param[in,out] uiOffset The absolute offset (position) of pkStrToAnalyse in
               the original text represented by this object. Required for
               finding the correct hyphenation points. When this method returns,
               this value is updated to reflect the offset of pkRestStr for
               further processing.
    */
    virtual void FitString(ExtendedString_t& rkResultStrData,
                           const eC_String& rkStrToAnalyse,
                           eC_String& rkRestStr,
                           eC_UInt& uiOffset);

    /** Prepare to print the text.
        This function may only be used in PrintText. It prepares the graphics
        wrapper state with the correct font, color etc.
    */
    virtual void PreparePrint();

    /** Prepare the clipping rectangle for text printing.
        This will set the clipping rectangle to the intersecting rectangle of the 
        text's specific clipping rectangle with the GUI's currently set clipping rectangle.
        @return false if the resulting clipping rectangle would be incomprehensive and 
                the text must not be printed, true otherwise.
    */
    eC_Bool PrepareClipRect();

    /// Height of this text's printing area.
    eC_Value        m_vTextHeight;

    /// Width of this text's printing area.
    eC_Value        m_vTextWidth;

    /// Indicates whether this text is single- or multi-line.
    eC_Bool         m_bSingleLine;

    /** Flag to keep track of whether AdaptText() needs to perform calculations
        upon its next call. Every method that changes text attributes that can
        potentially lead to a change in the text layout should set this flag to
        true. AdaptText() resets it.
      */
    eC_Bool m_bAdaptText;

    /** This contains the text string if it is a string literal, or a locally modified copy of a string in the Localisation handler.
        For instances of CGUIText which are referencing an unmodified TextResource in the localisation handler, this is NULL.
        A TextID will for instance be stored in a local copy if it is truncated or was word-wrapped.
        @see GetText()
    */
    eC_String* m_pkText;

    /** Returns the required text width.
        @return Required text width
    */
    inline eC_Value GetRequiredTextWidth() const {return m_vRequiredTextWidth;}

    /** Sets the required text width.
        @param vRequiredTextWidth Required text width to set.
    */
    inline void SetRequiredTextWidth(eC_Value vRequiredTextWidth) {m_vRequiredTextWidth = vRequiredTextWidth;}

    /** Returns the required text height.
        @return Required text height
    */
    inline eC_Value GetRequiredTextHeight() const {return m_vRequiredTextHeight;}

    /** Sets the required text height.
        @param vRequiredTextHeight Required text height to set.
    */
    inline void SetRequiredTextHeight(eC_Value vRequiredTextHeight) {m_vRequiredTextHeight = vRequiredTextHeight;}

    /// A single space character for text size calculations.
    static const eC_String ms_ckSingleSpace;

    /// An empty string.
    static const eC_String ms_ckEmptyString;

    /// An empty Extended String
    static const ExtendedString_t ms_ckEmptyExtendedString;

    /// The line spacing factor
    eC_Value m_vLineSpacing;

    /** Set to True if this text is an ID text but m_pkText points to a local
        copy instead to the string in the localisation handler. This happens
        if the string contains hyphenation marks.
      */
    eC_Bool m_bTextIDLocalCopy;

    /** Observe the localisation handler and the property handler.
        If the text behind an ID is changed in the localisation handler
        and this text is an ID text, this function will be called.
        If a color property which is used by this text is changed, the property handler calls this function.
    */
    virtual void OnNotification();

    /** Checks a string for non-escaped hyphenation marks.
        @param kStr The string to be checked for hyphenation marks.
        @return True if kStr contains at least one hyphenation mark, otherwise
                False.
     */
    eC_Bool ContainsHyphenationMarks(const eC_String& kStr) const;

    /** Removes hyphenation marks from a string and stores their positions in
        a list.
        It goes through the string, removes every hyphenation mark and adds its
        position (adjusted with the offset) to the list.
        @param[in,out] rkStr The string to be searched for hyphenation marks.
               When this method returns, all of them are removed.
        @param[in] uiOffset Offset of rkStr in this text's string.
     */
    virtual void CollectHyphenationMarks(eC_String& rkStr, eC_UInt uiOffset);

    /// Double linked List of unsigned integers
    typedef eC_TListDoubleLinked<eC_UInt> UIntList;
    /// List of hyphen positions in the original text.
    UIntList m_kHyphens;

    /** The required width of this text's original (not wrapped) string as
        calculated by the most recent call to AdaptText().
      */
    eC_Value m_vRequiredOrgTextWidth;

    /** The required height of this text's original (not wrapped) string as
        calculated by the most recent call to AdaptText().
      */
    eC_Value m_vRequiredOrgTextHeight;

    /** Sets the internal flagindicating thwther this text has been truncated.
        @param bIsCut True means that the string was truncated, False means that it was not truncated */
    void SetTextCut( const eC_Bool bIsCut) { m_bTextCut = bIsCut; }

    /** This creates a copy of a TextID which is stored in the localisation handler, for local modifications.
        The copy is stored in m:pkText. If a local copy already exists, this will do nothing.
    */
    void CreateLocalTextCopy();

private:
    
    /** Default-constructor. Should not be used.
        Dummy declaration with no implementation just to hide the constructor.
    */
    CGUIText();

    /** Initialize a text object.
        @param pParentObject The object which contains the text.
        @param vXPos         X position of text relative to the parent.
        @param vYPos         Y position of text relative to the parent.
        @param bSingleLine   true: text is always displayed in a single line,
                             false: lines can be wrapped.
    */
    void Init(const CGUIObject* const pParentObject, const eC_Value &vXPos,
              const eC_Value &vYPos, eC_Bool bSingleLine);

    /** Creates a list of strings that are printed by this text.
        Starting from the unmodified string represented by this object, this
        method builds a list of strings that can be printed in the area defined
        for this text object. There are two main cases:
        - Single line text: This method calls FitStringWithEllipsis() to create
        the single line to be printed. The string list will contain only this
        one line.
        - Multi-line text: This method expects the internal string list to be
        pre-filled with the original text broken into separate strings at user-
        defined positions. FitString() is then used to wrap each of these lines
        to make them fit into the text area. The resulting lines are stored in
        the extended string list. The last line is also processed by
        FitStringWithEllipsis().
    */
    virtual void CreateStringList();

    /** Fits a string to the area width of this text object, appending an
        ellipsis if the string needed to be cut for fitting.

        The goal of this method is to cut as few characters as possible from
        the string while using as much of the area width of this text object as
        possible. If the string has to be cut to achieve this, an ellipsis (...)
        is appended.

        The algorithm starts by searching for a fitting word boundary (blank
        characters). Starting from the beginning of the found word, it tries
        adjusting the width character by character until the width is met.

        @param[in] kOrgStr The original string that is to be analysed.
        @param bForceEllipsis If TRUE an ellipsis will be appended in any case,
                              if FALSE it will only be appended if the string was truncated.
        @return the resulting (fitting) extended string class element.
    */
    virtual ExtendedString_t FitStringWithEllipsis(const eC_String& kOrgStr, const eC_Bool bForceEllipsis = false);

    /** Fits a string to the area width of this text object using hyphenation.
        Hyphenation information has to be gathered by calling
        CollectHyphenationMarks() before.
        @param[in] kFirstPart String that needs to be displayed unmodified,
               i.e. it definitely fits. This is not checked here. The caller has
               to make sure of that.
        @param[in] kSecondPart String that should at least partly be fitted
               into this text's area together with kFirstPart. If it does not
               fit completely, it is hyphenated. Note that this string is also
               not modified (it is not const because a non-const method needs
               to be called on it).
        @param uiOffset The offset (position) of kSecondPart in the original
               text represented by this object. Required for finding the correct
               hyphenation positions.
        @param[out] kHyphenated Fitted version of kSecondPart. If it was cut at
               a hyphenation position, a hyphen is appended. Can also be an
               empty string if no hyphenation point was found in kSecondPart or
               if nothing fitted.
        @return The length of kFirstPart + (length of kHyphenated without
               hyphen).
     */
    eC_UInt FitStringWithHyphen(const eC_String& kFirstPart,
                                eC_String& kSecondPart,
                                eC_UInt uiOffset,
                                eC_String& kHyphenated) const;

    /// Property used for the text color for state "standard". By default, this value is DUMMY_PROPERTY, i.e. a fixed color is used.
    GlobalProperty_t m_eTextColorPropertyStandard;
    /// Text color for state "standard". If a property is used, the member holds the current value of the property.
    eC_UInt m_uiTextColorStandard;

     /// Property used for the text color for state "highlighted". By default, this value is DUMMY_PROPERTY, i.e. a fixed color is used.
    GlobalProperty_t m_eTextColorPropertyHighlighted;
    /// Text color for state "highlighted".
    eC_UInt m_uiTextColorHighlighted;

     /// Property used for the text color for state "grayed out". By default this value is DUMMY_PROPERTY, i.e. a fixed color is used.
    GlobalProperty_t m_eTextColorPropertyGrayedOut;
    /// Text color for state "grayed out" or disabled respectively.
    eC_UInt m_uiTextColorGrayedOut;

    /// Property used for the text color for state "pressed". By default, this value is DUMMY_PROPERTY, i.e. a fixed color is used.
    GlobalProperty_t m_eTextColorPropertyPressed;
    /// Text color for state "pressed".
    eC_UInt m_uiTextColorPressed;

    /// Font ID for this text object.
    FontResource_t  m_eFont;
    /// Font spacing for the text object.
    eC_Float        m_fFontSpacing;
    /// Horizontal alignment.
    HorAligned_t    m_eHorAligned;
    /// Vertical alignment.
    VerAligned_t   m_eVerAligned;

    /// The object containing (and printing) this text.
    const CGUIObject *m_pParentObject;

    /// X position of this text's printing area relative to the parent object.
    eC_Value        m_vRelXPos;

    /// Y position of this text's printing area relative to the parent object.
    eC_Value        m_vRelYPos;

    /// Indicates whether the text is cut and has an appended ellipsis.
    eC_Bool         m_bTextCut;

    /// Printing adjustment for m_vRelXPos.
    eC_Value        m_vXAdjustment;

    /// Printing adjustment for m_vRelYPos.
    eC_Value        m_vYAdjustment;

    /** The ID of the text. If this text is constructed with an ID or changed
        later (by calling SetTextID()) the ID is stored here. If this text is
        constructed with a string or changed later by calling SetTextString()
        this ID is set to TEXT_START_TAG, indicating that it is invalid.
     */
    TextResource_t m_eTextID;

    /// The character sequence denoting a soft hyphenation mark.
    static const eC_String ms_ckHyphenationMark;

    /// The length of the hyphenation mark sequence.
    static const eC_UInt ms_cuiHyphMarkLength;

    /** The string that is appended to texts that are cut-off at a hyphenation
        point.
    */
    static eC_String ms_kHyphen;

    /// The string that is appended to cut-off texts.
    static eC_String ms_kEllipsis;

    /** The required width of this text as calculated by the most recent call
        to AdaptText().
      */
    eC_Value m_vRequiredTextWidth;

    /** The required height of this text as calculated by the most recent call
        to AdaptText().
      */
    eC_Value m_vRequiredTextHeight;

    /** The rectangle against which the text is clipped.
        By default, this is defined by the text position and its dimensions.
        It can be overidden by calling SetSpecificCliprect().
        When printing the text, the clipping rectangle will be set to the intersected rectangle of 
        m_kSpecificClipRect and the GUI's currently active clipping rectangle.
    */
    CGUIRect m_kSpecificClipRect;

#ifdef GUILIANI_WRITE_GUI
    /** Restores the hyphenation marks.
        Uses the current string in m_pkText which should have been cleaned of
        hyphenation marks by CollectHyphenationMarks().
        @param[out] kResult The string with restored marks.
      */
    void RestoreHyphenationMarks(eC_String& kResult) const;
#endif
};

#endif
