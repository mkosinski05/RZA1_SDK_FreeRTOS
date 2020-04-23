/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifdef GUILIANI_STREAM_GUI

#ifndef GUI_STREAMREADER_XML__H
#define GUI_STREAMREADER_XML__H

#include "GUIStreamReader.h"

/// Reads streaming files written by CGUIStreamWriterXML.

/** This implementation has the ability to compare each tag that was read with
    the expected tag. If GUILIANI_STREAM_CMP_TAGS is defined, each of the read
    method performs this check and throws a CGUIStreamingException if the tags
    do not match.

    The ReadString() implementation unescapes XML predefined entities. See
    StringOperations::UnescapeXML() for details.

    @ingroup GUILIANI_STREAMING
*/
class CGUIStreamReaderXML :
    public CGUIStreamReader
{
public:
    CGUIStreamReaderXML(void);

    virtual const eC_String& GetFileSuffix() const
    {
        static const eC_String ckSuffix(".xml");
        return ckSuffix;
    }

    eC_Int ReadInt(const eC_Char* pcTag = NULL);
    eC_Int ReadHex(const eC_Char* pcTag = NULL);
    eC_Float ReadFloat(const eC_Char* pcTag = NULL);
    eC_String ReadString(const eC_Char* pcTag = NULL);
    eC_Short ReadShort(const eC_Char* pcTag = NULL);
    eC_Bool ReadBool(const eC_Char* pcTag = NULL);
    void ReadBinary(eC_TArray<eC_UByte>& aData, const eC_Char* pcTag = NULL);
    void DeleteCommentTag(const eC_Char* pcTag = NULL);
    void DeleteCommentTag(const eC_String &kString);

    virtual CGUIStreamReader* Clone() const;

    virtual eC_Int ReadStreamingFileHeader()
    {
        return ReadInt("FileVersion");
    }

private:
    /** This prototype is declared only to prevent auto-generation by the compiler.
        There is no implementation for this method. */
    CGUIStreamReaderXML(const CGUIStreamReaderXML& kSource);

    /** This prototype is declared only to prevent auto-generation by the compiler.
        There is no implementation for this method. */
    CGUIStreamReaderXML& operator=(const CGUIStreamReaderXML& kSource);

    /** Parses for leading and trailing XML tags and writes the enclosed
        value string into m_aReadBuffer.
        @param pcTag The expected tag. Is checked if GUILIANI_STREAM_CMP_TAGS
               is defined.
        @return True if node was successfully parsed, otherwise False.
        @throw CGUIStreamingException if a wrong tag was found.
      */
    eC_Bool ParseNode(const eC_Char* pcTag);

    /** Helper that reads the next character in the streaming file and
        increments the line counter if it is a newline character.
        @return The character read from the streaming file.
      */
    inline eC_Char ReadChar();
};

#endif
#endif
