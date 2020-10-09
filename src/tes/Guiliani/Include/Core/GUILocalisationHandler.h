/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUILOCALISATIONHANDLER__H_)
#define GUILOCALISATIONHANDLER__H_

#include "GUILocalisationTable.h"
#include "eC_String.h"
#include "GUISubject.h"
#include "GUITextResource.h"

/// Retrieve pointer to CGUILocalisationHandler instance
#define GETLOCALEHDL CGUILocalisationHandler::GetInstance()


/// This class is used to store international texts.
/** The localisation handler maps abstract text resource identifiers (of type TextResource_t) to strings of the currently active language.
    (Please refer to chapter @ref page_internationalization for further details on how internationalization is handled within Guiliani.)

    Switching between two languages is therefore only a matter of loading another language file. Language files are generally UTF-8 encoded text files
    in which each line of text corresponds to one TextTesource ID. Note that strings within the language file must be in the same order as their
    corresponding TextResource IDs (which are typically stored in UserTextResource.h)

    Note that the localisation handler is an observable subject, and all CGUIText objects within the user interface will be automatically notified via the subject/observer pattern
    when a TextResource gets updated by a call to SetText().

    Example code for loading a language file:
@code
    GETLOCALEHDL.LoadLocalisationFile("English.lng");
@endcode

    Example code for retrieving the string which is associated with a TextResource ID (defined in UserTextResource.h) within the given language:
@code
    const eC_String* pString = GETLOCALEHDL.GetText(TXT_HELLO);
@endcode
*/
class CGUILocalisationHandler:
    public CGUISubject
{
    friend class CGUI;

public:
    /// return pointer to language dependent texts
    static inline CGUILocalisationHandler& GetInstance(){ return ms_kLocalisationHandler; }

    /// Should only be called by the destructor. As a workaround for memory leak
    /// detection ~CGUI() may call it too, but it is forbidden to be called by
    /// other code.
    void DeInit();

    /**
    Loads international text strings from a file to a table.
    In case of error, the state of the internally stored Textstrings is undefined.
    @param kLangFileName File to load texts from.
    */
    void LoadLocalisationFile(const eC_String &kLangFileName)
    {
        m_pkLocalisationTable->LoadLocalisationFile(kLangFileName);
        NotifyObservers();
    }

    /**
    Retrieves a text from the internal text table.
    @param eTextResourcesIdx Index of text to retrieve.
    @return Constant pointer to the corresponding text string. If eTextResourcesIdx
            is DUMMY_TEXT or points to an uninitialized text, a pointer to an empty
            string is returned.
    */
    const eC_String *GetText(const TextResource_t &eTextResourcesIdx) const
    {
        const eC_String *pkText = m_pkLocalisationTable->GetText(eTextResourcesIdx);
        return (pkText) ? pkText : m_pkLocalisationTable->GetDummyText();
    }

    /** Sets (changes) the text for a specific ID.
        @param eID The ID of the text to be changed.
        @param rkText The text to be used for the ID.
        @param bNotifyObservers If true notification of all obeservers will be done.
      */
    void SetText(TextResource_t eID, const eC_String& rkText, eC_Bool bNotifyObservers = true)
    {
        m_pkLocalisationTable->SetText(eID, rkText);

        if (bNotifyObservers)
        {
            NotifyObservers();
        }
    }

    /**
    Adds a new Text and returns the associated dynamically created TextResource ID.
    @param kNewText String for which a new TextResource ID shall be created
    @return New TextResource ID.
    */
    TextResource_t AddDynamicText(const eC_String &kNewText)
    {
        return m_pkLocalisationTable->AddDynamicText(kNewText);
    }

    /**
    Frees the given TextResource ID.
    @param eID TextID which shall be freed
    */
    void FreeDynamicText(const TextResource_t eID)
    {
        return m_pkLocalisationTable->FreeDynamicText(eID);
    }

    /** Sets the total number of texts.
        @param uiNOFLocalisationTexts Number of texts
        @return True if internal storage has been successfully adapted to support the requested
        number of texts. False otherwise.
    */
    eC_Bool SetNOFLocalisationTexts( eC_UInt uiNOFLocalisationTexts )
    {
        return m_pkLocalisationTable->SetNOFLocalisationTexts(uiNOFLocalisationTexts);
    }

    /// Returns the total number of localisation texts
    eC_UInt GetNOFLocalisationTexts() const
    {
        return m_pkLocalisationTable->GetNOFLocalisationTexts();
    }

    /** Checks if TextResource ID is valid.
    @param eID TextResourceID to check.
    @return True if valid else False.
    */
    inline eC_Bool IsValidID(const TextResource_t &eID) const
    {
        return m_pkLocalisationTable->IsValidID(eID);
    }

    /**
    Writes all registered TextResources of the GUI to File.
    @param kPath Path and name of the destination file.
    @return Always returns true
    @throw CGUIException if destinationfile could not be opened.
    */
    eC_Bool WriteLocalisationFile(const eC_String& kPath) const
    {
        return m_pkLocalisationTable->WriteLocalisationFile(kPath);
    }

#ifndef eC_TARGET_ENV_POCKET
private:
#else
public: // workaround for the broken eVC4 compiler: it doesn't find a private destructor
#endif
    /**
    Destructor.
    */
    ~CGUILocalisationHandler(void);

private:
    /**
    Constructor.
    */
    CGUILocalisationHandler();

    /// Should only be called by the constructor
    void Init();

    /** This prototype is declared only to prevent auto-generation by the compiler.
        There is no implementation for this method. */
    CGUILocalisationHandler(const CGUILocalisationHandler& kSource);

    /** This prototype is declared only to prevent auto-generation by the compiler.
        There is no implementation for this method. */
    CGUILocalisationHandler& operator=(const CGUILocalisationHandler& kSource);

    /// singleton instance
    static CGUILocalisationHandler ms_kLocalisationHandler;

    /** Current localisation table. */
    CGUILocalisationTable *m_pkLocalisationTable;
};

#endif
