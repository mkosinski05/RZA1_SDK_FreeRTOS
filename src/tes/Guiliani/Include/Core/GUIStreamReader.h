/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifdef GUILIANI_STREAM_GUI

#ifndef GUISTREAMREADER__H_
#define GUISTREAMREADER__H_

/** This file contains the base class and global defines for the stream reader
    @file GUIStreamReader.h
*/

#include "eC_Types.h"
#include "eC_TArrayStack.h"
#include "GUIConfigDebug.h"
#include "GUIObjectHandleResource.h"
#include "GUIImageResource.h"
#include "GUITextResource.h"
#include "GUIFontResource.h"
#include "GUISoundResource.h"
#include "GUIGeneralResource.h"
#include "GUIDataPoolResource.h"
#include "GUIStreamTypes.h"
// These macros are used to remove the compare strings from the code in
// release versions
#if defined(GUILIANI_STREAM_CMP_TAGS)
/// Helper macro that should be used instead of calling CGUIStreamReader::ReadInt().
#define READ_INT(x) ReadInt(x)
/// Helper macro that should be used instead of calling CGUIStreamReader::ReadHex().
#define READ_HEX(x) ReadHex(x)
/// Helper macro that should be used instead of calling CGUIStreamReader::ReadFloat().
#define READ_FLOAT(x) ReadFloat(x)
/// Helper macro that should be used instead of calling CGUIStreamReader::ReadString().
#define READ_STRING(x) ReadString(x)
/// Helper macro that should be used instead of calling CGUIStreamReader::ReadShort().
#define READ_SHORT(x) ReadShort(x)
/// Helper macro that should be used instead of calling CGUIStreamReader::ReadBool().
#define READ_BOOL(x) ReadBool(x)
/// Helper macro that should be used instead of calling CGUIStreamReader::ReadBinary().
#define READ_BINARY(a, x) ReadBinary(a, x)
/// Helper macro that should be used instead of calling CGUIStreamReader::DeleteCommentTag().
#define DELETE_COMMENT_TAG(x) DeleteCommentTag(x)
/// Helper macro that should be used instead of calling CGUIStreamReader::ReadObjectID().
#define READ_OBJECTID(x) ReadObjectID(x)
/// Helper macro that should be used instead of calling CGUIStreamReader::ReadImageID().
#define READ_IMAGEID(x) ReadImageID(x)
/// Helper macro that should be used instead of calling CGUIStreamReader::ReadTextID().
#define READ_TEXTID(x) ReadTextID(x)
/// Helper macro that should be used instead of calling CGUIStreamReader::ReadFontID().
#define READ_FONTID(x) ReadFontID(x)
/// Helper macro that should be used instead of calling CGUIStreamReader::ReadSoundID().
#define READ_SOUNDID(x) ReadSoundID(x)
/// Helper macro that should be used instead of calling CGUIStreamReader::ReadGeneralResourceID().
#define READ_GENRESID(x) ReadGeneralResourceID(x)
/// Helper macro that should be used instead of calling CGUIStreamReader::ReadDataPoolID().
#define READ_DATAPOOLID(x) ReadDataPoolID(x)
#else
#define READ_INT(x) ReadInt()
#define READ_HEX(x) ReadHex()
#define READ_FLOAT(x) ReadFloat()
#define READ_STRING(x) ReadString()
#define READ_SHORT(x) ReadShort()
#define READ_BOOL(x) ReadBool()
#define READ_BINARY(a, x) ReadBinary(a)
#define DELETE_COMMENT_TAG(x) DeleteCommentTag()
#define READ_OBJECTID(x) ReadObjectID()
#define READ_IMAGEID(x) ReadImageID()
#define READ_TEXTID(x) ReadTextID()
#define READ_FONTID(x) ReadFontID()
#define READ_SOUNDID(x) ReadSoundID()
#define READ_GENRESID(x) ReadGeneralResourceID()
#define READ_DATAPOOLID(x) ReadDataPoolID()
#endif

/// Retrieve pointer to GUIStreamReader instance
#define GETINPUTSTREAM        CGUIStreamReader::GetInstance()

class eC_File;
class eC_String;

/// Size of the buffer for tags
#define BUFFER_LENGTH 1024


/// Base class for stream readers.

/** Stream readers are used to read GUI definitions from files. This abstract
    base class defines the interfaces for handling streaming files and reading
    values from them.

    Each instance of a stream reader operates on one file at a time. A file can
    be set by calling SetStreamingFile().

    @note CGUIStreamReader is NOT responsible for destruction of the streaming
    file, this should be done by the file creator. Call ResetStreamingFile()
    after destruction of it, to avoid streaming operations on a non valid file
    pointer.

    After setting a file, the various read methods can be called. Each of them
    expects a 'tag' which can be a descriptive text string or something else.
    Implementations can use these tags in whatever way they want, for instance
    for error checking. They may also choose to ignore the tags. However, a
    CGUIStreamReader implementation in general is expected to be able to read
    what its counterpart CGUIStreamWriter implementation wrote. Each of the
    methods returns the value read from the file.

    The stream reader offers the method GetCurLine() which returns an internal
    line number. Implementations are expected to increase the counter in a
    meaningful way that allows for printing debug messages that point users to
    errors in their streaming files.

    For each of the Read... methods declared in this class there is an
    additional macro, for instance @ref READ_INT for the method ReadInt().
    Streaming code should use these macros instead of direct method calls.
    If the preprocessor symbol GUILIANI_STREAM_CMP_TAGS is defined, the stream
    reading methods should check the tags in the files. If the symbol
    GUILIANI_STREAM_CMP_TAGS is not defined, the expected tag strings will not
    be compiled into the reading functions and thus no comparison will be done
    which is appropriate for the release version, but this optimization only
    works if the helper macros are used.

    @see GUILIANI_STREAMING
    @ingroup GUILIANI_STREAMING
*/
class CGUIStreamReader
{
public:
    /** Destructor.

        Removes this object from stream reader stack.
      */
    virtual ~CGUIStreamReader();

    /// Returns instance of current stream stack's top.
    static CGUIStreamReader& GetInstance();

    /// Clone this StreamReader. Caller is responsible for deleting the allocated instance.
    virtual CGUIStreamReader* Clone() const = 0;

    /** Returns the file suffix which is associated with this reader's file format. 
        This can be an empty string, if e.g. the reader works with formats which are unrelated to files. 
        @return String with the associated file suffix (e.g. ".xml")
    */
    virtual const eC_String& GetFileSuffix() const;

    /** Sets the file to read from. When the file is no longer needed, call
        ResetStreamingFile(). Always sets the streaming file mode to
        eC_File::FS_BIGENDIAN.
        @param pkStreamingFile The streaming file. Subsequent read method calls
               operate on this file.
      */
    void SetStreamingFile(eC_File* pkStreamingFile);

    /** Removes the streaming file. This method must be called whenever a file
        is no longer used and is closed by another instance. This method resets
        the line number and the internal pointer, ensuring that further read
        operations do not use invalid file pointers.
      */
    void ResetStreamingFile();

    /** Reads an integer value from the current streaming file.
        @param pcTag Optional descriptive tag.
        @return The value read from the file.
      */
    virtual eC_Int ReadInt(const eC_Char* pcTag = NULL) = 0;

    /** Reads an integer value from the current streaming file.
        @param pcTag Optional descriptive tag.
        @return The value read from the file.
      */
    virtual eC_Int ReadHex(const eC_Char* pcTag = NULL) = 0;

    /** Reads a float value from the current streaming file.
        @param pcTag Optional descriptive tag.
        @return The value read from the file.
      */
    virtual eC_Float ReadFloat(const eC_Char* pcTag = NULL) = 0;

    /** Reads a string from the current streaming file.
        @param pcTag Optional descriptive tag.
        @return The string that has been read.
    */
    virtual eC_String ReadString(const eC_Char* pcTag = NULL) = 0;

    /** Reads a short value from the current streaming file.
        @param pcTag Optional descriptive tag.
        @return The value read from the file.
      */
    virtual eC_Short ReadShort(const eC_Char* pcTag = NULL) = 0;

    /** Reads a boolean value from the current streaming file.
        @param pcTag Optional descriptive tag.
        @return The value read from the file.
      */
    virtual eC_Bool ReadBool(const eC_Char* pcTag = NULL) = 0;

    /** Reads binary stream. Array will be retrieved from stream in raw mode.
     *  @param pcTag Optional descriptive tag.
     *  @param[out] aData Array the read data will be set to.
     */
    virtual void ReadBinary(eC_TArray<eC_UByte>& aData, const eC_Char* pcTag = NULL) = 0;

    /** Reads an object handle from the current streaming file. The call is
        directly forwarded to ReadInt().
        @param pcTag Optional descriptive tag.
        @return The object handle read from the file.
      */
    virtual ObjectHandle_t ReadObjectID(const eC_Char* pcTag = NULL) { return static_cast<ObjectHandle_t>(READ_INT(pcTag)); }

    /** Reads an image ID from the current streaming file. The call is
        directly forwarded to ReadInt().
        @param pcTag Optional descriptive tag.
        @return The image ID read from the file.
      */
    virtual ImageResource_t ReadImageID(const eC_Char* pcTag = NULL) { return static_cast<ImageResource_t>(READ_INT(pcTag)); }

    /** Reads a text ID from the current streaming file. The call is
        directly forwarded to ReadInt().
        @param pcTag Optional descriptive tag.
        @return The text ID read from the file.
      */
    virtual TextResource_t ReadTextID(const eC_Char* pcTag = NULL) { return static_cast<TextResource_t>(READ_INT(pcTag)); }

    /** Reads a font ID from the current streaming file. The call is
        directly forwarded to ReadInt().
        @param pcTag Optional descriptive tag.
        @return The font ID read from the file.
      */
    virtual FontResource_t ReadFontID(const eC_Char* pcTag = NULL) { return static_cast<FontResource_t>(READ_INT(pcTag)); }

    /** Reads a sound ID from the current streaming file. The call is
        directly forwarded to ReadInt().
        @param pcTag Optional descriptive tag.
        @return The sound ID read from the file.
      */
    virtual SoundResource_t ReadSoundID(const eC_Char* pcTag = NULL) { return static_cast<SoundResource_t>(READ_INT(pcTag)); }

    /**
     * Reads a general resource ID from the current streaming file. The call
     * is directly forwarded to ReadInt().
     *
     * @param pcTag Optional descriptive tag.
     * @return The general resource ID read from the file.
     */
    virtual GeneralResource_t ReadGeneralResourceID(
        const eC_Char* pcTag = NULL)
    { return static_cast<GeneralResource_t>(READ_INT(pcTag)); }

    /** Reads a DataPool ID from the current streaming file. The call is
        directly forwarded to ReadInt().
        @param pcTag Optional descriptive tag.
        @return The sound ID read from the file.
      */
    virtual DataPoolResource_t ReadDataPoolID(const eC_Char* pcTag = NULL) { return static_cast<DataPoolResource_t>(READ_INT(pcTag)); }

    /** Returns the current line in the streaming file.
        @return The line number in the current streaming file.
      */
    eC_UInt GetCurLine() const { return m_uiCurLine; }

    /** Reads over (removes) a tag from the current streaming file without
        reading a value.
        @param pcTag Optional descriptive tag.
      */
    virtual void DeleteCommentTag(const eC_Char* pcTag = NULL) = 0;

    /** Reads over (removes) a tag from the current streaming file without
        reading a value.
        @param kTag descriptive tag.
      */
    virtual void DeleteCommentTag(const eC_String &kTag) = 0;

    /** Reads the file header of a streaming file
        @return version of the file
    */
    virtual eC_Int ReadStreamingFileHeader() = 0;

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
    /** Constructs a stream reader.
        Initially, no file is set. The line number is initialized to 1. This
        reader will be pushed on top of the reader stack.
        @param eFileType initial file type
        @param eFileEndianess initial file endianess
        @param eFileAlignment initial file alignment
      */
    CGUIStreamReader(FileType_t eFileType, FileEndianess_t eFileEndianess, FileAlignment_t eFileAlignment);

    /** The local read buffer will be resized by power-of-2 or reset to
        initial size of 128 Bytes.
        @param bReset Set this flag true initializes a new, empty buffer
         with initial size. False increments the current array size and
         preserves content.
     */
    void ResizeReadBuffer (eC_Bool bReset);

    /// Pointer to the current streaming file. All reading operations will be performed on this file.
    eC_File* m_pkCurrentInputFile;

    /// Dynamic array to hold currently read tag content.
    eC_TArray<eC_Char> m_aReadBuffer;

    /// Line number in current streaming file.
    eC_UInt m_uiCurLine;

    /// File Type
    FileType_t m_eFileType;

    /// Endianess of the file
    FileEndianess_t m_eFileEndianess;

        /// Alignment of the file
    FileAlignment_t m_eFileAlignment;

private:
    /// StreamReader stacked instances
    static eC_TArrayStack<CGUIStreamReader*> ms_apStreamReader;

    /** This prototype is declared only to prevent auto-generation by the compiler.
        There is no implementation for this method. */
    CGUIStreamReader(const CGUIStreamReader& kSource);

    /** This prototype is declared only to prevent auto-generation by the compiler.
        There is no implementation for this method. */
    CGUIStreamReader& operator=(const CGUIStreamReader& kSource);
};

#endif
#endif
