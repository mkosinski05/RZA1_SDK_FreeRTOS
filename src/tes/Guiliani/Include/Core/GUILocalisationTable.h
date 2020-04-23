/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#if !defined(GUILOCALISATIONTABLE__H_)
#define GUILOCALISATIONTABLE__H_

#include "GUITextResource.h"
#include "eC_String.h"
#include "eC_TArray.h"

/**
 * @brief This class is used to store and provide international texts.
 */
class CGUILocalisationTable
{
public:
    /**
     * Default constructor.
     */
    CGUILocalisationTable();

    /**
     * Destructor.
     */
    ~CGUILocalisationTable();

    /**
     * Loads international text strings from a file to a table.
     *
     * @param kLangFileName File to load texts from.
     * @throws CGUIException If language file could not be opened.
     */
    void LoadLocalisationFile(const eC_String &kLangFileName);

    /**
     * Writes all registered TextResources of the GUI to File.
     *
     * @param kPath Path and name of the destination file.
     * @return Always true.
     * @throw CGUIException If destination file could not be opened. Or
     *  another unknown error occurred.
     */
    eC_Bool WriteLocalisationFile(const eC_String &kPath) const;

    /**
     * Retrieves a text from the internal text table.
     *
     * @param eTextResourcesIdx Index of text to retrieve.
     * @return Constant pointer to the corresponding text string. If
     *  eTextResourcesIdx is DUMMY_TEXT, a pointer to an empty string is
     *  returned. In case of eTextResourcesIdx points to an uninitialized
     *  text, NULL is returned.
     */
    const eC_String* GetText(const TextResource_t &eTextResourcesIdx) const;

    /**
     * Sets (changes) the text for a specific ID.
     *
     * @param eID The ID of the text to be changed.
     * @param rkText The text to be used for the ID.
     * @throws CGUIException In case of an invalid text resource ID.
     */
    void SetText(TextResource_t eID, const eC_String &rkText);

    /**
     * Adds new Text and returns a new ID.
     *
     * @param kNewText The text string we want to register to localisation
     *  table.
     * @return New TextResource ID.
     * @throws CGUIException When localisation string storage was unable
     *  to hold new text string.
     */
    TextResource_t AddDynamicText(const eC_String &kNewText);

    /**
     * Frees a TextID which is no longer needed.
     *
     * @param eID ID of the text to free.
     * @throws CGUIException When the given ID is illegal
     */
    void FreeDynamicText(const TextResource_t eID);

    /**
     * Sets the total number of texts.
     *
     * @param uiNOFLocalisationTexts Number of localisation texts we want
     *  to store.
     * @return True if internal storage has been successfully adapted to
     *  support the requested number of texts. False otherwise.
     */
    eC_Bool SetNOFLocalisationTexts(eC_UInt uiNOFLocalisationTexts);

    /**
     * Returns the total number of localisation texts.
     *
     * @return Number of localisation texts.
     */
    eC_UInt GetNOFLocalisationTexts() const
    {
        return m_apLocalisationText.GetQuantity();
    }

    /**
     * Checks if TextResource ID is valid.
     *
     * @param eID TextResourceID to check.
     * @return True if valid else False.
     */
    inline eC_Bool IsValidID(const TextResource_t &eID) const
    {
        return (eID > TEXT_START_TAG && MAP_TEXTID_TO_ARRAY(eID)
            < m_apLocalisationText.GetQuantity()) ? true : false;
    }

    /**
     * Gets the dummy text
     *
     * @return The string of the dummy text
     */
    const eC_String* GetDummyText() const
    {
        return &m_kDummyTextStr;
    }

    /**
     * Get number of read text resources by last file operation.
     *
     * @return Number of text lines read.
     */
    inline eC_UInt GetNOFReadTextResources() const
    {
        return m_uiNOFReadTexts;
    }

private:
    /** Holds current localisation context. */
    eC_TArray<eC_String*> m_apLocalisationText;

    /** Frees the internal text table. Only called internally. */
    void DeleteLocalisationTable();

    /** Internal enumeration for support of different line-end styles. */
    enum LineEndType_t
    {
        LINE_END_NONE,
        LINE_END_CR,
        LINE_END_CRLF
    };

    /**
     * Handle EOL. Checks whether EOL was reached. Replace EOL by 0 in the
     * parameter passed.
     *
     * @param pucBuf Pointer to buffer.
     * @return 'End of Line' type that was reached.
     */
    LineEndType_t HandleEOL(eC_UByte *const pucBuf) const;

    /**
     * If the '\' and 'n' characters occur in the language string, it means
     * that the user wants to break the line in this position. To handle
     * this effect, the '\' and 'n' characters are deleted and a '\n'
     * character is inserted at this posiiton.
     *
     * @param kLineString The one line string that should be analysed and
     *  line ending replaced.
     */
    void HandleLineBreak(eC_String *kLineString) const;

    /**
     * Reads one line out of the buffer into a string.
     *
     * @param pcSourceBuffer Pointer to the buffer to read from.
     * @param uiOffset Offset of first byte to read in buffer.
     * @param kDestinationString String where the read line will be stored.
     * @return Amount of bytes read.
     */
    eC_UInt ReadLine(eC_UByte *pcSourceBuffer,
        eC_UInt uiBufferSize,
        eC_String &kDestinationString);

    /** String mapped to the DUMMY_TEXT ID. */
    static eC_String m_kDummyTextStr;

    /** Number of read text resources by last file operation. */
    eC_UInt m_uiNOFReadTexts;
};
#endif // GUILOCALISATIONTABLE__H_
