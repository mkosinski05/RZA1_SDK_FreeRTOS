/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifdef GUILIANI_WRITE_GUI
#ifndef GUI_STREAMWRITER_BINARY__H
#define GUI_STREAMWRITER_BINARY__H
#include "GUIStreamWriter.h"


/// Writes binary streaming files.

/** This implementation produces no human-readable output. It ignores any tags
    passed to the write methods.

    @see GUILIANI_STREAMING
    @ingroup GUILIANI_STREAMING
*/
class CGUIStreamWriterBinary :
    public CGUIStreamWriter
{
public:
    CGUIStreamWriterBinary(void);

    virtual const eC_String& GetFileSuffix() const
    {
        static const eC_String ckSuffix(".bin");
        return ckSuffix;
    };

    void WriteInt(const eC_Int iInt, const eC_Char* const pcTag);
    void WriteHex(const eC_Int iInt, const eC_Char* const pcTag) {WriteInt(iInt, pcTag);} ;
    void WriteFloat(const eC_Float fFloat, const eC_Char* const pcTag);
    void WriteString(const eC_String &pkString, const eC_Char* const pcTag);
    void WriteShort(const eC_Short Short, const eC_Char* const pcTag);
    void WriteBool(const eC_Bool bBool, const eC_Char* const pcTag);
    void WriteBinary(const eC_TArray<eC_UByte>& aData, const eC_Char* const pcTag);
    void WriteCommentTag(const eC_String& pkTag);
    void WriteCommentTag(const eC_Char* const pcTag);

    void WriteStreamingFileHeader(const eC_Int iFileVersion);
    eC_Bool SetFileEndianess(FileEndianess_t eFileEndianess);
    eC_Bool SetFileAlignment(FileAlignment_t eFileAlignment);
private:
    /** This prototype is declared only to prevent auto-generation by the compiler.
        There is no implementation for this method. */
    CGUIStreamWriterBinary(const CGUIStreamWriterBinary& kSource);

    /** This prototype is declared only to prevent auto-generation by the compiler.
        There is no implementation for this method. */
    CGUIStreamWriterBinary& operator=(const CGUIStreamWriterBinary& kSource);

    /** Writes pad bytes to the stream file to match alignment
        @param uiBytesWritten Bytes written with last operation
    */
    void WritePadding(eC_UInt uiBytesWritten);
};

#endif
#endif
