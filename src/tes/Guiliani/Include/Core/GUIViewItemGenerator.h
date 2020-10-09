/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUI_VIEWITEM_GENERATOR_H)
#define GUI_VIEWITEM_GENERATOR_H

#include "GUIViewIndex.h"

#include "eC_String.h"
#include "eC_TList_doubleLinked.h"
#include "GUIFontResource.h"
#include "GUIImageResource.h"
#include "GUIRect.h"

class CGUIObject;
class CGUIViewItem;
class CGUITableView;
class CGUITreeView;

/// Base class for ViewItemGenerator.
/**  A ViewItemGenerator is used to provide CGUIViewItem objects which are responsible for the
visual representation of the elements of a View. If necessary, the ViewItemGenerator provides special editor objects
to edit the value of a ViewItem. This is only necessary if the ViewItem does not provide a ViewObject which is an editor itself.
For example a check box can be visual representation and editor at once, while a TextField might use an input field as editor.

A ViewItem is able to provide different visual representations for
different states. Supported states are 'Normal', 'Selected' and 'Focused'.

CGUITableView uses CGUIViewItemGenerator to display the content of its Model in
different states. CGUIViewItemGenerator objects can be set for a view by column, by row or
by ModelIndex (column, row).

For example, you can set a CGUIViewItemGeneratorText for a whole column to get CGUITextField objects
as visual representation for that column
and set a CGUIViewItemGeneratorComboBox to get CGUIComboBox objects for another column.

Derive your own view item generator classes from this base, creating CGUIViewItems that
meet your needs.
*/
class CGUIViewItemGenerator
{
public:
    /// Constructor.
    CGUIViewItemGenerator();

    /// Destructor.
    virtual ~CGUIViewItemGenerator() {}

    /** Called by views of models containing this item to get a ViewItem that manages
        the ViewObjects for visual representation.

    @param pkView The View that shall be used as parent for the ViewObject that is provided by the ViewItem.
    @param kViewIndex The ViewIndex of the view item, used to obtain the data from the model.
    @return A view item representing this item in normal state.
    */
    virtual CGUIViewItem* CreateViewItem(CGUITableView* pkView, const CGUIViewIndex& kViewIndex) const = 0;

    /** Called by views of models containing this item to get a ViewItem that manages
        the ViewObjects of a CGUITreeView's tree column for visual representation.

    @param pkView The CGUITreeView that shall be used as parent for the ViewObject that is provided by the ViewItem.
    @param kViewIndex The ViewIndex of the view item, used to obtain the data from the model.
    @return A view item representing this item in normal state.
    */
    virtual CGUIViewItem* CreateTreeViewItem(CGUITreeView* pkView, const CGUIViewIndex& kViewIndex) const;

    /**
        Provides an editor object.
        @param pkObservingViewItem The ViewItem that observes the editor and needs to be informed when the
        value is changed. The ViewItem will inform the View about the new value.
        @return Pointer to the Editor Object.

    */
    virtual CGUIObject* CreateEditorObject(CGUIViewItem* pkObservingViewItem) const = 0;

 /**
        Reposition the editor object based on the target cell.
        The default implementation makes sure that the CGUIObject does not overlap the cell region.
        @param pkEditorObject Pointer to the EditorObject to reposition.
        @param rkCellRect The bounding rect of the table cell.
    */
    virtual void RepositionEditorObject(CGUIObject* pkEditorObject, const CGUIRect& rkCellRect) const;
};

/**
    @brief A CGUIViewItemGeneratorDummy that does not provide a ViewItem or EditorObject
*/
class CGUIViewItemGeneratorDummy : public CGUIViewItemGenerator
{
public:
    /// Constructor.
    CGUIViewItemGeneratorDummy() {}

    /// Destructor.
    virtual ~CGUIViewItemGeneratorDummy() {}

    virtual CGUIViewItem* CreateViewItem(CGUITableView* pkView, const CGUIViewIndex& kViewIndex) const {return NULL;}
    virtual CGUIViewItem* CreateTreeViewItem(CGUITreeView* pkView, const CGUIViewIndex& kViewIndex) const {return NULL;}
    virtual CGUIObject* CreateEditorObject(CGUIViewItem* pkObservingViewItem) const {return NULL;}
};

/** As EditorObject an InputField is generated.
    The ViewItem is added as InputFinishedObserver to the InputField to be able to
    inform the View about changes.
    The generator also provides several callbacks for the visual representation of the ViewObject.
    @brief A view item generator that provides a textual representation using CGUIBaseTextField for visualization
    */
class CGUIViewItemGeneratorText : public CGUIViewItemGenerator
{
public:
    /// Default constructor.
    CGUIViewItemGeneratorText();

    virtual CGUIViewItem* CreateViewItem(CGUITableView* pkView, const CGUIViewIndex& kViewIndex) const;

    virtual CGUIObject* CreateEditorObject(CGUIViewItem* pkObservingViewItem) const;

    /** Sets the used colors of the text.
        @param uiTextColorStandard Standard Text color in ARGB coding
        @param uiTextColorHighlighted Highlighted Text color in ARGB coding
        @param uiTextColorGrayedOut Grayed out Text color in ARGB coding
        @param uiTextColorPressed Pressed Text color in ARGB coding
    */
    void SetTextColors(const eC_UInt &uiTextColorStandard,
                       const eC_UInt &uiTextColorHighlighted,
                       const eC_UInt &uiTextColorGrayedOut,
                       const eC_UInt &uiTextColorPressed);

 
    /// @return eC_UInt Standard Text color in ARGB coding
    inline eC_UInt GetTextColorStandard() const {return m_uiTextColorStandard;}

    /// @return eC_UInt Highlighted Text color in ARGB coding
    inline eC_UInt GetTextColorHighlighted() const {return m_uiTextColorHighlighted;}

    /// @return eC_UInt Grayed out Text color in ARGB coding
    inline eC_UInt GetTextColorGrayedOut() const {return m_uiTextColorGrayedOut;}

    /// @return eC_UInt Pressed Text color in ARGB coding
    inline eC_UInt GetTextColorPressed() const {return m_uiTextColorPressed;}

    /// @return eC_UInt Selected Text color in ARGB coding
    inline eC_UInt GetTextColorSelected() const {return m_uiTextColorSelected;}

    /// @param uiBackgroundColor Background color in ARGB coding
    void SetBackgroundColor (const eC_UInt &uiBackgroundColor);

    /// @return eC_UInt Background color in ARGB coding
    inline eC_UInt GetBackgroundColor()const {return m_uiBGColorStandard; }

    /// @return eC_UInt Selected Text background color in ARGB coding
    inline eC_UInt GetBackgroundColorSelected() const {return m_uiBGColorSelected; }

    /// @param eFontID Font to be used
    void SetFont(const FontResource_t &eFontID);

    /// @return FontResource_t Current font
    inline FontResource_t GetFont() const {return m_enFontDefault;}

    /// @return FontResource_t Current selected font
    inline FontResource_t GetFontSelected() const {return m_enFontSelected;}

    /**
        Set the attributes for the "selected" state of the ViewItem.
        @param uiBackgroundColor Background color in ARGB coding
        @param uiTextColor Text color in ARGB coding
        @param eFontID Font to be used
    */
    void SetSelectionAttributes(const eC_UInt &uiBackgroundColor, const eC_UInt &uiTextColor, const FontResource_t &eFontID);

protected:

    /// Background color of selected text
    eC_UInt m_uiBGColorSelected;

    /// background color of standard text
    eC_UInt m_uiBGColorStandard;

    /// Color of selected text
    eC_UInt m_uiTextColorSelected;

    /// Color of standard text
    eC_UInt m_uiTextColorStandard;

    /// Color of highlighted text
    eC_UInt m_uiTextColorHighlighted;

    /// Color of grayed out text
    eC_UInt m_uiTextColorGrayedOut;

    /// Color of pressed text
    eC_UInt m_uiTextColorPressed;

    /// Font to use in selected mode.
    FontResource_t m_enFontSelected;

    /// Font to use in unselected mode.
    FontResource_t m_enFontDefault;

private:
    CGUIViewItemGeneratorText(const CGUIViewItemGeneratorText& kSource);
    CGUIViewItemGeneratorText& operator=(const CGUIViewItemGeneratorText& kSource);
    CGUIViewItemGeneratorText& operator=(const eC_String& refValue);
};

/** 
    The CheckBox is editable, therefore no EditorObject is provided.
    The ViewItem will observe its ViewObject to be able to inform the View about changes.
    @brief A view item generator that provides a check box visualization.
*/
class CGUIViewItemGeneratorCheckBox : public CGUIViewItemGenerator
{
public:
    /// Default constructor.
    CGUIViewItemGeneratorCheckBox() {}

    virtual CGUIViewItem* CreateViewItem(CGUITableView* pkView, const CGUIViewIndex& kViewIndex) const;

    virtual CGUIObject* CreateEditorObject(CGUIViewItem* pkObservingViewItem) const;


private:
    CGUIViewItemGeneratorCheckBox(const CGUIViewItemGeneratorCheckBox& kSource){}
    CGUIViewItemGeneratorCheckBox& operator=(const CGUIViewItemGeneratorCheckBox& kSource);
};


/**
    It will generate items of type CGUIViewItemTreeElement for cells within the TreeColumn of a CGUITreeView,
    and plain CGUIViewItemText when used within a CGUITableView (or within standard columns of a CGUITreeView).
    @brief A view item generator that provides expandable elements for use within CGUITreeViews.
 */
class CGUIViewItemGeneratorTreeElement : public CGUIViewItemGeneratorText
{
public:
    /// Default constructor.
    CGUIViewItemGeneratorTreeElement(): CGUIViewItemGeneratorText() {}

    virtual CGUIViewItem* CreateTreeViewItem(CGUITreeView* pkView, const CGUIViewIndex& kViewIndex) const;

private:
    CGUIViewItemGeneratorTreeElement(const CGUIViewItemGeneratorTreeElement& kSource);
    CGUIViewItemGeneratorTreeElement& operator=(const CGUIViewItemGeneratorTreeElement& kSource);
};


/** @brief A view item generator that provides a text visualization and a combobox for editing.
*/
class CGUIViewItemGeneratorTextComboBoxEditor : public CGUIViewItemGeneratorText
{
public:
    /// Default constructor.
    CGUIViewItemGeneratorTextComboBoxEditor() {}

    virtual CGUIObject* CreateEditorObject(CGUIViewItem* pkObservingViewItem) const;

    /**
        Add a combobox entry to the EditorObject.
        @param kString The new entry
    */
    virtual void AddComboBoxItem(const eC_String& kString);

private:

    typedef eC_TListDoubleLinked<eC_String> StringList;
    ///Holds all ComboBoxEntries that were added. The list will be added to the EditorObject if it is created.
    StringList m_kComboBoxItems;

    CGUIViewItemGeneratorTextComboBoxEditor(const CGUIViewItemGeneratorTextComboBoxEditor& kSource);
    CGUIViewItemGeneratorTextComboBoxEditor& operator=(const CGUIViewItemGeneratorTextComboBoxEditor& kSource);
};

/** @brief A view item generator that provides an image (icon) visualization.
*/
class CGUIViewItemGeneratorImage : public CGUIViewItemGenerator
{
public:
    /// Default constructor.
    CGUIViewItemGeneratorImage(void)
        :   m_ubAlpha(255),
            m_bStretchBlit(true)
    {};

    virtual CGUIViewItem* CreateViewItem(CGUITableView* pkView, const CGUIViewIndex& kViewIndex) const;
    virtual CGUIObject* CreateEditorObject(CGUIViewItem* pkObservingViewItem) const;

    /** Sets the alpha (transparency) value of the image(s)
        @param cubAlpha Value in the range of 0 (transparent) to 255 (opaque) */
    inline void SetAlpha(const eC_UByte cubAlpha) {m_ubAlpha = cubAlpha;}

    /** Accessor for the alpha (transparency) value of the image(s)
        @return eC_UByte The value in the range of 0 (transparent) to 255 (opaque) */
    inline eC_UByte GetAlpha(void) const {return m_ubAlpha;}

    /** Sets the stretch blit attribute for the image(s)
        @param cbStretchBlit Specifies whether the CGUIImage object(s) shall be automatically
               resized to match the given image's dimensions. */
    inline void SetStretchBlit(const eC_Bool cbStretchBlit) {m_bStretchBlit = cbStretchBlit;}

    /** Gets the stretch blit attribute for the image(s)
        @return eC_Bool True if the image(s) will be stretched */
    inline eC_Bool GetStretchBlit(void) const {return m_bStretchBlit;}

protected:
    eC_UByte    m_ubAlpha;  ///< transparency
    eC_Bool     m_bStretchBlit; ///< automatic stretch of the images
};

/** 
    The ViewItem is added as InputFinishedObserver to the InputField to be able to
    inform the View about changes.
    The generator also provides several callbacks for the visual representation of the ViewObject.
    @brief A view item generator that provides an input field representation using CGUIInputField for visualization.
*/
class CGUIViewItemGeneratorInputField : public CGUIViewItemGeneratorText
{
public:

    /// Default constructor.
    CGUIViewItemGeneratorInputField(void)
        :   CGUIViewItemGeneratorText(),
            m_eStandard(DUMMY_IMAGE),
            m_eHighlighted(DUMMY_IMAGE),
            m_eFocussed(DUMMY_IMAGE),
            m_eGrayedOut(DUMMY_IMAGE)
    {};

    /** Set current image IDs
        @param eStandard Standard image
        @param eHighlighted Highlighted image
        @param eFocussed Focussed image
        @param eGrayedOut Grayed-out image
    */
    void SetImages(const ImageResource_t &eStandard, const ImageResource_t &eHighlighted, const ImageResource_t &eFocussed, const ImageResource_t &eGrayedOut);

    /** Get current image IDs
        @param eStandard Standard image
        @param eHighlighted Highlighted image
        @param eFocussed Focussed image
        @param eGrayedOut Grayed-out image
    */
    void GetImages(ImageResource_t &eStandard, ImageResource_t &eHighlighted, ImageResource_t &eFocussed, ImageResource_t &eGrayedOut) const
    {
        eStandard=      m_eStandard;
        eHighlighted=   m_eHighlighted;
        eFocussed=      m_eFocussed;
        eGrayedOut=     m_eGrayedOut;
    }

    virtual CGUIViewItem* CreateViewItem(CGUITableView* pkView, const CGUIViewIndex& kViewIndex) const;
    virtual CGUIObject* CreateEditorObject(CGUIViewItem* pkObservingViewItem) const;

protected:

    ImageResource_t m_eStandard; ///< standard image
    ImageResource_t m_eHighlighted; ///< highlighted image
    ImageResource_t m_eFocussed; ///< focused image
    ImageResource_t m_eGrayedOut; ///< grayed out image
};

#endif // GUI_VIEWITEM_GENERATOR_H
