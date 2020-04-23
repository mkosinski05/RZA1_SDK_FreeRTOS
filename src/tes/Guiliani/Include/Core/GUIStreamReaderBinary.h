/*
* Copyright (C) TES Electronic Solutions GmbH,
* All Rights Reserved.
* Contact: info@guiliani.de
*
* This file is part of the Guiliani HMI framework
* for the development of graphical user interfaces on embedded systems.
*/

#ifdef GUILIANI_STREAM_GUI
#ifndef GUI_STREAMREADER_BINARY__H
#define GUI_STREAMREADER_BINARY__H

#include "GUIStreamReader.h"


/// Reads streaming files written by CGUIStreamWriterBinary.

/** Since CGUIStreamWriterBinary does not write the descriptive tags, each of
    the read methods in this implementation ignores the tag.

    @ingroup GUILIANI_STREAMING
*/
class CGUIStreamReaderBinary :
    public CGUIStreamReader
{
public:
    /// Constructor.
    CGUIStreamReaderBinary();
    

    virtual const eC_String& GetFileSuffix() const
    {
        static const eC_String ckSuffix(".bin");
        return ckSuffix;
    };

    eC_Int ReadInt(const eC_Char* pcTag = NULL);
    eC_Int ReadHex(const eC_Char* pcTag = NULL) {return ReadInt(pcTag); }
    eC_Float ReadFloat(const eC_Char* pcTag = NULL);
    eC_String ReadString(const eC_Char* pcTag = NULL);
    eC_Short ReadShort(const eC_Char* pcTag = NULL);
    eC_Bool ReadBool(const eC_Char* pcTag = NULL);
    void ReadBinary(eC_TArray<eC_UByte>& aData, const eC_Char* pcTag = NULL);
    void DeleteCommentTag(const eC_Char* pcTag = NULL);
    void DeleteCommentTag(const eC_String &kTag);
    virtual CGUIStreamReader* Clone() const;

    /** Reads the file header and returns the file version.
        @return file version
    */
    eC_Int ReadStreamingFileHeader();
    

protected:
    /** This prototype is declared only to prevent auto-generation by the compiler.
        There is no implementation for this method.
        @param kSource Unused
    */
    CGUIStreamReaderBinary(const CGUIStreamReaderBinary& kSource);

    /** This prototype is declared only to prevent auto-generation by the compiler.
        There is no implementation for this method.
        @param kSource Unused
        @return Unused
    */
    CGUIStreamReaderBinary& operator=(const CGUIStreamReaderBinary& kSource);
    
    /** Sets the endianess of the streamed file.
        @param eFileEndianess the desired endianess
        @retval true operation successful
        @retval false operation not supported
    */
    eC_Bool SetFileEndianess(FileEndianess_t eFileEndianess);
   
    /** Sets the alignment of the streamed file.
        @param eFileAlignment the desired alignment
        @retval true operation successful
        @retval false operation not supported
    */
    eC_Bool SetFileAlignment(FileAlignment_t eFileAlignment);
   
    /** Reads and discards pad bytes out of the stream file to match alignment.
        @param uiBytesRead Bytes read with last operation
    */
    void ReadPadding(eC_UInt uiBytesRead);
};

#endif
#endif
