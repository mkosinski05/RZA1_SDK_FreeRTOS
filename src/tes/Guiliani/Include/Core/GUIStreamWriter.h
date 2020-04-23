/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifdef GUILIANI_WRITE_GUI

#ifndef GUISTREAMWRITER__H_
#define GUISTREAMWRITER__H_

#include "eC_Types.h"
#include "eC_String.h"
#include "eC_TArrayStack.h"
#include "GUICommand.h"
#include "GUIObjectHandleResource.h"
#include "GUIImageResource.h"
#include "GUITextResource.h"
#include "GUIFontResource.h"
#include "GUISoundResource.h"
#include "GUIGeneralResource.h"
#include "GUIDataPoolResource.h"
#include "GUIStreamTypes.h"

/// Retrieve pointer to GUIStreamWriter instance
#define GETOUTPUTSTREAM CGUIStreamWriter::GetInstance()

class eC_File;
class CGUIObject;
class CGUIProperties;
class CGUIResourceManager;
class CGUITextTemplateManager;
class CGUIBehaviourDecorator;
class CGUILayouter;
class CGUIAnimation;
class CGUIText;


/// Base class for stream writers.

/** Stream writers are used to write GUI definitions to files. This abstract
    base class defines the interfaces for handling streaming files and writing
    values to them.

    Each instance of a stream writer operates on one file at a time. A file can
    be set by calling SetStreamingFile().

    @note CGUIStreamWriter is NOT responsible for destruction of the streaming
    file, this should be done by the file creator. Call SetStreamingFile(NULL)
    after destruction of it, to avoid streaming operations on a non valid file
    pointer.

    After setting a file, the various write methods can be called. Each of them
    expects a 'tag' which can be a descriptive text string or something else.
    Implementations can use these tags in whatever way they want. They may
    choose to ignore the tags.

    @see GUILIANI_STREAMING
    @ingroup GUILIANI_STREAMING
*/
class CGUIStreamWriter
{
public:
    /// Destructor.
    virtual ~CGUIStreamWriter();

    /// Returns instance of current stream stack's top.
    static CGUIStreamWriter& GetInstance();

    /** Sets the streaming file. Always sets the streaming file mode to the set endian
        @param pkStreamingFile Subsequent write method calls will write into
               this file.
      */
    void SetStreamingFile(eC_File* pkStreamingFile);

    /** Returns the file suffix which is associated with this writer's file format. 
        This can be an empty string, if e.g. the writer works with formats which are unrelated to files. 
        @return String with the associated file suffix (e.g. ".xml")
    */
    virtual const eC_String& GetFileSuffix() const;

    /** Writes a behaviour object to the streaming file set by WriteDialogToFile().
        @param pBehaviour Pointer to the behaviour which should be written.
    */
    void WriteBehaviourToFile(CGUIBehaviourDecorator* pBehaviour) const;

    /** Writes a layouter object to the streaming file set by WriteDialogToFile().
        @param pLayouter Pointer to the layouter which should be written.
    */
    void WriteLayouterToFile(CGUILayouter* pLayouter) const;

    /** Writes a command object to the streaming file set by WriteDialogToFile().
        @param pCommand Pointer to the command which should be written.
    */
    void WriteCommandToFile(CGUICommandPtr pCommand) const;

    /** Writes an animation object to the streaming file.
        @param pAnimation Pointer to the animation which should be written.
    */
    void WriteAnimationToFile(CGUIAnimation* pAnimation) const;

    /** Writes a text object to the streaming file set by WriteDialogToFile().
        @param pText Pointer to the text which should be written.
    */
    void WriteTextToFile(CGUIText* pText) const;

    /** Writes an integer to the streaming file.
        @param iInt Value which should be written.
        @param pcTag Optional descriptive tag.
    */
    virtual void WriteInt(const eC_Int iInt, const eC_Char* const pcTag) = 0;

    /** Writes a hex value to the streaming file. Same interface as WriteInt
        but writers that produce human-readable output may use this method for
        hex output.
        @param iInt Integer value which should be written as hex.
        @param pcTag Optional descriptive tag.
    */
    virtual void WriteHex(const eC_Int iInt, const eC_Char* const pcTag) = 0;

    /** Writes a float to the set streaming file.
        @param fFloat Value which should be written.
        @param pcTag Optional descriptive tag.
    */
    virtual void WriteFloat(const eC_Float fFloat, const eC_Char* const pcTag) = 0;

    /** Writes a string to the set streaming file.
        @param pkString Value which should be written.
        @param pcTag Optional descriptive tag.
    */
    virtual void WriteString(const eC_String &pkString, const eC_Char* const pcTag)=0;

    /** Writes a Short to the streaming file.
        @param iShort Value which should be written.
        @param pcTag Optional descriptive tag.
    */
    virtual void WriteShort(const eC_Short iShort, const eC_Char* const pcTag) = 0;

    /** Writes a bool to the streaming file.
        @param bBool Value which should be written.
        @param pcTag Optional descriptive tag.
    */
    virtual void WriteBool(const eC_Bool bBool, const eC_Char* const pcTag) = 0;

    /** Write binary stream. Array will be written to stream in raw mode.
        @param aData Array containing data to post
        @param pcTag Optional descriptive tag.
     */
    virtual void WriteBinary(const eC_TArray<eC_UByte>& aData, const eC_Char* const pcTag) = 0;

    /** Writes an object handle to the streaming file. The call is directly
        forwarded to WriteInt().
        @param eID The object ID to be written.
        @param pcTag Optional descriptive tag.
      */
    virtual void WriteObjectID(ObjectHandle_t eID, const eC_Char* const pcTag) { WriteInt(eID, pcTag); }

    /** Writes an image ID to the streaming file. The call is directly
        forwarded to WriteInt().
        @param eID The image ID to be written.
        @param pcTag Optional descriptive tag.
      */
    virtual void WriteImageID(ImageResource_t eID, const eC_Char* const pcTag) { WriteInt(eID, pcTag); }

    /** Writes a text ID to the streaming file. The call is directly
        forwarded to WriteInt().
        @param eID The text ID to be written.
        @param pcTag Optional descriptive tag.
      */
    virtual void WriteTextID(TextResource_t eID, const eC_Char* const pcTag) { WriteInt(eID, pcTag); }

    /** Writes a font ID to the streaming file. The call is directly
        forwarded to WriteInt().
        @param eID The font ID to be written.
        @param pcTag Optional descriptive tag.
      */
    virtual void WriteFontID(FontResource_t eID, const eC_Char* const pcTag) { WriteInt(eID, pcTag); }

    /** Writes a sound ID to the streaming file. The call is directly
        forwarded to WriteInt().
        @param eID The sound ID to be written.
        @param pcTag Optional descriptive tag.
      */
    virtual void WriteSoundID(SoundResource_t eID, const eC_Char* const pcTag) { WriteInt(eID, pcTag); }

    /**
       Writes a general resource ID to the streaming file. The call is directly
       forwarded to WriteInt().
       @param eID The general resource ID to be written.
       @param pcTag Optional descriptive tag.
     */
    virtual void WriteGeneralResourceID(GeneralResource_t eID,
        const eC_Char* const pcTag) { WriteInt(eID, pcTag); }

    /** Writes a DataPool ID to the streaming file. The call is directly
        forwarded to WriteInt().
        @param eID The DataPool ID to be written.
        @param pcTag Optional descriptive tag.
    */
    virtual void WriteDataPoolID(DataPoolResource_t eID, const eC_Char* const pcTag) { WriteInt(eID, pcTag); }

    /** Writes a single tag without a value.
        @param pcTag The tag to be written.
    */
    virtual void WriteCommentTag(const eC_Char* const pcTag)= 0;

    /** Writes a single tag without a value.
        @param kTag The tag to be written.
      */
    virtual void WriteCommentTag(const eC_String& kTag)= 0;

    /** Writes the file header
        @param iFileVersion file version
    */
    virtual void WriteStreamingFileHeader(const eC_Int iFileVersion) = 0;

    /** Sets the endianess of the streamed file
        @param eFileEndianess the desired endianess
        @retval true operation successful
        @retval false operation not supported
    */
    virtual eC_Bool SetFileEndianess(FileEndianess_t eFileEndianess) = 0;

    /** Sets the alignment of the streamed file
        @param eFileAlignment the desired alignment
        @retval true operation successful
        @retval false operation not supported
    */
    virtual eC_Bool SetFileAlignment(FileAlignment_t eFileAlignment) = 0;

    /** Gets the endianess of the file
        @return Endianess of the file
    */
    FileEndianess_t GetFileEndianess() const 
    {
        return m_eFileEndianess;
    }

    /** Gets the type of the file
        @return type of the file
    */
    FileType_t GetFileType() const 
    {
        return m_eFileType;
    }

    /** Gets the alignment of the file
        @return alignment of the file
    */
    FileAlignment_t GetFileAlignment() const
    {
        return m_eFileAlignment;
    }

protected:
    /** Constructor.
        @param eFileType initial file type
        @param eFileEndianess initial file endianess
        @param eFileAlignment initial file alignment
    */
    CGUIStreamWriter(FileType_t eFileType, FileEndianess_t eFileEndianess, FileAlignment_t eFileAlignment);

    /// The file that the content is written into.
    eC_File* m_pkStreamingFile;

    /// File Type
    FileType_t m_eFileType;

    /// Endianess of the file
    FileEndianess_t m_eFileEndianess;
    
    /// Alignment of the file
    FileAlignment_t m_eFileAlignment;

private:
    /// StreamWriter stacked instances
    static eC_TArrayStack<CGUIStreamWriter*> ms_apStreamWriter;

    /** This prototype is declared only to prevent auto-generation by the compiler.
        There is no implementation for this method. */
    CGUIStreamWriter(const CGUIStreamWriter& kSource);

    /** This prototype is declared only to prevent auto-generation by the compiler.
        There is no implementation for this method. */
    CGUIStreamWriter& operator=(const CGUIStreamWriter& kSource);
};

#endif

#endif
