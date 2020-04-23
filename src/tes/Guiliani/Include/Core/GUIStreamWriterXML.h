/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifdef GUILIANI_WRITE_GUI
#ifndef GUI_STREAMWRITER_XML__H
#define GUI_STREAMWRITER_XML__H
#include "GUIStreamWriter.h"

#define BUFFER_LENGTH 1024
class eC_String;


/// Writes XML-style streaming files.

/** This implementation writes human-readable output. It puts the descriptive
tags into angle brackets ('<' and '>'). One tag is written before the value,
another one with a preceding slash '/' after the value.

Example: the call
@code
WriteInt(17, "SomeInteger");
@endcode
will result in the following line written:
@verbatim
<SomeInteger>17</SomeInteger>
@endverbatim

The WriteString() implementation escapes special characters to XML predefined
entities. See StringOperations::EscapeXML() for details.

@ingroup GUILIANI_STREAMING
*/
class CGUIStreamWriterXML :
    public CGUIStreamWriter
{
public:
    CGUIStreamWriterXML(void);

    virtual const eC_String& GetFileSuffix() const
    {
        static const eC_String ckSuffix(".xml");
        return ckSuffix;
    };

    void WriteInt(const eC_Int iInt, const eC_Char* const pcTag);
    void WriteHex(const eC_Int iInt, const eC_Char* const pcTag);
    void WriteFloat(const eC_Float fFloat, const eC_Char* const pcTag);
    void WriteString(const eC_String &rkString, const eC_Char* const pcTag);
    void WriteShort(const eC_Short Short, const eC_Char* const pcTag);
    void WriteBool(const eC_Bool bBool, const eC_Char* const pcTag);
    void WriteBinary(const eC_TArray<eC_UByte>& aData, const eC_Char* const pcTag);
    void WriteCommentTag(const eC_Char* const pcTag);
    void WriteCommentTag(const eC_String &pcTag);
    void WriteStreamingFileHeader(const eC_Int iFileVersion);
    eC_Bool SetFileEndianess(FileEndianess_t eFileEndianess);
    eC_Bool SetFileAlignment(FileAlignment_t eFileAlignment);

private:
    /** This prototype is declared only to prevent auto-generation by the compiler.
        There is no implementation for this method. */
    CGUIStreamWriterXML(const CGUIStreamWriterXML& kSource);

    /** This prototype is declared only to prevent auto-generation by the compiler.
        There is no implementation for this method. */
    CGUIStreamWriterXML& operator=(const CGUIStreamWriterXML& kSource);

    void WriteStringWithoutTermination(const eC_String &pkString);

    /** Writes a tag in angle brackets.
        @param pcTag The tag to be written.
    */
    void WriteStartTag(const eC_Char* const pcTag);

    /** Writes a tag (preceded by a slash) in angle brackets.
        @param pcTag The tag to be written.
    */
    void WriteEndTag(const eC_Char* const pcTag);

    // Buffer to write tags before writing to file.
    eC_Char m_acWriteBuffer[BUFFER_LENGTH];
};

#endif
#endif
