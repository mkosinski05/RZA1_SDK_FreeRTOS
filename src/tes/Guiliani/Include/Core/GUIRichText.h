/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifndef GUIRICHTEXT__H_
#define GUIRICHTEXT__H_

#include "GUIFontResource.h"
#include "GUITextResource.h"
#include "eC_TList_doubleLinked.h"
#include "GUIText.h"
#include "FntWrap.h"

class CGUIObject;
// @guiliani_doxygen toplevel_module Rich Text
/**
    <table border="0">
    <tr>
        <td width="200">@image html rich_text.png</td>
            <td>
                The "rich text" offers a set of rich-text tags to format the displayed information. With the option to add underline, italic or bold attributes, very appealing formatting can be created.
                To ensure a fast font rendering, the supported list of rich-text tags is limited to the tags mentioned above (Class: CGUIRichText).
            </td>
        </tr>
    </table>
*/
// @endguiliani_doxygen

/** @defgroup GUILIANI_TEXT Text
@brief Group of classes within Guiliani which are related handling or displaying of text

Text is one of the most used features within an HMI.

Guiliani supports several controls specialized on text out- and input. 
The most important features are:
    - internationalization
    - word wrap
    - truncation
    - scrolling
    - input and cursor handling
    - support of fonts and font styles
    - multi line input and output
    - rich text

Examples for text (including example code) can be found here:

CGUIText

CGUIRichText

CGUITextField

CGUIMultiLineEdit
*/

/**
    Text implementation which supports rich text (bold, italic, underlined, color...) via tags embedded in the supplied text string.
    It is recommended to only use rich text where necessary, since it requires significantly more performance that Guiliani's plain standard texts.
    Prior to using the rich text object you need to supply it with one font each for "bold", "italic" and "bold-italic". Otherwise, the text's default
    font identifier will be used instead.

    Format tags must be embedded in the following syntax (equal to normal HTML syntax):

    <[FORMAT_TAG1]><[FORMAT_TAG2]>[STRING TO OUTPUT]</[FORMAT_TAG1]></[FORMAT_TAG2]>

    The following format tags are supported:

    \"\<colorFFFFFF55\>some string\</colorFFFFFF55\>\" Sets the color used for printing the given string. Color format is AARRGGBB.

    \"\<b\>some string\</b\>\" Prints the given string using the bold-font.

    \"\<i\>some string\</i\>\" Prints the given string using the italic-font.

    \"\<u\>some string\</u\>\" Prints the given string using the underlined-font.

    \"\<nobr\>some string\</nobr\>\" Avoids line breaking the string.

    Example:
@code
    // Create a textfield
    CGUIBaseTextField* pTextField = new CGUIBaseTextField( this, 10, 300, 250, 40, "", false);

    // A demo string including rich text formatting
    eC_String kRichText("This is a <colorFFFFFF55>text</colorFFFFFF55> which shall Wg demonstrate the use of <b>rich Wg text</b> features within <i><colorFFFF0000>Guiliani</colorFFFF0000></i>.");

    // Create richtext object
    CGUIRichText* pRichText = new CGUIRichText( this, kRichText);

    // Supply fonts for each format tag
    FontResource_t FontNormal = GETRESMANAGER.RegisterDynamicFont("DejaVuSerifCondensed.ttf", 15, CFntWrap::FNT_NORMAL, true);
    GETRESMANAGER.RequestFontResource( FontNormal);
    FontResource_t FontBold = GETRESMANAGER.RegisterDynamicFont("DejaVuSerifCondensed-Bold.ttf", 17, CFntWrap::FNT_BOLD, true);
    GETRESMANAGER.RequestFontResource( FontBold);
    FontResource_t FontItalic = GETRESMANAGER.RegisterDynamicFont("DejaVuSerifCondensed-Italic.ttf", 15, CFntWrap::FNT_ITALIC, true);
    GETRESMANAGER.RequestFontResource( FontItalic);
    FontResource_t FontBoldItalic = GETRESMANAGER.RegisterDynamicFont("DejaVuSerifCondensed-BoldItalic.ttf", 15, static_cast<CFntWrap::GUIFont_t>( CFntWrap::FNT_BOLD | CFntWrap::FNT_ITALIC), true);
    GETRESMANAGER.RequestFontResource( FontBoldItalic);
    pRichText->SetRichFonts( FontNormal, FontBold, FontItalic, FontBoldItalic);

    // Associate richtext with text field
    pTextField->SetText( pRichText);
    pRichText->SetTextWidth( pTextField->GetWidth() );
    pRichText->SetTextHeight( pTextField->GetHeight() );
    pRichText->SetAligned( CGUIText::V_CENTERED, CGUIText::H_CENTERED);
    pRichText->SetTextSingleLine( false);
@endcode
    @brief Text implementation which supports rich text
    @ingroup GUILIANI_TEXT
*/
class CGUIRichText : public CGUIText
{
public:
    /** Constructs a string rich text.
        By default, the text width and height are set to the width and height
        of the parent object minus the relative positions.
        @param pParentObject Pointer to object containing the text.
        @param kText         Reference of text string.
        @param vXPos         X position of text relative to the parent.
        @param vYPos         Y position of text relative to the parent.
        @param bSingleLine   true: text is always displayed in a single line,
                             false: lines can be wrapped.
    */
    CGUIRichText(
        const CGUIObject* const pParentObject,
        const eC_String &kText,
        const eC_Value &vXPos=eC_FromInt(0),
        const eC_Value &vYPos=eC_FromInt(0),
        const eC_Bool &bSingleLine = true);

    /** Constructs a string rich text.
        By default, the text width and height are set to the width and height
        of the parent object minus the relative positions.
        @param pParentObject Pointer to object containing the text.
        @param pkText        Pointer of text string.
        @param vXPos         X position of text relative to the parent.
        @param vYPos         Y position of text relative to the parent.
        @param bSingleLine   true: text is always displayed in a single line,
                             false: lines can be wrapped.
    */
    CGUIRichText(
        const CGUIObject* const pParentObject,
        const eC_String *const pkText,
        const eC_Value &vXPos=eC_FromInt(0),
        const eC_Value &vYPos=eC_FromInt(0),
        const eC_Bool &bSingleLine = true);

    /** Constructs an ID rich text.
        By default, the text width and height are set to the width and height
        of the parent object minus the relative positions.
        @param pParentObject Pointer to object containing the text.
        @param eTextID       ID of the internationalised text.
        @param vXPos         X position of text relative to the parent.
        @param vYPos         Y position of text relative to the parent.
        @param bSingleLine   true: text is always displayed in a single line,
                             false: lines can be wrapped.
    */
    CGUIRichText(
        const CGUIObject* const pParentObject,
        const TextResource_t &eTextID,
        const eC_Value &vXPos=eC_FromInt(0),
        const eC_Value &vYPos=eC_FromInt(0),
        const eC_Bool &bSingleLine = true);

    /** Copy constructor.
        @param kTextToBeCopied The rich text object to be copied.
    */
    CGUIRichText(const CGUIRichText& kTextToBeCopied);

    /** Destructor */
    virtual ~CGUIRichText();

    /** Sets the font resources which will be used for rich text formatting
        @param eFontNormal Normal font
        @param eFontBold Bold font
        @param eFontItalic Italic font
        @param eFontBoldItalic Bold italic font
    */
    virtual void SetRichFonts( const FontResource_t& eFontNormal = DUMMY_FONT,
                               const FontResource_t& eFontBold = DUMMY_FONT,
                               const FontResource_t& eFontItalic = DUMMY_FONT,
                               const FontResource_t& eFontBoldItalic = DUMMY_FONT);

    /** Sets all fonts (Normal, Bold, Italic, BoldItalic) to the given FontID
        @param eFontID ID for all internal fonts */
    virtual void SetFont(const FontResource_t &eFontID)
    {
        SetRichFonts( eFontID, eFontID, eFontID, eFontID);
    }

    /** Copies the base attributes from a source object.
        This rich text object will have the same containing object as the source.
        If you want to set another containing object, you should use
        SetContainingObject() to set it.
        @param kSource The source object whose attributes are copied.
        @return Where to assign to
    */
    CGUIRichText& operator=(const CGUIRichText& kSource);

    virtual void PrintText();
    virtual void SetTextID(const TextResource_t &eTextID);
    virtual void SetTextString(const eC_String& kTextString);
    virtual void SetTextColor(const eC_UInt &uiTextColorStandard, const eC_UInt &uiTextColorHighlighted, const eC_UInt &uiTextColorGrayedOut, const eC_UInt &uiTextColorPressed);
    virtual void GetRequiredOrgTextSpace(eC_Value& vTextWidth, eC_Value& vTextHeight);

    /** Sets the indicator string for the bold tag.
        @param kTag String to set.
    */
    inline void SetFormatTagBold(const eC_String& kTag) {m_kTagBold = kTag; ParseTextToRich();}

    /** Sets the indicator string for the italic tag.
        @param kTag String to set.
    */
    inline void SetFormatTagItalic(const eC_String& kTag) {m_kTagItalic = kTag; ParseTextToRich();}

    /** Sets the indicator string for the underlined tag.
        @param kTag String to set.
    */
    inline void SetFormatTagUnderlined(const eC_String& kTag) {m_kTagUnderlined = kTag; ParseTextToRich();}

    /** Sets the indicator string for the no line break tag.
        @param kTag String to set.
    */
    inline void SetFormatTagNoBreak(const eC_String& kTag) {m_kTagNoBreak = kTag; ParseTextToRich();}

    /** Sets the indicator string for the forced line break tag.
        @param kTag String to set.
    */
    inline void SetFormatTagLineBreak(const eC_String& kTag) {m_kTagLineBreak = kTag; ParseTextToRich();}

    /** Sets the indicator string for the unordered list tag.
        @param kTag String to set.
    */
    inline void SetFormatTagUnorderedList(const eC_String& kTag) {m_kTagUnorderedList = kTag; ParseTextToRich();}

    /** Sets the indicator string for the ordered list tag.
        @param kTag String to set.
    */
    inline void SetFormatTagOrderedList(const eC_String& kTag) {m_kTagOrderedList = kTag; ParseTextToRich();}

    /** Sets the indicator string for the list item tag.
        @param kTag String to set.
    */
    inline void SetFormatTagListItem(const eC_String& kTag) {m_kTagListItem = kTag; ParseTextToRich();}

    /** Sets the indicator string for the color tag.
        @param kTag String to set.
    */
    inline void SetFormatTagColor(const eC_String& kTag) {m_kTagColor = kTag; ParseTextToRich();}

    /** Sets the string for tag opening sign (e.g. "<")
        @param kTag String to set.
    */
    inline void SetFormatTagOpening(const eC_String& kTag) {m_kTagOpening = kTag; ParseTextToRich();}

    /** Sets the string for tag closing sign (e.g. ">")
        @param kTag String to set.
    */
    inline void SetFormatTagClosing(const eC_String& kTag) {m_kTagClosing = kTag; ParseTextToRich();}

    /** Sets the string for tag finished sign (e.g. "/")
        @param kTag String to set.
    */
    inline void SetFormatTagFinished(const eC_String& kTag) {m_kTagFinished = kTag; ParseTextToRich();}

    /** Returns the indicator string for the bold tag.
        @return Indicator string.
    */
    inline const eC_String& GetFormatTagBold() const {return m_kTagBold;}

    /** Returns the indicator string for the italic tag.
        @return Indicator string.
    */
    inline const eC_String& GetFormatTagItalic() const {return m_kTagItalic;}

    /** Returns the indicator string for the underlined tag.
        @return Indicator string.
    */
    inline const eC_String& GetFormatTagUnderlined() const {return m_kTagUnderlined;}

    /** Returns the indicator string for the no line break tag.
        @return Indicator string.
    */
    inline const eC_String& GetFormatTagNoBreak() const {return m_kTagNoBreak;}

    /** Returns the indicator string for the force line break tag.
        @return Indicator string.
    */
    inline const eC_String& GetFormatTagLineBreak() const {return m_kTagLineBreak;}

    /** Returns the indicator string for the unordered list tag.
        @return Indicator string.
    */
    inline const eC_String& GetFormatTagUnorderedList() const {return m_kTagUnorderedList;}

    /** Returns the indicator string for the ordered list tag.
        @return Indicator string.
    */
    inline const eC_String& GetFormatTagOrderedList() const {return m_kTagOrderedList;}

    /** Returns the indicator string for the list item tag.
        @return Indicator string.
    */
    inline const eC_String& GetFormatTagListItem() const {return m_kTagListItem;}

    /** Returns the indicator string for the color tag.
        @return Indicator string.
    */
    inline const eC_String& GetFormatTagColor() const {return m_kTagColor;}

    /** Returns the indicator string for the tag opening sign.
        @return Indicator string.
    */
    inline const eC_String& GetFormatTagOpening() const {return m_kTagOpening;}

    /** Returns the indicator string for the tag closing sign.
        @return Indicator string.
    */
    inline const eC_String& GetFormatTagClosing() const {return m_kTagClosing;}

    /** Returns the indicator string for the tag finished sign.
        @return Indicator string.
    */
    inline const eC_String& GetFormatTagFinished() const {return m_kTagFinished;}

    /** Enables the preformatting of the text by removing useless spaces, truncating
        leading and ending spaces and removing the line breaks.
        @param bPreformatText true to enable preformatting.
    */
    void EnablePreformatting(const eC_Bool& bPreformatText) {m_bPreformatText = bPreformatText;}

    /** Indicates if preformatting is enabled or not.
        @return True if preformatting of the text is enabled
    */
    eC_Bool IsTextPreformatted() {return m_bPreformatText;}

    /** Clones the instance of the GUIRichText object.
        @return Pointer to the cloned GUIRichText object. */
    virtual CGUIRichText* Clone() const;

protected:
    /// List type
    enum ListType_t
    {
        LT_ORDERED,
        LT_UNORDERED,
        LT_NONE
    };

    /** Parses the text for rich text tags and formats it accordingly.
        Please refer to the class documentation for a description of the supported rich text tags.
    */
    void ParseTextToRich();

    /** Removes line breaks, trims blanks at the beginning and end and combines repeated blanks. */
    void PreformatText();

    /** Rich texts consist of a number of text fragments. Each fragment represents a continuous section of text
        which uses the same font, colors and does NOT wrap around several lines.
        @brief One fragment of a rich text
    */
    class CRichTextFragment
    {
    public:
        CRichTextFragment():
            m_eFontID(DUMMY_FONT),
            m_kText(""),
            m_uiColorStandard(0xFF000000),
            m_uiColorHighlighted(0xFF000000),
            m_uiColorGrayedOut(0xFF000000),
            m_uiColorPressed(0xFF000000),
            m_vWidth(0),
            m_vHeight(0),
            m_uiLine(0),
            m_eFontStyle(CFntWrap::FNT_NORMAL),
            m_bLineBreakAllowed(true),
            m_uiForcedLineBreaks(0),
            m_uiPartOfListItem(0),
            m_eListType(LT_NONE)
        {
        }

        FontResource_t      m_eFontID;              ///< Font ID which is being used for printing this fragmend
        eC_String           m_kText;                ///< Text contained by this fragment
        eC_UInt             m_uiColorStandard;      ///< Color of standard text
        eC_UInt             m_uiColorHighlighted;   ///< Color of highlighted text
        eC_UInt             m_uiColorGrayedOut;     ///< Color of grayed out text
        eC_UInt             m_uiColorPressed;       ///< Color of pressed text
        eC_Value            m_vWidth;               ///< Width and height are cached to avoid redundant calculations
        eC_Value            m_vHeight;              ///< Width and height are cached to avoid redundant calculations
        eC_UInt             m_uiLine;               ///< Line in which this fragment appears.
        CFntWrap::GUIFont_t m_eFontStyle;           ///< Font styles
        eC_Bool             m_bLineBreakAllowed;    ///< Text fragment can be split up or not.
        eC_UInt             m_uiForcedLineBreaks;   ///< Number of linebreaks before the fragment
        eC_UInt             m_uiPartOfListItem;     ///< fragment is part of a list item with id X
        ListType_t          m_eListType;            ///< fragments list type
    };

    /// A rich text fragment list is a double-linked list of rich text fragments
    typedef eC_TListDoubleLinked<CRichTextFragment> RichTextFragmentList;

    /** List of all Text-fragments within this text.
        This list will be overwritten on every AdaptText() call.*/
    RichTextFragmentList m_kRichTextFragments;
    /// Original list of all Text-fragments within this text
    RichTextFragmentList m_kOriginalRichTextFragments;
    /// Width of each line of text (required for X-Alignments)
    eC_TArray< eC_Value> m_kLineWidths;

    /// Sets the font and all accompanying attributes of a text fragment. This needs to be called prior to any RequiredSpace() calls
    void PreparePrintForFragment( const CRichTextFragment &Fragment);

    /**
        Check if the text colors are updated and parse the text again if necessary.
    */
    void OnNotification();

    void AdaptText();

    /** Truncates a Text-fragment, so that it will fit into the supplied horizontal space.
        If the fragment fits completely into the available space, nothing will be done. If it does not, it will
        be truncated using ellipsis.
        @param TextFragment Text fragment which will be adapted.
        @param vMaxWidth Available width which must not be exceeded
    */
    void FitFragmentWithEllipsis(CRichTextFragment &TextFragment, eC_Value vMaxWidth );

    /** Checks if a text-fragment fits completely into the given horizontal space. If not, the fragment will be split into two separate
        fragments, where the first one optimally uses the available space, while the second one contains the remaining text portion.
        @param FirstTextFragment Text fragment whose length will be checked, and which will be split up if necessary
        @param SecondTextFragment This will contain any remaining text of FirstTextFragment, which did not fit into the given space.
        @param vMaxWidth Available width for First text fragment. It will be split when exceeding this width.
        @param uiCharOffsetFromStart Offset in characters from beginning of the entire text
    */
    void SplitFragment(CRichTextFragment &FirstTextFragment, CRichTextFragment &SecondTextFragment, eC_Value vMaxWidth, eC_UInt uiCharOffsetFromStart );

    // Font identifiers which will be used for the different formatting options
    FontResource_t m_eFontBold; ///< font resource: bold
    FontResource_t m_eFontItalic; ///< font resource: italic
    FontResource_t m_eFontBoldItalic; ///< font resource: bold italic

    // Supported format tags
    eC_String m_kTagBold; ///< Format tag: bold
    eC_String m_kTagItalic; ///< Format tag: italic
    eC_String m_kTagUnderlined; ///< Format tag: underlined
    eC_String m_kTagColor; ///< Format tag: color
    eC_String m_kTagNoBreak; ///< Format tag: no break
    eC_String m_kTagLineBreak; ///< Format tag: line break
    eC_String m_kTagUnorderedList; ///< Format tag: unordered list
    eC_String m_kTagOrderedList; ///< Format tag: ordered list
    eC_String m_kTagListItem; ///< Format tag: list item
    eC_String m_kTagOpening; ///< Format tag: opening
    eC_String m_kTagClosing; ///< Format tag: closing
    eC_String m_kTagFinished; ///< Format tag: finished

    eC_Bool m_bPreformatText; ///< Controls whether PreformatText() is called

    eC_Value m_vSpaceWidth; ///< Width of a space to avoid calling RequiredSpace() too often.
    eC_Value m_vUnorderedListSignWidth; ///< Width of the unordered list sign.
    eC_Value m_vListIndentation; ///< Default list indentation.

    eC_String m_kUnorderedListSign; ///< The unordered list sign

#ifdef GUILIANI_STREAM_GUI
    virtual void ReadFromStream();
#endif

#ifdef GUILIANI_WRITE_GUI
    virtual void WriteToStream(const eC_Bool bWriteTextType=false);
#endif

private:
    /** Default-constructor. Should not be used.
        Dummy declaration with no implementation just to hide the constructor.
    */
    CGUIRichText();

    /** Initialize a rich text object. */
    void Init();

    /** Adds a string fragment to fragment list
        @param kFragment Fragment to add.
        @param kString String to add.
    */
    void AddTextFragment(CRichTextFragment& kFragment, const eC_String& kStringFragment);

    /** Extracts the tag from the given string and returns the rest
        @param kString String to parse.
        @param kTag Returns the tag.
    */
    void ExtractTag(eC_String& kString, eC_String& kTag);

    /** Helper function used in AdaptText() to add an empty line. Be aware that all parameter could be touch inside this function!
        @param[in,out] vRemainingHeight Will be used to check, if there's enough horizontal space or degreased if a line was successfully added.
        @param[in,out] bAllAvailableSpaceUsed Will be set true, if all space is used.
        @param[in,out] uiLine Will be increased if a line was successfully added.
        @param[in,out] vRemainingSpaceInLine Will be reseted to the text width if a line was successfully added because the added line is empty.
    */
    void AddEmptyLine(eC_Value& vRemainingHeight, eC_Bool& bAllAvailableSpaceUsed, eC_UInt& uiLine, eC_Value& vRemainingSpaceInLine);

    /** Removes the leading space (if there is one) and reduces the text width inside the fragment.
        @param[in,out] kFragment Fragment to edit.
        @return True if the space was removed.
    */
    eC_Bool RemoveLeadingSpace(CRichTextFragment& kFragment);

    /** Calculates the needed space for the list sign (ordered or unordered).
        @param eListType List type.
        @param uiPartOfListItem The number of the list inside a list block (only used for ordered lists).
    */
    eC_Value GetListSignSpace(const ListType_t eListType, const eC_UInt uiPartOfListItem);
};
#endif
